import subprocess
from pathlib import Path

def query_ollama(model: str, prompt: str) -> str:
    try:
        result = subprocess.run(
            ["ollama", "run", model],
            input=prompt.encode("utf-8"),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=300,
            check=True
        )
        return result.stdout.decode("utf-8")
    except subprocess.CalledProcessError as e:
        print("Ollama call failed:", e.stderr.decode())
        return ""
    except subprocess.TimeoutExpired:
        print("Ollama call timed out")
        return ""

def build_project():
    build_dir = Path("build")
    build_dir.mkdir(exist_ok=True)
    try:
        subprocess.run(["cmake", ".."], cwd=build_dir, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        subprocess.run(["make", "-j4"], cwd=build_dir, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        return True, ""
    except subprocess.CalledProcessError as e:
        return False, e.stderr.decode()

def fix_build(test_path, log, model="qwen2:0.5b"):
    prompt_template = Path("prompts/fix_build.yaml").read_text()
    prompt = f"""{prompt_template}

test_file: |
{test_path.read_text()}

build_logs: |
{log}
"""
    out_path = Path("fixed_tests") / (test_path.stem + "_fixed.cpp")  # ✅ Fix this line
    fixed_code = query_ollama(model, prompt)
    out_path.write_text(fixed_code, encoding="utf-8")

def run_coverage():
    subprocess.run(["./runTests"], cwd="build", check=True)
    subprocess.run(["lcov", "--capture", "--directory", ".", "--output-file", "coverage.info"], cwd="build", check=True)
    subprocess.run(["genhtml", "coverage.info", "--output-directory", "coverage_html"], cwd="build", check=True)

def improve_test(test_path, coverage_summary, model="qwen2:0.5b"):
    prompt_template = Path("prompts/improve_coverage.yaml").read_text()
    prompt = f"""{prompt_template}

test_file: |
{test_path.read_text()}

coverage_summary: |
{coverage_summary}
"""
    result = query_ollama(model, prompt)
    out_path = Path("final_tests") / (test_path.stem + "_final.cpp")
    out_path.write_text(result, encoding="utf-8")

def parse_coverage_summary():
    summary_path = Path("build/coverage.info")
    if summary_path.exists():
        return summary_path.read_text()
    return ""

def run_pipeline():
    test_dir = Path("tests")
    Path("build_logs").mkdir(exist_ok=True)
    Path("fixed_tests").mkdir(exist_ok=True)
    Path("final_tests").mkdir(exist_ok=True)

    print("Building project...")
    success, log = build_project()

    if not success:
        print("Build failed! Sending tests + logs to LLM for fixes")
        for test in test_dir.glob("test_*.cpp"):
            log_path = Path("build_logs") / f"{test.stem}.log"
            log_path.write_text(log)
            fix_build(test, log)
    else:
        print("Build passed. Running tests & coverage...")
        run_coverage()
        coverage = parse_coverage_summary()

        print("Sending tests + coverage data to LLM for improvements")
        for test in test_dir.glob("test_*.cpp"):
            improve_test(test, coverage)

if __name__ == "__main__":
    run_pipeline()

from pathlib import Path
from llm_interface import query_ollama

SRC_DIR = Path("cpp_projects")
TEST_DIR = Path("tests")
PROMPT_PATH = Path("prompts/gen_tests.yaml")
MODEL = "qwen2:0.5b"

def build_prompt(code: str, yaml_prompt: str) -> str:
    return f"""
# YAML INSTRUCTION:
{yaml_prompt}

# C++ FILE:
{code}

# TASK:
Write a complete Google Test `.cpp` test file for the above code.
Include all required headers and use TEST() macros.
DO NOT include any explanations or markdown.
ONLY output a valid C++ file.
"""

def generate_tests():
    controller_files = (SRC_DIR / "plugins").glob("*.cc")
    filter_files = (SRC_DIR / "models").glob("*.cc")
    
    for cpp_file in list(controller_files) + list(filter_files):
        code = cpp_file.read_text()
        yaml = PROMPT_PATH.read_text()
        prompt = build_prompt(code, yaml)
        print(f"Sending {cpp_file.relative_to(SRC_DIR)} to LLM")

        result = query_ollama(MODEL, prompt)
        output_file = TEST_DIR / f"test_{cpp_file.stem}.cpp"
        output_file.write_text(result)
        print(f"Test written to: {output_file}")

if __name__ == "__main__":
    TEST_DIR.mkdir(exist_ok=True)
    generate_tests()

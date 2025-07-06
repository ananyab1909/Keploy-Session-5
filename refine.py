from pathlib import Path
from llm_interface import query_ollama

MODEL = "qwen2:0.5b"
TEST_DIR = Path("tests")  
REFINED_DIR = Path("refined_tests")  
PROMPT_PATH = Path("prompts/refine_tests.yaml")  

def build_refine_prompt(test_code: str, yaml_prompt: str) -> str:
    return f"""
# TASK INSTRUCTION:
{yaml_prompt}

# GOOGLE TEST CODE TO REFINE:
{test_code}

# TASK:
Refine this unit test code:
- Remove duplicate or redundant tests
- Add any missing includes or headers
- Improve test coverage by introducing edge cases
- Use Google Test best practices
- Preserve all valid tests
- Output clean and valid C++ code ONLY
"""

def refine_tests():
    print("Starting refinement step...")
    yaml_prompt = PROMPT_PATH.read_text()
    REFINED_DIR.mkdir(exist_ok=True)

    for test_file in TEST_DIR.glob("test_*.cpp"):
        print(f"Refining: {test_file.name}")
        test_code = test_file.read_text(encoding="utf-8")
        prompt = build_refine_prompt(test_code, yaml_prompt)

        refined_code = query_ollama(MODEL, prompt)

        output_file = REFINED_DIR / test_file.name.replace(".cpp", "_refined.cpp")
        output_file.write_text(refined_code, encoding="utf-8")
        print(f"Refined test saved: {output_file.name}")

if __name__ == "__main__":
    refine_tests()

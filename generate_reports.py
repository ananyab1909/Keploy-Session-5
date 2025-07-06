import json
from pathlib import Path
from datetime import datetime
import re

def parse_coverage_output(output: str, project_root: Path) -> dict:
    coverage = {}
    current_file = None
    lines = output.splitlines()

    for line in lines:
        file_match = re.match(r"^File '(.+)'$", line)
        if file_match:
            current_file = Path(file_match.group(1)).resolve()
            continue

        coverage_match = re.match(r"^Lines executed:(\d+\.\d+)% of (\d+)", line)
        if coverage_match and current_file and str(current_file).startswith(str(project_root)):
            percentage = float(coverage_match.group(1))
            total_lines = int(coverage_match.group(2))
            coverage[str(current_file)] = {
                "percentage": percentage,
                "totalLines": total_lines
            }
            current_file = None

    return coverage


def generate_report(tests, coverage: dict, output_dir: Path) -> dict:
    output_dir.mkdir(parents=True, exist_ok=True)
    report = {
        "timestamp": datetime.utcnow().isoformat() + "Z",
        "summary": {
            "totalTests": len(tests),
            "totalTestCases": 0,
            "averageCoverage": 0,
            "filesProcessed": [t['originalFile'].name for t in tests]
        },
        "details": []
    }

    total_coverage = 0
    coverage_count = 0

    for test in tests:
        content = test['content']
        test_case_count = len(re.findall(r"TEST(?:_F|_P)?\s*\(", content))
        report['summary']['totalTestCases'] += test_case_count

        file_path = str(test['originalFile'].resolve())
        file_coverage = coverage.get(file_path, {})
        coverage_percent = file_coverage.get('percentage', 0)

        if file_coverage:
            total_coverage += coverage_percent
            coverage_count += 1

        report['details'].append({
            "originalFile": test['originalFile'].name,
            "testFile": test['testFile'],
            "testCases": test_case_count,
            "coverage": coverage_percent
        })

    if coverage_count > 0:
        report['summary']['averageCoverage'] = round(total_coverage / coverage_count, 2)

    report_path = output_dir / "test_report.json"
    with open(report_path, "w") as f:
        json.dump(report, f, indent=2)

    print(" Report saved to:", report_path)
    print(" Total tests:", report["summary"]["totalTests"])
    print(" Total test cases:", report["summary"]["totalTestCases"])
    print("Average coverage:", report["summary"]["averageCoverage"], "%")

    return report

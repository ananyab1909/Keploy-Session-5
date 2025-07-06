import os
from pathlib import Path

def scan_and_copy_cpp_files(source_dir, dest_dir):
    cpp_extensions = ('.cpp', '.cc', '.cxx', '.c')

    source_dir = Path(source_dir).resolve()
    dest_dir = Path(dest_dir).resolve()

    print("Scanning for C++ source files...\n")

    file_count = 0

    for root, _, files in os.walk(source_dir):
        for file in files:
            if file.lower().endswith(cpp_extensions):
                full_path = Path(root) / file
                relative_path = full_path.relative_to(source_dir)
                target_path = dest_dir / relative_path

                target_path.parent.mkdir(parents=True, exist_ok=True)
                with open(full_path, 'r', encoding='utf-8') as src, open(target_path, 'w', encoding='utf-8') as dst:
                    dst.write(src.read())

                print(f"Copied: {relative_path}")
                file_count += 1

    print(f"\n Total {file_count} C++ source files copied to '{dest_dir}'.")


source_path = "C:\Coding\Keploy\DAY6\orgChartApi"        
destination_path = "./cpp_projects"         

scan_and_copy_cpp_files(source_path, destination_path)

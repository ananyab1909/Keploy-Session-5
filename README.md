# Keploy-Session-5

## Description
This repository contains an automated C++ unit test generation pipeline for a Drogon-based REST API. It uses LLMs to generate, refine, and improve Google Test cases for source files, while measuring and improving test coverage through lcov. The pipeline detects build failures, suggests code fixes, and iteratively enhances tests.

## Project Overview

Key features include:
  1. LLM-powered unit test generation using Ollama and Qwen2 to produce Google Test cases from real C++ source files
  2. Automated test refinement and recovery — detects build failures, sends errors to the LLM, and regenerates corrected test code
  3. Achieved 80%+ code coverage by iteratively improving tests based on lcov coverage reports
  4. Full coverage across core API logic including AuthController, Department, Job, and other business-critical components
  5. Integrated test coverage reporting using lcov and genhtml to produce a detailed index.html report
  6. Fully containerized pipeline with optional Docker support for consistent local and CI environments


## Implementation

  1. Clone the repository
      ```
      git clone https://github.com/ananyab1909/Keploy-Session-5.git
      cd Keploy-Session-5
      ```

  2. Scan the C++ project directory - Use scanfiles.py to recursively extract all .cc and .cpp files from any external C++ API directory (e.g., cpp_projects/).
       ```
       python scanfile.py
       ```
  3. Generate initial tests - Run run.py to generate basic unit tests using Ollama + Qwen2. This script reads C++ logic and produces corresponding Google Test files.
    ```
    python run.py
    ```
  4. Refine test quality - Improve formatting, remove duplication, and increase correctness using refine.py, which sends each generated test to the LLM with a refinement prompt.
      ```
      python refine.py
      ```
  5. Build, debug, and fix broken tests. Execute built.py to build your project with refined tests. If the build fails, the script captures the logs and sends them to the LLM to regenerate valid tests. If it           passes, it runs the test suite and calculates coverage.
       ```
       python built.py
       ```  
  6. Generate test coverage report - The script uses lcov and genhtml to produce a full HTML coverage report located at:
       ```
       coverage_html/index.html
       ```
  7. API Integration - If your C++ project exposes REST API endpoints (e.g., via Drogon), ensure the API server is running before running tests that depend on live HTTP interactions.
     

## Model Requirements

  - Ollama must be installed on device.
  - For __Linux__, execute `curl -fsSL https://ollama.com/install.sh | sh` in your terminal.
  - After installing, execute `ollama --version` to check whether ollama is properly configured.
  - For other platforms, please visit [Github](https://github.com/ollama/ollama/tree/main)
    

## About Me

  Hello, my name is Ananya Biswas. I am an Engineering Student at [Kalinga Institute of Industrial Technology](https://kiit.ac.in/). I enjoy making projects and now that my this project is over, I am open-      
  sourcing the project. Hope you like it! Lastly, I would like to put it out there that I have worked on other projects that you may like. You can check them out at my [Github](https://github.com/ananyab1909/). 
  Give it a whirl and let me know your thoughts.
  

## Socials

  - Portfolio : https://dub.sh/ananyabiswas
  - LinkedIn : https://linkedin.com/in/ananya-biswas-kiit/
  - Mastodon : https://mastodon.social/@captain_obvious/
  - Twitter : https://x.com/not_average_x/
  - Github : https://github.com/ananyab1909/

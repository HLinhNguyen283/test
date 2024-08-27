import argparse
import os

from engine.file_processor.file_processor import CFileProcessor
from engine.stub_gen.stub_generator import CStubGenerator
from engine.csv_gen.csv_generator import CCSVGenerator


def generate_stub_and_csv(project_name: str):
    # Instantiate the FileReader class
    file_processor = CFileProcessor(project_name)

    # Extract function signatures using the FunctionExtractor class
    all_functions = file_processor.extract_function_signatures()

    # Generate the stub.c file_processor
    stub_generator = CStubGenerator(all_functions, file_processor.project_path)
    stub_generator.write_stub_file()
    print(all_functions)
    # Generate the functions.csv file
    csv_generator = CCSVGenerator(all_functions, file_processor.project_path)
    csv_generator.write_csv_files()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Python mini-game")
    parser.add_argument("project_name", type=str, help="math_project")
    args = parser.parse_args()
    generate_stub_and_csv(args.project_name)

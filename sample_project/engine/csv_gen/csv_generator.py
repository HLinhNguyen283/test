import os
import csv

from engine.file_processor.file_processor import CFileProcessor


class CCSVGenerator:
    def __init__(self, file_processor: CFileProcessor):
        self._file_processor = file_processor

    def write_csv_files(self):
        # Prepare data for the CSV file
        output_dir = self._file_processor.get_test_csv_path()
        print(self._file_processor.get_functions())
        for func in self._file_processor.get_functions():
            # Create a CSV file for each function
            function_name = func["func_name"]
            source_file = (
                func.get("source_file", "unknown").replace(".c", "_c")
            )

            csv_file_name = f"{source_file}_{function_name}.csv"
            csv_file_path = os.path.join(output_dir, csv_file_name)

            with open(csv_file_path, mode="w", newline="") as file:
                writer = csv.writer(file)
                writer.writerow(["Function Name", "Return Type", "Parameters"])
                params_str = "; ".join(func["params"]) if func["params"] else "None"
                writer.writerow([func["func_name"], func["return_type"], params_str])

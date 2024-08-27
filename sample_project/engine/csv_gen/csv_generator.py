import os
import csv


class CCSVGenerator:
    def __init__(self, functions, project_path):
        self.functions = functions
        self.output_path = os.path.join(
            project_path, "tests", "unittest", "test_csv"
        )

    def write_csv_files(self):
        # Ensure the output directory exists
        os.makedirs(os.path.dirname(self.output_path), exist_ok=True)
        # Prepare data for the CSV file
        
        for func in self.functions:
            # Create a CSV file for each function
            function_name = func["name"]
            source_file = func.get("source_file", "unknown").replace(".c", "").replace(".", "_")
            csv_file_name = f"{source_file}_{function_name}.csv"
            csv_file_path = os.path.join(self.output_path, csv_file_name)

            with open(csv_file_path, mode="w", newline="") as file:
                writer = csv.writer(file)
                writer.writerow(["Function Name", "Return Type", "Parameters"])
                params_str = "; ".join(func["params"]) if func["params"] else "None"
                writer.writerow([func["name"], func["return_type"], params_str])
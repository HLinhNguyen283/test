import os
import csv


class CCSVGenerator:
    def __init__(self, functions, project_path):
        self.functions = functions
        self.output_path = os.path.join(
            project_path, "tests", "unittest", "test_csv", "functions.csv"
        )

    def write_csv_file(self):
        # Ensure the output directory exists
        os.makedirs(os.path.dirname(self.output_path), exist_ok=True)
        # Prepare data for the CSV file
        csv_data = [["Function Name", "Return Type", "Parameters"]]
        for func in self.functions:
            params_str = "; ".join(func["params"]) if func["params"] else "None"
            csv_data.append([func["name"], func["return_type"], params_str])
        with open(self.output_path, mode="w", newline="") as file:
            writer = csv.writer(file)
            writer.writerows(csv_data)

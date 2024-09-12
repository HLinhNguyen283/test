import os
import csv

from engine.file_processor.file_processor import CFileProcessor


class CCSVGenerator:
    def __init__(self, file_processor: CFileProcessor):
        self._file_processor = file_processor

    def _generate_csv_c_content(self):
        csv_content = ""
        for include in self._file_processor.get_includes():
            csv_content += f'#include "{include}"\n'
        csv_content += "\n"

        for func in self._file_processor.get_functions():
            csv_function_name = f"AMSTB_{func['func_name']}"
            param_strs = []
            for param in func["params"]:
                param_name = param.split()[-1]
                param_type = " ".join(param.split()[:-1])
                param_strs.append(
                    f"{param_type} volatile AMOUT_{param_name} = {param_name};"
                )
            params_declaration_str = ", ".join(func["params"])
            csv_content += f"{func['return_type']} {csv_function_name}({params_declaration_str}) {{\n"
            csv_content += f"    {func['return_type']} volatile AMIN_return;\n"
            csv_content += "    " + "\n    ".join(param_strs) + "\n"
            csv_content += f"    return AMIN_return;\n"
            csv_content += "}\n\n"

        return csv_content

    def write_csv_files(self):
        # Prepare data for the CSV file
        output_dir = self._file_processor.get_test_csv_path()
        print(self._file_processor.get_functions())
        for func in self._file_processor.get_functions():
            function_name = func["func_name"]
            source_file = func.get("source_file", "unknown").replace(".c", "")

            # Tạo tên file CSV
            csv_file_name = f"{source_file}_{function_name}.csv"
            csv_file_path = os.path.join(output_dir, csv_file_name)

            # Dữ liệu cho CSV
            data = []

            # Dòng 1: <sourcefile_name>/<function_name>;<number_of_input>;<number_of_output>
            input_count = len(func["params"])  # Số lượng input
            output_count = 2
            source_line = f"{source_file}/{function_name};{input_count};{output_count}"
            data.append([source_line])

            related_function_line = f"AMSTB_{function_name}_addFractions;addFractions"
            data.append([related_function_line])

            # Dòng 3: #COMMENT;... (các tham số và giá trị mẫu)
            comment_line = "#COMMENT;@fraction_1.numerator;@fraction_1.denominator;@fraction_2.numerator;@fraction_2.denominator;@fraction_3.numerator;@fraction_3.denominator;@@return.numerator;@@return.denominator"
            data.append([comment_line])

            # Dòng 4: Giá trị mẫu
            values_line = ";1;1;1;1;1;1;1;1"
            data.append([values_line])

            # Ghi dữ liệu vào file CSV
            with open(csv_file_path, mode="w", newline="") as file:
                writer = csv.writer(file, delimiter=";")
                for row in data:
                    writer.writerow(row)

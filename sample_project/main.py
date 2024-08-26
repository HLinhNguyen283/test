# import os
# # Function to generate the stub.c content
# def generate_stub_c(functions):
#     stub_content = []
    
#     for c_file, funcs in functions.items():
#         stub_content.append(f"// Stubs generated for {os.path.basename(c_file)}\n")
#         for return_type, func_name, params in funcs:
#             stub_content.append(f"{return_type} {func_name}({params}) {{\n")
#             if return_type != "void":
#                 stub_content.append(f"    // TODO: implement {func_name}\n")
#                 stub_content.append(f"    return ({return_type}) 0;\n")
#             else:
#                 stub_content.append(f"    // TODO: implement {func_name}\n")
#             stub_content.append("}\n\n")
    
#     return ''.join(stub_content)

# all_functions = {
#     './math_project/src/cal.c': [
#         ('Fraction', 'addThreeFractions', 'const Fraction& fraction1, const Fraction& fraction2, const Fraction& fraction3')
#     ],
#     './math_project/src/fraction.c': [
#         ('Fraction', 'createFraction', 'int numerator, int denominator'),
#         ('Fraction', 'addFractions', 'const Fraction& fraction1, const Fraction& fraction2'),
#         ('Fraction', 'subtractFractions', 'const Fraction& fraction1, const Fraction& fraction2'),
#         ('Fraction', 'multiplyFractions', 'const Fraction& fraction1, const Fraction& fraction2'),
#         ('Fraction', 'divideFractions', 'const Fraction& fraction1, const Fraction& fraction2'),
#         ('void', 'simplifyFraction', 'Fraction& fraction'),
#         ('void', 'printFraction', 'const Fraction& fraction')
#     ]
# }
   
# # Define the output path for the generated stub.c file
# stub_file_path = './math_project/tests/unittest/test_csv/generated_stub.c'

# def generate_stub_and_csv():
#     # Generate the stub.c content
#     stub_content = generate_stub_c(all_functions)

#     # Write the content to the generated_stub.c file
#     with open(stub_file_path, 'w') as stub_file:
#         stub_file.write(stub_content)

#     stub_file_path

# if __name__ == "__main__":
#     generate_stub_and_csv()

import os
import argparse

parser = argparse.ArgumentParser(description="Python mini-game")
parser.add_argument('project_name', type=str, help="math_project")

args = parser.parse_args()


# List the contents of the 'sample_project' directory
current_file_path = os.path.abspath(__file__)
project_path = os.path.dirname(current_file_path)

# List the contents of the 'math_project' directory
math_project_path = os.path.join(project_path, args.project_name)

# List the contents of the 'src' directory
src_path = os.path.join(math_project_path, 'src')
src_contents = os.listdir(src_path)
src_contents

# Read the contents of the .c files
cal_c_path = os.path.join(src_path, 'cal.c')
fraction_c_path = os.path.join(src_path, 'fraction.c')

with open(cal_c_path, 'r') as file:
    cal_c_content = file.read()

with open(fraction_c_path, 'r') as file:
    fraction_c_content = file.read()

# Show a portion of the content to understand the structure
cal_c_content[:500], fraction_c_content[:500]

import re

# Function to extract function signatures
def extract_function_signatures(c_content):
    # Regular expression to match function signatures
    function_pattern = re.compile(
        r'(\w[\w\s\*]+)\s+(\w+)\s*\(([^)]*)\)\s*{'
    )
    matches = function_pattern.findall(c_content)
    
    # Extracted information
    functions = []
    for return_type, name, params in matches:
        # Clean up the return type and parameters
        return_type = return_type.strip()
        params = [param.strip() for param in params.split(',')] if params else []
        functions.append({
            'return_type': return_type,
            'name': name,
            'params': params
        })
    return functions

# Extract function signatures from both files
cal_functions = extract_function_signatures(cal_c_content)
fraction_functions = extract_function_signatures(fraction_c_content)

# Display the extracted information
print(cal_functions)
print(fraction_functions)

# Function to generate stub.c content from function signatures
def generate_stub_c_content(functions):
    stub_content = ""
    for func in functions:
        params_str = ", ".join(func['params'])
        stub_content += f"{func['return_type']} {func['name']}({params_str});\n\n"
    return stub_content

# Generate the content for stub.c
stub_c_content = generate_stub_c_content(cal_functions + fraction_functions)

# Path to save the stub.c file
stub_c_path = os.path.join(project_path, 'stub.c')

# Write the stub.c file
with open(stub_c_path, 'w') as file:
    file.write(stub_c_content)

# Show the generated stub.c content
stub_c_content

import csv

# Path to save the CSV file
csv_file_path = os.path.join(project_path, 'functions.csv')

# Prepare data for the CSV file
csv_data = [['Function Name', 'Return Type', 'Parameters']]
for func in cal_functions + fraction_functions:
    params_str = "; ".join(func['params']) if func['params'] else "None"
    csv_data.append([func['name'], func['return_type'], params_str])

# Write to the CSV file
with open(csv_file_path, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(csv_data)

# Show the path where CSV is saved
csv_file_path

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



import re, os

# Function to extract function signatures from a C file content
def extract_function_signatures(c_file_content):
    # Regex to match C function signatures
    pattern = r'\w+\s+\w+\([^)]*\)\s*\{'
    matches = re.findall(pattern, c_file_content)
    return matches

# Extract function signatures from both cal.c and fraction.c
fraction_c_path = os.path.join(src_dir, 'fraction.c')

with open(fraction_c_path, 'r') as file:
    fraction_c_content = file.read()

# Extract function signatures
cal_signatures = extract_function_signatures(cal_c_content)
fraction_signatures = extract_function_signatures(fraction_c_content)

cal_signatures, fraction_signatures

import os


class CStubGenerator:
    def __init__(self, functions, project_path):
        self.functions = functions
        self.output_path = os.path.join(project_path, "tests", "unittest", "stub.c")

    def generate_stub_c_content(self):
        stub_content = ""
        for func in self.functions:
            params_str = ", ".join(func["params"])
            stub_content += f"{func['return_type']} {func['name']}({params_str});\n\n"
        return stub_content

    def write_stub_file(self):
        # Ensure the output directory exists
        os.makedirs(os.path.dirname(self.output_path), exist_ok=True)
        # Write the stub content to the file
        stub_content = self.generate_stub_c_content()
        with open(self.output_path, "w") as file:
            file.write(stub_content)

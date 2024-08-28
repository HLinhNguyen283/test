import os

from engine.file_processor.file_processor import CFileProcessor


class CStubGenerator:
    def __init__(self, file_processor: CFileProcessor):
        self._file_processor = file_processor
        self._output_path = os.path.join(
            self._file_processor.get_unittest_path(), "stub.c"
        )

    def generate_stub_c_content(self):
        stub_content = ""
        for include in self._file_processor.get_includes():
            stub_content += f'#include "{include}"\n'
        stub_content += "\n"

        for func in self._file_processor.get_functions():
            stub_function_name = f"AMSTB_{func['func_name']}"
            param_strs = []
            for param in func["params"]:
                param_name = param.split()[-1]
                param_type = " ".join(param.split()[:-1])
                param_strs.append(
                    f"{param_type} volatile AMOUT_{param_name} = {param_name};"
                )
            params_declaration_str = ", ".join(func["params"])
            stub_content += f"{func['return_type']} {stub_function_name}({params_declaration_str}) {{\n"
            stub_content += f"    {func['return_type']} volatile AMIN_return;\n"
            stub_content += "    " + "\n    ".join(param_strs) + "\n"
            stub_content += f"    return AMIN_return;\n"
            stub_content += "}\n\n"

        return stub_content

    def write_stub_file(self):
        # Write the stub content to the file
        stub_content = self.generate_stub_c_content()
        with open(self._output_path, "w") as file:
            file.write(stub_content)

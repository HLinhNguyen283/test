import os
import re

import os.path as osp


class CFileProcessor:
    def __init__(self, project_name):
        self.project_name = project_name
        self.project_path = osp.join(
            osp.dirname(osp.abspath(self.__module__)), self.project_name
        )
        self.src_path = osp.join(self.project_path, "src")

        self.c_file_contents = {}
        print(self.c_file_contents)
        
    def read_c_files(self):
        for root, dirs, files in os.walk(self.src_path):
            for file in files:
                if file.endswith(".c"):
                    file_path = os.path.join(root, file)
                    with open(file_path, "r") as c_file:
                        self.c_file_contents[file] = c_file.read()

    def extract_function_signatures(self):
        self.read_c_files()
        function_pattern = re.compile(r"(\w[\w\s\*]+)\s+(\w+)\s*\(([^)]*)\)\s*{")
        all_functions = []
        for file_name, content in self.c_file_contents.items():
            matches = function_pattern.findall(content)
            for return_type, name, params in matches:
                return_type = return_type.strip()
                params = (
                    [param.strip() for param in params.split(",")] if params else []
                )
                all_functions.append({
                    "return_type": return_type,
                    "name": name,
                    "params": params,
                    "source_file": file_name  # Add the source file name to the function info
                })
        return all_functions

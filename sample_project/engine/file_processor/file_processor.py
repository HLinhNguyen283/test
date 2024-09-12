import os
import re

import os.path as osp


class CFileProcessor(object):
    _instance = None

    def __new__(cls, project_name):
        if cls._instance is None:
            cls._instance = super(CFileProcessor, cls).__new__(cls)
            cls._instance._initialized = False
        return cls._instance

    def __init__(self, project_name):
        self._project_name = project_name
        self._project_path = osp.join(
            osp.dirname(osp.abspath(self.__module__)), self._project_name
        )
        self._src_path = osp.join(self._project_path, "src")
        self._tests_path = osp.join(self._project_path, "tests")
        self._unittest_path = osp.join(self._tests_path, "unittest")
        self._test_csv_path = osp.join(self._unittest_path, "test_csv")

        self._c_file_contents = {}
        self._includes = set()
        self._functions_signatures = None  # Store function signatures

        # Ensure the required directories exist
        self._create_directories()
        self._initialized = True

    def _create_directories(self):
        """
        Create the required directories (tests, tests/unittest, tests/unittest/test_csv)
        if they do not exist.
        """
        os.makedirs(self._tests_path, exist_ok=True)
        os.makedirs(self._unittest_path, exist_ok=True)
        os.makedirs(self._test_csv_path, exist_ok=True)

    def read_c_files(self):
        include_pattern = re.compile(r'#include\s+["<](.*?)[">]')
        for root, dirs, files in os.walk(self._src_path):
            for file in files:
                if file.endswith(".c"):
                    file_path = os.path.join(root, file)
                    with open(file_path, "r") as c_file:
                        content = c_file.read()
                        self._c_file_contents[file] = content
                        includes = include_pattern.findall(content)
                        self._includes.update(includes)

    def extract_function_signatures(self):
        if self._functions_signatures is None:
            function_pattern = re.compile(r"(\w[\w\s\*]+)\s+(\w+)\s*\(([^)]*)\)\s*{")
            func_call_pattern = re.compile(r"\b(\w+)\s*\(")
            self._functions_signatures = []
            for file_name, content in self._c_file_contents.items():
                matches = function_pattern.findall(content)
                calls = func_call_pattern.findall(content)

                for return_type, func_name, params in matches:
                    return_type = return_type.strip()
                    params = (
                        [param.strip() for param in params.split(",")] if params else []
                    )
                    self._functions_signatures.append(
                        {
                            "return_type": return_type,
                            "func_name": func_name,
                            "params": params,
                            "source_file": file_name,
                        }
                    )
                # Find function calls within this function
                self._find_called_functions(content, func_name)

    def _find_called_functions(self, content, function_name):
        # Regular expression to find function calls
        func_call_pattern = re.compile(r"\b(\w+)\s*\(")
        calls = func_call_pattern.findall(content)

        # Remove self function from the list of called functions
        calls = [call for call in calls if call != function_name]

        if function_name not in self._called_functions:
            self._called_functions[function_name] = calls

    def get_project_name(self) -> str:
        return self._project_name

    def get_project_path(self) -> str:
        return self._project_path

    def get_src_path(self) -> str:
        return self._src_path

    def get_tests_path(self) -> str:
        return self._tests_path

    def get_unittest_path(self) -> str:
        return self._unittest_path

    def get_test_csv_path(self) -> str:
        return self._test_csv_path

    def get_c_file_contents(self):
        return self._c_file_contents

    def get_includes(self):
        return self._includes

    def get_functions(self):
        return self._functions_signatures

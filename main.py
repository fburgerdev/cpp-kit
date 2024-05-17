from comments import *
from preprocessor import *
from os import path

target_file = "./tests/logging.hpp"
default_include = "./tests/src_logging/"
dest_file = "out_" + path.basename(target_file)

class HPPMergeFile:
    def __init__(self, target_file: str, default_include: str) -> None:
        self.int_include_paths = []
        self.ext_include_lines = []
        self.lines = []
        self.default_include = default_include
        self.__parse(target_file)
    def get_real_path(self, filepath: str, include_origin: str) -> str:
        if path.exists(include_origin + filepath):
            return path.realpath(include_origin + filepath)
        elif path.exists(self.default_include + filepath):
            return path.realpath(self.default_include + filepath)
        else:
            raise ValueError()
    def __parse(self, realpath: str):
        if realpath in self.int_include_paths:
            return
        first_real_line = True
        for line in remove_multline_comments(open(realpath).read()).split('\n'):
            if is_pragma_once(line):
                if not realpath in self.int_include_paths:
                    self.int_include_paths.append(realpath)
                continue
            if is_include(line):
                if is_quote_include(line):
                    self.__parse(self.get_real_path(get_quote_include(line), realpath[0:realpath.rfind("/")+1]))
                else:
                    include_line = f"#include <{get_angle_include(line)}>"
                    if not include_line in self.ext_include_lines:
                        self.ext_include_lines.append(include_line)
            else:
                if first_real_line and 0 < len(line.strip()):
                    self.lines.append(f'// #include "{path.basename(realpath)}" (HPPMERGE)')
                    first_real_line = False
                self.lines.append(line)
    def write(self, filepath: str) -> str:
        with open(filepath, "w") as file:
            file.write("#pragma once\n")
            file.write("// #include <...> (HPPMERGE)\n")
            file.write("\n".join(self.ext_include_lines))
            file.write("\n")
            file.write("\n".join(self.lines))

mergefile = HPPMergeFile(target_file, default_include)
mergefile.write(dest_file)
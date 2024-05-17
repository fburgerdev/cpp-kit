import sys
from os import path
# remove comments
def remove_multline_comments(source: str) -> str:
    newsource = ""
    in_multiline, in_string = False, False
    index = 0
    while index < len(source):
        if not in_string:
            if source[index] == '/':
                if (index + 1 < len(source) and source[index + 1] == '*'):
                    in_multiline = True
                if not in_multiline and (index + 1 < len(source) and source[index + 1] == '/'):
                    endofline = source.find('\n', index)
                    if endofline == -1:
                        return newsource + source[index:]
                    else:
                        newsource += source[index:endofline+1]
                        index = endofline + 1
                        continue
                if 0 < index and source[index - 1] == '*':
                    in_multiline = False
                    index += 1
                    continue
        if not in_multiline:
            newsource += source[index]
            if source[index] == '"' and (index == 0 or source[index - 1] != '\\'):
                in_string = not in_string
        index += 1
    return newsource
# preprocessor
# preprocessor :: pragma once
def is_pragma_once(line: str) -> bool:
    return line.strip().startswith("#pragma") and line[line.index("#pragma") + len("#pragma"):].strip().startswith("once")
# preprocessor :: include
def is_include(line: str) -> bool:
    return line.strip().startswith("#include")
def is_quote_include(line: str) -> bool:
    split = line.split('"')
    return 3 <= len(split) and split[0].strip() == "#include"
def is_angle_include(line: str) -> bool:
    split = line.split('<')
    return 2 <= len(split) and split[0].strip() == "#include"
def get_quote_include(line: str) -> bool:
    return line.split('"')[1]
def get_angle_include(line: str) -> bool:
    return line[line.index('<')+1:line.index('>')]

class HPPMergeFile:
    def __init__(self, target_file: str, default_includes: list[str]) -> None:
        self.int_include_paths = []
        self.ext_include_lines = []
        self.lines = []
        self.default_includes = default_includes
        self.__parse(target_file)
    def get_real_path(self, filepath: str, include_origin: str) -> str:
        if path.exists(include_origin + filepath):
            return path.realpath(include_origin + filepath)
        else:
            for default_include in self.default_includes:
                if path.exists(default_include + filepath):
                    return path.realpath(default_include + filepath)
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

if __name__ == "__main__":
    # args
    target_file = sys.argv[1]
    default_includes = sys.argv[2:]
    dest_file = path.dirname(target_file) + "/merged_" + path.basename(target_file)
    # merge
    mergefile = HPPMergeFile(target_file, default_includes)
    mergefile.write(dest_file)
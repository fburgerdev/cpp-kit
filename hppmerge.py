import sys
from os import path
# remove comments
def remove_multline_comments(source: str) -> str:
    out = str()
    in_multiline, in_string = False, False
    index = 0
    while index < len(source):
        if not in_string and index + 1 < len(source):
            if not in_multiline and source[index:index + 2] == '//':
                endofline = source.find('\n', index)
                if endofline == -1:
                    return out + source[index:]
                else:
                    out += source[index:endofline+1]
                    index = endofline + 1
                    continue
            if source[index:index + 2] == '/*':
                in_multiline = True
                index += 2
                continue
            if source[index:index + 2] == '*/':
                in_multiline = False
                index += 2
                continue
        if not in_multiline:
            out += source[index]
            if source[index] == '"' and (index == 0 or source[index - 1] != '\\'):
                in_string = not in_string
        index += 1
    return out
# preprocessor
# preprocessor :: pragma once
def is_pragma_once(line: str) -> bool:
    words = line.split()
    if 2 <= len(words) and words[0] == "#pragma" and words[1] == "once":
        return True
    elif 3 <= len(words) and words[0] == "#" and words[1] == "pragma" and words[2] == "once":
        return True
    else:
        return False
# preprocessor :: include
def is_include(line: str) -> bool:
    words = line.split()
    if 1 <= len(words) and words[0] == "#include":
        return True
    elif 2 <= len(words) and words[0] == "#" and words[1] == "include":
        return True
    else:
        return False
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

# HPPMergeFile
class HPPMergeFile:
    # constructor
    def __init__(self, target_file: str, default_include: str, ignore_includes: list[str] = []) -> None:
        self.int_include_paths = []
        self.ext_include_lines = []
        self.ignored_include_lines = []
        self.lines = []
        self.default_include = default_include
        self.ignore_includes = ignore_includes
        self.__parse(target_file)
    # path
    def get_real_path(self, filepath: str, include_origin: str, from_path: str) -> str:
        if path.exists(include_origin + filepath):
            return path.realpath(include_origin + filepath)
        else:
            if path.exists(self.default_include + filepath):
                return path.realpath(self.default_include + filepath)
        # error
        print(f"include '{filepath}' not found (included from '{from_path}')")
        sys.exit(1)
    # parse
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
                    include = get_quote_include(line)
                    if include in self.ignore_includes:
                        self.ignored_include_lines.append(line)
                    else:
                        self.__parse(self.get_real_path(include, path.dirname(realpath) + '/', realpath))
                else:
                    include_line = f"#include <{get_angle_include(line)}>"
                    if not include_line in self.ext_include_lines:
                        self.ext_include_lines.append(include_line)
            else:
                if first_real_line and 0 < len(line.strip()):
                    self.lines.append(f'// #include "{path.basename(realpath)}" (HPPMERGE)')
                    first_real_line = False
                self.lines.append(line)
    # write
    def write(self, filepath: str) -> str:
        with open(filepath, "w") as file:
            file.write("#pragma once\n")
            if len(self.ext_include_lines):
                file.write("// #include <...> (HPPMERGE)\n")
                file.write("\n".join(self.ext_include_lines))
                file.write("\n")
            if len(self.ignored_include_lines):
                file.write("// #include \"...\" (HPPMERGE)\n")
                file.write("\n".join(self.ignored_include_lines))
                file.write("\n")
            file.write("\n".join(self.lines))

# main
if __name__ == "__main__":
    # args
    if len(sys.argv) < 4:
        print(f"usage: {sys.argv[0]} target_file dest_file default_include [ignore_includes...]")
        sys.exit(1)
    # args :: origin_file
    origin_file = sys.argv[1]
    if not path.exists(origin_file):
        print(f"file '{origin_file}' not found")
        sys.exit(1)
    # args :: dest_file
    dest_file = sys.argv[2]
    # args :: default_include
    default_include = sys.argv[3]
    if 0 < len(default_include) and default_include[-1] != '/':
        default_include += '/'
    # args :: ignore_includes
    ignore_includes = []
    for i in range(4, len(sys.argv)):
        ignore_includes.append(sys.argv[i])
    # merge
    mergefile = HPPMergeFile(origin_file, default_include, ignore_includes)
    # write
    mergefile.write(dest_file)
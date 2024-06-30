import sys # sys.argv, sys.exit
from os import path # path.exists, path.basename, path.dirname
import re # re.split
from glob import glob # glob
from fnmatch import fnmatch # fnmatch
import pathlib # pathlib.Path.mkdir

# remove
# :: remove_comments
def remove_comments(source: str) -> str:
    out = str()
    in_comment, in_string = False, False
    index = 0
    while index < len(source):
        if not in_string and index + 2 <= len(source):
            if not in_comment and source[index : index + 2] == '//':
                new_line = source.find('\n', index)
                index = new_line if new_line != -1 else len(source)
                continue
            if source[index : index + 2] == '/*':
                in_comment = True
                index += 2
                continue
            if source[index : index + 2] == '*/':
                in_comment = False
                index += 2
                continue
        if not in_comment:
            out += source[index]
            if source[index] == '"' and (index == 0 or source[index - 1] != '\\'):
                in_string = not in_string
            index += 1
    return out
# :: remove_empty_lines
def remove_empty_lines(source: str) -> str:
    return '\n'.join([ line for line in source.split('\n') if len(line.strip()) ])

# preprocessor
# :: is_pragma_once
def is_pragma_once(line: str) -> bool:
    words = line.split()
    if 2 <= len(words) and words[0] == "#pragma" and words[1] == "once":
        return True
    elif 3 <= len(words) and words[0] == "#" and words[1] == "pragma" and words[2] == "once":
        return True
    return False
# :: get_include
def get_include(line: str) -> str | None:
    words = [ word for word in re.split('"|<|>|\\s', line) if len(word) ]
    if 1 < len(words) and words[0] == "#include":
        return words[1]
    elif 2 < len(words) and words[0] == "#" and words[1] == "include":
        return words[2]
    return None
# is_quote_include
def is_quote_include(line: str) -> bool:
    return get_include(line) and ('"' in line)
# is_angle_include
def is_angle_include(line: str) -> bool:
    return get_include(line) and ('<' in line and '>' in line)

# HPPMergeFile
class HPPMergeFile:
    # constructor
    def __init__(self, target_filepath: str, default_includes: list[str], ignore_includes: list[str] = []) -> None:
        self.internal_include_paths = []
        self.external_include_lines = []
        self.ignored_include_lines = []
        self.lines = []
        self.default_includes = default_includes
        self.ignore_includes = ignore_includes
        self.__parse(target_filepath)
    # get_include_path
    def get_include_path(self, filepath: str, include_origin: str) -> str:
        # local include
        if path.exists(f'{path.dirname(include_origin)}/{filepath}'):
            return f'{path.dirname(include_origin)}/{filepath}'
        # default include
        else:
            for default_include in self.default_includes:
                include_dirs = glob(f'{default_include}/{filepath}')
                if len(include_dirs):
                    return include_dirs[0]
        # error: include not found
        print(f"error: include '{filepath}' not found (included from '{include_origin}')")
        sys.exit(1)
    # __parse
    def __parse(self, filepath: str):
        if filepath in self.internal_include_paths:
            return
        first_line = True
        for line in remove_comments(open(filepath).read()).split('\n'):
            if len(line.strip()) == 0:
                continue
            if is_pragma_once(line):
                if not filepath in self.internal_include_paths:
                    self.internal_include_paths.append(filepath)
                continue
            if is_quote_include(line):
                include_path = self.get_include_path(get_include(line), filepath)
                for ignored_include in self.ignore_includes:
                    if fnmatch(include_path, ignored_include):
                        if f'#include <{include_path}>' not in self.ignored_include_lines:
                            self.ignored_include_lines.append(f'#include "{get_include(line)}"')
                        break
                else:
                    self.__parse(include_path)
            elif is_angle_include(line):
                include_path = get_include(line)
                if f'#include <{include_path}>' not in self.external_include_lines:
                    self.external_include_lines.append(f'#include <{include_path}>')
            else:
                if first_line:
                    self.lines.append(f'// #include "{path.basename(filepath)}" (HPPMERGE)')
                    first_line = False
                self.lines.append(line)
    # write
    def write(self, filepath: str) -> str:
        pathlib.Path(path.dirname(filepath)).mkdir(parents=True, exist_ok=True)
        with open(filepath, 'w') as file:
            file.write('#pragma once\n')
            if len(self.external_include_lines):
                file.write('// #include <...> (HPPMERGE)\n')
                file.write('\n'.join(self.external_include_lines))
                file.write('\n')
            if len(self.ignored_include_lines):
                file.write('// #include "..." (HPPMERGE)\n')
                file.write('\n'.join(self.ignored_include_lines))
                file.write('\n')
            file.write('\n'.join(self.lines))

# main
if __name__ == "__main__":
    # parameters
    if len(sys.argv) < 3 or (sys.argv[1].startswith('--') or sys.argv[2].startswith('--')):
        print(f"usage: python3 {sys.argv[0]} ORIGIN_FILEPATH DEST_FILEPATH --default DEFAULT1 DEFAULT2 ... --ignore IGNORE1 IGNORE2 ...")
        sys.exit(1)
    for arg in sys.argv[1:]:
        if arg.startswith('--') and arg not in [ '--default', '--ignore']:
            print(f'error: unknown option "{arg}"')
            sys.exit(1)
    # :: origin_filepath
    origin_filepath = sys.argv[1]
    if not path.exists(origin_filepath):
        print(f'error: origin file "{origin_filepath}" not found')
        sys.exit(1)
    # :: dest_filepath
    dest_filepath = sys.argv[2]
    # :: default_includes
    default_includes = []
    if '--default' in sys.argv:
        for index in range(sys.argv.index('--default') + 1, len(sys.argv)):
            if sys.argv[index].startswith('--'):
                break
            include_path = sys.argv[index] if sys.argv[index][-1] != '/' else sys.argv[index][:-1]
            default_includes.append(include_path)
    # : ignore_includes
    ignore_includes = []
    if '--ignore' in sys.argv:
        for index in range(sys.argv.index('--ignore') + 1, len(sys.argv)):
            if sys.argv[index].startswith('--'):
                break
            include_path = sys.argv[index] if sys.argv[index][-1] != '/' else sys.argv[index][:-1]
            ignore_includes.append(include_path)
    print(origin_filepath)
    print(dest_filepath)
    print(default_includes)
    print(ignore_includes)

    # hppmerge
    mergefile = HPPMergeFile(origin_filepath, default_includes, ignore_includes)
    # :: write
    mergefile.write(dest_filepath)
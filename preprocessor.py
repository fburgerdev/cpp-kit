def is_pragma_once(line: str) -> bool:
    return line.strip().startswith("#pragma") and line[line.index("#pragma") + len("#pragma"):].strip().startswith("once")
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
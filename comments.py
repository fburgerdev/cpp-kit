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
def find_substring(x: str, prefix: str, min_middle_len: int, f: list[str]) -> list[str]:
    x_index = 0
    result = []
    
    while x_index < len(x) - 3:
        x_index_begin = x_index
        if x[x_index:x_index + 3] == prefix:
            x_index += 3
            if x_index >= len(x) - min_middle_len or prefix in x[x_index:x_index+min_middle_len]:
                continue
            for f_str in f:
                if f_str in x[x_index:x_index+min_middle_len]:
                    continue
            x_index += min_middle_len
            while x_index <= len(x) - 3 and x[x_index:x_index+3] not in f and x[x_index:x_index+3] != prefix:
                x_index += 1
            if x_index > len(x) - 3:
                continue
            elif x[x_index:x_index+3] == prefix:
                continue
            else: # x[x_index:x_index+3] in f
                result.append(x_index_begin)
        else:
            x_index += 1
            continue

    return result

if __name__ == "__main__":
    prefix = "ATG"
    min_middle_len = 30
    f = ["TAA", "TAG", "TGA"]

    x = "ATGACCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCATGACCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCTAA"
    result = find_substring(x, prefix, min_middle_len, f)
    print(result)

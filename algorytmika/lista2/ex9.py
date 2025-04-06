def find_subsequence(x: str, prefix: str, min_middle_len: int, f: list[str]) -> list[str]:
    x_index = 0
    last_start_index = -1
    matched_prefix = False
    middle_part_len = 0
    result = []
    
    while x_index <= len(x) - 3:
        if not matched_prefix:
            if x[x_index:x_index + 3] == prefix:
                matched_prefix = True
                middle_part_len = 0
                last_start_index = x_index
                x_index += 3
                continue
            else:
                x_index += 1
                continue

        if matched_prefix and middle_part_len < min_middle_len:
            if x[x_index:x_index + 3] not in f and x[x_index:x_index + 3] != prefix:
                if middle_part_len + 3 == min_middle_len:
                    middle_part_len = min_middle_len
                    x_index += 3
                    continue
                else:
                    middle_part_len += 1
                    x_index += 1
                    continue
            elif x[x_index:x_index + 3] in f:
                matched_prefix = False
                middle_part_len = 0
                last_start_index = -1
                continue
            else: # x[x_index:x_index + 3] == prefix
                middle_part_len = 0
                last_start_index = x_index
                x_index += 3
                continue

        if matched_prefix and middle_part_len == min_middle_len:
            if x[x_index:x_index + 3] in f:
                result.append(x[last_start_index:x_index + 3])
                matched_prefix = False
                middle_part_len = 0
                last_start_index = -1
                x_index += 1
                continue
            elif x[x_index:x_index + 3] == prefix:
                matched_prefix = True
                middle_part_len = 0
                last_start_index = x_index
                x_index += 3
                continue
            else:
                x_index += 1
                continue

    return result

if __name__ == "__main__":
    prefix = "ATG"
    min_middle_len = 5
    f = ["TAA", "TAG", "TGA"]

    x = "CCATGAAAAAATAATGATGAAAAATAAA"
    result = find_subsequence(x, prefix, min_middle_len, f)
    print(result)

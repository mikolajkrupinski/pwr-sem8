def common_sufix_len(str1: str, str2: str) -> int:
    str_min_len = min(len(str1), len(str2))

    k_max = 0
    for k in range(1, str_min_len + 1):
        if str1[:k] == str2[-k:]:
            k_max = k

    return k_max

if __name__ == "__main__":
    print(common_sufix_len("kayak", "kayak"))   # 5
    print(common_sufix_len("abcdef", ""))       # 0
    print(common_sufix_len("abcdef", "xyz"))    # 0
    print(common_sufix_len("abcd", "xyzab"))    # 2

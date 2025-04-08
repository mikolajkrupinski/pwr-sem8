P = 17321
ALPHABET_SIZE = 256

def hash(s:str) -> int:
    result = 0
    for c in s:
        result = ((result * ALPHABET_SIZE) % P + ord(c)) % P
    return result

def update_hash(old_s: str, old_hash: int, new_char: str, q_k1: float) -> int:
    return (
        (old_hash - ord(old_s[0]) * q_k1) % P * ALPHABET_SIZE % P + ord(new_char)) % P

def rabin_karp(x: str, p: str) -> list[int]:
    n = len(x)
    k = len(p)
    hp = hash(p)
    q_k1 = pow(ALPHABET_SIZE, k - 1, P)

    pattern_indices = []
    hx = hash(x[:k])
    for i in range(0, n - k + 1):
        if hx == hp:
            if x[i:i+k] == p:
                pattern_indices.append(i)
        if i + k < n:
            hx = update_hash(x[i:i+k], hx, x[i+k], q_k1)
    return pattern_indices

if __name__ == "__main__":
    x = "ababababa"
    p = "ababa"
    result = rabin_karp(x, p)
    print(x)
    print()
    for s in result:
        print(" " * s + p)
        print(" " * s + "^")
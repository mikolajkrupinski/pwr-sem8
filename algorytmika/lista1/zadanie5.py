import sys

def computeTransition(pattern: str, alphabet: list):
    m = len(pattern)
    transition = [{} for _ in range(m + 1)]
    
    for q in range(m + 1):
        for a in alphabet:
            k = min(m, q + 1)
            while k > 0 and not (pattern[:q] + a).endswith(pattern[:k]):
                k -= 1
            transition[q][a] = k
    
    return transition


def FAMatcher(text: str, transition: list, patternLen: int):
    n = len(text)

    q = 0
    for i in range(n):
        q = 0 if text[i] not in transition[q].keys() else transition[q][text[i]]
        if q == patternLen:
            print(f'Pattern found with offset {i - patternLen + 1}')

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('First argument: pattern, Second argument: filename where to find the pattern')
        exit(1)
    
    with open(sys.argv[2], 'r') as inputFile:
        text = inputFile.read()

    alphabet = []
    for ch in sys.argv[1]:
        if ch not in alphabet:
            alphabet += ch
    transition = computeTransition(sys.argv[1], alphabet)
    FAMatcher(text, transition, len(sys.argv[1]))

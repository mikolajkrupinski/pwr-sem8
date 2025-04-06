def lcs(seq1: str, seq2: str, seq3: str) -> list[str]:
    m, n, o = len(seq1), len(seq2), len(seq3)
    dp = [[[0] * (o + 1) for _ in range(n + 1)] for _ in range(m + 1)]

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            for k in range(1, o + 1):
                if seq1[i - 1] == seq2[j - 1] == seq3[k - 1]:
                    dp[i][j][k] = dp[i - 1][j - 1][k - 1] + 1
                else:
                    dp[i][j][k] = max(dp[i - 1][j][k], dp[i][j - 1][k], dp[i][j][k - 1])

    lcs_length = dp[m][n][o]
    subsequences = set()

    def backtrack(i, j, k, current):
        if len(current) == lcs_length:
            subsequences.add("".join(reversed(current)))
            return
        if i == 0 or j == 0 or k == 0:
            return
        if seq1[i - 1] == seq2[j - 1] == seq3[k - 1]:
            backtrack(i - 1, j - 1, k - 1, current + [seq1[i - 1]])
        else:
            if dp[i - 1][j][k] == dp[i][j][k]:
                backtrack(i - 1, j, k, current)
            if dp[i][j - 1][k] == dp[i][j][k]:
                backtrack(i, j - 1, k, current)
            if dp[i][j][k - 1] == dp[i][j][k]:
                backtrack(i, j, k - 1, current)

    backtrack(m, n, o, [])
    return list(subsequences)

if __name__ == "__main__":
    seq1 = "axbcdeyz"
    seq2 = "xaceyz"
    seq3 = "xyzabce"
    result = lcs(seq1, seq2, seq3)
    print(f"The longest common subsequences of '{seq1}', '{seq2}', and '{seq3}' are: {result}")
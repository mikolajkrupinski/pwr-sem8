def next_sequence(seq: list[int], seq_len: int) -> tuple[list[int], bool]:
    if (len(seq) != seq_len):
        raise ValueError("The sequence length must be equal to the length of the sequence.")
    i = seq_len - 1
    while (i >= 0 and seq[i] == 1):
        seq[i] = 0
        i -= 1
    if (i >= 0):
        seq[i] = 1
    return (seq, not i >= 0)

def lcs(seq1: list[int], seq2: list[int]) -> int:
    m, n = len(seq1), len(seq2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if seq1[i - 1] == seq2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

    return dp[m][n]

if __name__ == "__main__":
    seq_len = 5
    sum_lcs = 0
    seq1 = [0] * seq_len
    last_seq1 = False
    while not last_seq1:
        seq2 = [0] * seq_len
        last_seq2 = False
        while not last_seq2:
            sum_lcs += lcs(seq1, seq2)
            seq2, last_seq2 = next_sequence(seq2, seq_len)
        seq1, last_seq1 = next_sequence(seq1, seq_len)
    print(f"Expected value of LCS for binary sequences of length {seq_len} is {sum_lcs / ((2 ** seq_len) ** 2)}")

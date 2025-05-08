def gen_0_seq(n: int) -> list:
    x = []
    for i in range(0, n):
        exp_if_xi_0 = 2 ** (-n + i - 1)
        exp_if_xi_1 = 0
        if exp_if_xi_0 > exp_if_xi_1:
            x.append(0)
        else:
            x.append(0)
    return x

print(gen_0_seq(5))
from matplotlib import pyplot as plt
import numpy as np

def pol_value(coeffs: list[float], x: float) -> tuple[float, int]:
    result = 0
    no_of_multiplications = 0
    for i, c in enumerate(reversed(coeffs)):
        result += c * (x ** i)
        no_of_multiplications += i
    return (result, no_of_multiplications)

def horner(coeffs: list[float], x: float) -> tuple[float, int]:
    result = 0
    no_of_multiplications = 0
    for coeff in coeffs:
        result = result * x + coeff
        no_of_multiplications += 1
    return (result, no_of_multiplications)

if __name__ == "__main__":
    MAX_DEGREE = 1000
    degrees = [i for i in range(1, MAX_DEGREE + 1)]
    no_of_multiplications_horner = []
    no_of_multiplications_naive = []
    for i in range(1, MAX_DEGREE + 1):
        coeffs = [1.0] * i
        _, multiplications_horner = horner(coeffs, 1)
        no_of_multiplications_horner.append(multiplications_horner)
        _, multiplications_naive = pol_value(coeffs, 1)
        no_of_multiplications_naive.append(multiplications_naive)

    plt.title("Horner vs Naive")
    plt.plot(degrees, no_of_multiplications_horner, label='Horner')
    plt.plot(degrees, no_of_multiplications_naive, label='Naive')
    plt.ylabel("Number of multiplications")
    plt.xlabel("Polynomial degree")
    plt.legend()
    plt.show()

    plt.title("Horner Complexity")
    plt.plot(degrees, np.array(no_of_multiplications_horner) / np.array(degrees))
    plt.ylabel("Number of multiplications / degrees")
    plt.xlabel("Polynomial degree")
    plt.show()

    plt.title("Naive Complexity")
    plt.plot(degrees, np.array(no_of_multiplications_naive) / (np.array(degrees) ** 2))
    plt.ylabel("Number of multiplications / degrees^2")
    plt.xlabel("Polynomial degree")
    plt.show()
        

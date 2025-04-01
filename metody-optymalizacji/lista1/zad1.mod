param n, integer, >= 1; # Wymiar macierzy Hilberta
set N := {1..n}; # Zbiór indeksów
param A{i in N, j in N} := 1 / (i + j - 1); # Macierz Hilberta
param b{i in N} := sum{j in N} A[i, j]; # Wektor prawych stron
param expected_x{i in N} := 1; # Oczekiwany wektor rozwiązań

var x{N} >= 0; # Wektor zmiennych decyzyjnych

minimize objective_function: # Funkcja celu
    sum{i in N} (b[i] * x[i]);

s.t. Ax_equal_b {i in N}: # Ograniczenie na Ax=b
    sum{j in N} (A[i, j] * x[j]) = b[i];

solve;

printf "x = [ ";
for {i in N} {
    printf "%.15f ", x[i].val;
}
printf "]\n";

printf "Relative error = %.15f\n", sqrt(sum{i in N} (x[i] - expected_x[i])^2) / sqrt(sum{i in N} expected_x[i]^2);

end;

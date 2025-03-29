/* Data declaration */

param n, integer, >= 1;
set N := {1..n};
param A{i in N, j in N} := 1 / (i + j - 1);
param b{i in N} := sum{j in N} A[i, j];
param expected_x{i in N} := 1;

/* Decision variables */

var x{N} >= 0;

/* Objective function */
minimize objective_function:
    sum{i in N} (b[i] * x[i]);

/* Constraints */

s.t. Ax_equal_b {i in N}:
    sum{j in N} (A[i, j] * x[j]) = b[i];

/* Solve and display data */

solve;

printf "x = [ ";
for {i in N} {
    printf "%.15f ", x[i].val;
}
printf "]\n";

printf "Relative error = %.15f\n", sqrt(sum{i in N} (x[i] - expected_x[i])^2) / sqrt(sum{i in N} expected_x[i]^2);

end;

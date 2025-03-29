/* Data declaration */

set cities;
param distance{cities, cities} >= 0;
param supply1{cities} >= 0;
param supply2{cities} >= 0;
param demand1{cities} >= 0;
param demand2{cities} >= 0;

/* Decision variables */

var transport1{cities, cities} >= 0;
var transport2{cities, cities} >= 0;

/* Objective function */

minimize transport_cost:
    sum{A in cities, B in cities} (transport1[A, B] * distance[A, B] + 1.2 * transport2[A, B] * distance[A, B]);

/* Constraints */

s.t. demand1_constraint {A in cities}:
    sum{B in cities} (transport1[B, A] + transport2[B, A]) >= demand1[A] + demand2[A];
s.t. supply1_constraint {A in cities}:
    sum{B in cities} transport1[A, B] <= supply1[A];
s.t. demand2_constraint {A in cities}:
    sum{B in cities} transport2[B, A] >= demand2[A];
s.t. supply2_constraint {A in cities}:
    sum{B in cities} transport2[A, B] <= supply2[A];

/* Solve and display data */

solve;

for {A in cities}: {
    for {B in cities : transport1[A, B] > 0}: {
        printf "1 %s -> %s : %d\n", A, B, transport1[A, B];
    }
    for {B in cities : transport2[A, B] > 0}: {
        printf "2 %s -> %s : %d\n", A, B, transport2[A, B];
    }
}

display transport_cost;

end;

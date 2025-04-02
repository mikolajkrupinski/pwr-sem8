# Mikołaj Krupiński, zadanie 2

set cities; # Zbiór miast
param distance{cities, cities} >= 0; # Macierz odległości między miastami
param supply1{cities} >= 0; # Nadmiar dźwigów typu I według miast
param supply2{cities} >= 0; # Nadmiar dźwigów typu II według miast
param demand1{cities} >= 0; # Niedobór dźwigów typu I według miast
param demand2{cities} >= 0; # Niedobór dźwigów typu II według miast

var transport1{cities, cities} >= 0; # transport1{A, B} - ilość dźwigów typu I transportowanych z miasta A do B
var transport2{cities, cities} >= 0; # transport2{A, B} - ilość dźwigów typu II transportowanych z miasta A do B

minimize transport_cost: # Sumaryczny koszt transportu wszytskich dźwigów
    sum{A in cities, B in cities} (transport1[A, B] * distance[A, B] + 1.2 * transport2[A, B] * distance[A, B]);

s.t. demand1_constraint {A in cities}: # Zaspokojenie popytu na dźwigi typu I, dźwig typu I może być zastąpiony przez typu II
    sum{B in cities} (transport1[B, A] + transport2[B, A]) >= demand1[A] + demand2[A];
s.t. supply1_constraint {A in cities}: # Nieprzekroczenie podaży dźwigów typu I
    sum{B in cities} transport1[A, B] <= supply1[A];
s.t. demand2_constraint {A in cities}: # Zaspokojenie popytu na dźwigi typu II
    sum{B in cities} transport2[B, A] >= demand2[A];
s.t. supply2_constraint {A in cities}: # Nieprzekroczenie podaży dźwigów typu II
    sum{B in cities} transport2[A, B] <= supply2[A];

solve;

for {A in cities}: {
    for {B in cities : transport1[A, B] > 0}: {
        printf "1 %s -> %s : %d\n", A, B, transport1[A, B];
    }
    for {B in cities : transport2[A, B] > 0}: {
        printf "2 %s -> %s : %d\n", A, B, transport2[A, B];
    }
}

printf "Total transport cost : %f\n", transport_cost.val;

end;

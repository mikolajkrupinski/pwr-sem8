using JuMP
using Cbc

p = 1 # liczba zasobów
N = [30] # limit zasobów
n = 8 # liczba czynności
ts = [50, 47, 55, 46, 32, 57, 15, 62] # czasy wykonania czynności
r = [[9], [17], [11], [4], [13], [7], [7], [17]] # zapotrzebowanie na zasoby
pred = [(1, 2), (1, 3), (1, 4), (2, 5), (3, 6), (4, 6), (4, 7), (5, 8), (6, 8), (7, 8)] # zależności między czynnościami

T = sum(ts) + 1

model = Model(Cbc.Optimizer)

@variable(model, x[1:n, 1:T], Bin) # 1 iff task i starts at time j
@variable(model, c_max >= 0, Int)

@constraint(model, [i in 1:n], sum(x[i, t] for t in 1:T) == 1)

@constraint(model, [j in 1:p, t in 1:T], sum(r[i][j] * x[i, s] for i in 1:n, s in max(1, t + 1 - ts[i]):t) <= N[j])

@constraint(model, [(i, j) in pred], sum((t + ts[i] - 1) * x[i, t] for t in 1:T+1-ts[i]) <= sum((t - 1) * x[j, t] for t in 1:T+1-ts[j]))

@constraint(model, [i in 1:n, t in 1:T], c_max >= (t + ts[i] - 1) * x[i, t])

@objective(model, Min, c_max)

optimize!(model)

println("C_max: ", objective_value(model))
println()

for t in 1:T
    for i in 1:n
        if value(x[i, t]) == 1
            println("Task ", i, " start at time ", t, " and ends at time ", t + ts[i] - 1, " using resources ", r[i])
        end
    end
end

println()

for t in 1:T
    if sum(value(x[i, s]) for i in 1:n, s in max(1, t - maximum(ts) + 1):T) == 0
        return
    end

    print("time = ", t, " : ")
    current_resources = zeros(Int, p)
    for i in 1:n
        if sum(value(x[i, s]) for s in max(1, t + 1 - ts[i]):t) > 0
            print(" (", i, ", ", r[i], ") ")
            for j in 1:p
                current_resources[j] += r[i][j]
            end
        end
    end
    println(" <- ", current_resources)
end

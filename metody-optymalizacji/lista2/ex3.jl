using JuMP
using GLPK

n = 9 # liczba zadań
m = 3 # liczba maszyn
p = [1, 2, 1, 2, 1, 1, 3, 6, 2] # czasy przetwarzania
pred = [(1, 4), (2, 4), (3, 4), (2, 5), (3, 5), (4, 6), (4, 7), (5, 7), (5, 8), (6, 9), (7, 9)] # kolejność zadań

T = sum(p)

model = Model(GLPK.Optimizer)

@variable(model, x[1:n, 1:m, 1:T], Bin) # 1 iff zadanie i zaczyna się na maszynie j w czasie k
@variable(model, c_max >= 0, Int)

@constraint(model, [i in 1:n], sum(x[i, j, t] for j in 1:m, t in 1:T) == 1)

@constraint(model, [j in 1:m, t in 1:T], sum(x[i, j, s] for i in 1:n, s in max(1, t + 1 - p[i]):t) <= 1)

@constraint(model, [(i, k) in pred], sum((t + p[i] - 1) * x[i, j, t] for j in 1:m, t in 1:T+1-p[i]) <= sum((t - 1) * x[k, j, t] for j in 1:m, t in 1:T+1-p[i]))

@constraint(model, [i in 1:n, j in 1:m, t in 1:T], c_max >= (t + p[i] - 1) * x[i, j, t])

@objective(model, Min, c_max)

optimize!(model)

println("C_max: ", objective_value(model))
println()

for j in 1:m
    print("M", j, ": ")
    for t in 1:T
        for i in 1:n
            if value(x[i, j, t]) == 1
                print("( ", i, " : ", t, " - ", t + p[i] - 1, " )")
            end
        end
    end
    println()
end

println()

for j in 1:m
    print("M", j, ": ")
    for t in 1:T
        time_slot_used = false
        for i in 1:n
            if sum(value(x[i, j, s]) for s in max(1, t + 1 - p[i]):t) > 0
                print(i)
                time_slot_used = true
                break
            end
        end
        if !time_slot_used
            print(" ")
        end
    end
    println()
end

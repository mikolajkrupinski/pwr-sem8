# Mikołaj Krupiński

using JuMP
using GLPK
using JSON

if length(ARGS) != 1
    println("Usage: julia ex3.jl <data_file>")
    exit(1)
end

data_file = ARGS[1]
data = JSON.parsefile(data_file)

n = data["no_of_tasks"]
m = data["no_of_machines"]
p = data["task_times"]
pred = data["task_predescence"]

T = sum(p) + 1

model = Model(GLPK.Optimizer)

# x[i, j, t] == 1 iff zadanie i rozpoczyna się na maszynie j w czasie t
@variable(model, x[1:n, 1:m, 1:T], Bin)
# czas zakończenia ostatniego zadania
@variable(model, c_max >= 0, Int)

# każde zadanie musi zostać wykonane dokładnie raz na jednej z maszyn
@constraint(model, [i in 1:n], sum(x[i, j, t] for j in 1:m, t in 1:T) == 1)

# zadania na jednej maszynie nie mogą być wykonywane równocześnie
@constraint(model, [j in 1:m, t in 1:T], sum(x[i, j, s] for i in 1:n, s in max(1, t + 1 - p[i]):t) <= 1)

# zadanie musi zostać wykonane po wykonaniu zadań je poprzedzających według ustalonego porządku
@constraint(model, [(i, k) in pred], sum((t + p[i] - 1) * x[i, j, t] for j in 1:m, t in 1:T+1-p[i]) <= sum((t - 1) * x[k, j, t] for j in 1:m, t in 1:T+1-p[i]))

# c_max jest nie mniejszy niż czas zakończenia każdego zadania
@constraint(model, [i in 1:n, j in 1:m, t in 1:T], c_max >= (t + p[i] - 1) * x[i, j, t])

# c_max jest równy czasowi zakończenia ostatniego zadania
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

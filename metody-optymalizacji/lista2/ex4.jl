# Mikołaj Krupiński

using JuMP
using Cbc
using JSON

if length(ARGS) != 1
    println("Usage: julia ex4.jl <data_file>")
    exit(1)
end

data_file = ARGS[1]
data = JSON.parsefile(data_file)

p = data["no_of_resources"]
N = data["resource_limits"]
n = data["no_of_tasks"]
ts = data["task_times"]
r = data["resource_requirements"]
pred = data["task_predescence"]

T = sum(ts) + 1

model = Model(Cbc.Optimizer)

# x[i, t] == 1 iff zadanie i rozpoczyna się w czasie t
@variable(model, x[1:n, 1:T], Bin)
# c_max - czas zakończenia ostatniego zadania
@variable(model, c_max >= 0, Int)

# każde zadasnie musi zostać wykonane dokładnie raz
@constraint(model, [i in 1:n], sum(x[i, t] for t in 1:T) == 1)

# w każdej chwili nie może zostać przekroczona podaż zasobów
@constraint(model, [j in 1:p, t in 1:T], sum(r[i][j] * x[i, s] for i in 1:n, s in max(1, t + 1 - ts[i]):t) <= N[j])

# muszą być spełnione ograniczenia dotyczące kolejności zadań
@constraint(model, [(i, j) in pred], sum((t + ts[i] - 1) * x[i, t] for t in 1:T+1-ts[i]) <= sum((t - 1) * x[j, t] for t in 1:T+1-ts[j]))

# c_max jest nie mniejszy niż czas zakończenia każdego zadania
@constraint(model, [i in 1:n, t in 1:T], c_max >= (t + ts[i] - 1) * x[i, t])

# c_max jest równy czasowi zakończenia ostatniego zadania
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

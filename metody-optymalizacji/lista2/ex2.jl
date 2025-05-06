# Mikołaj Krupiński

using JuMP
using GLPK
using JSON

if length(ARGS) != 1
    println("Usage: julia ex2.jl <data_file>")
    exit(1)
end

data_file = ARGS[1]
data = JSON.parsefile(data_file)

no_of_tasks = data["no_of_tasks"]
times = data["task_times"]
weights = data["task_weights"]
earliest_start = data["task_earliest_start"]

max_time = maximum(earliest_start) + sum(times) + 1

model = Model(GLPK.Optimizer)

# x[i, j] == 1 iff zadanie i rozpoczyna się w czasie j
@variable(model, x[1:no_of_tasks, 1:max_time], Bin)

# każde zadanie musi zostać wykonane dokładnie raz
@constraint(model, [i in 1:no_of_tasks], sum(x[i, j] for j in 1:max_time) == 1)

# zadanie nie może zostać wykonanie przed swoim najwcześniejszym czasem rozpoczęcia
@constraint(model, [i in 1:no_of_tasks], sum(x[i, j] for j in 1:earliest_start[i]-1) == 0)

# zadania nie mogą być wykonywane równocześnie
@constraint(model, [i in 1:max_time], sum(x[j, k] for j in 1:no_of_tasks, k in max(1, i + 1 - times[j]):i) <= 1)

# suma ważona czasów zakończenia zadań
@objective(model, Min, sum(x[i, j] * weights[i] * (j + times[i] - 1) for i in 1:no_of_tasks, j in 1:max_time))

optimize!(model)

println("Objective value: ", objective_value(model))
for i in 1:no_of_tasks
    for j in 1:max_time
        if value(x[i, j]) > 0
            println("Task ", i, " : ( ", j, " ", j + times[i] - 1, " )")
        end
    end
end

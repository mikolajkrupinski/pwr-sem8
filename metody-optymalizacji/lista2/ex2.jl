using JuMP
using GLPK

no_of_tasks = 3
times = [1, 10, 8]
weights = [2, 50, 1]
earliest_start = [1, 1, 1]

max_time = maximum(earliest_start) + sum(times) + 1

model = Model(GLPK.Optimizer)

@variable(model, x[1:no_of_tasks, 1:max_time], Bin) # 1 iff zadanie i rozpoczyna się w czasie j

@constraint(model, [i in 1:no_of_tasks], sum(x[i, j] for j in 1:max_time) == 1)

@constraint(model, [i in 1:no_of_tasks], sum(x[i, j] for j in 1:earliest_start[i]-1) == 0)

@constraint(model, [i in 1:max_time], sum(x[j, k] for j in 1:no_of_tasks, k in max(1, i + 1 - times[j]):i) <= 1)

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

using GLPK
using JuMP

function find_greedy_schedule_cost(J::Int64, M::Int64, p::Vector{Vector{Int64}})
    makespan = zeros(Int, M)
    for j in 1:J
        min_cost = p[1][j]
        min_machine = 1
        for i in 2:M
            if p[i][j] < min_cost
                min_cost = p[i][j]
                min_machine = i
            end
        end
        makespan[min_machine] += p[min_machine][j]
    end
    return maximum(makespan)
end

function create_model(J::Int64, M::Int64, p::Vector{Vector{Int64}}, T::Int64)
    model = Model(GLPK.Optimizer)

    @variable(model, x[1:M, 1:J] >= 0)
    @objective(model, Min, 0)

    @constraint(model, sum(x[i, j] for i in 1:M, j in 1:J if p[i][j] > T) == 0)
    @constraint(model, [j in 1:J], sum(x[i, j] for i in 1:M if p[i][j] <= T) == 1)
    @constraint(model, [i in 1:M], sum(x[i, j] for j in 1:J if p[i][j] <= T) <= T)

    return model
end

function is_model_solveable(model::Model)
    optimize!(model)
    return termination_status(model) == MOI.OPTIMAL
end

function get_model_solution(model::Model)
    optimize!(model)
    return value.(model[:x])
end

function binary_search_T(J::Int64, M::Int64, p::Vector{Vector{Int64}}, low::Int64, high::Int64)
    while low < high
        mid = div(low + high, 2)
        if is_model_solveable(create_model(J, M, p, mid))
            high = mid
        else
            low = mid + 1
        end
    end
    return low
end

function scheduling_approximation(J::Int64, M::Int64, p::Vector{Vector{Int64}})
    # Find an extreme point solution for the lowest possible lower bound for timespan T
    greedy_cost = find_greedy_schedule_cost(J, M, p)
    smallest_T = binary_search_T(J, M, p, div(greedy_cost, M), greedy_cost)
    x = get_model_solution(create_model(J, M, p, smallest_T))

    # Assign all integrally set jobs to machines
    assignment = zeros(Int, J)
    for j in 1:J
        for i in 1:M
            if x[i, j] == 1
                assignment[j] = i
                break
            end
        end
    end

    # Construct graph H for non-integraly set jobs
    H = zeros(Int, M, J)
    for j in 1:J
        if assignment[j] == 0
            for i in 1:M
                if x[i, j] > 0
                    H[i, j] = 1
                end
            end
        end
    end

    # Find perfect matching in graph H
    for i in 1:M
        if sum(H[i, :]) == 1
            for j in 1:J
                if H[i, j] == 1
                    assignment[j] = i
                    H[:, j] = 0
                    break
                end
            end
        end
    end
    for j in 1:J
        if assignment[j] == 0
            for i in 1:M
                if H[i, j] == 1
                    assignment[j] = i
                    H[:, j] = 0
                    break
                end
            end
        end
    end

    # Calculate the makespan of the schedule
    makespan = zeros(Int, M)
    for j in 1:J
        makespan[assignment[j]] += p[assignment[j]][j]
    end
    return maximum(makespan)
end


# Example usage
J = 5  # Number of jobs
M = 3  # Number of machines
p = [
    [2, 3, 4, 5, 6],  # Processing times for machine 1
    [3, 2, 5, 4, 6],  # Processing times for machine 2
    [4, 5, 2, 3, 6]   # Processing times for machine 3
]
T = scheduling_approximation(J, M, p)
println("Approximate makespan T: ", T)

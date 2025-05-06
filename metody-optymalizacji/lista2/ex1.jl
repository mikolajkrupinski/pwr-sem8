# Mikołaj Krupiński

using JuMP
using GLPK
using JSON

if length(ARGS) != 1
    println("Usage: julia ex1.jl <data_file>")
    exit(1)
end

data_file = ARGS[1]
data = JSON.parsefile(data_file)

plank_width = data["plank_width"]
ordered_planks = Dict{Int,Int}(parse(Int, k) => v for (k, v) in data["ordered_planks"])

ordered_planks_widths = sort(collect(keys(ordered_planks)), rev=true)
ordered_planks_demand = [ordered_planks[w] for w in ordered_planks_widths]
no_of_ordered_planks_types = length(ordered_planks_widths)

function generate_cuttings()
    methods = Vector{Vector{Int}}()

    function backtrack(remaining_width::Int, current_cutting::Vector{Int}, start_index::Int)
        already_added = false
        for i in start_index:length(ordered_planks_widths)
            if remaining_width < ordered_planks_widths[i]
                if !already_added
                    already_added = true
                    push!(methods, copy(current_cutting))
                end
            else
                new_cutting = copy(current_cutting)
                new_cutting[i] += 1
                backtrack(remaining_width - ordered_planks_widths[i], new_cutting, i)
            end
        end
    end

    backtrack(plank_width, zeros(Int, no_of_ordered_planks_types), 1)
    return methods
end

cutting_methods = generate_cuttings()
no_of_cutting_methods = length(cutting_methods)

println("Generated cutting methods:")
for method in cutting_methods
    println(method)
end

model = Model(GLPK.Optimizer)

# x[i] - liczba desek do cięcia metodą i
@variable(model, x[1:no_of_cutting_methods] >= 0, Int)

# zaspokojenie popytu na deski każdej szerokości
@constraint(model, [i in 1:no_of_ordered_planks_types], sum(x[j] * cutting_methods[j][i] for j in 1:no_of_cutting_methods) >= ordered_planks_demand[i])

# suma szerokości desek odpadów
@objective(model, Min, plank_width * sum(x[i] for i in 1:no_of_cutting_methods) - sum(ordered_planks_widths[i] * ordered_planks_demand[i] for i in 1:no_of_ordered_planks_types))

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("Optimal solution found")
    println("Total length of wasted plank: ", objective_value(model))
    for i in 1:no_of_cutting_methods
        if value(x[i]) > 0
            print("Cutting method [ ")
            for j in 1:no_of_ordered_planks_types
                for k in 1:cutting_methods[i][j]
                    print(ordered_planks_widths[j], " ")
                end
            end
            println("] : ", value(x[i]), " times")
        end
    end
else
    println("No optimal solution found")
end
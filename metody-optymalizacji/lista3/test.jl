# Mikołaj Krupiński

include("schedule_approximation.jl")

function read_data_from_file(filename::String)
    open(filename, "r") do file
        # Read the first line to get the number of jobs and machines
        first_line = readline(file)
        J, M, _ = split(first_line) |> x -> (parse(Int, x[1]), parse(Int, x[2]), parse(Int, x[3]))
        # Read the second line to discard it
        readline(file)

        # Read the processing times matrix
        p = zeros(Int, M, J)
        for j in 1:J
            line = readline(file)
            values = split(line) |> x -> parse.(Int, x)
            p[:, j] = values[2:2:end]
        end

        return J, M, p
    end
end

if length(ARGS) > 0
    filename = ARGS[1]
    J, M, p = read_data_from_file(filename)
    T = scheduling_approximation(J, M, p)
    println(T)
else
    println(stderr, "Please provide a filename as a console argument.")
end
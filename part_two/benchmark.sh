#!/bin/bash

# Numbers to test
numbers=(5 10 20 50 100 200)

# Create results directory if it doesn't exist
mkdir -p results

# Run tests for each number
for nr in "${numbers[@]}"; do
    echo "Testing random_${nr}.dat..."

    sum_fitness=0
    results_file="results/random_${nr}_results.txt"
    echo "Results for random_${nr}.dat:" > "$results_file"

    # Run 5 times for each instance
    for run in {1..5}; do
        echo "  Run $run of 5..."
        output_file="results/random_${nr}_run_${run}.txt"

        # Run the main program and save output
        ./main ./samples/random_${nr}.dat > "$output_file"

        # Extract the line containing "Fitness:" and get the value
        fitness_line=$(grep "^Fitness:" "$output_file")
        fitness=${fitness_line#*: }
        
        if [[ -n "$fitness" ]]; then
            echo "  Run $run fitness: $fitness"
            sum_fitness=$(echo "$sum_fitness + $fitness" | bc -l)
            echo "Run $run fitness: $fitness" >> "$results_file"
        else
            echo "  Warning: Could not extract fitness from run $run"
        fi

        # On first run, extract the configuration
        if [[ $run -eq 1 ]]; then
            # Extract configuration - everything after "Fitness:" line until end of file
            config=$(sed -n '/^Configuration:/,$p' "$output_file" | grep -v "Saving solution to file")
            echo -e "\nConfiguration:" >> "$results_file"
            echo "$config" >> "$results_file"
        fi
    done

    # Calculate average fitness
    avg_fitness=$(echo "scale=6; $sum_fitness / 5" | bc -l)

    # Save average fitness to results file
    echo -e "\nAverage Fitness: $avg_fitness" >> "$results_file"
    echo "  Average fitness: $avg_fitness"
    echo "  Results saved to $results_file"
    echo ""
done

echo "All tests completed!"

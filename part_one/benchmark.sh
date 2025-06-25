#!/bin/bash

# Parameters for the experiment
NUMBERS=(5 10 20 50 100 200 500)
TIMELIMITS=(1 2 5 10 20 30 60, 120, 240)
OUTPUT_DIR="results"
LOG_FILE="${OUTPUT_DIR}/experiment_results.csv"
SUMMARY_FILE="${OUTPUT_DIR}/summary.txt"

# Check if main executable exists
if [ ! -f "./main" ]; then
    echo "Error: main executable not found. Please compile the project first."
    exit 1
fi

# Create output directory if it doesn't exist
mkdir -p $OUTPUT_DIR

# Initialize log file with headers
echo "Instance,TimeLimit,Status,ObjectiveValue,SolutionTime,Solved" > $LOG_FILE

# Initialize arrays to track results
declare -A instance_solved

echo "Starting experiment to find largest solvable instance sizes..."

# Run experiments - going from smaller to larger instances
for num in "${NUMBERS[@]}"; do
    instance_file="./samples/random_${num}.dat"
    
    # Check if instance file exists
    if [ ! -f "$instance_file" ]; then
        echo "Warning: Instance file $instance_file not found. Skipping."
        continue
    fi
    
    echo "Testing instance random_${num}.dat..."
    solved=false
    
    # Try with increasing time limits
    for tl in "${TIMELIMITS[@]}"; do
        # Skip higher time limits if already solved
        if $solved; then
            echo "  Skipping time limit ${tl}s (already solved with lower time limit)"
            echo "random_${num},$tl,skipped,,,true" >> $LOG_FILE
            continue
        fi
        
        echo "  Running with time limit ${tl}s..."
        
        # Run the solver and capture output
        output=$(./main "$instance_file" gg "$tl" 2>&1)
        exit_code=$?
        
        # Parse results - looking for optimal solution indicators
        status=$(echo "$output" | grep -i "status:" | awk '{$1=""; print $0}' | xargs)
        objective=$(echo "$output" | grep -i "objective value:" | awk '{print $3}')
        solution_time=$(echo "$output" | grep -i "solution time:" | awk '{print $3}')
        
        # Save full output to log file
        echo "$output" > "${OUTPUT_DIR}/random_${num}_${tl}.log"
        
        # Check if optimal solution was found (accepting both status formats)
        if [[ "$status" == *"integer optimal solution"* ]] || [[ "$status" == *"integer optimal, tolerance"* ]]; then
            solved=true
            instance_solved[$num]=$tl
            echo "  SUCCESS! Solved optimally in ${solution_time}s with objective ${objective}"
        else
            echo "  Not solved optimally. Status: $status"
        fi
        
        # Record results to CSV
        echo "random_${num},$tl,$status,$objective,$solution_time,$solved" >> $LOG_FILE
    done
    
    echo "Completed testing instance random_${num}.dat"
    echo "----------------------------------------"
    
    # If this instance wasn't solved with the highest time limit, 
    # larger instances likely won't be solved either
    if ! $solved; then
        echo "Instance size $num couldn't be solved with any time limit. Skipping larger instances."
        break
    fi
done

# Generate summary report
echo "Experiment Summary" > $SUMMARY_FILE
echo "=================" >> $SUMMARY_FILE
echo "" >> $SUMMARY_FILE
echo "Results by instance size:" >> $SUMMARY_FILE

for num in "${NUMBERS[@]}"; do
    if [ -v instance_solved[$num] ]; then
        echo "Instance size ${num}: Solved with time limit ${instance_solved[$num]}s" >> $SUMMARY_FILE
    else
        echo "Instance size ${num}: Not solved with any tested time limit" >> $SUMMARY_FILE
    fi
done

echo "" >> $SUMMARY_FILE
echo "Largest solvable instance by time limit:" >> $SUMMARY_FILE

for tl in "${TIMELIMITS[@]}"; do
    largest=0
    for num in "${NUMBERS[@]}"; do
        if [ -v instance_solved[$num] ] && [ ${instance_solved[$num]} -le $tl ] && [ $num -gt $largest ]; then
            largest=$num
        fi
    done
    
    if [ $largest -gt 0 ]; then
        echo "Time limit ${tl}s: ${largest}" >> $SUMMARY_FILE
    else
        echo "Time limit ${tl}s: None solved" >> $SUMMARY_FILE
    fi
done

echo "Experiment completed. Results saved to $LOG_FILE"
echo "Summary available at $SUMMARY_FILE"

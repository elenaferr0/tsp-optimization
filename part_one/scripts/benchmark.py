#!/usr/bin/env python3
import subprocess
import sys
import csv
import os
from datetime import datetime

def run_with_timeout(binary_path, instance_size, timeout_seconds):
    """Run the binary with the given instance size and timeout."""
    try:
        # Run the process and wait for it to complete
        result = subprocess.run(
            [binary_path, f"random_{instance_size}", str(timeout_seconds)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=timeout_seconds,
            text=True
        )
        return True, result.returncode
    except subprocess.TimeoutExpired:
        # Kill any remaining process to ensure clean state
        print(" (timeout)", end="")
        return False, None
    except Exception as e:
        print(f"Error running instance size {instance_size}: {e}")
        return False, None

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} /path/to/binary")
        print(f"Example: {sys.argv[0]} ./bin/main")
        sys.exit(1)

    binary_path = sys.argv[1]

    # Check if binary exists and is executable
    if not os.path.isfile(binary_path) or not os.access(binary_path, os.X_OK):
        print(f"Error: {binary_path} does not exist or is not executable")
        sys.exit(1)

    time_thresholds = [0.1, 0.5, 1, 5, 15, 30, 60, 120, 180, 240]
    instance_sizes = [5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000]

    csv_data = [["Time Threshold (s)", "Max Instance Size"]]

    max_sizes = {}

    print(f"Testing {binary_path} with various instance sizes and time constraints...")
    print("-" * 50)

    for threshold in time_thresholds:
        print(f"Testing time threshold: {threshold} seconds")
        max_size = 0

        for size in sorted(instance_sizes):
            if any(t < threshold and max_sizes.get(t, 0) >= size for t in time_thresholds):
                print(f"  Skipping size {size} (smaller than successful instance at lower threshold)")
                continue

            print(f"  Testing size {size}...", end="", flush=True)
            success, return_code = run_with_timeout(binary_path, size, threshold)

            if success and return_code == 0:
                print(" ✓")
                max_size = size
            else:
                print(" ✗")
                # Break out of the loop - don't test larger instances if this one failed
                break

        max_sizes[threshold] = max_size
        print(f"  Maximum size for {threshold}s: {max_size}")
        print()

        # Add to CSV data
        csv_data.append([threshold, max_size])

    # Write results to CSV
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    csv_filename = f"benchmark_{timestamp}.csv"

    with open(csv_filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(csv_data)

    print(f"Results saved to {csv_filename}")
    print("\nSummary:")
    print("-" * 50)
    print("Time Threshold | Maximum Instance Size")
    print("-" * 50)

    for threshold, max_size in max_sizes.items():
        if threshold >= 60:
            minutes = int(threshold // 60)
            seconds = int(threshold % 60)
            if seconds == 0:
                threshold_str = f"{minutes}min"
            else:
                threshold_str = f"{minutes}m {seconds}s"
        else:
            threshold_str = f"{threshold}s"

        print(f"{threshold_str:14} | {max_size}")

if __name__ == "__main__":
    main()

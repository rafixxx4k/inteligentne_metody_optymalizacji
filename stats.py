import subprocess
import statistics

def run_exe(alg, instance, runs=200):
    """Run main.exe for a specific (alg, instance) combination multiple times."""
    results = []

    for i in range(runs):
        try:
            result = subprocess.run(["./main.exe", alg, f"{instance}.tsp", f"{alg}_{instance}_{i:03d}.out"], capture_output=True, text=True, check=True)
            output = float(result.stdout.strip())  # Convert to float for calculations
            results.append((f"{alg}_{instance}_{i:03d}.out", output))  # Store (index, value)
        except subprocess.CalledProcessError as e:
            print(f"Error running main.exe with {alg} {instance}: {e}")
    
    return results

def compute_statistics(data):
    """Compute required statistics for a list of (index, value) pairs."""
    if not data:
        return None  # Return None if no data is available

    values = [val for _, val in data]  # Extract values for calculations
    min_index, min_value = min(data, key=lambda x: x[1])
    max_index, max_value = max(data, key=lambda x: x[1])

    return {
        "min": min_value,
        "minPosition": min_index,  # Index of the min value
        "max": max_value,
        "maxPosition": max_index,  # Index of the max value
        "average": sum(values) / len(values),
        "standardDeviation": statistics.stdev(values) if len(values) > 1 else 0
    }

if __name__ == "__main__":
    algs = ["greedy", "greedyCycle", "regret2", "regretWeighted"]
    instances = ["kroA200", "kroB200"]
    times = 200

    all_results = {}

    for alg in algs:
        for instance in instances:
            print(f"Running {alg} on {instance} for {times} times...")
            results = run_exe(alg, instance, times)
            stats = compute_statistics(results)
            all_results[(alg, instance)] = stats

    # Print final results
    print("\nStatistics Summary:")
    for (alg, instance), stats in all_results.items():
        print(f"{alg} - {instance}: {stats}")

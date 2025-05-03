import subprocess
import statistics
import numpy as np

def run_exe(create, alg, instance, runs=200):
    """Run main.exe for a specific (alg, instance) combination multiple times."""
    results = []
    improve = []
    time = []
    for i in range(runs):
        try:
            result = subprocess.run(["./main.exe", create, alg, f"{instance}.tsp", f"{alg}_{instance}_{create}{i:03d}.out"], capture_output=True, text=True, check=True)
            output = result.stdout.strip().split()  # Convert to float for calculations
            results.append((f"{alg}_{instance}_{create}{i:03d}.out", float(output[1])))  # Store (index, value)
            # improve.append(float(output[1]) - float(output[0]))  
            improve.append(float(output[0]))  # Store improvement value
            time.append(float(output[2]))
        except subprocess.CalledProcessError as e:
            print(f"Error running main.exe with {alg} {instance}: {e}")
    
    return results, improve, time

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
    create = ["none"] # "regretWeighted" ,
    algs = [ "ILS"] # ,"localSteepVert", "localGreedyVert", "localSteepEdge","localGreedyEdge"
    instances = ["kroA200", "kroB200"]
    times = 10

    all_results = {}
    for c in create:
        for alg in algs:
            for instance in instances:
                print(f"Running {alg} on {instance} for {times} times...")
                results, improve, time = run_exe(c, alg, instance, times)
                stats = compute_statistics(results)
                stats["improve"] = float(np.mean(improve))
                stats["time"] = float(np.mean(time))
                print(f"{c} -> {alg} - {instance}: {stats}")


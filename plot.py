import matplotlib.pyplot as plt

def parse_file(filename):
    with open(filename, "r") as file:
        lines = file.readlines()

    # Convert the coordinates to integers and create tuples
    group1 = [tuple(map(int, x.strip("()").split(";"))) for x in lines[2].split()]
    group2 = [tuple(map(int, x.strip("()").split(";"))) for x in lines[4].split()]

    return group1, group2

def plot_groups(group1, group2):
    # Create the plot
    plt.figure(figsize=(8, 6))

    # Plot lines between consecutive points in group 1
    for i in range(len(group1) - 1):
        x1, y1 = group1[i]
        x2, y2 = group1[i + 1]
        plt.plot([x1, x2], [y1, y2], color='blue')  # Line between consecutive points in group 1

    # Plot lines between consecutive points in group 2
    for i in range(len(group2) - 1):
        x1, y1 = group2[i]
        x2, y2 = group2[i + 1]
        plt.plot([x1, x2], [y1, y2], color='red')  # Line between consecutive points in group 2

    # Add labels and a title
    plt.xlabel('X coordinates')
    plt.ylabel('Y coordinates')
    plt.title('Lines Between Consecutive Points')

    # Show the plot
    plt.grid(True)
    plt.show()

# Run the script
filename = "out.out"  # Change if needed
group1, group2 = parse_file(filename)
plot_groups(group1, group2)

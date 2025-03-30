import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import os
import glob

def parse_file(filename):
    with open(filename, "r") as file:
        lines = file.readlines()

    # Convert the coordinates to integers and create tuples
    group1 = [tuple(map(int, x.strip("()").split(";"))) for x in lines[2].split()]
    group2 = [tuple(map(int, x.strip("()").split(";"))) for x in lines[4].split()]

    return group1, group2

def plot_frame(group1, group2, frame, ax):
    ax.clear()
    ax.set_title(f"Frame {frame}")
    ax.set_xlabel("X coordinates")
    ax.set_ylabel("Y coordinates")
    ax.grid(True)

    # Plot lines between consecutive points in group 1
    for i in range(len(group1) - 1):
        x1, y1 = group1[i]
        x2, y2 = group1[i + 1]
        ax.plot([x1, x2], [y1, y2], color='blue')

    # Plot lines between consecutive points in group 2
    for i in range(len(group2) - 1):
        x1, y1 = group2[i]
        x2, y2 = group2[i + 1]
        ax.plot([x1, x2], [y1, y2], color='red')

def create_gif(filenames, output_gif):
    fig, ax = plt.subplots(figsize=(8, 6))

    def update(frame):
        filename = filenames[frame]
        group1, group2 = parse_file(filename)
        plot_frame(group1, group2, frame, ax)

    ani = FuncAnimation(fig, update, frames=len(filenames), repeat=False)
    ani.save(output_gif, writer="pillow", fps=2)  # Save as GIF with Pillow

# Example usage
prefix = "out"  # Replace with your desired prefix
filenames = sorted(glob.glob(os.path.join("out", f"{prefix}*.txt")))
output_gif = "output.gif"
create_gif(filenames, output_gif)
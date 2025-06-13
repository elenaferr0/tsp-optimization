import matplotlib.pyplot as plt
import re, os
import argparse


def parse_ga_solution_file(file_path):
    """
    Parses a .ga solution file and extracts nodes and their coordinates.
    Format expected: node_id x_coord y_coord on each line
    """
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Filter out comment lines and empty lines
    lines = [line.strip() for line in lines if line.strip() and not line.strip().startswith('//')]

    points = []
    node_order = []
    node_coords = {}

    for line in lines:
        parts = line.split()
        if len(parts) >= 3:
            node_id = int(parts[0])
            x_coord = float(parts[1])
            y_coord = float(parts[2])

            node_order.append(node_id)
            node_coords[node_id] = (x_coord, y_coord)
            points.append((x_coord, y_coord))

    # Create tour edges from the node order
    tour_edges = {}
    for i in range(len(node_order)):
        current_node = node_order[i]
        next_node = node_order[(i + 1) % len(node_order)]  # Wrap around to the first node
        tour_edges[(current_node, next_node)] = 1.0

    return points, node_coords, node_order, tour_edges


def plot_tsp_solution(points, node_coords, node_order, tour_edges, title="TSP Tour"):
    """
    Plots the TSP tour based on the nodes in order.
    """
    plt.figure(figsize=(10, 8))

    # Plot all nodes
    for node_id, (x, y) in node_coords.items():
        plt.scatter(x, y, color='green', s=100)
        plt.annotate(str(node_id), (x, y), textcoords="offset points",
                     xytext=(0, 10), ha='center')

    # Draw the tour based on node order
    for i in range(len(node_order)):
        current_node = node_order[i]
        next_node = node_order[(i + 1) % len(node_order)]

        current_x, current_y = node_coords[current_node]
        next_x, next_y = node_coords[next_node]

        plt.plot([current_x, next_x], [current_y, next_y],
                 '--k', linewidth=1.5, alpha=0.7, zorder=1)

    plt.title(title)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()


def main():
    parser = argparse.ArgumentParser(description='TSP Solution Visualizer for GA solutions')
    parser.add_argument('-i', '--instance', required=True, help='TSP instance name (without extension)')

    args = parser.parse_args()
    instance = args.instance

    working_dir = os.path.dirname(os.path.abspath(__file__))
    samples_dir = os.path.join(working_dir, "../samples")

    # Update file paths for .ga files
    sol_file = os.path.join(samples_dir, f"{instance}_sol.ga")
    output_file = os.path.join(samples_dir, f"{instance}_tour.png")

    if not os.path.exists(sol_file):
        print(f"Error: Solution file not found at {sol_file}")
        return

    try:
        points, node_coords, node_order, tour_edges = parse_ga_solution_file(sol_file)
        plot_tsp_solution(points, node_coords, node_order, tour_edges,
                          title=f"TSP Tour for {instance} (GA solution)")
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        print(f"GA tour plot saved to {output_file}")

        # Also show the plot
        plt.show()
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()

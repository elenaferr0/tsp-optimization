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


def parse_graph_file(file_path):
    """
    Parses a TSP graph file to extract the coordinates of the cities.
    """
    with open(file_path, 'r') as file:
        lines = file.readlines()

    n_points = int(lines[0])
    points = []
    for i in range(1, n_points + 1):
        if i < len(lines):
            coords = lines[i].strip().split(' ')[1:]
            assert len(coords) == 2, f"Expected 2 coordinates, got {len(coords)}"
            x, y = float(coords[0]), float(coords[1])
            points.append((x, y))
    return points


def plot_tsp_solution(points, node_coords, node_order, tour_edges, title="TSP Tour"):
    """
    Plots the TSP tour based on the nodes in order.
    """
    plt.figure(figsize=(10, 8))
    
    # If we have node_coords, use them; otherwise use points with indices
    if node_coords:
        # Plot all nodes using node_coords
        for node_id, (x, y) in node_coords.items():
            plt.scatter(x, y, color='red', s=50, zorder=3)
            plt.annotate(str(node_id), (x, y), textcoords="offset points",
                         xytext=(0, 10), ha='center', fontsize=8)
        
        # Draw the tour based on node order
        for i in range(len(node_order)):
            current_node = node_order[i]
            next_node = node_order[(i + 1) % len(node_order)]
            current_x, current_y = node_coords[current_node]
            next_x, next_y = node_coords[next_node]
            plt.plot([current_x, next_x], [current_y, next_y],
                     'b-', linewidth=2, alpha=0.7, zorder=2)
    else:
        # Fallback: just plot points
        x_coords = [p[0] for p in points]
        y_coords = [p[1] for p in points]
        plt.scatter(x_coords, y_coords, color='red', s=50, zorder=3)
        
        # Add point labels
        for i, (x, y) in enumerate(points):
            plt.annotate(str(i), (x, y), textcoords="offset points",
                         xytext=(0, 10), ha='center', fontsize=8)

    plt.title(title)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True, linestyle='--', alpha=0.3)
    plt.tight_layout()


def main():
    parser = argparse.ArgumentParser(description='TSP Visualizer - Plot points and optionally GA tour from solution')
    parser.add_argument('-d', '--dat', required=True, help='Path to TSP instance file (.dat)')
    parser.add_argument('-s', '--sol', help='Path to GA solution file (.ga) - optional')
    parser.add_argument('-o', '--output', help='Output image file path - optional')

    args = parser.parse_args()

    # Check if dat file exists
    if not os.path.exists(args.dat):
        print(f"Error: Instance file not found at {args.dat}")
        return

    # Parse the instance file
    try:
        points = parse_graph_file(args.dat)
        print(f"Loaded {len(points)} points from instance file")
    except Exception as e:
        print(f"Error parsing instance file: {e}")
        return

    # Check if GA solution file exists (if provided)
    node_coords = None
    node_order = None
    tour_edges = None
    
    if args.sol:
        if not os.path.exists(args.sol):
            print(f"Error: Solution file not found at {args.sol}")
            return
        try:
            points_from_ga, node_coords, node_order, tour_edges = parse_ga_solution_file(args.sol)
            print(f"Loaded GA tour with {len(tour_edges)} edges")
        except Exception as e:
            print(f"Error parsing GA solution file: {e}")
            return

    # Generate title from filename
    instance_name = os.path.splitext(os.path.basename(args.dat))[0]
    title = f"TSP Instance: {instance_name}"
    if tour_edges:
        title += " with GA Tour"

    # Plot the solution
    plot_tsp_solution(points, node_coords, node_order, tour_edges, title)

    # Save or show the plot
    if args.output:
        plt.savefig(args.output, dpi=300, bbox_inches='tight')
        print(f"Plot saved to {args.output}")
    else:
        # Generate default output filename
        output_dir = os.path.dirname(args.dat)
        if tour_edges:
            output_file = os.path.join(output_dir, f"{instance_name}_tour.png")
        else:
            output_file = os.path.join(output_dir, f"{instance_name}_points.png")

        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        print(f"Plot saved to {output_file}")

    plt.show()


if __name__ == "__main__":
    main()

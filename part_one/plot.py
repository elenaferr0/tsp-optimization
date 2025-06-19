import matplotlib.pyplot as plt
import xml.etree.ElementTree as ET
import re, os
import argparse


def parse_sol_file(file_path):
    """
    Parses a CPLEX solution XML file and extracts the tour edges.
    Works with both MTZ and GG formulations automatically.
    """
    tree = ET.parse(file_path)
    root = tree.getroot()

    variables = {}
    for var in root.find('variables'):
        name = var.get('name')
        value = float(var.get('value'))
        variables[name] = value

    tour_edges = {}  # To store edges for plotting

    # Extract edges from any x_ or y_ variables with value > 0.5
    for name, val in variables.items():
        if val > 0.5:  # Consider as selected edge
            if matches := re.match(r'[xy]_(\d+)_(\d+)', name):
                i, j = matches.groups()
                tour_edges[(int(i), int(j))] = val

    return tour_edges


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


def plot_tsp_solution(points, tour_edges=None, title="TSP Instance"):
    """
    Plots the TSP points and optionally the tour if tour_edges are provided.
    """
    x_coords = [p[0] for p in points]
    y_coords = [p[1] for p in points]

    plt.figure(figsize=(10, 8))
    plt.scatter(x_coords, y_coords, color='red', s=50, zorder=3)

    # Add point labels
    for i, (x, y) in enumerate(points):
        plt.annotate(str(i), (x, y), textcoords="offset points",
                     xytext=(0, 10), ha='center', fontsize=8)

    # Draw the tour if tour_edges are provided
    if tour_edges:
        for (i, j), val in tour_edges.items():
            plt.plot([points[i][0], points[j][0]],
                     [points[i][1], points[j][1]],
                     'b-', linewidth=2, alpha=0.7, zorder=2)
        title += " with Tour"

    plt.title(title)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True, linestyle='--', alpha=0.3)
    plt.tight_layout()


def main():
    parser = argparse.ArgumentParser(description='TSP Visualizer - Plot points and optionally tour from solution')
    parser.add_argument('-d', '--dat', required=True, help='Path to TSP instance file (.dat)')
    parser.add_argument('-s', '--sol', help='Path to solution file (.sol) - optional')
    parser.add_argument('-o', '--output', help='Output image file path - optional')

    args = parser.parse_args()

    # Check if dat file exists
    if not os.path.exists(args.dat):
        print(f"Error: Instance file not found at {args.dat}")
        return

    # Check if sol file exists (if provided)
    tour_edges = None
    if args.sol:
        if not os.path.exists(args.sol):
            print(f"Error: Solution file not found at {args.sol}")
            return
        try:
            tour_edges = parse_sol_file(args.sol)
            print(f"Loaded tour with {len(tour_edges)} edges")
        except Exception as e:
            print(f"Error parsing solution file: {e}")
            return

    # Parse the instance file
    try:
        points = parse_graph_file(args.dat)
        print(f"Loaded {len(points)} points from instance file")
    except Exception as e:
        print(f"Error parsing instance file: {e}")
        return

    # Generate title from filename
    instance_name = os.path.splitext(os.path.basename(args.dat))[0]
    title = f"TSP Instance: {instance_name}"

    # Plot the solution
    plot_tsp_solution(points, tour_edges, title)

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

import matplotlib.pyplot as plt
import xml.etree.ElementTree as ET
import re, os
import argparse


def parse_sol_file(file_path, formulation):
    """
    Parses a CPLEX solution XML file and extracts the relevant variables
    based on the specified formulation.
    """
    tree = ET.parse(file_path)
    root = tree.getroot()

    variables = {}
    for var in root.find('variables'):
        name = var.get('name')
        value = float(var.get('value'))
        variables[name] = value

    x_variables = {}
    tour_edges = {}  # To store edges for plotting

    if formulation.lower() == 'mtz':
        u_variables = {}
        for name, val in variables.items():
            if matches := re.match(r'x_(\d+)_(\d+)', name):
                i, j = matches.groups()
                x_variables[(int(i), int(j))] = val
                if val > 0.5:
                    tour_edges[(int(i), int(j))] = val
            elif matches := re.match(r'u_(\d+)', name):
                index = int(matches.group(1))
                u_variables[index] = int(val)
        return tour_edges
    elif formulation.lower() == 'gg':
        y_variables = {}
        for name, val in variables.items():
            if matches := re.match(r'x_(\d+)_(\d+)', name):
                i, j = matches.groups()
                x_variables[(int(i), int(j))] = val
            elif matches := re.match(r'y_(\d+)_(\d+)', name):
                i, j = matches.groups()
                y_variables[(int(i), int(j))] = val
                if val > 0.5:
                    tour_edges[(int(i), int(j))] = val
        return tour_edges
    else:
        raise ValueError(f"Unsupported formulation: {formulation}. Must be 'gg' or 'mtz'.")


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


def plot_tsp_solution(points, tour_edges, title="TSP Tour"):
    """
    Plots the TSP tour based on the extracted tour edges.
    """
    x_coords = [p[0] for p in points]
    y_coords = [p[1] for p in points]

    plt.figure(figsize=(10, 8))
    plt.scatter(x_coords, y_coords, color='green', s=100)

    for i, (x, y) in enumerate(points):
        plt.annotate(str(i), (x, y), textcoords="offset points",
                     xytext=(0, 10), ha='center')

    # Draw the tour based on tour_edges
    for (i, j), val in tour_edges.items():
        plt.plot([points[i][0], points[j][0]],
                 [points[i][1], points[j][1]],
                 '--k', linewidth=1.5, alpha=0.3)

    plt.title(title)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()


def main():
    parser = argparse.ArgumentParser(description='TSP Solution Visualizer for GG or MTZ Formulation')
    parser.add_argument('-i', '--instance', required=True, help='TSP instance name (without extension)')
    parser.add_argument('-f', '--formulation', required=True, choices=['gg', 'mtz'],
                        help='The ILP formulation used (gg or mtz)')

    args = parser.parse_args()
    instance = args.instance
    formulation = args.formulation.lower()
    working_dir = os.path.dirname(os.path.abspath(__file__))
    samples_dir = os.path.join(working_dir, "../samples")
    sol_file = os.path.join(samples_dir, f"{instance}_{formulation}.sol")
    graph_file = os.path.join(samples_dir, f"{instance}.dat")
    output_file = os.path.join(samples_dir, f"{instance}_{formulation}_tour.png")

    if not os.path.exists(sol_file):
        print(f"Error: Solution file not found at {sol_file}")
        return
    if not os.path.exists(graph_file):
        print(f"Error: Graph file not found at {graph_file}")
        return

    try:
        tour_edges = parse_sol_file(sol_file, formulation)
        points = parse_graph_file(graph_file)
        plot_tsp_solution(points, tour_edges, title=f"TSP Tour for {instance} ({formulation.upper()} formulation)")
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        print(f"{formulation.upper()} tour plot saved to {output_file}")
    except ValueError as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()

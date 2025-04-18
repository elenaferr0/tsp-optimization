import matplotlib.pyplot as plt
import xml.etree.ElementTree as ET
import re, os
import argparse

def parse_sol_file(file_path):
    # Parse the XML file
    tree = ET.parse(file_path)
    root = tree.getroot()

    # Extract all variables
    variables = {}
    for var in root.find('variables'):
        name = var.get('name')
        value = float(var.get('value'))
        variables[name] = value

    # Extract x variables that represent the tour
    x_variables = {}
    y_variables = {}

    for name, val in variables.items():
        if matches := re.match(r'x_(\d+)_(\d+)', name):
            i, j = matches.groups()
            x_variables[(int(i), int(j))] = val
        elif matches := re.match(r'y_(\d+)_(\d+)', name):
            i, j = matches.groups()
            y_variables[(int(i), int(j))] = val

    return x_variables, y_variables

def parse_graph_file(file_path):
    # Read the data from the file
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Parse number of points from the first line
    n_points = int(lines[0])

    # Parse coordinates
    points = []
    for i in range(1, n_points + 1):
        if i < len(lines):
            coords = lines[i].strip().split(' ')[1:]
            assert len(coords) == 2, f"Expected 2 coordinates, got {len(coords)}"
            x, y = float(coords[0]), float(coords[1])
            points.append((x, y))
    return points

def main():
    # Set up argument parser
    parser = argparse.ArgumentParser(description='TSP Solution Visualizer')
    parser.add_argument('-i', '--instance', required=True, help='TSP instance to be solved')

    args = parser.parse_args()

    instance = args.instance

    working_dir = os.path.dirname(os.path.abspath(__file__))
    sol_file = f"{working_dir}/../samples/{instance}.sol"
    graph_file = f"{working_dir}/../samples/{instance}.txt"
    output_file = f"{working_dir}/../samples/{instance}.png"

    x_variables, y_variables = parse_sol_file(sol_file)
    points = parse_graph_file(graph_file)

    x_coords = [p[0] for p in points]
    y_coords = [p[1] for p in points]

    plt.figure(figsize=(10, 8))
    plt.scatter(x_coords, y_coords, color='green', s=100)

    # Label each point with its index
    for i, (x, y) in enumerate(points):
        plt.annotate(str(i), (x, y), textcoords="offset points",
                     xytext=(0, 10), ha='center')

    # Draw the tour using y variables (which indicate the edges in the tour)
    # y_i_j = 1 means there's an edge from i to j in the tour
    for (i, j), val in y_variables.items():
        if val > 0.5:  # Only draw if the edge is in the tour
            plt.plot([points[i][0], points[j][0]],
                    [points[i][1], points[j][1]],
                    'k--', linewidth=1.5, alpha=0.3)

    plt.title('TSP Tour Visualization')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True, linestyle='--', alpha=0.7)

    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')

if __name__ == "__main__":
    main()
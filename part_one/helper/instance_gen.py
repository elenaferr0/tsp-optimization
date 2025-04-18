import argparse, math, random, os


def write_graph(coordinates, output: str):
    n = len(coordinates)

    with open(output, 'w') as f:
        f.write(f"{n}\n") # Number of nodes
        for i in range(n):
            x, y = coordinates[i]
            f.write(f"{i} {x} {y}\n")

        f.write(f"{int(n * (n - 1) / 2)}\n") # Number of edges
        # Write edges (all possible permutations without repetition and self-loops)
        for i in range(n):
            for j in range(i + 1, n):
                f.write(f"{i} {j}\n")

def generate_instances(size: int):
    b = size / 2

    # Generate random coordinates
    coordinates = []
    for i in range(size):
        x = random.uniform(0, b)
        y = random.uniform(0, b)
        coordinates.append((x, y))
    return coordinates


def main():
    parser = argparse.ArgumentParser(description='Random instance generator for TSP')
    parser.add_argument('-s', '--size', required=True, help='Number of nodes in the TSP instance')
    args = parser.parse_args()

    size = int(args.size)
    working_dir = os.path.dirname(os.path.abspath(__file__))
    output = f"{working_dir}/../samples/random{size}.txt"
    coordinates = generate_instances(size)
    write_graph(coordinates, output)


if __name__ == "__main__":
    main()

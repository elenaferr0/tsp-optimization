# TSP Optimization

A project implementing two approaches to solve the Traveling Salesman Problem (TSP) in the context of PCB production, where the objective is to find the shortest path for a drill to bore holes in a board.

## Structure

- **`part_one/`** - Exact methods using CPLEX with two formulations:
  - Gavish-Graves (GG)
  - Miller-Tucker-Zemlin (MTZ)
- **`part_two/`** - Genetic Algorithm implementation with configurable parameters
- **`random_instance_gen.py`** - Script to generate random TSP instances
- **`report/`** - Project documentation and analysis

## Quick Start

### Part One (Exact Methods)
```bash
cd part_one
make
./main <instance.dat> [gg|mtz] [timeout]
```

### Part Two (Genetic Algorithm)
```bash
cd part_two
make
./main <instance.dat>
```

### Generate Random Instance
```bash
python random_instance_gen.py -n <num_points> -o <output_file>
```

## File Format

Input instances are `.dat` files with format:
```
<number_of_points>
<id> <x> <y>
<id> <x> <y>
...
```

## Results

- **Exact methods** provide optimal solutions for small instances (≤20 nodes) in reasonable time
- **Genetic Algorithm** scales better to larger instances (50-100+ nodes) but with larger optimality gaps
- For small problems (<10 nodes), exact methods are faster and optimal
- For larger problems (>50 nodes), GA provides faster approximate solutions

## Requirements

- C++23 compiler
- CPLEX (for exact methods)
- Python 3 (for visualization and instance generation)
- CMake

## Visualization

Each part includes a `plot.py` script for visualizing solutions and generating tour plots.

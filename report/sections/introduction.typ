#import "../glossary.typ": defs
#import "@preview/glossy:0.8.0": init-glossary

#show : init-glossary.with(defs)

= Introduction
This report presents two approaches which were implemented to solve the @TSP: an exact method and a @GA. Specifically, this project addresses the @TSP in the context of @PCB:pl production, where the objective is to determine the shortest path for a drill to bore a set of holes in a board

== Project structure and usage
The project contains two folders corresponding to the two implemented approaches. Both parts contain an `include` directory for header files, a `samples` directory for input instances, and a `src` directory for the source code. The root directory also contains `random_instance_gen.py`, a script to generate random @TSP instances. As the two approaches generate solutions in different formats, each part of the project includes its own `plot.py` script for result visualization.

For simplicity, these scripts are written in Python, to leverage its plotting libraries and streamline instance generation.

Below is the structure of the project:
```
  ├── part_one
  │   ├── include
  │   ├── samples
  │   ├── src
  │   └── plot.py
  ├─── part_two
  │   ├── include
  │   ├── samples
  │   ├── src
  │   └── plot.py
  └─── random_instance_gen.py
```

=== File formats <sec:file-formats>
Input instances are stored in `.dat` files. The first line of the file contains the total number of holes, followed by the coordinates of each point. Each hole's data is formatted as `id x y`, where id is its incremental identifier, and `x` and `y` are its coordinates.

For example, a file with two holes would look like this:
```
2
0 1 2
1 3 4
```

For simplicity, this same file format is also used for the solutions produced by the @GA. In that case, the produced file is named as `<instance>_sol.dat`, where `<instance>` is the name of the input instance file.

For what concerns the solution files produced by the exact algorithm instead, they are stored using CPLEX's standard `.sol` format. 
To prevent the solution file from being cluttered with unnecessary data, only non-zero variables are exported. This is achieved by setting the `CPX_PARAM_WRITELEVEL` parameter to `CPX_WRITELEVEL_NOZEROVARS` in the CPLEX environment.

=== Running part one
The following commands assume the user is initially positioned in the root directory of the project. The first part can be run with the following commands:

```bash
cd part_one
make
./main <instance> [formulation] [timeout]
```

Here, `<instance>` is the path to the input instance, and `formulation` is an optional parameter that specifies either `gg` for @GG formulation or `mtz` for the @MTZ one. If no formulation is specified, the default is `gg`. The `timeout` parameter is optional too and limits the maximum time in seconds to run the algorithm. If no timeout is specified, the algorithm will run until completion.
In order to set the timeout, the `CPX_PARAM_TILIM` parameter is set in the CPLEX environment.

After running the algorithm, a `.sol` solution file will be generated in the same directory as the input instance, named as `<instance>_<formulation>.sol`. This file contains the solution to the problem, which can be visualized using the provided `plot.py` script.

Example of usage of a @GG formulated instance named `random_10.dat` with a timeout of 60 seconds:
```bash
./main samples/random_10.dat gg 60
```
The solution will be saved in `samples/random_10_gg.sol`. 

==== Plot visualization
To visualize the solution, the `plot.py` script can be used. It takes the following arguments:
```bash
plot.py [-h] -d DAT [-s SOL] [-o OUTPUT]
```
Where `-d DAT` is the path to the input instance file, `-s SOL` is the path to the solution file (optional), and `-o OUTPUT` is the output file for the plot (optional). If no output file is specified, the plot will be displayed on screen.
To visualize the solution from the previous step, run:
```bash
py plot.py -d ./samples/random_10.dat -s ./samples/random_10_gg.sol
```
This will produce an image showing the holes in the instance and the path taken by the drill to bore them, as well as the total distance traveled. The image will be saved in the same directory as the input instance, named as `<instance>_tour.png`.

The plot script can also be used to visualize the input instance without a solution file. To achieve this, it's sufficient to run:
```bash
py plot.py -d ./samples/random_10.dat
```

=== Running part two
A similar procedure can be followed for the second part of the project. The commands to run the second part are as follows:
```bash
cd part_two
make
./main <instance>
```

Note that the second part runs the @GA with pre-tuned parameters, so no additional arguments are required. The solution will be saved in the same directory as the input instance, named as `<instance>_sol.dat`.

==== Plot visualization
A separate script is used for the second part, since the @GA's solution format differs from that of the exact algorithm. Its usage however, is identical:
```bash
py plot.py -d ./samples/random_10.dat -s ./samples/random_10_sol.dat
```
This will produce an image showing the holes in the instance and the path taken by the drill to bore them. The image will be saved in the same directory as the input instance, named as `<instance>_tour.png`.

== Problem instances
In order to test the implemented algorithms, a set of random instances was generated. Each consists of a number of holes represented as points in a 2D plane. Distances are calculated using the Euclidean distance formula.

The goal is to generate instances that are representative of real-world @PCB production scenarios. To achieve this, points are generated according to one of the following patterns: 
- line: three to five points are aligned in a straight line, with either a uniform or non-uniform random distance between them. The line can be orientated in several directions, including diagonally;
- triangle: three points are randomly placed in a triangular shape, with varying size and orientation. These can also be irregular triangles;
- rectangle: four points are placed in a rectangular shape, with varying size and orientation;
- parallelogram: four points are placed in a parallelogram shape, with varying size, orientation and skewness.

Other than these, a few points are randomly created. During this generation process, a minimum distance between points and patterns is enforced, to avoid overlapping holes. In a real-world scenario indeed, a minimum distance between points is probably required to ensure that the drill can operate correctly without damaging the board or the holes. A density parameter controls the sparsity, with a higher density resulting in more closely packed holes. Note that these patterns are, in general, not very visible for smaller instances, but they become more apparent as the number of holes increases. #ref(<fig:random-instance-eg>) shows an example of a random instance with 100 holes, with quite evident patterns.

#figure(
  image("../imgs/random_instance_eg.png", width: 65%),
  caption: "Example of a random instance with 100 holes.",
) <fig:random-instance-eg>

=== Script usage
The script used to generate random instances is available in the root directory of the project, under the name `random_instance_gen.py`. It can be run from the command line with the following syntax:
```bash
python3 random_instance_gen.py -s SIZE [-d DENSITY] -o OUTPUT_DIR
```
For instance, to generate a random instance with 100 holes and a density of 0.5, and save it in the `part_one/samples` directory, one would run:
```bash
python3 random_instance_gen.py -s 100 -d 0.5 -o part_one/samples
```
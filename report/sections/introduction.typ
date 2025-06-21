#import "../glossary.typ": defs
#import "@preview/glossy:0.8.0": init-glossary

#show : init-glossary.with(defs)

= Introduction
This report presents two ways which were implemented to solve the @TSP:both: an exact approach and a genetic algorithm. More specifically, this project positions itself in the context of @PCB:pl production, where the goal is to find the shortest path for a drill to take when boring holes in a board.

== Project structure
#text("TODO: structure, code, scripts", red, size: 18pt)
```
├── part_one
│   ├── include
│   ├── samples
│   └── src
├─── part_two
│   ├── include
│   ├── samples
│   └── src
└─── random_instance_gen.py
```

== Problem instances
In order to test the implemented algorithms, a set of random instances was generated. Each consists of a number of holes represented as points in a 2D plane. The distance between two holes is calculated using the Euclidean distance formula.

The goal is that of generating instances which are representative of real-world scenarios for @PCB production. To achieve this, points are generated according to one of the following patterns: 
- line: three to five points are aligned in a straight line, with either a uniform or non-uniform random distance between them. The line can be orientated in several directions, including diagonally;
- triangle: three points are randomly placed in a triangular shape, with varying size and orientation. These can also be irregular triangles;
- rectangle: four points are placed in a rectangular shape, with varying size and orientation;
- parallelogram: four points are placed in a parallelogram shape, with varying size, orientation and skewness.

Other than these, a few points are randomly created. During this generation process, a minimum distance between points and patterns is enforced, to avoid overlapping holes. A density parameter controls the spacing, with a higher density resulting in more closely packed holes. Note that these patterns are, in general, not very visible for smaller instances, but they become more apparent as the number of holes increases. #ref(<fig:random-instance-eg>) shows an example of a random instance with 100 holes, with quite evident patterns.

#figure(
  image("../imgs/random_instance_eg.png", width: 60%),
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
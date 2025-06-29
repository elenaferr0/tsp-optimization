#import "../glossary.typ": defs
#import "@preview/glossy:0.8.0": init-glossary

#show: init-glossary.with(defs)

= Exact approach
With regard to the exact approach, two compact formulations were implemented:
- the one proposed by @GG:both @gavish-graves-1978, which uses a network flow-based approach to eliminate subtours #footnote[In the context of the TSP, a subtour is a closed route that visits a subset of the cities but not all of them, forming a disconnected cycle within the graph. Such cycles must be eliminated to find the optimal complete tour.]\;
- the one introduced by @MTZ:both @miller-tucker-zemlin-1960, which uses additional variables representing the order or position of each city in the tour; the elimination is achieved by ensuring that these variables maintain a sequential order, thus preventing subtours.

The problems built according to these two formulations are then fed to the IBM CPLEX 22.11 solver through its C++ API.

== @MTZ formulation
The @MTZ formulation is based on the mathematical model described in #ref(<eq:mtz-formulation>).

$
 min & sum_(i = 1)^n sum_(j != i, j = 1)^n c_(i j) x_(i j) quad quad &\
 & x_(i j) in {0, 1} & forall i, i = 1, dots, n \
 & sum_(i != j, i = 1)^n x_(i j) = 1 & forall j = 1, dots, n \
 & sum_(j != i, j = 1)^n x_(i j) = 1 & forall i = 1, dots, n \
 & u_i - u_j + n x_(i j) <= n - 1 quad & 2 <= i != j <= n\
 & 2 <= u_i <= n - 1 quad & 2 <= i <= n
$ <eq:mtz-formulation>

== Implementation details
This section provides an overview of some details regarding the exact approach, including certain aspects of the practical implementation.

=== Graph representation <sec:graph>
A `Graph` class is used to represent the problem instances, containing a vector of `Node` objects, each representing a hole in the board. The solution representation used throughout this project is indeed the path representation.
This class provides a convenient way to read input instances from files and store both the coordinates of points and their Euclidean distance.

=== Formulations
The `Formulation` class is an abstract base class that defines the interface for the two specific formulations: `GavishGraves` and `MillerTuckerZemlin`. Each formulation implements the methods necessary to create the variables and constraints required to solve the @TSP. Furthermore, it contains common logic to setup CPLEX, solve the problem and export the solution.

=== Time logger
To ease the process of measuring the time taken by the solver to create the model and solve it, a `TimeLogger` class is provided. Once it has been instantiated and started, calling the `tick` method with a message as argument will log the elapsed time since the last tick or the instantiation of the logger. At the end of the process, the `log_total_time` can be called to display the total time taken by the process.

=== Variables and constraints creation
Particular attention was given to the creation of variables and constraints. This section provides an overview of how the variables and constraints are created, including considerations for performance and memory usage.

==== Maps
Internally, CPLEX considers variables to be all stored in a single vectorial structure; it is therefore not straightforward to associate the variable's position in the vector with the corresponding indexes in the problem, after the variable has been created. To address this, both formulations use matrices to store, for each pair of coordinates (i, j), the variable's index in the vector. This allows for easy access to the variables when creating constraints or retrieving solutions.
The two formulations differ in how they store these indices:
- for the @GG formulation, two integer matrices are used, holding indices for variables $x_(i j)$ and $y_(i j)$\;
- in the @MTZ implementation instead, a single integer matrix is used to hold the indices for the variables $x_(i j)$, and a vector memorizes indices for the order variables $u_i$.

==== Optimizations
For optimal performance and memory efficiency, both formulations create all variables at once, rather than one by one. This approach minimizes the overhead associated with multiple calls to the CPLEX API, which can be costly in terms of performance. To do so, the formulations leverage the custom implemented `Constraints` and `Variables` support classes, that are iteratively filled with the necessary data and then fed to CPLEX in a single call.

Furthermore, only the minimum number of variables and constraints necessary to solve the problem are created. Assuming $N$ to be the number of nodes in the problem, below is a summary of the number of variables and constraints created by each formulation.

===== @GG:long formulation
The @GG formulation creates the following variables:
+ $y_(i j)$: $n (n - 1)$ variables, given that they are created for each pair of nodes $(i, j)$ with $i != j$\;
+ $x_(i j)$: $(n - 1)^2$ variables, given that they are created for each pair of nodes $(i, j)$ with $i != j$ starting from $j = 1$, since $j = 0$ corresponds to the depot node.
Therefore, a total of $n (n - 1) + (n - 1)^2 = (n - 1)(2n - 1) =2 n^2 - 3 n + 1$ variables are created.

For constraints instead, it creates:
+ flow conservation: $(n - 1)$ constraints, applied for all nodes except node 0 (depot)\;
+ outgoing degree: $n$ constraints, one per node ensuring exactly one outgoing edge\;
+ incoming degree: $n$ constraints, one per node ensuring exactly one incoming edge\;
+ flow-edge coupling: $n(n - 1) - (n - 1) = (n - 1)^2$ constraints, created for all pairs $(i, j)$ where both $x_(i j)$ and $y_(i j)$ variables exist.

So in total, $(n - 1) + n + n + (n - 1)^2 = n (n + 1)$ constraints are created.

===== @MTZ:long formulation
The @MTZ formulation creates the following variables:
+ $x_(i j)$: $n (n - 1)$ variables, given that they are created for each pair of nodes $(i, j)$ with $i != j$\;
+ $u_i$: $n - 1$ variables, given that they are created for each node $i$ except the depot node 0.

In total, the @MTZ formulation creates $n (n - 1) + (n - 1) = n^2 - 1$ variables.

For what concerns the constraints, it creates:
+ outgoing degree: $n$ constraints, one per node ensuring exactly one outgoing edge\;
+ incoming degree: $n$ constraints, one per node ensuring exactly one incoming edge\;
+ subtour elimination: $(n - 1)^2 - (n - 1) = (n - 1)(n - 2)$ constraints, created for all pairs $(i, j)$ where $i != j$ and both $i, j != 0$\;
+ lower bound for order variables: $(n - 1)$ constraints, setting $u_i >= 1$ for each node $i != 0$\;
+ upper bound for order variables: $(n - 1)$ constraints, setting $u_i <= n - 1$ for each node $i != 0$.

In total, the @MTZ formulation creates $n + n + (n - 1) (n - 2) + (n - 1) + (n - 1) = n(n + 1)$ constraints.

===== Comparison
To understand which implementation is more efficient in terms of memory, the number of variables and constraints created by each formulation can be compared.
- with regard to variables, @GG produces $2 n^2 - 3 n + 1$, while @MTZ: $n^2 - 1$. Therefore, given that
  $ 2 n^2 - 3 n + 1 < n^2 - 1 => 1 < n < 2 $
  the @MTZ formulation is, in practice, producing less variables than @GG with equal size of the problem.

- for what concerns constraints instead, they both create $n(n + 1)$ and therefore they are equivalent in this regard.

To summarize, both formulation create a polynomial number of variables and constraints, more specifically $O(n^2)$.

== Benchmark results
To evaluate the performance of the exact approach, a set of randomly generated instances of different sizes were solved with increasing time limits. The goal was to determine the largest instance that could be solved within each time limit.
Note that to ensure a reliable comparison, more than one instance per size should be taken under consideration. However, due to the time constraints of this project, only one problem per size was used.

In order to simplify the process of reproducing this benchmark, a `benchmark.sh` script has been implemented, and is provided inside the `part_one` folder. It's sufficient to run it without arguments as follows:
```bash
sh benchmark.sh
```
The results will be stored in the `results` folder as text files.

Below is a summary of the instances used in the benchmark, which are all randomly generated problems with a number of nodes ranging from 5 to 200. The instances are named according to the number of nodes they contain, for example `random5` for a problem with 5 nodes.
#let instances = (
  "random5",
  "random10",
  "random20",
  "random50",
  "random100",
  "random200",
)

#figure(block(breakable: false, grid(
  row-gutter: 12pt,
  columns: (1fr, 1fr, 1fr),
  ..instances.map(i => raw(i))
)))

The following time limits in seconds were taken under consideration: 1, 2, 5, 10, 20, 30, 60, 120 and 240.

#let instance_by_solving_limit = (
  (size: 5, limit: 1),
  (size: 10, limit: 1),
  (size: 20, limit: 1),
  (size: 50, limit: 5),
  (size: 100, limit: 120),
  (size: 200, limit: -1),
)

#let largest_solvable_by_limit = (
  (limit: 1, size: 20),
  (limit: 2, size: 20),
  (limit: 5, size: 50),
  (limit: 10, size: 50),
  (limit: 20, size: 50),
  (limit: 30, size: 50),
  (limit: 60, size: 50),
  (limit: 120, size: 100),
  (limit: 240, size: 100),
)

#ref(<tab:instance-solving-limits>) shows the time limit within which each instance was solved; the largest problem, `random200`, was not solved within the time limit of 240 seconds.

#ref(<tab:largest-solvable-by-timelimit>) instead, shows the largest instance which was solved within each time limit. For example, the largest instance that could be solved within 60 seconds was `random50`, while the largest instance that could be solved within 120 seconds was `random100`. From this table it is quite evident that, as expected, the performance of the exact approach degrades exponentially as the size of the problem increases: solving a 20 nodes problem indeed only took 2 seconds, however for a 50 cities one the elapsed time is 60 seconds, that is, 30 times larger.

#grid(
  columns: (50%, 50%),
  [
    #figure(
      table(
        columns: 2,
        table.header([Instance], [Time Limit (s)]),
        ..instance_by_solving_limit.map(i => (
          [#raw("random_" + str(i.size))],
          [#if (i.limit == -1) { "N/A" } else { i.limit }],
        )).flatten(),
      ),
      caption: "Instances by solving limit",
    ) <tab:instance-solving-limits>
  ],
  [
    #figure(
      table(
        columns: 2,
        table.header([Time Limit (s)], [Largest solvable instance]),
        ..largest_solvable_by_limit
          .map(i => (
            [#raw(str(i.limit))],
            [#raw("random_" + str(i.size))],
          ))
          .flatten(),
      ),
      caption: "Largest solvable instance by timelimit",
    ) <tab:largest-solvable-by-timelimit>
  ],
)

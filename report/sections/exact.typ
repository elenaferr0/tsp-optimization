#import "../glossary.typ": defs
#import "@preview/glossy:0.8.0": init-glossary

#show : init-glossary.with(defs)

= Exact approach
With regard to the exact approach, two compact formulations were implemented: 
- the one proposed by Gavish and Graves (GG) @gavish-graves-1978, which uses a network flow-based approach to eliminate subtours #footnote[In the context of the TSP, a subtour is a closed route that only visits a subset of the cities, not all of them, and is not connected to the rest of the route. These are invalid solutions and need to be eliminated to find the optimal tour.]\;
- the one introduced by Miller, Tucker, and Zemlin (MTZ) @miller-tucker-zemlin-1960, which uses additional variables representing the order or position of each city in the tour; the elimination is achieved by ensuring that these variables maintain a sequential order, thus preventing subtours.

The problems built according to these two formulations are then fed to the IBM CPLEX 22.11 solver through its C++ API.

== Implementation details
This section provides an overview of some details regarding the exact approach, including certain aspects of the practical implementation.

=== Graph representation
A `Graph` class is used to represent the problem instances, containing a vector of `Node` objects, each representing a hole in the board. This class provides a convenient way to read input instances from files and store both the coordinates of points and their Euclidean distance.

=== Formulations
The `Formulation` class is an abstract base class that defines the interface for the two specific formulations: `GavishGraves` and `MillerTuckerZemlin`. Each formulation implements the methods necessary to create the variables and constraints required to solve the @TSP. Furthermore, it contains common logic to setup CPLEX, solve the problem and export the solution.

=== Time logger
To ease the process of measuring the time taken by the solver to create the model and solve it, a `TimeLogger` class is provided. Once it has been instantiated and started, calling the `tick` method with a message will log the elapsed time since the last tick or the instantiation of the logger. At the end of the process, the `log_total_time` can be called to display the total time taken by the process.

=== Variables and constraints creation
Particular attention was given to the creation of variables and constraints. This section provides an overview of how the variables and constraints are created, including considerations for performance and memory usage.

==== Maps
Internally, CPLEX considers variables to be all stored in a single vectorial structure; it is therefore not straightforward to associate the variable's position in the vector with the corresponding indexes in the problem, after the variable has been created. To address this, both formulations use matrices to store, for each pair of coordinates (i, j), the variable's index in the vector. This allows for easy access to the variables when creating constraints or retrieving solutions.
The two formulations differ in how they store these indices:
- for the Gavish and Graves formulation, two integer matrices are used, holding indices for variables $x_(i j)$ and $y_(i j)$\;
- in the Miller, Tucker, and Zemlin implementation instead, a single integer matrix is used to hold the indices for the variables $x_(i j)$, and a vector memorizes indices for the order variables $u_i$.

==== Optimizations
// How variables and constraints are created (all at once), minimum number necessary

For optimal performance and memory efficiency, both formulations create all variables at once, rather than one by one. This approach minimizes the overhead associated with multiple calls to the CPLEX API, which can be costly in terms of performance. To do so, the formulations leverage the custom implemented `Constraints` and `Variables` support classes, which are iteratively filled with the necessary data, which is then fed to CPLEX in a single call.


Furthermore, only the minimum number of variables and constraints necessary to solve the problem are created. Assuming $N$ to be the number of nodes in the problem, below is a summary of the number of variables and constraints created by each formulation.

===== Gavish and Graves formulation
The Gavish and Graves formulation creates the following variables:
+ $y_(i j)$ variables: $n (n - 1)$, given that they are created for each pair of nodes $(i, j)$ with $i != j$\;
+ $x_(i j)$ variables: $(n - 1)^2$ variables, given that they are created for each pair of nodes $(i, j)$ with $i != j$ starting from $j = 1$, since $j = 0$ corresponds to the depot node.
So in total, $n (n - 1) + (n - 1)^2 = (n - 1)(2n - 1)$ variables are created.

For constraints instead, it creates:
+ flow conservation: $(n - 1)$ constraints, applied for all nodes except node 0 (depot)\;
+ outgoing degree: $n$ constraints, one per node ensuring exactly one outgoing edge\;
+ incoming degree: $n$ constraints, one per node ensuring exactly one incoming edge\;
+ flow-edge coupling: $n(n - 1)$ constraints, created for all pairs $(i, j)$ where both $x_(i j)$ and $y_(i j)$ variables exist.

So in total, the Gavish and Graves formulation creates $(n - 1) + n + n + n(n - 1) =n^2 + 2 n - 1  $ constraints.

===== Miller, Tucker, and Zemlin formulation
#text("TODO: add details for MTZ formulation", size: 14pt, red)
#import "../glossary.typ": defs
#import "@preview/glossy:0.8.0": init-glossary

#show : init-glossary.with(defs)

= Exact approach
With regard to the exact approach, two compact formulations were implemented: 
- the one proposed by @GG @gavish-graves-1978, which uses a network flow-based approach to eliminate subtours #footnote[In the context of the TSP, a subtour is a closed route that only visits a subset of the cities, not all of them, and is not connected to the rest of the route. These are invalid solutions and need to be eliminated to find the optimal tour.]\;
- the one introduced by @MTZ @miller-tucker-zemlin-1960, which uses additional variables representing the order or position of each city in the tour; the elimination is achieved by ensuring that these variables maintain a sequential order, thus preventing subtours.

The problems built according to these two formulations are then fed to the IBM CPLEX 22.11 solver through its C++ API.

== Implementation details
This section provides an overview of some details regarding the exact approach, including certain aspects of the practical implementation.

=== Graph representation <sec:graph>
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
- for the @GG formulation, two integer matrices are used, holding indices for variables $x_(i j)$ and $y_(i j)$\;
- in the @MTZ implementation instead, a single integer matrix is used to hold the indices for the variables $x_(i j)$, and a vector memorizes indices for the order variables $u_i$.

==== Optimizations
For optimal performance and memory efficiency, both formulations create all variables at once, rather than one by one. This approach minimizes the overhead associated with multiple calls to the CPLEX API, which can be costly in terms of performance. To do so, the formulations leverage the custom implemented `Constraints` and `Variables` support classes, which are iteratively filled with the necessary data, which is then fed to CPLEX in a single call.


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
+ flow-edge coupling: $n(n - 1)$ constraints, created for all pairs $(i, j)$ where both $x_(i j)$ and $y_(i j)$ variables exist.

So in total, $(n - 1) + n + n + n(n - 1) = n^2 + 2 n - 1$ constraints are created.

===== @MTZ:long formulation
The @MTZ formulation creates the following variables:
+ $x_(i j)$: $n (n - 1)$ variables, given that they are created for each pair of nodes $(i, j)$ with $i != j$\;
+ $u_i$: $n - 1$ variables, given that they are created for each node $i$ except the depot node 0.

In total, the @MTZ formulation creates $n (n - 1) + (n - 1) = n^2 - 1$ variables.

For what concerns the constraints, it creates:
+ outgoing degree: $n$ constraints, one per node ensuring exactly one outgoing edge\;
+ incoming degree: $n$ constraints, one per node ensuring exactly one incoming edge\;
+ subtour elimination: $(n - 1)^2 - (n - 1)$ constraints, created for all pairs $(i, j)$ where $i != j$ and both $i, j != 0$\;
+ lower bound for order variables: $(n - 1)$ constraints, setting $u_i >= 1$ for each node $i != 0$\;
+ upper bound for order variables: $(n - 1)$ constraints, setting $u_i <= n - 1$ for each node $i != 0$.

In total, the @MTZ formulation creates $n + n + (n-1)^2 - (n-1) + 2(n-1) = n (n - 1)$ constraints.

===== Comparison
To understand which implementation is more efficient in terms of memory, the number of variables and constraints created by each formulation can be compared.
- For what concerns variables, @GG produces $2 n^2 - 3 n + 1$, while @MTZ: $n^2 - 1$. Therefore, given that 
  $ 2 n ^2 - 3 n + 1 < n^2 - 1 => 1 < n < 2 $
  the @MTZ formulation is, in practice, producing less variables than @GG with equal size of the problem.

- For what concerns constraints, @GG produces $n^2 + 2 n - 1$, while @MTZ: $n (n - 1)$. Therefore, given that
  $ n^2 + 2 n - 1 < n (n - 1) => n < 1/3 $
  the @MTZ formulation is producing less constraints than @GG with equal size of the problem.

== Results
#text("TODO results benchmarks", size: 14pt, red)
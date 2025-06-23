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
#text("TODO", size: 14pt, red)

=== Variables and constraints creation
==== Maps
#text("TODO", size: 14pt, red)
// Maps used to create variables
==== Optimizations
#text("TODO", size: 14pt, red)
// How variables and constraints are created (all at once), minimum number necessary
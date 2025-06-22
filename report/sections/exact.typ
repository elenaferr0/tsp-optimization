= Exact approach
With regard to the exact approach, two compact formulations were implemented: 
- the one proposed by Gavish and Graves (GG) @gavish-graves-1978, which uses a network flow-based approach to eliminate subtours #footnote[In the context of the TSP, a subtour is a closed route that only visits a subset of the cities, not all of them, and is not connected to the rest of the route. These are invalid solutions and need to be eliminated to find the optimal tour.]\;
- the one introduced by Miller, Tucker, and Zemlin (MTZ) @miller-tucker-zemlin-1960, which uses additional variables representing the order or position of each city in the tour; the elimination is achieved by ensuring that these variables maintain a sequential order, thus preventing subtours.

== Implementation details
#text("TODO", size: 14pt, red)

== Variables and constraints creation
=== Maps
#text("TODO", size: 14pt, red)
// Maps used to create variables
=== Optimizations
#text("TODO", size: 14pt, red)
// How variables and constraints are created (all at once), minimum number necessary
#import "../glossary.typ": defs
#import "@preview/glossy:0.8.0": init-glossary

#show : init-glossary.with(defs)

= Genetic algorithm approach
#text("TODO introduction", size: 14pt, red)
// Mention that it's implemented in C++

// The Genetic Algorithm uses several operators to evolve the population of solutions: selection, crossover, mutation and replacement. 
== Implementation details
#text("TODO chromosome", size: 14pt, red)
// Chromosome
// Parameters
// Logger

=== Population initialization
The number of chromosomes in the initial population is determined by the `population_size` parameter and remains constant throughout the algorithm. Two techniques are used in conjunction to initialize the population:
- Random Initialization: generates a random permutation of cities for each chromosome, ensuring that all cities are included in the tour. This method provides a diverse starting point for the algorithm;
- Convex Hull Initialization: involves generating a convex hull, forming an initial partial route using the cities on the boundary of the hull. The remaining interior cities are then inserted into this partial tour one by one. Each of them is inserted at the position that results in the minimum incremental cost, calculated by finding the smallest increase in distance when placing the city between two existing cities in the partial tour.

Through empirical testing, it has been observed that employing the Convex Hull Initialization approach exclusively yielded an initial population suffering from poor diversity. This was primarily due to the fact that this technique tends to produce similar chromosomes, as it starts to create tours based on the convex hull, which is the same for all chromosomes.  To mitigate this issue, the Random Initialization technique is used to introduce diversity into the population. The final population is then formed by combining both initialization methods, ensuring a diverse set of chromosomes that can effectively explore the solution space. The ratio of combination between the two is regulated through the `convex_hull_random_init_ratio` parameter.

=== Genetic Operators
This section describes the implemented operators used in the Genetic Algorithm.
==== Selection
The goal of the selection operator is to choose individuals from the current population to create a new generation. The selection process is based on the fitness of each individual, which is determined by the total distance of the tour represented by the chromosome. This phase should aim at identifying the fittest individuals, however, to avoid premature convergence, it is also important to maintain diversity between the solutions. The following selection schemes have been implemented:
- Linear Ranking Selection: sorts individuals by increasing fitness and assign a rank $sigma_i$ to each individual $i$ in the population. The probability of selection is then given by $p_i = (2 sigma_i)/(N (N+1))$, where $N$ is the population size;
- n-Tournament Selection: randomly selects $n$ individuals from the population and chooses the one with the best fitness. The process is repeated until the desired number of individuals is selected. The value of $n$ can be tuned to balance exploration and exploitation.

==== Crossover
The aim of the crossover operator is to combine two parent chromosomes to create offspring that inherit characteristics from both parents. The following crossover methods have been implemented:
- Order Crossover (OX): selects a random subsection from one parent and copies it directly to the offspring. The remaining cities are then filled in the order they appear in the other parent, preserving the relative order of cities outside the copied section;

- Edge Recombination Crossover (ERX): focuses on preserving edges rather than simply city positions. It constructs an offspring by prioritizing edges present in either parent. It works by building a list of neighboring cities for each city from both parents and then iteratively selecting the next city based on which has the fewest available unvisited neighbors @larranaga-genetic-algorithms-TSP-1999.

==== Mutation
Mutation is intended to introduce variability into the population by altering the chromosomes of individuals. This helps to maintain genetic diversity and prevent premature convergence. The following mutation methods have been implemented:
- Simple Inversion Mutation: selects a random subsection of the chromosome and reverses the order of the cities within that segment. This operation changes the order of a contiguous block of cities while keeping their relative positions outside the inverted segment intact @larranaga-genetic-algorithms-TSP-1999;

- Displacement Mutation: involves selecting a random subtour from the chromosome and then reinserting it at a different, randomly chosen position. The cities outside the displaced subtour remain in their original relative order, and the displaced segment is inserted without altering its internal order.

==== Replacement
In the replacement phase, the new generation of individuals is created by replacing some or all of the individuals in the current population with the offspring generated by the crossover and mutation operators. The following replacement strategies have been implemented:

- Steady State Replacement: replaces the worst-performing individuals in the population with the newly created offspring. This approach ensures that the best individuals are retained while allowing for the introduction of new genetic material;

- Elitism Replacement: keeps few of the best individuals from the current population and replaces the rest with offspring.

== Parameters
Below is a list of the parameters that can be tuned in the Genetic Algorithm implementation, along with their default values and descriptions.

#let params = (
  population_size: (default: 100, description: "The number of chromosomes in the population."),
  mutation_rate: (default: 0.01, description: "The probability of mutating a chromosome."),
  parents_replacement_rate: (default: 0.8, description: "The proportion of parents that are replaced in the next generation."),
  selection_n_parents: (default: 300, description: "The number of parents selected for crossover in each generation."),
  selection_tournament_size: (default: 5, description: "The size of the tournament for the n-Tournament Selection method."),
  time_limit_seconds: (default: 60, description: "The maximum time allowed for the algorithm to run, in seconds."),
  max_non_improving_generations: (default: 100, description: "The maximum number of generations without improvement before stopping the algorithm."),
  convex_hull_random_init_ratio: (default: $(0.1, 0.9)$, description: "The ratio of convex hull initialization to random initialization in the population initialization phase. The first value is the proportion of chromosomes initialized using the convex hull method, while the second value is the proportion initialized randomly."),
)

#table(
  columns: 3,
  table.header("Parameter", "Default", "Description"),
  ..params.pairs().map(((param_name, ((default, description)))) => 
    (raw(param_name), [#default], [#description])
  ).flatten(),
)

=== Parameter tuning
To ease the parameter tuning process, a `GridSearch` class heavily inspired by the one from Python's `scikit-learn` library has been implemented #footnote[https://scikit-learn.org/stable/modules/generated/sklearn.model_selection.GridSearchCV.html]. Given a problem instance, it allows a systematic search over a specified set of parameter, evaluating the performance of the algorithm for each combination. The performance is measured using a fitness function, which, in this case, is the total distance of the tour represented by the chromosome.

This process is repeated for instances of different sizes, so to obtain a more general understanding of the algorithm's performance. Clearly, to obtain meaningful results several runs should be performed on different instance sizes, since the algorithm has stochastic components. The results are then averaged over the different executions, and the best parameters are selected based on the average performance.

#text("TODO: parameter tuning results", size: 14pt, red)
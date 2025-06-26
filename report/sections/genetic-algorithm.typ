#import "../glossary.typ": defs
#import "@preview/glossy:0.8.0": init-glossary

#show: init-glossary.with(defs)

= @GA:long approach
A C++ implementation of a @GA:long is proposed as an alternative approach to solve the @TSP. The goal is to provide a faster way to find good solutions for large instances, which are too expensive to solve using exact methods. The @GA is a metaheuristic that mimics the process of natural selection, where the fittest individuals from a population of solutions are selected to create new generations.

== Implementation details

=== Chromosomes
The implemented @GA represents solutions as a vector of nodes, where each of them corresponds to a city in the @TSP. A `Graph` class based on the one described in #ref(<sec:graph>) holds the coordinates of the cities and their Euclidean distances. To ease the process of evaluating the fitness, overriding a part of the path and saving the solution to file, a `Chromosome` class has been implemented as a wrapper around the `Graph` class.

// #text("TODO logger", size: 14pt, red)
// Parameters
// Logger

=== Algorithm overview
To render the @GA implementation more flexible and customizable, it has been designed to allow different operators to be plugged in for selection, crossover, mutation and replacement. The `GeneticAlgorithm` class orchestrates the execution of the algorithm, initializing the population and applying the operators in each generation.

The `start` method of the `GeneticAlgorithm` class constitutes the algorithm's entry point. It takes an hyperparameter configuration `HyperParam` as input, and will take care of initializing the population, applying the selection, crossover, mutation and replacement operators. Finally, it returns the best solution found once the stopping criteria are met.

==== A note on population initialization and stopping criteria
Given the necessity to run the algorithm using different techniques for population initialization and stopping criteria at the same time, the `GeneticAlgorithm` class has been designed to allow these to be provided as vectors, instead of single class instances. More specifically:
- the population initialization can be performed using multiple techniques, better described in #ref(<sec:population-init>), and the final population is formed by combining the results of these techniques;
- the stopping criteria can be defined as a set of different conditions, such as a time limit or a maximum number of generations without improvement. The algorithm will stop when any of these conditions is met.

==== Parameters
#ref(<tab:params>) contains the list of the parameters that can be tuned in the @GA implementation, along with their default values and descriptions.

#let params = (
  population_size: (default: 100, description: "The number of chromosomes in the population."),
  mutation_rate: (default: 0.01, description: "The probability of mutating a chromosome."),
  parents_replacement_rate: (
    default: 0.8,
    description: "The proportion of parents that are replaced in the next generation.",
  ),
  selection_n_parents: (default: 300, description: "The number of parents selected for crossover in each generation."),
  selection_tournament_size: (
    default: 5,
    description: "The size of the tournament for the n-Tournament Selection method. This is only relevant if the Selection method is set to N-Tournament Selection.",
  ),
  time_limit_seconds: (default: 60, description: "The maximum time allowed for the algorithm to run, in seconds."),
  max_non_improving_generations: (
    default: 100,
    description: "The maximum number of generations without improvement before stopping the algorithm.",
  ),
  convex_hull_random_init_ratio: (
    default: $(0.1, 0.9)$,
    description: "The ratio of convex hull initialization to random initialization in the population initialization phase. The first value is the proportion of chromosomes initialized using the convex hull method, while the second value is the proportion initialized randomly.",
  ),
)

#figure(
  table(
    columns: 3,
    table.header("Parameter", "Default", "Description"),
    ..params
      .pairs()
      .map(((param_name, (default, description))) => (raw(param_name), [#default], [#description]))
      .flatten(),
  ),
  caption: "Parameters of the Genetic Algorithm implementation",
) <tab:params>

Note that `population_size`, `parents_replacement_rate`, `selection_n_parents`, `time_limit_seconds` and `max_non_improving_generations` are not actually changed during the parameter tuning process, but are set to the default values. They have been included in the list as they are relevant to the algorithm's performance and can be manually adjusted if needed.

=== Population initialization <sec:population-init>
The number of chromosomes in the initial population is determined by the `population_size` parameter and remains constant throughout the algorithm. Two techniques are used in conjunction to initialize the population:
- Random Initialization: generates a random permutation of cities for each chromosome, ensuring that all cities are included in the tour. This method provides a diverse starting point for the algorithm;
- Convex Hull Initialization: involves generating a convex hull, forming an initial partial route using the cities on the boundary of the hull. The remaining interior cities are then inserted into this partial tour one by one. Each of them is inserted at the position that results in the minimum incremental cost, calculated by finding the smallest increase in distance when placing the city between two existing cities in the partial tour @convex-hull-2023.

It has been observed by empirical tests that employing the Convex Hull Initialization method alone can lead to poor diversity in the initial population. This was primarily due to the fact that it starts to create tours based on the convex hull, which is the same for all chromosomes. The randomization introduced by inserting the internal cities in different positions is not sufficient to ensure a diverse population. To mitigate this issue, the Random Initialization technique has been integrated.  The final population is then formed by combining both initialization methods, ensuring a diverse set of chromosomes that can effectively explore the solution space. The ratio of combination between the two is regulated through the `convex_hull_random_init_ratio` parameter.

=== Selection
The goal of the selection operator is to choose individuals from the current population to create a new generation. The selection process is based on the fitness of each individual, which is determined by the total distance of the tour represented by the chromosome. This phase should aim at identifying the fittest individuals, however, to avoid premature convergence, it is also important to maintain diversity between the solutions. The following selection schemes have been implemented:
- Linear Ranking Selection: sorts individuals by increasing fitness and assign a rank $sigma_i$ to each individual $i$ in the population. The probability of selection is then given by $p_i = (2 sigma_i)/(N (N+1))$, where $N$ is the population size;
- n-Tournament Selection: randomly selects $n$ individuals from the population and chooses the one with the best fitness. The process is repeated until the desired number of individuals is selected. The value of $n$ can be tuned to balance exploration and exploitation.

=== Crossover
The aim of the crossover operator is to combine two parent chromosomes to create offspring that inherit characteristics from both parents. The following crossover methods have been implemented:
- Order Crossover (OX): selects a random subsection from one parent and copies it directly to the offspring. The remaining cities are then filled in the order they appear in the other parent, preserving the relative order of cities outside the copied section;

- Edge Recombination Crossover (ERX): focuses on preserving edges rather than simply city positions. It constructs an offspring by prioritizing edges present in either parent. It works by building a list of neighboring cities for each city from both parents and then iteratively selecting the next city based on which has the fewest available unvisited neighbors @larranaga-genetic-algorithms-TSP-1999.

=== Mutation
Mutation is intended to introduce variability into the population by altering the chromosomes of individuals. This helps to maintain genetic diversity and prevent premature convergence. The following mutation methods have been implemented:
- Simple Inversion Mutation: selects a random subsection of the chromosome and reverses the order of the cities within that segment. This operation changes the order of a contiguous block of cities while keeping their relative positions outside the inverted segment intact @larranaga-genetic-algorithms-TSP-1999;

- Displacement Mutation: involves selecting a random subtour from the chromosome and then reinserting it at a different, randomly chosen position. The cities outside the displaced subtour remain in their original relative order, and the displaced segment is inserted without altering its internal order.

=== Replacement
In the replacement phase, the new generation of individuals is created by replacing some or all of the individuals in the current population with the offspring generated by the crossover and mutation operators. The following replacement strategies have been implemented:

- Steady State Replacement: replaces the worst-performing individuals in the population with the newly created offspring. This approach ensures that the best individuals are retained while allowing for the introduction of new genetic material;

- Elitism Replacement: keeps few of the best individuals from the current population and replaces the rest with offspring.

=== Parameter tuning
To ease the parameter tuning process, a `GridSearch` class heavily inspired by the one from Python's `scikit-learn` library has been implemented #footnote[https://scikit-learn.org/stable/modules/generated/sklearn.model_selection.GridSearchCV.html]. Given a problem instance, it allows a systematic search over a specified set of parameter, evaluating the performance of the algorithm for each combination. The performance is measured using a fitness function, which, in this case, is the total distance of the tour represented by the chromosome.

// #text("TODO: parameter tuning results", size: 14pt, red)

By running the `GridSearch` class with different configurations for the randomly generated instances, it is possible to identify the best set of parameters for the @GA implementation. The chosen configuration consistently performed the best across all instances, with the exception of `random_10`, where `Edge Recombination Crossover` outperformed `Order Crossover`.

 The results of this tuning process are summarized below:
- Selection: `LinearRankingSelection`;
- Crossover: `OrderCrossover`;
- Mutation: `DisplacementMutation` with rate $0.1$;
- Replacement: `ElitismReplacement` with rate $0.3$;
- Convex Hull to Random Initialization Ratio: $(0.2, 0.8)$.
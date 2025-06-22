= Computational results
// Computational results,  (tables and comments)
  // how long/far from optimal, how long it took in two cases

#let results = (
  (problem_name: "random_5", n_nodes: 5, exact_solution: 14.073648, genetic_solution: 14.073648, exact_time: "", genetic_time: 1.563),
)

#table(
  columns: 3,
  table.header("Parameter", "Default", "Description"),
  ..results.pairs().map(((param_name, ((default, description)))) => 
    (raw(param_name), [#default], [#description])
  ).flatten(),
)
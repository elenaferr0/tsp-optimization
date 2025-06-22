#import "../utils.typ": *
#import "@elenaferr0/plotst:0.2.0": graph_plot, axis, plot


= Computational results
// Computational results,  (tables and comments)
  // how long/far from optimal, how long it took in two cases

#let results = (
  (problem_name: "random_5", nodes: 5, exact_solution: 14.073648, genetic_solution: 14.073648, exact_time: 32, genetic_time: 194),
  (problem_name: "random_10", nodes: 10, exact_solution: 40.217334, genetic_solution: 40.887331, exact_time: 97, genetic_time: 367),
  (problem_name: "random_20", nodes: 20, exact_solution: 140.953759, genetic_solution: 214.234813, exact_time: 222, genetic_time: 901),
)

#table(
  columns: 6,
  align: (left, right, right, right, right, right),
  table.header(
    "Problem name", "Exact solution", "Genetic solution", "Exact time (ms)", "Genetic time (ms)", "Gap (%)"
  ),
  ..results.map(((problem_name, exact_solution, genetic_solution, exact_time, genetic_time)) => {
    let gap = if exact_solution == 0 {
        "N/A"
      } else {
        format((genetic_solution - exact_solution) / exact_solution * 100, 2)
      }
    (raw(problem_name), [#format(exact_solution, 4)], [#format(genetic_solution, 4)], [#exact_time\ms], [#genetic_time\ms], [#gap%])
    }
  ).flatten(),
)

#let graph_plot_test() = {
    let data = (
      (0, 4), (2, 2), (3, 0), (4, 4), (5, 7), (6, 6), (7, 9), (8, 5), (9, 9), (10, 1)
    )
    let data2 = (
      (0, 0), (2, 2), (3, 1), (4, 4), (5, 2), (6, 6), (7, 5), (8, 7), (9, 10), (10, 3)
    )
    let x_axis = axis(min: 0, max: 11, step: 2, location: "bottom")
    let y_axis = axis(min: 0, max: 11, step: 2, location: "left", helper_lines: false)
    let pl = plot(data: data, axes: (x_axis, y_axis))
    graph_plot(pl, (100%, 25%), markings: [])
    graph_plot(pl, (100%, 25%), rounding: 30%, caption: "Graph Plot with caption and rounding", markings: [#emoji.rocket])
}

#graph_plot_test()
#import "../utils.typ": *
#import "@elenaferr0/plotst:0.2.0": plot, overlay, graph_plot, axis


= Computational results
// Computational results,  (tables and comments)
// how long/far from optimal, how long it took in two cases

#let results = (
  (
    problem_name: "random_5",
    nodes: 5,
    exact_solution: 14.073648,
    genetic_solution: 14.073648,
    exact_time: 32,
    genetic_time: 194,
  ),
  (
    problem_name: "random_10",
    nodes: 10,
    exact_solution: 40.217334,
    genetic_solution: 40.887331,
    exact_time: 97,
    genetic_time: 367,
  ),
  (
    problem_name: "random_20",
    nodes: 20,
    exact_solution: 140.953759,
    genetic_solution: 214.234813,
    exact_time: 222,
    genetic_time: 901,
  ),
  (
    problem_name: "random_50",
    nodes: 50,
    exact_solution: 599.882932,
    genetic_solution: 1847.670084,
    exact_time: 4597,
    genetic_time: 3822,
  ),
  (
    problem_name: "random_100",
    nodes: 100,
    exact_solution: 1288.580487,
    genetic_solution: 6090.911625,
    exact_time: 112441,
    genetic_time: 9172,
  ),
)

#let results_with_gap = results.map(((problem_name, nodes, exact_solution, genetic_solution, exact_time, genetic_time)) => {
  let gap = (genetic_solution - exact_solution) / exact_solution * 100
  (problem_name: problem_name, nodes: nodes, exact_solution: exact_solution, genetic_solution: genetic_solution, exact_time: exact_time, genetic_time: genetic_time, gap: gap)
})

#table(
  columns: 6,
  align: (left, right, right, right, right, right),
  table.header(
    "Problem name",
    "Exact solution",
    "Genetic solution",
    "Exact time (ms)",
    "Genetic time (ms)",
    "Gap (%)",
  ),
  ..results_with_gap
    .map(((problem_name, exact_solution, genetic_solution, exact_time, genetic_time, gap)) => {
      (
        raw(problem_name),
        [#format(exact_solution, 4)],
        [#format(genetic_solution, 4)],
        [#exact_time\ms],
        [#genetic_time\ms],
        [#format(gap, 2)%],
      )
    })
    .flatten(),
)


#{
  let exact = results_with_gap.map(((nodes, exact_time)) => (nodes, exact_time))
  let genetic = results_with_gap.map(((nodes, genetic_time)) => (nodes, genetic_time))

  let x_max = calc.max(..exact.map(((nodes, exact_time)) => nodes))
  let y_max = calc.max(
    ..exact.map(((nodes, exact_time)) => exact_time),
    ..genetic.map(((nodes, genetic_time)) => genetic_time)
  )

  let x_axis = axis(min: 0, max: x_max, step: 10, location: "bottom", helper_lines: false, title: "Number of Nodes", marking_length: 2pt)
  let y_axis = log-axis(exact, location: "left", helper_lines: false, title: "Time (ms) - log scale")

  let exact_plot = plot(data: log-data(exact), axes: (x_axis, y_axis))
  let genetic_plot = plot(data: log-data(genetic), axes: (x_axis, y_axis))

  let exact_graph_plot = graph_plot(exact_plot, (100%, 25%), rounding: 10%, caption: "Exact vs Genetic Algorithm solution time", stroke: red)
  let genetic_graph_plot = graph_plot(genetic_plot, (100%, 25%), rounding: 30%, stroke: blue)

  stack(
    dir: rtl,
    spacing: -450pt,
    box(
      inset: 10pt, 
      stroke: black,
      [
        #text(sym.square.filled, red) #text("Exact time")
        #linebreak()
        #text(sym.square.filled, blue) #text("Genetic Algorithm time")
      ]
    ),
    overlay((exact_graph_plot, genetic_graph_plot), (100%, 25%))
  )
}

#{
  let gap_by_nodes = results_with_gap.map(((nodes, gap)) => (nodes, gap))
  let x_max = calc.max(..gap_by_nodes.map(((nodes, gap)) => nodes))

  let x_axis = axis(min: 0, max: x_max, step: 10, location: "bottom", helper_lines: false, title: "Number of Nodes", marking_length: 2pt)
  let y_axis = log-axis(gap_by_nodes, location: "left", helper_lines: false, title: "Gap (%) - log scale")

  let plot = plot(data: log-data(gap_by_nodes), axes: (x_axis, y_axis))

  let graph_plot = graph_plot(plot, (100%, 25%), rounding: 10%, caption: "Optimality gap for Genetic Algorithm", stroke: blue)

  stack(
    dir: rtl,
    spacing: -450pt,
    box(
      inset: 10pt, 
      stroke: black,
      [
        #text(sym.square.filled, blue) #text("Gap (%)")
        #linebreak()
      ]
    ),
    graph_plot
  )
}
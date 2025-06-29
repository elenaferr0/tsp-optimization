#import "../utils.typ": *
#import "@elenaferr0/plotst:0.2.0": axis, graph_plot, overlay, plot

= Computational results
In this section, a performance comparison between the exact and @GA approaches is carried out.

Since the implemented @GA has stochastic components, its results can vary between runs. The algorithm was executed five times for each instance and parameter combination, and the average results were used in order to obtain a more reliable performance estimate. To reproduce this process, the `benchmark.sh` script can be used, found in the `part_two` folder of the project. It takes care of running the @GA with the best parameters found in the previous section, and saving the results in the `results` folder. The script can be executed with the following command:
```bash
sh benchmark.sh
```

From the results summarized in #ref(<tab:results>), it emerges that the @GA is able to provide a sufficiently accurate solution for problems of reduced size - below 10 nodes - although with a much longer computational time than the exact method. This might be accounted for by the fact that the exponential growth of the search space for the @TSP is not yet significant for such small instances, allowing the exact method to find the optimal solution in a reasonable time.

For larger instances on the other hand, the @GA is able to produce a solution much quicker, but with an excessively large optimality gap, which can be as high as 351% for the largest considered instance (100 nodes).

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
    genetic_solution: 40.245537,
    exact_time: 97,
    genetic_time: 367,
  ),
  (
    problem_name: "random_20",
    nodes: 20,
    exact_solution: 140.953759,
    genetic_solution: 212.932003,
    exact_time: 222,
    genetic_time: 901,
  ),
  (
    problem_name: "random_50",
    nodes: 50,
    exact_solution: 599.882932,
    genetic_solution: 1678.920023,
    exact_time: 4597,
    genetic_time: 3822,
  ),
  (
    problem_name: "random_100",
    nodes: 100,
    exact_solution: 1288.580487,
    genetic_solution: 5817.054886,
    exact_time: 112441,
    genetic_time: 9172,
  ),
)

#let results_with_gap = results.map((
  (problem_name, nodes, exact_solution, genetic_solution, exact_time, genetic_time),
) => {
  let gap = (genetic_solution - exact_solution) / exact_solution * 100
  (
    problem_name: problem_name,
    nodes: nodes,
    exact_solution: exact_solution,
    genetic_solution: genetic_solution,
    exact_time: exact_time,
    genetic_time: genetic_time,
    gap: gap,
  )
})

#figure(
  table(
    columns: 6,
    align: (left, right, right, right, right, right),
    table.header(
      "Problem name", "Exact solution", "Genetic solution", "Exact time (ms)", "Genetic time (ms)", "Gap (%)"
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
  ),
  caption: "Results summary for the exact and genetic algorithm approaches to the TSP",
) <tab:results>

#ref(<plt:exact>) highlights that up to 50 nodes, the exact method produces a solution in a shorter time than the @GA, while for larger instances. From the limited extent of this chart, it seems that the time taken by the exact method grows much faster than that of the @GA, with the increase in the number of nodes. This is quite sensible given the nature of the @TSP.

#{
  let exact = results_with_gap.map(((nodes, exact_time)) => (nodes, exact_time))
  let genetic = results_with_gap.map(((nodes, genetic_time)) => (nodes, genetic_time))

  let x_max = calc.max(..exact.map(((nodes, exact_time)) => nodes))
  let y_max = calc.max(..exact.map(((nodes, exact_time)) => exact_time), ..genetic.map(((nodes, genetic_time)) => {
    genetic_time
  }))

  let x_axis = axis(
    min: 0,
    max: x_max,
    step: 10,
    location: "bottom",
    helper_lines: false,
    title: "Number of Nodes",
    marking_length: 2pt,
  )
  let y_axis = log-axis(exact, location: "left", helper_lines: false, title: "Time (ms) - log scale")

  let exact_plot = plot(data: log-data(exact), axes: (x_axis, y_axis))
  let genetic_plot = plot(data: log-data(genetic), axes: (x_axis, y_axis))

  let exact_graph_plot = graph_plot(
    exact_plot,
    (100%, 25%),
    rounding: 5%,
    caption: "Exact vs Genetic Algorithm solution time",
    stroke: red,
  )
  let genetic_graph_plot = graph_plot(genetic_plot, (100%, 25%), rounding: 30%, stroke: blue)

  block(breakable: false, stack(
    dir: rtl,
    spacing: -450pt,
    box(inset: 10pt, stroke: black, [
      #text(sym.square.filled, red) #text("Exact time")
      #linebreak()
      #text(sym.square.filled, blue) #text("Genetic Algorithm time")
    ]),
    overlay(
      (exact_graph_plot, genetic_graph_plot),
      (100%, 25%),
      label_str: "plt:exact",
    ),
  ))
}

#ref(<plt:optimality-gap>) instead shows the growth of the optimality gap for the @GA with respect to the number of nodes. The increase is steeper for the first 20 nodes, after which it tends to stabilize and become more linear. After 20 nodes, the search space becomes too large for the @GA to maintain a low gap, leading to a significant increase in the gap values.

Both the discussed plots are in logarithmic scale, to allow a better visualization of the results, given the large differences in time and gap values.

#{
  let gap_by_nodes = results_with_gap.map(((nodes, gap)) => (nodes, gap))

  let x_max = calc.max(..gap_by_nodes.map(((nodes, gap)) => nodes))

  let x_axis = axis(
    min: 0,
    max: x_max,
    step: 10,
    location: "bottom",
    helper_lines: false,
    title: "Number of Nodes",
    marking_length: 2pt,
  )
  let y_axis = log-axis(gap_by_nodes, location: "left", helper_lines: false, title: "Gap (%) - log scale")

  let plot = plot(data: log-data(gap_by_nodes), axes: (x_axis, y_axis))

  let graph_plot = graph_plot(
    plot,
    (100%, 25%),
    rounding: 5%,
    caption: "Optimality gap for Genetic Algorithm",
    stroke: blue,
  )

  block(breakable: false, stack(
    dir: rtl,
    spacing: -450pt,
    box(inset: 10pt, stroke: black, [
      #text(sym.square.filled, blue) #text("Gap (%)")
      #linebreak()
    ]),
    [#graph_plot <plt:optimality-gap>],
  ))
}

#ref(<img:random-10-exact>) and #ref(<img:random-10-ga>) show the tours produced by the exact method and the @GA for a random instance of 10 nodes. It can be observed that the @GA solution not only isn't optimal, but also contains a subtour starting and ending at node 3. This is clearly a limitation of the implemented @GA approach.

#grid(
  columns: 2,
  [#figure(
    image("../imgs/random_10_tour_exact.png"),
    caption: "Exact solution for the random 10 nodes TSP instance"
  ) <img:random-10-exact>],
  [
    #figure(
      image("../imgs/random_10_tour_ga.png"),
      caption: "Genetic Algorithm solution for the random 10 nodes TSP instance") <img:random-10-ga>
  ],
)

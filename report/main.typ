#import "template.typ": ieee
#import "./glossary.typ": defs
#import "@preview/glossy:0.8.0": init-glossary

#show : init-glossary.with(defs)

#show: ieee.with(
  title: [Exact and @GA:long approaches for the @TSP:long],
  n-columns: 1,
  authors: (
    (
      name: "Elena Ferro",
      department: [ID 2166466],
      organization: [Computer Science, University of Padua],
      location: [Methods and Models for Combinatorial Optimization],
      email: [`elena.ferro.7@studenti.unipd.it`]
    ),
  ),
  bibliography: bibliography("refs.bib"),
  figure-supplement: [Fig.],
  // paper-size: "a4",
  code-font-family: "Liberation Mono",
  // body-font-size: 11pt
)

// Additional custom styles
#show link: underline
#set table(inset: (y: 8pt))
#show table.cell.where(y: 0): strong
#show figure: set block(breakable: true)

#include "sections/introduction.typ"
#include "sections/exact.typ"
#include "sections/genetic-algorithm.typ"
// #include "sections/computational-results.typ"
#include "sections/conclusion.typ"
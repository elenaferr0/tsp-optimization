#import "@preview/glossy:0.8.0": *

#let defs = (
  TSP: (
    long: "Traveling Salesman Problem",
    short: "TSP",
  ),
  PCB: (
    long: "Printed Circuit Board",
    short: "PCB",
    plural: "PCBs",
    longplural: "Printed Circuit Boards",
  ),
)

#show: init-glossary.with(defs)

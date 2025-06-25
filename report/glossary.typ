#import "@preview/glossy:0.8.0": *

#let defs = (
  TSP: (
    long: "Travelling Salesman Problem",
    short: "TSP",
  ),
  PCB: (
    long: "Printed Circuit Board",
    short: "PCB",
    plural: "PCBs",
    longplural: "Printed Circuit Boards",
  ),
  GG: (
    long: "Gavish and Graves",
    short: "GG",
  ),
  MTZ: (
    long: "Miller, Tucker, and Zemlin",
    short: "MTZ",
  ),
  GA: (
    long: "Genetic Algorithm",
    short: "GA",
    plural: "Genetic Algorithms",
  ),
)

#show: init-glossary.with(defs)

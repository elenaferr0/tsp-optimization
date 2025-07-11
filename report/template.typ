// This function gets your whole document as its `body` and formats
// it as an article in the style of the IEEE.
#let ieee(
  // The paper's title.
  title: [Paper Title],
  // An array of authors. For each author you can specify a name,
  // department, organization, location, and email. Everything but
  // but the name is optional.
  authors: (),
  // The paper's abstract. Can be omitted if you don't have one.
  abstract: none,
  // A list of index terms to display after the abstract.
  index-terms: (),
  // The article's paper size. Also affects the margins.
  paper-size: "us-letter",
  // The result of a call to the `bibliography` function or `none`.
  bibliography: none,
  // How figures are referred to from within the text.
  // Use "Figure" instead of "Fig." for computer-related publications.
  figure-supplement: [Fig.],
  // The font used for the paper's text.
  body-font-family: "TeX Gyre Termes",
  // The font used for the paper's code blocks.
  code-font-family: "TeX Gyre Cursor",
  body-font-size: 11.5pt,
  n-columns: 2,
  body,
) = {
  // Set document metadata.
  set document(title: title, author: authors.map(author => author.name))

  // Set the body font.
  // As of 2024-08, the IEEE LaTeX template uses wider interword spacing
  // - See e.g. the definition \def\@IEEEinterspaceratioM{0.35} in IEEEtran.cls
  set text(font: body-font-family, size: body-font-size, spacing: .35em)

  // Enums numbering
  set enum(numbering: "1)A)a)")

  // Tables & figures
  show figure: set block(spacing: 15.5pt)
  show figure: set place(clearance: 15.5pt)
  show figure.where(kind: table): set figure.caption(position: top, separator: [\ ])
  show figure.where(kind: table): set text(size: body-font-size - 2pt)
  show figure.where(kind: table): set figure(numbering: "I")
  show figure.where(kind: image): set figure(supplement: figure-supplement, numbering: "1")
  show figure.caption: set text(size: body-font-size - 2pt)
  show figure.caption.where(kind: table): set align(center)

  // Adapt supplement in caption independently from supplement used for
  // references.
  set figure.caption(separator: [. ])
  show figure: fig => {
    let prefix = (
      if fig.kind == table [TABLE] else if fig.kind == image [Fig.] else [#fig.supplement]
    )
    let numbers = numbering(fig.numbering, ..fig.counter.at(fig.location()))
    show figure.caption: it => [#prefix~#numbers#it.separator#it.body]
    show figure.caption.where(kind: table): smallcaps
    fig
  }

  // Code blocks
  show raw: set text(
    font: code-font-family,
    ligatures: false,
    size: 1em / 0.92,
    spacing: 100%,
  )

  // Configure the page and multi-column properties.
  set columns(gutter: 12pt)
  set page(
    columns: n-columns,
    paper: paper-size,
    // The margins depend on the paper size.
    margin: if paper-size == "a4" {
      (x: 41.5pt, top: 80.51pt, bottom: 89.51pt)
    } else {
      (
        x: (50pt / 216mm) * 100%,
        top: (55pt / 279mm) * 100%,
        bottom: (64pt / 279mm) * 100%,
      )
    },
  )

  // Configure equation numbering and spacing.
  set math.equation(numbering: "(1)")
  show math.equation: set block(spacing: 0.65em)

  // Configure appearance of equation references
  show ref: it => {
    if it.element != none and it.element.func() == math.equation {
      // Override equation references.
      link(
        it.element.location(),
        numbering(
          it.element.numbering,
          ..counter(math.equation).at(it.element.location()),
        ),
      )
    } else {
      // Other references as usual.
      it
    }
  }

  // Configure lists.
  set enum(indent: 10pt, body-indent: 9pt)
  set list(indent: 10pt, body-indent: 9pt)

  // Configure headings.
  set heading(numbering: "I.1.1")
  show heading: set text(weight: 400, style: "italic")
  show heading.where(level: 1): it => {
    set align(center)
    show: smallcaps
    show: block.with(above: 15pt, below: 13.75pt, sticky: true)
    let levels = counter(heading).get()
    let deepest = if levels != () {
      levels.last()
    } else {
      1
    }
    set text(style: "normal")
    if it.numbering != none {
      numbering("I.", deepest)
      h(7pt, weak: true)
    }
    it.body
  }
  // show heading: it => {
  //   // Find out the final number of the heading counter.
  //   let levels = counter(heading).get()
  //   let deepest = if levels != () {
  //     levels.last()
  //   } else {
  //     1
  //   }

  //   set text(12pt, weight: 400)
  //   if it.level == 1 {
  //     // First-level headings are centered smallcaps.
  //     // We don't want to number the acknowledgment section.
  //     let is-ack = it.body in ([Acknowledgment], [Acknowledgement], [Acknowledgments], [Acknowledgements])
  //     set align(center)
  //     set text(if is-ack { body-font-size } else { body-font-size + 2pt })
  //     show: block.with(above: 15pt, below: 13.75pt, sticky: true)
  //     show: smallcaps
  //     if it.numbering != none and not is-ack {
  //       numbering("I.", deepest)
  //       h(7pt, weak: true)
  //     }
  //     it.body
  //   } else if it.level == 2 {
  //     // Second-level headings are run-ins.
  //     set par(first-line-indent: 0pt)
  //     set text(style: "italic")
  //     show: block.with(spacing: 10pt, sticky: true)
  //     if it.numbering != none {
  //       numbering("A.", deepest)
  //       h(7pt, weak: true)
  //     }
  //     it.body
  //   } else [
  //     // Third level headings are run-ins too, but different.
  //     #if it.level == 3 {
  //       numbering("a)", deepest)
  //       [ ]
  //     }
  //     _#(it.body):_
  //   ]
  // }

  // Style bibliography.
  show std.bibliography: set text(body-font-size - 2pt)
  show std.bibliography: set block(spacing: 0.5em)
  set std.bibliography(title: text(body-font-size - 1pt)[References], style: "ieee")

  // Display the paper's title and authors at the top of the page,
  // spanning all columns (hence floating at the scope of the
  // columns' parent, which is the page).
  place(
    top,
    float: true,
    scope: "parent",
    clearance: 30pt,
    {
      v(3pt, weak: true)
      align(center, par(leading: 0.5em, text(size: 24pt, title)))
      v(8.35mm, weak: true)

      // Display the authors list.
      set par(leading: 0.6em)
      for i in range(calc.ceil(authors.len() / 3)) {
        let end = calc.min((i + 1) * 3, authors.len())
        let is-last = authors.len() == end
        let slice = authors.slice(i * 3, end)
        grid(
          columns: slice.len() * (1fr,),
          gutter: 12pt,
          ..slice.map(author => align(
            center,
            {
              text(size: body-font-size, author.name)
              if "department" in author [
                \ #emph(author.department)
              ]
              if "organization" in author [
                \ #emph(author.organization)
              ]
              if "location" in author [
                \ #author.location
              ]
              if "email" in author {
                if type(author.email) == str [
                  \ #link("mailto:" + author.email)
                ] else [
                  \ #author.email
                ]
              }
            },
          ))
        )

        if not is-last {
          v(16pt, weak: true)
        }
      }
    },
  )

  // Configure paragraph properties.
  set par(spacing: 0.45em, justify: true, first-line-indent: 1em, leading: 0.45em)

  // Display abstract and index terms.
  if abstract != none [
    #set text(body-font-size - 1pt, weight: 700, spacing: 150%)
    #h(1em) _Abstract_---#h(weak: true, 0pt)#abstract

    #if index-terms != () [
      #h(.3em)_Index Terms_---#h(weak: true, 0pt)#index-terms.join(", ")
    ]
    #v(2pt)
  ]

  // Display the paper's contents.
  set par(leading: 0.75em, spacing: 1em)
  body

  // Display bibliography.
  bibliography
}


#import "@elenaferr0/plotst:0.2.0": axis

#let format(number, precision) = {
  assert(precision>0)
  let s = str(calc.round(number, digits: precision))
  let after_dot = s.find(regex("\..*"))
  if after_dot==none {
    s=s+"."
    after_dot="."
  }
  for i in range(precision - after_dot.len()+1){
    s=s+"0"
  }
  s
}

#let log-axis(data, location: str, title: str, helper_lines: bool, marking-length: 2pt) = {
  let max = calc.max(..data.map((d) => d.at(1)).flatten())
  let n-values = calc.floor(calc.log(max, base: 10)) + 1
  let values = range(0, n-values).map((i) => calc.pow(10, i))

  return axis(values: values, location: location, title: title, helper_lines: helper_lines)
}

#let log-data(data, base: 10) = {
  return data.map((d) => (d.at(0), if (d.at(1) == 0) { 0 } else { calc.log(d.at(1), base: base) }))
}
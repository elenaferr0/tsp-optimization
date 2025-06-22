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

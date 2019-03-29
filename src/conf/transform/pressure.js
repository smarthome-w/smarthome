(function(i) {
   var pressure = parseFloat(i.replace(",", "."))
   pressure = pressure + 17.59
   return pressure.toString(10)
})(input)

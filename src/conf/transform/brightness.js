(function (i) {
  var brightness = parseFloat(i.replace(",", "."));
  brightness = 1024 - brightness;
  return brightness.toString(10);
})(input);

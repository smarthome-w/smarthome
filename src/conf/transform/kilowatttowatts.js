(function (i) {
  var kw = parseFloat(i.replace(",", "."));
  kw = kw * 1000;
  return kw.toString(10);
})(input);

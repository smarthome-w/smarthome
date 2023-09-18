(function (i, offset) {
  var number = parseFloat(i.replace(",", "."));
  number = number + offset;
  return number.toString(10);
})(input);

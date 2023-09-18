(function (data) {
  var dataString = JSON.parse(data);
  var value = "UNDEF";
  value = dataString.ANALOG.A0;
  value = 1024 - value;
  return value.toString(10);
})(input);

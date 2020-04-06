(function (data) {
  var dataString = JSON.parse(data);
  var value = "UNDEF";
  value = dataString.BMP280.Pressure;
  value = value + 17.59;
  return value.toString(10);
})(input);

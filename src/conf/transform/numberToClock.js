(function (i) {
  var hours = Math.floor(i / 60);
  var minutes = i % 60;

  hours = hours < 10 ? "0" + hours : hours;
  minutes = minutes < 10 ? "0" + minutes : minutes;

  return hours + ":" + minutes;
})(input);

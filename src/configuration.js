var invertFace = window.localStorage.getItem('invertFace') ? window.localStorage.getItem('invertFace') : 0,
    invertRow = window.localStorage.getItem('invertRow') ? window.localStorage.getItem('invertRow') : 0,
    formatDate = window.localStorage.getItem('formatDate') ? window.localStorage.getItem('formatDate') : 0;

function sendToPebble(invertFace, invertRow, formatDate) {  
  var dictionary = {
    'INVERTFACE': invertFace,
    'INVERTROW': invertRow,
    'FORMATDATE' : formatDate
  };
  
  Pebble.sendAppMessage(dictionary);
}

Pebble.addEventListener("showConfiguration", function() {  
  Pebble.openURL('http://spidie.de/pebble/squareface/1.4/configuration.html?invertFace=' + (invertFace == 1 ? 'true' : 'false') 
                                                                        + '&invertRow=' + invertRow
                                                                        + '&formatDate=' + formatDate);
});

Pebble.addEventListener("webviewclosed", function(e) {
  // Using primitive JSON validity and non-empty check
  if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
    var options = JSON.parse(decodeURIComponent(e.response));
    
    // analyze return
    if (options['invert-face'] == "true")
      invertFace = 1;
    else
      invertFace = 0;
    
    if (options['invert-toprow'] === true)
      invertRow = 1;
    else if (options['invert-bottomrow'] === true)
      invertRow = 2;
    else 
      invertRow = 0;
    
    if (options['format-date-mmdd'] === true)
      formatDate = 1;
    else
      formatDate = 0;
    
    // set to local storage
    window.localStorage.setItem('invertFace', invertFace);
    window.localStorage.setItem('invertRow', invertRow);
    window.localStorage.setItem('formatDate', formatDate);
    
    // send to pebble
    sendToPebble(invertFace, invertRow, formatDate);
  }
});

Pebble.addEventListener("ready", function(e) {
  "use strict";
  console.log("Pebble JS ready");
});
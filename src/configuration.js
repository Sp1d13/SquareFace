var invertFace = window.localStorage.getItem('invertFace') ? window.localStorage.getItem('invertFace') : 'false',
    invertRow = window.localStorage.getItem('invertRow') ? window.localStorage.getItem('invertRow') : '0';

function sendToPebble(invertFace, invertRow) {  
  var dictionary = {
    'INVERTFACE': invertFace,
    'INVERTROW': invertRow
  };
  
  Pebble.sendAppMessage(dictionary);
}

Pebble.addEventListener("ready", function() {
  sendToPebble(invertFace, invertRow);
});

Pebble.addEventListener("showConfiguration", function() {
  Pebble.openURL('http://spidie.de/pebble/squareface/configuration.html?invertFace='+ invertFace +'&invertRow='+ invertRow);
});

Pebble.addEventListener("webviewclosed", function(e) {
  // Using primitive JSON validity and non-empty check
  if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
    var options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
    
    // analyze return
    invertFace = options['invert-face'];
    
    if (options['invert-toprow'] === true)
      invertRow = '1';
    else if (options['invert-bottomrow'] === true)
      invertRow = '2';
    else 
      invertRow = '0';
    
    // set to local storage
    window.localStorage.setItem('invertFace', invertFace);
    window.localStorage.setItem('invertRow', invertRow);
    
    // send to pebble
    sendToPebble(invertFace, invertRow);
  }
});
play = true;
// var streamLink = "http://192.168.1.1:8080/?action=stream";
var streamLink = "http://192.168.1.1:8080/?action=stream";
var humData = [];
var tempData = [];
var chartTemp;
var chartHum;

$(document).ready(function () {
    
    chartTemp = new CanvasJS.Chart("chartTemperature", {
        title: {
            text: "Temperature"
        },
        data: [{
            type: "line",
            dataPoints: tempData
        }]
    });

    chartHum = new CanvasJS.Chart("chartHumidity", {
        title: {
            text: "Humidity"
        },
        data: [{
            type: "line",
            dataPoints: humData
        }]
    });
    
    setInterval (function (){
    $.get( "datalog.txt", function( data ) {
    var array = data.split(',');
    if (array[0]){
        if (array[1]){
            console.log(array[0]);
            console.log(array[1]);
            updateChart(parseInt(array[0]), parseInt(array[1]))
        }
    }})}, 3000);


    streamOn();
    setTimeout(function () { $("#stream").attr("src", streamLink); }, 3000);

    $('#onOff').click(function () {
        if (play == true) {
            streamOff();
            console.log("onOff button was clicked and stream is closed.");
            play = false;
        } else {
            streamOn();
            setTimeout(function () { $("#stream").attr("src", streamLink); }, 3000);
            console.log("onOff button was clicked and stream is opened.");
            play = true;
        }
    });

    $('#screenShot').click(function () {
       console.log("screenShot button was clicked.");
       screenShot();
       setTimeout(function () { $("#stream").attr("src", streamLink); }, 6000);
    });
    
    $('#steering').on('change', function(){
        console.log($(this).val());
        $.get('/arduino/servo/' + $(this).val(), function () { });
    });
    
    $('#frontDCRight').click(function(){
        console.log("right clicked");
        $.get('/arduino/frontDC/1', function () { });
    });
    
    $('#frontDCLeft').click(function(){
        console.log("left clicked");
        $.get('/arduino/frontDC/0', function () { });
    });
    
    $('#moveForward').click(function(){
        console.log("forward clicked");
        $.get('/arduino/move/1', function () { });
    });
    
    $('#moveBack').click(function(){
        console.log("back clicked");
        $.get('/arduino/move/0', function () { });
    });
});

function streamOn() {
    $.get('/arduino/onOff/1', function () { });
}

function streamOff() {
    $.get('/arduino/onOff/0', function () { });
}

function screenShot() {
    $.get('/arduino/screenShot/1', function () { });
}

function updateChart(hum, temp) {
    humData.push({x:humData.length + 1 , y : hum}); 
    tempData.push({x:tempData.length + 1 , y : temp});
    
    chartTemp.render();
    chartHum.render();
}

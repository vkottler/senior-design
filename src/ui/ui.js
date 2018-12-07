// Function to display variable in html (iterates through all counts
// of elements and sets values accordingly)
function displayVals(elm_name, value) {
    var occurences = document.getElementsByName(elm_name).length;
    for(i = 0; i < occurences; i++)
        document.getElementsByName(elm_name)[i].innerHTML = value;
}

function openTab(evt, tabName) {
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    document.getElementById(tabName).style.display = "block";
    evt.currentTarget.className += " active";
}

function send_stop()
{
    client.send('stop')
}

function sendGain()
{
    var data = []
    var currentSliderArray = document.getElementsByClassName("gain_slider")
    var occurences = currentSliderArray.length;
    for(j = 0; j < occurences;j++){
        let currentSliderId = currentSliderArray[j].id
        console.log('slider id: ' + currentSliderId)
        var value = document.getElementById(currentSliderId+'_val').value
        data.push(value)
    }
    console.log(data)

    //not sure how to do, but basic layout 
    //send('pid p_gain '+ data[0])
    //send('pid i_gain '+ data[1])
    //send('pid d_gain '+ data[2])
}

function sendDesiredVals()
{
    var throttle_value = document.getElementById("thrust_slider_val").value 
    var data = []
    var currentSliderArray = document.getElementsByClassName("gyro_slider")
    var occurences = currentSliderArray.length;
    for(j = 0; j < occurences;j++){
        let currentSliderId = currentSliderArray[j].id
        console.log('slider id: ' + currentSliderId)
        var value = document.getElementById(currentSliderId+'_val').value
        data.push(value)
    }
    console.log(throttle_value)
    console.log(data)
    //todo
    //send(throttle_value)
    //send('pid p_gain '+ data[0])
    //send('pid i_gain '+ data[1])
    //send('pid d_gain '+ data[2])
}


var slider_types = ["gain_slider","gyro_slider", "thrust_slider"];

$(function() {
    for(i = 0; i < slider_types.length; i++)
    {
        let currentSliderArray = document.getElementsByClassName(slider_types[i])
        let min = 0; 
        let max = 0; 
        switch(i){
            case 0: 
                 min = 0.000;
                 max = 10.000;
                break;
            case 1: 
                 min = -90.000;
                 max = 90.000;
                break;
            case 2:
                 min = 0.000;
                 max = 500.000;
                break;
        }
        var occurences = currentSliderArray.length;
        for(j = 0; j < occurences;j++){
            let currentSliderId = currentSliderArray[j].id
            $("#"+currentSliderId).slider({
            min: min,
            max: max,
            step: .001,
            range: "min",
            value: $("#"+currentSliderId+"_val").val(),
            slide: function( event, ui ) {
                $("#"+currentSliderId+"_val").val(ui.value);
            }
            });
            $("#"+currentSliderId+"_val").on( "keyup", function() {
                if((this.value < min) || (this.value > max)) this.value = max;
                $("#"+currentSliderId).slider( "value", this.value );
            });
        }
    }
});
 

/* open the first tab by default */
let start_tab = "plot-tab";
let mock_evt = {currentTarget: document.getElementById("start-tab")};
openTab(mock_evt, start_tab);

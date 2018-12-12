let throttle_requests = [];
let gyro_x_requests = [];
let gyro_y_requests = [];
let gyro_z_requests = [];

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
    client.send_message('abort');
    client.send_message('abort');
    client.send_message('abort');
    client.send_message('abort');
}

function sendGain()
{
    var data = []
    var currentSliderArray = document.getElementsByClassName("gain_slider")
    var occurences = currentSliderArray.length;
    for(j = 0; j < occurences;j++){
        let currentSliderId = currentSliderArray[j].id
        var value = document.getElementById(currentSliderId+'_val').value
        data.push(value)
    }
    let axis = document.getElementById("gain-axis").value;
    client.send_message(`pid ${axis} ${data[0]} ${data[1]} ${data[2]}`);
}

function sendDesiredVals()
{
    var throttle_value = document.getElementById("thrust_slider_val").value 
    var data = []
    var currentSliderArray = document.getElementsByClassName("gyro_slider")
    var occurences = currentSliderArray.length;
    for(j = 0; j < occurences;j++){
        let currentSliderId = currentSliderArray[j].id
        var value = document.getElementById(currentSliderId+'_val').value
        data.push(value)
    }
    client.send_message(`set throttle ${throttle_value}`);
    client.send_message(`set req_x ${data[0]}`);
    client.send_message(`set req_y ${data[1]}`);
    client.send_message(`set req_z ${data[2]}`);
}


var slider_types = ["gain_slider","gyro_slider", "thrust_slider"];

$(function() {
    for(i = 0; i < slider_types.length; i++)
    {
        let currentSliderArray = document.getElementsByClassName(slider_types[i]);
        let min;
        let max; 
        let step;
        let start;
        switch(i)
        {
            case 0: 
                start = 0.0;
                min =   0.0;
                max =  30.0;
                step =  0.1;
                break;
            case 1: 
                start =  0.0;
                min =  -90.0;
                max =   90.0;
                step =   5.0;
                break;
            case 2:
                start = 50.0;
                min =   50.0;
                max =  500.0;
                step =   5.0;
                break;
            default: throw "Unknown slider type.";
        }
        var occurences = currentSliderArray.length;
        for(j = 0; j < occurences; j++)
        {
            let currentSliderId  = currentSliderArray[j].id;
            let slider_val = `#${currentSliderId}_val`;

            /* set to the minimum by default */
            $(slider_val).val(start);

            $("#"+currentSliderId).slider({
                min: min, max: max, step: step,
                range: "min",
                value: $(slider_val).val(),
                slide: (evt, ui) => {
                    $(slider_val).val(ui.value);

                    /* send a command if applicable */
                    if (slider_val.includes("gyro"))
                    {
                        if (slider_val.includes("x"))
                            gyro_x_requests.push(ui.value);
                        else if (slider_val.includes("y"))
                            gyro_y_requests.push(ui.value);
                        else if (slider_val.includes("z"))
                            gyro_z_requests.push(ui.value);
                    }
                    else if (slider_val.includes("thrust"))
                        throttle_requests.push(ui.value);
                }
            });

            $(slider_val).on("keyup", function() {
                if (this.value < min)      this.value = min;
                else if (this.value > max) this.value = max;
                $("#"+currentSliderId).slider("value", this.value);
            });
        }
    }
});

/* on an interval, service requests to change throttle and rotation */
function service_requests()
{
    if (throttle_requests.length > 0)
        client.send_message(`set throttle ${throttle_requests.shift()}`);
    else if (gyro_x_requests.length > 0)
        client.send_message(`set req_x ${gyro_x_requests.shift()}`);
    else if (gyro_y_requests.length > 0)
        client.send_message(`set req_y ${gyro_y_requests.shift()}`);
    else if (gyro_z_requests.length > 0)
        client.send_message(`set req_z ${gyro_z_requests.shift()}`);
}
setInterval(service_requests, 500);
 
/* open the first tab by default */
let start_tab = "plot-tab";
let mock_evt = {currentTarget: document.getElementById("start-tab")};
openTab(mock_evt, start_tab);

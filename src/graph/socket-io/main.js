const lidard1_dataline = "lidar_d1"
const lidard2_dataline = "lidar_d1"
const gyrox_dataline = "gyro_x"
const gyroy_dataline = "gyro_y"
const gyroz_dataline = "gyro_z"
const battc1_dataline= "batt_v_cell1"
const battc2_dataline = "batt_v_cell2"
const battc3_dataline = "batt_v_cell3"
const batt_total_dataline = "batt_v_total"
const battcurr_dataline = "batt_current"

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

$(function () {
    var socket = io();

    // Data Connection Status
    socket.on('data_status', (incoming_status) => {
        var status = incoming_status
        if(status == 1)
            document.getElementById("data_sign").style.backgroundColor = "green";
        else
            document.getElementById("data_sign").style.backgroundColor = "red";
    });

    // Server Connection Status
    socket.on('server_status', (incoming_status) => {
        var status = incoming_status
        if(status == 1)
            document.getElementById("server_sign").style.backgroundColor = "green";
        else
            document.getElementById("server_sign").style.backgroundColor = "red";
    });

    // MANIFEST DATALINE
    socket.on('manifestLine', (data_set) => {
        table = document.getElementById("manifest_table");
        for(i = 0; i < data_set.length; i++)
        {
            var tr = document.createElement('tr');
            var td1 = document.createElement('td');
            var td2 = document.createElement('td');
            var td3 = document.createElement('td');
            var td4 = document.createElement('td');
            var td5 = document.createElement('td');

            var td_index = document.createTextNode(data_set[i].index);
            var td_type = document.createTextNode(data_set[i].data_type);
            var td_size = document.createTextNode(data_set[i].size);
            var td_name = document.createTextNode(data_set[i].name);
            var td_units = document.createTextNode(data_set[i].units);

            td1.appendChild(td_index)
            td2.appendChild(td_type) 
            td3.appendChild(td_size)
            td4.appendChild(td_name)
            td5.appendChild(td_units)

            tr.appendChild(td1)
            tr.appendChild(td2)
            tr.appendChild(td3)
            tr.appendChild(td4)
            tr.appendChild(td5)

            table.append(tr)

            /*
            console.log("-------------------------------------------")
            console.log("Channel Index: " + data_set[i].index)
            console.log("Channel Type: " + data_set[i].data_type)
            console.log("Channel Size: " + data_set[i].size)
            console.log("Channel Name: " + data_set[i].name)
            console.log("Channel Units: " + data_set[i].units)
            */
        }
    });

    // Batt Current Dataline Implementation
    socket.on(battcurr_dataline, (data_set) => {
        var batt_curr = data_set;
        // Method to find all instances of batt vals and sets it accordinly in html
        displayVals("batt_current",batt_curr);
    });

    // Batt Total Voltage Dataline Implementation
    socket.on(batt_total_dataline, (data_set) => {
        var batt_total_voltage = data_set;
        // Method to find all instances of batt vals and sets it accordinly in html
        displayVals("batt_total",batt_total_voltage);
    });

    // Batt Cell 3 Dataline Implementation
    socket.on(battc3_dataline, (data_set) => {
        var batt_cell3 = data_set;
        // Method to find all instances of batt vals and sets it accordinly in html
        displayVals("batt_cell3",batt_cell3);
    });

    // Batt Cell 2 Dataline Implementation
    socket.on(battc2_dataline, (data_set) => {
        var batt_cell2 = data_set;
        // Method to find all instances of batt vals and sets it accordinly in html
        displayVals("batt_cell2",batt_cell2);
    });

    // Batt Cell 1 Dataline Implementation
    socket.on(battc1_dataline, (data_set) => {
        var batt_cell1 = data_set;
        // Method to find all instances of batt vals and sets it accordinly in html
        displayVals("batt_cell1",batt_cell1);
    });
   
    // ESC Dataline Implementation
    /*
    socket.on(esc_data_line, (data_set) => {
        var incoming_esc_FL = data_set[0].FL;
        var incoming_esc_FR = data_set[0].FR;
        var incoming_esc_BL = data_set[0].BL;
        var incoming_esc_BR = data_set[0].BR;
                    
        //Method to find all instances of esc vals and sets it accordinly in html
        displayVals("esc_front_left",incoming_esc_FL);
        displayVals("esc_front_right",incoming_esc_FR);
        displayVals("esc_back_left",incoming_esc_BL);
        displayVals("esc_back_right",incoming_esc_BR);

        //PUSH ESC VALS TO ARRAY 
        data_esc_frontleft.push({
            y: incoming_esc_FL 
        });
        data_esc_frontright.push({
            y: incoming_esc_FR
        });
        data_esc_backleft.push({
            y: incoming_esc_BL
        });
        data_esc_backright.push({
            y: incoming_esc_BR
        });

        esc_front_left_chart.render();
        esc_front_right_chart.render();
        esc_back_left_chart.render();
        esc_back_right_chart.render();
    });
    */

    // PID Dataline Implementation
    /*
    socket.on(pid_data_line, (data_set) => {
        var incoming_pid_pitch = data_set[0].pitch
        var incoming_pid_roll = data_set[0].roll
        var incoming_pid_yaw = data_set[0].yaw
                    
        // Method to find all instances of pid vals and sets it accordinly in html
        displayVals("pid_pitch",incoming_pid_pitch);
        displayVals("pid_roll",incoming_pid_roll);
        displayVals("pid_yaw",incoming_pid_yaw);

        // PUSH GYRO VALS TO ARRAY 
        data_pitch.push({
            y: incoming_pid_pitch 
        });
        data_roll.push({
            y: incoming_pid_roll
        });
        data_yaw.push({
            y: incoming_pid_yaw
        });

        pid_pitch_chart.render();
        pid_roll_chart.render();
        pid_yaw_chart.render();
    });
    */

    // Lidar D1 Dataline Implementation
    socket.on(lidard1_dataline, (data_set) => {
        var incoming_lidar_d1 = data_set;

        //Method to find all instances of lidar vals and sets it accordinly in html
        displayVals("lidar_d1",incoming_lidar_d1);

        //PUSH lidar VALUES TO RESPECTIVE GRAPH DATA SETS
        data_lidar_d1.push({
            y: incoming_lidar_d1
        });
        lidar_chart_d1.render();
    });

    // Lidar D2 Dataline Implementation
    socket.on(lidard2_dataline, (data_set) => {
        var incoming_lidar_d2 = data_set;
        displayVals("lidar_d2",incoming_lidar_d2);
        data_lidar_d2.push({
            y: incoming_lidar_d2
        });

        lidar_chart_d2.render();
    });

    // Gyro X Dataline Implementation
    socket.on(gyrox_dataline, (data_set) => {
        var incoming_gyro_x = data_set
        //Method to find all instances of gyro vals and sets it accordinly in html
        displayVals("gyro_x",incoming_gyro_x);
        //PUSH GYRO VALS TO ARRAY 
        data_gyro_x.push({
            y: incoming_gyro_x 
        }); 
        gyro_chart_x.render();
    });

    // Gyro Y Dataline Implementation
    socket.on(gyroy_dataline, (data_set) => {
        var incoming_gyro_y = data_set;
        //Method to find all instances of gyro vals and sets it accordinly in html
        displayVals("gyro_y",incoming_gyro_y);
        //PUSH GYRO VALS TO ARRAY 
        data_gyro_y.push({
            y: incoming_gyro_y
        });     
        gyro_chart_y.render();
    });

    // Gyro Dataline Implementation
    socket.on(gyroz_dataline, (data_set) => {
        var incoming_gyro_z = data_set;
        //Method to find all instances of gyro vals and sets it accordinly in html
        displayVals("gyro_z",incoming_gyro_z);
        //PUSH GYRO VALS TO ARRAY
        data_gyro_z.push({
            y: incoming_gyro_z
        });
        gyro_chart_z.render();
    });
});

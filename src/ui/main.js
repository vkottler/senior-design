/* globals */
let activity_log;
let data_log;
let client;

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
        manifest_handle(data_set, socket)
    });
});

/* add behavior for connecting */
function start_client()
{
    if (client) client.close();
    let host = document.getElementById("host").value;
    let port = document.getElementById("port").value;
    client = new TelemetryClient(host, port, "state",
                                 "disconnect-button", "to-send", "send-button",
                                 data_log, activity_log);
}

/* load the commanding application */
$(function() {
    $("#commanding-content").load("commanding/commanding.html", function () {
        activity_log = new LogElement("activity-log");
        data_log = new LogElement("data-log");

        connect_button = document.getElementById("connect-button");
        connect_button.onclick = start_client;
        document.getElementById("host").value = window.location.hostname;

        /* automatically attempt to connect */
        start_client();
    });

    /* register telemetry starting/stopping butons */
    document.getElementById("start-telemetry").onclick = start_telemetry;
    document.getElementById("stop-telemetry").onclick = stop_telemetry;
});

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

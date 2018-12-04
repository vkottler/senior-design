//DATA ARRAYS FOR GRAPH 
var data_lidar_d1 = [];
var data_lidar_d2 = []; 

var data_gyro_x = [];
var data_gyro_y = []; 
var data_gyro_z = []; 

var data_pitch = [];
var data_roll = []; 
var data_yaw = []; 

var data_esc_frontleft = [];
var data_esc_frontright = []; 
var data_esc_backleft = []; 
var data_esc_backright = [];

//THE GRAPH OBJECTS 
var lidar_chart_d1 = new CanvasJS.Chart("lidar_d1_chartContainer", {
    zoomEnabled: true,
        title: {
        text: "Lidar (D1) v. Time"
    },
    axisY: {	
        title: "Distance (m)",
        includeZero: true
    },
    axisX: {
        title: "Time (s)",
        includeZero: false
    },         
    data: [
        //lidar d1 
        {	
            type: "line",
            dataPoints: data_lidar_d1,
            showInLegend: true,
            legendText: "Lidar D1", 
            color: "blue"
        }
    ]
});

var lidar_chart_d2 = new CanvasJS.Chart("lidar_d2_chartContainer", {
    zoomEnabled: true,
    title: {
        text: "Lidar (D2) v. Time"
    },
    axisY: {	
        title: "Distance (m)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        //lidar d2 
        {	
            type: "line",
            dataPoints: data_lidar_d2,
            showInLegend: true,
            legendText: "Lidar D2", 
            color: "red"
        }
    ]
});

var gyro_chart_x = new CanvasJS.Chart("gyroX_chartContainer", {
    zoomEnabled: true,
    title: {
        text: "Gyro (X) v. Time"
    },
    axisY: {	
        title: "Gyro (X) (deg/sec)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        //gyro x 
        {	
            type: "line",
            dataPoints: data_gyro_x,
            showInLegend: true,
            legendText: "Gyro X", 
            color: "blue"
        }
    ]
});

var gyro_chart_y = new CanvasJS.Chart("gyroY_chartContainer", {
    zoomEnabled: true,
    title: {
        text: "Gyro (Y) v. Time"
    },
    axisY: {	
        title: "Gyro (Y) (deg/sec)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        //gyro y 
        {	
            type: "line",
            dataPoints: data_gyro_y,
            showInLegend: true,
            legendText: "Gyro Y", 
            color: "red"
        }
    ]
});

var gyro_chart_z = new CanvasJS.Chart("gyroZ_chartContainer", {
    zoomEnabled: true,
    title: {
        text: "Gyro (Z) v. Time"
    },
    axisY: {	
        title: "Gyro (Z) (deg/sec)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        //gyro z 
        {	
            type: "line",
            dataPoints: data_gyro_z,
            showInLegend: true,
            legendText: "Gyro Z", 
            color: "green"
        }
    ]
});

var pid_pitch_chart = new CanvasJS.Chart("pidpitch_chartContainer", {
    zoomEnabled: true,
        title: {
        text: "PID Pitch v. Time"
    },
    axisY: {	
        title: "Pitch (deg/sec)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        {	
            type: "line",
            dataPoints: data_pitch,
            showInLegend: true,
            legendText: "PID Pitch", 
            color: "green"
        }
    ]
});

var pid_roll_chart = new CanvasJS.Chart("pidroll_chartContainer", {
    zoomEnabled: true,
    title: {
        text: "PID Roll v. Time"
    },
    axisY: {	
        title: "Roll (deg/sec)",
        includeZero: true
    },
    axisX: {
        title: "Time (s)",
        includeZero: false
    },         
    data: [
        {	
            type: "line",
            dataPoints: data_roll,
            showInLegend: true,
            legendText: "PID Roll", 
            color: "Red"
        }
    ]
});

var pid_yaw_chart = new CanvasJS.Chart("pidyaw_chartContainer", {
    zoomEnabled: true,
    title :{
        text: "PID Yaw v. Time"
    },
    axisY: {	
        title: "Yaw (deg/sec)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        {	
            type: "line",
            dataPoints: data_yaw,
            showInLegend: true,
            legendText: "PID Yaw", 
            color: "Blue"
        }
    ]
});

var esc_back_left_chart = new CanvasJS.Chart("esc_back_left_chartContainer", {
    zoomEnabled: true,
    title: {
        text: "ESC Back Left v. Time"
    },
    axisY: {	
        title: "ESC Back Left (%)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        {	
            type: "line",
            dataPoints: data_esc_backleft,
            showInLegend: true,
            legendText: "ESC Back Left", 
            color: "Blue"
        }
    ]
});

var esc_back_right_chart = new CanvasJS.Chart("esc_back_right_chartContainer", {
    zoomEnabled: true,
    title:{
        text: "ESC Back Right v. Time"
    },
    axisY: {	
        title: "ESC Back Right (%)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        {	
            type: "line",
            dataPoints: data_esc_backright,
            showInLegend: true,
            legendText: "ESC Back Right", 
            color: "Red"
        }
    ]
});

var esc_front_left_chart = new CanvasJS.Chart("esc_front_left_chartContainer", {
    zoomEnabled: true,
    title: {
        text: "ESC Front Left v. Time"
    },
    axisY: {	
        title: "ESC Front Left (%)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        {	
            type: "line",
            dataPoints: data_esc_frontleft,
            showInLegend: true,
            legendText: "ESC Front Left", 
            color: "Green"
        }
    ]
});

var esc_front_right_chart = new CanvasJS.Chart("esc_front_right_chartContainer", {
    zoomEnabled: true,
        title: {
        text: "ESC Front Right v. Time"
    },
    axisY: {	
        title: "ESC Front Right (%)",
        includeZero: true
    },
    axisX: {
        title:"Time (s)",
        includeZero: false
    },         
    data: [
        {	
            type: "line",
            dataPoints: data_esc_frontright,
            showInLegend: true,
            legendText: "ESC Front Right", 
            color: "Red"
        }
    ]
});

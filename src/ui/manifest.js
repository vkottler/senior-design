let start_time = new Date().getTime() / 1000.0;
let window_size = 100;
let num_plots = 0;
let plots = {};

function add_manifest_table_entry(manifest_entry)
{
    var tr = document.createElement('tr');
    var td1 = document.createElement('td');
    var td2 = document.createElement('td');
    var td3 = document.createElement('td');
    var td4 = document.createElement('td');
    var td5 = document.createElement('td');
    td1.appendChild(document.createTextNode(manifest_entry.index));
    td2.appendChild(document.createTextNode(manifest_entry.data_type));
    td3.appendChild(document.createTextNode(manifest_entry.size));
    td4.appendChild(document.createTextNode(manifest_entry.name));
    td5.appendChild(document.createTextNode(manifest_entry.units));
    tr.appendChild(td1);
    tr.appendChild(td2);
    tr.appendChild(td3);
    tr.appendChild(td4);
    tr.appendChild(td5);
    table.append(tr);
}

function get_time() { return (new Date().getTime() / 1000.0) - start_time; }

function manifest_handle(data_set, socket)
{
    table = document.getElementById("manifest_table");
    for(i = 0; i < data_set.length; i++)
    {
        let manifest_entry = data_set[i];

        add_manifest_table_entry(manifest_entry);

        /* create a container for the new plot */
        let plot_container_name = "left-plot-col";
        if (num_plots % 2 != 0)
            plot_container_name = "right-plot-col";
        num_plots++;
        let plot_container = document.getElementById(plot_container_name);
        let new_plot_container = document.createElement("div");
        let new_plot_container_name = `${manifest_entry.name}-container`;
        new_plot_container.setAttribute("id", new_plot_container_name);
        new_plot_container.className = "plot-container";

        /* add new plot container to the existing DOM */
        plot_container.appendChild(new_plot_container);

        /* create the CanvasJS plot */
        let plot_data = [];
        let chart_config = {
            zoomEnabled: false,
            title: { text: `${manifest_entry.name} (${manifest_entry.units})` },
            axisY: { title: manifest_entry.units, includeZero: true },
            axisX: { title: "Seconds (since load)", includeZero: false },         
            data: [{ type: "line", dataPoints: plot_data, showInLegend: false }]
        };
        let plot = new CanvasJS.Chart(new_plot_container_name, chart_config);
        plots[manifest_entry.name] = {
            plot: plot,
            to_render: false,
        };
        plot.render();

        /* add socket hook */
        socket.on(manifest_entry.name, (data) => {
            let content;
            switch (manifest_entry.data_type)
            {
                case "FLOAT":
                    content = data.toFixed(3);
                    break;
                case "STRING":
                    content = data;
                    break;
                default:
                    content = String(data);
            }
            plot_data.push({x: get_time(), y: data});
            if (plot_data.length > window_size)
                plot_data.shift();
            plots[manifest_entry.name].to_render = true;
        });
    }
}

/* update the global timer */
setInterval(function() {
    document.getElementById("time").innerHTML = get_time().toFixed(2) + "s";
}, 50);

let odd_interval;
let odd_running = false;
let even_interval;
let even_running = false;

function get_plot_interval()
{
    return document.getElementById("update-rate").value;
}

function update_odd()
{
    for (plot_name in plots)
    {
        let plot = plots[plot_name];
        if (i % 2 === 0 && plot.to_render)
        {
            plot.to_render = false;
            plot.plot.render();
        }
    }
}

function update_even()
{
    for (plot_name in plots)
    {
        let plot = plots[plot_name];
        if (i % 2 === 1 && plot.to_render)
        {
            plot.to_render = false;
            plot.plot.render();
        }
    }
}

function stop_plots()
{
    if (odd_running)
    {
        clearInterval(odd_interval);
        odd_running = false;
    }
    if (even_running)
    {
        clearInterval(even_interval);
        even_running = false;
    }
}

/* start both loops, offset by half the amount of the interval */
function start_plots()
{
    /* if intervals are already started, make sure this call is idempotent */
    if (odd_running === true || even_running === true)
        return;

    let interval = get_plot_interval();
    odd_interval = setInterval(update_odd, interval);
    odd_running = true;
    setTimeout(function () {
        even_interval = setInterval(update_even, interval);
        even_running = true;
    }, interval / 2);
}

/* start the plots with the default value when the page loads */
$(function() {
    document.getElementById("stop-plots").onclick = stop_plots;
    document.getElementById("start-plots").onclick = start_plots;
    start_plots();
});

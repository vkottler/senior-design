let start_time = 0;
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

function get_time() { return new Date().getTime() / 1000.0; }

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
            plot_data.push({x: get_time() - start_time, y: data});
            if (plot_data.length > window_size)
                plot_data.shift();
            plots[manifest_entry.name].to_render = true;
        });
    }
}

start_time = get_time();
setInterval(function() {
    for (plot_name in plots)
    {
        let plot = plots[plot_name];
        if (plot.to_render)
        {
            plot.plot.render();
            plot.to_render = false;
        }
    }
}, 50);

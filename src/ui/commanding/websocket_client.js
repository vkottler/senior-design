/* a simple websocket client for telemetry purposes */
class TelemetryClient
{
    constructor(host, port, state_id, disconnect_id, input_id, send_id,
                data_log, activity_log)
    {
        this.data_log = data_log;
        this.activity_log = activity_log;
        this.endpoint = `${host}:${port}`;

        this.socket = new WebSocket(`ws://${this.endpoint}`, "telemetry");

        this.state_element = document.getElementById(state_id);
        this.disconnect_element = document.getElementById(disconnect_id);
        this.input_element = document.getElementById(input_id);
        this.send_element = document.getElementById(send_id);

        if (!this.socket || !this.state_element || !this.disconnect_element ||
            !this.input_element || !this.send_element)
            throw `couldn't create socket for ${this.endpoint}, or find elements: '${state_id}', '${disconnect_id}'`;

        this.disconnect_element.onclick = this.close.bind(this);
        this.send_element.onclick = this.send.bind(this);

        this.socket.onclose = this.handle_close.bind(this);
        this.socket.onerror = this.handle_error.bind(this);
        this.socket.onmessage = this.handle_message.bind(this);
        this.socket.onopen = this.handle_open.bind(this);

        this.state = "INITED";
        this.closed = false;

        /* initialize silence byte array */
        let silence_byte = parseInt("0xAA", 16);
        let num_silence_bytes = 32;
        this.silence_byte_array = new Uint8Array(num_silence_bytes);
        for (let i = 0; i < num_silence_bytes; i++)
            this.silence_byte_array[i] = silence_byte;
    }

    set state(state_str)
    {
        this.state_element.innerHTML = state_str;
    }

    log_activity(message)
    {
        this.activity_log.log(`(${this.endpoint}) ${message}`);
    }

    close()
    {
        if (!this.closed)
        {
            this.log_activity("closing connection");
            this.state = "CLOSING";
            this.socket.close();
        }
    }

    handle_message(evt)
    {
        this.data_log.log(evt.data);
    }

    send_message(data)
    {
        if (this.socket.readyState === WebSocket.OPEN)
        {
            this.socket.send(this.silence_byte_array);
            this.socket.send(data + '\n');
            this.log_activity(`sent: ${data}`);
        }
        else console.log(`Can't send '${data}', connection not active.`);
    }

    send()
    {
        this.send_message(this.input_element.value);
    }

    handle_error(evt)
    {
        this.log_activity(`connection error`);
        this.state = "ERROR";
        this.close();
    }

    handle_open(evt)
    {
        this.log_activity("connection established");
        this.state = "OPEN";
    }

    handle_close(evt)
    {
        this.log_activity("connection closed");
        this.state = "CLOSED";
        this.closed = true;
    }
}

/* a div-as-log convenience interface */
class LogElement
{
    constructor(id)
    {
        this.log_element = document.getElementById(id);
        this.clear_element = document.getElementById(id + "-clear");
        if (!this.log_element || !this.clear_element)
            throw `Missing element ${id} or ${id}-clear`;
        this.current_text = "";
        this.clear_element.onclick = this.clear.bind(this);
    }

    scroll_bottom()
    {
        this.log_element.scrollTop = this.log_element.scrollHeight - this.log_element.clientHeight;
    }

    update()
    {
        this.log_element.innerHTML = this.current_text;
        this.scroll_bottom();
    }

    log(message)
    {
        let lines = message.split("\n");
        for (let i in lines)
        {
            let stripped_line = lines[i].replace("\r", "");
            stripped_line = lines[i].replace("\n", "");
            if (stripped_line !== "")
                this.current_text += stripped_line + "<br>";
        }
        this.update();
    }

    clear()
    {
        this.current_text = "";
        this.update();
    }
}

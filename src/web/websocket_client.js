let curr_val = 0;
let curr_text = "";

function scroll_to_bottom(id) {
    let div = document.getElementById(id);
    div.scrollTop = div.scrollHeight - div.clientHeight;
}

function test_logs()
{
    curr_text += "test" + curr_val++ + "<br>";
    log1 = document.getElementById("activity-log");
    log2 = document.getElementById("data-log");
    log1.innerHTML = curr_text;
    log2.innerHTML = curr_text;
    scroll_to_bottom("activity-log");
    scroll_to_bottom("data-log");
}

setInterval(test_logs, 500);

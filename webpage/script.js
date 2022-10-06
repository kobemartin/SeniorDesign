function updateEvent() {
    let formData = new FormData(document.querySelector('form'))
    let link = "https://ggst1z4j2i.execute-api.us-east-1.amazonaws.com/prod/esp32?operation=updateEvent&" +
        "?eventId=" + formData.get('eventId') +
        "&medication=" + formData.get('medication') +
        "&dayTime=" + formData.get('dayTime');
    fetch(link)
        .then(resp => resp.json())
        .then(data => updateEventResponse(data));
}

function updateEventResponse(data) {
    console.log(data);
    document.querySelector("output").innerHTML = "The event has been updated: " +  data['status'];
}









function startLogSearch() {
    let formData = new FormData(document.querySelector('form'))
    let link = "https://fn4v7y1ur9.execute-api.us-west-2.amazonaws.com/beta/" +
        "?fleet=" + formData.get('fleet') +
        "&clientRequestId=" + formData.get('clientRequestId') +
        "&startDate=" + formData.get('startDate') +
        "&endDate=" + formData.get('endDate') +
        "&startTime=" + formData.get('startTime') +
        "&endTime=" + formData.get('endTime');
    fetch(link)
        .then(resp => resp.json())
        .then(data => displayStartLogSearchOutput(data));
}

function displayStartLogSearchOutput(data) {
    document.querySelector("output").innerHTML = "The log search has started. The logs will be saved to the following search request id: " +  data.s3Folder;
    document.getElementById("s3ObjectInput").value = data.s3Folder;
}

function getLogs() {
    let formData = new FormData(document.getElementById('getLogs'))
    let link = "https://fn4v7y1ur9.execute-api.us-west-2.amazonaws.com/beta/output" + "?s3Object=" + formData.get('s3Object');
    fetch(link)
        .then(resp => resp.json())
        .then(logs =>  generate_table(logs));
}

function generate_table(logs) {
    // If log search is not yet complete, the status message will not be in JSON, thus just display contents of S3 Object
    if (!(logs.includes("{")) ) {
        document.getElementById("logOutput").innerHTML = logs;
    } else {
        const tbl = document.createElement("table");
        tbl.setAttribute("class", "table")

        // Create table head row with table headers
        const tblHead = document.createElement("thead");
        const row = document.createElement("tr");
        const headers = [" ", "Log", "ATROPS Request Id", "Display log on ATROPS Web"];
        for (let i = 0; i < headers.length; i++) {
            const th = document.createElement("th");
            const cellText = document.createTextNode(headers[i]);
            th.appendChild(cellText);
            row.appendChild(th);
        }
        tblHead.appendChild(row);
        tbl.appendChild(tblHead);

        // Display logs in table body
        const tblBody = document.createElement("tbody");
        logs = JSON.parse(logs)
        for (let i = 0; i < logs.length; i++) {
            // Create new row
            let row = document.createElement("tr");

            // Create +/- to expand and collapse log content
            let cell = document.createElement("td");
            let span = document.createElement("span")
            span.setAttribute("class", "expandChildTable");
            cell.appendChild(span);
            row.appendChild(cell);

            // Add log number to row
            cell = document.createElement("th");
            let cellText = document.createTextNode(i + 1);
            cell.appendChild(cellText);
            row.appendChild(cell);

            // Add log request id to row
            cell = document.createElement("td");
            cellText = document.createTextNode(JSON.parse(logs[i]).requestID);
            cell.appendChild(cellText);
            row.appendChild(cell);

            // Add link to display log on ATROPS Web
            cell = document.createElement("td");
            let a = document.createElement("a");
            logString = JSON.stringify(logs[i])
            a.href = "javascript:openLogInATROPS({logcontent:" + logString + "})"
            a.text = "gsp"
            cell.appendChild(a);
            row.appendChild(cell);

            tblBody.appendChild(row);

            // Add raw log contents to next row, log contents will be initially hidden until user clicks '+'
            row = document.createElement("tr");
            row.setAttribute("class", "childTableRow");
            cell = document.createElement("td");
            cell.setAttribute("colspan", 4);
            cellText = document.createTextNode(logs[i]);
            let scrollBox = document.createElement("div");
            scrollBox.setAttribute("class", "scrollBox");
            scrollBox.textContent = logs[i];
            cell.appendChild(scrollBox);
            row.appendChild(cell);
            tblBody.appendChild(row);

        }
        
        tbl.appendChild(tblBody);
        document.getElementById("logOutput").innerHTML = tbl.outerHTML;

        // Allow +/- to expand and collapse raw log contents
        $(function () {
            $('.expandChildTable').on('click', function () {
                $(this).toggleClass('selected').closest('tr').next().toggle();
            })
        });


    }
}

// Function to pass raw log data to GetShippingPossiblities on ATROPS Web
function openLogInATROPS(data) {
    var form = document.createElement("form");
    form.target = "_blank";
    form.method = "POST";
    form.action = "https://atrops-web-na.amazon.com/operations/gsp_fill_data_using_logs";
    form.style.display = "none";

    for (var key in data) {
        var input = document.createElement("input");
        input.type = "hidden";
        input.name = key;
        input.value = data[key];
        form.appendChild(input);
    }

    document.body.appendChild(form);
    form.submit();
    document.body.removeChild(form);
}

getData();
function updateEvent() {
    let formData = new FormData(document.querySelector('form'))
    let link = "https://ggst1z4j2i.execute-api.us-east-1.amazonaws.com/prod/esp32?operation=updateEvent&" +
        "eventId=" + formData.get('eventId') +
        "&medication=" + formData.get('medication') +
        "&day=" + formData.get('day') +
        "&time=" + formData.get('time');
    fetch(link)
        .then(resp => resp.json())
        .then(data => updateEventResponse(data));
}

function updateEventResponse(data) {
    console.log(data);
    // document.querySelector("output").innerHTML = "The event has been updated: " +  data['status'];
    getData();
}

function getData() {
    let link = "https://ggst1z4j2i.execute-api.us-east-1.amazonaws.com/prod/esp32?operation=getEvents";
    fetch(link)
        .then(resp => resp.json())
        .then(logs =>  generate_table2(logs));
}

function generate_table2(data) {

    const tbl = document.createElement("table");
    tbl.setAttribute("class", "table")

    // Create table head row with table headers
    const tblHead = document.createElement("thead");
    const row = document.createElement("tr");
    const headers = ["Event #", "Medication", "Day", "Time"];
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
    console.log("Here");
    console.log(data);
    console.log(Object.keys(data).length);
    // console.log(data[1]);
    // console.log(data[1].day);
    // data = JSON.parse(data)
    for (let i = 1; i < Object.keys(data).length + 1; i++) {
        // Create new row
        let row = document.createElement("tr");

        // Add log number to row
        cell = document.createElement("th");
        let cellText = document.createTextNode(i);
        cell.appendChild(cellText);
        row.appendChild(cell);

        // Add log request id to row
        cell = document.createElement("td");
        cellText = document.createTextNode(data[i].medication);
        cell.appendChild(cellText);
        row.appendChild(cell);

        // Add log request id to row
        cell = document.createElement("td");
        cellText = document.createTextNode(data[i].day);
        cell.appendChild(cellText);
        row.appendChild(cell);

        // Time
        cell = document.createElement("td");
        cellText = document.createTextNode(data[i].time);
        cell.appendChild(cellText);
        row.appendChild(cell);

        tblBody.appendChild(row);


        tblBody.appendChild(row);

    }


    tbl.appendChild(tblBody);
    document.getElementById("tableOutput").innerHTML = tbl.outerHTML;


}

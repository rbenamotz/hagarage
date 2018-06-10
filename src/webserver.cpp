#include "webserver.h"
#include <ESP8266WebServer.h>
#include "common.h"
#include "doors.h"
#include "user_config.h"
#include "ota.h"

const char* root_page = "<html>  <head>    <script>      var intervalID1 = setInterval(loadData, 500);      var intervalID2 = setInterval(loadLog, 1000);      function handleDoorStatus(doorNum,status) {        var div = document.getElementById(\"divDoor\" + doorNum + \"Part2\");        var newHeight = \"7em\";        if (\"open\" == status)          newHeight = \"0.5em\";        div.style.height = newHeight;      }      function onload() {        var xhr = new XMLHttpRequest();        xhr.onreadystatechange = function() {          if (xhr.readyState === 4) {            var j = JSON.parse(xhr.response);            mainHeader.innerText = j[\"garageName\"];            var doors = j[\"doors\"];            for (var i=0; i<doors.length; i++) {              var elem = document.getElementById(\"doorHeader\" + (i+1));              elem.innerText = doors[i];            }          }        };        xhr.open(\"GET\", \"/env\");        xhr.send();      }      function toggleDoor(door) {        var xhr = new XMLHttpRequest();        xhr.open(\"POST\", \"/toggle/\" + door);        xhr.send();      }      function loadData() {        var xhr = new XMLHttpRequest();        xhr.onreadystatechange = function() {          if (xhr.readyState === 4) {            var j = JSON.parse(xhr.response);            for (var i=1; i<=2; i++) {              var status = j[\"door\" + i];              handleDoorStatus(i,status);            }          }        };        xhr.open(\"GET\", \"/data\");        xhr.send();      }      function loadLog() {        var xhr = new XMLHttpRequest();        xhr.onreadystatechange = function() {          if (xhr.readyState === 4) {            preAllDoors.innerText = xhr.response;          }        };        xhr.open(\"GET\", \"/log\");        xhr.send();      }    </script>    <style>    body    {      padding: 0;      margin: 0;    }    h1 {      font-size: 2em;      color: #847bf7;      text-transform: uppercase;      font-weight: normal;      line-height: 1;      text-align: center;      margin: 0px 50px 0 50px;      padding-top: -60px;    }    .divAllDoors    {        text-align: center;        display:  table;        width:auto;        border-spacing:1em;        margin:auto;    }    .divRow    {       display:table-row;       width:auto;    }    .divCell    {        float:left;        display:table-column;        width:20em;        height: 10em;        margin-left: 1em;        margin-right: 1em;        background-color:#ccc;        padding-bottom: 1em;        padding-left: 1em;        padding-right: 1em;        margin-bottom: 1em;    }    .doorHeader    {      width: 100%;      height: 1em;      font-weight: bold;      text-align: center;      padding-top: 0.25em;      padding-bottom: 0.25em;    }    .doorPart1 {      width: 100%;      background-color: 606dbc;      height: 1.5em;      margin-bottom: 0.2em;      text-align: center;      color: white;    }    .doorPart2 {      width: 100%;      height: 7em;      margin-bottom: 0.2em;      background: repeating-linear-gradient(45deg, #606dbc, #606dbc 10px, #465298 10px, #465298 20px);    }    #preAllDoors {      border: 1px black dotted;      background-color: #EFEFEF;      width: 80%;      margin: auto;      padding: 1em;      height: 20em;      overflow: auto;    }    .garageButton {      float: right;      width: 1em;      height: 1em;      border: 1px #606dbc dotted;      background-color: #465298;      border-radius: 50%;      display: block;      cursor: pointer;    }    .footer {      position: fixed;      left: 0;      bottom: 0;      width: 100%;      background-color: #EFEFEF;      color: black;      font-size: small;      text-align: center;    }    a {      text-decoration: none;    }    </style>    <title>Garage Controller</title>  </head>  <body onload=\"onload()\">    <h1 id=\"mainHeader\">loading...</h1>    <div class=\"divAllDoors\">      <div class=\"divRow\">        <div class=\"divCell\">          <div class=\"doorHeader\">            <span id=\"doorHeader1\">loading...</span>            <div class=\"garageButton\" onclick=\"toggleDoor(1);\"></div>          </div>          <div class=\"doorPart1\"></div>          <div class=\"doorPart2\" id=\"divDoor1Part2\"></div>        </div>        <div class=\"divCell\">          <div class=\"doorHeader\">            <span id=\"doorHeader2\">loading...</span>            <div class=\"garageButton\" onclick=\"toggleDoor(2);\"></div>          </div>          <div class=\"doorPart1\"></div>          <div class=\"doorPart2\" id=\"divDoor2Part2\"></div>        </div>      </div>    </div>    <pre id=\"preAllDoors\">Loading...</pre>    <div class=\"footer\">created by Roey Benamotz. <a href=\"mailto:roey@benamotz.com\">roey@benamotz.com</a><div>  </body></html>";
const String door_names[] = DOOR_NAMES;

ESP8266WebServer server(80);

void handleRoot()
{
  server.send(200, "text/html", root_page);
}

void buildJsonInt(char* output, String name, int value) {
  char temp[100];
  sprintf(temp,", \"%s\" : %d",name.c_str(),value);
  strcat(output,temp);
}


void handleData() {
  char output[200];
  char temp[20];
  strcpy(output,"{");
  for (int i=0; i<TOTAL_DOORS; i++) {
    if (is_door_open[i]) {
      sprintf(temp,"\"door%d\" : \"%s\"",i+1,"open");
    } else {
      sprintf(temp,"\"door%d\" : \"%s\"",i+1,"closed");
    }
    if (i>0) {
      strcat(output,",");
    }
    strcat(output, temp);
  }
  buildJsonInt(output, "ota",is_ota_enabled);
  strcat(output,"}");
  server.send(200, "application/json", output);
}

void handleToggle() {
  String s = server.uri().substring(server.uri().length() - 1);
  int door = s.toInt();
  write_to_log("Toggle for door %d called from REST API",door);
  toggle_door_state(door - 1);
  server.send(200,"application/json","");
}

void handleLog() {
  server.send(200,"text/plain",read_log_buffer());
}

void handleOta() {
  is_ota_enabled = !is_ota_enabled;
  char temp[10];
  sprintf(temp,"{\"ota\" : %d}", is_ota_enabled);
  server.send(200,"application/json",temp);
}

void handleRestart() {
  write_to_log("Restarting due to API request");
  ESP.restart();
}

void handleEnviroment() {
  String output = "{";
  output+= "\"garageName\" : \"";
  output+= GARAGE_NAME;
  output+= "\", \"doors\" : [";
  for (int i=0; i<TOTAL_DOORS; i++) {
    if (i>0) {
      output+= ", ";
    }
    output+= "\"";
    output+= door_names[i];
    output+= "\"";
  }
  output+= "]";
  output+= "}";
  server.send(200,"application/json",output);
}


void init_web_server() {
  char temp[10];
  server.on("/", handleRoot);
  for (int i=0; i<TOTAL_DOORS; i++) {
    sprintf(temp,"/toggle/%d",i+1);
    server.on(temp,HTTP_POST, handleToggle);
  }
  server.on("/env", handleEnviroment);
  server.on("/log", handleLog);
  server.on("/data", handleData);
  server.on("/ota", handleOta);
  server.on("/rst", handleRestart);
  server.begin();
  write_to_log("Web server listening");
}

void loop_web_server() {
  server.handleClient();
}

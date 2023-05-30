//Bibliotecas
#include <Wifi.h>

//Constantes
const char* ssid = "NOME_REDE_WIFI";
const char* password = "SENHA_REDE_WIFI";

#define correnteMax 15
#define correnteMin 0.2
#define tensaobulkMax 29
#define tensaofloatMax 26.8
#define tensaofloatMin 26.4

// Constantes para mapeamento
#define tensaomax 29.0
#define tensaomin 20.0
#define correnteAbsMax 15.0
#define correnteAbsMin 0.2
#define tensoabateriaMin 20.0
#define tensoabateriaMax 26.0


//Variaveis
float tensaobateria = 0;
float correntebateria = 0;
float tensaoplaca = 0;
float correnteplaca = 0;
float potenciaplaca = 0;
float Nivelbateria = 0;
int mode = 0;
String modeStr = "OFF";

// Definição das portas
WebServer server(80);
const int tensaopl = 34;
const int correntepl = 35;
const int tensaobt = 36;
const int correntebt = 39;


void setup() {
Serial.begin(115200);
Serial.println();
Serial.print("Conectando-se a ");
Serial.println(ssid);
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}

Serial.println("");
Serial.println("WiFi conectada.");
Serial.println("Endereço de IP: ");
Serial.println(WiFi.localIP());
 
server.begin();

}

void loop() {
 
//analise de tensao na bateria
tensaobateria = analogRead(tensaobt);
tensaobateria = (tensaobateria * 29.4) / 4095.0;

//analise de corrente e potencia nas placas
tensaoplaca = analogRead(tensaopl);
tensaoplaca = (tensaoplaca * 42) / 4095.0;

//coleta de dados com sensor ACS720
correnteplaca = analogRead(correntebt);
correnteplaca = (correnteplaca*26.66*0.185)/4095.0;

//analise da potencia da placas
potenciaplaca = tensaoplaca * correnteplaca;

//estrutura condicinal para determinação de estagio de carregamento e nivel de bateria
if (correnteplaca == 0){
  mode=0;
  modeStr= "OFF";
  Nivelbateria = mapFloat(tensaobateria, tensoabateriaMin, tensoabateriaMax, 0.0, 100.0);
 }
if(tensaobateria < tensaobulkMax && correnteplaca == correnteMax){
  mode=1
  modeStr= "BULK";
  Nivelbateria = mapFloat(tensaobateria, tensaomin, tensaomax, 0.0, 80.0);
 }
if(tensaobateria > tensaobulkMax && correnteplaca <= correnteMax && correnteplaca >= correnteMin){
 mode=2;
 modeStr= "ABSORTION";
 Nivelbateria = mapFloat(tensaobateria, correnteAbsMax, correnteAbsMin, 81.0, 99.0);
 }
if (tensaobateria >= tensaofloatMin && tensaobateria <= tensaofloatMax && correnteplaca <= correnteMin){
  mode=3;
  modeStr= "FLOAT";
  Nivelbateria = 100.0
 }  

WiFiClient client = server.available();
  if (client) {
    Serial.println("Novo Cliente Conectado.");
    while (client.connected()) {
      if (client.available()) {

String response = "HTTP/1.1 200 OK\n";
response += "Content-Type: text/html\n";
response += "Connection: close\n";
response += "\n";
response += "<!DOCTYPE html>\n";
response += "<html>\n";
response += "<head>\n";
response += " <title>Leitura Dos Dados</title>\n";
response += "<meta charset="UTF-8">\n";
response += " <style>\n";
response += " body {\n";
response += " font-family: Arial, sans-serif;\n";
response += " text-align: center;\n";
response += " background-color: #111d32;\n";
response += " color: #ffffff;\n";
response += " margin: 0;\n";
response += " padding: 0;\n";
response += " }\n";
response += " h1 {\n";
response += " margin-top: 140px;\n";
response += " font-size: 36px;\n";
response += " font-weight: bold;\n";
response += " text-transform: none;\n";
response += " }\n";
response += " .container {\n";
response += " display: flex;\n";
response += " justify-content: space-around;\n";
response += " margin-top: 30px;\n";
response += " }\n";
response += " .gauge {\n";
response += " width: 250px;\n";
response += " height: 250px;\n";
response += " display: inline-block;\n";
response += " }\n";
response += " .value {\n";
response += " font-size: 28px;\n";
response += " fill: #ffffff;\n";
response += " }\n";
response += " .label {\n";
response += " margin-top: 20px;\n";
response += " color: #ffffff;\n";
response += " }\n";
response += " .logo {\n";
response += " position: absolute;\n";
response += " top: 10px;\n";
response += " left: 10px;\n";
response += " width: 400px;\n";
response += " height: 133px;\n";
response += " }\n";
response += " .button-container {\n";
response += " margin-top: 20px;\n";
response += " }\n";
response += " .update-button {\n";
response += " font-size: 18px;\n";
response += " margin-top: 10px;\n";
response += " }\n";
response += " .variable-value {\n";
response += " font-size: 24px;\n";
response += " color: #ffffff;\n";
response += " margin-top: 10px;\n";
response += " }\n";
response += " </style>\n";
response += "</head>\n";
response += "<body>\n";
response += " <img class="logo" src="https://storage.googleapis.com/production-hostgator-brasil-v1-0-0/550/364550/v9bPQxy0/cfcafd65777e43dcacfbf8df13381dba\" alt="Logo">\n";
response += " <h1>Leitura dos Dados</h1>\n";
response += " <div class="container">\n";
response += " <div>\n";
response += "  <canvas class=\"gauge\" id=\"tensaobtGauge\" width=\"250\" height=\"250\"></canvas>\n";
response += "  <div class=\"variable-value\" id=\"tensaobtValue\">Tensão BT (V): 0</div>\n";
response += "  <div class=\"variable-value\" id=\"modeValue\">Modo: OFF</div>\n";
response += "  <div class=\"variable-value\" id=\"niveldebateriaValue\">Nível de energia: 0%</div>\n";
response += "</div>\n";
response += "<div>\n";
response += "  <canvas class=\"gauge\" id=\"correnteGauge\" width=\"250\" height=\"250\"></canvas>\n";
response += "  <div class=\"variable-value\" id=\"correnteValue\">Corrente (A): 0</div>\n";
response += "</div>\n";
response += "<div>\n";
response += "  <canvas class=\"gauge\" id=\"potenciaGauge\" width=\"250\" height=\"250\"></canvas>\n";
response += "  <div class=\"variable-value\" id=\"potenciaValue\">Potência (W): 0</div>\n";
response += "</div>\n";
response += "</div>\n";
response += "<script>\n";
response += "  function drawGauge(canvasId, value, maxValue, label) {\n";
response += "    var canvas = document.getElementById(canvasId);\n";
response += "    var ctx = canvas.getContext(\"2d\");\n";
response += "    var centerX = canvas.width / 2;\n";
response += "    var centerY = canvas.height / 2;\n";
response += "    var radius = canvas.width / 2 - 20;\n";
response += "    var startAngle = -Math.PI / 2;\n";
response += "    var endAngle = startAngle + (Math.PI * 2) * (value / maxValue);\n";
response += "    var percentage = ((value / maxValue) * 100).toFixed(1);\n";
response += "    ctx.clearRect(0, 0, canvas.width, canvas.height);\n";
response += "    ctx.beginPath();\n";
response += "    ctx.arc(centerX, centerY, radius, 0, Math.PI * 2);\n";
response += "    ctx.lineWidth = 30;\n";
response += "    ctx.strokeStyle = \"#444444\";\n";
response += "    ctx.stroke();\n";
response += "    ctx.beginPath();\n";
response += "    ctx.arc(centerX, centerY, radius, startAngle, endAngle);\n";
response += "    ctx.lineWidth = 30;\n";
response += "    if (canvasId === \"tensaobtGauge\") {\n";
response += "      ctx.strokeStyle = \"#00FF00\";\n";
response += "    } else {\n";
response += "      ctx.strokeStyle = \"#FFC107\";\n";
response += "    }\n";
response += "    ctx.stroke();\n";
response += "    ctx.font = \"28px Arial\";\n";
response += "    ctx.fillStyle = \"#ffffff\";\n";
response += "    ctx.textAlign = \"center\";\n";
response += "    ctx.fillText(percentage + \"%\", centerX, centerY);\n";
response += "    ctx.font = \"14px Arial\";\n";
response += "    ctx.fillStyle = \"#ffffff\";\n";
response += "    ctx.textAlign = \"center\";\n";
response += "    ctx.fillText(label, centerX, centerY + 20);\n";
response += "  }\n";
response += "  function updateGauges() {\n";
response += "      var xhttp = new XMLHttpRequest();\n";
response += "      xhttp.onreadystatechange = function() {\n";
response += "        if (this.readyState == 4 && this.status == 200) {\n";
response += "          var values = JSON.parse(this.responseText);\n
response += "    var niveldebateria = Nivelbateria;\n";
response += "    var tensaobtValue = tensaobateria;\n";
response += "    var correnteValue = correnteplaca;\n";
response += "    var potenciaValue = potenciaplaca;\n";
response += "    var mode = modeStr;\n";
response += "    drawGauge(\"tensaobtGauge\", tensaobtValue, 29.4, \"Baterias (V)\");\n";
response += "    drawGauge(\"correnteGauge\", correnteValue, 26.66, \"Corrente (A)\");\n";
response += "    drawGauge(\"potenciaGauge\", potenciaValue, 1120, \"Potência (W)\");\n";
response += "    document.getElementById(\"tensaobtValue\").textContent = \"Tensão BT (V): \" + tensaobtValue.toFixed(2);\n";
response += "    document.getElementById(\"correnteValue\").textContent = \"Corrente (A): \" + correnteValue.toFixed(2);\n";
response += "    document.getElementById(\"potenciaValue\").textContent = \"Potência (W): \" + potenciaValue.toFixed(2);\n";
response += "  }\n";
response += "      };\n";
response += "      xhttp.open(\"GET\", \"/values\", true);\n";
response += "      xhttp.send();\n";
response += "    }\n";
response += "  setInterval(updateGauges, 1000);\n";
response += "</script>\n";
response += "</body>\n";
response += "</html>\n";

client.print(response);
delay(1)
client.stop();
Serial.println("Cliente Disconectado.");
    }
  }
}

}


// Função de mapeamento para a corrente
float mapFloat(float x, float inMin, float inMax, float outMin, float outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}



//Bibliotecas
#include <Wifi.h>

//Constantes
const char* ssid = "NOME_REDE_WIFI";
const char* password = "SENHA_REDE_WIFI";

#define correnteMax 15
#define correnteMin 0.2
#define tensaobulkMax 29
#define tensaofloatMax 26.8
#define tensaofloatMin 26.4

// Constantes para mapeamento
#define tensaomax 29.0
#define tensaomin 20.0
#define correnteAbsMax 15.0
#define correnteAbsMin 0.2
#define tensoabateriaMin 20.0
#define tensoabateriaMax 26.0


//Variaveis
float tensaobateria = 0;
float correntebateria = 0;
float tensaoplaca = 0;
float correnteplaca = 0;
float potenciaplaca = 0;
float Nivelbateria = 0;
int mode = 0;
string modeStr = "OFF";

// Definição das portas
WebServer servidor(80);
const int tensaopl = 34;
const int correntepl = 35;
const int tensaobt = 36;
const int correntebt = 39;


void setup() {
Serial.begin(115200);
Serial.println();
Serial.print("Conectando-se a ");
Serial.println(ssid);
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}

Serial.println("");
Serial.println("WiFi conectada.");
Serial.println("Endereço de IP: ");
Serial.println(WiFi.localIP());
 
server.begin();

}

void loop() {
 
//analise de tensao na bateria
tensaobateria = analogRead(tensaobt);
tensaobateria = (tensaobateria * 29.4) / 4095.0;

//analise de corrente e potencia nas placas
tensaoplaca = analogRead(tensaopl);
tensaoplaca = (tensaoplaca * 42) / 4095.0;

//coleta de dados com sensor ACS720
correnteplaca = analogRead(correntebt);
correnteplaca = (correnteplaca*26.66*0.185)/4095.0;

//analise da potencia da placas
potenciaplaca = tensaoplaca * correnteplaca;

//estrutura condicinal para determinação de estagio de carregamento e nivel de bateria
if (correnteplaca == 0){
  mode=0;
  modeStr= "OFF";
  Nivelbateria = mapFloat(tensaobateria, tensoabateriaMin, tensoabateriaMax, 0.0, 100.0);
 }
if(tensaobateria < tensaobulkMax && correnteplaca == correnteMax){
  mode=1
  modeStr= "BULK";
  Nivelbateria = mapFloat(tensaobateria, tensaomin, tensaomax, 0.0, 80.0);
 }
if(tensaobateria > tensaobulkMax && correnteplaca <= correnteMax && correnteplaca >= correnteMin){
 mode=2;
 modeStr= "ABSORTION";
 Nivelbateria = mapFloat(tensaobateria, correnteAbsMax, correnteAbsMin, 81.0, 99.0);
 }
if (tensaobateria >= tensaofloatMin && tensaobateria <= tensaofloatMax && correnteplaca <= correnteMin){
  mode=3;
  modeStr= "FLOAT";
  Nivelbateria = 100.0
 }  

WiFiClient client = server.available();
  if (client) {
    Serial.println("Novo Cliente Conectado");
    while (client.connected()) {
      if (client.available()) {

String response = "HTTP/1.1 200 OK\n";
response += "Content-Type: text/html\n";
response += "Connection: close\n";
response += "\n";
response += "<!DOCTYPE html>\n";
response += "<html>\n";
response += "<head>\n";
response += " <title>Leitura Dos Dados</title>\n";
response += "<meta charset="UTF-8">\n";
response += " <style>\n";
response += " body {\n";
response += " font-family: Arial, sans-serif;\n";
response += " text-align: center;\n";
response += " background-color: #111d32;\n";
response += " color: #ffffff;\n";
response += " margin: 0;\n";
response += " padding: 0;\n";
response += " }\n";
response += " h1 {\n";
response += " margin-top: 140px;\n";
response += " font-size: 36px;\n";
response += " font-weight: bold;\n";
response += " text-transform: none;\n";
response += " }\n";
response += " .container {\n";
response += " display: flex;\n";
response += " justify-content: space-around;\n";
response += " margin-top: 30px;\n";
response += " }\n";
response += " .gauge {\n";
response += " width: 250px;\n";
response += " height: 250px;\n";
response += " display: inline-block;\n";
response += " }\n";
response += " .value {\n";
response += " font-size: 28px;\n";
response += " fill: #ffffff;\n";
response += " }\n";
response += " .label {\n";
response += " margin-top: 20px;\n";
response += " color: #ffffff;\n";
response += " }\n";
response += " .logo {\n";
response += " position: absolute;\n";
response += " top: 10px;\n";
response += " left: 10px;\n";
response += " width: 400px;\n";
response += " height: 133px;\n";
response += " }\n";
response += " .button-container {\n";
response += " margin-top: 20px;\n";
response += " }\n";
response += " .update-button {\n";
response += " font-size: 18px;\n";
response += " margin-top: 10px;\n";
response += " }\n";
response += " .variable-value {\n";
response += " font-size: 24px;\n";
response += " color: #ffffff;\n";
response += " margin-top: 10px;\n";
response += " }\n";
response += " </style>\n";
response += "</head>\n";
response += "<body>\n";
response += " <img class="logo" src="https://storage.googleapis.com/production-hostgator-brasil-v1-0-0/550/364550/v9bPQxy0/cfcafd65777e43dcacfbf8df13381dba\" alt="Logo">\n";
response += " <h1>Leitura dos Dados</h1>\n";
response += " <div class="container">\n";
response += " <div>\n";
response += "  <canvas class=\"gauge\" id=\"tensaobtGauge\" width=\"250\" height=\"250\"></canvas>\n";
response += "  <div class=\"variable-value\" id=\"tensaobtValue\">Tensão BT (V): 0</div>\n";
response += "  <div class=\"variable-value\" id=\"modeValue\">Modo: OFF</div>\n";
response += "  <div class=\"variable-value\" id=\"niveldebateriaValue\">Nível de energia: 0%</div>\n";
response += "</div>\n";
response += "<div>\n";
response += "  <canvas class=\"gauge\" id=\"correnteGauge\" width=\"250\" height=\"250\"></canvas>\n";
response += "  <div class=\"variable-value\" id=\"correnteValue\">Corrente (A): 0</div>\n";
response += "</div>\n";
response += "<div>\n";
response += "  <canvas class=\"gauge\" id=\"potenciaGauge\" width=\"250\" height=\"250\"></canvas>\n";
response += "  <div class=\"variable-value\" id=\"potenciaValue\">Potência (W): 0</div>\n";
response += "</div>\n";
response += "</div>\n";
response += "<script>\n";
response += "  function drawGauge(canvasId, value, maxValue, label) {\n";
response += "    var canvas = document.getElementById(canvasId);\n";
response += "    var ctx = canvas.getContext(\"2d\");\n";
response += "    var centerX = canvas.width / 2;\n";
response += "    var centerY = canvas.height / 2;\n";
response += "    var radius = canvas.width / 2 - 20;\n";
response += "    var startAngle = -Math.PI / 2;\n";
response += "    var endAngle = startAngle + (Math.PI * 2) * (value / maxValue);\n";
response += "    var percentage = ((value / maxValue) * 100).toFixed(1);\n";
response += "    ctx.clearRect(0, 0, canvas.width, canvas.height);\n";
response += "    ctx.beginPath();\n";
response += "    ctx.arc(centerX, centerY, radius, 0, Math.PI * 2);\n";
response += "    ctx.lineWidth = 30;\n";
response += "    ctx.strokeStyle = \"#444444\";\n";
response += "    ctx.stroke();\n";
response += "    ctx.beginPath();\n";
response += "    ctx.arc(centerX, centerY, radius, startAngle, endAngle);\n";
response += "    ctx.lineWidth = 30;\n";
response += "    if (canvasId === \"tensaobtGauge\") {\n";
response += "      ctx.strokeStyle = \"#00FF00\";\n";
response += "    } else {\n";
response += "      ctx.strokeStyle = \"#FFC107\";\n";
response += "    }\n";
response += "    ctx.stroke();\n";
response += "    ctx.font = \"28px Arial\";\n";
response += "    ctx.fillStyle = \"#ffffff\";\n";
response += "    ctx.textAlign = \"center\";\n";
response += "    ctx.fillText(percentage + \"%\", centerX, centerY);\n";
response += "    ctx.font = \"14px Arial\";\n";
response += "    ctx.fillStyle = \"#ffffff\";\n";
response += "    ctx.textAlign = \"center\";\n";
response += "    ctx.fillText(label, centerX, centerY + 20);\n";
response += "  }\n";
response += "  function updateGauges() {\n";
response += "      var xhttp = new XMLHttpRequest();\n";
response += "      xhttp.onreadystatechange = function() {\n";
response += "        if (this.readyState == 4 && this.status == 200) {\n";
response += "          var values = JSON.parse(this.responseText);\n
response += "    var niveldebateria = Nivelbateria;\n";
response += "    var tensaobtValue = tensaobateria;\n";
response += "    var correnteValue = correnteplaca;\n";
response += "    var potenciaValue = potenciaplaca;\n";
response += "    var mode = modeStr;\n";
response += "    drawGauge(\"tensaobtGauge\", tensaobtValue, 29.4, \"Baterias (V)\");\n";
response += "    drawGauge(\"correnteGauge\", correnteValue, 26.66, \"Corrente (A)\");\n";
response += "    drawGauge(\"potenciaGauge\", potenciaValue, 1120, \"Potência (W)\");\n";
response += "    document.getElementById(\"tensaobtValue\").textContent = \"Tensão BT (V): \" + tensaobtValue.toFixed(2);\n";
response += "    document.getElementById(\"correnteValue\").textContent = \"Corrente (A): \" + correnteValue.toFixed(2);\n";
response += "    document.getElementById(\"potenciaValue\").textContent = \"Potência (W): \" + potenciaValue.toFixed(2);\n";
response += "  }\n";
response += "      };\n";
response += "      xhttp.open(\"GET\", \"/values\", true);\n";
response += "      xhttp.send();\n";
response += "    }\n";
response += "  setInterval(updateGauges, 1000);\n";
response += "</script>\n";
response += "</body>\n";
response += "</html>\n";

 client.print(response);
 delay(1);

 }
       }
    }
client.stop();
Serial.println("Client Disconnected.");

}


// Função de mapeamento para a corrente
float mapFloat(float x, float inMin, float inMax, float outMin, float outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}








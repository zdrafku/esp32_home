/* Web setver setup and template parsing */
#include "home_web_spiffs.h"
#include <AsyncElegantOTA.h>

AsyncWebServer server(80);

// https://github.com/me-no-dev/ESPAsyncWebServer#respond-with-content-coming-from-a-file-containing-templates

String web_request;
String relay_id;
String relay_status;
String auto_id;
String auto_status;
String switch_id;
String switch_status;
String timer_id;
int door_id;

String template_processor(const String& var) {
  if ( var == "web_uptime" ) return web_uptime;
  if ( var == "web_ip" ) return web_ip;
  if ( var == "config.wifi_ssid" ) return config.wifi_ssid;
  if ( var == "if_config_ap_enabled" ) {
    if ( !config.ap_enabled || !config.ap_ssid ) return String();
    return "<tr>\n"
           "\t<td align=\"right\" colspan=2>AP SSID: </td>\n"
           "\t<td align=\"left\" colspan=2>" + config.ap_ssid + "</td>\n"
           "</tr>";
  }
  if ( var == "config.ap_ssid" ) return config.ap_ssid;
  if ( var == "config.ota_host" ) return config.ota_host;
  if ( var == "tepm" ) return String(t);
  if ( var == "hum" ) return String((int)h);
  if ( var == "relays" ) {
    String relay_line;
    relay_line = "<tr>\n\t\t\t\t<td>&nbsp;</td>\n"
      "\t\t\t\t<td width=\"40px\" align=\"left\">R</td>\n";
    if ( config.switches[0] ) {
      relay_line += "\t\t\t\t<td width=\"40px\" align=\"left\">SW</td>\n";
    } else {
      relay_line += "\t\t\t\t<td></td>\n";
    }
    relay_line += "\t\t\t\t<td width=\"40px\">Auto</td>\n\t\t\t</tr>";

    for (int pin = 0; pin < PIN_NUM; pin++) {
      if ( !config.relays[pin] || (config.door_opener_position -1) == pin || (config.laser_pin_position -1) == pin ) continue;
      if ( web_request == "relay" && relay_id.toInt() == pin ) {
        digitalWrite(config.relays[pin], relay_status.toInt());
        config.auto_status[pin] = false;
      }
      if ( web_request == "auto" && auto_id.toInt() == pin ) {
        relay_previousMillis[pin] = millis();
        config.auto_status[pin] = auto_status.toInt();
        if (config.auto_status[pin] && !digitalRead(config.switches[pin])) digitalWrite(config.relays[pin], config.relays_defaults[pin]);
      }
      if ( web_request == "switch" && switch_id.toInt() == pin ) {
        digitalWrite(config.relays[pin], switch_status.toInt());
        digitalWrite(config.switches[pin], switch_status.toInt());
      }
      relay_line += "\t\t\t<tr>\n\t\t\t\t<td align=\"left\"><span class=\"dht-labels\">Relay " + String(pin + 1) + " (" + config.info[pin] + ")</span>";
      if ( config.auto_status[pin] && relay_previousMillis[pin] > 10 && config.intervals[pin] && config.intervals[pin]*60*1000 >= millis() - relay_previousMillis[pin]) {
        relay_line += "<br>";
        relay_line += config.hum_auto[pin];
        relay_line += "%% for ";
        relay_line += config.intervals[pin];
        relay_line += "mins ";
        // char mins[5];
        // sprintf(mins, "(%02d:", (int)((millis() - relay_previousMillis[pin])/1000/60)%60); // minutes
        // char secs[4];
        // sprintf(secs, "%02d)", (int)((millis() - relay_previousMillis[pin])/1000)%60); // seconds
        // relay_line += mins;
        // relay_line += secs;
      }
      relay_line += "</td>\n";

      // relay status
      if ( digitalRead(config.relays[pin]) ) {
        relay_line += "\t\t\t\t<td><a href=\"/relay/" + String(pin) + "/0\"><i class=\"fas fa-lightbulb\" style=\"color:#F5F33F;\"></i></a> " + config.relays[pin] + "</td>\n";
      } else {
        relay_line += "\t\t\t\t<td><a href=\"/relay/" + String(pin) + "/1\"><i class=\"fas fa-lightbulb\" style=\"color:#E4E4E4;\"></i></a> " + config.relays[pin] + "</td>\n";
      }
      // switch status
      if ( config.switches[0] ) {
        if ( digitalRead(config.switches[pin]) ) {
          relay_line += "\t\t\t\t<td><a href=\"/switch/" + String(pin) + "/0\"><i class=\"fas fa-mobile\" style=\"color:#F5F33F;\"></i></a> " + config.switches[pin] + "</td>\n";
          //relay_line += "\t\t\t\t<td><i class=\"fas fa-mobile\" style=\"color:#F5F33F;\"></i> " + config.switches[pin] + "</td>\n";
        } else {
          relay_line += "\t\t\t\t<td><a href=\"/switch/" + String(pin) + "/1\"><i class=\"fas fa-mobile-alt\" style=\"color:#E4E4E4;\"></i></a> " + config.switches[pin] + "</td>\n";
          //relay_line += "\t\t\t\t<td><i class=\"fas fa-mobile-alt\" style=\"color:#E4E4E4;\"></i> " + config.switches[pin] + "</td>\n";
        }
      } else {
          relay_line += "\t\t\t\t<td></td>\n";
      }
      // set automatically
      if ( config.auto_status[pin] ) {
        relay_line += "\t\t\t\t<td><a href=\"/auto/" + String(pin) + "/0\"><i class=\"fas fa-toggle-on\" style=\"color:#00add6;\"></i></a> " + config.auto_status[pin] + "</td>\n";
      } else {
        relay_line += "\t\t\t\t<td><a href=\"/auto/" + String(pin) + "/1\"><i class=\"fas fa-toggle-off\" style=\"color:#F78484;\"></i></a> " + config.auto_status[pin] + "</td>\n";
      }
			relay_line += "\t\t\t</tr>\n";
    }
    return relay_line;
  }
  if ( var == "reset_timer" && config.reset_timer ) {
    String line = "<p><a href=\"/timer/";
    line += config.reset_timer;
    line += "\"><i class=\"fas fa-biohazard\" style=\"color:green;\"></i></a></p>";
    if ( web_request == "timer" ) h = config.hum_auto[timer_id.toInt() -1];
    return line;
  }
  if ( var == "door_opener" && config.door_opener_position ) {
    String line = "<p><a href=\"/door/cycle/";
    line += config.door_opener_position;
    line += "\"><i class=\"fas fa-sync-alt\" style=\"color:#F78484;\"></i></a></p>";
    return line;
  }
  web_request = "";
  return String();
}

void web_spiffs_setup() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, template_processor);
  });
  server.on("^\\/(relay)\\/([0-9]+)\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
    web_request = request->pathArg(0);
    relay_id = request->pathArg(1);
    relay_status = request->pathArg(2);
    request->send(SPIFFS, "/index.html", String(), false, template_processor);
  });
  server.on("^\\/(auto)\\/([0-9]+)\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
    web_request = request->pathArg(0);
    auto_id = request->pathArg(1);
    auto_status = request->pathArg(2);
    request->send(SPIFFS, "/index.html", String(), false, template_processor);
  });
  if ( config.switches[0] ) {
    server.on("^\\/(switch)\\/([0-9]+)\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
      web_request = request->pathArg(0);
      switch_id = request->pathArg(1);
      switch_status = request->pathArg(2);
      request->send(SPIFFS, "/index.html", String(), false, template_processor);
    });
  }
  server.on("^\\/(timer)\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
    web_request = request->pathArg(0);
    timer_id = request->pathArg(1);
    request->send(SPIFFS, "/index.html", String(), false, template_processor);
  });
  server.on("^\\/(door)/cycle\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
    web_request = request->pathArg(0);
    door_cycle(request->pathArg(1).toInt());
    request->send(SPIFFS, "/index.html", String(), false, template_processor);
  });
  server.on("^\\/(restart)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
    web_request = request->pathArg(0);
    // request->send(SPIFFS, "/index.html", String(), false, template_processor);
    ESP.restart();
  });
  AsyncElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
}
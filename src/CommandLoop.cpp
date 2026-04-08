#include "logging.h"
#include "sensors.h"
#include "Control.h"
#include "RoasterPrefs.h"
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <cmath>
#include <cstring>


void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len) {

  switch (type) {
  case WS_EVT_CONNECT:
    logf("[%u] Connected!\n", client->id());
    // client->text("Connected");

    break;
  case WS_EVT_DISCONNECT: {
    logf("[%u] Disconnected!\n", client->id());
    // turn off heater and set fan to 100%
  } break;
  case WS_EVT_DATA: {

    AwsFrameInfo *info = (AwsFrameInfo *)arg;
#ifdef DEBUG
    logf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(),
         (info->opcode == WS_TEXT) ? "text" : "binary", info->len);
    logf("final: %d\n", info->final);
#endif
    String msg = "";
    /*if (info->opcode != WS_TEXT || !info->final) {*/
    /*  break;*/
    /*}*/

    for (size_t i = 0; i < info->len; i++) {
      msg += (char)data[i];
    }
#ifdef DEBUG
    logf("msg: %s\n", msg.c_str());
#endif

    JsonDocument doc;

    // DEBUG WEBSOCKET
    // logf("[%u] get Text: %s\n", num, payload);

    // Extract Values lt. https://arduinojson.org/v6/example/http-client/
    // Artisan Anleitung: https://artisan-scope.org/devices/websockets/

    deserializeJson(doc, msg);

    long ln_id = doc["id"].as<long>();
    // Get BurnerVal from Artisan over Websocket
    if ((doc["Mode"].isNull() || strncmp(doc["Mode"].as<const char *>(), "Manual", 6) == 0) && !doc["BurnerVal"].isNull()) {
      float val = doc["BurnerVal"].as<float>();
      logf("BurnerVal: %d\n", val);
      // DimmerVal = doc["BurnerVal"].as<long>();
      setHeater(val);
    }
    if (!doc["Mode"].isNull() && strncmp(doc["Mode"].as<const char *>(), "PID", 3) == 0 && !doc["Setpoint"].isNull()) {
      float setpoint = doc["Setpoint"].as<float>();
      logf("Setpoint: %d\n", setpoint);
      setSetpoint(setpoint);
    }
    if (!doc["Target"].isNull()) {
      const char *target = doc["Target"].as<const char *>();
      if (strncmp(target, "BT", 2) == 0)
        setTemperatureTarget(TemperatureTarget::BT);
      if (strncmp(target, "ET", 2) == 0)
        setTemperatureTarget(TemperatureTarget::ET);
      if (strncmp(target, "MAX", 3) == 0)
        setTemperatureTarget(TemperatureTarget::MAX);
    }
    if (!doc["FanVal"].isNull()) {
      float fanVal = doc["FanVal"].as<float>();
      logf("FanVal: %d\n", fanVal);
      setFan(fanVal);
    }

    // Send Values to Artisan over Websocket
    const char *command = doc["command"].as<const char *>();
    if (command != NULL && strncmp(command, "setBurner", 9) == 0) {
      long val = doc["value"].as<long>();
      logf("BurnerVal: %d\n", val);
      setHeater(val);
    }
    if (command != NULL && strncmp(command, "setFan", 6) == 0) {
      long val = doc["value"].as<long>();
      logf("FanVal: %d\n", val);
      setFan(val);
    }

    if (command != NULL && strncmp(command, "autotune", 8) == 0) {
      if (getFan() < 30) setFan(60);
      startAutotune();
    }

    if (command != NULL && strncmp(command, "setPreferences", 14) == 0) {
      if (!doc["pidKp"].isNull() && !doc["pidKi"].isNull() && !doc["pidKd"].isNull()) {
        float pidKp = doc["pidKp"].as<float>();
        float pidKi = doc["pidKi"].as<float>();
        float pidKd = doc["pidKd"].as<float>();
        setFloatValue("pidKp", pidKp);
        setFloatValue("pidKi", pidKi);
        setFloatValue("pidKd", pidKd);
        setPidValues(pidKp, pidKi, pidKd);
      }

      if (!doc["cooldownFanSpeed"].isNull()) {
        long cooldownFanSpeed = doc["cooldownFanSpeed"].as<long>();
        logf("cooldownFanSpeed: %d\n", cooldownFanSpeed);
        setLongValue("coolFanSpeed", cooldownFanSpeed);
      }
    }

    if (command != NULL && (strncmp(command, "setPreferences", 14) == 0 || strncmp(command, "getPreferences", 14) == 0)) {
      JsonObject root = doc.to<JsonObject>();
      JsonObject data = root["data"].to<JsonObject>();

      root["id"] = ln_id;
      data["type"] = "preferences";
      data["pidKp"] = getFloatValue("pidKp", 1.0);
      data["pidKi"] = getFloatValue("pidKi", 0.1);
      data["pidKd"] = getFloatValue("pidKd", 0.01);
      data["cooldownFanSpeed"] = getLongValue("coolFanSpeed", 65);
    }

    if (command != NULL && strncmp(command, "getData", 7) == 0) {
      JsonObject root = doc.to<JsonObject>();
      JsonObject data = root["data"].to<JsonObject>();
      root["id"] = ln_id;
      float etbt[3];
      getETBTReadings(etbt);
      data["type"] = "status";
      data["ET"] = etbt[0]; // Med_ExhaustTemp.getMedian()
      data["BT"] = etbt[1]; // Med_BeanTemp.getMedian();
      data["Amb"] = etbt[2];
      data["BurnerVal"] = getHeater();
      data["Setpoint"] = getSetpoint();
      data["Target"] = getTemperatureTarget();
      data["Mode"] = getMode();
      data["FanVal"] = getFan();
      data["pidKp"] = getKp();
      data["pidKi"] = getKi();
      data["pidKd"] = getKd();
    }

    char buffer[200];                        // create temp buffer
    size_t len = serializeJson(doc, buffer); // serialize to buffer
    // DEBUG WEBSOCKET
    log(buffer);

    client->text(buffer);
    // send message to client
    // webSocket.sendTXT(num, "message here");

    // send data to all connected clients
    // webSocket.broadcastTXT("message here");
  } break;
  default: // send message to client
    logf("unhandled message type: %d\n", type);
    // webSocket.sendBIN(num, payload, length);
    break;
  }
}

void setupMainLoop(AsyncWebSocket *ws) {
  ws->onEvent(onWsEvent);
}

#include "logging.h"
#include <ESPAsyncWebServer.h>
#include <Preferences.h>

#include "preferenceKeys.h"

void setupApi(AsyncWebServer *server, Preferences *preferences) {
  log("setting up api");
  server->on("/api/wifi", HTTP_GET, [preferences](AsyncWebServerRequest *request) {
    if (!request->hasParam("ssid") || !request->hasParam("pass")) {
      AsyncWebServerResponse *response = request->beginResponse(400);
      request->send(response);
      return;
    }

    const char *ssid = request->getParam("ssid")->value().c_str();
    const char *pass = request->getParam("pass")->value().c_str();

    preferences->putString(wifiSSIDKey, ssid);
    preferences->putString(wifiPassKey, pass);
    logf("saving to prefs, ssid: %s", ssid);
    request->send(200);
  });
}

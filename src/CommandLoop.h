#ifndef COMMANDLOOP_H
#define COMMANDLOOP_H
#include <Preferences.h>
#include "AsyncWebSocket.h"
#include "Control.h"

class WSRequestHandler {
private:
  Control *control;
  Preferences *preferences;

public:
  WSRequestHandler(AsyncWebSocket *ws,Control *control, Preferences *preferences);
  void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                 AwsEventType type, void *arg, uint8_t *data, size_t len);
};
#endif // COMMANDLOOP_H

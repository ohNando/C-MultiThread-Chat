#ifndef GUI_BRIDGE_H
#define GUI_BRIDGE_H

#include "common.h"
#include "utility.h"
#include "queue.h"

#include <libwebsockets.h>

#define PROTOCOL_NAME "tcp-chat-protocol"
#define WEBSOCKET_PORT 8081

extern MessageQueue queue_to_gui;
extern MessageQueue queue_from_gui;

void* start_websocket_server(void* arg);
int send_message_to_gui(const char*);

#endif
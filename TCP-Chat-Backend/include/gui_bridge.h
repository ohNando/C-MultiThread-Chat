#ifndef GUI_BRIDGE_H
#define GUI_BRIDGE_H

#include "common.h"
#include "utility.h"
#include "queue.h"

#include <libwebsockets.h>

#define PROTOCOL_NAME "tcp-chat-protocol"
#define WS_PORT 8081

extern MessageQueue queue_to_gui;
extern MessageQueue queue_from_gui;

void* start_websocket_server(void*);
int send_message_to_gui(const char*);
int callback_chat(struct lws*, enum lws_callback_reasons,
                         void*, void*, size_t);

static struct lws_protocols protocols[] = {
    {
        .name = PROTOCOL_NAME,
        .callback = callback_chat,
        .per_session_data_size = 0,
        .rx_buffer_size = 0,
    },
    { NULL, NULL, 0, 0 } // terminator
};

#endif
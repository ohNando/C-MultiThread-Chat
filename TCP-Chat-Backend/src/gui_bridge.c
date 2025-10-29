#include "gui_bridge.h"

MessageQueue queue_to_gui;
MessageQueue queue_from_gui;

static struct lws_context* websocket_context = NULL;
static struct lws* gui_wsi = NULL;
pthread_mutex_t websocket_client_mutex = PTHREAD_MUTEX_INITIALIZER;

int callback_chat(struct lws* wsi, enum lws_callback_reasons reason,
                         void* user, void* in, size_t len) {
    char incoming_messsage[MAX_BUFSIZE];

    switch (reason){
        case LWS_CALLBACK_ESTABLISHED:
            init_queue(&queue_to_gui);
            init_queue(&queue_from_gui);

            LOCK(&websocket_client_mutex);
            gui_wsi = wsi;
            UNLOCK(&websocket_client_mutex);
            printf("(+)| WebSocket connection established. WSI: %p\n", (void*)gui_wsi);
            break;
        case LWS_CALLBACK_RECEIVE:
            if(len >= MAX_BUFSIZE) break;
            strncpy(incoming_messsage, (const char*)in, len);
            incoming_messsage[len] = '\0';

            if(enqueue(&queue_from_gui, incoming_messsage) == 0){
                printf("(WS)| Received message from GUI: %s\n", incoming_messsage);  
            }
            break;
        case LWS_CALLBACK_CLOSED:
            LOCK(&websocket_client_mutex);
            if(gui_wsi == wsi){
                gui_wsi = NULL;
                printf("(WS)| WebSocket connection closed. WSI: %p\n", (void*)wsi);
            }
            UNLOCK(&websocket_client_mutex);
            break;
        case LWS_CALLBACK_SERVER_WRITEABLE: //
            if(gui_wsi){
                char message_to_send[MAX_BUFSIZE];

                if(dequeue_nonblocking(&queue_to_gui, message_to_send, MAX_BUFSIZE) == 0){
                    size_t len = strlen(message_to_send);
                    unsigned char* buf = (unsigned char*)malloc(LWS_PRE + len + 1);
                    if(!buf) break;

                    memcpy(buf + LWS_PRE, message_to_send, len);
                    buf[LWS_PRE + len] = '\0';

                    lws_write(wsi, buf + LWS_PRE, len, LWS_WRITE_TEXT);
                    free(buf);

                    if(queue_to_gui.count > 0){
                        lws_callback_on_writable(wsi);
                    }
                }
            }
            break;
    }

    return 0;
}

int send_message_to_gui(const char* message){
    int result = enqueue(&queue_to_gui, message);

    if(result == 0){
        LOCK(&websocket_client_mutex);
        if(gui_wsi){
            lws_callback_on_writable(gui_wsi);
        }
        UNLOCK(&websocket_client_mutex);
    }
    
    return result;
}

void* start_websocket_server(void* arg){
    struct lws_context_creation_info info ={
        .port = WS_PORT,
        .iface = NULL,
        .protocols = protocols,
        .options = LWS_SERVER_OPTION_VALIDATE_UTF8 || LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT,
        .gid = -1,
        .uid = -1,
        .max_http_header_data = 1024
    };

    websocket_context = lws_create_context(&info);

    if(!websocket_context){
        fprintf(stderr, "(E)| Failed to create Websocket context!\n");
        return NULL;
    }

    printf("(I)| WebSocket server started on port %d\n", WS_PORT);

    int n = 0;
    while(n >= 0){
        n = lws_service(websocket_context, 100); //100 ms timeout
    }

    lws_context_destroy(websocket_context);
    printf("(W)| WebSocket server terminated.\n");

    return NULL;
}
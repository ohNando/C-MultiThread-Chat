#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"
#include "utility.h"
#include "network.h"

void* receiveMsgHandler(void*);
void* sendMsgHandler(void*);

int initClient(const char*,int);
void startClientLoop(int);

#endif
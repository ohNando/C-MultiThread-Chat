# ----- Binary Names -----
SERVER_BIN = ./bin/server
CLIENT_BIN = ./bin/client

# ----- Object Files -----
SERVER_OBJ = ./lib/server.o
CLIENT_OBJ = ./lib/client.o
UTILITY_OBJ = ./lib/utility.o
NETWORK_OBJ = ./lib/network.o

COMMON_OBJ = $(UTILITY_OBJ) $(NETWORK_OBJ)
ALL_OBJ = $(SERVER_OBJ) $(CLIENT_OBJ) $(COMMON_OBJ)

# ----- Include Files -----
INC_PATHS = -I./include/

# ---- Libs -----
LIBS = -lpthread

# ----- Default Port and IP -----
PORT = 8080
SERVER_IP = 127.0.0.1

all: build

$(SERVER_OBJ): ./src/server.c
	gcc -g $(INC_PATHS) -o $(SERVER_OBJ) -c ./src/server.c

$(CLIENT_OBJ): ./src/client.c
	gcc -g $(INC_PATHS) -o $(CLIENT_OBJ) -c ./src/client.c

$(UTILITY_OBJ): ./src/utility.c
	gcc -g $(INC_PATHS) -o $(UTILITY_OBJ) -c ./src/utility.c

$(NETWORK_OBJ): ./src/network.c
	gcc -g $(INC_PATHS) -o $(NETWORK_OBJ) -c ./src/network.c

build: build-server build-client

build-server: $(SERVER_OBJ) $(COMMON_OBJ)
	gcc -o $(SERVER_BIN) $(SERVER_OBJ) $(COMMON_OBJ) $(LIBS)

build-client: $(CLIENT_OBJ)
	gcc -o $(CLIENT_BIN) $(CLIENT_OBJ) $(COMMON_OBJ) $(LIBS)

run-server: build-server
	$(SERVER_BIN) $(PORT)

run-client: build-client 
	$(CLIENT_BIN) $(SERVER_IP) $(PORT)  

clean:
	rm -f $(ALL_OBJ) $(SERVER_BIN) $(CLIENT_BIN) 

# ----- Binary Names -----
SERVER_BIN = ./bin/Server
CLIENT_BIN = ./bin/Client

# ----- Object Files -----
SERVER_OBJ = ./lib/server/Server.o 
CLIENT_OBJ = ./lib/client/Client.o 

# ----- Include Files -----
SERVER_INC = -I ./include/server/
CLIENT_INC = -I ./include/client/

# ---- Libs -----
LIBS = -lpthread

# ----- Default Port and IP -----
PORT = 8080
SERVER_IP = 127.0.0.1

all: build

$(SERVER_OBJ): ./src/server/Server.c
	gcc -g $(SERVER_INC) -o $(SERVER_OBJ) -c ./src/server/Server.c 

$(CLIENT_OBJ): ./src/client/Client.c 
	gcc -g $(CLIENT_INC) -o $(CLIENT_OBJ) -c ./src/client/Client.c 

build: build-server build-client

build-server: $(SERVER_OBJ)
	gcc -o $(SERVER_BIN) $(SERVER_OBJ) $(LIBS)

build-client: $(CLIENT_OBJ)
	gcc -o $(CLIENT_BIN) $(CLIENT_OBJ) $(LIBS)

run-server: build-server 
	$(SERVER_BIN) $(PORT)

run-client: build-client 
	$(CLIENT_BIN) $(SERVER_IP) $(PORT)  

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER_BIN) $(CLIENT_BIN)



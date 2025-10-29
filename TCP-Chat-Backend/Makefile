# ----- Binary Names -----
SERVER_BIN = ./bin/server
CLIENT_BIN = ./bin/client

# ----- Object Files -----
SERVER_OBJ = ./lib/server.o
CLIENT_OBJ = ./lib/client.o
UTILITY_OBJ = ./lib/utility.o
NETWORK_OBJ = ./lib/network.o
GUI_BRIDGE_OBJ = ./lib/gui_bridge.o
QUEUE_OBJ = ./lib/queue.o

# GEREKLİ YENİ TANIMLAMALAR
# 1. Sunucunun İhtiyaç Duyduğu Tüm Ortak Objeler
SERVER_COMMON_OBJ = $(UTILITY_OBJ) $(NETWORK_OBJ) $(GUI_BRIDGE_OBJ) $(QUEUE_OBJ)
# 2. İstemcinin İhtiyaç Duyduğu Temel Objeler (GUI Bridge hariç)
CLIENT_COMMON_OBJ = $(UTILITY_OBJ) $(NETWORK_OBJ)
ALL_OBJ = $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER_COMMON_OBJ)

# ----- Include Files -----
INC_PATHS = -I./include/

# ---- Libs -----
LIBS = -lpthread

# ----- Default Port and IP -----
PORT = 8080
SERVER_IP = 127.0.0.1

all: build

# ----------------- NESNE DERLEME KURALLARI (Aynı Kalır) -----------------
$(SERVER_OBJ): ./src/server.c
	gcc -g $(INC_PATHS) -o $(SERVER_OBJ) -c ./src/server.c

$(CLIENT_OBJ): ./src/client.c
	gcc -g $(INC_PATHS) -o $(CLIENT_OBJ) -c ./src/client.c

$(UTILITY_OBJ): ./src/utility.c
	gcc -g $(INC_PATHS) -o $(UTILITY_OBJ) -c ./src/utility.c

$(NETWORK_OBJ): ./src/network.c
	gcc -g $(INC_PATHS) -o $(NETWORK_OBJ) -c ./src/network.c

$(GUI_BRIDGE_OBJ): ./src/gui_bridge.c
	gcc -g $(INC_PATHS) -o $(GUI_BRIDGE_OBJ) -c ./src/gui_bridge.c

$(QUEUE_OBJ): ./src/queue.c
	gcc -g $(INC_PATHS) -o $(QUEUE_OBJ) -c ./src/queue.c

# ----------------- BUILD HEDEFLERI (Düzeltildi) -----------------

build: build-server build-client

build-server: $(SERVER_OBJ) $(SERVER_COMMON_OBJ)
    # LWS ve GUI Bridge'i linkler
	gcc -o $(SERVER_BIN) $(SERVER_OBJ) $(SERVER_COMMON_OBJ) $(LIBS) -lwebsockets

build-client: $(CLIENT_OBJ) $(CLIENT_COMMON_OBJ)
    # Sadece TCP için gerekli olan Utility ve Network'ü linkler.
    # GUI_BRIDGE ve QUEUE objeleri KALDIRILDI.
	gcc -o $(CLIENT_BIN) $(CLIENT_OBJ) $(CLIENT_COMMON_OBJ) $(LIBS)

run-server: build-server
	$(SERVER_BIN) $(PORT)

run-client: build-client
	$(CLIENT_BIN) $(SERVER_IP) $(PORT)

clean:
	rm -f $(ALL_OBJ) $(SERVER_BIN) $(CLIENT_BIN)
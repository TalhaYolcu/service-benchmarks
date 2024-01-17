# Makefile for asio_tcp_client.cpp and asio_tcp_server.cpp

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall

# Asio library path
ASIO_INCLUDE := -I/usr/local/include

# Source files
CLIENT_SRC := tcp_client/src/asio_tcp_client.cpp
SERVER_SRC := tcp_server/src/asio_tcp_server.cpp

# Output directories
BUILD_DIR := build
CLIENT_BUILD_DIR := $(BUILD_DIR)/tcp_client
SERVER_BUILD_DIR := $(BUILD_DIR)/tcp_server

# Output executables
CLIENT_OUT := $(CLIENT_BUILD_DIR)/asio_tcp_client
SERVER_OUT := $(SERVER_BUILD_DIR)/asio_tcp_server

# Targets
all: $(CLIENT_OUT) $(SERVER_OUT)

$(CLIENT_OUT): $(CLIENT_SRC)
	mkdir -p $(CLIENT_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(ASIO_INCLUDE) -o $@ $<

$(SERVER_OUT): $(SERVER_SRC)
	mkdir -p $(SERVER_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(ASIO_INCLUDE) -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

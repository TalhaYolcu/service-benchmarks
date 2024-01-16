#!/bin/bash

# Set the paths to your server and client directories
server_dir="build/server/"
client_dir="client/"

# Set the names oşf your server and client executables
server_executable="rpc_server"
client_executable="rpc_client"

# Run the server
echo "Starting server..."
cd "$server_dir" || exit 1
./"$server_executable" -c 1000000 -s 4096 &
server_pid=$!
cd ..

# Wait for a short duration to ensure the server has started
sleep 2

# Run the client
echo "Starting client..."
cd "$client_dir" || exit 1
./"$client_executable" -c 1000000 -s 4096
cd ..

echo "Server and client started successfully."

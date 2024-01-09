#include <iostream>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {

    //  Prepare our context and socket
    zmq::context_t context (2);
    zmq::socket_t socket (context, zmq::socket_type::rep);
    socket.bind ("tcp://*:5556");

    zmq::message_t request;

    std::cout<<"Waiting for a client connection..."<<std::endl;
    //  Wait for next request from client
    socket.recv (request, zmq::recv_flags::none);

    // Cast a dynamically allocated string to 'void*'.
    void *vp = request.data();

    std::string keyStr = std::string((char*)vp,request.size());

    std::cout<<keyStr<<std::endl;

    if(keyStr=="KEYGEN") {
        std::cout << "Received KEYGEN request" << std::endl;

        key_t key = ftok("shm", 65); // Generate a unique key for the shared memory segment

        int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT); // Create a shared memory segment
        char* shared_memory = (char*)shmat(shmid, (void*)0, 0); // Attach the shared memory segment

        //Send reply back to client
        zmq::message_t reply (2);

        std::cout<<"Generated key : "<<keyStr<<std::endl;

        memcpy (reply.data (), "OK",2);
        socket.send (reply, zmq::send_flags::none);  

        std::cout << "Server: Waiting for client message..." << std::endl;
        while (true) {
            while (strcmp(shared_memory, "Hello from client") != 0) {
                // Wait for client's message
                usleep(1000);
            }
            std::cout << "Server: Received message from client: " << shared_memory << std::endl;

            // Respond to the client
            strcpy(shared_memory, "world from server");

            std::cout << "Server: Sent message to client " << std::endl;
            break;
        }

        zmq::message_t endReq;
        socket.recv(endReq,zmq::recv_flags::none);
        if(endReq.data()=="endReq") {
            shmdt(shared_memory); // Detach the shared memory segment
            shmctl(shmid, IPC_RMID, NULL); // Remove the shared memory segment
        }
    }
    else {
        std::cout<<"Because of there is no KEYGEN request, returning back...";
    }
    return 0;
}

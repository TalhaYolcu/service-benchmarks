#include <iostream>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <zmq.hpp>
#include <string>
#include <ctime>
#include <sys/time.h>

#define SHM_SIZE 1024

int main() {

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, zmq::socket_type::req);

    std::cout << "Connecting to server..." << std::endl;
    socket.connect ("tcp://localhost:5556");

    zmq::message_t request (6);
    memcpy (request.data (), "KEYGEN", 6);
    
    
    // Get start time
    struct timeval start, end;
    gettimeofday(&start, NULL);


    socket.send (request, zmq::send_flags::none);

    //  Get the reply.
    zmq::message_t reply;
    socket.recv (reply, zmq::recv_flags::none);
 
    // Cast a dynamically allocated string to 'void*'.
    void *vp = request.data();

    std::string answerStr = std::string((char*)vp,request.size());

    std::cout << "Received answer : " << answerStr << std::endl;

    key_t key = ftok("shm", 65); // Generate a unique key for the shared memory segment

    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT); // Create a shared memory segment
    char* shared_memory = (char*)shmat(shmid, (void*)0, 0); // Attach the shared memory segment

    std::cout << "Client: Sending message to server..." << std::endl;
    strcpy(shared_memory, "Hello from client");

    while (strcmp(shared_memory, "world from server") != 0) {
        // Wait for server's response
        usleep(1000);
    }

    std::cout << "Client: Received message from server: " << shared_memory << std::endl;

    zmq::message_t endReq(6);
    memcpy (endReq.data (), "endReq", 6);
    socket.send (request, zmq::send_flags::none);

    // Get end time
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    std::cout << "Time taken by client: " << elapsed << " seconds" << std::endl;


    shmdt(shared_memory); // Detach the shared memory segment
    shmctl(shmid, IPC_RMID, NULL); // Remove the shared memory segment

    return 0;
}

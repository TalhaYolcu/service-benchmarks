#include <zmq.h>
#include <stdio.h>

int main (int argc, char *argv [])
{
    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, "tcp://localhost:5556");
    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

    char message[12];

    while(1)
    {
        rc = zmq_recv(subscriber, message, 12, 0);
        printf("%s\n", message);
    }

    zmq_close(subscriber);
    zmq_ctx_destroy(context);
    return 0;
}
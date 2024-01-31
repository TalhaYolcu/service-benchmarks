#include <zmq.h>
#include <stdio.h>
#include <czmq.h>
int main (int argc, char *argv [])
{
    zsock_t *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind(publisher, "tcp://127.0.0.1:5556");

    //  Ensure subscriber connection has time to complete
    sleep (1);

    while(1)
    {
        rc = zmq_send(publisher, "Hello World!", 12, 0);
    }

    zmq_close(publisher);
    zmq_ctx_destroy(context);
    return 0;
}
#include <myproto/response.pb.h>
#include <myproto/query.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <cstring>
#include <unistd.h>


using grpc::Channel;
using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using expcmake::GetHello;
using expcmake::query;
using expcmake::Response;

int main(int argc, char const *argv[])
{
    std::cout<<"This is rpc client"<<std::endl;
    int iterations= 0;
    int message_size =  0;
    // Check if the correct number of arguments is provided
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " -c <value> -s <value>" << std::endl;
        return 1;
    }

    // Parse command-line arguments
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-c") == 0) {
            // Handle the -c option (number of iterations)
            iterations = std::stoi(argv[i + 1]);
            std::cout << "Number of iterations: " << iterations << std::endl;
        } else if (strcmp(argv[i], "-s") == 0) {
            // Handle the -s option (size)
            message_size = std::stoi(argv[i + 1]);
            std::cout << "Size: " << message_size << std::endl;
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            return 1;
        }
    }

    // Get start time
    struct timeval start, end;
    gettimeofday(&start, NULL);

    CompletionQueue cq;
    Status status;
    Response myResponse;
    query MyQuery;
    MyQuery.set_name("Noyan");
    auto channel = grpc::CreateChannel("localhost:50052", grpc::InsecureChannelCredentials());
    std::unique_ptr<GetHello::Stub> stub_(GetHello::NewStub(channel));



    for(int i =0;i<iterations;i++) {
        
        ClientContext context;
        std::unique_ptr<ClientAsyncResponseReader<Response >> rpc(
        stub_->AsyncGetHelloWorld(&context, MyQuery, &cq));

        // Request that, upon completion of the RPC, "reply" be updated with the
        // server's response; "status" with the indication of whether the operation
        // was successful. Tag the request with the integer 1.
        rpc->Finish(&myResponse, &status, (void*)1);
        void* got_tag;
        bool ok = false;

        // Block until the next result is available in the completion queue "cq".
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or the cq_ is shutting down.
        cq.Next(&got_tag, &ok);

        // Act upon the status of the actual RPC.
        /*if (status.ok()) {
            std::cout<<myResponse.message() <<")"<<i<<std::endl;
        }
        else {
            std::cout<<"RPC failed :  "<<status.error_code()<<std::endl;
        }*/
    }

 




/*
    // Call
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    std::unique_ptr<expcmake::GetHello::Stub> stub = expcmake::GetHello::NewStub(channel);
    
    // Setup request
    expcmake::query query;
    expcmake::Response result;

/*
    for(int i =0;i<iterations;i++) {
        query.set_name("World");
        grpc::ClientContext context;

        stub->GetHelloWorld(&context, query, &result);
        // Output result
        //std::cout <<i+1<< ")Message : " << result.message() << std::endl;
    }
*/
    // Get end time
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    std::cout << "Time taken by client: " << elapsed << " seconds" << std::endl;

 






    std::cout<<"rpc_client stops..."<<std::endl;
    return 0;
}

#include <myproto/response.pb.h>
#include <myproto/query.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <iostream>
#include <sys/time.h>
#include <csignal>
#include <cstdlib>

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using expcmake::GetHello;
using expcmake::query;
using expcmake::Response;

int message_size = 0;
int iterations = 0;

/*
class GetHelloService final : public expcmake::GetHello::Service {
    public:
        virtual ::grpc::Status GetHelloWorld(::grpc::ServerContext* context, const ::expcmake::query* request, ::expcmake::Response* response)
        {        
            //std::string generated = generateHelloWorldString(message_size);
            //std::cout<<"Generated : "<<generated<<std::endl;
            response->set_message(generateHelloWorldString(message_size));
            return grpc::Status::OK;
        }
        std::string generateHelloWorldString(int length) {
            const std::string helloWorld = "HelloWorld";
            std::string result;

            if (length <= 0) {
                std::cerr << "Error: Length should be greater than zero." << std::endl;
                return result;
            }

            while (result.length() < length) {
                result += helloWorld;
            }

            // Trim the string to the required length
            result.resize(length);

            return result;
        }
};
*/
class ServerImpl final {
 public:
  ~ServerImpl() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
  }

  // There is no shutdown handling in this code.
  void Run() {
    
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:50052", grpc::InsecureServerCredentials());

    builder.RegisterService(&service_);

    //std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    //server->Wait();

    //std::cout<<"rpc_server stops..."<<std::endl;
    
    cq_ = builder.AddCompletionQueue();
    // Finally assemble the server.
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << "0.0.0.0:50052" << std::endl;

    // Proceed to the server's main loop.
    HandleRpcs();
  }

 private:
  // Class encompasing the state and logic needed to serve a request.
  class CallData {
   public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    CallData(GetHello::AsyncService* service, ServerCompletionQueue* cq)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
      // Invoke the serving logic right away.
      Proceed();
    }
    int i=0;

    std::string generateHelloWorldString(int length) {
        const std::string helloWorld = "HelloWorld";
        std::string result;

        if(i==5) {
            sleep(2);
            std::cout<<"AAAA"<<std::endl;
        }
        i++;

        if (length <= 0) {
            std::cerr << "Error: Length should be greater than zero." << std::endl;
            return result;
        }

        while (result.length() < length) {
            result += helloWorld;
        }

        // Trim the string to the required length
        result.resize(length);

        return result;
    }

    void Proceed() {
      if (status_ == CREATE) {
        // Make this instance progress to the PROCESS state.
        status_ = PROCESS;

        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different CallData
        // instances can serve different requests concurrently), in this case
        // the memory address of this CallData instance.
        service_->RequestGetHelloWorld(&ctx_, &request_, &responder_, cq_, cq_,
                                  this);
      } else if (status_ == PROCESS) {
        // Spawn a new CallData instance to serve new clients while we process
        // the one for this CallData. The instance will deallocate itself as
        // part of its FINISH state.
        new CallData(service_, cq_);



        // The actual processing.
        std::string prefix=generateHelloWorldString(message_size);
        reply_.set_message(prefix);

        // And we are done! Let the gRPC runtime know we've finished, using the
        // memory address of this instance as the uniquely identifying tag for
        // the event.
        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this);
      } else {
        GPR_ASSERT(status_ == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
      }
    }

   private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    GetHello::AsyncService* service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* cq_;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

    // What we get from the client.
    query request_;
    // What we send back to the client.
    Response reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<Response> responder_;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.
  };

  // This can be run in multiple threads if needed.
  void HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service_, cq_.get());
    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
      GPR_ASSERT(cq_->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<CallData*>(tag)->Proceed();
    }
  }

  std::unique_ptr<ServerCompletionQueue> cq_;
  GetHello::AsyncService service_;
  std::unique_ptr<Server> server_;
};
int main(int argc, char const *argv[])
{
    std::cout<<"This is rpc_server"<<std::endl;

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
/*
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

    GetHelloService my_service;
    builder.RegisterService(&my_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    server->Wait();

    std::cout<<"rpc_server stops..."<<std::endl;
*/

  ServerImpl server;
  server.Run();
    return 0;
}



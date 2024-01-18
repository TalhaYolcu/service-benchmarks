#include <myproto/response.pb.h>
#include <myproto/query.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <iostream>
#include <sys/time.h>
#include <csignal>
#include <cstdlib>


int message_size = 0;
int iterations = 0;
bool exit_signal = false;

// Function to handle Ctrl+C signal
void signalHandler(int signum) {
    std::cout << "Interrupt signal received (" << signum << "). Exiting...\n";

    if(signum==SIGINT) {
        exit_signal = true;
    }

}

class GetHelloService final : public expcmake::GetHello::Service {
    public:
        virtual ::grpc::Status GetHelloWorld(::grpc::ServerContext* context, const ::expcmake::query* request, ::expcmake::Response* response)
        {        
            std::string generated = generateHelloWorldString(message_size);
            //std::cout<<"Generated : "<<generated<<std::endl;
            response->set_message(generated);
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

int main(int argc, char const *argv[])
{
    std::cout<<"This is rpc_server"<<std::endl;

    // Register signal handler for Ctrl+C
    std::signal(SIGINT, signalHandler);

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

    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

    GetHelloService my_service;
    builder.RegisterService(&my_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    while(!exit_signal) {
        server->Wait();
    }

    std::cout<<"rpc_server stops..."<<std::endl;

    return 0;
}



#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <iostream>
#include <sys/time.h>
#include <csignal>
#include <cstdlib>


#define NUMBER_OF_PROPERTY = 5
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

class AddressBookService final : public expcmake::AddressBook::Service {
    public:
        virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::expcmake::NameQuerry* request, ::expcmake::Address* response)
        {
            //std::cout << "Server: GetAddress for \"" << request->name() << "\"." << std::endl;
            response->set_name(request->name());
            
            std::string randomStr1 = generateRandomString(message_size/5);
            std::string randomStr2 = generateRandomString(message_size/5);
            std::string randomStr3 = generateRandomString(message_size/5);
            std::string randomStr4 = generateRandomString(message_size/5); 
            response->set_city(randomStr1.c_str());
            response->set_zip(randomStr2.c_str());
            response->set_street(randomStr3.c_str());
            response->set_country(randomStr4.c_str());
            return grpc::Status::OK;
        }
        std::string generateRandomString(int length) {
            // Define the characters to be used in the random string
            const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

            // Initialize an empty string to store the result
            std::string randomString;

            // Generate the random string
            for (int i = 0; i < length; ++i) {
                // Generate a random index within the range of the characters string
                int randomIndex = std::rand() % characters.size();

                // Append the randomly chosen character to the result string
                randomString.push_back(characters[randomIndex]);
            }

            // Return the generated random string
            return randomString;
        }
};

int main(int argc, char const *argv[])
{
    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
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

    AddressBookService my_service;
    builder.RegisterService(&my_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    while(!exit_signal) {
        server->Wait();
    }

    std::cout<<"rpc_server stops..."<<std::endl;

    return 0;
}



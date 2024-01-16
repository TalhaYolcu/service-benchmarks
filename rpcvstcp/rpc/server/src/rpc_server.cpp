#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <iostream>
#include <sys/time.h>

#define NUMBER_OF_PROPERTY = 5


int message_size = 0;
int iterations = 0;

class AddressBookService final : public expcmake::AddressBook::Service {
    public:
        virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::expcmake::NameQuerry* request, ::expcmake::Address* response)
        {
            std::cout << "Server: GetAddress for \"" << request->name() << "\"." << std::endl;
            response->set_name(request->name());
            
            const char* randomStr1 = generateRandomString(message_size/5);
            const char* randomStr2 = generateRandomString(message_size/5);
            const char* randomStr3 = generateRandomString(message_size/5);
            const char* randomStr4 = generateRandomString(message_size/5);

            
            response->set_city(randomStr1);
            response->set_zip(randomStr2);
            response->set_street(randomStr3);
            response->set_country(randomStr4);

            delete[] randomStr1;
            delete[] randomStr2;
            delete[] randomStr3;
            delete[] randomStr4;                        


            return grpc::Status::OK;
        }
        const char* generateRandomString(int length) {
            static const char charset[] =
                "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

            char* randomString = new char[length + 1]; // +1 for null terminator
            int charsetSize = sizeof(charset) - 1;

            // Seed for random number generation
            std::srand(static_cast<unsigned int>(std::time(nullptr)));

            for (int i = 0; i < length; ++i) {
                randomString[i] = charset[std::rand() % charsetSize];
            }

            randomString[length] = '\0'; // Null-terminate the string

            return randomString;
        }
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

    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

    AddressBookService my_service;
    builder.RegisterService(&my_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();

    std::cout<<"rpc_server stops..."<<std::endl;

    return 0;
}



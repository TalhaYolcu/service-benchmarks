#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <cstring>


#define NUMBER_OF_PROPERTY = 5


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

    // Call
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    std::unique_ptr<expcmake::AddressBook::Stub> stub = expcmake::AddressBook::NewStub(channel);
    grpc::ClientContext context;

    for(int i =0;i<iterations;i++) {
        // Setup request
        expcmake::NameQuerry query;
        expcmake::Address result;


        const char* randomStr = generateRandomString(message_size/5);

        query.set_name(randomStr);

        // Remember to delete the allocated memory
        delete[] randomStr;

        grpc::Status status = stub->GetAddress(&context, query, &result);

        // Output result
        std::cout << "I got:" << std::endl;
        std::cout << "Name: " << result.name() << std::endl;
        std::cout << "City: " << result.city() << std::endl;
        std::cout << "Zip:  " << result.zip() << std::endl;
        std::cout << "Street: " << result.street() << std::endl;
        std::cout << "Country: " << result.country() << std::endl;

    }

    // Get end time
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    std::cout << "Time taken by client: " << elapsed << " seconds" << std::endl;

 

    std::cout<<"rpc_client stops..."<<std::endl;
    return 0;
}
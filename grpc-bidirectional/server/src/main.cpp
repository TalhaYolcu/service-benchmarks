#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <iostream>
#include <thread>
#include <grpcpp/create_channel.h>

class AddressBookService final : public expcmake::AddressBook::Service {
    public:
        virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::expcmake::NameQuerry* request, ::expcmake::Address* response)
        {
            std::cout << "Server: GetAddress for \"" << request->name() << "\"." << std::endl;
            response->set_name(request->name());
            response->set_city("Gebze-FromServer");
            response->set_zip("12345");
            response->set_street("Huzur");
            response->set_country("TR");
            return grpc::Status::OK;
        }
};

void listenerThreadFunction() {
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

    AddressBookService my_service;
    builder.RegisterService(&my_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
}

int main(int argc, char* argv[])
{

    std::thread listenerThread(listenerThreadFunction);
  
    // Setup request
    expcmake::NameQuerry query;
    expcmake::Address result;
    query.set_name("John");

    sleep(2);

    // Get start time
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Call
    auto channel = grpc::CreateChannel("localhost:50052", grpc::InsecureChannelCredentials());
    std::unique_ptr<expcmake::AddressBook::Stub> stub = expcmake::AddressBook::NewStub(channel);
    grpc::ClientContext context;
    grpc::Status status = stub->GetAddress(&context, query, &result);


    // Get end time
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    std::cout << "Time taken by client: " << elapsed << " seconds" << std::endl;

    // Output result
    std::cout << "I got:" << std::endl;
    std::cout << "Name: " << result.name() << std::endl;
    std::cout << "City: " << result.city() << std::endl;
    std::cout << "Zip:  " << result.zip() << std::endl;
    std::cout << "Street: " << result.street() << std::endl;
    std::cout << "Country: " << result.country() << std::endl;

    listenerThread.join();
    
    return 0;
}

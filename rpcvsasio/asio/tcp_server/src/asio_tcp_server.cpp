#define ASIO_SEPARATE_COMPILATION

#include <iostream>
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <asio/impl/src.hpp> 

using asio::ip::tcp;


std::string generateHelloWorld(int iterationCount) {
    const std::string helloWorld = "HelloWorld";
    std::string result;

    if (iterationCount <= 0) {
        std::cerr << "Error: Length should be greater than zero." << std::endl;
        return result;
    }

    while (result.length() < iterationCount) {
        result += helloWorld;
    }

    // Trim the string to the required length
    result.resize(iterationCount);

    return result;
}
int main(int argc, char const *argv[])
{
    std::cout<<"This is asio_server"<<std::endl;
    int iterations= 0;
    int message_size =  0;
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

    try
    {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 25002));
        for (;;)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout<<"One connection reached"<<std::endl;
            
            for(int i =0;i<iterations;i++) {
                std::array<char, 1024> data;
                std::size_t length = socket.read_some(asio::buffer(data));

                std::string request(data.data(), length);
                // Handle the request with a lambda function
                

                std::string response = generateHelloWorld(message_size);

                auto handle_request=[&]() {
                    asio::write(socket, asio::buffer(response));
                    
                };
                handle_request();
            }
            

        }     
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;

}
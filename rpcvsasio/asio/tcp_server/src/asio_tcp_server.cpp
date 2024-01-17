#define ASIO_SEPARATE_COMPILATION

#include <iostream>
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <asio/impl/src.hpp> 

using asio::ip::tcp;


int main(int argc, char const *argv[])
{
    std::cout<<"This is asio_server"<<std::endl;

    try
    {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 25002));
        for (;;)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout<<"One connection reached"<<std::endl;
            std::string message= "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam ut erat vel nislorem ipsum dolor sit amet, consectetur adipisci";

            asio::error_code ignored_error;
            asio::write(socket, asio::buffer(message), ignored_error);
        }     
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;

}
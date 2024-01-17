
#define ASIO_SEPARATE_COMPILATION
#include <iostream>
#include <asio.hpp>
#include <asio/impl/src.hpp> 
#include <sys/time.h>
#include <cstring>
#include <ctime>
#include <array>
#include <boost/array.hpp>
using asio::ip::tcp;

#define NUMBER_OF_PROPERTY = 5

int main(int argc, char* argv[])
{
  try
  {
    std::cout<<"This is rpc client"<<std::endl;
    // Seed the random number generator with the current time
    asio::io_context io_context;

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

    
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "25002");
    tcp::socket socket(io_context);
    asio::connect(socket, endpoints);

    std::cout<<"Connected"<<std::endl;

    for (int i =0;i<iterations;i++)
    {
      boost::array<char, 128> buf;
      asio::error_code error;

      size_t len = socket.read_some(asio::buffer(buf), error);

      if (error == asio::error::eof) {
        std::cout<<"Error happened"<<std::endl;
        break; // Connection closed cleanly by peer.
      }

      else if (error) {
        std::cout<<"Error happened exception"<<std::endl;
        throw asio::system_error(error); // Some other error.

      }

      std::cout.write(buf.data(), len);
    }

        // Get end time
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    std::cout << "Time taken by client: " << elapsed << " seconds" << std::endl;


  }

  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

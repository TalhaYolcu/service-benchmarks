#include <zmq.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <iostream>
#include <csignal>

bool sigIntReceived = false;

void threadFunction() {
    while(true) {
        if (sigIntReceived) {
            // Perform cleanup or additional tasks if needed
            std::cout << "Thread received SIGINT and is exiting gracefully." << std::endl;
            break;
        }
    }
    std::cout<<"Exiting from thread...\n";
}

// Signal handler for SIGINT
void sigIntHandler(int signum) {
    sigIntReceived = true;
}

int main() {
    std::signal(SIGINT, sigIntHandler);
    std::thread myThread(threadFunction);
    // Prepare context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, zmq::socket_type::req);
    socket.connect ("tcp://localhost:5555");

    while (true) {
        // Send a request to the server
        zmq::message_t request(1);
        memcpy(request.data(), "0", 1);
        socket.send(request);

        // Receive the frame from the server
        zmq::message_t reply;
        socket.recv(&reply);

        // Convert the received data back to OpenCV Mat
        std::string frameData = std::string(static_cast<char*>(reply.data()), reply.size());
        std::vector<uchar> buffer(frameData.begin(), frameData.end());
        cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);

        // Display the received frame
        cv::imshow("Video Stream", frame);

        // Check for the 'ESC' key to exit the loop
        if (cv::waitKey(1) == 27 || sigIntReceived) {
            std::cout<<"Exiting from client main thread...\n";
            break;
        }
    }
    sigIntReceived=true;
    myThread.join();
    return 0;
}

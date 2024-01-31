#include <zmq.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <csignal>

bool sigIntReceived = false;

// Signal handler for SIGINT
void sigIntHandler(int signum) {
    sigIntReceived = true;
}

int main() {
    std::signal(SIGINT, sigIntHandler);
    // Prepare context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, zmq::socket_type::req);
    socket.connect ("ipc://localhost:5555");

    // Variables for measuring time
    auto start_time = std::chrono::high_resolution_clock::now();
    int frame_count = 0;

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

        // Calculate FPS and bit rate
        frame_count++;
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        if (elapsed_time >= 1000) {
            double fps = static_cast<double>(frame_count) / (elapsed_time / 1000.0);
            double bit_rate = static_cast<double>(frame_count * frame.cols * frame.rows * frame.channels() * 8) / (elapsed_time / 1000.0);

            std::cout << "FPS: " << fps << " | Bit Rate: " << bit_rate << " bps" << std::endl;

            // Reset variables for the next second
            frame_count = 0;
            start_time = std::chrono::high_resolution_clock::now();
        }

        // Check for the 'ESC' key to exit the loop
        if (cv::waitKey(1) == 27 || sigIntReceived) {
            std::cout<<"Exiting from client...\n";
            break;
        }
    }
    return 0;
}

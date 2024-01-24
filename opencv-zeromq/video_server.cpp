#include <zmq.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Prepare context and socket
    zmq::context_t context (2);
    zmq::socket_t socket (context, zmq::socket_type::rep);
    socket.bind ("tcp://*:5555");

    // Open the camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    while (true) {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Error: Failed to capture frame." << std::endl;
            break;
        }

        // Serialize the OpenCV Mat to a string
        std::vector<uchar> buffer;
        cv::imencode(".jpg", frame, buffer);
        std::string frameData(buffer.begin(), buffer.end());

        zmq::message_t request;

        //  Wait for next request from client
        socket.recv (request, zmq::recv_flags::none);
 

        // Send the serialized frame to the client
        zmq::message_t reply(frameData.size());
        memcpy(reply.data(), frameData.data(), frameData.size());
        socket.send(reply);
    }

    return 0;
}

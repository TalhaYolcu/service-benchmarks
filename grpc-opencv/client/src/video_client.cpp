#include <iostream>
#include <myproto/request.grpc.pb.h>
#include <myproto/frame.pb.h>
#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <csignal>

bool sigIntReceived = false;

// Signal handler for SIGINT
void sigIntHandler(int signum) {
    sigIntReceived = true;
}
int main(int argc, char const *argv[])
{
    std::signal(SIGINT, sigIntHandler);

    //std::cout<<"Hello from video_client\n";
    // Setup request
    expcmake::Request query;
    expcmake::Frame result;
    query.set_name("John");

    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    std::unique_ptr<expcmake::gRPCOpenCV::Stub> stub = expcmake::gRPCOpenCV::NewStub(channel);

    // Variables for measuring time
    auto start_time = std::chrono::high_resolution_clock::now();
    int frame_count = 0;

    while(true) {
        grpc::ClientContext context;
        grpc::Status status = stub->GetFrame(&context, query, &result);

        std::string frameData = result.name();

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
            std::cout<<"Exitiing from client...\n";
            break;
        }        
    }
    return 0;
}

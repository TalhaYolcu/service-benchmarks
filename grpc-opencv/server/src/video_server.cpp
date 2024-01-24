#include <iostream>
#include <myproto/request.grpc.pb.h>
#include <myproto/frame.pb.h>
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include <opencv2/opencv.hpp>


class gRPCOpenCVService final : public expcmake::gRPCOpenCV::Service {

    public:
        cv::VideoCapture cap; 
        cv::Mat frame;
        gRPCOpenCVService() {
            // Open the camera
            cap=cv::VideoCapture(0);
            if (!cap.isOpened()) {
                std::cerr << "Error: Could not open camera.\n";
            }            
        }
        virtual ::grpc::Status GetFrame(::grpc::ServerContext* context, const ::expcmake::Request* request, ::expcmake::Frame* response)
        {
            cap >> frame;

            if (frame.empty()) {
                std::cerr << "Error: Failed to capture frame.\n";
                return grpc::Status::OK;
            }

            // Serialize the OpenCV Mat to a string
            std::vector<uchar> buffer;
            cv::imencode(".jpg", frame, buffer);
            std::string frameData(buffer.begin(), buffer.end());

            //std::cout << "Server: GetAddress for \"" << request->name() << "\"." << std::endl;
            response->set_name(frameData);
            return grpc::Status::OK;
        }
};
int main(int argc, char const *argv[])
{
    std::cout<<"Hello from video_server\n";

    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

    gRPCOpenCVService my_service;
    builder.RegisterService(&my_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();

    return 0;
}

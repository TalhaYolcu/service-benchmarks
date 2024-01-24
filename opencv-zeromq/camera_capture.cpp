#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


//g++ -o camera_capture camera_capture.cpp `pkg-config opencv4 --cflags --libs`


int main() {
    // GStreamer pipeline string for capturing video from /dev/video0
    const std::string pipeline = "v4l2src device=/dev/video0 ! videoconvert ! appsink";

    // Open the GStreamer pipeline
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    
    // Check if the camera opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    // Create a window to display the camera feed
    cv::namedWindow("Camera Feed", cv::WINDOW_NORMAL);

    while (true) {
        cv::Mat frame;

        // Read a frame from the camera
        cap >> frame;

        // Check if the frame is empty
        if (frame.empty()) {
            std::cerr << "Error: Failed to capture frame." << std::endl;
            break;
        }

        // Display the frame
        cv::imshow("Camera Feed", frame);

        // Check for the 'ESC' key to exit the loop
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    // Release the VideoCapture object
    cap.release();

    // Destroy the OpenCV window
    cv::destroyAllWindows();

    return 0;
}

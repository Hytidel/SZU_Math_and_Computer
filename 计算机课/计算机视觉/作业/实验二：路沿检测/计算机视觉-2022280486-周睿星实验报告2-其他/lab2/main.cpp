#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <filesystem>
using namespace cv;
using namespace std;
const string videoFolderDir = "resource/origin_video";
struct VideoInfo {
    string videoPath;
    string frameFolderPath;
    string outPutVideoPath;
    int totalFrame;
    double fps;
    cv::Size frameSize;
};
vector <VideoInfo> videoInfo;
void initPath() {
    for (const auto & entry : filesystem::directory_iterator(videoFolderDir)) {
        VideoInfo info;
        info.videoPath = entry.path().string();
        string fileName = entry.path().filename().string();
        string frameDir =  "resource/videoFrames/" + fileName + "/";
        info.frameFolderPath = frameDir;
        info.outPutVideoPath = "output/output"  + fileName;
        VideoCapture cap(info.videoPath);
        info.fps = cap.get(CAP_PROP_FPS);
        info.frameSize = cv::Size((int) cap.get(CAP_PROP_FRAME_WIDTH), (int) cap.get(CAP_PROP_FRAME_HEIGHT));
        info.totalFrame = cap.get(CAP_PROP_FRAME_COUNT);
        videoInfo.push_back(info);
        cap.release();
    }
    if(filesystem::exists("output"))filesystem::remove_all("output");
    filesystem::create_directories("output");

};

void extractFrame() {
    for (int i = 0; i < videoInfo.size(); i++) {
        VideoCapture cap(videoInfo[i].videoPath);
        if (!cap.isOpened()) {
            cerr << "Error! Unable to open video file " << videoInfo[i].videoPath << endl;
            return;
        }
        int frameCnt = 0;
        int totalFrame = cap.get(CAP_PROP_FRAME_COUNT);
        string frameDir = videoInfo[i].frameFolderPath;
        if(!filesystem::exists(frameDir)){
            filesystem::create_directories(frameDir);
        }
        Mat frame;
        while (true) {
            cap >> frame;
            if (frame.empty()) {
                break;
            }
            frameCnt++;
            imwrite(frameDir + to_string(frameCnt) + ".png", frame);
        }
        cout<<"Total frames: "<<totalFrame<<endl;
        cout<<"Extracted frames: "<<frameCnt << " to " << frameDir << endl;
        cout << "Percent : [ " << (float)frameCnt / totalFrame * 100 << "% ]" << endl;
        cap.release();
    }
}
int main() {
    initPath();
    extractFrame();
    for (int i = 0; i < videoInfo.size(); i++) {
        if(!filesystem::is_directory(videoInfo[i].frameFolderPath)){
            cout<<"Not a directory : "<<videoInfo[i].frameFolderPath<<endl;
            continue;
        }
        Mat image, orgImage;
        cv::Mat Mask = imread("resource/mask/mask" + to_string(i+1) + ".png", cv::IMREAD_GRAYSCALE);
        cv::VideoWriter video(videoInfo[i].outPutVideoPath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), videoInfo[i].fps, videoInfo[i].frameSize);
        int startTime = clock();
        int frameProcessed = 0, totalFrame = videoInfo[i].totalFrame;
        for(int j = 1; j <= totalFrame; j++){
            string framePath = videoInfo[i].frameFolderPath + to_string(j) + ".png";
            image = imread(framePath);
            orgImage = image.clone();
            cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
            cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);
            int thresholdHough = 80, thresholdCanny = 80, threshold = 150;
            if(i != 2) {
                cv::threshold(image, image, 170, 255, cv::THRESH_BINARY);
                thresholdHough = 50;
                threshold = 50;
                thresholdCanny = 50;
            }
            cv::Canny(image, image, thresholdCanny, 150);
            std::vector<cv::Vec4i> lines;
            cv::HoughLinesP(image, lines, 1, CV_PI / 180, thresholdHough, threshold, 10);
            for (size_t i = 0; i < lines.size(); i++) {
                cv::Vec4i l = lines[i];
                if(Mask.at<uchar>(l[1], l[0]) == 0 || Mask.at<uchar>(l[3], l[2]) == 0){
                    continue;
                }
                cv::line(orgImage, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
            }
            video.write(orgImage);
            frameProcessed++;
            if(clock() - startTime > 1000){
                cout<<"Processing video "<<i+1<<" : "<<frameProcessed<<" / "<<totalFrame<<endl;
                startTime = clock();
            }
        }
        cout << frameProcessed << " / " << totalFrame << endl;
    }
    return 0;
}


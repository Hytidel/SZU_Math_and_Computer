//
// Created by pc on 2024/4/8.
//
#include <opencv2/opencv.hpp>

#ifndef CV_IMAGEPROCESS_H
#define CV_IMAGEPROCESS_H
namespace ImgProcess {
    // 获取直方图
    float* getHist(cv::Mat img);
    // 归一化直方图
    void normalizeHist(float* hist);
    // 获取累积分布函数
    float* getCDF(float* hist);
    // 直方图均衡化
    void equalizeHist(cv::Mat img, cv::Mat& equalized_img);
    // 伽马变换
    void powerTransform(cv::Mat img, cv::Mat& power_img, float gamma, float c = 1.0);
}
#endif //CV_IMAGEPROCESS_H

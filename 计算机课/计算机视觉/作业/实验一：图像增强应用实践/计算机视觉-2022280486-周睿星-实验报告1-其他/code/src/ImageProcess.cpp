//
// Created by pc on 2024/4/8.
//
#include "../include/ImageProcess.h"

namespace ImgProcess {
    // 获取直方图
    float* getHist(cv::Mat img) {
        // 像素值范围0-255
        int histSize = 256;
        float* hist = (float*)malloc(sizeof(float) * histSize);
        memset(hist, 0, sizeof(float) * histSize);
        // 遍历图像，统计每个像素值的个数
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                hist[img.at<uchar>(i, j)]++;
            }
        }
        return hist;
    }
    void normalizeHist(float* hist) {
        float sum = 0;
        for (int i = 0; i < 256; i++) {
            sum += hist[i];
        }
        for (int i = 0; i < 256; i++) {
            hist[i] /= sum;
        }
    }
    // 获取累积分布函数
    float* getCDF(float* hist) {
        float* cdf = (float*)malloc(sizeof(float) * 256);
        cdf[0] = hist[0];
        for (int i = 1; i < 256; i++) {
            // 累积分布函数求法：前一个像素的累积分布函数值+当前像素的概率
            cdf[i] = cdf[i - 1] + hist[i];
        }
        return cdf;
    }
    void equalizeHist(cv::Mat img, cv::Mat& equalized_img) {
        float* hist = getHist(img);
        normalizeHist(hist);
        float* cdf = getCDF(hist);
        equalized_img = img.clone();
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                // 像素值映射 原像素 --> cdf[原像素] * 255
                equalized_img.at<uchar>(i, j) = cdf[img.at<uchar>(i, j)] * 255;
            }
        }
        free(hist);
        free(cdf);
    }
    void powerTransform(cv::Mat img, cv::Mat& power_img, float gamma, float c) {
        power_img = img.clone();
        float L = 255;
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                // 像素值映射 原像素 --> c * (原像素 / L ) ^ gamma * L
                power_img.at<uchar>(i, j) = c * pow(img.at<uchar>(i, j) / L, gamma) * L;
            }
        }
    }
}
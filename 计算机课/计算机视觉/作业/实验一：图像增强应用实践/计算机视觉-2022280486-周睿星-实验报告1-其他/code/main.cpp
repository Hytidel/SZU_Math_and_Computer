/*
Created by: Pur1fy
Date: 2024/4/8
Author: 周睿星
学号：2022280486
Description: This is a simple example of using the ImageProcess class to perform power transformation and histogram equalization on an image.
*/
#include "include/ImageProcess.h"
using namespace cv;

int main() {
    Mat img = imread("../images/car_3.jpg", IMREAD_GRAYSCALE);
    Mat power_img;
    Mat equalized_img;
    ImgProcess::powerTransform(img, power_img, 0.5);
    ImgProcess::equalizeHist(img, equalized_img);
    imshow("Power Img", power_img);
    imshow("Origin Img", img);
    imshow("Equalized Img", equalized_img);
    waitKey(0);
    return 0;
}

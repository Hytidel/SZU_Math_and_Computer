#pragma once
#ifndef _FACEDETECTFMTCNN_H_
#define _FACEDETECTFMTCNN_H_

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc/types_c.h> //����cvPoint����
using namespace std;
using namespace cv;

typedef struct FaceBox {
	float xmin;
	float ymin;
	float xmax;
	float ymax;
	float score;
} FaceBox;
typedef struct FaceInfo {
	float bbox_reg[4];
	float landmark_reg[10];//currently no value
	float landmark[10];
	FaceBox bbox;
} FaceInfo;

class CFaceDetectFMTCNN
{
public:
	CFaceDetectFMTCNN(void);
	~CFaceDetectFMTCNN(void);

	//CFaceDetectFMTCNN(const string& proto_model_dir);
	vector<FaceInfo> Detect_mtcnn(const cv::Mat& img, const int min_size, const float* threshold, const float factor, const int stage);
	//protected:
	vector<FaceInfo> ProposalNet(const cv::Mat& img, int min_size, float threshold, float factor);
	vector<FaceInfo> NextStage(const cv::Mat& image, vector<FaceInfo> &pre_stage_res, int input_w, int input_h, int stage_num, const float threshold);
	void BBoxRegression(vector<FaceInfo>& bboxes);
	void BBoxPadSquare(vector<FaceInfo>& bboxes, int width, int height);
	void BBoxPad(vector<FaceInfo>& bboxes, int width, int height);
	void GenerateBBox(Mat* confidence, Mat* reg_box, float scale, float thresh);
	std::vector<FaceInfo> NMS(std::vector<FaceInfo>& bboxes, float thresh, char methodType);
	float IoU(float xmin, float ymin, float xmax, float ymax, float xmin_, float ymin_, float xmax_, float ymax_, bool is_iom = false);

	void DetectFace_FMTCNN(Mat image,vector<FaceInfo> & faceInfo);

	void DispFaceBoxLandMarks(Mat image, vector<FaceInfo> faceInfo);

	void FaceDetectDemo();


	//    std::shared_ptr<dnn::Net> PNet_;
	//    std::shared_ptr<dnn::Net> ONet_;
	//    std::shared_ptr<dnn::Net> RNet_;
public:
	dnn::Net PNet_;
	dnn::Net RNet_;
	dnn::Net ONet_;

	std::vector<FaceInfo> candidate_boxes_;
	std::vector<FaceInfo> total_boxes_;

	float factor;
	float threshold[3];
	int minSize;

private:
	const float pnet_stride = 2;
	const float pnet_cell_size = 12;
	const int pnet_max_detect_num = 5000;
	//mean & std
	const float mean_val = 127.5f;
	const float std_val = 0.0078125f;
	//minibatch size
	const int step_size = 128;
	const string proto_model_dir = "model_FMTCNN";

	

	//��෵��10������
	//��ͼ���еľֲ�����LocalArea����Adaboost��������ȫ������������������Ǿֲ������ϵ�λ��
	/*void FaceDetection(Mat image, Rect LocalArea, vector<Rect>&FaceVector, vector<Rect>&PreProcessFaceVector);
	void DrawOnImg(Mat &image, vector<Rect>Faces);
	void TestOnImgDemo();
	void TestOnVideoDemo();*/
};
#endif
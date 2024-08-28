

//float std_points[10] = { 89.3095, 72.9025, 169.3095, 72.9025, 127.8949, 127.0441, 96.8796, 184.8907, 159.1065, 184.7601 };
float std_points[10] = { 30.2946, 51.6963, 65.5318, 51.5014, 48.0252, 71.7366, 33.5493, 92.3655, 62.7299, 92.2041 };
Mat getTformMatrix(float* std_points, float* feat_points);
Mat cropFace(Mat src, FaceDetector &fd);


//Mat cropFace(IplImage *src, seeta::FaceDetection &detector, seeta::FaceAlignment &point_detector)
Mat cropFace(Mat src, FaceDetector &fd)
{
	vector<FaceDetector::BoundingBox> res = fd.Detect(src, FaceDetector::BGR, FaceDetector::ORIENT_UP, 20, 0.6, 0.7, 0.7);

	// ¼ÙÉèÊäÈëÍ¼Æ¬ÖĞÖ»ÓĞÒ»ÕÅÈËÁ³
	float facial_points[10];//×ø±êÒÀ´ÎÊÇ£º×óÑÛX£¬×óÑÛY, ÓÒÑÛX£¬ÓÒÑÛY, ±Ç¼âX£¬±Ç¼âY, ×ó×ì½ÇX, ×ó×ì½ÇY, ÓÒ×ì½ÇX, ÓÒ×ì½ÇY
	for (int i = 0; i < 5; i++) {
		facial_points[2 * i] = res[0].points_x[i];
		facial_points[2 * i + 1] = res[0].points_y[i];
	}
	Mat tform = getTformMatrix(std_points, facial_points);
	Mat dstImage(112, 96, CV_8UC3);
	warpAffine(src, dstImage, tform, dstImage.size(), 1, 0, Scalar(0));

	Mat subfactor = 127.5 * Mat(112, 96, CV_32FC3, Scalar(1, 1, 1));
	dstImage.convertTo(dstImage, CV_32FC3);
	dstImage = dstImage - subfactor;
	dstImage = dstImage / 128;
	return dstImage;
}


Mat getTformMatrix(float* std_points, float* feat_points) {
	int points_num_ = 5;
	double sum_x = 0, sum_y = 0;
	double sum_u = 0, sum_v = 0;
	double sum_xx_yy = 0;
	double sum_ux_vy = 0;
	double sum_vx__uy = 0;
	for (int c = 0; c < points_num_; ++c) {
		int x_off = c * 2;
		int y_off = x_off + 1;
		sum_x += std_points[c * 2];
		sum_y += std_points[c * 2 + 1];
		sum_u += feat_points[x_off];
		sum_v += feat_points[y_off];
		sum_xx_yy += std_points[c * 2] * std_points[c * 2] +
			std_points[c * 2 + 1] * std_points[c * 2 + 1];
		sum_ux_vy += std_points[c * 2] * feat_points[x_off] +
			std_points[c * 2 + 1] * feat_points[y_off];
		sum_vx__uy += feat_points[y_off] * std_points[c * 2] -
			feat_points[x_off] * std_points[c * 2 + 1];
	}
	double q = sum_u - sum_x * sum_ux_vy / sum_xx_yy
		+ sum_y * sum_vx__uy / sum_xx_yy;
	double p = sum_v - sum_y * sum_ux_vy / sum_xx_yy
		- sum_x * sum_vx__uy / sum_xx_yy;
	double r = points_num_ - (sum_x * sum_x + sum_y * sum_y) / sum_xx_yy;
	double a = (sum_ux_vy - sum_x * q / r - sum_y * p / r) / sum_xx_yy;
	double b = (sum_vx__uy + sum_y * q / r - sum_x * p / r) / sum_xx_yy;
	double c = q / r;
	double d = p / r;


	Mat Tinv = (cv::Mat_<float>(3, 3) << a, b, 0, -b, a, 0, c, d, 1);
	Mat T = Tinv.inv();
	Mat res = T.colRange(0, 2).clone();
	return res.t();

}



#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;



void imgminRect()//轮廓最小外接矩形
{
	Mat img = imread("D:/ImageTest/convex.png");//加载图片
	Mat dst;
	cv::cvtColor(img, img, COLOR_RGB2GRAY);//进行，灰度处理
										   //阈值分割
	threshold(img,                 //输入图像,原始数组 (单通道 , 8-bit of 32-bit 浮点数).
		dst,                          //输出图像,输出数组，必须与 src 的类型一致，或者为 8-bit.
		200,                           //分割值
		255,                         // 使用 CV_THRESH_BINARY 和 CV_THRESH_BINARY_INV 的最大值.
		THRESH_BINARY_INV); //阈值类型,opencv认为白色部分为被分割出来的部分
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dst, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point());//所有轮廓CV_CHAIN_APPROX_SIMPLE
	Mat drawing = Mat::zeros(img.size(), CV_8UC3);
	vector<Rect> boundRect(contours.size());
	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color((rand() & 255), (rand() & 255), (rand() & 255));//随机产生颜色 rand()产生随机数
		drawContours(drawing, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point());
		//绘制轮廓的最小外结矩形
		RotatedRect rect = minAreaRect(contours[i]);
		Point2f P[4];
		rect.points(P);
		for (int j = 0; j <= 3; j++)
		{
			line(drawing, P[j], P[(j + 1) % 4], color, 1);
		}
		boundRect[i] = boundingRect(Mat(contours[i]));//由轮廓（点集）确定出正外接矩形
													  //获得正外接矩形的左上角坐标及宽高
		int width = boundRect[i].width;
		int  height = boundRect[i].height;
		int x = boundRect[i].x;
		int y = boundRect[i].y;
		//用画矩形方法绘制正外接矩形
		rectangle(drawing, Rect(x, y, width, height), Scalar(0, 0, 255), 1, 8);
	}
	imshow("srcImg", img);
	imshow("rect", drawing);
	waitKey(0);
}

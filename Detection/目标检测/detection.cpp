
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include "predictor.hpp"
#include <algorithm>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{
	//打开摄像头
	char c = 0;
	VideoCapture capture(0);
	while (1)

	{   
		int n=1;
		Mat frame;
		capture >> frame;
		



		//【1】Mat变量定义   
		Mat midImage;//目标图的定义

					 //【2】转为灰度图并进行图像平滑
		cvtColor(frame, midImage, CV_BGR2GRAY);//转化边缘检测后的图为灰度图
		GaussianBlur(midImage, midImage, Size(9, 9), 2, 2);

		//【3】进行霍夫圆变换
		vector<Vec3f> circles;
		HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, 1, 10, 200, 75, 0, 0);

		
		for (size_t i = 0; i < circles.size(); i++)
		{
			
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			int rx = cvRound(circles[i][0]);
			int ry = cvRound(circles[i][1]);
			Point point;
			point.x = sqrt(radius * radius) / 2;
			point.y = sqrt(radius * radius) / 2;
			Point pt1 = Point(0, 0);
			Point pt2 = Point(0, 0);
			Point pt3 = Point(0, 0);
			Point pt4 = Point(0, 0);
			pt1 = center - point;
			pt2.x = rx + point.x;
			pt2.y = ry -point.y;
			pt3 = center + point;
			pt4.x = rx - point.x;
			pt4.y = ry + point.y;
		
			
		

			Predictor predictor1[8];
			predictor1[0].setRecord(pt1.x,n);
			predictor1[1].setRecord(pt1.y,n);
			predictor1[2].setRecord(pt2.x,n);
			predictor1[3].setRecord(pt2.y,n);
			predictor1[4].setRecord(pt3.x,n);
			predictor1[5].setRecord(pt3.y,n);
			predictor1[6].setRecord(pt4.x,n);
			predictor1[7].setRecord(pt4.y,n);
			double   predictx1, predicty1, predictx2, predicty2,predictx3, predicty3, predictx4, predicty4;
			predictx1 = predictor1[0].predict(n);
			predicty1 = predictor1[1].predict(n);
			predictx2 = predictor1[2].predict(n);
			predicty2 = predictor1[3].predict(n);
			predictx3 = predictor1[4].predict(n);
			predicty3 = predictor1[5].predict(n);
			predictx4 = predictor1[6].predict(n);
			predicty4 = predictor1[7].predict(n);
			++n;

			circle(frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);

			circle(frame, center, radius, Scalar(155, 50, 255), 3, 8, 0);


			//Point2f pt[4] = { pt1,pt2 ,pt3 , pt4 };
			cout << pt1 << "\t" << pt2 << "\t" << pt3 << "\t" << pt4<<"预测\t"<<
			"["<< predictx1<<"\t"<< predicty1<<"]"<<  "[" << predictx2 << "\t" << predicty2 << "]" 
		<< "[" << predictx3 << "\t" << predicty3 << "]" <<  "[" << predictx4 << "\t" << predicty4 << "]" << endl;//在控制台输出检测到的目标内接矩形的四个角点坐标
		    
		}

		
		namedWindow("效果");
		imshow("效果", frame);
		c = cvWaitKey(30);
		if (c == 27)//Esc键退出
		{
			break;
		}
	}
	return 0;
}



/*
#include "pch.h"          基尔霍夫变化另一种检测的方法
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture capture(0);

	Mat srcImage;
	Mat midImage;
	IplImage *img;

	vector<Vec3f> circles;

	while (1)
	{
		capture >> srcImage;

		IplImage *src;
		src = &IplImage(srcImage);

		IplImage imgTmp = srcImage;
		img = cvCloneImage(&imgTmp);


		cvtColor(srcImage, midImage, COLOR_BGRA2GRAY);
		GaussianBlur(midImage, midImage, Size(9, 9), 2, 2);

		HoughCircles(midImage, circles, HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);

		int bVal = 0;
		int gVal = 0;
		int rVal = 0;
		string c = "";

		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			int rx = cvRound(circles[i][0]);
			int ry = cvRound(circles[i][1]);
			Point point;
			point.x = sqrt(radius * radius) / 2;
			point.y = sqrt(radius * radius) / 2;
			Point pt1 = Point(0, 0);
			Point pt2 = Point(0, 0);
			Point pt3 = Point(0, 0);
			Point pt4 = Point(0, 0);
			pt1 = center - point;
			pt2.x = rx + point.x;
			pt2.y = ry - point.y;
			pt3 = center + point;
			pt4.x = rx - point.x;
			pt4.y = ry + point.y;

			circle(srcImage, center, 3, Scalar(0, 0, 255), -1, 8, 0);

			circle(srcImage, center, radius, Scalar(0, 0, 0), 3, 8, 0);

			cv::Vec3b bgr = srcImage.at<cv::Vec3b>(center.x, center.y);

			/*
			bVal = (unsigned int)bgr.val[0];
			gVal = (unsigned int)bgr.val[1];
			rVal = (unsigned int)bgr.val[2];

			cout << bVal;
			cout << endl;
			cout << gVal;
			cout << endl;
			cout << rVal;
			cout << endl;
			cout << endl;

			if (bVal >= gVal + rVal)
			{
				c = "Blue";
			}
			else if (gVal >= bVal + rVal)
			{
				c = "Green";
			}
			else if (rVal >= gVal + bVal)
			{
				c = "Red";
			}
			else
			{
				c = "Unknown";
			}

			src = &IplImage(srcImage);

			CvFont font;
			cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 2.0, 3.0);

			string x = "x=" + std::to_string(center.x) + " ";
			string y = "y=" + std::to_string(center.y) + " ";
			string xy = x + y;

			string r = "radius=" + std::to_string(radius);
			string color = "color=" + c;

			const char* xytem = xy.data();
			const char* rtem = r.data();
			const char* colortem = color.data();

			cvPutText(src, xytem, cvPoint(center.x - 200, center.y), &font, cvScalar(0, 0, 0));
			cvPutText(src, rtem, cvPoint(center.x - 200, center.y + 50), &font, cvScalar(0, 0, 0));
			cvPutText(src, colortem, cvPoint(center.x - 200, center.y + 100), &font, cvScalar(0, 0, 0));
			
			cout << pt1 << "\t" << pt2 << "\t" << pt3 << "\t" << pt4 << endl;

		}
		
			
		cvShowImage("效果图像", src);

		if (waitKey(30) >= 0) break;

		waitKey(30);

	}

	return 0;

}

*/
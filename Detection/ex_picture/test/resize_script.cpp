#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	string path = "E:/ex_picture/sample/cur/buff_1024/2.jpg";
	int resize_height = 416;
	int resize_width = 416;
	Mat src = imread(path); //从路径名中读取图片
	Mat dst;
	imshow("src", src);//显示图片
	resize(src, dst, Size(resize_width, resize_height), (0, 0), (0, 0), INTER_LINEAR);//重新调整图像大小
	imshow("dst", dst);
	imwrite("E:/ex_picture/sample/cur/buff_1024/2_1.jpg",dst);
	waitKey(0);
	return 0;
	
	/*
	int num = 179;
	int n = 18882;
	for (int i = 1; i <= num; i++)
	{
		
		string path = "E:/ex_picture/sample/7/";
		path = path + "1 (" + to_string(i) + ")" + ".jpg";
		Mat  img = imread(path);
		Mat dst;
		resize(img, dst, Size(416, 416));
		string path_size = "E:/ex_picture/sample/det/" + to_string(n)+".jpg";
		n++;
		imwrite(path_size, dst);
	} 
	*/
	return 0;
}

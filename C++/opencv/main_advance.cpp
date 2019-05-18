#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#include <direct.h>
#include <io.h>

// #include <string>
// #include <sstream>


#include<ctime>

using namespace cv;
using namespace std;


int main(int argc, char const *argv[])
{
    clock_t start,end;
    double endtime;
    
    Mat src_img = imread("pictures\\16-1.jpg");

    // 转成灰度图
    Mat gray_img;
    cvtColor(src_img, gray_img, COLOR_BGR2GRAY);

    start=clock();

    // 二值化处理
    Mat thread_img;
    threshold(gray_img, thread_img, 60, 255, CV_THRESH_BINARY_INV);
//    imshow("thread_img",thread_img);

    // end=clock();
    // endtime=(double)(end-start)/CLOCKS_PER_SEC;
    // cout<<"1 Total time:"<<endtime*1000<<"ms"<<endl;

    // start=clock();

    // 轮廓检测
    vector<vector<Point>>contours;
    vector<Vec4i> hierarchy;
    findContours(thread_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    // 找到目标轮廓
    Mat draw_img = Mat::zeros(src_img.size(), CV_8UC1);

    for(int i = 0; i < contours.size(); i++)
    {
        RotatedRect minRect = minAreaRect(contours[i]);
        Size2f minRectSize = minRect.size;

        double rate = minRectSize.height / minRectSize.width;
        double area = contourArea(contours[i]);

        // 过滤掉非圆形和面积较小的轮廓
        if(rate > 0.9 && rate < 1.1 && area > 1000 && area < 10000)
        {
            // 画出轮廓
            drawContours(draw_img, contours, i, 255, 2, (0,0,255), hierarchy, 0, Point(0, 0));
        }
    }

    // end=clock();
    // endtime=(double)(end-start)/CLOCKS_PER_SEC;
    // cout<<"2 Total time:"<<endtime*1000<<"ms"<<endl;

    // start=clock();

    // imshow("draw_img",draw_img);

    // 霍夫圆检测
    vector<Vec3f>circles;
    HoughCircles(draw_img, circles, HOUGH_GRADIENT, 1.5, 35, 200, 80, 0, 0);

    end=clock();
    endtime=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<"3 Total time:"<<endtime*1000<<"ms"<<endl;

    for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        circle(src_img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        circle(src_img, center, radius, Scalar(155, 50, 255), 3, 8, 0);

		stringstream sstr;
		sstr << "c: " << center;
		putText(src_img, sstr.str(), center, cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255), 4, 8, 0);

		sstr.clear();//清空流
		sstr.str("");//清空流缓存
    }

    namedWindow("img", WINDOW_NORMAL);

    imshow("img",src_img);
    waitKey();
    destroyAllWindows();
}
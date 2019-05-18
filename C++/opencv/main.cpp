#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>


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

    // 高斯滤波
    Mat blur_img;
    GaussianBlur(gray_img, blur_img, Size(9, 9), 2, 2);

    end=clock();
    endtime=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<"1 Total time:"<<endtime*1000<<"ms"<<endl;

    start=clock();

    // sobel边缘检测
    Mat sobel_img, grad_x, abs_grad_x, grad_y, abs_grad_y;
    Sobel(blur_img, grad_x, CV_16S, 1, 0);//, 3, 1, 1,BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);
    Sobel(blur_img, grad_y, CV_16S, 0, 1);//,3, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(grad_y,abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel_img);
    // imshow("sobel_img",sobel_img);

    end=clock();
    endtime=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<"1 Total time:"<<endtime*1000<<"ms"<<endl;

    start=clock();

    // 二值化处理
    Mat thread_img;
    threshold(sobel_img, thread_img, 60, 255, CV_THRESH_BINARY_INV);
    // imshow("thread_img",thread_img);

    end=clock();
    endtime=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<"1 Total time:"<<endtime*1000<<"ms"<<endl;

    start=clock();

    // 轮廓检测
    vector<vector<Point>>contours;
    vector<Vec4i> hierarchy;
    findContours(thread_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    // 找到目标轮廓
    int current_idx = -1;
    int next_idx = -1;

    Mat draw_img = Mat::zeros(src_img.size(), CV_8UC1);

    for(int i = 0; i < contours.size(); i++)
    {
        RotatedRect minRect = minAreaRect(contours[i]);
        Size2f minRectSize = minRect.size;

        double rate = minRectSize.height / minRectSize.width;
        double area = contourArea(contours[i]);

        current_idx = i;
        // 过滤掉非圆形，面积较小的轮廓，并去除内轮廓
        if(rate > 0.9 && rate < 1.1 && area > 1000 && area < 10000 && current_idx != next_idx)
        {
            // 画出轮廓
            drawContours(draw_img, contours, i, 255, 2, 8, hierarchy, 0, Point(0, 0));

            next_idx = hierarchy[i][2];    
        }
    }
    // imshow("draw_img",draw_img);

    end=clock();
    endtime=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<"2 Total time:"<<endtime*1000<<"ms"<<endl;

    start=clock();

    //霍夫圆检测
    vector<Vec3f>circles;
    HoughCircles(draw_img, circles, HOUGH_GRADIENT, 1.5, 35, 200, 60, 0, 0);

    for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        circle(src_img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        circle(src_img, center, radius, Scalar(155, 50, 255), 3, 8, 0);
    }

    end=clock();
    endtime=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<"3 Total time:"<<endtime*1000<<"ms"<<endl;

    imshow("img",src_img);
    waitKey();
}
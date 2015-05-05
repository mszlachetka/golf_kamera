#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
 #include <QTimer>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "MatToQImage.h"
#include <QVector>

#include <QtCore/qmath.h>

#include <myrect.h>
#include <mycircle.h>

#include <QSharedMemory>
#include <QBuffer>



using namespace cv;
using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void get_cv_frame();
    void MyLine( Mat img, Point start, Point end,int blue,int green,int red );
     void MyEllipse( Mat img, double angle,int radius,Point center,int blue, int green, int red  );

      void setLabel(Mat& im, const string label, Point &pt);
      bool addCentralPoint_Circle(vector<Point>& contour);
      bool addCentralPoint_Rect(vector<Point>& contour);


      void Update_shareMemory(QSharedMemory *sharedMemory);
      void Load_from_sharedMemory(QSharedMemory *sharedMemory);







      void on_lineEdit_returnPressed();



private:
    Ui::MainWindow *ui;

     QTimer *mTimer=new QTimer(this);
    CvCapture *capture=cvCaptureFromCAM(CV_CAP_ANY);
    int iteration=0;

    int  displacement_y=480;



    Mat frame, binary;



     QVector<Point> central_Points_Triangle;

     int position=0;

QSharedMemory sharedMemory_GamePad;
QSharedMemory sharedMemory_Camera;




       bool is_center_of_marker=true;
       bool is_lower_side_of_marker=true;
       bool is_higher_side_of_marker=true;

       double robot_x=9999,robot_y=9999,robot_a=9999;
       double ball_x=9999,ball_y=9999;
       double map_x=9999,map_y=9999,map_a=9999,map_distance=9999;
       double hole_x=9999,hole_y=9999;



       QVector <MyCircle *> map_markers;
       double map_angle=0;
        QVector <MyCircle *> central_Points_Circle;

       QVector <MyCircle *> ball_marker;

       QVector <MyRect *> hole_marker;


         QVector <MyRect *> robot_marker;
         double robot_angle=0;
       QVector <MyRect *> central_Points_Rect;


       double gamepad_robot_x=100;
       double gamepad_robot_y=100;
       double gamepad_robot_theta=0;



       Point temp_Point;

       int posistion_of_center_in_vector=-1;




    static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
    {
        double dx1 = pt1.x - pt0.x;
        double dy1 = pt1.y - pt0.y;
        double dx2 = pt2.x - pt0.x;
        double dy2 = pt2.y - pt0.y;
        return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    }



    int lowerb = 50, upperb = 210 ;



};

#endif // MAINWINDOW_H

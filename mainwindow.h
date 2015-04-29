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
    void on_pushButton_clicked();
    void get_cv_frame();
    void MyLine( Mat img, Point start, Point end,int blue,int green,int red );
     void MyEllipse( Mat img, double angle,int radius,Point center,int blue, int green, int red  );
     void setLabel(Mat& im, const string label, Point &pt);
      bool addCentralPoint_Circle(vector<Point>& contour);
      bool addCentralPoint_Rect(vector<Point>& contour);








      void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;

     QTimer *mTimer=new QTimer(this);
    CvCapture *capture= cvCaptureFromCAM(CV_CAP_ANY);
    Mat frame, binary;
     QVector<Point> central_Points_Circle;
     QVector <MyRect *> central_Points_Rect;
     QVector<Point> central_Points_Triangle;




       bool is_center_of_marker=true;
       bool is_lower_side_of_marker=true;
       bool is_higher_side_of_marker=true;

       Point center_of_marker;
       QVector <Point> lower_side_of_marker;
       QVector <Point> higher_side_of_marker;

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

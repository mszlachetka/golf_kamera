#ifndef MYRECT_H
#define MYRECT_H

#include <QObject>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <MatToQImage.h>
#include <QVector>

using namespace cv;
using namespace std;

class MyCircle;

class MyRect
{
public:
    MyRect();
    ~MyRect();


    Point centrum;
    double a_lenght=0;
    double b_lenght=0;

     vector <Point>  Mcontour;

      QVector <MyCircle *> circles_inside;

      bool used=false;





};

#endif // MYRECT_H

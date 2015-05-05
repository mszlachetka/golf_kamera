#ifndef MYCIRCLE_H
#define MYCIRCLE_H

#include <QObject>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <MatToQImage.h>
#include <QVector>

using namespace cv;
using namespace std;

class MyRect;


class MyCircle
{
public:
    MyCircle();
    ~MyCircle();

    Point centrum;
    double r=0;

    vector <Point>  Mcontour;

    QVector <MyRect *> rects_inside;

    bool used=false;


};

#endif // MYCIRCLE_H

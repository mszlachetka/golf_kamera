#ifndef MARKER_RECT_IN_CIRCLE_H
#define MARKER_RECT_IN_CIRCLE_H

#include <QObject>
#include <QVector>
#include <mycircle.h>

class Marker_rect_in_circle
{
public:
    Marker_rect_in_circle();
    ~Marker_rect_in_circle();

   MyCircle map_markers;
    int number_of_rects=0;

};

#endif // MARKER_RECT_IN_CIRCLE_H

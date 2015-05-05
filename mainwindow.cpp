#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), sharedMemory_GamePad("brzoza"),sharedMemory_Camera("palma"),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this->mTimer, SIGNAL(timeout()), this, SLOT(get_cv_frame()));
    mTimer->start(20);




}

MainWindow::~MainWindow()
{

   this->frame.release();
    this->binary.release();
       cvReleaseCapture(&this->capture);

    delete ui;

}



 void MainWindow::get_cv_frame()
 {
    vector<vector<Point> > contours;
     vector<Vec4i> hierarchy;


     QImage mImage;


     this->map_markers.clear();
     this->robot_marker.clear();
     this->ball_marker.clear();
     this->hole_marker.clear();

     qDeleteAll(this->central_Points_Circle);
     qDeleteAll(this->central_Points_Rect);
    // qDeleteAll(this->map_markers);
    // qDeleteAll(this->robot_marker);
    // qDeleteAll(this->hole_marker);
    // qDeleteAll(this->ball_marker);


     central_Points_Circle.clear();
     central_Points_Rect.clear();


    this->frame=cvQueryFrame(this->capture);


   mImage = MatToQImage(this->frame);
     ui->mImage_Label->setPixmap(QPixmap::fromImage(mImage));




//Canny i adaptative najlepsze

    //cvtColor(frame, binary, CV_BGR2GRAY);

    if(frame.empty())
     {

    }
    else if(frame.channels()>1)
    {
        cvtColor(frame,binary, CV_BGR2GRAY);

    }
    else binary = frame;

    if(!frame.empty())
     {

   //threshold(binary, binary,ui->horizontalSlider->value(), 255,THRESH_BINARY_INV); //nie przerywa sygnalu ale czasem ciezko filtrowac

    //binary= binary<ui->horizontalSlider->value();

 blur(binary,binary,Size(3,3),Point(-1,-1),BORDER_DEFAULT);
  Canny(binary, binary, ui->horizontalSlider->value(),  ui->horizontalSlider->value()*3, 3,false );//slider do 100 //wszystko ale przerywa czasem
blur(binary,binary,Size(3,3),Point(-1,-1),BORDER_DEFAULT);//pogrubienie linii
 threshold(binary, binary,10, 255,THRESH_BINARY);






 mImage = MatToQImage(this->binary);
ui->mBinary_Image->setPixmap(QPixmap::fromImage(mImage));


 findContours(binary.clone(), contours,hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);



 vector<Point> approx;
 position=0;


 for (int i = 0; i < contours.size(); i+=2)
 {


   approxPolyDP(Mat(contours[i]), approx,
                    arcLength(Mat(contours[i]), true)*0.02, true);




   if ( !cv::isContourConvex(approx)) continue;
   if (approx.size() == 3)
   {

       Rect r = boundingRect(contours[i]);

       Point pt(r.x + ((r.width) / 2), r.y + ((r.height) / 2));


       if(r.width>ui->horizontalSlider_2->value() && r.height >ui->horizontalSlider_2->value())
       {

          // setLabel(frame, "TRI", pt);
            MyLine(frame,approx[0],approx[2],0,255,0);
             MyLine(frame,approx[0],approx[1],0,255,0);
              MyLine(frame,approx[1],approx[2],0,255,0);
               position++;
       }



   }
   else if(approx.size()>=4 && approx.size()<6)
   {
       double maxCosine = 0;

             for( int j = 2; j < 5; j++ )
             {
                 double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                 maxCosine = MAX(maxCosine, cosine);

             }

             if( maxCosine < 0.4 )
             {

                 if(addCentralPoint_Rect(contours[i]))
                 {

                  // setLabel(frame, "RECT", central_Points_Rect.first()->centrum);

                   drawContours(frame,contours,i,Scalar(0,0,255),2,8);
                    position++;




                 }

             }
         }

   else if(approx.size()>7)
   {
           double area = cv::contourArea(contours[i]);
           cv::Rect r = cv::boundingRect(contours[i]);
           int radius = r.width / 2;

           if (std::abs(1 - ((double)r.width / r.height)) <= 0.4 && //0.2 w miare dobre
               std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.4)
           {

            if(addCentralPoint_Circle(contours[i]))
            {
                drawContours(frame,contours,i,Scalar(255,0,0),2,8);
           // setLabel(frame, "CIR", central_Points_Circle.first()->centrum);
             position++;
            }

           }
 }


 }

 //rects in circles
 for(int j=0;j<this->central_Points_Circle.size();j++)
 {
for(int i=0;i<this->central_Points_Rect.size();i++)
{
     if(contourArea(this->central_Points_Circle.at(j)->Mcontour)>contourArea(this->central_Points_Rect.at(i)->Mcontour)
             && pointPolygonTest(this->central_Points_Circle.at(j)->Mcontour,this->central_Points_Rect.at(i)->centrum,true) > 0)
     {
        this->central_Points_Circle.at(j)->rects_inside.push_back(this->central_Points_Rect.at(i));
         this->central_Points_Circle.at(j)->used=true;
         this->central_Points_Rect.at(i)->used=true;
     }
}
 }

 //circles in rect
 for(int j=0;j<this->central_Points_Rect.size();j++)
 {
for(int i=0;i<this->central_Points_Circle.size();i++)
{
     if(contourArea(this->central_Points_Rect.at(j)->Mcontour)>contourArea(this->central_Points_Circle.at(i)->Mcontour)
              && pointPolygonTest(this->central_Points_Rect.at(j)->Mcontour,this->central_Points_Circle.at(i)->centrum,true) > 0)
     {
         this->central_Points_Rect.at(j)->circles_inside.push_back(this->central_Points_Circle.at(i));
          this->central_Points_Circle.at(i)->used=true;
         this->central_Points_Rect.at(j)->used=true;
     }
}
 }
 //map markers and ball markers
 for(int j=0;j<this->central_Points_Circle.size();j++)
    {
 if(this->central_Points_Circle.at(j)->rects_inside.size()==1)
 {
     MyEllipse(frame,360,this->central_Points_Circle.at(j)->r,this->central_Points_Circle.at(j)->centrum,255,255,255);
      this->map_markers.push_back(this->central_Points_Circle.at(j));
 }
 else if(this->central_Points_Circle.at(j)->used==false) //else case clear circle is a ball
 {
     MyEllipse(frame,360,this->central_Points_Circle.at(j)->r,this->central_Points_Circle.at(j)->centrum,0,0,0);
     this->ball_marker.push_back(this->central_Points_Circle.at(j));
     this->temp_Point=this->central_Points_Circle.at(j)->centrum;

     setLabel(frame,"BALL",this->temp_Point);
 }

 }

 //robot marker and hole marker
 for(int j=0;j<this->central_Points_Rect.size();j++)
    {
 if(this->central_Points_Rect.at(j)->circles_inside.size()==2)
 {
      this->robot_marker.push_back(this->central_Points_Rect.at(j));
     MyEllipse(frame,360,this->central_Points_Rect.at(j)->circles_inside.at(0)->r,
               this->central_Points_Rect.at(j)->circles_inside.at(0)->centrum,0,255,0);
     MyEllipse(frame,360,this->central_Points_Rect.at(j)->circles_inside.at(1)->r,
               this->central_Points_Rect.at(j)->circles_inside.at(1)->centrum,0,255,0);

 }
 else if(this->central_Points_Rect.at(j)->circles_inside.size()==1) //hole marker
 {
      this->hole_marker.push_back(this->central_Points_Rect.at(j));
     MyEllipse(frame,360,this->central_Points_Rect.at(j)->circles_inside.at(0)->r,
               this->central_Points_Rect.at(j)->circles_inside.at(0)->centrum,0,255,255);

     this->temp_Point=this->central_Points_Rect.at(j)->centrum;
     setLabel(frame,"HOLE",this->temp_Point);


 }
 else if(this->central_Points_Rect.at(j)->circles_inside.size()==0 && this->central_Points_Rect.at(j)->used==false)
 {
      this->temp_Point=this->central_Points_Rect.at(j)->centrum;
     setLabel(frame,"EAGLE EYE!",this->temp_Point);
 }
 }


//angle on map marker
if(this->map_markers.size()==2)
{
    double MyAngle;

    if(this->map_markers.at(1)->centrum.x>this->map_markers.at(0)->centrum.x)
    {
    double cosine= angle(this->map_markers.at(0)->centrum,
                         Point(this->map_markers.at(1)->centrum.x+100,this->map_markers.at(1)->centrum.y),
                         this->map_markers.at(1)->centrum);
            MyAngle=qAcos(cosine)*180/3.14;
             MyAngle=180-MyAngle;

             if(this->map_markers.at(1)->centrum.y>this->map_markers.at(0)->centrum.y) MyAngle=-MyAngle;

             this->map_angle=MyAngle;


    MyLine(frame,this->map_markers.at(0)->centrum,this->map_markers.at(1)->centrum,255,255,255);

    const QString Mstring=QString::number(MyAngle);
    this->temp_Point.x=(this->map_markers.at(1)->centrum.x+this->map_markers.at(0)->centrum.x)/2;
    this->temp_Point.y=(this->map_markers.at(1)->centrum.y+this->map_markers.at(0)->centrum.y)/2;
    setLabel(frame,Mstring.toStdString(),this->temp_Point);
    this->temp_Point.y=this->temp_Point.y+15;
    setLabel(frame,"MAP",this->temp_Point);
    }
    else if(this->map_markers.at(0)->centrum.x>this->map_markers.at(1)->centrum.x)
    {
    double cosine= angle(this->map_markers.at(1)->centrum,
                         Point(this->map_markers.at(0)->centrum.x+100,this->map_markers.at(0)->centrum.y),
                         this->map_markers.at(0)->centrum);
            MyAngle=qAcos(cosine)*180/3.14;
            MyAngle=180-MyAngle;

            this->map_angle=MyAngle;
             if(this->map_markers.at(0)->centrum.y>this->map_markers.at(1)->centrum.y) MyAngle=-MyAngle;
    MyLine(frame,this->map_markers.at(0)->centrum,this->map_markers.at(1)->centrum,255,255,255);

    const QString Mstring=QString::number(MyAngle);
    this->temp_Point.x=(this->map_markers.at(1)->centrum.x+this->map_markers.at(0)->centrum.x)/2;
    this->temp_Point.y=(this->map_markers.at(1)->centrum.y+this->map_markers.at(0)->centrum.y)/2;
    setLabel(frame,Mstring.toStdString(),this->temp_Point);
    this->temp_Point.y=this->temp_Point.y+15;
    setLabel(frame,"MAP",this->temp_Point);
    }
}

//angle on robot marker
if(this->robot_marker.size()==1)
{
     double MyAngle;
    if(this->robot_marker.at(0)->circles_inside.at(0)->r>this->robot_marker.at(0)->circles_inside.at(1)->r)
    {

    MyLine(frame,this->robot_marker.at(0)->circles_inside.at(0)->centrum,this->robot_marker.at(0)->circles_inside.at(1)->centrum,255,255,255);
 double cosine = angle(this->robot_marker.at(0)->circles_inside.at(0)->centrum,
                       Point(this->robot_marker.at(0)->circles_inside.at(1)->centrum.x+100,this->robot_marker.at(0)->circles_inside.at(1)->centrum.y)
                      ,this->robot_marker.at(0)->circles_inside.at(1)->centrum);

 if(this->robot_marker.at(0)->circles_inside.at(1)->centrum.y>this->robot_marker.at(0)->circles_inside.at(0)->centrum.y)  MyAngle=qAcos(cosine)*180/3.14;
 else  MyAngle=360-qAcos(cosine)*180/3.14;

     const QString Mstring=QString::number(MyAngle);

     this->robot_angle=MyAngle;

     this->temp_Point.x=(this->robot_marker.at(0)->circles_inside.at(0)->centrum.x+this->robot_marker.at(0)->circles_inside.at(1)->centrum.x)/2;
     this->temp_Point.y=(this->robot_marker.at(0)->circles_inside.at(0)->centrum.y+this->robot_marker.at(0)->circles_inside.at(1)->centrum.y)/2;
 setLabel(frame,Mstring.toStdString(),this->temp_Point);
 this->temp_Point.y=this->temp_Point.y+15;
 setLabel(frame,"ROBOT",this->temp_Point);
 this->temp_Point.y=this->temp_Point.y-30;
//const QString position_label="("+QString::number(this->robot_marker.at(0)->centrum.x)+","+QString::number(this->robot_marker.at(0)->centrum.y)+")";
//setLabel(frame,position_label.toStdString(),this->temp_Point);

    }
    else
    {

        MyLine(frame,this->robot_marker.at(0)->circles_inside.at(1)->centrum,this->robot_marker.at(0)->circles_inside.at(0)->centrum,255,255,255);
     double cosine = angle(this->robot_marker.at(0)->circles_inside.at(1)->centrum,
                           Point(this->robot_marker.at(0)->circles_inside.at(0)->centrum.x+100,this->robot_marker.at(0)->circles_inside.at(0)->centrum.y)
                          ,this->robot_marker.at(0)->circles_inside.at(0)->centrum);

     if(this->robot_marker.at(0)->circles_inside.at(0)->centrum.y>this->robot_marker.at(0)->circles_inside.at(1)->centrum.y)  MyAngle=qAcos(cosine)*180/3.14;
     else  MyAngle=360-qAcos(cosine)*180/3.14;

         const QString Mstring=QString::number(MyAngle);

         this->robot_angle=MyAngle;

         this->temp_Point.x=(this->robot_marker.at(0)->circles_inside.at(1)->centrum.x+this->robot_marker.at(0)->circles_inside.at(0)->centrum.x)/2;
         this->temp_Point.y=(this->robot_marker.at(0)->circles_inside.at(1)->centrum.y+this->robot_marker.at(0)->circles_inside.at(0)->centrum.y)/2;
     setLabel(frame,Mstring.toStdString(),this->temp_Point);
     this->temp_Point.y=this->temp_Point.y+15;
     setLabel(frame,"ROBOT",this->temp_Point);
    //   this->temp_Point.y=this->temp_Point.y-30;
    // const QString position_label="("+QString::number(this->robot_marker.at(0)->centrum.x)+","+QString::number(this->robot_marker.at(0)->centrum.y)+")";
    // setLabel(frame,position_label.toStdString(),this->temp_Point);
    }
}



Update_shareMemory(&sharedMemory_Camera);
Load_from_sharedMemory(&sharedMemory_GamePad);
//MyEllipse(frame,360,20,Point((double)this->gamepad_robot_x,(double)this->gamepad_robot_y),0,255,255);


/*const QString Mstring1=QString::number((double)this->gamepad_robot_x);
const QString Mstring2=QString::number((double)this->gamepad_robot_y);

this->temp_Point=Point(300,300);
setLabel(frame,Mstring1.toStdString(),this->temp_Point);

this->temp_Point.y=this->temp_Point.y+15;
setLabel(frame,Mstring2.toStdString(),this->temp_Point);*/

 mImage = MatToQImage(this->frame);
ui->mImage_Label->setPixmap(QPixmap::fromImage(mImage));



iteration++;


mImage=QImage();

}

  /*  if(iteration==1000)
    {
        cvReleaseCapture(&this->capture);
        this->capture= cvCaptureFromCAM(ui->lineEdit->text().toInt());
        this->frame.release();
         this->binary.release();
        iteration=0;
    }*/

 }





 void MainWindow::MyLine( Mat img, Point start, Point end, int blue, int green, int red )
   {
       int thickness = 2;
        int lineType = 8;
        line( img,
              start,
              end,
              Scalar( blue, green, red ),
              thickness,
              lineType );
   }

   void MainWindow::MyEllipse( Mat img, double angle,int radius,Point center,int blue, int green, int red  )
   {
     int thickness = 2;
     int lineType = 8;

     ellipse( img,
              center,
              Size( radius, radius ),
              angle,
              0,
              360,
              Scalar( blue, green, red ),
              thickness,
              lineType );
   }

   void MainWindow::setLabel(Mat& im, const string label, Point &pt)
   {
       int fontface = cv::FONT_HERSHEY_SIMPLEX;
       double scale = 0.4;
       int thickness = 1;
       int baseline = 0;

       Size text = getTextSize(label, fontface, scale, thickness, &baseline);
       //
       //Rect r = boundingRect(contour);

       Point pt2(pt.x - (text.width) / 2,pt.y + ((text.height) / 2));
       rectangle(im, pt2 + Point(0, baseline), pt2 + Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
       putText(im, label, pt2, fontface, scale, CV_RGB(0,0,0), thickness, 8);
   }


  bool MainWindow::addCentralPoint_Circle(vector<Point>& contour)
   {
       Rect r = boundingRect(contour);

        MyCircle *resultCircle= new MyCircle;


       bool result=false;
       Point pt(r.x + ((r.width) / 2), r.y + ((r.height) / 2));
        if(r.width>ui->horizontalSlider_2->value() && r.height >ui->horizontalSlider_2->value())
        {
            resultCircle->centrum=pt;
            resultCircle->r=r.width/2;

            for(int i=0;i<contour.size();i++)
            resultCircle->Mcontour.push_back(contour.at(i));

       central_Points_Circle.push_front(resultCircle);



       result= true;
         //    circle(frame,pt,r.width/2,Scalar(255,0,0),2,8,0);
        }

    return result;


   }


   bool MainWindow::addCentralPoint_Rect(vector<Point>& contour)
   {
       Rect r = boundingRect(contour);

       MyRect *resultRect= new MyRect;

bool result=false;
Point pt(r.x + ((r.width) / 2), r.y + ((r.height) / 2));
        if(r.width>ui->horizontalSlider_2->value()/2 && r.height >ui->horizontalSlider_2->value()/2)
        {
            resultRect->centrum=pt;
            resultRect->a_lenght=r.size().width;
            resultRect->b_lenght=r.size().height;
            for(int i=0;i<contour.size();i++)
            resultRect->Mcontour.push_back(contour.at(i));

      central_Points_Rect.push_front(resultRect);



        result= true;
        }

        return result;


   }

void MainWindow::on_lineEdit_returnPressed()
{
     cvReleaseCapture(&this->capture);
    this->capture = cvCaptureFromCAM(ui->lineEdit->text().toInt());
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 752 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480);


}

void MainWindow::Load_from_sharedMemory(QSharedMemory *sharedMemory)
{
    if (!sharedMemory->attach()) {
      return;
     }

    QBuffer buffer;
     QDataStream in(&buffer);

     sharedMemory->lock();
     buffer.setData((char*)sharedMemory->constData(), sharedMemory->size());
     buffer.open(QBuffer::ReadOnly);

     in>>this->gamepad_robot_x>>this->gamepad_robot_y;


     sharedMemory->unlock();

     sharedMemory->detach();


 }

#include<QDebug>
 void MainWindow::Update_shareMemory(QSharedMemory *sharedMemory)
{
     if (sharedMemory->isAttached())
       sharedMemory->detach();

     QBuffer buffer;
     // load into shared memory
     buffer.open(QBuffer::ReadWrite);
     QDataStream out(&buffer);

     QString status=NULL;

     status+="2";
    if(this->robot_marker.size()==1)
    {
        this->robot_x=this->robot_marker.at(0)->centrum.x;
                this->robot_y=-1*this->robot_marker.at(0)->centrum.y+this->displacement_y;
        this->robot_a=this->robot_angle;
        status+="1";
    }
    else status+="0";
    if(this->ball_marker.size()==1)
    {
        this->ball_x=this->ball_marker.at(0)->centrum.x;
                this->ball_y=-1*this->ball_marker.at(0)->centrum.y+this->displacement_y;
         status+="1";
    }
    else  status+="0";
    if( this->map_markers.size()==2)
    {
          if(this->map_markers.at(0)->centrum.x>this->map_markers.at(1)->centrum.x)
          {
              this->map_x=this->map_markers.at(1)->centrum.x;
                      this->map_y=-1*map_markers.at(1)->centrum.y+this->displacement_y;
                      this->map_a=this->map_angle;
              this->map_distance=sqrt(pow(this->map_markers.at(0)->centrum.x-this->map_markers.at(1)->centrum.x,2)
                                     +pow(this->map_markers.at(0)->centrum.y-this->map_markers.at(1)->centrum.y,2));
              status+="1";
          }
          else
          {
              this->map_x=this->map_markers.at(0)->centrum.x;
                      this->map_y=-1*map_markers.at(0)->centrum.y+this->displacement_y;
                      this->map_a=this->map_angle;
              this->map_distance=sqrt(pow(this->map_markers.at(0)->centrum.x-this->map_markers.at(1)->centrum.x,2)
                                     +pow(this->map_markers.at(0)->centrum.y-this->map_markers.at(1)->centrum.y,2));
              status+="1";
          }

    }
    else status+="0";
      if(this->hole_marker.size()==1)
      {
          this->hole_x=this->hole_marker.at(0)->centrum.x;
                  this->hole_y=-1*this->hole_marker.at(0)->centrum.y+this->displacement_y;
          status+="1";
      }
      else status+="0";





     out<<(double)status.toDouble()<<
          (double)this->robot_x<<(double)this->robot_y<<(double)this->robot_a
       <<(double)this->ball_x<<(double)this->ball_y
      <<(double)map_x<<(double)map_y<<(double)this->map_a<<(double)this->map_distance
     <<(double)this->hole_x<<(double)this->hole_y;


     int size = buffer.size();

     sharedMemory->create(size);

     sharedMemory->lock();
     char *to = (char*)sharedMemory->data();
     const char *from = buffer.data().data();
     memcpy(to, from, qMin(sharedMemory->size(), size));
     sharedMemory->unlock();
 }





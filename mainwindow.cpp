#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
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
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{


}







 void MainWindow::get_cv_frame()
 {
     vector<vector<Point> > contours;
   vector<Vec4f> lines;





    this->frame=cvQueryFrame(this->capture);

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
 blur(binary,binary,Size(3,3),Point(-1,-1),BORDER_DEFAULT);
 threshold(binary, binary,10, 255,THRESH_BINARY);






            QImage Binary_Image = MatToQImage(this->binary);
ui->mBinary_Image->setPixmap(QPixmap::fromImage(Binary_Image));

 findContours(binary.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

 central_Points_Circle.clear();
 central_Points_Rect.clear();
 vector<Point> approx;

 for (int i = 0; i < contours.size(); i++)
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



           setLabel(frame, "TRI", pt);
            MyLine(frame,approx[0],approx[2],0,255,0);
             MyLine(frame,approx[0],approx[1],0,255,0);
              MyLine(frame,approx[1],approx[2],0,255,0);
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

             if( maxCosine < 0.3 )
             {

                 if(addCentralPoint_Rect(contours[i]))
                 {

                   setLabel(frame, "RECT", central_Points_Rect.first()->centrum);




                   drawContours(frame,contours,i,Scalar(0,0,255),2,8);
                 }

             }
         }

   else if(approx.size()>7)
   {
           double area = cv::contourArea(contours[i]);
           cv::Rect r = cv::boundingRect(contours[i]);
           int radius = r.width / 2;

           if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
               std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
           {

            if(addCentralPoint_Circle(contours[i]))
            {
                drawContours(frame,contours,i,Scalar(255,0,0),2,8);
            setLabel(frame, "CIR", central_Points_Circle.first());
            }

           }
 }




 }


//marker na dwoch prostokatach

if(this->central_Points_Rect.size()>1)
{
     double MyAngle;
    if(this->central_Points_Rect.at(0)->a_lenght*this->central_Points_Rect.at(0)->b_lenght>this->central_Points_Rect.at(1)->a_lenght*this->central_Points_Rect.at(1)->b_lenght)
    {
    MyLine(frame,this->central_Points_Rect.at(1)->centrum,Point(this->central_Points_Rect.at(1)->centrum.x+100,this->central_Points_Rect.at(1)->centrum.y),0,0,0);
    MyLine(frame,this->central_Points_Rect.at(0)->centrum,this->central_Points_Rect.at(1)->centrum,255,255,255);
 double cosine = angle(this->central_Points_Rect.at(0)->centrum,Point(this->central_Points_Rect.at(1)->centrum.x+100,this->central_Points_Rect.at(1)->centrum.y)
                      ,this->central_Points_Rect.at(1)->centrum);



 if(this->central_Points_Rect.at(1)->centrum.y>this->central_Points_Rect.at(0)->centrum.y)  MyAngle=qAcos(cosine)*180/3.14;
 else  MyAngle=360-qAcos(cosine)*180/3.14;

     const QString Mstring=QString::number(MyAngle);

     this->temp_Point.x=(this->central_Points_Rect.at(1)->centrum.x+this->central_Points_Rect.at(0)->centrum.x)/2;
     this->temp_Point.y=(this->central_Points_Rect.at(1)->centrum.y+this->central_Points_Rect.at(0)->centrum.y)/2;
 setLabel(frame,Mstring.toStdString(),this->temp_Point);

    }
    else
    {
        MyLine(frame,this->central_Points_Rect.at(0)->centrum,Point(this->central_Points_Rect.at(0)->centrum.x+100,this->central_Points_Rect.at(0)->centrum.y),0,0,0);
        MyLine(frame,this->central_Points_Rect.at(1)->centrum,this->central_Points_Rect.at(0)->centrum,255,255,255);
     double cosine = angle(this->central_Points_Rect.at(1)->centrum,Point(this->central_Points_Rect.at(0)->centrum.x+100,this->central_Points_Rect.at(0)->centrum.y)
                          ,this->central_Points_Rect.at(0)->centrum);


     if(this->central_Points_Rect.at(1)->centrum.y<this->central_Points_Rect.at(0)->centrum.y) MyAngle=qAcos(cosine)*180/3.14;
     else  MyAngle=360-qAcos(cosine)*180/3.14;

     this->temp_Point.x=(this->central_Points_Rect.at(1)->centrum.x+this->central_Points_Rect.at(0)->centrum.x)/2;
     this->temp_Point.y=(this->central_Points_Rect.at(1)->centrum.y+this->central_Points_Rect.at(0)->centrum.y)/2;

     const QString Mstring=QString::number(MyAngle);
     setLabel(frame,Mstring.toStdString(),this->temp_Point);
    }
}


 QImage RawImage = MatToQImage(this->frame);
 ui->mImage_Label->setPixmap(QPixmap::fromImage(RawImage));
}

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



       bool result=false;
       Point pt(r.x + ((r.width) / 2), r.y + ((r.height) / 2));
        if(r.width>ui->horizontalSlider_2->value() && r.height >ui->horizontalSlider_2->value())
        {
       central_Points_Circle.push_front(pt);
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
        if(r.width>ui->horizontalSlider_2->value() && r.height >ui->horizontalSlider_2->value())
        {
            resultRect->centrum=pt;
            resultRect->a_lenght=r.size().width;
            resultRect->b_lenght=r.size().height;
      central_Points_Rect.push_front(resultRect);

        result= true;
        }

        return result;


   }

void MainWindow::on_lineEdit_returnPressed()
{
    this->capture = cvCaptureFromCAM(ui->lineEdit->text().toInt());
}

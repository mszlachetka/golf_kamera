#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T16:43:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = golf_kamera
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MatToQImage.cpp \
    myrect.cpp

INCLUDEPATH +=C:\opencv\release\install\include
LIBS += "C:\\\opencv\release\lib\libopencv_gpu2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_haartraining_engined.a"
LIBS += "C:\\opencv\release\lib\libopencv_highgui_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_highgui2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_imgproc_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_imgproc2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_legacy_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_legacy2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_ml_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_ml2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_nonfree_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_nonfree2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_objdetect_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_objdetect2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_ocl_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_ocl2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_calib3d_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_core_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_features2d_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_gpu_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_highgui_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_imgproc_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_nonfree_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_objdetect_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_ocl_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_photo_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_stitching_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_superres_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_perf_video_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_photo_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_photo2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_stitching_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_stitching2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_superres_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_superres2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_calib3d_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_contrib_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_core_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_features2d_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_flann_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_gpu_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_highgui_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_imgproc_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_legacy_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_ml_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_nonfree_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_objdetect_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_ocl_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_photo_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_stitching_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_superres_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_test_video_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_ts_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_ts2410d.a"
LIBS += "C:\\opencv\release\lib\libopencv_video_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_video2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_videostab_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_videostab2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_calib3d_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_calib3d2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_contrib_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_contrib2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_core_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_core2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_features2d_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_features2d2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_flann_pch_dephelpd.a"
LIBS += "C:\\opencv\release\lib\libopencv_flann2410d.dll.a"
LIBS += "C:\\opencv\release\lib\libopencv_gpu_pch_dephelpd.a"



HEADERS  += mainwindow.h \
    MatToQImage.h \
    myrect.h

FORMS    += mainwindow.ui

TARGET = test-opencv
LIBS += opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui opencv_flann opencv_features2d opencv_calib3d opencv_ml
OPENCV_PORT_DIR := $(call select_from_ports,opencv)
SRC_CC = main.cc


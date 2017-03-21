#include "imageanalysis.h"
#include <QDebug>
#include <QFileInfo>
#include <QtMath>
#include <QPoint>
#include <iostream>
#include <vector>
#include <opencv2/core/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>

ImageAnalysis::ImageAnalysis(QObject *parent) : QObject(parent)
{

}

float ImageAnalysis::eIndex() {
    return erythemaIndex;
}

void ImageAnalysis::split(QUrl url) {
    input = cv::imread(url.toLocalFile().toStdString(),CV_LOAD_IMAGE_COLOR);
    qDebug()<<url.toLocalFile();
    cv::split(input, channel);

    //    qDebug()<<"Depth of channel "<<channel[0].depth();  // CV_8U = 0
    //    cv::namedWindow("Red",CV_WINDOW_AUTOSIZE);
    //    cv::namedWindow("Green",CV_WINDOW_AUTOSIZE);
    //    cv::namedWindow("Blue",CV_WINDOW_AUTOSIZE);

    //    if(!channel[0].empty())
    //        cv::imshow("Blue", channel[0]);
    //    if(!channel[1].empty())
    //        cv::imshow("Green", channel[1]);
    //    if(!channel[2].empty())
    //        cv::imshow("Red", channel[2]);
    /*
     * Note: OpenCV uses BGR color order
     * channel[0] -> blue channel
     * channel[1] -> green channel
     * channel[2] -> red channel
     */
}

void ImageAnalysis::rectRegionOfInterest(int x, int y, int width, int height) {
    cv::Rect roi(x, y, width, height);
    cv::Mat image_roi_blue = channel[0](roi);
    cv::Mat image_roi_green = channel[1](roi);
    cv::Mat image_roi_red = channel[2](roi);

    cv::Scalar mean_roi_blue = cv::mean(image_roi_blue);
    cv::Scalar mean_roi_green = cv::mean(image_roi_green);
    cv::Scalar mean_roi_red = cv::mean(image_roi_red);

    qDebug()<<"mean white brightness in B = "<<mean_roi_blue[0];
    qDebug()<<"mean white brightness in G = "<<mean_roi_green[0];
    qDebug()<<"mean white brightness in R = "<<mean_roi_red[0];

    double muladjB = 200/mean_roi_blue[0];
    double muladjG = 200/mean_roi_green[0];
    double muladjR = 200/mean_roi_red[0];

    qDebug()<<"Blue factor "<<muladjB;
    qDebug()<<"Green factor "<<muladjG;
    qDebug()<<"Red factor "<<muladjR;

    channel[0] *= muladjB;
    channel[1] *= muladjG;
    channel[2] *= muladjR;

    cv::merge(channel,3,standardised_RGB);

    log(channel[1]);
    log(channel[2]);

    cv::subtract(channel[2],channel[1],ei);
    //    cv::namedWindow("ei",CV_WINDOW_AUTOSIZE);
    //    if(!ei.empty())
    //            cv::imshow("ei", ei);
}

void ImageAnalysis::polyRegionOfInterest(QVariantList points) {
    cv::Mat mask = cv::Mat::zeros(ei.rows, ei.cols, CV_8UC1);
    std::vector<cv::Point> ROI_Vertices;
    for(int i=0; i < points.size(); ++i)
        ROI_Vertices.push_back(cv::Point(points[i].toPoint().x(),points[i].toPoint().y()));
    std::vector<cv::Point> ROI_Poly;
    cv::approxPolyDP(ROI_Vertices, ROI_Poly, 1.0, true);
    cv::fillConvexPoly(mask, &ROI_Poly[0], ROI_Poly.size(), 255, 8, 0);
    cv::Scalar tmp = cv::mean(ei,mask);
    erythemaIndex = tmp[0];
//    qDebug()<<"erythema index = "<<erythemaIndex;
    emit eIndexChanged();
//    cv::namedWindow("ei",CV_WINDOW_AUTOSIZE);
//    if(!palpebral.empty())
//        cv::imshow("ei", palpebral);
}

cv::Mat& ImageAnalysis::log(cv::Mat& I) {
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);

    int channels = I.channels();

    int nRows = I.rows;
    int nCols = I.cols * channels;

    if (I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i,j;
    uchar* p;
    for( i = 0; i < nRows; ++i)
    {
        p = I.ptr<uchar>(i);
        for ( j = 0; j < nCols; ++j)
        {
            //            std::cout<<(int)p[j]<<" ";
            p[j] = qLn((int)p[j])*255/qLn(255);
        }
    }
    return I;
}

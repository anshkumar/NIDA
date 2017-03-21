#ifndef IMAGEANALYSIS_H
#define IMAGEANALYSIS_H

#include <QObject>
#include <QUrl>
#include <QVariant>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImageAnalysis : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float eIndex READ eIndex NOTIFY eIndexChanged)
public:
    explicit ImageAnalysis(QObject *parent = 0);
    cv::Mat &log(cv::Mat &I);
    float eIndex();

signals:
    void eIndexChanged();

public slots:
    void split(QUrl url);
    void rectRegionOfInterest(int x, int y, int width, int height);
    void polyRegionOfInterest(QVariantList points);

private:
    cv::Mat input;
    cv::Mat channel[3];
    cv::Mat standardised_RGB;
    cv::Mat ei;
    float erythemaIndex;
};

#endif // IMAGEANALYSIS_H

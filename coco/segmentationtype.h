#ifndef SEGMENTATIONTYPE_H
#define SEGMENTATIONTYPE_H

#include <QObject>
#include <QVector>

class SegmentationType : public QObject
{
    Q_OBJECT

public:

    SegmentationType(QVector<float> list, int width, int height);

    QVector<float> getSegmentation();
    int getWidth();
    int getHeight();

private:

    QVector<float> segmentation;
    int width;
    int height;
};

#endif // SEGMENTATIONTYPE_H

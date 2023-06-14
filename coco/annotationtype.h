#ifndef ANNOTATIONTYPE_H
#define ANNOTATIONTYPE_H

#include <QObject>
#include <QVector>

#include "segmentationtype.h"

class AnnotationType : public QObject
{
    Q_OBJECT

public:

    AnnotationType(uint64_t id, uint64_t category_id, uint64_t image_id, int iscrowd, int area, QVector<float> bbox, QVector<SegmentationType *> segmentation);

    uint64_t getId();
    uint64_t getCategoryId();
    uint64_t getImageId();
    int getIsCrowd();
    int getArea();
    QVector<float> getBbox();
    QVector<SegmentationType* > getSegmentation();

private:

    uint64_t                id;
    uint64_t                category_id;
    uint64_t                image_id;
    int                     iscrowd;
    int                     area;
    QVector<float>          bbox;
    QVector<SegmentationType *> segmentation;

};

#endif // ANNOTATIONTYPE_H

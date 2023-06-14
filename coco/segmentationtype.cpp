#include "segmentationtype.h"

SegmentationType::SegmentationType(QVector<float> list, int width, int height)
{
    this->segmentation = list;
    this->width = width;
    this->height = height;
}

QVector<float> SegmentationType::getSegmentation()
{
    return segmentation;
}

int SegmentationType::getWidth()
{
    return width;
}

int SegmentationType::getHeight()
{
    return height;
}

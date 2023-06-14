#include "annotationtype.h"

AnnotationType::AnnotationType(uint64_t id, uint64_t category_id, uint64_t image_id, int iscrowd, int area, QVector<float> bbox, QVector<SegmentationType *> segmentation)
{
    this->id            = id;
    this->category_id   = category_id;
    this->image_id      = image_id;
    this->iscrowd       = iscrowd;
    this->area          = area;
    this->bbox          = bbox;
    this->segmentation  = segmentation;
}

uint64_t AnnotationType::getId()
{
    return id;
}

uint64_t AnnotationType::getCategoryId()
{
    return category_id;
}

uint64_t AnnotationType::getImageId()
{
    return image_id;
}

int AnnotationType::getIsCrowd()
{
    return iscrowd;
}

int AnnotationType::getArea()
{
    return area;
}

QVector<float> AnnotationType::getBbox()
{
    return bbox;
}

QVector<SegmentationType *> AnnotationType::getSegmentation()
{
    return segmentation;
}

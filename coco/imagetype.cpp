#include "imagetype.h"

ImageType::ImageType(int license, QString fileName, QString cocoUrl, int height, int width, QString dateCaptured, QString flickrUrl, uint64_t id)
{    
    this->license = license;
    this->fileName = fileName;
    this->cocoUrl = cocoUrl;
    this->height = height;
    this->width = width;
    this->dateCaptured = dateCaptured;
    this->flickrUrl = flickrUrl;
    this->id = id;
}

int ImageType::getLicense()
{
    return license;
}

QString ImageType::getFileName()
{
    return fileName;
}

QString ImageType::getCocoUrl()
{
    return cocoUrl;
}

int ImageType::getHeight()
{
    return height;
}

int ImageType::getWidth()
{
    return width;
}

QString ImageType::getDateCaptured()
{
    return dateCaptured;
}

QString ImageType::getFlickrUrl()
{
    return flickrUrl;
}

uint64_t ImageType::getId()
{
    return id;
}

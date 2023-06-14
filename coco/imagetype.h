#ifndef IMAGETYPE_H
#define IMAGETYPE_H

#include <QObject>

class ImageType : public QObject
{
    Q_OBJECT

public:

    ImageType(int license, QString fileName, QString cocoUrl, int height, int width, QString dateCaptured, QString flickrUrl, uint64_t id);

    int getLicense();
    QString getFileName();
    QString getCocoUrl();
    int getHeight();
    int getWidth();
    QString getDateCaptured();
    QString getFlickrUrl();
    uint64_t getId();

private:

    int license;
    QString fileName;
    QString cocoUrl;
    int height;
    int width;
    QString dateCaptured;
    QString flickrUrl;
    uint64_t id;
};

#endif // IMAGETYPE_H

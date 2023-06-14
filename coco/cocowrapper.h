#ifndef COCOWRAPPER_H
#define COCOWRAPPER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QVector>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "rapidjson/document.h"
#include "coco/imagetype.h"
#include "coco/annotationtype.h"
#include "coco/categoriestype.h"
#include "coco/segmentationtype.h"


class COCOWrapper : public QObject
{
    Q_OBJECT

public:

    COCOWrapper(QString json_file);

    void parseJson();

    QVector<uint64_t> getImgIds(QVector<uint64_t> categories_ids);
    QVector<ImageType *> loadImgs(QVector<uint64_t> image_ids);
    void downloadImgs(const QString &url, QString dest_path);
    QVector<AnnotationType *> getAnnotations(QVector<uint64_t> categories_ids);
    QString getCategoryNameFromId(uint32_t id);
    QStringList getAllCategories();

private:


    QString     json_file_path;
    QFile       * json_file;

    QMetaObject::Connection     connection_download;

    QVector<ImageType *>        images;
    QVector<AnnotationType *>   annotations;
    QVector<CategoriesType *>   categories;
};

#endif // COCOWRAPPER_H

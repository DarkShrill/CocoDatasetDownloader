#ifndef DATASETCREATOR_H
#define DATASETCREATOR_H

#include <QObject>
#include <QDirIterator>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QFuture>
#include "coco/cocowrapper.h"
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

class DatasetCreatorConnector : public QObject{
    Q_OBJECT

    Q_PROPERTY(NOTIFY processedImage)
    Q_PROPERTY(NOTIFY totalImages)
    Q_PROPERTY(NOTIFY jsonLoaded)

public:

    DatasetCreatorConnector();

signals:

    //DA QML A CPP
    void setDatasetJsonFile(QString file);
    void setDatasetFolder(QString folder);
    void setCategoryId(uint32_t cat);
    void setBatchSize(uint32_t batch);
    void startDownload();


    //DA CPP A QML
    void processedImage(uint64_t processed_images);
    void totalImages(uint64_t total_images);
    void jsonLoaded();
    void sendCategories(QStringList categories);
};

class DatasetCreator : public QObject
{
    Q_OBJECT

public:

    DatasetCreator();

public slots:

    void onSetDatasetJsonFile(QString file);
    void onSetDatasetFolder(QString folder);
    void onSetCategoryId(uint32_t cat);
    void onSetBatchSize(uint32_t batch);
    void onStartDownload();

signals:

    void processedImage(uint64_t processed_images);
    void totalImages(uint64_t total_images);
    void jsonLoaded();
    void sendCategories(QStringList categories);

private:

    void downloadImages(QVector<ImageType *> imgs, QString image_path);

    QString     json_file;
    QString     dataset_folder;
    uint32_t    cat_id;
    uint32_t    batch_size;

    COCOWrapper * coco;

    QThread     * thread;

};

#endif // DATASETCREATOR_H

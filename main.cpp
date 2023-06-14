#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include "radialbar.h"
#include "datasetcreator.h"


static DatasetCreator * dataset_creator = nullptr;
static DatasetCreatorConnector * dataset_creator_connector = nullptr;

static QObject * Get_DatasetCreator(QQmlEngine * engine, QJSEngine *scriptEngine)
{


    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);


    if(dataset_creator == nullptr){
        dataset_creator = new DatasetCreator();
    }
    if(dataset_creator_connector == nullptr){
        dataset_creator_connector = new DatasetCreatorConnector();
    }

    QObject::connect(dataset_creator_connector, &DatasetCreatorConnector::setDatasetJsonFile, dataset_creator, &DatasetCreator::onSetDatasetJsonFile);
    QObject::connect(dataset_creator_connector, &DatasetCreatorConnector::setDatasetFolder, dataset_creator, &DatasetCreator::onSetDatasetFolder);
    QObject::connect(dataset_creator_connector, &DatasetCreatorConnector::setCategoryId, dataset_creator, &DatasetCreator::onSetCategoryId);
    QObject::connect(dataset_creator_connector, &DatasetCreatorConnector::setBatchSize, dataset_creator, &DatasetCreator::onSetBatchSize);
    QObject::connect(dataset_creator_connector, &DatasetCreatorConnector::startDownload, dataset_creator, &DatasetCreator::onStartDownload);

    QObject::connect(dataset_creator, &DatasetCreator::processedImage, dataset_creator_connector, &DatasetCreatorConnector::processedImage);
    QObject::connect(dataset_creator, &DatasetCreator::totalImages, dataset_creator_connector, &DatasetCreatorConnector::totalImages);
    QObject::connect(dataset_creator, &DatasetCreator::jsonLoaded, dataset_creator_connector, &DatasetCreatorConnector::jsonLoaded);
    QObject::connect(dataset_creator, &DatasetCreator::sendCategories, dataset_creator_connector, &DatasetCreatorConnector::sendCategories);


    return dataset_creator_connector;
}



int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    app.setOrganizationName("DarkShrill");
    app.setOrganizationDomain("https://github.com/DarkShrill");


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    qmlRegisterType<RadialBar>("it.videx.progressbar", 1, 0, "CustomProgressBar");
    qmlRegisterSingletonType<DatasetCreatorConnector>("it.videx.dataset",1,0,"DatasetCreatorConnector",Get_DatasetCreator);
    qRegisterMetaType<uint32_t>("uint32_t");
    qRegisterMetaType<uint64_t>("uint64_t");

    engine.load(url);

    return app.exec();
}

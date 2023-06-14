#include "datasetcreator.h"

DatasetCreator::DatasetCreator()
{
    thread  = new QThread();

    this->moveToThread(this->thread);

    thread->start();
}

extern bool loadJson(COCOWrapper * coco){

    coco->parseJson();

    return true;
}

void DatasetCreator::onSetDatasetJsonFile(QString file)
{
    this->json_file = file;
    coco = new COCOWrapper(json_file);

    QFuture<bool> res = QtConcurrent::run(loadJson, coco);
    if(res.result() == true){
        emit jsonLoaded();
        emit sendCategories(coco->getAllCategories());
    }
}

void DatasetCreator::onSetDatasetFolder(QString folder)
{
    this->dataset_folder = folder;
}

void DatasetCreator::onSetCategoryId(uint32_t cat)
{
    this->cat_id = cat;
}

void DatasetCreator::onSetBatchSize(uint32_t batch)
{
    this->batch_size = batch;
}

void DatasetCreator::onStartDownload()
{

    uint8_t CATEGORY_ID = this->cat_id;

    //"C:\\Users\\e.papa\\Downloads\\COCO-YOLO-Parser-main\\COCO-YOLO-Parser-main\\annotations\\instances_val2017.json"


    QVector<uint64_t> img_ids               = coco->getImgIds(QVector<uint64_t>() << CATEGORY_ID);
    QVector<AnnotationType *> annotations   = coco->getAnnotations(QVector<uint64_t>() << CATEGORY_ID);
    QVector<ImageType *> images             = coco->loadImgs(img_ids);


    uint32_t total_images                   = images.length();
    uint32_t processed_image                = 0;

    emit totalImages(total_images);
    qDebug() << "TOTAL IMAGE: " << total_images;


    QString generic_path                    = this->dataset_folder;
    QString image_path                      = generic_path + "\\images";
    QString label_path                      = generic_path + "\\label";



    if(QDir(image_path).exists() == false){
        QDir().mkdir(image_path);
    }

    if(QDir(label_path).exists() == false){
        QDir().mkdir(label_path);
    }

    QFile * file = new QFile();
    file->setFileName(generic_path + "\\obj.names");
    file->open(QIODevice::ReadWrite);


    file->write(coco->getCategoryNameFromId(CATEGORY_ID).toUtf8());

    file->close();
    delete file;







    for(int i = 0; i < images.length(); i+= batch_size){

        QVector<ImageType *> imgs;
        for(int j = 0; j < batch_size; j++){
//            qDebug() << "j + (10*i): " << j + (i);
            if(images.length() - processed_image < batch_size){

                for(int k = 0; k < (images.length() - processed_image); k++){
                    imgs.append(images.operator[](j + (i) + k));
                }

                break;
            }
            imgs.append(images.operator[](j + (i)));
        }


        //qDebug() << "Processing image ID: " << image->getId();
        if(images.length() - processed_image < batch_size)
            processed_image+= (images.length() - processed_image);
        else
            processed_image+=batch_size;

        downloadImages(imgs, image_path);

        for(int j = 0; j < imgs.length(); j++){

            QString labelFileName = label_path + "/" + imgs.operator[](j)->getFileName().mid(0, imgs.operator[](j)->getFileName().length() - 4) + ".txt";

            file = new QFile();
            file->setFileName(labelFileName);
            file->open(QIODevice::ReadWrite);

            for(AnnotationType * anno : annotations){

                if(anno->getImageId() != imgs.operator[](j)->getId())
                    continue;

                double centerX  = (anno->getBbox()[0] + anno->getBbox()[2] / 2) / imgs.operator[](j)->getWidth();
                double centerY  = (anno->getBbox()[1] + anno->getBbox()[3] / 2) / imgs.operator[](j)->getHeight();
                double width    =  anno->getBbox()[2] / imgs.operator[](j)->getWidth();
                double height   =  anno->getBbox()[3] / imgs.operator[](j)->getHeight();

                QString data;

                data = QString::number(anno->getCategoryId()) + " " + QString::number(centerX) + " " + QString::number(centerY) + " " + QString::number(width) + " " + QString::number(height) + "\n";

                file->write(data.toUtf8().data(), data.length());
            }

            file->close();
            delete file;
        }


        emit processedImage((((float)((float)processed_image)/((float)total_images))*100));
        qDebug() << (((float)((float)processed_image)/((float)total_images))*100) << "%";

    }


//FUNZIONA SOTTO
/*
    for(const auto& image : images){
        qDebug() << "Processing image ID: " << image->getId();
        processed_image++;

        QString imgPath = image_path + "\\" + image->getFileName();
        coco->downloadImgs(image->getCocoUrl(), imgPath);
        QString labelFileName = label_path + "/" + image->getFileName().mid(0, image->getFileName().length() - 4) + ".txt";

        file = new QFile();
        file->setFileName(labelFileName);
        file->open(QIODevice::ReadWrite);

        for(AnnotationType * anno : annotations){

            double centerX  = (anno->getBbox()[0] + anno->getBbox()[2] / 2) / image->getWidth();
            double centerY  = (anno->getBbox()[1] + anno->getBbox()[3] / 2) / image->getHeight();
            double width    =  anno->getBbox()[2] / image->getWidth();
            double height   =  anno->getBbox()[3] / image->getHeight();

            QString data;

            data = QString::number(CATEGORY_ID) + " " + QString::number(centerX) + " " + QString::number(centerY) + " " + QString::number(width) + " " + QString::number(height) + "\n";

            file->write(data.toUtf8().data(), data.length());
        }

        file->close();
        delete file;

        qDebug() << (((float)((float)processed_image)/((float)total_images))*100) << "%";
    }
*/

    //CREATE TRAIN FILE

    QVector<QString> filenames;

    QDirIterator it(image_path, QStringList() << "*.jpg", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()){
       filenames.append(it.next());
    }

    file = new QFile();
    file->setFileName(generic_path + "\\train.txt");
    file->open(QIODevice::ReadWrite);

    for (const auto& filename : filenames){

        file->write(QString("data/obj/" + QUrl(filename).fileName() + "\n").toUtf8());
    }

    file->close();
    delete file;
}

bool downloadImage(const QString &url, const QString &image_save_path)
{
    QFile                   * file      = new QFile();
    QNetworkAccessManager   * manager   = new QNetworkAccessManager();
    QNetworkRequest         request;
    QEventLoop              eventLoop;


    QString imgPath     = image_save_path + "\\" + QUrl(url).fileName();

    file->setFileName(imgPath);
    if(file->open(QIODevice::ReadWrite | QIODevice::Truncate)){

    }else{
        qDebug() << "FILE NON APERTO";
    }


    request.setUrl(QUrl(url));



    QObject::connect(manager, &QNetworkAccessManager::finished, [&](QNetworkReply * reply){

        eventLoop.quit();

        if (reply->error() == QNetworkReply::NoError) {
            file->write(reply->readAll());
            file->close();
        }

        delete file;
        delete manager;
    });


    manager->get(request);
    eventLoop.exec();

    return true;
}

void DatasetCreator::downloadImages(QVector<ImageType *> imgs, QString image_path){

    QVector<QFuture<bool> > vect;

    for(int i = 0; i < imgs.length(); i++){
        vect.append(QtConcurrent::run(downloadImage, imgs.at(i)->getCocoUrl(), image_path));
    }

    for(int i = 0; i < imgs.length(); i++){
        vect.at(i).result();
    }

}


DatasetCreatorConnector::DatasetCreatorConnector()
{

}

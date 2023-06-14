#include "cocowrapper.h"

COCOWrapper::COCOWrapper(QString json_file)
{
    this->json_file_path = json_file;
}

void COCOWrapper::parseJson()
{
    rapidjson::Document parser;
    rapidjson::Value::ConstValueIterator itr;

    json_file       = new QFile(json_file_path);
    if(json_file->open(QIODevice::ReadOnly) == false){
        qDebug() << "ERRORE LETTURA JSON";
        return;
    }

    QByteArray raw_data = json_file->readAll();

    json_file->close();
    delete json_file;




    parser.Parse<0>(raw_data.data());


    auto &imagesArr         = parser["images"];
    auto &annotationsArr    = parser["annotations"];
    auto &categoriesArr     = parser["categories"];





    QString description         = parser["info"]["description"].GetString();
    QString url                 = parser["info"]["url"].GetString();
    QString version             = parser["info"]["version"].GetString();
    int year                    = parser["info"]["year"].GetInt64();
    QString contributor         = parser["info"]["contributor"].GetString();
    QString dateCreated         = parser["info"]["date_created"].GetString();



    for (itr = imagesArr.Begin(); itr != imagesArr.End(); ++itr) {

        int license             = itr->GetObject()["license"].GetInt64();
        QString fileName        = itr->GetObject()["file_name"].GetString();
        QString cocoUrl         = itr->GetObject()["coco_url"].GetString();
        int height              = itr->GetObject()["height"].GetInt64();
        int width               = itr->GetObject()["width"].GetInt64();
        QString dateCaptured    = itr->GetObject()["date_captured"].GetString();
        QString flickrUrl       = itr->GetObject()["flickr_url"].GetString();
        int id                  = itr->GetObject()["id"].GetInt64();


        images.append(new ImageType(license,
                                    fileName,
                                    cocoUrl,
                                    height,
                                    width,
                                    dateCaptured,
                                    flickrUrl,
                                    id));
    }







    for (itr = annotationsArr.Begin(); itr != annotationsArr.End(); ++itr) {

        double area                     = itr->GetObject()["area"].GetDouble();
        int iscrowd                     = itr->GetObject()["iscrowd"].GetInt64();
        auto &bboxArr                   = itr->GetObject()["bbox"];
        const rapidjson::Value &bseg    = itr->GetObject()["segmentation"];

        double x;
        double y;
        double width;
        double height;

        QVector<float>              bbox;
        QVector<SegmentationType *> segmentation;




        uint64_t imageId        = itr->GetObject()["image_id"].GetInt64();
        uint64_t categoryId     = itr->GetObject()["category_id"].GetInt64();
        uint64_t id             = itr->GetObject()["id"].GetInt64();



        for (rapidjson::Value::ConstValueIterator itr2 = bboxArr.Begin(); itr2 != bboxArr.End(); ++itr2) {
            bbox.append(itr2->GetDouble());
        }

        x       = bbox.operator[](0);
        y       = bbox.operator[](1);
        width   = bbox.operator[](2);
        height  = bbox.operator[](3);


        //CHECK

        if (bseg.IsObject()){


            QVector<float> vect;
            QVector<float> vect_size;


            auto countsIterator = bseg.FindMember("counts");
            if (countsIterator != bseg.MemberEnd() && countsIterator->value.IsArray()) {
                const rapidjson::Value& counts = countsIterator->value;

                for (rapidjson::SizeType j = 0; j < counts.Size(); j++) {
                    vect.append(counts[j].GetDouble());
                }
            }


            auto sizeIterator = bseg.FindMember("size");
            if (sizeIterator != bseg.MemberEnd() && sizeIterator->value.IsArray()) {
                const rapidjson::Value& size = sizeIterator->value;

                for (rapidjson::SizeType j = 0; j < size.Size(); j++) {
                    vect_size.append(size[j].GetDouble());
                }
            }

            SegmentationType * seg_type = new SegmentationType(vect, vect_size.operator[](0), vect_size.operator[](1));
            segmentation.append(seg_type);

        }else if(bseg.IsArray()){

            for (rapidjson::Value::ConstValueIterator itr2 = bseg.Begin(); itr2 != bseg.End(); ++itr2) {
                auto arr = itr2->GetArray();
                QVector<float> vect;

                for (rapidjson::Value::ConstValueIterator itr3 = arr.Begin(); itr3 != arr.End(); ++itr3) {
                    vect.append(itr3->GetDouble());
                }

                SegmentationType * seg_type = new SegmentationType(vect, 0, 0);
                segmentation.append(seg_type);
            }
        }




        annotations.append(new AnnotationType(id,
                                              categoryId,
                                              imageId,
                                              iscrowd,
                                              area,
                                              bbox,
                                              segmentation));

    }





    for (rapidjson::Value::ConstValueIterator itr = categoriesArr.Begin(); itr != categoriesArr.End(); ++itr) {

        QString super_category  = itr->GetObject()["supercategory"].GetString();
        QString name            = itr->GetObject()["name"].GetString();
        uint64_t id             = itr->GetObject()["id"].GetInt64();


        categories.append(new CategoriesType(id,
                                             name,
                                             super_category));

    }
}

QVector<uint64_t> COCOWrapper::getImgIds(QVector<uint64_t> categories_ids)
{
    QVector<uint64_t> images_ids;

    for (AnnotationType * annotation : annotations){

        uint64_t categoryId = annotation->getCategoryId();
        if (std::find(categories_ids.begin(), categories_ids.end(), categoryId) != categories_ids.end()){

            uint64_t imageId = annotation->getImageId();
            images_ids.append(imageId);
        }
    }

    return images_ids;
}

QVector<ImageType *> COCOWrapper::loadImgs(QVector<uint64_t> image_ids)
{
    QVector<ImageType *> images_data;

    for (ImageType * image : images){

        uint64_t image_id = image->getId();
        if (std::find(image_ids.begin(), image_ids.end(), image_id) != image_ids.end()){

            images_data.append(image);
        }
    }

    return images_data;
}

void COCOWrapper::downloadImgs(const QString &url, QString dest_path)
{
    QFile                   * file      = new QFile();
    QNetworkAccessManager   * manager   = new QNetworkAccessManager();
    QNetworkRequest         request;
    QEventLoop              eventLoop;




    file->setFileName(dest_path);
    if(file->open(QIODevice::ReadWrite | QIODevice::Truncate)){

    }else{
        qDebug() << "FILE NON APERTO";
    }


    request.setUrl(QUrl(url));



    connection_download = QObject::connect(manager, &QNetworkAccessManager::finished, [&](QNetworkReply * reply){

        eventLoop.quit();

        if (reply->error() == QNetworkReply::NoError) {
            file->write(reply->readAll());
            file->close();
        }

        delete file;
        delete manager;

        disconnect(connection_download);
    });


    manager->get(request);
    eventLoop.exec();
}

QVector<AnnotationType *> COCOWrapper::getAnnotations(QVector<uint64_t> categories_ids)
{
    QVector<AnnotationType *> annotation_data;

    for (AnnotationType * anno : annotations){

        uint64_t cat_id = anno->getCategoryId();
        if (std::find(categories_ids.begin(), categories_ids.end(), cat_id) != categories_ids.end()){

            annotation_data.append(anno);
        }
    }

    return annotation_data;
}

QString COCOWrapper::getCategoryNameFromId(uint32_t id)
{
    for(CategoriesType * cat : categories){
        if(cat->getId() == id){
            return cat->getCategory();
        }
    }
}

QStringList COCOWrapper::getAllCategories()
{
    QStringList list;

    list.append("");

    for(CategoriesType * cat : categories){
        list.append(cat->getCategory().toUpper());
    }
    return list;
}

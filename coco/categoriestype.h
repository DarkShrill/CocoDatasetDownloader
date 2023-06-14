#ifndef CATEGORIESTYPE_H
#define CATEGORIESTYPE_H

#include <QObject>

class CategoriesType : public QObject
{
    Q_OBJECT

public:

    CategoriesType(uint64_t id, QString category, QString superCategory);

    uint64_t getId();
    QString getCategory();
    QString getSuper_category();

private:

    uint64_t     id;
    QString category;
    QString super_category;
};

#endif // CATEGORIESTYPE_H

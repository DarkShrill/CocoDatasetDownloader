#include "categoriestype.h"

CategoriesType::CategoriesType(uint64_t id, QString category, QString superCategory)
{
    this->id                = id;
    this->category          = category;
    this->super_category    = superCategory;
}

uint64_t CategoriesType::getId()
{
    return id;
}

QString CategoriesType::getCategory()
{
    return category;
}

QString CategoriesType::getSuper_category()
{
    return super_category;
}

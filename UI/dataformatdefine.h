#ifndef DATAFORMATDEFINE_H
#define DATAFORMATDEFINE_H

#include <QString>


typedef struct item_
{
    QString name;
    QString strValue;
    int value;
    QVector<item_> childrens;
}itemData;

#endif // DATAFORMATDEFINE_H

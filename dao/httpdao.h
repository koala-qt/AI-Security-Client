#ifndef HTTPDAO_H
#define HTTPDAO_H

#include "core/httpexector.h"
#include <QDateTime>
namespace DLL{
class HttpDao : public HttpExector
{
public:
    HttpDao();
    int searchMeiaUrl(int, QString &);
    int getAllDevices(QList<SecrityDevice> &);
    int searchWaring(HistoryWaringArgs &args, HistoryWaring &resWaring, QString &error);
    int searchWaringRect(int cameraId,QVector<QPolygon> &polyons,QString &error);
};
}

#endif // HTTPDAO_H

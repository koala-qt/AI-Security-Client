#include "portraitsearch.h"

PortraitSearch::PortraitSearch(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Portrait search"));

    setUserStyle(userStyle());
}

void PortraitSearch::setUserStyle(int s)
{

}

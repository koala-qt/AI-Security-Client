#include <QWebEngineView>
#include <QHBoxLayout>
#include <QApplication>
#include "combinationpage.h"

CombinationPage::CombinationPage(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Semantic search"));
    QHBoxLayout *mainLay = new QHBoxLayout;
    contentView_ = new QWebEngineView;
    mainLay->addWidget(contentView_);
    setLayout(mainLay);

    contentView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/index.html"));
    contentView_->page()->setBackgroundColor(Qt::transparent);
}

void CombinationPage::setUserStyle(WidgetManagerI::SkinStyle s)
{

}

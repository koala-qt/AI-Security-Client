#include "trajectorysearch.h"
#include "baidumap.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLabel>

#pragma execution_character_set("utf-8")
TrajectorySearch::TrajectorySearch(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("轨迹检索"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    m_startPosL = new QLabel(tr("起点位置"));
    m_startPosBox = new QComboBox;
    m_endPosL = new QLabel(tr("终点位置"));
    m_endPosBox = new QComboBox;
    m_startTimeL = new QLabel(tr("开始时间"));
    m_startTimeEdit = new QDateTimeEdit;
    m_endTimeL = new QLabel(tr("结束时间"));
    m_endTimeEdit = new QDateTimeEdit;
    m_searchBtn = new QPushButton(tr("检索"));
    hlay->addWidget(m_startPosL);
    hlay->addWidget(m_startPosBox);
    hlay->addStretch();
    hlay->addWidget(m_endPosL);
    hlay->addWidget(m_endPosBox);
    hlay->addStretch();
    hlay->addWidget(m_startTimeL);
    hlay->addWidget(m_startTimeEdit);
    hlay->addStretch();
    hlay->addWidget(m_endTimeL);
    hlay->addWidget(m_endTimeEdit);
    hlay->addStretch();
    hlay->addWidget(m_searchBtn);
    hlay->addStretch();
    mainLay->addLayout(hlay);
    m_mapW = new BaiduMap;
    mainLay->addWidget(m_mapW);
    mainLay->setMargin(0);
    setLayout(mainLay);

    connect(m_searchBtn,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
}

void TrajectorySearch::setUserStyle(WidgetManagerI::SkinStyle s)
{
    if(s == WidgetManagerI::Danyahei){
    }
}

void TrajectorySearch::slotSearchBtnClicked()
{
    static bool first = true;
    QVector<QPair<float,float>> points;
    m_mapW->slotClearTrack();
    if(first){
        points << qMakePair(104.86513,30.542262) << qMakePair(104.796513,30.582262) << qMakePair(104.646513,30.702262) << qMakePair(104.696513,30.482262);
        first = false;
    }else{
        points << qMakePair(104.116513,30.592262) << qMakePair(104.236513,30.602262) << qMakePair(104.566513,30.722262) << qMakePair(104.786513,30.522262);
    }
    m_mapW->slotDrawTrack(points);
}

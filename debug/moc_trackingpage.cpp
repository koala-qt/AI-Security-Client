/****************************************************************************
** Meta object code from reading C++ file 'trackingpage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/trackingpage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trackingpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TrackingPage_t {
    QByteArrayData data[12];
    char stringdata0[230];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrackingPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrackingPage_t qt_meta_stringdata_TrackingPage = {
    {
QT_MOC_LITERAL(0, 0, 12), // "TrackingPage"
QT_MOC_LITERAL(1, 13, 17), // "slotImgBtnClicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 20), // "slotSearchBtnClicked"
QT_MOC_LITERAL(4, 53, 16), // "slotOnCameraInfo"
QT_MOC_LITERAL(5, 70, 33), // "QVector<RestServiceI::CameraI..."
QT_MOC_LITERAL(6, 104, 15), // "slotTrackingNew"
QT_MOC_LITERAL(7, 120, 41), // "QVector<RestServiceI::Trackin..."
QT_MOC_LITERAL(8, 162, 12), // "slotTracking"
QT_MOC_LITERAL(9, 175, 19), // "QVector<SearchFace>"
QT_MOC_LITERAL(10, 195, 19), // "slotOnCameraClicked"
QT_MOC_LITERAL(11, 215, 14) // "slotOnWebError"

    },
    "TrackingPage\0slotImgBtnClicked\0\0"
    "slotSearchBtnClicked\0slotOnCameraInfo\0"
    "QVector<RestServiceI::CameraInfo>\0"
    "slotTrackingNew\0"
    "QVector<RestServiceI::TrackingReturnData>\0"
    "slotTracking\0QVector<SearchFace>\0"
    "slotOnCameraClicked\0slotOnWebError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrackingPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    1,   51,    2, 0x08 /* Private */,
       6,    1,   54,    2, 0x08 /* Private */,
       8,    1,   57,    2, 0x08 /* Private */,
      10,    1,   60,    2, 0x08 /* Private */,
      11,    1,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void TrackingPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrackingPage *_t = static_cast<TrackingPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotImgBtnClicked(); break;
        case 1: _t->slotSearchBtnClicked(); break;
        case 2: _t->slotOnCameraInfo((*reinterpret_cast< QVector<RestServiceI::CameraInfo>(*)>(_a[1]))); break;
        case 3: _t->slotTrackingNew((*reinterpret_cast< QVector<RestServiceI::TrackingReturnData>(*)>(_a[1]))); break;
        case 4: _t->slotTracking((*reinterpret_cast< QVector<SearchFace>(*)>(_a[1]))); break;
        case 5: _t->slotOnCameraClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->slotOnWebError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject TrackingPage::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_TrackingPage.data,
      qt_meta_data_TrackingPage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TrackingPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrackingPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TrackingPage.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int TrackingPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WidgetI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

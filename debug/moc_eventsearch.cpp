/****************************************************************************
** Meta object code from reading C++ file 'eventsearch.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/eventsearch.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eventsearch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EventSearch_t {
    QByteArrayData data[11];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EventSearch_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EventSearch_t qt_meta_stringdata_EventSearch = {
    {
QT_MOC_LITERAL(0, 0, 11), // "EventSearch"
QT_MOC_LITERAL(1, 12, 16), // "slotOnCameraInfo"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 33), // "QVector<RestServiceI::CameraI..."
QT_MOC_LITERAL(4, 64, 20), // "slotSearchBtnClicked"
QT_MOC_LITERAL(5, 85, 16), // "slotAlarmHistory"
QT_MOC_LITERAL(6, 102, 31), // "RestServiceI::EventSearchReturn"
QT_MOC_LITERAL(7, 134, 26), // "slotSearchPageAlarmHistory"
QT_MOC_LITERAL(8, 161, 4), // "page"
QT_MOC_LITERAL(9, 166, 15), // "slotOnSceneInfo"
QT_MOC_LITERAL(10, 182, 23) // "RestServiceI::SceneInfo"

    },
    "EventSearch\0slotOnCameraInfo\0\0"
    "QVector<RestServiceI::CameraInfo>\0"
    "slotSearchBtnClicked\0slotAlarmHistory\0"
    "RestServiceI::EventSearchReturn\0"
    "slotSearchPageAlarmHistory\0page\0"
    "slotOnSceneInfo\0RestServiceI::SceneInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EventSearch[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    1,   43,    2, 0x08 /* Private */,
       7,    1,   46,    2, 0x08 /* Private */,
       9,    1,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, 0x80000000 | 10,    2,

       0        // eod
};

void EventSearch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EventSearch *_t = static_cast<EventSearch *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotOnCameraInfo((*reinterpret_cast< QVector<RestServiceI::CameraInfo>(*)>(_a[1]))); break;
        case 1: _t->slotSearchBtnClicked(); break;
        case 2: _t->slotAlarmHistory((*reinterpret_cast< RestServiceI::EventSearchReturn(*)>(_a[1]))); break;
        case 3: _t->slotSearchPageAlarmHistory((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotOnSceneInfo((*reinterpret_cast< RestServiceI::SceneInfo(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject EventSearch::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_EventSearch.data,
      qt_meta_data_EventSearch,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *EventSearch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EventSearch::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EventSearch.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int EventSearch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WidgetI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

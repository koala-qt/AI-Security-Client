/****************************************************************************
** Meta object code from reading C++ file 'baidumap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/baidumap.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'baidumap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_bridge_t {
    QByteArrayData data[6];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_bridge_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_bridge_t qt_meta_stringdata_bridge = {
    {
QT_MOC_LITERAL(0, 0, 6), // "bridge"
QT_MOC_LITERAL(1, 7, 11), // "sigYourName"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 4), // "name"
QT_MOC_LITERAL(4, 25, 10), // "showMsgBox"
QT_MOC_LITERAL(5, 36, 11) // "getYourName"

    },
    "bridge\0sigYourName\0\0name\0showMsgBox\0"
    "getYourName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_bridge[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   32,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void bridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        bridge *_t = static_cast<bridge *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigYourName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->showMsgBox(); break;
        case 2: _t->getYourName(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (bridge::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&bridge::sigYourName)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject bridge::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_bridge.data,
      qt_meta_data_bridge,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *bridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *bridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_bridge.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int bridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void bridge::sigYourName(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_BaiduMap_t {
    QByteArrayData data[11];
    char stringdata0[142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaiduMap_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaiduMap_t qt_meta_stringdata_BaiduMap = {
    {
QT_MOC_LITERAL(0, 0, 8), // "BaiduMap"
QT_MOC_LITERAL(1, 9, 15), // "slotGetCityName"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "slotGetWebWidth"
QT_MOC_LITERAL(4, 42, 15), // "searchLocaltion"
QT_MOC_LITERAL(5, 58, 9), // "longitude"
QT_MOC_LITERAL(6, 68, 8), // "latitude"
QT_MOC_LITERAL(7, 77, 13), // "slotDrawTrack"
QT_MOC_LITERAL(8, 91, 28), // "QVector<QPair<float,float> >"
QT_MOC_LITERAL(9, 120, 6), // "points"
QT_MOC_LITERAL(10, 127, 14) // "slotClearTrack"

    },
    "BaiduMap\0slotGetCityName\0\0slotGetWebWidth\0"
    "searchLocaltion\0longitude\0latitude\0"
    "slotDrawTrack\0QVector<QPair<float,float> >\0"
    "points\0slotClearTrack"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaiduMap[] = {

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
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    2,   41,    2, 0x0a /* Public */,
       7,    1,   46,    2, 0x0a /* Public */,
      10,    0,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,

       0        // eod
};

void BaiduMap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaiduMap *_t = static_cast<BaiduMap *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotGetCityName(); break;
        case 1: _t->slotGetWebWidth(); break;
        case 2: _t->searchLocaltion((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->slotDrawTrack((*reinterpret_cast< QVector<QPair<float,float> >(*)>(_a[1]))); break;
        case 4: _t->slotClearTrack(); break;
        default: ;
        }
    }
}

const QMetaObject BaiduMap::staticMetaObject = {
    { &QWebEngineView::staticMetaObject, qt_meta_stringdata_BaiduMap.data,
      qt_meta_data_BaiduMap,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BaiduMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaiduMap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BaiduMap.stringdata0))
        return static_cast<void*>(this);
    return QWebEngineView::qt_metacast(_clname);
}

int BaiduMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebEngineView::qt_metacall(_c, _id, _a);
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

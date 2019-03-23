/****************************************************************************
** Meta object code from reading C++ file 'treecharts.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/treecharts.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'treecharts.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TreeBridge_t {
    QByteArrayData data[6];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TreeBridge_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TreeBridge_t qt_meta_stringdata_TreeBridge = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TreeBridge"
QT_MOC_LITERAL(1, 11, 14), // "sigDataChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 15), // "sigStartWaiting"
QT_MOC_LITERAL(4, 43, 14), // "sigStopWaiting"
QT_MOC_LITERAL(5, 58, 11) // "onInitsized"

    },
    "TreeBridge\0sigDataChanged\0\0sigStartWaiting\0"
    "sigStopWaiting\0onInitsized"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TreeBridge[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       3,    0,   37,    2, 0x06 /* Public */,
       4,    0,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void TreeBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TreeBridge *_t = static_cast<TreeBridge *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigDataChanged((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 1: _t->sigStartWaiting(); break;
        case 2: _t->sigStopWaiting(); break;
        case 3: _t->onInitsized(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (TreeBridge::*_t)(QJsonObject );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TreeBridge::sigDataChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TreeBridge::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TreeBridge::sigStartWaiting)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TreeBridge::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TreeBridge::sigStopWaiting)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject TreeBridge::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TreeBridge.data,
      qt_meta_data_TreeBridge,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TreeBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TreeBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TreeBridge.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TreeBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TreeBridge::sigDataChanged(QJsonObject _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TreeBridge::sigStartWaiting()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TreeBridge::sigStopWaiting()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_TreeCharts_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TreeCharts_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TreeCharts_t qt_meta_stringdata_TreeCharts = {
    {
QT_MOC_LITERAL(0, 0, 10) // "TreeCharts"

    },
    "TreeCharts"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TreeCharts[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void TreeCharts::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject TreeCharts::staticMetaObject = {
    { &QWebEngineView::staticMetaObject, qt_meta_stringdata_TreeCharts.data,
      qt_meta_data_TreeCharts,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TreeCharts::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TreeCharts::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TreeCharts.stringdata0))
        return static_cast<void*>(this);
    return QWebEngineView::qt_metacast(_clname);
}

int TreeCharts::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebEngineView::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'trackingwebview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/trackingwebview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trackingwebview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TrackingBridge_t {
    QByteArrayData data[12];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrackingBridge_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrackingBridge_t qt_meta_stringdata_TrackingBridge = {
    {
QT_MOC_LITERAL(0, 0, 14), // "TrackingBridge"
QT_MOC_LITERAL(1, 15, 18), // "sigHostNameChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 22), // "sigTrackingDataChanged"
QT_MOC_LITERAL(4, 58, 16), // "sigMovieingStart"
QT_MOC_LITERAL(5, 75, 12), // "sigMovieStop"
QT_MOC_LITERAL(6, 88, 16), // "sigCameraClicked"
QT_MOC_LITERAL(7, 105, 11), // "sigWebError"
QT_MOC_LITERAL(8, 117, 11), // "onInitsized"
QT_MOC_LITERAL(9, 129, 15), // "onCameraClicked"
QT_MOC_LITERAL(10, 145, 8), // "cameraId"
QT_MOC_LITERAL(11, 154, 12) // "alertNoPoint"

    },
    "TrackingBridge\0sigHostNameChanged\0\0"
    "sigTrackingDataChanged\0sigMovieingStart\0"
    "sigMovieStop\0sigCameraClicked\0sigWebError\0"
    "onInitsized\0onCameraClicked\0cameraId\0"
    "alertNoPoint"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrackingBridge[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       3,    1,   62,    2, 0x06 /* Public */,
       4,    0,   65,    2, 0x06 /* Public */,
       5,    0,   66,    2, 0x06 /* Public */,
       6,    1,   67,    2, 0x06 /* Public */,
       7,    1,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   73,    2, 0x0a /* Public */,
       9,    1,   74,    2, 0x0a /* Public */,
      11,    1,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QJsonArray,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void TrackingBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrackingBridge *_t = static_cast<TrackingBridge *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigHostNameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sigTrackingDataChanged((*reinterpret_cast< QJsonArray(*)>(_a[1]))); break;
        case 2: _t->sigMovieingStart(); break;
        case 3: _t->sigMovieStop(); break;
        case 4: _t->sigCameraClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->sigWebError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->onInitsized(); break;
        case 7: _t->onCameraClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->alertNoPoint((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (TrackingBridge::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackingBridge::sigHostNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TrackingBridge::*_t)(QJsonArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackingBridge::sigTrackingDataChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TrackingBridge::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackingBridge::sigMovieingStart)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TrackingBridge::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackingBridge::sigMovieStop)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (TrackingBridge::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackingBridge::sigCameraClicked)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (TrackingBridge::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackingBridge::sigWebError)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject TrackingBridge::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TrackingBridge.data,
      qt_meta_data_TrackingBridge,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TrackingBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrackingBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TrackingBridge.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TrackingBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TrackingBridge::sigHostNameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TrackingBridge::sigTrackingDataChanged(QJsonArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TrackingBridge::sigMovieingStart()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TrackingBridge::sigMovieStop()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TrackingBridge::sigCameraClicked(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TrackingBridge::sigWebError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
struct qt_meta_stringdata_TrackingWebView_t {
    QByteArrayData data[4];
    char stringdata0[46];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrackingWebView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrackingWebView_t qt_meta_stringdata_TrackingWebView = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TrackingWebView"
QT_MOC_LITERAL(1, 16, 16), // "sigCameraClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 11) // "sigWebError"

    },
    "TrackingWebView\0sigCameraClicked\0\0"
    "sigWebError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrackingWebView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       3,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void TrackingWebView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrackingWebView *_t = static_cast<TrackingWebView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigCameraClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sigWebError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (TrackingWebView::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackingWebView::sigCameraClicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TrackingWebView::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackingWebView::sigWebError)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TrackingWebView::staticMetaObject = {
    { &QWebEngineView::staticMetaObject, qt_meta_stringdata_TrackingWebView.data,
      qt_meta_data_TrackingWebView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TrackingWebView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrackingWebView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TrackingWebView.stringdata0))
        return static_cast<void*>(this);
    return QWebEngineView::qt_metacast(_clname);
}

int TrackingWebView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebEngineView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void TrackingWebView::sigCameraClicked(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TrackingWebView::sigWebError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'videoplayback.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/videoplayback.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videoplayback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoPlayback_t {
    QByteArrayData data[9];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoPlayback_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoPlayback_t qt_meta_stringdata_VideoPlayback = {
    {
QT_MOC_LITERAL(0, 0, 13), // "VideoPlayback"
QT_MOC_LITERAL(1, 14, 15), // "slotItemClicked"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(4, 48, 13), // "slotAddDevice"
QT_MOC_LITERAL(5, 62, 33), // "QVector<RestServiceI::CameraI..."
QT_MOC_LITERAL(6, 96, 26), // "slotTableItemDoubleClicked"
QT_MOC_LITERAL(7, 123, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(8, 141, 15) // "slotUploadTable"

    },
    "VideoPlayback\0slotItemClicked\0\0"
    "QTreeWidgetItem*\0slotAddDevice\0"
    "QVector<RestServiceI::CameraInfo>\0"
    "slotTableItemDoubleClicked\0QTableWidgetItem*\0"
    "slotUploadTable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoPlayback[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x08 /* Private */,
       4,    1,   39,    2, 0x08 /* Private */,
       6,    1,   42,    2, 0x08 /* Private */,
       8,    1,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, QMetaType::QDate,    2,

       0        // eod
};

void VideoPlayback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoPlayback *_t = static_cast<VideoPlayback *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotItemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->slotAddDevice((*reinterpret_cast< QVector<RestServiceI::CameraInfo>(*)>(_a[1]))); break;
        case 2: _t->slotTableItemDoubleClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->slotUploadTable((*reinterpret_cast< QDate(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject VideoPlayback::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_VideoPlayback.data,
      qt_meta_data_VideoPlayback,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VideoPlayback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoPlayback::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoPlayback.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int VideoPlayback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WidgetI::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE

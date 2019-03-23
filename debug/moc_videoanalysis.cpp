/****************************************************************************
** Meta object code from reading C++ file 'videoanalysis.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/videoanalysis.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videoanalysis.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoAnalysis_t {
    QByteArrayData data[6];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoAnalysis_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoAnalysis_t qt_meta_stringdata_VideoAnalysis = {
    {
QT_MOC_LITERAL(0, 0, 13), // "VideoAnalysis"
QT_MOC_LITERAL(1, 14, 16), // "slotFileSelected"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16), // "slotCancelUPload"
QT_MOC_LITERAL(4, 49, 22), // "slotCurrenIndexChanged"
QT_MOC_LITERAL(5, 72, 15) // "slotBackClicked"

    },
    "VideoAnalysis\0slotFileSelected\0\0"
    "slotCancelUPload\0slotCurrenIndexChanged\0"
    "slotBackClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoAnalysis[] = {

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
       1,    1,   34,    2, 0x08 /* Private */,
       3,    0,   37,    2, 0x08 /* Private */,
       4,    1,   38,    2, 0x08 /* Private */,
       5,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void VideoAnalysis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoAnalysis *_t = static_cast<VideoAnalysis *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotFileSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->slotCancelUPload(); break;
        case 2: _t->slotCurrenIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slotBackClicked(); break;
        default: ;
        }
    }
}

const QMetaObject VideoAnalysis::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_VideoAnalysis.data,
      qt_meta_data_VideoAnalysis,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VideoAnalysis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoAnalysis::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoAnalysis.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int VideoAnalysis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

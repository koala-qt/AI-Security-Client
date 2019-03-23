/****************************************************************************
** Meta object code from reading C++ file 'multiplefaceanalysis.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/multiplefaceanalysis.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multiplefaceanalysis.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MultipleFaceAnalysis_t {
    QByteArrayData data[8];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultipleFaceAnalysis_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultipleFaceAnalysis_t qt_meta_stringdata_MultipleFaceAnalysis = {
    {
QT_MOC_LITERAL(0, 0, 20), // "MultipleFaceAnalysis"
QT_MOC_LITERAL(1, 21, 16), // "slotOnCameraInfo"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 33), // "QVector<RestServiceI::CameraI..."
QT_MOC_LITERAL(4, 73, 4), // "data"
QT_MOC_LITERAL(5, 78, 24), // "onBtnUploadFolderClicked"
QT_MOC_LITERAL(6, 103, 18), // "onBtnSearchClicked"
QT_MOC_LITERAL(7, 122, 21) // "onBtnOperationClicked"

    },
    "MultipleFaceAnalysis\0slotOnCameraInfo\0"
    "\0QVector<RestServiceI::CameraInfo>\0"
    "data\0onBtnUploadFolderClicked\0"
    "onBtnSearchClicked\0onBtnOperationClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultipleFaceAnalysis[] = {

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
       5,    0,   37,    2, 0x08 /* Private */,
       6,    0,   38,    2, 0x08 /* Private */,
       7,    0,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MultipleFaceAnalysis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MultipleFaceAnalysis *_t = static_cast<MultipleFaceAnalysis *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotOnCameraInfo((*reinterpret_cast< QVector<RestServiceI::CameraInfo>(*)>(_a[1]))); break;
        case 1: _t->onBtnUploadFolderClicked(); break;
        case 2: _t->onBtnSearchClicked(); break;
        case 3: _t->onBtnOperationClicked(); break;
        default: ;
        }
    }
}

const QMetaObject MultipleFaceAnalysis::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_MultipleFaceAnalysis.data,
      qt_meta_data_MultipleFaceAnalysis,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MultipleFaceAnalysis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultipleFaceAnalysis::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MultipleFaceAnalysis.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int MultipleFaceAnalysis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

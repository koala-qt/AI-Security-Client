/****************************************************************************
** Meta object code from reading C++ file 'realmonitorsetting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/realmonitorsetting.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'realmonitorsetting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RealMonitorSetting_t {
    QByteArrayData data[10];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RealMonitorSetting_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RealMonitorSetting_t qt_meta_stringdata_RealMonitorSetting = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RealMonitorSetting"
QT_MOC_LITERAL(1, 19, 15), // "slotCellClicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 13), // "slotAddStatis"
QT_MOC_LITERAL(4, 50, 16), // "slotRemoveStatis"
QT_MOC_LITERAL(5, 67, 12), // "slotSnapInfo"
QT_MOC_LITERAL(6, 80, 19), // "QVector<StatisTask>"
QT_MOC_LITERAL(7, 100, 16), // "slotOnCameraInfo"
QT_MOC_LITERAL(8, 117, 33), // "QVector<RestServiceI::CameraI..."
QT_MOC_LITERAL(9, 151, 15) // "slotOnCameraMap"

    },
    "RealMonitorSetting\0slotCellClicked\0\0"
    "slotAddStatis\0slotRemoveStatis\0"
    "slotSnapInfo\0QVector<StatisTask>\0"
    "slotOnCameraInfo\0QVector<RestServiceI::CameraInfo>\0"
    "slotOnCameraMap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RealMonitorSetting[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x08 /* Private */,
       3,    1,   49,    2, 0x08 /* Private */,
       4,    1,   52,    2, 0x08 /* Private */,
       5,    1,   55,    2, 0x08 /* Private */,
       7,    1,   58,    2, 0x08 /* Private */,
       9,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, QMetaType::QVariantMap,    2,

       0        // eod
};

void RealMonitorSetting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RealMonitorSetting *_t = static_cast<RealMonitorSetting *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotCellClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->slotAddStatis((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slotRemoveStatis((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->slotSnapInfo((*reinterpret_cast< QVector<StatisTask>(*)>(_a[1]))); break;
        case 4: _t->slotOnCameraInfo((*reinterpret_cast< QVector<RestServiceI::CameraInfo>(*)>(_a[1]))); break;
        case 5: _t->slotOnCameraMap((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject RealMonitorSetting::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_RealMonitorSetting.data,
      qt_meta_data_RealMonitorSetting,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RealMonitorSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RealMonitorSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RealMonitorSetting.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int RealMonitorSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

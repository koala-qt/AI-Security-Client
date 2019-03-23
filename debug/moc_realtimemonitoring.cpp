/****************************************************************************
** Meta object code from reading C++ file 'realtimemonitoring.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/realtimemonitoring.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'realtimemonitoring.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RealtimeMonitoring_t {
    QByteArrayData data[25];
    char stringdata0[549];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RealtimeMonitoring_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RealtimeMonitoring_t qt_meta_stringdata_RealtimeMonitoring = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RealtimeMonitoring"
QT_MOC_LITERAL(1, 19, 19), // "sigSwitchBtnClicked"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 27), // "slotEventComboxIndexChanged"
QT_MOC_LITERAL(4, 68, 25), // "slotTreeItemDoubleClicked"
QT_MOC_LITERAL(5, 94, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(6, 111, 15), // "slotAddFaceitem"
QT_MOC_LITERAL(7, 127, 32), // "NotifyPersonI::FaceSnapEventData"
QT_MOC_LITERAL(8, 160, 19), // "slotOnIntruderEvent"
QT_MOC_LITERAL(9, 180, 31), // "NotifyEventI::IntruderEventData"
QT_MOC_LITERAL(10, 212, 17), // "slotOnPersonEvent"
QT_MOC_LITERAL(11, 230, 29), // "NotifyEventI::PersonEventData"
QT_MOC_LITERAL(12, 260, 17), // "slotOnAbDoorEvent"
QT_MOC_LITERAL(13, 278, 29), // "NotifyEventI::ABDoorEventData"
QT_MOC_LITERAL(14, 308, 16), // "slotOnClimbEvent"
QT_MOC_LITERAL(15, 325, 28), // "NotifyEventI::ClimbEventData"
QT_MOC_LITERAL(16, 354, 18), // "slotOngGatherEvent"
QT_MOC_LITERAL(17, 373, 29), // "NotifyEventI::GatherEventData"
QT_MOC_LITERAL(18, 403, 13), // "slotAddDevice"
QT_MOC_LITERAL(19, 417, 33), // "QVector<RestServiceI::CameraI..."
QT_MOC_LITERAL(20, 451, 17), // "slotOnCameraGroup"
QT_MOC_LITERAL(21, 469, 33), // "QVector<RestServiceI::CameraG..."
QT_MOC_LITERAL(22, 503, 15), // "slotOnSceneInfo"
QT_MOC_LITERAL(23, 519, 23), // "RestServiceI::SceneInfo"
QT_MOC_LITERAL(24, 543, 5) // "sinfo"

    },
    "RealtimeMonitoring\0sigSwitchBtnClicked\0"
    "\0slotEventComboxIndexChanged\0"
    "slotTreeItemDoubleClicked\0QTreeWidgetItem*\0"
    "slotAddFaceitem\0NotifyPersonI::FaceSnapEventData\0"
    "slotOnIntruderEvent\0NotifyEventI::IntruderEventData\0"
    "slotOnPersonEvent\0NotifyEventI::PersonEventData\0"
    "slotOnAbDoorEvent\0NotifyEventI::ABDoorEventData\0"
    "slotOnClimbEvent\0NotifyEventI::ClimbEventData\0"
    "slotOngGatherEvent\0NotifyEventI::GatherEventData\0"
    "slotAddDevice\0QVector<RestServiceI::CameraInfo>\0"
    "slotOnCameraGroup\0QVector<RestServiceI::CameraGoup>\0"
    "slotOnSceneInfo\0RestServiceI::SceneInfo\0"
    "sinfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RealtimeMonitoring[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   75,    2, 0x08 /* Private */,
       4,    2,   78,    2, 0x08 /* Private */,
       6,    1,   83,    2, 0x08 /* Private */,
       8,    1,   86,    2, 0x08 /* Private */,
      10,    1,   89,    2, 0x08 /* Private */,
      12,    1,   92,    2, 0x08 /* Private */,
      14,    1,   95,    2, 0x08 /* Private */,
      16,    1,   98,    2, 0x08 /* Private */,
      18,    1,  101,    2, 0x08 /* Private */,
      20,    1,  104,    2, 0x08 /* Private */,
      22,    1,  107,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, 0x80000000 | 15,    2,
    QMetaType::Void, 0x80000000 | 17,    2,
    QMetaType::Void, 0x80000000 | 19,    2,
    QMetaType::Void, 0x80000000 | 21,    2,
    QMetaType::Void, 0x80000000 | 23,   24,

       0        // eod
};

void RealtimeMonitoring::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RealtimeMonitoring *_t = static_cast<RealtimeMonitoring *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigSwitchBtnClicked(); break;
        case 1: _t->slotEventComboxIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotTreeItemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->slotAddFaceitem((*reinterpret_cast< NotifyPersonI::FaceSnapEventData(*)>(_a[1]))); break;
        case 4: _t->slotOnIntruderEvent((*reinterpret_cast< NotifyEventI::IntruderEventData(*)>(_a[1]))); break;
        case 5: _t->slotOnPersonEvent((*reinterpret_cast< NotifyEventI::PersonEventData(*)>(_a[1]))); break;
        case 6: _t->slotOnAbDoorEvent((*reinterpret_cast< NotifyEventI::ABDoorEventData(*)>(_a[1]))); break;
        case 7: _t->slotOnClimbEvent((*reinterpret_cast< NotifyEventI::ClimbEventData(*)>(_a[1]))); break;
        case 8: _t->slotOngGatherEvent((*reinterpret_cast< NotifyEventI::GatherEventData(*)>(_a[1]))); break;
        case 9: _t->slotAddDevice((*reinterpret_cast< QVector<RestServiceI::CameraInfo>(*)>(_a[1]))); break;
        case 10: _t->slotOnCameraGroup((*reinterpret_cast< QVector<RestServiceI::CameraGoup>(*)>(_a[1]))); break;
        case 11: _t->slotOnSceneInfo((*reinterpret_cast< RestServiceI::SceneInfo(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RealtimeMonitoring::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RealtimeMonitoring::sigSwitchBtnClicked)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject RealtimeMonitoring::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_RealtimeMonitoring.data,
      qt_meta_data_RealtimeMonitoring,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RealtimeMonitoring::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RealtimeMonitoring::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RealtimeMonitoring.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int RealtimeMonitoring::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WidgetI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void RealtimeMonitoring::sigSwitchBtnClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

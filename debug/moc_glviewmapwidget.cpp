/****************************************************************************
** Meta object code from reading C++ file 'glviewmapwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/glviewmapwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glviewmapwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GlViewMapWidget_t {
    QByteArrayData data[13];
    char stringdata0[265];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GlViewMapWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GlViewMapWidget_t qt_meta_stringdata_GlViewMapWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "GlViewMapWidget"
QT_MOC_LITERAL(1, 16, 19), // "slotOnIntruderEvent"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 31), // "NotifyEventI::IntruderEventData"
QT_MOC_LITERAL(4, 69, 4), // "info"
QT_MOC_LITERAL(5, 74, 17), // "slotOnAbDoorEvent"
QT_MOC_LITERAL(6, 92, 29), // "NotifyEventI::ABDoorEventData"
QT_MOC_LITERAL(7, 122, 17), // "slotOnPersonEvent"
QT_MOC_LITERAL(8, 140, 29), // "NotifyEventI::PersonEventData"
QT_MOC_LITERAL(9, 170, 16), // "slotOnClimbEvent"
QT_MOC_LITERAL(10, 187, 28), // "NotifyEventI::ClimbEventData"
QT_MOC_LITERAL(11, 216, 18), // "slotOngGatherEvent"
QT_MOC_LITERAL(12, 235, 29) // "NotifyEventI::GatherEventData"

    },
    "GlViewMapWidget\0slotOnIntruderEvent\0"
    "\0NotifyEventI::IntruderEventData\0info\0"
    "slotOnAbDoorEvent\0NotifyEventI::ABDoorEventData\0"
    "slotOnPersonEvent\0NotifyEventI::PersonEventData\0"
    "slotOnClimbEvent\0NotifyEventI::ClimbEventData\0"
    "slotOngGatherEvent\0NotifyEventI::GatherEventData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GlViewMapWidget[] = {

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
       5,    1,   42,    2, 0x08 /* Private */,
       7,    1,   45,    2, 0x08 /* Private */,
       9,    1,   48,    2, 0x08 /* Private */,
      11,    1,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void, 0x80000000 | 10,    4,
    QMetaType::Void, 0x80000000 | 12,    4,

       0        // eod
};

void GlViewMapWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GlViewMapWidget *_t = static_cast<GlViewMapWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotOnIntruderEvent((*reinterpret_cast< NotifyEventI::IntruderEventData(*)>(_a[1]))); break;
        case 1: _t->slotOnAbDoorEvent((*reinterpret_cast< NotifyEventI::ABDoorEventData(*)>(_a[1]))); break;
        case 2: _t->slotOnPersonEvent((*reinterpret_cast< NotifyEventI::PersonEventData(*)>(_a[1]))); break;
        case 3: _t->slotOnClimbEvent((*reinterpret_cast< NotifyEventI::ClimbEventData(*)>(_a[1]))); break;
        case 4: _t->slotOngGatherEvent((*reinterpret_cast< NotifyEventI::GatherEventData(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject GlViewMapWidget::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_GlViewMapWidget.data,
      qt_meta_data_GlViewMapWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GlViewMapWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GlViewMapWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GlViewMapWidget.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int GlViewMapWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

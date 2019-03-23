/****************************************************************************
** Meta object code from reading C++ file 'hompage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/hompage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hompage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HomePageWebBridge_t {
    QByteArrayData data[8];
    char stringdata0[126];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HomePageWebBridge_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HomePageWebBridge_t qt_meta_stringdata_HomePageWebBridge = {
    {
QT_MOC_LITERAL(0, 0, 17), // "HomePageWebBridge"
QT_MOC_LITERAL(1, 18, 18), // "sigHostNameChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 16), // "sigCameraClicked"
QT_MOC_LITERAL(4, 55, 19), // "sigWebSwitchClicked"
QT_MOC_LITERAL(5, 75, 11), // "onInitsized"
QT_MOC_LITERAL(6, 87, 15), // "onCameraClicked"
QT_MOC_LITERAL(7, 103, 22) // "onSwitchToVideoClicked"

    },
    "HomePageWebBridge\0sigHostNameChanged\0"
    "\0sigCameraClicked\0sigWebSwitchClicked\0"
    "onInitsized\0onCameraClicked\0"
    "onSwitchToVideoClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HomePageWebBridge[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    1,   47,    2, 0x06 /* Public */,
       4,    0,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   51,    2, 0x0a /* Public */,
       6,    1,   52,    2, 0x0a /* Public */,
       7,    0,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void HomePageWebBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HomePageWebBridge *_t = static_cast<HomePageWebBridge *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigHostNameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sigCameraClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sigWebSwitchClicked(); break;
        case 3: _t->onInitsized(); break;
        case 4: _t->onCameraClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->onSwitchToVideoClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (HomePageWebBridge::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomePageWebBridge::sigHostNameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HomePageWebBridge::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomePageWebBridge::sigCameraClicked)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HomePageWebBridge::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomePageWebBridge::sigWebSwitchClicked)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject HomePageWebBridge::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HomePageWebBridge.data,
      qt_meta_data_HomePageWebBridge,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HomePageWebBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HomePageWebBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HomePageWebBridge.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HomePageWebBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void HomePageWebBridge::sigHostNameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HomePageWebBridge::sigCameraClicked(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HomePageWebBridge::sigWebSwitchClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_HomPage_t {
    QByteArrayData data[15];
    char stringdata0[320];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HomPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HomPage_t qt_meta_stringdata_HomPage = {
    {
QT_MOC_LITERAL(0, 0, 7), // "HomPage"
QT_MOC_LITERAL(1, 8, 19), // "sigSwitchBtnClicked"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 27), // "slotEventComboxIndexChanged"
QT_MOC_LITERAL(4, 57, 19), // "slotOnIntruderEvent"
QT_MOC_LITERAL(5, 77, 31), // "NotifyEventI::IntruderEventData"
QT_MOC_LITERAL(6, 109, 17), // "slotOnPersonEvent"
QT_MOC_LITERAL(7, 127, 29), // "NotifyEventI::PersonEventData"
QT_MOC_LITERAL(8, 157, 17), // "slotOnAbDoorEvent"
QT_MOC_LITERAL(9, 175, 29), // "NotifyEventI::ABDoorEventData"
QT_MOC_LITERAL(10, 205, 16), // "slotOnClimbEvent"
QT_MOC_LITERAL(11, 222, 28), // "NotifyEventI::ClimbEventData"
QT_MOC_LITERAL(12, 251, 18), // "slotOngGatherEvent"
QT_MOC_LITERAL(13, 270, 29), // "NotifyEventI::GatherEventData"
QT_MOC_LITERAL(14, 300, 19) // "slotOnCameraClicked"

    },
    "HomPage\0sigSwitchBtnClicked\0\0"
    "slotEventComboxIndexChanged\0"
    "slotOnIntruderEvent\0NotifyEventI::IntruderEventData\0"
    "slotOnPersonEvent\0NotifyEventI::PersonEventData\0"
    "slotOnAbDoorEvent\0NotifyEventI::ABDoorEventData\0"
    "slotOnClimbEvent\0NotifyEventI::ClimbEventData\0"
    "slotOngGatherEvent\0NotifyEventI::GatherEventData\0"
    "slotOnCameraClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HomPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   55,    2, 0x08 /* Private */,
       4,    1,   58,    2, 0x08 /* Private */,
       6,    1,   61,    2, 0x08 /* Private */,
       8,    1,   64,    2, 0x08 /* Private */,
      10,    1,   67,    2, 0x08 /* Private */,
      12,    1,   70,    2, 0x08 /* Private */,
      14,    1,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void HomPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HomPage *_t = static_cast<HomPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigSwitchBtnClicked(); break;
        case 1: _t->slotEventComboxIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotOnIntruderEvent((*reinterpret_cast< NotifyEventI::IntruderEventData(*)>(_a[1]))); break;
        case 3: _t->slotOnPersonEvent((*reinterpret_cast< NotifyEventI::PersonEventData(*)>(_a[1]))); break;
        case 4: _t->slotOnAbDoorEvent((*reinterpret_cast< NotifyEventI::ABDoorEventData(*)>(_a[1]))); break;
        case 5: _t->slotOnClimbEvent((*reinterpret_cast< NotifyEventI::ClimbEventData(*)>(_a[1]))); break;
        case 6: _t->slotOngGatherEvent((*reinterpret_cast< NotifyEventI::GatherEventData(*)>(_a[1]))); break;
        case 7: _t->slotOnCameraClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (HomPage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomPage::sigSwitchBtnClicked)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject HomPage::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_HomPage.data,
      qt_meta_data_HomPage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HomPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HomPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HomPage.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int HomPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WidgetI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void HomPage::sigSwitchBtnClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

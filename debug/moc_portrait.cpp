/****************************************************************************
** Meta object code from reading C++ file 'portrait.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/portrait.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'portrait.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Portrait_t {
    QByteArrayData data[6];
    char stringdata0[80];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Portrait_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Portrait_t qt_meta_stringdata_Portrait = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Portrait"
QT_MOC_LITERAL(1, 9, 11), // "slotSetData"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 33), // "RestServiceI::PortraitReturnD..."
QT_MOC_LITERAL(4, 56, 4), // "data"
QT_MOC_LITERAL(5, 61, 18) // "slotSaveBtnClicked"

    },
    "Portrait\0slotSetData\0\0"
    "RestServiceI::PortraitReturnData&\0"
    "data\0slotSaveBtnClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Portrait[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       5,    0,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void Portrait::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Portrait *_t = static_cast<Portrait *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotSetData((*reinterpret_cast< RestServiceI::PortraitReturnData(*)>(_a[1]))); break;
        case 1: _t->slotSaveBtnClicked(); break;
        default: ;
        }
    }
}

const QMetaObject Portrait::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_Portrait.data,
      qt_meta_data_Portrait,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Portrait::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Portrait::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Portrait.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int Portrait::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WidgetI::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE

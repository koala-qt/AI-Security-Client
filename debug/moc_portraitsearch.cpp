/****************************************************************************
** Meta object code from reading C++ file 'portraitsearch.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/portraitsearch.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'portraitsearch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PortraitSearch_t {
    QByteArrayData data[10];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PortraitSearch_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PortraitSearch_t qt_meta_stringdata_PortraitSearch = {
    {
QT_MOC_LITERAL(0, 0, 14), // "PortraitSearch"
QT_MOC_LITERAL(1, 15, 12), // "slotAddImage"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 7), // "QImage&"
QT_MOC_LITERAL(4, 37, 3), // "img"
QT_MOC_LITERAL(5, 41, 18), // "onBtnSearchClicked"
QT_MOC_LITERAL(6, 60, 15), // "onBtnImgClicked"
QT_MOC_LITERAL(7, 76, 16), // "onSectionClicked"
QT_MOC_LITERAL(8, 93, 5), // "index"
QT_MOC_LITERAL(9, 99, 21) // "onBtnOperationClicked"

    },
    "PortraitSearch\0slotAddImage\0\0QImage&\0"
    "img\0onBtnSearchClicked\0onBtnImgClicked\0"
    "onSectionClicked\0index\0onBtnOperationClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PortraitSearch[] = {

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
       1,    1,   39,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,
       7,    1,   44,    2, 0x08 /* Private */,
       9,    0,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,

       0        // eod
};

void PortraitSearch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PortraitSearch *_t = static_cast<PortraitSearch *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotAddImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->onBtnSearchClicked(); break;
        case 2: _t->onBtnImgClicked(); break;
        case 3: _t->onSectionClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onBtnOperationClicked(); break;
        default: ;
        }
    }
}

const QMetaObject PortraitSearch::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_PortraitSearch.data,
      qt_meta_data_PortraitSearch,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PortraitSearch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PortraitSearch::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PortraitSearch.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int PortraitSearch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

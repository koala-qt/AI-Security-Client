/****************************************************************************
** Meta object code from reading C++ file 'personmark.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/personmark.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'personmark.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PersonMark_t {
    QByteArrayData data[6];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PersonMark_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PersonMark_t qt_meta_stringdata_PersonMark = {
    {
QT_MOC_LITERAL(0, 0, 10), // "PersonMark"
QT_MOC_LITERAL(1, 11, 16), // "slotOnPersonType"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 33), // "QVector<RestServiceI::PersonT..."
QT_MOC_LITERAL(4, 63, 28), // "slotOnTypeComboxIndexChanged"
QT_MOC_LITERAL(5, 92, 20) // "slotSubmitBtnClicked"

    },
    "PersonMark\0slotOnPersonType\0\0"
    "QVector<RestServiceI::PersonType>\0"
    "slotOnTypeComboxIndexChanged\0"
    "slotSubmitBtnClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PersonMark[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x08 /* Private */,
       4,    1,   32,    2, 0x08 /* Private */,
       5,    0,   35,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void PersonMark::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PersonMark *_t = static_cast<PersonMark *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotOnPersonType((*reinterpret_cast< QVector<RestServiceI::PersonType>(*)>(_a[1]))); break;
        case 1: _t->slotOnTypeComboxIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotSubmitBtnClicked(); break;
        default: ;
        }
    }
}

const QMetaObject PersonMark::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PersonMark.data,
      qt_meta_data_PersonMark,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PersonMark::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PersonMark::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PersonMark.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int PersonMark::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

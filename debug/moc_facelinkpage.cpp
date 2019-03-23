/****************************************************************************
** Meta object code from reading C++ file 'facelinkpage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/facelinkpage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'facelinkpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FaceLinkPage_t {
    QByteArrayData data[6];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FaceLinkPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FaceLinkPage_t qt_meta_stringdata_FaceLinkPage = {
    {
QT_MOC_LITERAL(0, 0, 12), // "FaceLinkPage"
QT_MOC_LITERAL(1, 13, 20), // "slotSearchBtnClicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 20), // "slotFaceLinkFinished"
QT_MOC_LITERAL(4, 56, 16), // "slotFaceLinkTree"
QT_MOC_LITERAL(5, 73, 17) // "slotImgBtnClicked"

    },
    "FaceLinkPage\0slotSearchBtnClicked\0\0"
    "slotFaceLinkFinished\0slotFaceLinkTree\0"
    "slotImgBtnClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FaceLinkPage[] = {

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
       1,    0,   34,    2, 0x08 /* Private */,
       3,    1,   35,    2, 0x08 /* Private */,
       4,    1,   38,    2, 0x08 /* Private */,
       5,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QJsonObject,    2,
    QMetaType::Void,

       0        // eod
};

void FaceLinkPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FaceLinkPage *_t = static_cast<FaceLinkPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotSearchBtnClicked(); break;
        case 1: _t->slotFaceLinkFinished((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotFaceLinkTree((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 3: _t->slotImgBtnClicked(); break;
        default: ;
        }
    }
}

const QMetaObject FaceLinkPage::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_FaceLinkPage.data,
      qt_meta_data_FaceLinkPage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FaceLinkPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FaceLinkPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FaceLinkPage.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int FaceLinkPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

/****************************************************************************
** Meta object code from reading C++ file 'combinationpage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/combinationpage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'combinationpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CombinationPage_t {
    QByteArrayData data[11];
    char stringdata0[211];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CombinationPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CombinationPage_t qt_meta_stringdata_CombinationPage = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CombinationPage"
QT_MOC_LITERAL(1, 16, 27), // "slotFaceTabelSectionClicked"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 27), // "slotBodyTabelSectionClicked"
QT_MOC_LITERAL(4, 73, 16), // "slotOnCameraInfo"
QT_MOC_LITERAL(5, 90, 33), // "QVector<RestServiceI::CameraI..."
QT_MOC_LITERAL(6, 124, 20), // "slotSearchBtnClicked"
QT_MOC_LITERAL(7, 145, 19), // "slotImageBtnClicked"
QT_MOC_LITERAL(8, 165, 15), // "slotOnSceneInfo"
QT_MOC_LITERAL(9, 181, 23), // "RestServiceI::SceneInfo"
QT_MOC_LITERAL(10, 205, 5) // "sinfo"

    },
    "CombinationPage\0slotFaceTabelSectionClicked\0"
    "\0slotBodyTabelSectionClicked\0"
    "slotOnCameraInfo\0QVector<RestServiceI::CameraInfo>\0"
    "slotSearchBtnClicked\0slotImageBtnClicked\0"
    "slotOnSceneInfo\0RestServiceI::SceneInfo\0"
    "sinfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CombinationPage[] = {

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
       1,    1,   44,    2, 0x08 /* Private */,
       3,    1,   47,    2, 0x08 /* Private */,
       4,    1,   50,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    1,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void CombinationPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CombinationPage *_t = static_cast<CombinationPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotFaceTabelSectionClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slotBodyTabelSectionClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotOnCameraInfo((*reinterpret_cast< QVector<RestServiceI::CameraInfo>(*)>(_a[1]))); break;
        case 3: _t->slotSearchBtnClicked(); break;
        case 4: _t->slotImageBtnClicked(); break;
        case 5: _t->slotOnSceneInfo((*reinterpret_cast< RestServiceI::SceneInfo(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CombinationPage::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_CombinationPage.data,
      qt_meta_data_CombinationPage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CombinationPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CombinationPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CombinationPage.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int CombinationPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WidgetI::qt_metacall(_c, _id, _a);
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

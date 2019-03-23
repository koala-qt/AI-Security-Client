/****************************************************************************
** Meta object code from reading C++ file 'facesearch.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/facesearch.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'facesearch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FaceSearch_t {
    QByteArrayData data[12];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FaceSearch_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FaceSearch_t qt_meta_stringdata_FaceSearch = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FaceSearch"
QT_MOC_LITERAL(1, 11, 10), // "slotAddRow"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 38), // "QVector<RestServiceI::DataRec..."
QT_MOC_LITERAL(4, 62, 16), // "slotOnCameraInfo"
QT_MOC_LITERAL(5, 79, 33), // "QVector<RestServiceI::CameraI..."
QT_MOC_LITERAL(6, 113, 18), // "slotSectionClicked"
QT_MOC_LITERAL(7, 132, 17), // "slotSearchClicked"
QT_MOC_LITERAL(8, 150, 17), // "slotImgBtnClicked"
QT_MOC_LITERAL(9, 168, 15), // "slotOnSceneInfo"
QT_MOC_LITERAL(10, 184, 23), // "RestServiceI::SceneInfo"
QT_MOC_LITERAL(11, 208, 5) // "sinfo"

    },
    "FaceSearch\0slotAddRow\0\0"
    "QVector<RestServiceI::DataRectureItem>\0"
    "slotOnCameraInfo\0QVector<RestServiceI::CameraInfo>\0"
    "slotSectionClicked\0slotSearchClicked\0"
    "slotImgBtnClicked\0slotOnSceneInfo\0"
    "RestServiceI::SceneInfo\0sinfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FaceSearch[] = {

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
       1,    1,   44,    2, 0x0a /* Public */,
       4,    1,   47,    2, 0x08 /* Private */,
       6,    1,   50,    2, 0x08 /* Private */,
       7,    0,   53,    2, 0x08 /* Private */,
       8,    0,   54,    2, 0x08 /* Private */,
       9,    1,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void FaceSearch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FaceSearch *_t = static_cast<FaceSearch *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotAddRow((*reinterpret_cast< QVector<RestServiceI::DataRectureItem>(*)>(_a[1]))); break;
        case 1: _t->slotOnCameraInfo((*reinterpret_cast< QVector<RestServiceI::CameraInfo>(*)>(_a[1]))); break;
        case 2: _t->slotSectionClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slotSearchClicked(); break;
        case 4: _t->slotImgBtnClicked(); break;
        case 5: _t->slotOnSceneInfo((*reinterpret_cast< RestServiceI::SceneInfo(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject FaceSearch::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_FaceSearch.data,
      qt_meta_data_FaceSearch,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FaceSearch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FaceSearch::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FaceSearch.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int FaceSearch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

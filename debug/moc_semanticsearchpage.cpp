/****************************************************************************
** Meta object code from reading C++ file 'semanticsearchpage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/semanticsearchpage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'semanticsearchpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SemanticSearchPage_t {
    QByteArrayData data[21];
    char stringdata0[331];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SemanticSearchPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SemanticSearchPage_t qt_meta_stringdata_SemanticSearchPage = {
    {
QT_MOC_LITERAL(0, 0, 18), // "SemanticSearchPage"
QT_MOC_LITERAL(1, 19, 23), // "slotAttributeBtnClicked"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 20), // "slotPageIndexChanged"
QT_MOC_LITERAL(4, 65, 13), // "slotSearchAll"
QT_MOC_LITERAL(5, 79, 9), // "attrAsync"
QT_MOC_LITERAL(6, 89, 18), // "slotSemanticSearch"
QT_MOC_LITERAL(7, 108, 18), // "slotSearchFaceLink"
QT_MOC_LITERAL(8, 127, 20), // "slotSearchBtnClicked"
QT_MOC_LITERAL(9, 148, 16), // "slotOnCameraInfo"
QT_MOC_LITERAL(10, 165, 33), // "QVector<RestServiceI::CameraI..."
QT_MOC_LITERAL(11, 199, 19), // "slotTreeItemChanged"
QT_MOC_LITERAL(12, 219, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(13, 236, 14), // "createTreeItem"
QT_MOC_LITERAL(14, 251, 12), // "QTreeWidget*"
QT_MOC_LITERAL(15, 264, 5), // "treeW"
QT_MOC_LITERAL(16, 270, 10), // "parentItem"
QT_MOC_LITERAL(17, 281, 9), // "itemData&"
QT_MOC_LITERAL(18, 291, 5), // "items"
QT_MOC_LITERAL(19, 297, 27), // "onSourceCurrentIndexChanged"
QT_MOC_LITERAL(20, 325, 5) // "index"

    },
    "SemanticSearchPage\0slotAttributeBtnClicked\0"
    "\0slotPageIndexChanged\0slotSearchAll\0"
    "attrAsync\0slotSemanticSearch\0"
    "slotSearchFaceLink\0slotSearchBtnClicked\0"
    "slotOnCameraInfo\0QVector<RestServiceI::CameraInfo>\0"
    "slotTreeItemChanged\0QTreeWidgetItem*\0"
    "createTreeItem\0QTreeWidget*\0treeW\0"
    "parentItem\0itemData&\0items\0"
    "onSourceCurrentIndexChanged\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SemanticSearchPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    1,   65,    2, 0x08 /* Private */,
       4,    2,   68,    2, 0x08 /* Private */,
       6,    2,   73,    2, 0x08 /* Private */,
       7,    2,   78,    2, 0x08 /* Private */,
       8,    0,   83,    2, 0x08 /* Private */,
       9,    1,   84,    2, 0x08 /* Private */,
      11,    2,   87,    2, 0x08 /* Private */,
      13,    3,   92,    2, 0x08 /* Private */,
      19,    1,   99,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    5,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, 0x80000000 | 12, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 12, 0x80000000 | 17,   15,   16,   18,
    QMetaType::Void, QMetaType::Int,   20,

       0        // eod
};

void SemanticSearchPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SemanticSearchPage *_t = static_cast<SemanticSearchPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotAttributeBtnClicked(); break;
        case 1: _t->slotPageIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotSearchAll((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->slotSemanticSearch((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->slotSearchFaceLink((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->slotSearchBtnClicked(); break;
        case 6: _t->slotOnCameraInfo((*reinterpret_cast< QVector<RestServiceI::CameraInfo>(*)>(_a[1]))); break;
        case 7: _t->slotTreeItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->createTreeItem((*reinterpret_cast< QTreeWidget*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2])),(*reinterpret_cast< itemData(*)>(_a[3]))); break;
        case 9: _t->onSourceCurrentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SemanticSearchPage::staticMetaObject = {
    { &WidgetI::staticMetaObject, qt_meta_stringdata_SemanticSearchPage.data,
      qt_meta_data_SemanticSearchPage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SemanticSearchPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SemanticSearchPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SemanticSearchPage.stringdata0))
        return static_cast<void*>(this);
    return WidgetI::qt_metacast(_clname);
}

int SemanticSearchPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WidgetI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

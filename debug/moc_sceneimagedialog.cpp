/****************************************************************************
** Meta object code from reading C++ file 'sceneimagedialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/sceneimagedialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sceneimagedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SceneImageDialog_t {
    QByteArrayData data[9];
    char stringdata0[142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SceneImageDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SceneImageDialog_t qt_meta_stringdata_SceneImageDialog = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SceneImageDialog"
QT_MOC_LITERAL(1, 17, 9), // "sigImages"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 15), // "QVector<QImage>"
QT_MOC_LITERAL(4, 44, 18), // "slotSaveBtnClicked"
QT_MOC_LITERAL(5, 63, 20), // "slotSearchBtnClicked"
QT_MOC_LITERAL(6, 84, 18), // "slotSureBtnClicked"
QT_MOC_LITERAL(7, 103, 19), // "slotDeleteBtnClicke"
QT_MOC_LITERAL(8, 123, 18) // "slotOnClickedImage"

    },
    "SceneImageDialog\0sigImages\0\0QVector<QImage>\0"
    "slotSaveBtnClicked\0slotSearchBtnClicked\0"
    "slotSureBtnClicked\0slotDeleteBtnClicke\0"
    "slotOnClickedImage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SceneImageDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   47,    2, 0x08 /* Private */,
       5,    0,   48,    2, 0x08 /* Private */,
       6,    0,   49,    2, 0x08 /* Private */,
       7,    0,   50,    2, 0x08 /* Private */,
       8,    1,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,    2,

       0        // eod
};

void SceneImageDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SceneImageDialog *_t = static_cast<SceneImageDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigImages((*reinterpret_cast< QVector<QImage>(*)>(_a[1]))); break;
        case 1: _t->slotSaveBtnClicked(); break;
        case 2: _t->slotSearchBtnClicked(); break;
        case 3: _t->slotSureBtnClicked(); break;
        case 4: _t->slotDeleteBtnClicke(); break;
        case 5: _t->slotOnClickedImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QImage> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SceneImageDialog::*_t)(QVector<QImage> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SceneImageDialog::sigImages)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SceneImageDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SceneImageDialog.data,
      qt_meta_data_SceneImageDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SceneImageDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SceneImageDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SceneImageDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SceneImageDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SceneImageDialog::sigImages(QVector<QImage> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

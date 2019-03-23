/****************************************************************************
** Meta object code from reading C++ file 'timeaxis.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/timeaxis.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'timeaxis.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TimeAxis_t {
    QByteArrayData data[10];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TimeAxis_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TimeAxis_t qt_meta_stringdata_TimeAxis = {
    {
QT_MOC_LITERAL(0, 0, 8), // "TimeAxis"
QT_MOC_LITERAL(1, 9, 17), // "graduationChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 15), // "minValueChanged"
QT_MOC_LITERAL(4, 44, 15), // "maxValueChanged"
QT_MOC_LITERAL(5, 60, 12), // "valueChanged"
QT_MOC_LITERAL(6, 73, 10), // "graduation"
QT_MOC_LITERAL(7, 84, 8), // "minValue"
QT_MOC_LITERAL(8, 93, 8), // "maxValue"
QT_MOC_LITERAL(9, 102, 5) // "value"

    },
    "TimeAxis\0graduationChanged\0\0minValueChanged\0"
    "maxValueChanged\0valueChanged\0graduation\0"
    "minValue\0maxValue\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TimeAxis[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       4,   38, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    0,   36,    2, 0x06 /* Public */,
       5,    0,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       6, QMetaType::Double, 0x00495103,
       7, QMetaType::Double, 0x00495103,
       8, QMetaType::Double, 0x00495103,
       9, QMetaType::Double, 0x00095103,

 // properties: notify_signal_id
       0,
       1,
       2,
       0,

       0        // eod
};

void TimeAxis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TimeAxis *_t = static_cast<TimeAxis *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->graduationChanged(); break;
        case 1: _t->minValueChanged(); break;
        case 2: _t->maxValueChanged(); break;
        case 3: _t->valueChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (TimeAxis::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TimeAxis::graduationChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TimeAxis::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TimeAxis::minValueChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TimeAxis::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TimeAxis::maxValueChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TimeAxis::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TimeAxis::valueChanged)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TimeAxis *_t = static_cast<TimeAxis *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->graduation(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->minValue(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->maxValue(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->value(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        TimeAxis *_t = static_cast<TimeAxis *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setGraduation(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setMinValue(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setMaxValue(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setValue(*reinterpret_cast< double*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject TimeAxis::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TimeAxis.data,
      qt_meta_data_TimeAxis,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TimeAxis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TimeAxis::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TimeAxis.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TimeAxis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TimeAxis::graduationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TimeAxis::minValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TimeAxis::maxValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TimeAxis::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

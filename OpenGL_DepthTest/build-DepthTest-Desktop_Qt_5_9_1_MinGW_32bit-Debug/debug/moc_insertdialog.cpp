/****************************************************************************
** Meta object code from reading C++ file 'insertdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DepthTest/insertdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'insertdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InsertDialog_t {
    QByteArrayData data[9];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InsertDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InsertDialog_t qt_meta_stringdata_InsertDialog = {
    {
QT_MOC_LITERAL(0, 0, 12), // "InsertDialog"
QT_MOC_LITERAL(1, 13, 12), // "transmitData"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "location"
QT_MOC_LITERAL(4, 36, 6), // "rotate"
QT_MOC_LITERAL(5, 43, 5), // "scale"
QT_MOC_LITERAL(6, 49, 3), // "dir"
QT_MOC_LITERAL(7, 53, 4), // "path"
QT_MOC_LITERAL(8, 58, 16) // "CreateFileDialog"

    },
    "InsertDialog\0transmitData\0\0location\0"
    "rotate\0scale\0dir\0path\0CreateFileDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InsertDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVector3D, QMetaType::QVector4D, QMetaType::QVector3D, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,    7,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void InsertDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InsertDialog *_t = static_cast<InsertDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->transmitData((*reinterpret_cast< QVector3D(*)>(_a[1])),(*reinterpret_cast< QVector4D(*)>(_a[2])),(*reinterpret_cast< QVector3D(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 1: _t->CreateFileDialog(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (InsertDialog::*_t)(QVector3D , QVector4D , QVector3D , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InsertDialog::transmitData)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject InsertDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_InsertDialog.data,
      qt_meta_data_InsertDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *InsertDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InsertDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InsertDialog.stringdata0))
        return static_cast<void*>(const_cast< InsertDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int InsertDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void InsertDialog::transmitData(QVector3D _t1, QVector4D _t2, QVector3D _t3, QString _t4, QString _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

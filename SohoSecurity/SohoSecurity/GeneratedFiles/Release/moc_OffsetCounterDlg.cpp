/****************************************************************************
** Meta object code from reading C++ file 'OffsetCounterDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../OffsetCounterDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OffsetCounterDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_OffsetCounterDlg_t {
    QByteArrayData data[7];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OffsetCounterDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OffsetCounterDlg_t qt_meta_stringdata_OffsetCounterDlg = {
    {
QT_MOC_LITERAL(0, 0, 16), // "OffsetCounterDlg"
QT_MOC_LITERAL(1, 17, 17), // "onPsBtnRVAClicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 17), // "onPsBtnFOAClicked"
QT_MOC_LITERAL(4, 54, 23), // "onPsBtnExecCountClicked"
QT_MOC_LITERAL(5, 78, 12), // "onText2Upper"
QT_MOC_LITERAL(6, 91, 7) // "strText"

    },
    "OffsetCounterDlg\0onPsBtnRVAClicked\0\0"
    "onPsBtnFOAClicked\0onPsBtnExecCountClicked\0"
    "onText2Upper\0strText"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OffsetCounterDlg[] = {

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
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    1,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void OffsetCounterDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OffsetCounterDlg *_t = static_cast<OffsetCounterDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onPsBtnRVAClicked(); break;
        case 1: _t->onPsBtnFOAClicked(); break;
        case 2: _t->onPsBtnExecCountClicked(); break;
        case 3: _t->onText2Upper((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject OffsetCounterDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_OffsetCounterDlg.data,
      qt_meta_data_OffsetCounterDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *OffsetCounterDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OffsetCounterDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_OffsetCounterDlg.stringdata0))
        return static_cast<void*>(const_cast< OffsetCounterDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int OffsetCounterDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE

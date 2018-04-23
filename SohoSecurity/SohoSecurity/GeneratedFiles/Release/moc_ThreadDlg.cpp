/****************************************************************************
** Meta object code from reading C++ file 'ThreadDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../ThreadDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ThreadDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ThreadDlg_t {
    QByteArrayData data[8];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThreadDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThreadDlg_t qt_meta_stringdata_ThreadDlg = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ThreadDlg"
QT_MOC_LITERAL(1, 10, 37), // "onTableWidgetCustomContextMen..."
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 3), // "pos"
QT_MOC_LITERAL(4, 53, 16), // "onPopMenuRefresh"
QT_MOC_LITERAL(5, 70, 22), // "onPopMenuSuspendThread"
QT_MOC_LITERAL(6, 93, 21), // "onPopMenuResumeThread"
QT_MOC_LITERAL(7, 115, 18) // "onPopMenuEndThread"

    },
    "ThreadDlg\0onTableWidgetCustomContextMenuRequest\0"
    "\0pos\0onPopMenuRefresh\0onPopMenuSuspendThread\0"
    "onPopMenuResumeThread\0onPopMenuEndThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThreadDlg[] = {

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
       1,    1,   39,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ThreadDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ThreadDlg *_t = static_cast<ThreadDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTableWidgetCustomContextMenuRequest((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 1: _t->onPopMenuRefresh(); break;
        case 2: _t->onPopMenuSuspendThread(); break;
        case 3: _t->onPopMenuResumeThread(); break;
        case 4: _t->onPopMenuEndThread(); break;
        default: ;
        }
    }
}

const QMetaObject ThreadDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ThreadDlg.data,
      qt_meta_data_ThreadDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ThreadDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThreadDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ThreadDlg.stringdata0))
        return static_cast<void*>(const_cast< ThreadDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int ThreadDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE

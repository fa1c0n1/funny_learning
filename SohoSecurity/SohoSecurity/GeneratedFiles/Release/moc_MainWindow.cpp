/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SohoSecurity_t {
    QByteArrayData data[17];
    char stringdata0[243];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SohoSecurity_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SohoSecurity_t qt_meta_stringdata_SohoSecurity = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SohoSecurity"
QT_MOC_LITERAL(1, 13, 9), // "activated"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 18), // "onActionLockScreen"
QT_MOC_LITERAL(4, 43, 14), // "onActionLogOff"
QT_MOC_LITERAL(5, 58, 17), // "onActionHibernate"
QT_MOC_LITERAL(6, 76, 13), // "onActionSleep"
QT_MOC_LITERAL(7, 90, 15), // "onActionRestart"
QT_MOC_LITERAL(8, 106, 16), // "onActionShutdown"
QT_MOC_LITERAL(9, 123, 18), // "onUpdateProcessNum"
QT_MOC_LITERAL(10, 142, 8), // "nProcNum"
QT_MOC_LITERAL(11, 151, 16), // "onUpdateCpuUsage"
QT_MOC_LITERAL(12, 168, 9), // "nCpuUsage"
QT_MOC_LITERAL(13, 178, 16), // "onUpdateMemUsage"
QT_MOC_LITERAL(14, 195, 10), // "dwMemUsage"
QT_MOC_LITERAL(15, 206, 12), // "onStopThread"
QT_MOC_LITERAL(16, 219, 23) // "onUpdateMemOptimizeInfo"

    },
    "SohoSecurity\0activated\0\0onActionLockScreen\0"
    "onActionLogOff\0onActionHibernate\0"
    "onActionSleep\0onActionRestart\0"
    "onActionShutdown\0onUpdateProcessNum\0"
    "nProcNum\0onUpdateCpuUsage\0nCpuUsage\0"
    "onUpdateMemUsage\0dwMemUsage\0onStopThread\0"
    "onUpdateMemOptimizeInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SohoSecurity[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    0,   75,    2, 0x0a /* Public */,
       4,    0,   76,    2, 0x0a /* Public */,
       5,    0,   77,    2, 0x0a /* Public */,
       6,    0,   78,    2, 0x0a /* Public */,
       7,    0,   79,    2, 0x0a /* Public */,
       8,    0,   80,    2, 0x0a /* Public */,
       9,    1,   81,    2, 0x0a /* Public */,
      11,    1,   84,    2, 0x0a /* Public */,
      13,    1,   87,    2, 0x0a /* Public */,
      15,    0,   90,    2, 0x0a /* Public */,
      16,    0,   91,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SohoSecurity::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SohoSecurity *_t = static_cast<SohoSecurity *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->activated(); break;
        case 1: _t->onActionLockScreen(); break;
        case 2: _t->onActionLogOff(); break;
        case 3: _t->onActionHibernate(); break;
        case 4: _t->onActionSleep(); break;
        case 5: _t->onActionRestart(); break;
        case 6: _t->onActionShutdown(); break;
        case 7: _t->onUpdateProcessNum((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->onUpdateCpuUsage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onUpdateMemUsage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onStopThread(); break;
        case 11: _t->onUpdateMemOptimizeInfo(); break;
        default: ;
        }
    }
}

const QMetaObject SohoSecurity::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SohoSecurity.data,
      qt_meta_data_SohoSecurity,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SohoSecurity::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SohoSecurity::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SohoSecurity.stringdata0))
        return static_cast<void*>(const_cast< SohoSecurity*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SohoSecurity::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

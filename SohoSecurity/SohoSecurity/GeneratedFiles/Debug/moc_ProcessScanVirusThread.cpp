/****************************************************************************
** Meta object code from reading C++ file 'ProcessScanVirusThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../ProcessScanVirusThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProcessScanVirusThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ProcessScanVirusThread_t {
    QByteArrayData data[7];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProcessScanVirusThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProcessScanVirusThread_t qt_meta_stringdata_ProcessScanVirusThread = {
    {
QT_MOC_LITERAL(0, 0, 22), // "ProcessScanVirusThread"
QT_MOC_LITERAL(1, 23, 22), // "updateProcScanProgress"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 11), // "strProcName"
QT_MOC_LITERAL(4, 59, 17), // "catchVirusProcess"
QT_MOC_LITERAL(5, 77, 7), // "strPath"
QT_MOC_LITERAL(6, 85, 8) // "strState"

    },
    "ProcessScanVirusThread\0updateProcScanProgress\0"
    "\0strProcName\0catchVirusProcess\0strPath\0"
    "strState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProcessScanVirusThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    3,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    5,    6,

       0        // eod
};

void ProcessScanVirusThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ProcessScanVirusThread *_t = static_cast<ProcessScanVirusThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateProcScanProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->catchVirusProcess((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ProcessScanVirusThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ProcessScanVirusThread::updateProcScanProgress)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ProcessScanVirusThread::*_t)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ProcessScanVirusThread::catchVirusProcess)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ProcessScanVirusThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ProcessScanVirusThread.data,
      qt_meta_data_ProcessScanVirusThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ProcessScanVirusThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProcessScanVirusThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ProcessScanVirusThread.stringdata0))
        return static_cast<void*>(const_cast< ProcessScanVirusThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ProcessScanVirusThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void ProcessScanVirusThread::updateProcScanProgress(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ProcessScanVirusThread::catchVirusProcess(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

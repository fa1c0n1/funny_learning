/****************************************************************************
** Meta object code from reading C++ file 'LocalScanVirusThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../LocalScanVirusThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LocalScanVirusThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LocalScanVirusThread_t {
    QByteArrayData data[5];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LocalScanVirusThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LocalScanVirusThread_t qt_meta_stringdata_LocalScanVirusThread = {
    {
QT_MOC_LITERAL(0, 0, 20), // "LocalScanVirusThread"
QT_MOC_LITERAL(1, 21, 23), // "updateLocalScanProgress"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 11), // "strFilePath"
QT_MOC_LITERAL(4, 58, 10) // "catchVirus"

    },
    "LocalScanVirusThread\0updateLocalScanProgress\0"
    "\0strFilePath\0catchVirus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LocalScanVirusThread[] = {

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
       4,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void LocalScanVirusThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LocalScanVirusThread *_t = static_cast<LocalScanVirusThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateLocalScanProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->catchVirus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LocalScanVirusThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalScanVirusThread::updateLocalScanProgress)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (LocalScanVirusThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalScanVirusThread::catchVirus)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject LocalScanVirusThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_LocalScanVirusThread.data,
      qt_meta_data_LocalScanVirusThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LocalScanVirusThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LocalScanVirusThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LocalScanVirusThread.stringdata0))
        return static_cast<void*>(const_cast< LocalScanVirusThread*>(this));
    return QThread::qt_metacast(_clname);
}

int LocalScanVirusThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void LocalScanVirusThread::updateLocalScanProgress(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LocalScanVirusThread::catchVirus(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

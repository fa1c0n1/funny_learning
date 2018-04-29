/****************************************************************************
** Meta object code from reading C++ file 'CleanSysAndBrwsThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../CleanSysAndBrwsThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CleanSysAndBrwsThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CleanSysAndBrwsThread_t {
    QByteArrayData data[9];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CleanSysAndBrwsThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CleanSysAndBrwsThread_t qt_meta_stringdata_CleanSysAndBrwsThread = {
    {
QT_MOC_LITERAL(0, 0, 21), // "CleanSysAndBrwsThread"
QT_MOC_LITERAL(1, 22, 25), // "updateClnTmpFilesProgress"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 11), // "strFilePath"
QT_MOC_LITERAL(4, 61, 25), // "updateClnSysFilesProgress"
QT_MOC_LITERAL(5, 87, 27), // "updateClnRecycleBinProgress"
QT_MOC_LITERAL(6, 115, 25), // "updateClnSysCacheProgress"
QT_MOC_LITERAL(7, 141, 20), // "updateClnLogProgress"
QT_MOC_LITERAL(8, 162, 24) // "updateClnBrowserProgress"

    },
    "CleanSysAndBrwsThread\0updateClnTmpFilesProgress\0"
    "\0strFilePath\0updateClnSysFilesProgress\0"
    "updateClnRecycleBinProgress\0"
    "updateClnSysCacheProgress\0"
    "updateClnLogProgress\0updateClnBrowserProgress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CleanSysAndBrwsThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       5,    1,   50,    2, 0x06 /* Public */,
       6,    1,   53,    2, 0x06 /* Public */,
       7,    1,   56,    2, 0x06 /* Public */,
       8,    1,   59,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void CleanSysAndBrwsThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CleanSysAndBrwsThread *_t = static_cast<CleanSysAndBrwsThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateClnTmpFilesProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->updateClnSysFilesProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->updateClnRecycleBinProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->updateClnSysCacheProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->updateClnLogProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->updateClnBrowserProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CleanSysAndBrwsThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CleanSysAndBrwsThread::updateClnTmpFilesProgress)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CleanSysAndBrwsThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CleanSysAndBrwsThread::updateClnSysFilesProgress)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CleanSysAndBrwsThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CleanSysAndBrwsThread::updateClnRecycleBinProgress)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CleanSysAndBrwsThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CleanSysAndBrwsThread::updateClnSysCacheProgress)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (CleanSysAndBrwsThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CleanSysAndBrwsThread::updateClnLogProgress)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (CleanSysAndBrwsThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CleanSysAndBrwsThread::updateClnBrowserProgress)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject CleanSysAndBrwsThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CleanSysAndBrwsThread.data,
      qt_meta_data_CleanSysAndBrwsThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CleanSysAndBrwsThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CleanSysAndBrwsThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CleanSysAndBrwsThread.stringdata0))
        return static_cast<void*>(const_cast< CleanSysAndBrwsThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CleanSysAndBrwsThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CleanSysAndBrwsThread::updateClnTmpFilesProgress(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CleanSysAndBrwsThread::updateClnSysFilesProgress(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CleanSysAndBrwsThread::updateClnRecycleBinProgress(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CleanSysAndBrwsThread::updateClnSysCacheProgress(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CleanSysAndBrwsThread::updateClnLogProgress(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CleanSysAndBrwsThread::updateClnBrowserProgress(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE

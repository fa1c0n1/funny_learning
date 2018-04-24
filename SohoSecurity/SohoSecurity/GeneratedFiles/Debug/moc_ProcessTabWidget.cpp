/****************************************************************************
** Meta object code from reading C++ file 'ProcessTabWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../ProcessTabWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProcessTabWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ProcessTabWidget_t {
    QByteArrayData data[10];
    char stringdata0[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProcessTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProcessTabWidget_t qt_meta_stringdata_ProcessTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ProcessTabWidget"
QT_MOC_LITERAL(1, 17, 16), // "updateProcessNum"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 8), // "nProcNum"
QT_MOC_LITERAL(4, 44, 37), // "onTableWidgetCustomContextMen..."
QT_MOC_LITERAL(5, 82, 3), // "pos"
QT_MOC_LITERAL(6, 86, 16), // "onPopMenuRefresh"
QT_MOC_LITERAL(7, 103, 23), // "onPopMenuTraverseModule"
QT_MOC_LITERAL(8, 127, 23), // "onPopMenuTraverseThread"
QT_MOC_LITERAL(9, 151, 19) // "onPopMenuEndProcess"

    },
    "ProcessTabWidget\0updateProcessNum\0\0"
    "nProcNum\0onTableWidgetCustomContextMenuRequest\0"
    "pos\0onPopMenuRefresh\0onPopMenuTraverseModule\0"
    "onPopMenuTraverseThread\0onPopMenuEndProcess"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProcessTabWidget[] = {

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
       4,    1,   47,    2, 0x0a /* Public */,
       6,    0,   50,    2, 0x0a /* Public */,
       7,    0,   51,    2, 0x0a /* Public */,
       8,    0,   52,    2, 0x0a /* Public */,
       9,    0,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ProcessTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ProcessTabWidget *_t = static_cast<ProcessTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateProcessNum((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onTableWidgetCustomContextMenuRequest((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 2: _t->onPopMenuRefresh(); break;
        case 3: _t->onPopMenuTraverseModule(); break;
        case 4: _t->onPopMenuTraverseThread(); break;
        case 5: _t->onPopMenuEndProcess(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ProcessTabWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ProcessTabWidget::updateProcessNum)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ProcessTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ProcessTabWidget.data,
      qt_meta_data_ProcessTabWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ProcessTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProcessTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ProcessTabWidget.stringdata0))
        return static_cast<void*>(const_cast< ProcessTabWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ProcessTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void ProcessTabWidget::updateProcessNum(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'SoftUninstallTabWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../SoftUninstallTabWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SoftUninstallTabWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SoftUninstallTabWidget_t {
    QByteArrayData data[5];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SoftUninstallTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SoftUninstallTabWidget_t qt_meta_stringdata_SoftUninstallTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 22), // "SoftUninstallTabWidget"
QT_MOC_LITERAL(1, 23, 37), // "onTableWidgetCustomContextMen..."
QT_MOC_LITERAL(2, 61, 0), // ""
QT_MOC_LITERAL(3, 62, 3), // "pos"
QT_MOC_LITERAL(4, 66, 15) // "onUninstallSoft"

    },
    "SoftUninstallTabWidget\0"
    "onTableWidgetCustomContextMenuRequest\0"
    "\0pos\0onUninstallSoft"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SoftUninstallTabWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    0,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,    3,
    QMetaType::Void,

       0        // eod
};

void SoftUninstallTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SoftUninstallTabWidget *_t = static_cast<SoftUninstallTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTableWidgetCustomContextMenuRequest((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 1: _t->onUninstallSoft(); break;
        default: ;
        }
    }
}

const QMetaObject SoftUninstallTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SoftUninstallTabWidget.data,
      qt_meta_data_SoftUninstallTabWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SoftUninstallTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SoftUninstallTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SoftUninstallTabWidget.stringdata0))
        return static_cast<void*>(const_cast< SoftUninstallTabWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SoftUninstallTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'ServiceTabWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../ServiceTabWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ServiceTabWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ServiceTabWidget_t {
    QByteArrayData data[7];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ServiceTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ServiceTabWidget_t qt_meta_stringdata_ServiceTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ServiceTabWidget"
QT_MOC_LITERAL(1, 17, 37), // "onTableWidgetCustomContextMen..."
QT_MOC_LITERAL(2, 55, 0), // ""
QT_MOC_LITERAL(3, 56, 3), // "pos"
QT_MOC_LITERAL(4, 60, 16), // "onPopMenuRefresh"
QT_MOC_LITERAL(5, 77, 21), // "onPopMenuStartService"
QT_MOC_LITERAL(6, 99, 20) // "onPopMenuStopService"

    },
    "ServiceTabWidget\0onTableWidgetCustomContextMenuRequest\0"
    "\0pos\0onPopMenuRefresh\0onPopMenuStartService\0"
    "onPopMenuStopService"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ServiceTabWidget[] = {

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
       1,    1,   34,    2, 0x0a /* Public */,
       4,    0,   37,    2, 0x0a /* Public */,
       5,    0,   38,    2, 0x0a /* Public */,
       6,    0,   39,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ServiceTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ServiceTabWidget *_t = static_cast<ServiceTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTableWidgetCustomContextMenuRequest((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 1: _t->onPopMenuRefresh(); break;
        case 2: _t->onPopMenuStartService(); break;
        case 3: _t->onPopMenuStopService(); break;
        default: ;
        }
    }
}

const QMetaObject ServiceTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ServiceTabWidget.data,
      qt_meta_data_ServiceTabWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ServiceTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServiceTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ServiceTabWidget.stringdata0))
        return static_cast<void*>(const_cast< ServiceTabWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ServiceTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    return _id;
}
QT_END_MOC_NAMESPACE

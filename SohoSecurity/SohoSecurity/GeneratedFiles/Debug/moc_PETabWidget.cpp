/****************************************************************************
** Meta object code from reading C++ file 'PETabWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../PETabWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PETabWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PETabWidget_t {
    QByteArrayData data[11];
    char stringdata0[237];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PETabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PETabWidget_t qt_meta_stringdata_PETabWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "PETabWidget"
QT_MOC_LITERAL(1, 12, 26), // "onPsBtnSectionTableClicked"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 22), // "onPsBtnResTableClicked"
QT_MOC_LITERAL(4, 63, 24), // "onPsBtnRelocTableClicked"
QT_MOC_LITERAL(5, 88, 27), // "onPsBtnOffsetCounterClicked"
QT_MOC_LITERAL(6, 116, 22), // "onPsBtnImpTableClicked"
QT_MOC_LITERAL(7, 139, 22), // "onPsBtnExpTableClicked"
QT_MOC_LITERAL(8, 162, 24), // "onPsBtnDelayTableClicked"
QT_MOC_LITERAL(9, 187, 26), // "onPsBtnDataDirTableClicked"
QT_MOC_LITERAL(10, 214, 22) // "onPsBtnTLSTableClicked"

    },
    "PETabWidget\0onPsBtnSectionTableClicked\0"
    "\0onPsBtnResTableClicked\0"
    "onPsBtnRelocTableClicked\0"
    "onPsBtnOffsetCounterClicked\0"
    "onPsBtnImpTableClicked\0onPsBtnExpTableClicked\0"
    "onPsBtnDelayTableClicked\0"
    "onPsBtnDataDirTableClicked\0"
    "onPsBtnTLSTableClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PETabWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a /* Public */,
       3,    0,   60,    2, 0x0a /* Public */,
       4,    0,   61,    2, 0x0a /* Public */,
       5,    0,   62,    2, 0x0a /* Public */,
       6,    0,   63,    2, 0x0a /* Public */,
       7,    0,   64,    2, 0x0a /* Public */,
       8,    0,   65,    2, 0x0a /* Public */,
       9,    0,   66,    2, 0x0a /* Public */,
      10,    0,   67,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PETabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PETabWidget *_t = static_cast<PETabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onPsBtnSectionTableClicked(); break;
        case 1: _t->onPsBtnResTableClicked(); break;
        case 2: _t->onPsBtnRelocTableClicked(); break;
        case 3: _t->onPsBtnOffsetCounterClicked(); break;
        case 4: _t->onPsBtnImpTableClicked(); break;
        case 5: _t->onPsBtnExpTableClicked(); break;
        case 6: _t->onPsBtnDelayTableClicked(); break;
        case 7: _t->onPsBtnDataDirTableClicked(); break;
        case 8: _t->onPsBtnTLSTableClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject PETabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PETabWidget.data,
      qt_meta_data_PETabWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PETabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PETabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PETabWidget.stringdata0))
        return static_cast<void*>(const_cast< PETabWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PETabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

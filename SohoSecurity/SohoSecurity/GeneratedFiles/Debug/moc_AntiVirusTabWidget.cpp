/****************************************************************************
** Meta object code from reading C++ file 'AntiVirusTabWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../AntiVirusTabWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AntiVirusTabWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AntiVirusTabWidget_t {
    QByteArrayData data[14];
    char stringdata0[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AntiVirusTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AntiVirusTabWidget_t qt_meta_stringdata_AntiVirusTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "AntiVirusTabWidget"
QT_MOC_LITERAL(1, 19, 16), // "onSelectScanPath"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 21), // "onPsBtnLocalAVClicked"
QT_MOC_LITERAL(4, 59, 21), // "onPsBtnCloudAVClicked"
QT_MOC_LITERAL(5, 81, 22), // "onPsBtnProcScanClicked"
QT_MOC_LITERAL(6, 104, 25), // "onUpdateLocalScanProgress"
QT_MOC_LITERAL(7, 130, 11), // "strFilePath"
QT_MOC_LITERAL(8, 142, 12), // "onCatchVirus"
QT_MOC_LITERAL(9, 155, 24), // "onUpdateProcScanProgress"
QT_MOC_LITERAL(10, 180, 11), // "strProcName"
QT_MOC_LITERAL(11, 192, 19), // "onCatchVirusProcess"
QT_MOC_LITERAL(12, 212, 7), // "strPath"
QT_MOC_LITERAL(13, 220, 8) // "strState"

    },
    "AntiVirusTabWidget\0onSelectScanPath\0"
    "\0onPsBtnLocalAVClicked\0onPsBtnCloudAVClicked\0"
    "onPsBtnProcScanClicked\0onUpdateLocalScanProgress\0"
    "strFilePath\0onCatchVirus\0"
    "onUpdateProcScanProgress\0strProcName\0"
    "onCatchVirusProcess\0strPath\0strState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AntiVirusTabWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    1,   58,    2, 0x0a /* Public */,
       8,    1,   61,    2, 0x0a /* Public */,
       9,    1,   64,    2, 0x0a /* Public */,
      11,    3,   67,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,   12,   13,

       0        // eod
};

void AntiVirusTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AntiVirusTabWidget *_t = static_cast<AntiVirusTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onSelectScanPath(); break;
        case 1: _t->onPsBtnLocalAVClicked(); break;
        case 2: _t->onPsBtnCloudAVClicked(); break;
        case 3: _t->onPsBtnProcScanClicked(); break;
        case 4: _t->onUpdateLocalScanProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->onCatchVirus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->onUpdateProcScanProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->onCatchVirusProcess((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject AntiVirusTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AntiVirusTabWidget.data,
      qt_meta_data_AntiVirusTabWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AntiVirusTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AntiVirusTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AntiVirusTabWidget.stringdata0))
        return static_cast<void*>(const_cast< AntiVirusTabWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AntiVirusTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

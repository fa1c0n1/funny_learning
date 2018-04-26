/****************************************************************************
** Meta object code from reading C++ file 'CleanTrashTabWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../CleanTrashTabWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CleanTrashTabWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CleanTrashTabWidget_t {
    QByteArrayData data[12];
    char stringdata0[275];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CleanTrashTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CleanTrashTabWidget_t qt_meta_stringdata_CleanTrashTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "CleanTrashTabWidget"
QT_MOC_LITERAL(1, 20, 27), // "onPsBtnClnSysAndBrwsClicked"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 24), // "onpsBtnCleanVSprjClicked"
QT_MOC_LITERAL(4, 74, 24), // "onUpdateClnVSPrjProgress"
QT_MOC_LITERAL(5, 99, 11), // "strFilePath"
QT_MOC_LITERAL(6, 111, 27), // "onUpdateClnTmpFilesProgress"
QT_MOC_LITERAL(7, 139, 27), // "onUpdateClnSysFilesProgress"
QT_MOC_LITERAL(8, 167, 29), // "onUpdateClnRecycleBinProgress"
QT_MOC_LITERAL(9, 197, 27), // "onUpdateClnSysCacheProgress"
QT_MOC_LITERAL(10, 225, 22), // "onUpdateClnLogProgress"
QT_MOC_LITERAL(11, 248, 26) // "onUpdateClnBrowserProgress"

    },
    "CleanTrashTabWidget\0onPsBtnClnSysAndBrwsClicked\0"
    "\0onpsBtnCleanVSprjClicked\0"
    "onUpdateClnVSPrjProgress\0strFilePath\0"
    "onUpdateClnTmpFilesProgress\0"
    "onUpdateClnSysFilesProgress\0"
    "onUpdateClnRecycleBinProgress\0"
    "onUpdateClnSysCacheProgress\0"
    "onUpdateClnLogProgress\0"
    "onUpdateClnBrowserProgress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CleanTrashTabWidget[] = {

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
       4,    1,   61,    2, 0x0a /* Public */,
       6,    1,   64,    2, 0x0a /* Public */,
       7,    1,   67,    2, 0x0a /* Public */,
       8,    1,   70,    2, 0x0a /* Public */,
       9,    1,   73,    2, 0x0a /* Public */,
      10,    1,   76,    2, 0x0a /* Public */,
      11,    1,   79,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void CleanTrashTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CleanTrashTabWidget *_t = static_cast<CleanTrashTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onPsBtnClnSysAndBrwsClicked(); break;
        case 1: _t->onpsBtnCleanVSprjClicked(); break;
        case 2: _t->onUpdateClnVSPrjProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->onUpdateClnTmpFilesProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->onUpdateClnSysFilesProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->onUpdateClnRecycleBinProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->onUpdateClnSysCacheProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->onUpdateClnLogProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->onUpdateClnBrowserProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CleanTrashTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CleanTrashTabWidget.data,
      qt_meta_data_CleanTrashTabWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CleanTrashTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CleanTrashTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CleanTrashTabWidget.stringdata0))
        return static_cast<void*>(const_cast< CleanTrashTabWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CleanTrashTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

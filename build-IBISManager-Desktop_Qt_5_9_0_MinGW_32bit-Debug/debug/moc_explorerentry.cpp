/****************************************************************************
** Meta object code from reading C++ file 'explorerentry.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IBISManager/explorerentry.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'explorerentry.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ExplorerEntry_t {
    QByteArrayData data[26];
    char stringdata0[233];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ExplorerEntry_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ExplorerEntry_t qt_meta_stringdata_ExplorerEntry = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ExplorerEntry"
QT_MOC_LITERAL(1, 14, 11), // "textChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 4), // "text"
QT_MOC_LITERAL(4, 32, 19), // "symbolSourceChanged"
QT_MOC_LITERAL(5, 52, 12), // "symbolSource"
QT_MOC_LITERAL(6, 65, 12), // "layerChanged"
QT_MOC_LITERAL(7, 78, 5), // "layer"
QT_MOC_LITERAL(8, 84, 14), // "visibleChanged"
QT_MOC_LITERAL(9, 99, 7), // "visible"
QT_MOC_LITERAL(10, 107, 11), // "openChanged"
QT_MOC_LITERAL(11, 119, 4), // "open"
QT_MOC_LITERAL(12, 124, 10), // "hidChanged"
QT_MOC_LITERAL(13, 135, 3), // "hid"
QT_MOC_LITERAL(14, 139, 7), // "setText"
QT_MOC_LITERAL(15, 147, 5), // "_text"
QT_MOC_LITERAL(16, 153, 9), // "setSource"
QT_MOC_LITERAL(17, 163, 7), // "_source"
QT_MOC_LITERAL(18, 171, 8), // "setLayer"
QT_MOC_LITERAL(19, 180, 6), // "_layer"
QT_MOC_LITERAL(20, 187, 10), // "setVisible"
QT_MOC_LITERAL(21, 198, 8), // "_visible"
QT_MOC_LITERAL(22, 207, 7), // "setOpen"
QT_MOC_LITERAL(23, 215, 5), // "_open"
QT_MOC_LITERAL(24, 221, 6), // "setHid"
QT_MOC_LITERAL(25, 228, 4) // "_hid"

    },
    "ExplorerEntry\0textChanged\0\0text\0"
    "symbolSourceChanged\0symbolSource\0"
    "layerChanged\0layer\0visibleChanged\0"
    "visible\0openChanged\0open\0hidChanged\0"
    "hid\0setText\0_text\0setSource\0_source\0"
    "setLayer\0_layer\0setVisible\0_visible\0"
    "setOpen\0_open\0setHid\0_hid"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ExplorerEntry[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       6,  110, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       6,    1,   80,    2, 0x06 /* Public */,
       8,    1,   83,    2, 0x06 /* Public */,
      10,    1,   86,    2, 0x06 /* Public */,
      12,    1,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    1,   92,    2, 0x0a /* Public */,
      16,    1,   95,    2, 0x0a /* Public */,
      18,    1,   98,    2, 0x0a /* Public */,
      20,    1,  101,    2, 0x0a /* Public */,
      22,    1,  104,    2, 0x0a /* Public */,
      24,    1,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Int,   13,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::Bool,   21,
    QMetaType::Void, QMetaType::Bool,   23,
    QMetaType::Void, QMetaType::Int,   13,

 // properties: name, type, flags
      25, QMetaType::Int, 0x00495003,
      15, QMetaType::QString, 0x00495003,
      17, QMetaType::QString, 0x00495003,
      19, QMetaType::Int, 0x00495003,
      21, QMetaType::Bool, 0x00495003,
      23, QMetaType::Bool, 0x00495003,

 // properties: notify_signal_id
       5,
       0,
       1,
       2,
       3,
       4,

       0        // eod
};

void ExplorerEntry::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ExplorerEntry *_t = static_cast<ExplorerEntry *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->textChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->symbolSourceChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->layerChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->visibleChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->openChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->hidChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setSource((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->setLayer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->setOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->setHid((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ExplorerEntry::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ExplorerEntry::textChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ExplorerEntry::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ExplorerEntry::symbolSourceChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ExplorerEntry::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ExplorerEntry::layerChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ExplorerEntry::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ExplorerEntry::visibleChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ExplorerEntry::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ExplorerEntry::openChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (ExplorerEntry::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ExplorerEntry::hidChanged)) {
                *result = 5;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        ExplorerEntry *_t = static_cast<ExplorerEntry *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->_hid(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->_text(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->_source(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->_layer(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->_visible(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->_open(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        ExplorerEntry *_t = static_cast<ExplorerEntry *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setHid(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setText(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setSource(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setLayer(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setVisible(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->setOpen(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject ExplorerEntry::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ExplorerEntry.data,
      qt_meta_data_ExplorerEntry,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ExplorerEntry::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ExplorerEntry::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ExplorerEntry.stringdata0))
        return static_cast<void*>(const_cast< ExplorerEntry*>(this));
    return QObject::qt_metacast(_clname);
}

int ExplorerEntry::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ExplorerEntry::textChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ExplorerEntry::symbolSourceChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ExplorerEntry::layerChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ExplorerEntry::visibleChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ExplorerEntry::openChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ExplorerEntry::hidChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

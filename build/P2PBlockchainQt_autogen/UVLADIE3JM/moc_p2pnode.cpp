/****************************************************************************
** Meta object code from reading C++ file 'p2pnode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/p2pnode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'p2pnode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_P2PNode_t {
    QByteArrayData data[21];
    char stringdata0[271];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_P2PNode_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_P2PNode_t qt_meta_stringdata_P2PNode = {
    {
QT_MOC_LITERAL(0, 0, 7), // "P2PNode"
QT_MOC_LITERAL(1, 8, 13), // "peerConnected"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "peerInfo"
QT_MOC_LITERAL(4, 32, 16), // "peerDisconnected"
QT_MOC_LITERAL(5, 49, 20), // "peerConnectionFailed"
QT_MOC_LITERAL(6, 70, 6), // "reason"
QT_MOC_LITERAL(7, 77, 22), // "newTransactionReceived"
QT_MOC_LITERAL(8, 100, 11), // "Transaction"
QT_MOC_LITERAL(9, 112, 2), // "tx"
QT_MOC_LITERAL(10, 115, 16), // "newBlockReceived"
QT_MOC_LITERAL(11, 132, 5), // "Block"
QT_MOC_LITERAL(12, 138, 5), // "block"
QT_MOC_LITERAL(13, 144, 12), // "chainUpdated"
QT_MOC_LITERAL(14, 157, 15), // "onNewConnection"
QT_MOC_LITERAL(15, 173, 11), // "onConnected"
QT_MOC_LITERAL(16, 185, 17), // "onConnectionError"
QT_MOC_LITERAL(17, 203, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(18, 232, 11), // "socketError"
QT_MOC_LITERAL(19, 244, 11), // "onReadyRead"
QT_MOC_LITERAL(20, 256, 14) // "onDisconnected"

    },
    "P2PNode\0peerConnected\0\0peerInfo\0"
    "peerDisconnected\0peerConnectionFailed\0"
    "reason\0newTransactionReceived\0Transaction\0"
    "tx\0newBlockReceived\0Block\0block\0"
    "chainUpdated\0onNewConnection\0onConnected\0"
    "onConnectionError\0QAbstractSocket::SocketError\0"
    "socketError\0onReadyRead\0onDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_P2PNode[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       5,    2,   75,    2, 0x06 /* Public */,
       7,    1,   80,    2, 0x06 /* Public */,
      10,    1,   83,    2, 0x06 /* Public */,
      13,    0,   86,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   87,    2, 0x08 /* Private */,
      15,    0,   88,    2, 0x08 /* Private */,
      16,    1,   89,    2, 0x08 /* Private */,
      19,    0,   92,    2, 0x08 /* Private */,
      20,    0,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void P2PNode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<P2PNode *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->peerConnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->peerDisconnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->peerConnectionFailed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->newTransactionReceived((*reinterpret_cast< const Transaction(*)>(_a[1]))); break;
        case 4: _t->newBlockReceived((*reinterpret_cast< const Block(*)>(_a[1]))); break;
        case 5: _t->chainUpdated(); break;
        case 6: _t->onNewConnection(); break;
        case 7: _t->onConnected(); break;
        case 8: _t->onConnectionError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 9: _t->onReadyRead(); break;
        case 10: _t->onDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (P2PNode::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&P2PNode::peerConnected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (P2PNode::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&P2PNode::peerDisconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (P2PNode::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&P2PNode::peerConnectionFailed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (P2PNode::*)(const Transaction & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&P2PNode::newTransactionReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (P2PNode::*)(const Block & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&P2PNode::newBlockReceived)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (P2PNode::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&P2PNode::chainUpdated)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject P2PNode::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_P2PNode.data,
    qt_meta_data_P2PNode,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *P2PNode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *P2PNode::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_P2PNode.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int P2PNode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void P2PNode::peerConnected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void P2PNode::peerDisconnected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void P2PNode::peerConnectionFailed(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void P2PNode::newTransactionReceived(const Transaction & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void P2PNode::newBlockReceived(const Block & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void P2PNode::chainUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

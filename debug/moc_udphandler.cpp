/****************************************************************************
** Meta object code from reading C++ file 'udphandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../udphandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udphandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UdpHandler_t {
    QByteArrayData data[22];
    char stringdata0[325];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UdpHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UdpHandler_t qt_meta_stringdata_UdpHandler = {
    {
QT_MOC_LITERAL(0, 0, 10), // "UdpHandler"
QT_MOC_LITERAL(1, 11, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 9), // "connected"
QT_MOC_LITERAL(4, 46, 20), // "newDetectionReceived"
QT_MOC_LITERAL(5, 67, 13), // "DetectionData"
QT_MOC_LITERAL(6, 81, 9), // "detection"
QT_MOC_LITERAL(7, 91, 17), // "detectionsUpdated"
QT_MOC_LITERAL(8, 109, 13), // "errorOccurred"
QT_MOC_LITERAL(9, 123, 5), // "error"
QT_MOC_LITERAL(10, 129, 17), // "statisticsUpdated"
QT_MOC_LITERAL(11, 147, 15), // "packetsReceived"
QT_MOC_LITERAL(12, 163, 14), // "packetsDropped"
QT_MOC_LITERAL(13, 178, 8), // "dataRate"
QT_MOC_LITERAL(14, 187, 15), // "dspSettingsSent"
QT_MOC_LITERAL(15, 203, 7), // "success"
QT_MOC_LITERAL(16, 211, 20), // "readPendingDatagrams"
QT_MOC_LITERAL(17, 232, 20), // "cleanupOldDetections"
QT_MOC_LITERAL(18, 253, 16), // "updateStatistics"
QT_MOC_LITERAL(19, 270, 13), // "onSocketError"
QT_MOC_LITERAL(20, 284, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(21, 313, 11) // "socketError"

    },
    "UdpHandler\0connectionStatusChanged\0\0"
    "connected\0newDetectionReceived\0"
    "DetectionData\0detection\0detectionsUpdated\0"
    "errorOccurred\0error\0statisticsUpdated\0"
    "packetsReceived\0packetsDropped\0dataRate\0"
    "dspSettingsSent\0success\0readPendingDatagrams\0"
    "cleanupOldDetections\0updateStatistics\0"
    "onSocketError\0QAbstractSocket::SocketError\0"
    "socketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UdpHandler[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       7,    0,   70,    2, 0x06 /* Public */,
       8,    1,   71,    2, 0x06 /* Public */,
      10,    3,   74,    2, 0x06 /* Public */,
      14,    1,   81,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    0,   84,    2, 0x08 /* Private */,
      17,    0,   85,    2, 0x08 /* Private */,
      18,    0,   86,    2, 0x08 /* Private */,
      19,    1,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double,   11,   12,   13,
    QMetaType::Void, QMetaType::Bool,   15,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void UdpHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UdpHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->newDetectionReceived((*reinterpret_cast< const DetectionData(*)>(_a[1]))); break;
        case 2: _t->detectionsUpdated(); break;
        case 3: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->statisticsUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 5: _t->dspSettingsSent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->readPendingDatagrams(); break;
        case 7: _t->cleanupOldDetections(); break;
        case 8: _t->updateStatistics(); break;
        case 9: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
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
            using _t = void (UdpHandler::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpHandler::connectionStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UdpHandler::*)(const DetectionData & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpHandler::newDetectionReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UdpHandler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpHandler::detectionsUpdated)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UdpHandler::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpHandler::errorOccurred)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (UdpHandler::*)(int , int , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpHandler::statisticsUpdated)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (UdpHandler::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpHandler::dspSettingsSent)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UdpHandler::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_UdpHandler.data,
    qt_meta_data_UdpHandler,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UdpHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UdpHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UdpHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UdpHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void UdpHandler::connectionStatusChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UdpHandler::newDetectionReceived(const DetectionData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UdpHandler::detectionsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void UdpHandler::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void UdpHandler::statisticsUpdated(int _t1, int _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void UdpHandler::dspSettingsSent(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'communicationtool.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../communicationtool.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communicationtool.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11P2PChatToolE_t {};
} // unnamed namespace

template <> constexpr inline auto P2PChatTool::qt_create_metaobjectdata<qt_meta_tag_ZN11P2PChatToolE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "P2PChatTool",
        "messageReceived",
        "",
        "message",
        "connectionEstablished",
        "userDiscovered",
        "name",
        "QHostAddress",
        "ip",
        "port",
        "inviteReceived",
        "from",
        "subject",
        "QJsonArray",
        "timeSlots",
        "inviteAccepted",
        "chosenTime",
        "onNewConnection",
        "onReadyRead",
        "processDiscoveryDatagrams"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'messageReceived'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'connectionEstablished'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'userDiscovered'
        QtMocHelpers::SignalData<void(const QString &, const QHostAddress &, quint16)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 }, { 0x80000000 | 7, 8 }, { QMetaType::UShort, 9 },
        }}),
        // Signal 'inviteReceived'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QJsonArray &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 }, { QMetaType::QString, 12 }, { 0x80000000 | 13, 14 },
        }}),
        // Signal 'inviteAccepted'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 }, { QMetaType::QString, 16 },
        }}),
        // Slot 'onNewConnection'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReadyRead'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'processDiscoveryDatagrams'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<P2PChatTool, qt_meta_tag_ZN11P2PChatToolE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject P2PChatTool::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11P2PChatToolE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11P2PChatToolE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11P2PChatToolE_t>.metaTypes,
    nullptr
} };

void P2PChatTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<P2PChatTool *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->messageReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->connectionEstablished(); break;
        case 2: _t->userDiscovered((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QHostAddress>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<quint16>>(_a[3]))); break;
        case 3: _t->inviteReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QJsonArray>>(_a[3]))); break;
        case 4: _t->inviteAccepted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->onNewConnection(); break;
        case 6: _t->onReadyRead(); break;
        case 7: _t->processDiscoveryDatagrams(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (P2PChatTool::*)(const QString & )>(_a, &P2PChatTool::messageReceived, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (P2PChatTool::*)()>(_a, &P2PChatTool::connectionEstablished, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (P2PChatTool::*)(const QString & , const QHostAddress & , quint16 )>(_a, &P2PChatTool::userDiscovered, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (P2PChatTool::*)(const QString & , const QString & , const QJsonArray & )>(_a, &P2PChatTool::inviteReceived, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (P2PChatTool::*)(const QString & , const QString & )>(_a, &P2PChatTool::inviteAccepted, 4))
            return;
    }
}

const QMetaObject *P2PChatTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *P2PChatTool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11P2PChatToolE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int P2PChatTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void P2PChatTool::messageReceived(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void P2PChatTool::connectionEstablished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void P2PChatTool::userDiscovered(const QString & _t1, const QHostAddress & _t2, quint16 _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3);
}

// SIGNAL 3
void P2PChatTool::inviteReceived(const QString & _t1, const QString & _t2, const QJsonArray & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3);
}

// SIGNAL 4
void P2PChatTool::inviteAccepted(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}
QT_WARNING_POP

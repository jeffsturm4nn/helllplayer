/****************************************************************************
** Meta object code from reading C++ file 'HelllMain.hpp'
**
** Created: Wed 20. Jun 02:30:01 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "HelllMain.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HelllMain.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HELLL__HelllMain[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      31,   17,   17,   17, 0x08,
      44,   17,   17,   17, 0x08,
      56,   17,   17,   17, 0x08,
      71,   17,   17,   17, 0x08,
      81,   17,   17,   17, 0x08,
      93,   17,   17,   17, 0x08,
     106,   17,   17,   17, 0x08,
     121,   17,   17,   17, 0x08,
     134,   17,   17,   17, 0x08,
     150,   17,   17,   17, 0x08,
     168,   17,   17,   17, 0x08,
     181,   17,   17,   17, 0x08,
     196,   17,   17,   17, 0x08,
     212,   17,   17,   17, 0x08,
     230,   17,   17,   17, 0x08,
     245,   17,   17,   17, 0x08,
     262,   17,   17,   17, 0x08,
     281,   17,   17,   17, 0x08,
     296,   17,   17,   17, 0x08,
     312,   17,   17,   17, 0x08,
     330,   17,   17,   17, 0x08,
     345,   17,   17,   17, 0x08,
     362,   17,   17,   17, 0x08,
     382,   17,   17,   17, 0x08,
     400,   17,   17,   17, 0x08,
     417,   17,   17,   17, 0x08,
     449,   17,  444,   17, 0x08,
     461,   17,  444,   17, 0x08,
     477,   17,   17,   17, 0x08,
     493,   17,   17,   17, 0x08,
     505,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HELLL__HelllMain[] = {
    "HELLL::HelllMain\0\0updateTime()\0"
    "getSysInfo()\0loadFiles()\0setPlayPause()\0"
    "setStop()\0setVolume()\0setPBSpeed()\0"
    "setSliderPos()\0setPlayPos()\0enableWReverb()\0"
    "setWReverbOnOff()\0enableEcho()\0"
    "setEchoOnOff()\0enableFlanger()\0"
    "setFlangerOnOff()\0enableGargle()\0"
    "setGargleOnOff()\0enableCompressor()\0"
    "setCompOnOff()\0enableDistort()\0"
    "setDistortOnOff()\0enableChorus()\0"
    "setChorusOnOff()\0disableAllEffects()\0"
    "addTrack(QString)\0deleteSelected()\0"
    "setTrack(QListWidgetItem*)\0bool\0"
    "nextTrack()\0previousTrack()\0checkKeyboard()\0"
    "loadPList()\0savePList()\0"
};

void HELLL::HelllMain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HelllMain *_t = static_cast<HelllMain *>(_o);
        switch (_id) {
        case 0: _t->updateTime(); break;
        case 1: _t->getSysInfo(); break;
        case 2: _t->loadFiles(); break;
        case 3: _t->setPlayPause(); break;
        case 4: _t->setStop(); break;
        case 5: _t->setVolume(); break;
        case 6: _t->setPBSpeed(); break;
        case 7: _t->setSliderPos(); break;
        case 8: _t->setPlayPos(); break;
        case 9: _t->enableWReverb(); break;
        case 10: _t->setWReverbOnOff(); break;
        case 11: _t->enableEcho(); break;
        case 12: _t->setEchoOnOff(); break;
        case 13: _t->enableFlanger(); break;
        case 14: _t->setFlangerOnOff(); break;
        case 15: _t->enableGargle(); break;
        case 16: _t->setGargleOnOff(); break;
        case 17: _t->enableCompressor(); break;
        case 18: _t->setCompOnOff(); break;
        case 19: _t->enableDistort(); break;
        case 20: _t->setDistortOnOff(); break;
        case 21: _t->enableChorus(); break;
        case 22: _t->setChorusOnOff(); break;
        case 23: _t->disableAllEffects(); break;
        case 24: _t->addTrack((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->deleteSelected(); break;
        case 26: _t->setTrack((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 27: { bool _r = _t->nextTrack();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 28: { bool _r = _t->previousTrack();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 29: _t->checkKeyboard(); break;
        case 30: _t->loadPList(); break;
        case 31: _t->savePList(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HELLL::HelllMain::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HELLL::HelllMain::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_HELLL__HelllMain,
      qt_meta_data_HELLL__HelllMain, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HELLL::HelllMain::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HELLL::HelllMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HELLL::HelllMain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HELLL__HelllMain))
        return static_cast<void*>(const_cast< HelllMain*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int HELLL::HelllMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

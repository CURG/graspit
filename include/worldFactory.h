#ifndef WORLD_FACTORY_H
#define WORLD_FACTORY_H


#include <QObject>

class World;
class IVmgr;

namespace WorldFactory {


    // TODO: Change this to std::unique_ptr once project is changed to c++11
    World *newEmptyWorld(QObject *parent=0,const char *name=0, IVmgr *mgr=NULL);

}

#endif

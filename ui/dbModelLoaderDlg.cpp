#include "dbModelLoaderDlg.h"

#include <QValidator>
#include <QString>
#include <world.h>

void DbModelLoaderDlg::init()	{
}


void DbModelLoaderDlg::loadButton_clicked() {
    QString fn("random_filename");
    world->importBodyFromBuffer("GraspableBody", fn);
}


 void DbModelLoaderDlg::setMembers(World *_world) {

    world = _world;
 }

#include "dbModelLoaderDlg.h"

#include <QValidator>
#include <QString>
#include <world.h>
#include <fileDownloader.h>

#include "qjson4/QJsonArray.h"
#include "qjson4/QJsonDocument.h"
#include "qjson4/QJsonObject.h"
#include "qjson4/QJsonParseError.h"

void DbModelLoaderDlg::init()	{
}


void DbModelLoaderDlg::loadButton_clicked() {
    QString fn("random_filename");
    world->importBodyFromBuffer("GraspableBody", fn);
}


 void DbModelLoaderDlg::setMembers(World *_world) {

    world = _world;
 }


 void DbModelLoaderDlg::setCategories() {

     QStringList categories;
     SyncHTTP h("borneo.cs.columbia.edu", 8080);
     QString url_path = "/models/metas/category?access_token=robolabapi";

     QBuffer output;
     bool status = h.syncGet(url_path,&output);
     if(!status)
     {
         return;
     }

     QByteArray bytes = output.readAll();

     QJsonParseError error;
     QJsonDocument doc = QJsonDocument::fromJson(bytes, &error);
     QJsonArray arr;

     // check validity of the document
     if(!doc.isNull())
     {
         if(doc.isArray())
         {
             arr = doc.array();
         }
         else
         {
             std::cout  << "Document is not an array" << std::endl;
         }
     }
     else
     {
        std::cout<<error.errorString().toStdString().c_str() << std::endl;
     }

     QVariantList list = arr.toVariantList();
      for (int i = 0; i <list.size(); i++) {

          categories.append(list[i].toString());
      }

     categoriesComboBox->addItems(categories);
 }

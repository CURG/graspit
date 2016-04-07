#include "dbModelLoader.h"

#include <QValidator>
#include <QString>
#include <world.h>
#include <body.h>
#include <fileDownloader.h>

#include "qjson4/QJsonArray.h"
#include "qjson4/QJsonDocument.h"
#include "qjson4/QJsonObject.h"
#include "qjson4/QJsonParseError.h"
#include <debug.h>

 void DbModelLoader::setMembers(World *_world) {

    world = _world;
 }

 void DbModelLoader::setEnv() {
     apiUrl = getenv("API_URL");
     if(apiUrl == "") {
         std::cout << "Setting API_URL to default borneo.cs.columbia.edu:8080" << std::endl;
         apiUrl = "borneo.cs.columbia.edu";
         apiPort = 8080;
     } else {

         QUrl url(apiUrl);
         apiUrl = url.host();
         apiPort = url.port();
         if(apiPort <= 0){
             apiPort = 80;

         }


     }

     apiKey = getenv("API_KEY");
}

 QStringList DbModelLoader::getCategories() {

     QStringList categories;
     SyncHTTP h(apiUrl, apiPort);
     QString url_path = "/models/metas/category?access_token=" + apiKey;

     QBuffer output;
     bool status = h.syncGet(url_path,&output);
     if(!status)
     {
         return categories;
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

     return categories;
 }


 QStringList DbModelLoader::getModels(const QString &category) {

     models = QJsonObject();
     QStringList modelNames;
     SyncHTTP h(apiUrl, apiPort);
     QString url_path = "/models?category=" + category + "&access_token=" + apiKey;

     QBuffer output;
     bool status = h.syncGet(url_path,&output);
     if(!status)
     {
         return modelNames;
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

     for(int i = 0; i < arr.size(); i++) {
        QString modelName = arr[i].toObject()["name"].toString();
        models[modelName] = arr[i].toObject()["url"].toString();
        modelNames.append(modelName);
     }

     return modelNames;
 }

 void DbModelLoader::loadModel(const QString &modelName) {
     QString url = models[modelName].toString();

     std::cout << url.toStdString().c_str() << std::endl;
     Body *b = world->importBodyFromBuffer("GraspableBody", url);
     //b->getIVScaleTran()
     //b->setGeometryScaling(1000,1000,1000);
 }

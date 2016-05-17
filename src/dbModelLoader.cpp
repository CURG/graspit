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

#include "graspitGUI.h"
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include "ivmgr.h"

 void DbModelLoader::setEnv() {
     apiUrl = getenv("API_URL");
     if(apiUrl == "") {
         std::cerr << "WARNING: Setting API_URL to default borneo.cs.columbia.edu:8080" << std::endl;
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
     if(apiKey == "") {
         std::cerr << "ERROR: ENV API_KEY not set" << std::endl;
     }

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


QJsonObject DbModelLoader::loadRandomModel() {


     SyncHTTP h(apiUrl, apiPort);
     QString url_path = "/grasps/random_combo?access_token=" + apiKey;

     QBuffer output;
     bool status = h.syncGet(url_path,&output);
     if(!status) {
        std::cout << "Failed to get model from " << url_path.toStdString().c_str() << std::endl;

     }

     QByteArray bytes = output.readAll();

     QJsonParseError error;
     QJsonDocument doc = QJsonDocument::fromJson(bytes, &error);
     QJsonObject object;

     // check validity of the document
     if(!doc.isNull()) {
         if(doc.isObject()) {
             object = doc.object();
         }
         else {
             std::cout  << "Document is not an object" << std::endl;
         }
     } else {
        std::cout<<error.errorString().toStdString().c_str() << std::endl;
     }


     QString hand = object["hand"].toString();
     QString url = object["model"].toObject()["url"].toString();
     QString name = object["model"].toObject()["name"].toString();
     QString material = object["model"].toObject()["material"].toString();

    double dimension = object["model"].toObject()["dimension"].toDouble();
    QString robotPath = QString(getenv("GRASPIT")) + QString("/models/robots/") + hand + QString("/") + hand + QString(".xml");


    std::cout << url.toStdString().c_str() << std::endl;

    graspItGUI->getMainWorld()->importRobot(robotPath.toStdString().c_str());

    dimension = 125;
    loadModelFromUrl(url, name, material, dimension);

    return object["model"].toObject();
 }

 void DbModelLoader::loadModelFromName(const QString &modelName) {
    QString url = models[modelName].toString();
    double dimension = 100;
    loadModelFromUrl(url, modelName, QString("rubber"), dimension);
 }

 void DbModelLoader::loadModelFromUrl(const QString &url, const QString &modelName, const QString &material, double dimension) {

     std::cout << url.toStdString().c_str() << std::endl;
     Body *b = world->importBodyFromBuffer("GraspableBody", url);

     SoGetBoundingBoxAction *bba =
         new SoGetBoundingBoxAction(graspItGUI->getIVmgr()->getViewer()->getViewportRegion());
     bba->apply(b->getIVGeomRoot());
     SbVec3f bbmin,bbmax;
     bba->getBoundingBox().getBounds(bbmin, bbmax);
     delete bba;
     double a1 = (bbmax[0] - bbmin[0]);
     double b1 = (bbmax[1] - bbmin[1]);
     double c1 = (bbmax[2] - bbmin[2]);
     double largest_dim = std::max(std::max(a1, b1), c1);
//     double smallest_dim = std::min(std::min(a1,b1), c1);
     double average_dim = (a1 + b1 +c1)/ 3.0;

//     double x = dimension/largest_dim;
//     double y = dimension/largest_dim;
//     double z = dimension/largest_dim;

     double x = dimension/average_dim;
     double y = dimension/average_dim;
     double z = dimension/average_dim;

//     double x = dimension/smallest_dim;
//     double y = dimension/smallest_dim;
//     double z = dimension/smallest_dim;
     //want largest dim to be 10cm
     //
     b->setGeometryScaling(x, y, z);
     b->setMaterial(world->getMaterialIdx(material));
     b->setName(modelName);
     ((GraspableBody *)b)->setMass(100.0);

 }

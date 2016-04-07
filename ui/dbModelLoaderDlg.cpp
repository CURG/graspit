#include "dbModelLoaderDlg.h"

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

void DbModelLoaderDlg::init()	{
}


void DbModelLoaderDlg::loadButton_clicked() {
    QString modelName = modelsComboBox->currentText();
    std::cout << modelName.toStdString().c_str() << std::endl;
    loadModel(modelName);
    close();
}

 void DbModelLoaderDlg::categoryComboBox_clicked(const QString &category) {
//    std::cout << category.toStdString().c_str() <<std::endl;
    setModels(category);
 }

 void DbModelLoaderDlg::setMembers(World *_world) {

    world = _world;
 }

 void DbModelLoaderDlg::setEnv() {
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

 void DbModelLoaderDlg::setCategories() {

     QStringList categories;
     SyncHTTP h(apiUrl, apiPort);
     QString url_path = "/models/metas/category?access_token=" + apiKey;

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


 void DbModelLoaderDlg::setModels(const QString &category) {

//     models = NULL;
     models = QJsonObject();

     SyncHTTP h(apiUrl, apiPort);
     QString url_path = "/models?category=" + category + "&access_token=" + apiKey;

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

     modelsComboBox->clear();
     for(int i = 0; i < arr.size(); i++) {
        QString modelName = arr[i].toObject()["name"].toString();
        models[modelName] = arr[i].toObject()["url"].toString();

        modelsComboBox->addItem(modelName);
//        std::cout << modelName.toStdString().c_str() << std::endl;
//        std::cout << models[modelName].toString().toStdString().c_str() << std::endl;

     }
 }

 void DbModelLoaderDlg::loadModel(const QString &modelName) {
     QString url = models[modelName].toString();

    std::cout << url.toStdString().c_str() << std::endl;
     Body *b = world->importBodyFromBuffer("GraspableBody", url);
     //b->getIVScaleTran()
     //b->setGeometryScaling(1000,1000,1000);
 }

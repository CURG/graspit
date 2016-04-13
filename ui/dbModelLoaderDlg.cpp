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
    loader.loadModelFromName(modelName);
    close();
}

 void DbModelLoaderDlg::categoryComboBox_clicked(const QString &category) {
//    std::cout << category.toStdString().c_str() <<std::endl;
    setModels(category);
 }

 void DbModelLoaderDlg::setCategories() {

     QStringList categories = this->loader.getCategories();

     categoriesComboBox->addItems(categories);
 }


 void DbModelLoaderDlg::setModels(const QString &category) {


    QStringList modelNames = loader.getModels(category);
     modelsComboBox->clear();
     for(int i = 0; i < modelNames.size(); i++) {

         modelsComboBox->addItem(modelNames[i]);
     }
 }

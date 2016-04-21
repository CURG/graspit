#ifndef _dbModelLoader_h_
#define _dbModelLoader_h_

#include <QDialog>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject.h>
#include <iostream>
#include <QUrl>
#include "include/graspitGUI.h"

class World;
class DbModelLoader
{
private:
    World *world;

    QString apiUrl;
    QString apiKey;
    int apiPort;
protected:
    void setEnv();
public:

    DbModelLoader() : world(NULL){

        world = graspItGUI->getMainWorld();
        setEnv();
//        getCategories();

    }
    QJsonObject models;
    QStringList getCategories();
    QStringList getModels(const QString &category);
    void loadModelFromName(const QString &modelName);
    void loadModelFromUrl(const QString &url, const QString &modelName, const QString &material, double dimension);
    QJsonObject loadRandomModel();
};


#endif

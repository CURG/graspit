#ifndef _dbModelLoader_h_
#define _dbModelLoader_h_

#include <QDialog>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject.h>
#include <iostream>
#include <QUrl>

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
        setEnv();
//        getCategories();

    }
    QJsonObject models;
    void setMembers(World *_world);
    QStringList getCategories();
    QStringList getModels(const QString &category);
    void loadModel(const QString &modelName);

};


#endif

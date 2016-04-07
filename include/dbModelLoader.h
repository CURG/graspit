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
    QJsonObject models;
    QString apiUrl;
    QString apiKey;
    int apiPort;
protected:
    void setEnv();
    QStringList getCategories();
    QStringList getModels(const QString &category);
    void loadModel(const QString &modelName);
public:

    DbModelLoader() : world(NULL){
        setEnv();
        getCategories();

    }

    void setMembers(World *_world);

};


#endif

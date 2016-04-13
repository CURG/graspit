#ifndef _dbModelLoaderdlg_h_
#define _dbModelLoaderdlg_h_

#include "ui_dbModelLoaderDlg.h"
#include <QDialog>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject.h>
#include <iostream>
#include <QUrl>
#include <dbModelLoader.h>

class World;
class DbModelLoaderDlg : public QDialog, public Ui::DbModelLoaderDlgUI
{
    Q_OBJECT
private:
	void init();
    World *world;
    QString apiUrl;
    QString apiKey;
    DbModelLoader loader;
    int apiPort;
protected:
    void setCategories();
    void setModels(const QString &category);
public:

    DbModelLoaderDlg(QWidget *parent) : QDialog(parent), world(NULL){
        setupUi(this);
        init();
        setCategories();

	}

    void setMembers(World *_world);
public Q_SLOTS:
    void loadButton_clicked();
    void categoryComboBox_clicked(const QString &category);

};


#endif

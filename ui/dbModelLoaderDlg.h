#ifndef _dbModelLoaderdlg_h_
#define _dbModelLoaderdlg_h_

#include "ui_dbModelLoaderDlg.h"
#include <QDialog>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class World;
class DbModelLoaderDlg : public QDialog, public Ui::DbModelLoaderDlgUI
{
    Q_OBJECT
private:
	void init();
    World *world;
protected:
    void setCategories();
public:

    DbModelLoaderDlg(QWidget *parent) : QDialog(parent), world(NULL){
		setupUi(this);
        init();
        setCategories();
//        QString url = QString("http://borneo.cs.columbia.edu:8080/models/metas/category?access_token=robolabapi");
//        QUrl httpRequest(url);
//        QNetworkRequest request;
//        QNetworkReply *reply = request.
//                sendRequest(httpRequest);
//        connect(reply, SIGNAL(finished()), this, SLOT(setCategories(QNetworkReply));


//        setCategories();
//        this->categoriesComboBox->addItems(
//        this->OKButton->
	}

    void setMembers(World *_world);
public Q_SLOTS:
    void loadButton_clicked();

};


#endif

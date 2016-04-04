#ifndef _dbModelLoaderdlg_h_
#define _dbModelLoaderdlg_h_

#include "ui_dbModelLoaderDlg.h"
#include <QDialog>
class World;
class DbModelLoaderDlg : public QDialog, public Ui::DbModelLoaderDlgUI
{
private:
	void init();
    World *world;
public:

    DbModelLoaderDlg(QWidget *parent) : QDialog(parent), world(NULL){
		setupUi(this);
		init();
//        this->OKButton->
	}

    void setMembers(World *_world);
public Q_SLOTS:
    void loadButton_clicked();

};

#endif

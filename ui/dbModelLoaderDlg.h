#ifndef _dbModelLoaderdlg_h_
#define _dbModelLoaderdlg_h_

#include "ui_dbModelLoaderDlg.h"
#include <QDialog>

class DbModelLoaderDlg : public QDialog, public Ui::DbModelLoaderDlgUI
{
private:
	void init();
public:
    DbModelLoaderDlg(QWidget *parent) : QDialog(parent) {
		setupUi(this);
		init();
        this->OKButton->
	}
};

#endif

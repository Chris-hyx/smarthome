#include "idnex.h"
#include "ui_idnex.h"

idnex::idnex(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::idnex)
{
    ui->setupUi(this);
}

idnex::~idnex()
{
    delete ui;
}

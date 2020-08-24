#ifndef IDNEX_H
#define IDNEX_H

#include <QDialog>

namespace Ui {
class idnex;
}

class idnex : public QDialog
{
    Q_OBJECT

public:
    explicit idnex(QWidget *parent = 0);
    ~idnex();

private:
    Ui::idnex *ui;
};

#endif // IDNEX_H

#include "verticedialog.h"
#include "ui_verticedialog.h"
#include <QColorDialog>

verticeDialog::verticeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::verticeDialog)
{
    ui->setupUi(this);
}

verticeDialog::~verticeDialog()
{
    delete ui;
}

void verticeDialog::set(int index, const float *value)
{
    this->index = index;
    for (int i = 0; i < 3; ++i)
        this->position[i] = value[i];
    this->color.setRgb(int(value[3] * 255), int(value[4] * 255), int(value[5] * 255));
    ui->xLineEdit->setText(QString::number(double(this->position[0])));
    ui->yLineEdit->setText(QString::number(double(this->position[1])));
    ui->zLineEdit->setText(QString::number(double(this->position[2])));
}

void verticeDialog::on_Color_clicked()
{
    color = QColorDialog::getColor(color, this, tr("point color"));
}

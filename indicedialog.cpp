#include "indicedialog.h"
#include "ui_indicedialog.h"
#include <QMenuBar>
#include <QToolBar>

indiceDialog::indiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::indiceDialog)
{
    ui->setupUi(this);
    this->setMinimumSize(QSize(200, 130));
    ui->OK->setStyleSheet(QString("background: #00FFFF;"));
    ui->Delete->setStyleSheet(QString("background: red;"));
}

indiceDialog::~indiceDialog()
{
    delete ui;
}

void indiceDialog::set(int index, const unsigned int *value)
{
    this->index = index;
    if (value == nullptr) {
        this->setWindowTitle(QString("New Indice"));
        ui->aSpinBox->setValue(0);
        ui->bSpinBox->setValue(0);
        ui->cSpinBox->setValue(0);
        ui->Delete->setVisible(false);
    } else {
        this->setWindowTitle(QString("CurIndiceID: ") + QString::number(index));
        for (int i = 0; i < 3; ++i)
            this->position[i] = value[i];
        ui->aSpinBox->setValue(int(this->position[0]));
        ui->bSpinBox->setValue(int(this->position[1]));
        ui->cSpinBox->setValue(int(this->position[2]));
        ui->Delete->setVisible(true);
    }
}

void indiceDialog::on_OK_clicked()
{
    position[0] = static_cast<unsigned int>(ui->aSpinBox->value());
    position[1] = static_cast<unsigned int>(ui->bSpinBox->value());
    position[2] = static_cast<unsigned int>(ui->cSpinBox->value());
    if (index != -1)
        emit ok(index, position[0], position[1], position[2]);
    else
        emit add(position[0], position[1], position[2]);
    this->close();
}

void indiceDialog::on_Delete_clicked()
{
    emit delete_cmd(this->index);
    this->close();
}

void indiceDialog::on_Cancel_clicked()
{
    this->close();
}

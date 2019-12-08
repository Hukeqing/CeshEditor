#include "verticedialog.h"
#include "ui_verticedialog.h"
#include <QColorDialog>
#include <QDebug>
#include <QDoubleSpinBox>

verticeDialog::verticeDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::verticeDialog)
{
    ui->setupUi(this);
    this->setMinimumSize(QSize(250, 150));
    ui->OK->setStyleSheet(QString("background: #00FFFF;"));
    ui->Delete->setStyleSheet(QString("background: red;"));
}

verticeDialog::~verticeDialog()
{
    delete ui;
}

void verticeDialog::set(int index, const float *value)
{
    this->index = index;
    if (value == nullptr) {
        this->setWindowTitle(QString("New Vertice"));
        ui->xDoubleSpinBox->setValue(0);
        ui->yDoubleSpinBox->setValue(0);
        ui->zDoubleSpinBox->setValue(0);
        color = Qt::white;
        ui->Delete->setVisible(false);
        style_set();
    } else {
        this->setWindowTitle(QString("CurVerticeID: ") + QString::number(index));
        for (int i = 0; i < 3; ++i)
            this->position[i] = value[i];
        this->color.setRgb(int(value[3] * 255), int(value[4] * 255), int(value[5] * 255));
//        ui->xLineEdit->setText(QString::number(double(this->position[0])));
        ui->xDoubleSpinBox->setValue(double(this->position[0]));
        ui->yDoubleSpinBox->setValue(double(this->position[1]));
        ui->zDoubleSpinBox->setValue(double(this->position[2]));
        ui->Delete->setVisible(true);
        style_set();
    }
}

void verticeDialog::on_Color_clicked()
{
    QColor tmp;
    tmp = QColorDialog::getColor(color, this, tr("point color"));
    if (tmp.isValid()) {
        color = tmp;
        style_set();
    }
}

void verticeDialog::on_Cancel_clicked()
{
    this->close();
}

void verticeDialog::on_OK_clicked()
{
    position[0] = float(ui->xDoubleSpinBox->value());
    position[1] = float(ui->yDoubleSpinBox->value());
    position[2] = float(ui->zDoubleSpinBox->value());
    if (index != -1)
        emit ok(index, position[0], position[1], position[2], color.red(), color.green(), color.blue());
    else
        emit add(position[0], position[1], position[2], color.red(), color.green(), color.blue());
    this->close();
}

void verticeDialog::on_Delete_clicked()
{
    emit delete_cmd(this->index);
    this->close();
}

void verticeDialog::style_set()
{
    QString res = "background: rgb(" + QString::number(color.red()) + ", " +
                  QString::number(color.green()) + ", " +
                  QString::number(color.blue()) + ");";
    if (color.red() * 0.299f +
        color.green() * 0.578f +
        color.blue() * 0.114f >= 192) {
        res += "color: black;";
    } else {
        res += "color: white;";
    }
    ui->Color->setStyleSheet(res);
}

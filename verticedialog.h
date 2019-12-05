#ifndef VERTICEDIALOG_H
#define VERTICEDIALOG_H

#include <QDialog>

namespace Ui {
class verticeDialog;
}

class verticeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit verticeDialog(QWidget *parent = nullptr);
    ~verticeDialog();
    void set(int index, const float *value);
signals:
    void ok(int index, float x, float y, float z, float r, float g, float b);
private slots:
    void on_Color_clicked();
private:
    Ui::verticeDialog *ui;
    int index;
    float position[3];
    QColor color;
};

#endif // VERTICEDIALOG_H

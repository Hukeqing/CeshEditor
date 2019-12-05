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
    void delete_cmd(int index);
private slots:
    void on_Color_clicked();
    void on_Cancel_clicked();
    void on_OK_clicked();
    void editor_position();
    void on_Delete_clicked();
private:
    Ui::verticeDialog *ui;
    int index;
    float position[3];
    QColor color;
    void style_set();
};

#endif // VERTICEDIALOG_H

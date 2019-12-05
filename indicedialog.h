#ifndef INDICEDIALOG_H
#define INDICEDIALOG_H

#include <QDialog>

namespace Ui {
class indiceDialog;
}

class indiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit indiceDialog(QWidget *parent = nullptr);
    ~indiceDialog();
    void set(int index, const unsigned int *value);
signals:
    void ok(int index, unsigned int a, unsigned int b, unsigned int c);
    void add(unsigned int a, unsigned int b, unsigned int c);
    void delete_cmd(int index);
private slots:
    void on_Cancel_clicked();
    void on_OK_clicked();
    void on_Delete_clicked();
private:
    Ui::indiceDialog *ui;
    int index;
    unsigned int position[3];
};

#endif // INDICEDIALOG_H

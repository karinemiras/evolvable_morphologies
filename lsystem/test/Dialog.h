#ifndef LSYSTEM_PROTO_DIALOG_H
#define LSYSTEM_PROTO_DIALOG_H

#include <QtGui/QtGui>
#include <QtWidgets/QDialog>
#include <QtCore/QtCore>
#include <QtWidgets/QWidget>

namespace Ui{

    class Dialog;
}

class Dialog : public QDialog
{
Q_OBJECT
public:
    explicit Dialog(QWidget *parent =0);
    ~Dialog();

private:

    Ui::Dialog *ui;

protected:
    void paintEvent(QPaintEvent *event);


};

#endif //LSYSTEM_PROTO_DIALOG_H
#include "Dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent):

    QDialog(parent),
    ui(new Ui::Dialog){
        ui->setupUi(this);
    }


Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);



    QPen myPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(myPen);



    QPolygon polygon;
    polygon << QPoint(130, 140) << QPoint(180, 170)
             << QPoint(180, 140) << QPoint(220, 110)
             << QPoint(140, 100);
     painter.drawPolygon(polygon);

 
}
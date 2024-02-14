#ifndef CAMERAVALUEDIALOG_H
#define CAMERAVALUEDIALOG_H
#include <QDialog>

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QDebug>
#include "define_unifieddata.h"


namespace post3dapp{

class CameraValues
{
    CameraValues(){}
};

class CameraVectorWidget : public QWidget
{
    Q_OBJECT
public:
    CameraVectorWidget( QWidget* parent ): QWidget( parent )
    {
        QLabel* label1=new QLabel(QString::fromLocal8Bit("vx:"),this);
        xLineEdit=new QLineEdit(this);
        xLineEdit->setValidator(new QDoubleValidator(-9999.999,9999.999,3,this));
        xLineEdit->setAlignment(Qt::AlignRight);
        xLineEdit->setFixedWidth(75);
        QLabel *label2=new QLabel(QString::fromLocal8Bit("vy:"),this);
        yLineEdit=new QLineEdit(this);
        yLineEdit->setValidator(new QDoubleValidator(-9999.999,9999.999,3,this));
        yLineEdit->setAlignment(Qt::AlignRight);
        yLineEdit->setFixedWidth(75);
        QLabel *label3=new QLabel(QString::fromLocal8Bit("vz:"),this);
        zLineEdit=new QLineEdit(this);
        zLineEdit->setValidator(new QDoubleValidator(-9999.999,9999.999,3,this));
        zLineEdit->setAlignment(Qt::AlignRight);
        zLineEdit->setFixedWidth(75);

        QHBoxLayout* hlay=new QHBoxLayout;
        hlay->setContentsMargins(0,0,0,0);
        hlay->setSpacing(5);
        hlay->addWidget(label1);
        hlay->addWidget(xLineEdit);
        hlay->addSpacing(10);
        hlay->addWidget(label2);
        hlay->addWidget(yLineEdit);
        hlay->addSpacing(10);
        hlay->addWidget(label3);
        hlay->addWidget(zLineEdit);
        hlay->addStretch();
        this->setLayout(hlay);
    }

    void setCameraVector(XYZ xyz){
        xLineEdit->setText(QString("%1").arg(xyz.x,0,'f',3));
        yLineEdit->setText(QString("%1").arg(xyz.y,0,'f',3));
        zLineEdit->setText(QString("%1").arg(xyz.z,0,'f',3));
    }
    QLineEdit* xLineEdit;
    QLineEdit* yLineEdit;
    QLineEdit* zLineEdit;
};

class CameraValueDialog : public QDialog
{
    Q_OBJECT

public:

    CameraValueDialog(QWidget* parent=0 ) : QDialog(parent)
    {
        set_button=new QPushButton(QString::fromLocal8Bit("SET"),this);
        get_button=new QPushButton(QString::fromLocal8Bit("GET"),this);

//        connect( set_button, SIGNAL( clicked() ), this, SLOT( setCameraValues() ) );
//        connect( get_button, SIGNAL( clicked() ), this, SLOT( getCameraValues() ) );

        QLabel *camera1Label=new QLabel(QString::fromLocal8Bit("視点位置:"),this);
        camera1Label->setFixedWidth(100);
        camera1Layout=new CameraVectorWidget(this);
        QHBoxLayout* hlay_camera1=new QHBoxLayout;
        hlay_camera1->addWidget(camera1Label);
        hlay_camera1->addWidget(camera1Layout);
        hlay_camera1->addStretch();

        QLabel *camera2Label=new QLabel(QString::fromLocal8Bit("上下ベクトル:"),this);
        camera2Label->setFixedWidth(100);
        camera2Layout=new CameraVectorWidget(this);
        QHBoxLayout* hlay_camera2=new QHBoxLayout;
        hlay_camera2->addWidget(camera2Label);
        hlay_camera2->addWidget(camera2Layout);
        hlay_camera2->addStretch();

        QLabel *camera3Label=new QLabel(QString::fromLocal8Bit("方向ベクトル:"),this);
        camera3Label->setFixedWidth(100);
        camera3Layout=new CameraVectorWidget(this);
        QHBoxLayout* hlay_camera3=new QHBoxLayout;
        hlay_camera3->addWidget(camera3Label);
        hlay_camera3->addWidget(camera3Layout);
        hlay_camera3->addStretch();

        QHBoxLayout *hlay_button=new QHBoxLayout;
        hlay_button->addWidget(set_button);
        hlay_button->addWidget(get_button);

        QVBoxLayout* hlay_camera=new QVBoxLayout;
        hlay_camera->setSpacing(5);
        hlay_camera->addLayout(hlay_camera1);
        hlay_camera->addLayout(hlay_camera2);
        hlay_camera->addLayout(hlay_camera3);
        hlay_camera->addLayout(hlay_button);

        this->setLayout(hlay_camera);
        this->setWindowFlags(Qt::Dialog|Qt::WindowTitleHint);
        this->setFont(QFont(QString::fromLocal8Bit("Microsoft JhengHei"),9));

    }
    void setCameraViewPointVector(XYZ xyz){
        camera1Layout->setCameraVector(xyz);
    }
    void setCameraUpwardVector(XYZ xyz){
        camera2Layout->setCameraVector(xyz);
    }
    void setCameraFocusVector(XYZ xyz){
        camera3Layout->setCameraVector(xyz);
    }

    XYZ getCameraViewPointVector(){
        return XYZ(
            camera1Layout->xLineEdit->text().toDouble(),
            camera1Layout->yLineEdit->text().toDouble(),
            camera1Layout->zLineEdit->text().toDouble()
        );
    }
    XYZ getCameraUpwardVector(){
        return XYZ(
            camera2Layout->xLineEdit->text().toDouble(),
            camera2Layout->yLineEdit->text().toDouble(),
            camera2Layout->zLineEdit->text().toDouble()
        );
    }
    XYZ getCameraFocusVector(){
        return XYZ(
            camera3Layout->xLineEdit->text().toDouble(),
            camera3Layout->yLineEdit->text().toDouble(),
            camera3Layout->zLineEdit->text().toDouble()
        );
    }
    QPushButton* set_button;
    QPushButton* get_button;


private slots:


private:

    CameraVectorWidget* camera1Layout;
    CameraVectorWidget* camera2Layout;
    CameraVectorWidget* camera3Layout;
};

}

#endif // CAMERAVALUEDIALOG_H

#include <QtGui>

#include "changeangle_dialog.h"
namespace post3dapp{
ChangeAngleDialog::ChangeAngleDialog(const QString &str_ag, QWidget *parent)
    : QDialog(parent)
{
    createLayout();
    angleLineEdit->setText(str_ag);

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"コードアングルの変更");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void ChangeAngleDialog::createLayout()
{
    QLabel *changedLabel = new QLabel(u8"コードアングル（度） ：", this);
    angleLineEdit = new QLineEdit(this);
    angleLineEdit->setValidator(new QDoubleValidator(-180.0, 180.0, 3, this));
    angleLineEdit->setAlignment(Qt::AlignRight);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(changedLabel);
    hlay1->addWidget(angleLineEdit);

    QPushButton *okButton = new QPushButton(u8"変　更", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->addStretch();
    hlay2->addWidget(okButton);
    hlay2->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(hlay1);
    vlay->addLayout(hlay2);
    vlay->addStretch();

    this->setLayout(vlay);
}

QString ChangeAngleDialog::valueText() const
{
    return angleLineEdit->text();
}

/*
void PersChangeAngleDialog::slotChangeAngle()
{
    bool ok=true;
    qreal ag=angleLineEdit->text().toDouble(&ok);
    if( angleLineEdit->text()=="***" ){ this->accept(); return; }

    if( !ok ){
        warningInvalidValue();
        return;
    }

    qreal eps=1.0e-5;
    bool inRange=( ag > -180.0-eps && ag < 180.0+eps );
    while( !inRange ){
        if( ag < -180.0 ) ag+=360.0;
        if( ag > 180.0 ) ag-=360.0;
        inRange=( ag > -180.0-eps && ag < 180.0+eps );
    }

    parentViewer->changeCodeAngle( ag );
    this->accept();
}

void PersChangeAngleDialog::warningInvalidValue()
{
    QMessageBox::warning( this, tr("WARNING"),
                          tr("入力された値が不正です"),
                          QMessageBox::NoButton,QMessageBox::Warning );
}

*/
} // namespace post3dapp

#include <QtGui>

#include "changeshift_dialog.h"
namespace post3dapp{
ChangeShiftDialog::ChangeShiftDialog(const QString &str1, const QString &str2,
                                     QWidget *parent)
    : QDialog(parent)
{
    createLayout();

    shiftYLineEdit->setText(str1);
    shiftZLineEdit->setText(str2);

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"部材寄り長さの変更");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void ChangeShiftDialog::createLayout()
{
    shiftYLineEdit = new QLineEdit(this);
    shiftYLineEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    shiftYLineEdit->setAlignment(Qt::AlignRight);
    shiftZLineEdit = new QLineEdit(this);
    shiftZLineEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    shiftZLineEdit->setAlignment(Qt::AlignRight);

    QGridLayout *glay1 = new QGridLayout;
    glay1->setSpacing(5);
    glay1->addWidget(new QLabel(u8"寄り長さy方向（mm） ：", this), 0, 0, 1, 1);
    glay1->addWidget(shiftYLineEdit, 0, 1, 1, 1);
    glay1->addWidget(new QLabel(u8"寄り長さz方向（mm） ：", this), 1, 0, 1, 1);
    glay1->addWidget(shiftZLineEdit, 1, 1, 1, 1);

    QPushButton *okButton = new QPushButton(u8"変　更", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addStretch();
    hlay1->addWidget(okButton);
    hlay1->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(glay1);
    vlay->addLayout(hlay1);
    vlay->addStretch();

    this->setLayout(vlay);
}

QString ChangeShiftDialog::shiftYText() const
{
    return shiftYLineEdit->text();
}

QString ChangeShiftDialog::shiftZText() const
{
    return shiftZLineEdit->text();
}
} // namespace post3dapp

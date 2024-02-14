#include "dividemember_dialog.h"

#include <QtGui>

namespace post3dapp{
DivideMemberDialog::DivideMemberDialog( QWidget *parent)
    : QDialog(parent)
{
    createLayout();
    initializeValues();

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"部材の分割");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void DivideMemberDialog::createLayout()
{
    QRadioButton *inter_button = new QRadioButton(u8"（新規の）内分点で分割する", this);
    QRadioButton *comps_button = new QRadioButton(u8"（新規の）構成点で分割する", this);
    QRadioButton *change_button = new QRadioButton(u8"既存の内分点を構成点にする", this);

    typeGroup = new QButtonGroup;
    typeGroup->addButton(inter_button, 0);
    typeGroup->addButton(comps_button, 1);
    typeGroup->addButton(change_button, 2);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->addWidget(inter_button);
    vlay1->addWidget(comps_button);
    vlay1->addWidget(change_button);
    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->setContentsMargins(5, 5, 5, 5);
    hlay1->addLayout(vlay1);
    hlay1->addStretch();

    QGroupBox *typeBox = new QGroupBox(u8"分割の種類", this);
    typeBox->setLayout(hlay1);

    QRadioButton *equal_button = new QRadioButton(u8"等分割   ", this);
    QRadioButton *ratio_button = new QRadioButton(u8"比率", this);
    QRadioButton *ilen_button = new QRadioButton(u8"i端からの距離", this);
    QRadioButton *jlen_button = new QRadioButton(u8"j端からの距離", this);

    divideGroup = new QButtonGroup;
    divideGroup->addButton(equal_button, 0);
    divideGroup->addButton(ratio_button, 1);
    divideGroup->addButton(ilen_button, 2);
    divideGroup->addButton(jlen_button, 3);

    equal_spin = new QSpinBox(this);
    equal_spin->setMinimum(2);
    equal_spin->setMaximum(99);
    QHBoxLayout *equal_lay = new QHBoxLayout;
    equal_lay->addWidget(new QLabel(u8"分割数:", this));
    equal_lay->addWidget(equal_spin);

    ratio1_spin = new QSpinBox(this);
    ratio1_spin->setMinimum(1);
    ratio1_spin->setMaximum(9999);
    ratio2_spin = new QSpinBox(this);
    ratio2_spin->setMinimum(1);
    ratio2_spin->setMaximum(9999);
    QHBoxLayout *ratio_lay = new QHBoxLayout;
    ratio_lay->addWidget(ratio1_spin);
    ratio_lay->addWidget(new QLabel("：", this));
    ratio_lay->addWidget(ratio2_spin);

    ilen_spin = new QSpinBox(this);
    ilen_spin->setMinimum(1);
    ilen_spin->setMaximum(99999);
    QHBoxLayout *ilen_lay = new QHBoxLayout;
    ilen_lay->addWidget(ilen_spin);
    ilen_lay->addWidget(new QLabel("(mm)", this));

    jlen_spin = new QSpinBox(this);
    jlen_spin->setMinimum(1);
    jlen_spin->setMaximum(99999);
    QHBoxLayout *jlen_lay = new QHBoxLayout;
    jlen_lay->addWidget(jlen_spin);
    jlen_lay->addWidget(new QLabel("(mm)", this));

    QGridLayout *glay = new QGridLayout;
    glay->setContentsMargins(5, 5, 5, 5);
    glay->addWidget(equal_button, 0, 0, 1, 1);
    glay->addWidget(ratio_button, 1, 0, 1, 1);
    glay->addWidget(ilen_button, 2, 0, 1, 1);
    glay->addWidget(jlen_button, 3, 0, 1, 1);
    glay->addLayout(equal_lay, 0, 1, 1, 1);
    glay->addLayout(ratio_lay, 1, 1, 1, 1);
    glay->addLayout(ilen_lay, 2, 1, 1, 1);
    glay->addLayout(jlen_lay, 3, 1, 1, 1);

    divideBox = new QGroupBox(u8"分割方法", this);
    divideBox->setLayout(glay);

    connect(typeGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &DivideMemberDialog::slotChangeDivideBoxStatus);

    QPushButton *okButton = new QPushButton(u8"変　更", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &QDialog::accept );
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    QHBoxLayout *hlay6 = new QHBoxLayout;
    hlay6->addStretch();
    hlay6->addWidget(okButton);
    hlay6->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(typeBox);
    vlay->addWidget(divideBox);
    vlay->addLayout(hlay6);
    vlay->addStretch();

    this->setLayout(vlay);
}

void DivideMemberDialog::initializeValues()
{
    typeGroup->button(0)->setChecked(true);
    divideGroup->button(0)->setChecked(true);
    divideBox->setEnabled(true);
    equal_spin->setValue(2);
    ratio1_spin->setValue(1);
    ratio2_spin->setValue(1);
    ilen_spin->setValue(1500);
    jlen_spin->setValue(1500);
}

void DivideMemberDialog::slotChangeDivideBoxStatus()
{
    divideBox->setEnabled( typeGroup->checkedId() != 2 );
}

void DivideMemberDialog::warningInvalidValue()
{
    QMessageBox::warning( this,
                          "WARNING",
                          u8"入力された値が不正です。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}
} // namespace post3dapp

#include "section_col_filterdialog.h"
#include"define_sectiontable.h"

namespace post3dapp{
SectionColumnFilterDialog::SectionColumnFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    :SectionFilterDialog(tabelWidgets, parent)
{
    setWindowTitle(u8"表示設定　柱");
    createLayout();
}

QVBoxLayout* SectionColumnFilterDialog::createSwitchLayout()
{
    QVBoxLayout *switchlayout = SectionFilterDialog::createSwitchLayout();
    QPushButton *abar_on = new QPushButton(u8"全表示", this);
    connect(abar_on, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_axialBarOn);
    QPushButton *abar_off = new QPushButton(u8"全非表示", this);
    connect(abar_off, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_axialBarOff);
    QHBoxLayout *abarswitch = new QHBoxLayout;
    abarswitch->setSpacing(3);
    abarswitch->addWidget(new QLabel(u8"芯鉄筋："));
    abarswitch->addStretch(1);
    abarswitch->addWidget(abar_on);
    abarswitch->addWidget(abar_off);
    switchlayout->addLayout(abarswitch);

    QPushButton *rkry_on = new QPushButton(u8"全表示", this);
    connect(rkry_on, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_rkryOn);
    QPushButton *rkry_off = new QPushButton(u8"全非表示", this);
    connect(rkry_off, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_rkryOff);
    QHBoxLayout *rkryswitch = new QHBoxLayout;
    rkryswitch->setSpacing(3);
    rkryswitch->addWidget(new QLabel(u8"主筋間隔・主筋距離："));
    rkryswitch->addStretch(1);
    rkryswitch->addWidget(rkry_on);
    rkryswitch->addWidget(rkry_off);
    switchlayout->addLayout(rkryswitch);

    QLabel *spos_label = new QLabel(u8"鉄骨位置：", this);
    QPushButton *spos_on = new QPushButton(u8"全表示", this);
    connect(spos_on, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_steelPositionOn);
    QPushButton *spos_off = new QPushButton(u8"全非表示", this);
    connect(spos_off, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_steelPositionOff);
    QHBoxLayout *sposswitch = new QHBoxLayout;
    sposswitch->setSpacing(3);
    sposswitch->addWidget(spos_label);
    sposswitch->addStretch(1);
    sposswitch->addWidget(spos_on);
    sposswitch->addWidget(spos_off);
    switchlayout->addLayout(sposswitch);

    return switchlayout;
}
} // namespace post3dapp

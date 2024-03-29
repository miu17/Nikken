﻿#include "operation_slab_page.h"

#include <QtGui>

#include "custom_checkbutton.h"
#include "reference_combobox.h"

namespace post3dapp{
OperationSlabPage::OperationSlabPage( QWidget *parent )
    : AbstractElementPage( parent )
{
    myElementType = ELSLAB;

    ReferenceCombobox *fugo_combo = new ReferenceCombobox(DATATYPE::TPSLAB, UnifiedDataType::Input, this);
    fugo_combo->setMinimumWidth(125);
    connect(fugo_combo, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &AbstractElementPage::signalCurrentInputName);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(new QLabel(u8"・荷重符号：", this));
    hlay1->addWidget(fugo_combo);

    /* 追加　レイアウト */
    CustomCheckButton *pointhit_button = new CustomCheckButton(u8"周囲点指定", this);
    CustomCheckButton *onehit_button = new CustomCheckButton(u8"１点ヒット", this);
    CustomCheckButton *batch_button = new CustomCheckButton(u8"一括入力", this);
    pointhit_button->setFixedHeight(25);
    onehit_button->setFixedHeight(25);
    batch_button->setFixedHeight(25);
    pointhit_button->setChecked(true);

    myButtonGroup->addButton(pointhit_button, POINTHITMODE);
    myButtonGroup->addButton(onehit_button, ONEHITMODE);
    myButtonGroup->addButton(batch_button, BATCHMODE);

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->setSpacing(3);
    hlay2->addWidget(pointhit_button);
    hlay2->addWidget(onehit_button);
    hlay2->addWidget(batch_button);

    createStackedLayout();

    connect(myButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &OperationSlabPage::slotChangeStackLayout);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addLayout(hlay1);
    vlay1->addSpacing(5);
    vlay1->addLayout(hlay2);
    vlay1->addSpacing(5);
    vlay1->addWidget(slabStackedWidget);

    myAddWidget->setLayout(vlay1);

    /* 編集　レイアウト */
    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->addWidget(new SnapJointWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddMoveWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddedMovedWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new DeleteWidget(this));
    vlay2->addStretch();

    myEditWidget->setLayout(vlay2);
}

void OperationSlabPage::createStackedLayout()
{
    /* 周囲点指定 */
    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addWidget(new SnapJointWidget(this));
    vlay1->addSpacing(10);
    vlay1->addWidget(new AddMoveWidget(this));
    vlay1->addSpacing(10);
    vlay1->addWidget(new AddedMovedWidget(this));
    vlay1->addStretch();

    QWidget *sub1_widget = new QWidget(this);
    sub1_widget->setLayout(vlay1);

    /* １点ヒット */
    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->addWidget(new TargetAreaWidget(this));
    vlay2->addStretch();

    QWidget *sub2_widget = new QWidget(this);
    sub2_widget->setLayout(vlay2);

    /* 一括入力 */
    QVBoxLayout *vlay3 = new QVBoxLayout;
    vlay3->setContentsMargins(0, 0, 0, 0);
    vlay3->addWidget(new TargetAreaWidget(this));
    vlay3->addStretch();
    QWidget *sub3_widget = new QWidget(this);
    sub3_widget->setLayout(vlay3);

    slabStackedWidget = new QStackedWidget(this);
    slabStackedWidget->addWidget(sub1_widget);
    slabStackedWidget->addWidget(sub2_widget);
    slabStackedWidget->addWidget(sub3_widget);
}

void OperationSlabPage::slotChangeStackLayout(int id)
{
    signalCurrentMode(id);

    switch (id) {
    case POINTHITMODE:
        slabStackedWidget->setCurrentIndex(0);
        break;
    case ONEHITMODE:
        slabStackedWidget->setCurrentIndex(1);
        break;
    case BATCHMODE:
        slabStackedWidget->setCurrentIndex(2);
        break;
    default:
        slabStackedWidget->setCurrentIndex(0);
        break;
    }
}

} // namespace post3dapp

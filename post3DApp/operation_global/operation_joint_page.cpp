#include "operation_joint_page.h"

#include <QtGui>

#include "custom_checkbutton.h"

namespace post3dapp{
OperationJointPage::OperationJointPage( QWidget *parent )
    : AbstractElementPage( parent )
{
    myElementType = ELJOINT;

    /* 追加　レイアウト */
    CustomCheckButton *onehit_button = new CustomCheckButton(u8"１点ヒット", this);
    CustomCheckButton *batch_button = new CustomCheckButton(u8"一括入力", this);
    onehit_button->setFixedHeight(25);
    batch_button->setFixedHeight(25);
    onehit_button->setChecked(true);

    myButtonGroup->addButton(onehit_button, POINTHITMODE);
    myButtonGroup->addButton(batch_button, BATCHMODE);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->setSpacing(3);
    hlay1->addWidget(onehit_button);
    hlay1->addWidget(batch_button);

    createStackedLayout();

    connect(myButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &OperationJointPage::slotChangeStackLayout);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addLayout(hlay1);
    vlay1->addSpacing(5);
    vlay1->addWidget(jointStackedWidget);

    myAddWidget->setLayout(vlay1);

    /* 編集　レイアウト */
    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->addWidget(new SnapJointWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddMoveWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new DeleteWidget(this));
    vlay2->addStretch();

    myEditWidget->setLayout(vlay2);
}


void OperationJointPage::createStackedLayout()
{
    /* 点ヒット */
    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addWidget(new SnapJointWidget(this));
    vlay1->addSpacing(10);
    vlay1->addWidget(new AddMoveWidget(this));
    vlay1->addStretch();

    QWidget *sub1_widget = new QWidget(this);
    sub1_widget->setLayout(vlay1);

    /* 一括入力 */
    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->addWidget(new JointBatchWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddMoveWidget(this));
    vlay2->addStretch();

    QWidget *sub2_widget = new QWidget(this);
    sub2_widget->setLayout(vlay2);

    jointStackedWidget = new QStackedWidget(this);
    jointStackedWidget->addWidget(sub1_widget);
    jointStackedWidget->addWidget(sub2_widget);
}

void OperationJointPage::slotChangeStackLayout(int id)
{
    signalCurrentMode(id);

    switch (id) {
    case POINTHITMODE:
        jointStackedWidget->setCurrentIndex(0);
        break;
    case BATCHMODE:
        jointStackedWidget->setCurrentIndex(1);
        break;
    default:
        jointStackedWidget->setCurrentIndex(0);
        break;
    }
}
} // namespace post3dapp


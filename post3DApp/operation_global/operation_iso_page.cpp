#include "operation_iso_page.h"

#include <QtGui>

#include "custom_checkbutton.h"
#include "reference_combobox.h"

namespace post3dapp{
OperationIsoPage::OperationIsoPage( QWidget *parent )
    : AbstractElementPage( parent )
{
    myElementType = ELISO;

    ReferenceCombobox *fugo_combo = new ReferenceCombobox(DATATYPE::TPISO, UnifiedDataType::Input, this);
    fugo_combo->setMinimumWidth(125);
    connect(fugo_combo, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &AbstractElementPage::signalCurrentInputName);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(new QLabel(u8"・部材符号：", this));
    hlay1->addWidget(fugo_combo);

    /* 追加　レイアウト */
    CustomCheckButton *twohit_button = new CustomCheckButton(u8"２点ヒット", this);
    CustomCheckButton *onehit_button = new CustomCheckButton(u8"伏図１点ヒット", this);
    twohit_button->setFixedHeight(25);
    onehit_button->setFixedHeight(25);
    twohit_button->setChecked(true);
    myButtonGroup->addButton(twohit_button, POINTHITMODE);
    myButtonGroup->addButton(onehit_button, ONEHITMODE);

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->setSpacing(3);
    hlay2->addWidget(twohit_button);
    hlay2->addWidget(onehit_button);

    createStackedLayout();

    connect(myButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &OperationIsoPage::slotChangeStackLayout);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addLayout(hlay1);
    vlay1->addSpacing(5);
    vlay1->addLayout(hlay2);
    vlay1->addSpacing(5);
    vlay1->addWidget(isoStackedWidget);

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

void OperationIsoPage::createStackedLayout()
{
    /* ２点ヒット */
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
    vlay2->addWidget(new AddMoveWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddedMovedWidget(this));
    vlay2->addStretch();

    QWidget *sub2_widget = new QWidget(this);
    sub2_widget->setLayout(vlay2);

    isoStackedWidget = new QStackedWidget(this);
    isoStackedWidget->addWidget(sub1_widget);
    isoStackedWidget->addWidget(sub2_widget);
}

void OperationIsoPage::slotChangeStackLayout(int id)
{
    signalCurrentMode(id);

    switch (id) {
    case POINTHITMODE:
        isoStackedWidget->setCurrentIndex(0);
        break;
    case ONEHITMODE:
        isoStackedWidget->setCurrentIndex(1);
        break;
    default:
        isoStackedWidget->setCurrentIndex(0);
        break;
    }
}

} // namespace post3dapp

#include "operation_jointload_page.h"

#include <QtGui>

#include "custom_checkbutton.h"
#include "custom_label.h"
#include "reference_combobox.h"

namespace post3dapp{
OperationJointLoadPage::OperationJointLoadPage( QWidget *parent )
    : AbstractElementPage( parent )
{
    myElementType = ELJOINTLOAD;

    ReferenceCombobox *fugo_combo = new ReferenceCombobox(DATATYPE::TPJOINTLOAD, UnifiedDataType::Input, this);
    fugo_combo->setMinimumWidth(125);
    connect(fugo_combo, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &AbstractElementPage::signalCurrentInputName);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(new QLabel(u8"・荷重符号：", this));
    hlay1->addWidget(fugo_combo);

    /* 追加　レイアウト */
    CustomCheckButton *onehit_button = new CustomCheckButton(u8"１点ヒット", this);
    CustomCheckButton *batch_button = new CustomCheckButton(u8"節点選択", this);
    onehit_button->setFixedHeight(25);
    batch_button->setFixedHeight(25);
    onehit_button->setChecked(true);

    myButtonGroup->addButton(onehit_button, POINTHITMODE);
    myButtonGroup->addButton(batch_button, SELECTHITMODE);

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->setSpacing(3);
    hlay2->addWidget(onehit_button);
    hlay2->addWidget(batch_button);

    createStackedLayout();

    connect(myButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &OperationJointLoadPage::slotChangeStackLayout);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addLayout(hlay1);
    vlay1->addSpacing(5);
    vlay1->addLayout(hlay2);
    vlay1->addSpacing(5);
    vlay1->addWidget(jointLoadStackedWidget);

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

void OperationJointLoadPage::createStackedLayout()
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

    /* 節点選択 */
    QString msg = u8"※　入力済みの節点を選択し,同位置に節点\n";
    msg += u8"   　荷重を入力する.領域を選択して左クリ\n";
    msg += u8"   　ックで入力,右クリックで選択が解除さ\n";
    msg += u8"   　れる.";

    CustomLabel *clabel = new CustomLabel(msg, 3, QMargins(5, 5, 5, 5), this);
    clabel->setMinimumHeight(90);

    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->addWidget(clabel);
    vlay2->addStretch();

    QWidget *sub2_widget = new QWidget(this);
    sub2_widget->setLayout(vlay2);

    jointLoadStackedWidget = new QStackedWidget(this);
    jointLoadStackedWidget->addWidget(sub1_widget);
    jointLoadStackedWidget->addWidget(sub2_widget);
}

void OperationJointLoadPage::slotChangeStackLayout(int id)
{
    signalCurrentMode(id);

    switch (id) {
    case POINTHITMODE:
        jointLoadStackedWidget->setCurrentIndex(0);
        break;
    case SELECTHITMODE:
        jointLoadStackedWidget->setCurrentIndex(1);
        break;
    default:
        jointLoadStackedWidget->setCurrentIndex(0);
        break;
    }
}

} // namespace post3dapp


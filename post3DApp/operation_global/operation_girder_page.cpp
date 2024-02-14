#include "operation_girder_page.h"

#include <QtGui>

#include "custom_checkbutton.h"
#include "custom_label.h"
#include "reference_combobox.h"

namespace post3dapp{
OperationGirderPage::OperationGirderPage( QWidget *parent )
    : AbstractElementPage( parent )
{
    myElementType = ELGIRDER;

    ReferenceCombobox *fugo_combo = new ReferenceCombobox(DATATYPE::TPGIRDER, UnifiedDataType::Input, this);
    fugo_combo->setMinimumWidth(125);
    connect(fugo_combo, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &AbstractElementPage::signalCurrentInputName);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(new QLabel(u8"・部材符号：", this));
    hlay1->addWidget(fugo_combo);

    /* 追加　レイアウト */
    CustomCheckButton *twohit_button = new CustomCheckButton(u8"２点ヒット", this);
    CustomCheckButton *batch_button = new CustomCheckButton(u8"一括入力", this);
    twohit_button->setFixedHeight(25);
    batch_button->setFixedHeight(25);
    twohit_button->setChecked(true);

    myButtonGroup->addButton(twohit_button, POINTHITMODE);
    myButtonGroup->addButton(batch_button, BATCHMODE);

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->setSpacing(3);
    hlay2->addWidget(twohit_button);
    hlay2->addWidget(batch_button);

    createStackedLayout();

    connect(myButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &OperationGirderPage::slotChangeStackLayout);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addLayout(hlay1);
    vlay1->addSpacing(5);
    vlay1->addLayout(hlay2);
    vlay1->addSpacing(5);
    vlay1->addWidget(girderStackedWidget);

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

void OperationGirderPage::createStackedLayout()
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

    /* 一括入力 */
    /*
        QString msg=tr(" ※  通りと通りの交点上の既存節点に対し,\n");
        msg+=tr(" 　 通り軸に沿った位置に大梁を自動入力\n");
        msg+=tr(" 　 する.領域を選択して左クリックで入力,\n");
        msg+=tr(" 　 右クリックで選択が解除される.");
    */
    QString msg = u8"※　通りと通りの交点上の既存節点に対し,\n";
    msg += u8"   　伏図では上階とを結ぶ,軸図では2点間\n";
    msg += u8"   　を結ぶ位置に柱を自動入力する.領域を\n";
    msg += u8"   　選択して左クリックで入力,右クリック";

    CustomLabel *clabel = new CustomLabel(msg, 3, QMargins(5, 5, 5, 5), this);
    clabel->setMinimumHeight(90);

    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->addWidget(clabel);
    vlay2->addWidget(new AddMoveWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddedMovedWidget(this));
    vlay2->addStretch();

    QWidget *sub2_widget = new QWidget(this);
    sub2_widget->setLayout(vlay2);

    girderStackedWidget = new QStackedWidget(this);
    girderStackedWidget->addWidget(sub1_widget);
    girderStackedWidget->addWidget(sub2_widget);
}

void OperationGirderPage::slotChangeStackLayout(int id)
{
    signalCurrentMode(id);

    switch (id) {
    case POINTHITMODE:
        girderStackedWidget->setCurrentIndex(0);
        break;
    case BATCHMODE:
        girderStackedWidget->setCurrentIndex(1);
        break;
    default:
        girderStackedWidget->setCurrentIndex(0);
        break;
    }
}

} // namespace post3dapp


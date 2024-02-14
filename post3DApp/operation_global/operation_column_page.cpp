#include "operation_column_page.h"

#include <QtGui>

#include "custom_label.h"
#include "custom_checkbutton.h"
#include "reference_combobox.h"

namespace post3dapp{
OperationColumnPage::OperationColumnPage( QWidget *parent )
    : AbstractElementPage( parent )
{
    myElementType = ELCOLUMN;

    ReferenceCombobox *fugo_combo = new ReferenceCombobox(DATATYPE::TPCOLUMN, UnifiedDataType::Input, this);
    fugo_combo->setMinimumWidth(125);
    connect(fugo_combo, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &AbstractElementPage::signalCurrentInputName);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(new QLabel(u8"・部材符号：", this));
    hlay1->addWidget(fugo_combo);

    /* 追加　レイアウト */
    CustomCheckButton *twohit_button = new CustomCheckButton(u8"２点ヒット", this);
    CustomCheckButton *onehit_button = new CustomCheckButton(u8"伏図１点ヒット", this);
    CustomCheckButton *batch_button = new CustomCheckButton(u8"一括入力", this);
    twohit_button->setFixedHeight(25);
    onehit_button->setFixedHeight(25);
    batch_button->setFixedHeight(25);
    twohit_button->setChecked(true);
    myButtonGroup->addButton(twohit_button, POINTHITMODE);
    myButtonGroup->addButton(onehit_button, ONEHITMODE);
    myButtonGroup->addButton(batch_button, BATCHMODE);

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->setSpacing(3);
    hlay2->addWidget(twohit_button);
    hlay2->addWidget(onehit_button);
    hlay2->addWidget(batch_button);

    createStackedLayout();

    connect(myButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &OperationColumnPage::slotChangeStackLayout);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addLayout(hlay1);
    vlay1->addSpacing(5);
    vlay1->addLayout(hlay2);
    vlay1->addSpacing(5);
    vlay1->addWidget(columnStackedWidget);

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

void OperationColumnPage::createStackedLayout()
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
    vlay2->addStretch();

    QWidget *sub2_widget = new QWidget(this);
    sub2_widget->setLayout(vlay2);

    /* 一括入力 */

    QString msg = u8"※　通りと通りの交点上の既存節点に対し,\n";
    msg += u8"   　伏図では上階とを結ぶ,軸図では2点間\n";
    msg += u8"   　を結ぶ位置に柱を自動入力する.領域を\n";
    msg += u8"   　選択して左クリックで入力,右クリック\n";
    msg += u8"   　で選択が解除される.";

    CustomLabel *clabel = new CustomLabel(msg, 3, QMargins(5, 5, 5, 5), this);
    clabel->setMinimumHeight(110);

    QVBoxLayout *vlay3 = new QVBoxLayout;
    vlay3->setContentsMargins(0, 0, 0, 0);
    vlay3->addWidget(clabel);
    vlay3->addWidget(new AddMoveWidget(this));
    vlay3->addSpacing(10);
    vlay3->addWidget(new AddedMovedWidget(this));
    vlay3->addStretch();

    QWidget *sub3_widget = new QWidget(this);
    sub3_widget->setLayout(vlay3);

    columnStackedWidget = new QStackedWidget(this);
    columnStackedWidget->addWidget(sub1_widget);
    columnStackedWidget->addWidget(sub2_widget);
    columnStackedWidget->addWidget(sub3_widget);
}

void OperationColumnPage::slotChangeStackLayout(int id)
{
    signalCurrentMode(id);

    switch (id) {
    case POINTHITMODE:
        columnStackedWidget->setCurrentIndex(0);
        break;
    case ONEHITMODE:
        columnStackedWidget->setCurrentIndex(1);
        break;
    case BATCHMODE:
        columnStackedWidget->setCurrentIndex(2);
        break;
    default:
        columnStackedWidget->setCurrentIndex(0);
        break;
    }
}
} // namespace post3dapp

#include "operation_memberload_page.h"

#include <QtGui>

#include "custom_checkbutton.h"
#include "custom_label.h"
#include "reference_combobox.h"

namespace post3dapp{
OperationMemberLoadPage::OperationMemberLoadPage( QWidget *parent )
    : AbstractElementPage( parent )
{
    myElementType = ELMEMBERLOAD;

    QLabel *fugo_label = new QLabel(u8"・荷重符号：", this);
    ReferenceCombobox *fugo_combo = new ReferenceCombobox(DATATYPE::TPMEMBERLOAD, UnifiedDataType::Input, this);
    fugo_combo->setMinimumWidth(125);
    connect(fugo_combo, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &AbstractElementPage::signalCurrentInputName);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(fugo_label);
    hlay1->addWidget(fugo_combo);

    /* 追加　レイアウト */
    CustomCheckButton *twohit_button = new CustomCheckButton(u8"両端指定", this);
    CustomCheckButton *batch_button = new CustomCheckButton(u8"部材選択", this);
    twohit_button->setFixedHeight(25);
    batch_button->setFixedHeight(25);
    twohit_button->setChecked(true);

    myButtonGroup->addButton(twohit_button, POINTHITMODE);
    myButtonGroup->addButton(batch_button, SELECTHITMODE);

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->setSpacing(3);
    hlay2->addWidget(twohit_button);
    hlay2->addWidget(batch_button);

    createStackedLayout();

    connect(myButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &OperationMemberLoadPage::slotChangeStackLayout);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addLayout(hlay1);
    vlay1->addSpacing(5);
    vlay1->addLayout(hlay2);
    vlay1->addSpacing(5);
    vlay1->addWidget(memberLoadStackedWidget);

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

void OperationMemberLoadPage::createStackedLayout()
{
    /* 両端指定 */
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

    /* 部材選択 */
    QString msg = u8"※　入力済みの部材を選択し,同位置に部材\n";
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

    memberLoadStackedWidget = new QStackedWidget(this);
    memberLoadStackedWidget->addWidget(sub1_widget);
    memberLoadStackedWidget->addWidget(sub2_widget);
}

void OperationMemberLoadPage::slotChangeStackLayout(int id)
{
    signalCurrentMode(id);

    switch (id) {
    case POINTHITMODE:
        memberLoadStackedWidget->setCurrentIndex(0);
        break;
    case SELECTHITMODE:
        memberLoadStackedWidget->setCurrentIndex(1);
        break;
    default:
        memberLoadStackedWidget->setCurrentIndex(0);
        break;
    }
}
} // namespace post3dapp

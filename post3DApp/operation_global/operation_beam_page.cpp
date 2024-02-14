#include "operation_beam_page.h"

#include <QtGui>

#include "custom_checkbutton.h"
#include "reference_combobox.h"

namespace post3dapp{
OperationBeamPage::OperationBeamPage( QWidget *parent )
    : AbstractElementPage( parent )
{
    myElementType = ELBEAM;

    ReferenceCombobox *fugo_combo = new ReferenceCombobox(DATATYPE::TPBEAM, UnifiedDataType::Input, this);
    fugo_combo->setMinimumWidth(125);
    connect(fugo_combo, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &AbstractElementPage::signalCurrentInputName);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(new QLabel(u8"・部材符号：", this));
    hlay1->addWidget(fugo_combo);

    /* 追加　レイアウト */
    CustomCheckButton *twohit_button = new CustomCheckButton(u8"２節点", this);
    CustomCheckButton *uni_button = new CustomCheckButton(u8"等分割", this);
    CustomCheckButton *par_button = new CustomCheckButton(u8"平行", this);
    CustomCheckButton *ver_button = new CustomCheckButton(u8"垂直", this);
    CustomCheckButton *ext_button = new CustomCheckButton(u8"延長", this);
    twohit_button->setFixedHeight(25);
    uni_button->setFixedHeight(25);
    par_button->setFixedHeight(25);
    ver_button->setFixedHeight(25);
    ext_button->setFixedHeight(25);
    twohit_button->setChecked(true);

    myButtonGroup->addButton(twohit_button, POINTHITMODE);
    myButtonGroup->addButton(uni_button, DIVIDEMODE);
    myButtonGroup->addButton(par_button, PARALLELMODE);
    myButtonGroup->addButton(ver_button, VERTICALMODE);
    myButtonGroup->addButton(ext_button, EXTENDMODE);

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->setSpacing(3);
    hlay2->addWidget(twohit_button);
    hlay2->addWidget(uni_button);
    hlay2->addWidget(par_button);
    hlay2->addWidget(ver_button);
    hlay2->addWidget(ext_button);

    createStackedLayout();

    connect(myButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &OperationBeamPage::slotChangeStackLayout);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addLayout(hlay1);
    vlay1->addSpacing(5);
    vlay1->addLayout(hlay2);
    vlay1->addSpacing(5);
    vlay1->addWidget(beamStackedWidget);

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

void OperationBeamPage::createStackedLayout()
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

    /* 等分割 */
    OperationSpinBox *div_spin = new OperationSpinBox(DIVIDEMODE, this);
    div_spin->setMinimum(2);
    div_spin->setMaximum(99);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->setSpacing(0);
    hlay1->addWidget(new QLabel(u8"・分割数：", this));
    hlay1->addStretch();
    hlay1->addWidget(div_spin);

    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->addLayout(hlay1);
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddMoveWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddedMovedWidget(this));
    vlay2->addStretch();

    QWidget *sub2_widget = new QWidget(this);
    sub2_widget->setLayout(vlay2);

    /* 平行 */
    OperationSpinBox *pitch_spin = new OperationSpinBox(PARALLELMODE, this);
    pitch_spin->setMinimum(1);
    pitch_spin->setMaximum(99999);

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->setSpacing(0);
    hlay2->addWidget(new QLabel(u8"・ピッチ(mm)：", this));
    hlay2->addStretch();
    hlay2->addWidget(pitch_spin);

    QVBoxLayout *vlay3 = new QVBoxLayout;
    vlay3->setContentsMargins(0, 0, 0, 0);
    vlay3->addLayout(hlay2);
    vlay3->addSpacing(10);
    vlay3->addWidget(new AddMoveWidget(this));
    vlay3->addSpacing(10);
    vlay3->addWidget(new AddedMovedWidget(this));
    vlay3->addStretch();

    QWidget *sub3_widget = new QWidget(this);
    sub3_widget->setLayout(vlay3);

    /* 垂直 */
    OperationSpinBox *len1_spin = new OperationSpinBox(VERTICALMODE, this);
    len1_spin->setMinimum(1);
    len1_spin->setMaximum(99999);

    QHBoxLayout *hlay3 = new QHBoxLayout;
    hlay3->setSpacing(0);
    hlay3->addWidget(new QLabel(u8"・i端からの距離(mm)：", this));
    hlay3->addStretch();
    hlay3->addWidget(len1_spin);

    QVBoxLayout *vlay4 = new QVBoxLayout;
    vlay4->setContentsMargins(0, 0, 0, 0);
    vlay4->addLayout(hlay3);
    vlay4->addSpacing(10);
    vlay4->addWidget(new AddMoveWidget(this));
    vlay4->addSpacing(10);
    vlay4->addWidget(new AddedMovedWidget(this));
    vlay4->addStretch();

    QWidget *sub4_widget = new QWidget(this);
    sub4_widget->setLayout(vlay4);

    /* 延長 */
    OperationSpinBox *len2_spin = new OperationSpinBox(EXTENDMODE, this);
    len2_spin->setMinimum(1);
    len2_spin->setMaximum(99999);

    QHBoxLayout *hlay4 = new QHBoxLayout;
    hlay4->setSpacing(0);
    hlay4->addWidget(new QLabel(u8"・寸法(mm)：", this));
    hlay4->addStretch();
    hlay4->addWidget(len2_spin);

    QVBoxLayout *vlay5 = new QVBoxLayout;
    vlay5->setContentsMargins(0, 0, 0, 0);
    vlay5->addLayout(hlay4);
    vlay5->addSpacing(10);
    vlay5->addWidget(new AddMoveWidget(this));
    vlay5->addSpacing(10);
    vlay5->addWidget(new AddedMovedWidget(this));
    vlay5->addStretch();

    QWidget *sub5_widget = new QWidget(this);
    sub5_widget->setLayout(vlay5);

    beamStackedWidget = new QStackedWidget(this);
    beamStackedWidget->addWidget(sub1_widget);
    beamStackedWidget->addWidget(sub2_widget);
    beamStackedWidget->addWidget(sub3_widget);
    beamStackedWidget->addWidget(sub4_widget);
    beamStackedWidget->addWidget(sub5_widget);

}

void OperationBeamPage::slotChangeStackLayout(int id)
{
    signalCurrentMode(id);

    switch (id) {
    case POINTHITMODE:
        beamStackedWidget->setCurrentIndex(0);
        break;
    case DIVIDEMODE:
        beamStackedWidget->setCurrentIndex(1);
        break;
    case PARALLELMODE:
        beamStackedWidget->setCurrentIndex(2);
        break;
    case VERTICALMODE:
        beamStackedWidget->setCurrentIndex(3);
        break;
    case EXTENDMODE:
        beamStackedWidget->setCurrentIndex(4);
        break;
    default:
        beamStackedWidget->setCurrentIndex(0);
        break;
    }
}
} // namespace post3dapp

#include "operation_widget_parts.h"

#include <QtGui>

#include "unified_editingdata.h"

namespace post3dapp{
/*個別要素の抽象ページ*/
AbstractElementPage::AbstractElementPage( QWidget *parent )
    : QWidget( parent )
{
    addEditWidget = new QStackedWidget(this);
    myAddWidget = new QWidget(this);
    myEditWidget = new QWidget(this);
    myButtonGroup = new QButtonGroup;

    addEditWidget->addWidget(myAddWidget);
    addEditWidget->addWidget(myEditWidget);
    addEditWidget->setCurrentIndex(0);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->addWidget(addEditWidget);
    this->setLayout(vlay);
}

void AbstractElementPage::slotChangeAddEditLayout(int id)
{
    addEditWidget->setCurrentIndex(id);
    (id == 0) ? signalCurrentMode(myButtonGroup->checkedId()) :
    signalCurrentMode(EDITMODE);
}

void AbstractElementPage::signalCurrentMode(int id)
{
    UnifiedEditingData::getInstance()->changeCurrentMode(id);
}

void AbstractElementPage::signalCurrentInputName(const QString &str)
{
    UnifiedEditingData::getInstance()->uploadElementName(myElementType, str);
}

/*候補点の設定*/
SnapJointWidget::SnapJointWidget( QWidget *parent ) : QWidget( parent )
{
    signalOn = false;
    snap_label = new QLabel(u8"・マウス入力時の候補点（スナップ点）", this);
    grid_check = new QCheckBox(u8"グリッド点", this);
    grid_label = new QLabel(u8"ピッチ(mm)：", this);
    gpitch_spin = new QSpinBox(this);
    gpitch_spin->setMinimum(1);
    gpitch_spin->setMaximum(99999);
    cross_check = new QCheckBox(u8"通り芯の交点", this);
    framedef_check = new QCheckBox(u8"通りの定義点", this);
    div_check = new QCheckBox(u8"部材軸上の分割点", this);
    divuni_check = new QCheckBox(u8"等分割", this);
    divuni_label = new QLabel(u8"分割数:", this);
    divuni_spin = new QSpinBox(this);
    divuni_spin->setMinimum(2);
    divuni_spin->setMaximum(99);
    divdis_check = new QCheckBox(u8"端点からの距離(mm)", this);
    divdisI_label = new QLabel(u8"i端:", this);
    divdisI_spin = new QSpinBox(this);
    divdisI_spin->setMinimum(1);
    divdisI_spin->setMaximum(9999);
    divdisJ_label = new QLabel(u8"j端:", this);
    divdisJ_spin = new QSpinBox(this);
    divdisJ_spin->setMinimum(1);
    divdisJ_spin->setMaximum(9999);
    crossoffset_label = new QLabel(u8"オフセット(mm):", this);
    crossoffset_spin = new QSpinBox(this);
    crossoffset_spin->setMinimum(0);
    crossoffset_spin->setMaximum(99999);

    connect(grid_check, &QAbstractButton::toggled, grid_label, &QWidget::setEnabled);
    connect(grid_check, &QAbstractButton::toggled, gpitch_spin, &QWidget::setEnabled);
    connect(cross_check, &QAbstractButton::toggled, crossoffset_spin, &QWidget::setEnabled);

    connect(div_check, &QAbstractButton::toggled, this, &SnapJointWidget::resetDivButtonStatus);
    connect(divuni_check, &QAbstractButton::toggled, this, &SnapJointWidget::resetDivButtonStatus);
    connect(divdis_check, &QAbstractButton::toggled, this, &SnapJointWidget::resetDivButtonStatus);

    grid_label->setEnabled(false);
    gpitch_spin->setEnabled(false);
    divuni_check->setEnabled(false);
    divuni_label->setEnabled(false);
    divuni_spin->setEnabled(false);
    divdis_check->setEnabled(false);
    divdisI_label->setEnabled(false);
    divdisI_spin->setEnabled(false);
    divdisJ_label->setEnabled(false);
    divdisJ_spin->setEnabled(false);

    connect(grid_check, &QAbstractButton::toggled, this, &SnapJointWidget::uploadSettingStatus);
    connect(gpitch_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &SnapJointWidget::uploadSettingStatus);
    connect(cross_check, &QAbstractButton::toggled, this, &SnapJointWidget::uploadSettingStatus);
    connect(framedef_check, &QAbstractButton::toggled, this, &SnapJointWidget::uploadSettingStatus);
    connect(div_check, &QAbstractButton::toggled, this, &SnapJointWidget::uploadSettingStatus);
    connect(divuni_check, &QAbstractButton::toggled, this, &SnapJointWidget::uploadSettingStatus);
    connect(divuni_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &SnapJointWidget::uploadSettingStatus);
    connect(divdis_check, &QAbstractButton::toggled, this, &SnapJointWidget::uploadSettingStatus);
    connect(divdisI_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &SnapJointWidget::uploadSettingStatus);
    connect(divdisJ_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &SnapJointWidget::uploadSettingStatus);
    connect(crossoffset_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &SnapJointWidget::uploadSettingStatus);

    QHBoxLayout *hlay0 = new QHBoxLayout;
    hlay0->setSpacing(0);
    hlay0->addWidget(cross_check);
    hlay0->addStretch();
    hlay0->addWidget(crossoffset_label);
    hlay0->addWidget(crossoffset_spin);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->setSpacing(0);
    hlay1->addWidget(grid_check);
    hlay1->addStretch();
    hlay1->addWidget(grid_label);
    hlay1->addWidget(gpitch_spin);

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->setSpacing(0);
    hlay2->addSpacing(20);
    hlay2->addWidget(divuni_check);
    hlay2->addStretch();
    hlay2->addWidget(divuni_label);
    hlay2->addWidget(divuni_spin);

    QHBoxLayout *hlay3 = new QHBoxLayout;
    hlay3->setSpacing(0);
    hlay3->addSpacing(20);
    hlay3->addWidget(divdis_check);

    QHBoxLayout *hlay4 = new QHBoxLayout;
    hlay4->addSpacing(40);
    hlay4->setSpacing(0);
    hlay4->addWidget(divdisI_label);
    hlay4->addWidget(divdisI_spin);
    hlay4->addStretch();
    hlay4->addWidget(divdisJ_label);
    hlay4->addWidget(divdisJ_spin);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(1);
    vlay->addWidget(snap_label);
    vlay->addSpacing(5);
    vlay->addLayout(hlay0);
    vlay->addWidget(framedef_check);
    vlay->addLayout(hlay1);
    vlay->addWidget(div_check);
    vlay->addLayout(hlay2);
    vlay->addLayout(hlay3);
    vlay->addLayout(hlay4);

    this->setLayout(vlay);

    setWidgetStatus(UnifiedEditingData::getInstance()->currentSnapState());
    signalOn = true;

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentSnapStateChanged,
            this, &SnapJointWidget::setWidgetStatus );
}

void SnapJointWidget::resetDivButtonStatus()
{
    signalOn = false;
    bool flg_div = div_check->isChecked();
    bool flg_divuni = (flg_div) ? divuni_check->isChecked() : false;
    bool flg_divdis = (flg_div) ? divdis_check->isChecked() : false;

    divuni_label->setEnabled(flg_divuni);
    divuni_spin->setEnabled(flg_divuni);
    divdisI_label->setEnabled(flg_divdis);
    divdisI_spin->setEnabled(flg_divdis);
    divdisJ_label->setEnabled(flg_divdis);
    divdisJ_spin->setEnabled(flg_divdis);

    divuni_check->setEnabled(flg_div);
    divdis_check->setEnabled(flg_div);
    signalOn = true;
}

void SnapJointWidget::setWidgetStatus(SNAPSTATE state)
{
    signalOn = false;
    grid_check->setChecked(state.gridIsChecked);
    gpitch_spin->setValue(state.gridPitch);
    cross_check->setChecked(state.crossIsChecked);
    framedef_check->setChecked(state.defineIsChecked);
    div_check->setChecked(state.divideIsChecked);
    divuni_check->setChecked(state.divideEquallyIsChecked);
    divuni_spin->setValue(state.divideParts);
    divdis_check->setChecked(state.divideFromIJIsChecked);
    divdisI_spin->setValue(state.divideFromI);
    divdisJ_spin->setValue(state.divideFromJ);
    crossoffset_spin->setValue(state.crossOffset);
    signalOn = true;
}

void SnapJointWidget::uploadSettingStatus()
{
    if (!signalOn) return;
    SNAPSTATE state = SNAPSTATE(grid_check->isChecked(), gpitch_spin->value(),
                                cross_check->isChecked(), framedef_check->isChecked(),
                                div_check->isChecked(), divuni_check->isChecked(),
                                divuni_spin->value(), divdis_check->isChecked(),
                                divdisI_spin->value(), divdisJ_spin->value(),crossoffset_spin->value());
    UnifiedEditingData::getInstance()->uploadSettingStatus(state);
}


/*追加・移動時の設定*/
AddMoveWidget::AddMoveWidget( QWidget *parent ) : QWidget( parent )
{
    signalOn = false;

    QLabel *pos_label = new QLabel(this);
    QRadioButton *inter_button = new QRadioButton(this);
    QRadioButton *compos_button = new QRadioButton(this);
    QRadioButton *self_button = new QRadioButton(this);

    pos_label->setText(u8"・追加/変更節点が他要素上に位置");
    inter_button->setText(u8"他要素上の内分点とする");
    compos_button->setText(u8"他要素を構成点により分割する");
    self_button->setText(u8"他要素と無関係の構成点とする");

    am_group = new QButtonGroup;
    am_group->addButton(inter_button, 0);
    am_group->addButton(compos_button, 1);
    am_group->addButton(self_button, 2);

    connect(am_group, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &AddMoveWidget::uploadSettingStatus);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addWidget(pos_label);
    vlay->addSpacing(5);
    vlay->addWidget(inter_button);
    vlay->addWidget(compos_button);
    vlay->addWidget(self_button);

    this->setLayout(vlay);

    setWidgetStatus(UnifiedEditingData::getInstance()->currentAddMoveAction());

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentAddMoveActionChanged,
            this, &AddMoveWidget::setWidgetStatus );

    signalOn = true;
}

void AddMoveWidget::setWidgetStatus(ADDMOVEACTION act)
{
    signalOn = false;
    am_group->button(act)->setChecked(true);
    signalOn = true;
}

void AddMoveWidget::uploadSettingStatus()
{
    if (!signalOn) return;
    ADDMOVEACTION id = static_cast<ADDMOVEACTION>(am_group->checkedId());
    UnifiedEditingData::getInstance()->uploadSettingStatus(id);
}


/*追加・移動された時の設定*/
AddedMovedWidget::AddedMovedWidget( QWidget *parent ) : QWidget( parent )
{
    signalOn = false;

    QLabel *pos_label = new QLabel(this);
    QRadioButton *inter_button = new QRadioButton(this);
    QRadioButton *compos_button = new QRadioButton(this);

    pos_label->setText(u8"・追加/変更された要素上に他節点が位置");
    inter_button->setText(u8"要素上の内分点とする");
    compos_button->setText(u8"要素と無関係の節点と見なす");

    amd_group = new QButtonGroup;
    amd_group->addButton(inter_button, 0);
    amd_group->addButton(compos_button, 1);

    connect(amd_group, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &AddedMovedWidget::uploadSettingStatus);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addWidget(pos_label);
    vlay->addSpacing(5);
    vlay->addWidget(inter_button);
    vlay->addWidget(compos_button);

    this->setLayout(vlay);

    setWidgetStatus(UnifiedEditingData::getInstance()->currentAddMovedAction());

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentAddMovedActionChanged,
            this, &AddedMovedWidget::setWidgetStatus );

    signalOn = true;
}

void AddedMovedWidget::setWidgetStatus(ADDMOVEDACTION act)
{
    signalOn = false;
    amd_group->button(act)->setChecked(true);
    signalOn = true;
}

void AddedMovedWidget::uploadSettingStatus()
{
    if (!signalOn) return;
    ADDMOVEDACTION id = static_cast<ADDMOVEDACTION>(amd_group->checkedId());
    UnifiedEditingData::getInstance()->uploadSettingStatus(id);
}


/*削除時の設定*/
DeleteWidget::DeleteWidget( QWidget *parent ) : QWidget( parent )
{
    signalOn = false;

    QLabel *del_label = new QLabel(this);
    QRadioButton *del_button = new QRadioButton(this);
    QRadioButton *remain_button = new QRadioButton(this);

    del_label->setText(u8"・編集後に要素が属さない節点ができた場合");
    del_button->setText(u8"節点を自動的に削除する");
    remain_button->setText(u8"単独の構成点として残す");

    del_group = new QButtonGroup;
    del_group->addButton(del_button, 0);
    del_group->addButton(remain_button, 1);

    connect(del_group, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &DeleteWidget::uploadSettingStatus);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addWidget(del_label);
    vlay->addSpacing(5);
    vlay->addWidget(del_button);
    vlay->addWidget(remain_button);

    this->setLayout(vlay);

    setWidgetStatus(UnifiedEditingData::getInstance()->currentDeleteAction());

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentDeleteActionChanged,
            this, &DeleteWidget::setWidgetStatus );

    signalOn = true;
}

void DeleteWidget::setWidgetStatus(DELETEACTION act)
{
    signalOn = false;
    del_group->button(act)->setChecked(true);
    signalOn = true;
}

void DeleteWidget::uploadSettingStatus()
{
    if (!signalOn) return;
    DELETEACTION id = static_cast<DELETEACTION>(del_group->checkedId());
    UnifiedEditingData::getInstance()->uploadSettingStatus(id);
}


/*節点一括入力時の設定*/
JointBatchWidget::JointBatchWidget( QWidget *parent ) : QWidget( parent )
{
    signalOn = false;
    QRadioButton *cross_button = new QRadioButton(u8"通り芯の交点", this);
    QRadioButton *define_button = new QRadioButton(u8"通りの定義点", this);
    QRadioButton *grid_button = new QRadioButton(u8"グリッド点", this);
    grid_label = new QLabel(u8"ピッチ(mm)：", this);
    gpitch_spin = new QSpinBox(this);
    gpitch_spin->setMinimum(1);
    gpitch_spin->setMaximum(99999);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setSpacing(0);
    hlay->addWidget(grid_button);
    hlay->addStretch();
    hlay->addWidget(grid_label);
    hlay->addWidget(gpitch_spin);

    batch_group = new QButtonGroup;
    batch_group->addButton(cross_button, 0);
    batch_group->addButton(define_button, 1);
    batch_group->addButton(grid_button, 2);

    connect(batch_group, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &JointBatchWidget::uploadSettingStatus);
    connect(gpitch_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &JointBatchWidget::uploadSettingStatus);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addWidget(new QLabel(u8"・一括入力の対象", this));
    vlay->addSpacing(5);
    vlay->addWidget(cross_button);
    vlay->addWidget(define_button);
    vlay->addLayout(hlay);

    this->setLayout(vlay);

    setWidgetStatus(UnifiedEditingData::getInstance()->currentJointBatchAction(),
                    UnifiedEditingData::getInstance()->currentJointBatchPitch());

    signalOn = true;
}

void JointBatchWidget::setWidgetStatus(JOINTBATCHACTION act, int value)
{
    signalOn = false;

    batch_group->button(act)->setChecked(true);
    if (value > 0) gpitch_spin->setValue(value);

    bool ok = (act == SPGRID) ;
    grid_label->setEnabled(ok);
    gpitch_spin->setEnabled(ok);

    signalOn = true;
}

void JointBatchWidget::uploadSettingStatus()
{
    if (!signalOn) return;
    JOINTBATCHACTION act = static_cast<JOINTBATCHACTION>(batch_group->checkedId());
    bool ok = (act == SPGRID) ;
    grid_label->setEnabled(ok);
    gpitch_spin->setEnabled(ok);
    int value = gpitch_spin->value();
    UnifiedEditingData::getInstance()->uploadSettingStatus(act, value);
}


/*スピンボックス*/
OperationSpinBox::OperationSpinBox( ADDEDITMODE mode, QWidget *parent ) :
    QSpinBox( parent ), myMode(mode)
{
    signalOn = false;

    this->setMinimum(1);
    this->setMaximum(99999);
    connect(this, QOverload<int>::of(&QSpinBox::valueChanged), this, &OperationSpinBox::uploadSettingStatus);

    if (myMode == DIVIDEMODE) {
        setWidgetStatus(UnifiedEditingData::getInstance()->currentBeamDivValue());
    } else if (myMode == PARALLELMODE) {
        setWidgetStatus(UnifiedEditingData::getInstance()->currentBeamPalValue());
    } else if (myMode == VERTICALMODE) {
        setWidgetStatus(UnifiedEditingData::getInstance()->currentBeamVerValue());
    } else if (myMode == EXTENDMODE) {
        setWidgetStatus(UnifiedEditingData::getInstance()->currentBeamExtValue());
    }

    signalOn = true;
}

void OperationSpinBox::setWidgetStatus(int value)
{
    signalOn = false;
    this->setValue(value);
    signalOn = true;
}

void OperationSpinBox::uploadSettingStatus()
{
    if (!signalOn) return;
    UnifiedEditingData::getInstance()->uploadSettingStatus(myMode, this->value());
}

/*削除時の設定*/
TargetAreaWidget::TargetAreaWidget( QWidget *parent ) : QWidget( parent )
{
    signalOn = false;

    QLabel *tar_label = new QLabel(this);
    QRadioButton *cg_button = new QRadioButton(this);
    QRadioButton *cgb_button = new QRadioButton(this);

    tar_label->setText(u8"・入力候補とする領域");
    cg_button->setText(u8"大梁/柱に囲まれる領域");
    cgb_button->setText(u8"小梁/大梁/柱に囲まれる領域");

    tar_group = new QButtonGroup;
    tar_group->addButton(cg_button, 0);
    tar_group->addButton(cgb_button, 1);

    connect(tar_group, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &TargetAreaWidget::uploadSettingStatus);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addWidget(tar_label);
    vlay->addSpacing(5);
    vlay->addWidget(cg_button);
    vlay->addWidget(cgb_button);

    this->setLayout(vlay);

    setWidgetStatus(UnifiedEditingData::getInstance()->currentTargetAction());

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentTargetActionChanged,
            this, &TargetAreaWidget::setWidgetStatus );

    signalOn = true;
}

void TargetAreaWidget::setWidgetStatus(TARGETACTION act)
{
    signalOn = false;
    tar_group->button(act)->setChecked(true);
    signalOn = true;
}

void TargetAreaWidget::uploadSettingStatus()
{
    if (!signalOn) return;
    TARGETACTION id = static_cast<TARGETACTION>(tar_group->checkedId());
    UnifiedEditingData::getInstance()->uploadSettingStatus(id);
}


} // namespace post3dapp

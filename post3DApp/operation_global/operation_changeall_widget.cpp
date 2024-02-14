#include "operation_changeall_widget.h"

#include <QtGui>
#include <QDebug>

#include "elided_label.h"
#include "expand_label.h"
#include "nameselect_dialog.h"
#include "reference_combobox.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "utility.h"
#include "vhline.h"

namespace post3dapp{
OperationChangeAllWidget::OperationChangeAllWidget( QWidget *parent )
    : QWidget( parent )
{
    createFloorCopyWidget();
    createChangeNameWidget();
    createSortJointWidget();
    createStackedLayout();
}

void OperationChangeAllWidget::createFloorCopyWidget()
{
    //  コピー元
    QLabel *source_label = new QLabel(u8"コピー元：", this);
    source_label->setFixedWidth(60);
    sourceCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::Input, this);
    sourceCombobox->setMinimumWidth(125);
    sourceCombobox->setDefaultName(u8"（階の選択）");

    QHBoxLayout *hlay_source = new QHBoxLayout;
    hlay_source->addWidget(source_label);
    hlay_source->addWidget(sourceCombobox);

    //  コピー先
    QLabel *dest_label = new QLabel(u8"コピー先：", this);
    dest_label->setFixedWidth(60);
    QPushButton *select_button = new QPushButton(u8"階選択", this);
    select_button->setFixedSize(60, 20);
    destinationFloorLabel = new ElidedLabel("", this);
    destinationFloorLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    destinationFloorLabel->setTextFormat(Qt::AutoText);

    connect(select_button, &QAbstractButton::clicked, this, &OperationChangeAllWidget::showSelectFloorDialog);

    QHBoxLayout *hlay_dest = new QHBoxLayout;
    hlay_dest->addWidget(dest_label);
    hlay_dest->addWidget(select_button);
    hlay_dest->addWidget(destinationFloorLabel);

    //  ・コピーの基準
    QRadioButton *base_xy_button = new QRadioButton(u8"XY座標", this);
    QRadioButton *base_attach_button = new QRadioButton(u8"通り属性", this);
    baseButtonGroup = new QButtonGroup;
    baseButtonGroup->addButton(base_xy_button, 0);
    baseButtonGroup->addButton(base_attach_button, 1);
    base_xy_button->setChecked(true);
    base_xy_button->setFixedWidth(110);

    QHBoxLayout *hlay_base1 = new QHBoxLayout;
    hlay_base1->addWidget(new QLabel(u8"・コピーの基準", this));
    hlay_base1->addStretch();
    QHBoxLayout *hlay_base2 = new QHBoxLayout;
    hlay_base2->addSpacing(15);
    hlay_base2->addWidget(base_xy_button);
    hlay_base2->addWidget(base_attach_button);
    hlay_base2->addStretch();

    //  ・上階に繋がる要素
    QRadioButton *upper_contain_button = new QRadioButton(u8"コピーに含める", this);
    QRadioButton *upper_ignore_button = new QRadioButton(u8"コピーに含めない", this);
    upperButtonGroup = new QButtonGroup;
    upperButtonGroup->addButton(upper_contain_button, 0);
    upperButtonGroup->addButton(upper_ignore_button, 1);
    upper_contain_button->setChecked(true);
    upper_contain_button->setFixedWidth(110);

    QHBoxLayout *hlay_upper1 = new QHBoxLayout;
    hlay_upper1->addWidget(new QLabel(u8"一つ上の階に繋がる部材/荷重", this));
    hlay_upper1->addStretch();
    QHBoxLayout *hlay_upper2 = new QHBoxLayout;
    hlay_upper2->addSpacing(15);
    hlay_upper2->addWidget(upper_contain_button);
    hlay_upper2->addWidget(upper_ignore_button);
    hlay_upper2->addStretch();

    //  ・節点が重複する場合
    QRadioButton *joint_overwrite_button = new QRadioButton(u8"上書きする", this);
    QRadioButton *joint_ignore_button = new QRadioButton(u8"コピーしない", this);
    jointButtonGroup = new QButtonGroup;
    jointButtonGroup->addButton(joint_overwrite_button, 0);
    jointButtonGroup->addButton(joint_ignore_button, 1);
    joint_overwrite_button->setChecked(true);
    joint_overwrite_button->setFixedWidth(110);

    QHBoxLayout *hlay_joint1 = new QHBoxLayout;
    hlay_joint1->addWidget(new QLabel(u8"・コピー先で節点が重複する場合", this));
    hlay_joint1->addStretch();
    QHBoxLayout *hlay_joint2 = new QHBoxLayout;
    hlay_joint2->addSpacing(15);
    hlay_joint2->addWidget(joint_overwrite_button);
    hlay_joint2->addWidget(joint_ignore_button);
    hlay_joint2->addStretch();

    //  ・部材が重複する場合
    QRadioButton *member_overwrite_button = new QRadioButton(u8"上書きする", this);
    QRadioButton *member_ignore_button = new QRadioButton(u8"コピーしない", this);
    memberButtonGroup = new QButtonGroup;
    memberButtonGroup->addButton(member_overwrite_button, 0);
    memberButtonGroup->addButton(member_ignore_button, 1);
    member_overwrite_button->setChecked(true);
    member_overwrite_button->setFixedWidth(110);

    QHBoxLayout *hlay_member1 = new QHBoxLayout;
    hlay_member1->addWidget(new QLabel(u8"・コピー先で部材/荷重が重複する場合", this));
    hlay_member1->addStretch();
    QHBoxLayout *hlay_member2 = new QHBoxLayout;
    hlay_member2->addSpacing(15);
    hlay_member2->addWidget(member_overwrite_button);
    hlay_member2->addWidget(member_ignore_button);
    hlay_member2->addStretch();

    //  ・通りの制限
    frameCheckBox = new QCheckBox(u8"通りを制限する（全所属）", this);

    QHBoxLayout *hlay_frame1 = new QHBoxLayout;
    hlay_frame1->addWidget(frameCheckBox);
    hlay_frame1->addStretch();

    frameButton = new QPushButton(u8"通り選択", this);
    frameButton->setFixedSize(60, 20);
    frameRangeLabel = new ElidedLabel("", this);
    frameRangeLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    frameRangeLabel->setTextFormat(Qt::AutoText);

    connect(frameButton, &QAbstractButton::clicked, this, &OperationChangeAllWidget::showFrameRangeDialog);

    QHBoxLayout *hlay_frame2 = new QHBoxLayout;
    hlay_frame2->addSpacing(15);
    hlay_frame2->addWidget(frameButton);
    hlay_frame2->addWidget(frameRangeLabel);

    frameCheckBox->setChecked(false);
    slotEnableFrameLimit(false);
    connect(frameCheckBox, QOverload<bool>::of(&QAbstractButton::toggled), this, &OperationChangeAllWidget::slotEnableFrameLimit );

    //  ・対象要素の制限
    elementCheckBox = new QCheckBox(u8"対象要素（部材/荷重）を制限する", this);

    QHBoxLayout *hlay_element1 = new QHBoxLayout;
    hlay_element1->addWidget(elementCheckBox);
    hlay_element1->addStretch();

    elementButton = new QPushButton(u8"要素選択", this);
    elementButton->setFixedSize(60, 20);
    limitedElementLabel = new ElidedLabel("", this);
    limitedElementLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    limitedElementLabel->setTextFormat(Qt::AutoText);

    connect(elementButton, &QAbstractButton::clicked, this, &OperationChangeAllWidget::showElementLimitDialog);

    QHBoxLayout *hlay_element2 = new QHBoxLayout;
    hlay_element2->addSpacing(15);
    hlay_element2->addWidget(elementButton);
    hlay_element2->addWidget(limitedElementLabel);

    elementCheckBox->setChecked(false);
    slotEnableElementLimit(false);
    connect(elementCheckBox, &QAbstractButton::toggled, this, &OperationChangeAllWidget::slotEnableElementLimit );

    //  ・XY座標の制限
    rangeCheckBox = new QCheckBox(u8"コピー範囲のXY座標を制限する", this);

    QHBoxLayout *hlay_range1 = new QHBoxLayout;
    hlay_range1->addWidget(rangeCheckBox);
    hlay_range1->addStretch();

    rangeMinXLabel = new QLabel(u8"最小 X(m)：", this);
    minXLine = new QLineEdit(this);
    minXLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    minXLine->setAlignment(Qt::AlignRight);
    minXLine->setFixedWidth(60);
    rangeMinYLabel = new QLabel("Y(m)：", this);
    minYLine = new QLineEdit(this);
    minYLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    minYLine->setAlignment(Qt::AlignRight);
    minYLine->setFixedWidth(60);

    QHBoxLayout *hlay_range2 = new QHBoxLayout;
    hlay_range2->addSpacing(15);
    hlay_range2->addWidget(rangeMinXLabel);
    hlay_range2->addWidget(minXLine);
    hlay_range2->addSpacing(5);
    hlay_range2->addWidget(rangeMinYLabel);
    hlay_range2->addWidget(minYLine);

    rangeMaxXLabel = new QLabel(u8"最大 X(m)：");
    maxXLine = new QLineEdit(this);
    maxXLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    maxXLine->setAlignment(Qt::AlignRight);
    maxXLine->setFixedWidth(60);
    rangeMaxYLabel = new QLabel("Y(m)：", this);
    maxYLine = new QLineEdit(this);
    maxYLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    maxYLine->setAlignment(Qt::AlignRight);
    maxYLine->setFixedWidth(60);

    QHBoxLayout *hlay_range3 = new QHBoxLayout;
    hlay_range3->addSpacing(15);
    hlay_range3->addWidget(rangeMaxXLabel);
    hlay_range3->addWidget(maxXLine);
    hlay_range3->addSpacing(5);
    hlay_range3->addWidget(rangeMaxYLabel);
    hlay_range3->addWidget(maxYLine);

    rangeCheckBox->setChecked(false);
    slotEnableRangeLimit(false);
    connect(rangeCheckBox, &QAbstractButton::toggled, this, &OperationChangeAllWidget::slotEnableRangeLimit );

    //  階コピーの実行
    QPushButton *floorcopy_button = new QPushButton(u8"階コピーの実行", this);
    floorcopy_button->setFixedHeight(25);
    connect(floorcopy_button, &QAbstractButton::clicked, this, &OperationChangeAllWidget::executeFloorCopy);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay_source);
    vlay->addSpacing(3);
    vlay->addLayout(hlay_dest);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addLayout(hlay_base1);
    vlay->addLayout(hlay_base2);
    vlay->addSpacing(3);
    vlay->addLayout(hlay_upper1);
    vlay->addLayout(hlay_upper2);
    vlay->addSpacing(3);
    vlay->addLayout(hlay_joint1);
    vlay->addLayout(hlay_joint2);
    vlay->addSpacing(3);
    vlay->addLayout(hlay_member1);
    vlay->addLayout(hlay_member2);
    vlay->addSpacing(3);
    vlay->addLayout(hlay_frame1);
    vlay->addLayout(hlay_frame2);
    vlay->addSpacing(3);
    vlay->addLayout(hlay_element1);
    vlay->addLayout(hlay_element2);
    vlay->addSpacing(3);
    vlay->addLayout(hlay_range1);
    vlay->addLayout(hlay_range2);
    vlay->addLayout(hlay_range3);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addWidget(floorcopy_button);
    vlay->addStretch();

    floorCopyWidget = new QWidget(this);
    floorCopyWidget->setLayout(vlay);
}

void OperationChangeAllWidget::createChangeNameWidget()
{
    // 種類
    QLabel *element_label = new QLabel(u8"要素種類：", this);
    element_label->setFixedWidth(90);

    changedTypeCombobox = new QComboBox(this);
    changedTypeCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    changedTypeCombobox->setMaxVisibleItems(14);

    QStringList namelist;
    for ( int i = static_cast<int>(DATATYPE::TPCOLUMN); i <= static_cast<int>(DATATYPE::TPSLABLOAD); i++ ) {
        namelist.append( UnifiedInputData::getInInstance()->dataTypeToJapanese( static_cast<DATATYPE>(i) ) );
    }
    changedTypeCombobox->addItems(namelist);
    changedTypeCombobox->setMinimumWidth(100);
    changedTypeCombobox->setCurrentIndex(0);

    QHBoxLayout *hlay_element = new QHBoxLayout;
    hlay_element->addWidget(element_label);
    hlay_element->addWidget(changedTypeCombobox);

    // 変更前
    QLabel *oldname_label = new QLabel(u8"変更前の符号：", this);
    oldname_label->setFixedWidth(90);

    changedNameCombobox = new QComboBox(this);
    changedNameCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    changedNameCombobox->setMinimumWidth(100);
    changedNameCombobox->addItems(UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPCOLUMN) );

    connect ( changedTypeCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
              this, &OperationChangeAllWidget::updateChangedNameCombobox );
    connect ( UnifiedInputData::getInInstance(), &UnifiedData::dataUpdated,
              this, &OperationChangeAllWidget::updateChangedNameCombobox );

    QHBoxLayout *hlay_oldname = new QHBoxLayout;
    hlay_oldname->addWidget(oldname_label);
    hlay_oldname->addWidget(changedNameCombobox);

    // 変更後
    QLabel *newname_label = new QLabel(u8"変更後の符号：", this);
    newname_label->setFixedWidth(90);

    newNameLineEdit = new QLineEdit(this);
    newNameLineEdit->setMinimumWidth(100);

    QHBoxLayout *hlay_newname = new QHBoxLayout;
    hlay_newname->addWidget(newname_label);
    hlay_newname->addWidget(newNameLineEdit);

    //  変更の実行
    QPushButton *change_button = new QPushButton(u8"符号名の変更", this);
    change_button->setFixedHeight(25);
    connect(change_button, &QAbstractButton::clicked, this, &OperationChangeAllWidget::executeChangeName);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay_element);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addLayout(hlay_oldname);
    vlay->setSpacing(3);
    vlay->addLayout(hlay_newname);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addWidget(change_button);
    vlay->addStretch();

    changeNameWidget = new QWidget(this);
    changeNameWidget->setLayout(vlay);
}

void OperationChangeAllWidget::createSortJointWidget()
{
    // 第一
    QLabel *first_label = new QLabel(u8"第一優先：", this);
    first_label->setFixedWidth(80);

    QLabel *first_label2 = new QLabel(u8"Z座標", this);
    first_label2->setMinimumWidth(100);

    QRadioButton *first_ascend = new QRadioButton(u8"昇順", this);
    QRadioButton *first_descend = new QRadioButton(u8"降順", this);
    firstButtonGroup = new QButtonGroup;
    firstButtonGroup->addButton(first_ascend, 0);
    firstButtonGroup->addButton(first_descend, 1);
    first_ascend->setChecked(true);
    first_ascend->setFixedWidth(100);

    QHBoxLayout *hlay_first = new QHBoxLayout;
    hlay_first->addWidget(first_label);
    hlay_first->addWidget(first_label2);
    QHBoxLayout *hlay_first2 = new QHBoxLayout;
    hlay_first2->addSpacing(15);
    hlay_first2->addWidget(first_ascend);
    hlay_first2->addWidget(first_descend);
    hlay_first2->addStretch();

    // パターン
    QStringList patternList{u8"無し", u8"X座標", u8"Y座標", "X+Y"};

    // 第二
    QLabel *second_label = new QLabel(u8"第二優先：", this);
    second_label->setFixedWidth(80);

    secondCombobox = new QComboBox(this);
    secondCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    secondCombobox->addItems( patternList );
    secondCombobox->setCurrentIndex(1);

    QRadioButton *second_ascend = new QRadioButton(u8"昇順", this);
    QRadioButton *second_descend = new QRadioButton(u8"降順", this);
    secondButtonGroup = new QButtonGroup;
    secondButtonGroup->addButton(second_ascend, 0);
    secondButtonGroup->addButton(second_descend, 1);
    second_ascend->setChecked(true);
    second_ascend->setFixedWidth(100);

    QHBoxLayout *hlay_second = new QHBoxLayout;
    hlay_second->addWidget(second_label);
    hlay_second->addWidget(secondCombobox);
    QHBoxLayout *hlay_second2 = new QHBoxLayout;
    hlay_second2->addSpacing(15);
    hlay_second2->addWidget(second_ascend);
    hlay_second2->addWidget(second_descend);
    hlay_second2->addStretch();

    // 第三
    QLabel *third_label = new QLabel(u8"第三優先：", this);
    third_label->setFixedWidth(80);

    thirdCombobox = new QComboBox(this);
    thirdCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    thirdCombobox->addItems( patternList );
    thirdCombobox->setCurrentIndex(2);

    QRadioButton *third_ascend = new QRadioButton(u8"昇順", this);
    QRadioButton *third_descend = new QRadioButton(u8"降順", this);
    thirdButtonGroup = new QButtonGroup;
    thirdButtonGroup->addButton(third_ascend, 0);
    thirdButtonGroup->addButton(third_descend, 1);
    third_ascend->setChecked(true);
    third_ascend->setFixedWidth(100);

    QHBoxLayout *hlay_third = new QHBoxLayout;
    hlay_third->addWidget(third_label);
    hlay_third->addWidget(thirdCombobox);
    QHBoxLayout *hlay_third2 = new QHBoxLayout;
    hlay_third2->addSpacing(15);
    hlay_third2->addWidget(third_ascend);
    hlay_third2->addWidget(third_descend);
    hlay_third2->addStretch();


    QPushButton *exec_button = new QPushButton(u8"節点番号のソート実行", this);
    exec_button->setFixedHeight(25);
    connect(exec_button, &QAbstractButton::clicked, this, &OperationChangeAllWidget::executeJointSort);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay_first);
    vlay->addLayout(hlay_first2);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addLayout(hlay_second);
    vlay->addLayout(hlay_second2);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addLayout(hlay_third);
    vlay->addLayout(hlay_third2);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addWidget(exec_button);
    vlay->addStretch();

    sortJointWidget = new QWidget(this);
    sortJointWidget->setLayout(vlay);
}

void OperationChangeAllWidget::createStackedLayout()
{
    QComboBox *operationComb = new QComboBox(this);
    operationComb->setStyle( UnifiedEditingData::getInstance()->getWindowXPStyle() );
    operationComb->setMaxVisibleItems(5);
    operationComb->addItems(QStringList{{u8"階コピー", u8"符号名の変更", u8"節点番号のソート"}});

    QStackedWidget *stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(floorCopyWidget);
    stackedWidget->addWidget(changeNameWidget);
    stackedWidget->addWidget(sortJointWidget);

    connect(operationComb, QOverload<int>::of(&QComboBox::currentIndexChanged),
            stackedWidget, &QStackedWidget::setCurrentIndex);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(3);
    vlay->addWidget(operationComb);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addWidget(stackedWidget);
    vlay->addStretch();

    this->setLayout(vlay);
}

void OperationChangeAllWidget::slotEnableFrameLimit(bool onoff)
{
    frameButton->setEnabled(onoff);
    frameRangeLabel->setEnabled(onoff);
}

void OperationChangeAllWidget::slotEnableElementLimit(bool onoff)
{
    elementButton->setEnabled(onoff);
    limitedElementLabel->setEnabled(onoff);
}

void OperationChangeAllWidget::slotEnableRangeLimit(bool onoff)
{
    rangeMinXLabel->setEnabled(onoff);
    minXLine->setEnabled(onoff);
    rangeMinYLabel->setEnabled(onoff);
    minYLine->setEnabled(onoff);
    rangeMaxXLabel->setEnabled(onoff);
    maxXLine->setEnabled(onoff);
    rangeMaxYLabel->setEnabled(onoff);
    maxYLine->setEnabled(onoff);
}

void OperationChangeAllWidget::showSelectFloorDialog()
{
    QStringList fllist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFLOOR);
    NameSelectDialog dialog(fllist, destinationFloors, this);
    dialog.setWindowTitle(u8"階の選択");
    if ( dialog.exec() ) {
        destinationFloors = dialog.selectedNames();
        QString str = ( destinationFloors.isEmpty() ) ? QString("") : destinationFloors.join(",") ;
        destinationFloorLabel->setText(str);
    }
}

void OperationChangeAllWidget::showFrameRangeDialog()
{
    QStringList frlist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFRAMEP);
    NameSelectDialog dialog(frlist, rangeFrames, this);
    dialog.setWindowTitle(u8"階の選択");
    if ( dialog.exec() ) {
        rangeFrames = dialog.selectedNames();
        QString str = ( rangeFrames.isEmpty() ) ? QString() : rangeFrames.join(",") ;
        frameRangeLabel->setText(str);
    }
}

void OperationChangeAllWidget::showElementLimitDialog()
{
    const QStringList namelist{u8"柱", u8"大梁", u8"ブレース", u8"小梁", u8"壁", u8"スラブ",
                               u8"地下外壁", u8"制振要素", u8"免震要素", u8"節点荷重", u8"部材荷重", u8"床荷重"};

    QStringList selected_names;
    Q_FOREACH ( int id, limitedElements ) {
        selected_names.append(namelist.at(id));
    }

    NameSelectDialog dialog(namelist, selected_names, this);
    dialog.setWindowTitle(u8"対象要素の選択");
    if ( dialog.exec() ) {
        selected_names = dialog.selectedNames();
        QString str = ( selected_names.isEmpty() ) ? QString() : selected_names.join(",") ;
        limitedElementLabel->setText(str);
        limitedElements.clear();
        Q_FOREACH ( QString name, selected_names ) {
            int idx = namelist.indexOf(name);
            limitedElements.append(idx);
        }
    }
}

void OperationChangeAllWidget::executeFloorCopy()
{
    QUuid sourceID = sourceCombobox->currentUuid();
    if ( sourceID.isNull() ) {
        Utility::showErrorMessage2(this, u8"コピー元の階名が指定されていません。");
        return;
    }

    QList<QUuid> destFloorIDs;
    Q_FOREACH ( QString str, destinationFloors ) {
        QUuid id = UnifiedInputData::getInInstance()->nameToID(str, DATATYPE::TPFLOOR);
        if ( !id.isNull() ) destFloorIDs.append(id);
    }
    if ( destFloorIDs.isEmpty() ) {
        Utility::showErrorMessage2(this, u8"コピー先の階が指定されていません。");
        return;
    }
    if ( destFloorIDs.contains(sourceID) ) {
        Utility::showErrorMessage2(this, u8"コピー元とコピー先の階が重複しています。");
        return;
    }

    QList<QUuid> limitFrames;
    Q_FOREACH ( QString str, rangeFrames ) {
        QUuid id = UnifiedInputData::getInInstance()->nameToID(str, DATATYPE::TPFRAMEP);
        if ( !id.isNull() ) limitFrames.append(id);
    }
    if ( frameCheckBox->isChecked() && limitFrames.isEmpty() ) {
        Utility::showErrorMessage2(this, u8"通りを制限する場合は通りを指定してください。");
        return;
    }

    QList<DATATYPE> limitDataTypes;
    Q_FOREACH ( int ii, limitedElements ) {
        DATATYPE type = DATATYPE::NODATATYPE;
        if ( ii == 0 ) {
            type = DATATYPE::TPCOLUMN;
        } else if ( ii == 1 ) {
            type = DATATYPE::TPGIRDER;
        } else if ( ii == 2 ) {
            type = DATATYPE::TPBRACE;
        } else if ( ii == 3 ) {
            type = DATATYPE::TPBEAM;
        } else if ( ii == 4 ) {
            type = DATATYPE::TPWALL;
        } else if ( ii == 5 ) {
            type = DATATYPE::TPSLAB;
        } else if ( ii == 6 ) {
            type = DATATYPE::TPUWALL;
        } else if ( ii == 7 ) {
            type = DATATYPE::TPDAMPER;
        } else if ( ii == 8 ) {
            type = DATATYPE::TPISO;
        } else if ( ii == 9 ) {
            type = DATATYPE::TPJOINTLOAD;
        } else if ( ii == 10 ) {
            type = DATATYPE::TPMEMBERLOAD;
        } else if ( ii == 11 ) {
            type = DATATYPE::TPSLABLOAD;
        }

        if ( type != DATATYPE::NODATATYPE ) limitDataTypes.append(type);
    }
    if ( elementCheckBox->isChecked() && limitDataTypes.isEmpty() ) {
        if ( Utility::showWarningMessage2(this, u8"制限する要素が指定されていません。\n節点のみのコピーを続行しますか？"))
            return;
    }

    qreal min_x = minXLine->text().toDouble();
    qreal min_y = minYLine->text().toDouble();
    qreal max_x = maxXLine->text().toDouble();
    qreal max_y = maxYLine->text().toDouble();

    if ( rangeCheckBox->isChecked() ) {
        if ( min_x + 1.0e-3 > max_x || min_y + 1.0e-3 > max_y ) {
            Utility::showErrorMessage2(this, u8"制限するXY座標の範囲が不正です。");
            return;
        }
    }

    if ( Utility::showWarningMessage2(this, u8"Undo/Redoバッファはクリアされます。\n階コピーを実行しますか？"))
        return;

    UnifiedEditingData::getInstance()->clearUndoStack();

    QProgressDialog progress(u8"階コピーを実行中です...", u8"キャンセル", 0, 100, this);
    progress.setFont( QFont(tr("Microsoft JhengHei"), 9) );
    progress.setWindowModality( Qt::WindowModal );
    progress.setWindowTitle(u8"階コピー");
    progress.setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
    qApp->processEvents();
    progress.setValue(0);

    FLOORCOPYTERM copyValue(sourceID, destFloorIDs, baseButtonGroup->checkedId() == 0,
                            upperButtonGroup->checkedId() == 0,
                            jointButtonGroup->checkedId() == 0, memberButtonGroup->checkedId() == 0,
                            frameCheckBox->isChecked(), limitFrames,
                            elementCheckBox->isChecked(), limitDataTypes,
                            rangeCheckBox->isChecked(), QPointF(min_x, min_y), QPointF(max_x, max_y));

    UnifiedInputData::getInInstance()->executeFloorCopy(copyValue, &progress, this);

    progress.setValue(100);
    progress.close();
}

void OperationChangeAllWidget::executeChangeName()
{
    DATATYPE dtype = static_cast<DATATYPE>( changedTypeCombobox->currentIndex() + static_cast<int>(DATATYPE::TPCOLUMN) );
    QString old_name = changedNameCombobox->currentText();
    QString new_name = newNameLineEdit->text();

    if ( !UnifiedInputData::getInInstance()->isExistedName(old_name, dtype) ) {
        Utility::showErrorMessage2(this, u8"変更前の符号が不正です。");
        return;
    }
    if ( new_name.trimmed().isEmpty() ) {
        Utility::showErrorMessage2(this, u8"変更後の符号が不正です。");
        return;
    }
    if ( UnifiedInputData::getInInstance()->isExistedName(new_name, dtype) ) {
        Utility::showErrorMessage2(this, u8"変更後の符号は既に存在しています。");
        return;
    }

    QString msg = u8"Undo/Redoバッファはクリアされます。\n";
    msg += u8"符号[" + old_name +u8"]を符号[" + new_name + u8"]に変更しますか？";

    if ( Utility::showWarningMessage2(this, msg) ) return;

    UnifiedEditingData::getInstance()->clearUndoStack();
    UnifiedInputData::getInInstance()->executeChangeName(dtype, old_name, new_name);
}

void OperationChangeAllWidget::executeJointSort()
{
    if ( Utility::showWarningMessage2(this, u8"Undo/Redoバッファはクリアされます。\n節点番号のソートを実行しますか？") )
        return;

    UnifiedEditingData::getInstance()->clearUndoStack();

    bool first_order = ( firstButtonGroup->checkedId() == 0 );
    JOINTSORTPATTERN second_pt = static_cast<JOINTSORTPATTERN>( secondCombobox->currentIndex() );
    bool second_order = ( secondButtonGroup->checkedId() == 0 );
    JOINTSORTPATTERN third_pt = static_cast<JOINTSORTPATTERN>( thirdCombobox->currentIndex() );
    bool third_order = ( thirdButtonGroup->checkedId() == 0 );

    UnifiedInputData::getInInstance()->executeJointSort( JOINTSORTTERM( first_order, second_pt, second_order,
                                                                 third_pt, third_order ) );

    Utility::showInformationMessage(this, u8"節点番号がソートされました。");
}

void OperationChangeAllWidget::updateChangedNameCombobox()
{
    DATATYPE dtype = static_cast<DATATYPE>( changedTypeCombobox->currentIndex() + static_cast<int>(DATATYPE::TPCOLUMN) );
    changedNameCombobox->clear();
    changedNameCombobox->addItems( UnifiedInputData::getInInstance()->getNameList(dtype) );
}

} // namespace post3dapp

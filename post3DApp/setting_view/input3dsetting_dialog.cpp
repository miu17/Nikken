#include "input3dsetting_dialog.h"

#include <QtGui>

#include "define_individualsetting.h"
#include "elided_label.h"
#include "individualsetting_tablewidget.h"
#include "nameselect_dialog.h"
#include "reference_combobox.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
#include "unified_inputdata.h"
#include "unified_settings.h"
#include "vhline.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{
Input3DSettingDialog::Input3DSettingDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setFont(QFont(tr("Microsoft JhengHei"), 9));

    connect( UnifiedInputData::getInInstance(), &UnifiedData::dataUpdated,
             this, &Input3DSettingDialog::updateSelectedFloorFrame );

    createAllWidgets();
    createIndividualWidgets();

    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(persAllWidget, u8"全　体");
    tabWidget->addTab(persIndividualWidget, u8"符号別");
    QPalette pal = tabWidget->palette();
    pal.setBrush(QPalette::Window, QBrush(Qt::white));
    tabWidget->setPalette(pal);

    QPushButton *fileOpenButton = new QPushButton(u8"ファイル読込", this);
    QPushButton *fileSaveButton = new QPushButton(u8"ファイル保存", this);
    QPushButton *defaultButton = new QPushButton(u8"デフォルト値", this);
    QPushButton *uploadButton = new QPushButton(u8"画面更新", this);
    fileOpenButton->setFixedWidth(90);
    fileSaveButton->setFixedWidth(90);
    defaultButton->setFixedWidth(90);
    uploadButton->setFixedWidth(90);

    connect( fileSaveButton, &QAbstractButton::clicked, this, &Input3DSettingDialog::slotFileSave );
    connect( fileOpenButton, &QAbstractButton::clicked, this, &Input3DSettingDialog::slotFileOpen );
    connect( defaultButton, &QAbstractButton::clicked, this, &Input3DSettingDialog::initializeSettings );
    connect( uploadButton,  &QAbstractButton::clicked, this, &Input3DSettingDialog::uploadViewSettings );

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setSpacing(5);
    hlay->addWidget(fileOpenButton);
    hlay->addWidget(fileSaveButton);
    hlay->addWidget(defaultButton);
    hlay->addStretch();
    hlay->addWidget(uploadButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(hlay);
    vlay->addWidget(tabWidget);
    this->setLayout(vlay);

    this->setWindowTitle(u8"形状モデル（３Ｄパース）　表示設定");
    this->setWindowIcon(QIcon(QPixmap(":/icons/color_settings.png")));
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(800, 650));

    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    ( QFile(filepath).exists() ) ? readSettings(filepath) : initializeSettings() ;
    uploadViewSettings();
}

Input3DSettingDialog::~Input3DSettingDialog()
{
    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    saveSettings(filepath);
}

void Input3DSettingDialog::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    QDialog::closeEvent(event);
}

void Input3DSettingDialog::initializeSettings()
{
    applySettings(Input3DViewSettings::getDefault());
    /*
    priorityCheckbox->setChecked(true);
    limitedFloorGroup->button(0)->setChecked(true);
    limitedFloors.clear();
    limitedFloorLabel->setText(QString());
    limitedFrameGroup->button(0)->setChecked(true);
    limitedFrames.clear();
    limitedFrameLabel->setText(QString());

    scaleColorCombobox->setCurrentIndex(80); // olivedrab
    frscaleCheck->setChecked(true);
    frscaleFloorCombobox->setCurrentIndex(0); // 表示しない
    flscaleCheck->setChecked(true);
    flscaleXLine->setText("-5.00");
    flscaleYLine->setText("-5.00");

    jointSizeCombobox->setCurrentIndex(1); // 小さめサイズ
    jointColorCombobox->setCurrentIndex(40); // blue
    memberSizeCombobox->setCurrentIndex(2); // 標準サイズ
    memberColorCombobox->setCurrentIndex(97); // glodenrod


    QColor dummy(TypicalColorComboBox::nameToColor("darkolivegreen"));
    dummy.setAlpha(100);
    elementStateWidgets.value(ELEMENTSTATE::ELDUMMY)->setElementSetting(COLORBRUSHSETTING(TypicalColorComboBox::nameToColor("green"), dummy));
    QColor error(TypicalColorComboBox::nameToColor("magenta"));
    error.setAlpha(100);
    elementStateWidgets.value(ELEMENTSTATE::ELERROR)->setElementSetting(COLORBRUSHSETTING(TypicalColorComboBox::nameToColor("magenta"), error));
    QColor select(TypicalColorComboBox::nameToColor("lawngreen"));
    select.setAlpha(150);
    elementStateWidgets.value(ELEMENTSTATE::ELSELECT)->setElementSetting(COLORBRUSHSETTING(TypicalColorComboBox::nameToColor("lime"), select));


    elementWidgets.value(ELEMENTTYPE::ELJOINT)->setElementSetting(ELEMENTVIEW(true, false, TypicalColorComboBox::nameToColor("blue")));
    QColor column(TypicalColorComboBox::nameToColor("skyblue"));
    column.setAlpha(200);
    elementWidgets.value(ELEMENTTYPE::ELCOLUMN)->setElementSetting(ELEMENTVIEW(true, true, TypicalColorComboBox::nameToColor("midnightblue"), column));
    QColor girder(TypicalColorComboBox::nameToColor("lightsteelblue"));
    girder.setAlpha(200);
    elementWidgets.value(ELEMENTTYPE::ELGIRDER)->setElementSetting(ELEMENTVIEW(true, true, TypicalColorComboBox::nameToColor("darkblue"), girder));
    QColor brace(TypicalColorComboBox::nameToColor("blue"));
    brace.setAlpha(200);
    elementWidgets.value(ELEMENTTYPE::ELBRACE)->setElementSetting(ELEMENTVIEW(true, true, TypicalColorComboBox::nameToColor("maroon"), brace));
    QColor beam(TypicalColorComboBox::nameToColor("gainsboro"));
    beam.setAlpha(200);
    elementWidgets.value(ELEMENTTYPE::ELBEAM)->setElementSetting(ELEMENTVIEW(true, true, TypicalColorComboBox::nameToColor("maroon"), beam));
    QColor slab(TypicalColorComboBox::nameToColor("seashell"));
    slab.setAlpha(150);
    elementWidgets.value(ELEMENTTYPE::ELSLAB)->setElementSetting(ELEMENTVIEW(true, true, TypicalColorComboBox::nameToColor("royalblue"), slab));
    QColor wall(TypicalColorComboBox::nameToColor("midnightblue"));
    wall.setAlpha(150);
    elementWidgets.value(ELEMENTTYPE::ELWALL)->setElementSetting(ELEMENTVIEW(true, true, TypicalColorComboBox::nameToColor("purple"), wall));
    QColor uwall(TypicalColorComboBox::nameToColor("lightskyblue"));
    uwall.setAlpha(200);
    elementWidgets.value(ELEMENTTYPE::ELUWALL)->setElementSetting(ELEMENTVIEW(true, true, TypicalColorComboBox::nameToColor("royalblue"), uwall));
    QColor damper(TypicalColorComboBox::nameToColor("orange"));
    damper.setAlpha(100);
    elementWidgets.value(ELEMENTTYPE::ELDAMPER)->setElementSetting(ELEMENTVIEW(true, true, TypicalColorComboBox::nameToColor("darkcyan"), damper));
    QColor iso(damper);
    elementWidgets.value(ELEMENTTYPE::ELISO)->setElementSetting(ELEMENTVIEW(true, true, TypicalColorComboBox::nameToColor("darkcyan"), iso));
    */
}

void Input3DSettingDialog::uploadViewSettings()
{
    QHash<ELEMENTTYPE, ELEMENTVIEW> elementHash;
    QHash<ELEMENTSTATE, COLORBRUSHSETTING> stateHash;

    Q_FOREACH ( ELEMENTSTATE state, elementStateWidgets.keys() )
        stateHash.insert( state, elementStateWidgets.value(state)->getElementSetting() );

    QMap<QString, COLORBRUSHSETTING> colorHash = persIndividualWidget->getColorTableData();

    QString str_fl = frscaleFloorCombobox->currentText();
    QPointF pp( flscaleXLine->text().toDouble() * 1000.0, flscaleYLine->text().toDouble() * 1000.0 );

    /*
    Input3DViewSettings settings = Input3DViewSettings(
                priorityCheckbox->isChecked(),
                static_cast<ELEMENTLIMITED>( limitedFloorGroup->checkedId() ),
                UnifiedInputData::getInInstance()->namesToIDs(limitedFloors, DATATYPE::TPFLOOR),
                static_cast<ELEMENTLIMITED>( limitedFrameGroup->checkedId() ),
                UnifiedInputData::getInInstance()->namesToIDs(limitedFrames, DATATYPE::TPFRAMEP),
                scaleColorCombobox->color(),
                frscaleCheck->isChecked(),
                UnifiedInputData::getInInstance()->nameToID(str_fl, DATATYPE::TPFLOOR),
                flscaleCheck->isChecked(),
                pp,
                jointSizeCombobox->currentIndex(),
                jointColorCombobox->color(),
                memberSizeCombobox->currentIndex(),
                memberColorCombobox->color(),
                stateHash,
                elementHash,
                colorHash,
                true);
    */
    Input3DViewSettings settings = makeSettings();
    bool isChanged = false;
    Q_FOREACH ( ELEMENTTYPE type, elementWidgets.keys() ) {
        ELEMENTVIEW ev = elementWidgets.value(type)->getElementSetting();
        elementHash.insert( type, ev );
        if ( !isChanged ) {
            isChanged = ( ev.isVisible
                          != UnifiedSettings::getInstance()->getInput3DViewSettings().isVisibleElement(type) );
        }
    }

    if ( !isChanged ) {
        isChanged = ( settings.getFloorLimitedState()
                      != UnifiedSettings::getInstance()->getInput3DViewSettings().getFloorLimitedState() );
    }
    if ( !isChanged ) {
        isChanged = ( settings.getFrameLimitedState()
                      != UnifiedSettings::getInstance()->getInput3DViewSettings().getFrameLimitedState() );
    }
    if ( !isChanged ) {
        isChanged = ( settings.getLimitedFloors().count()
                      != UnifiedSettings::getInstance()->getInput3DViewSettings().getLimitedFloors().count() );
    }
    if ( !isChanged ) {
        QList<QUuid> floors = UnifiedSettings::getInstance()->getInput3DViewSettings().getLimitedFloors();
        Q_FOREACH ( QUuid id, settings.getLimitedFloors() ) {
            if ( !floors.contains(id) ) {
                isChanged = true;
                break;
            }
        }
    }
    if ( !isChanged ) {
        isChanged = ( settings.getLimitedFrames().count()
                      != UnifiedSettings::getInstance()->getInput3DViewSettings().getLimitedFrames().count() );
    }
    if ( !isChanged ) {
        QList<QUuid> frames = UnifiedSettings::getInstance()->getInput3DViewSettings().getLimitedFrames();
        Q_FOREACH ( QUuid id, settings.getLimitedFrames() ) {
            if ( !frames.contains(id) ) {
                isChanged = true;
                break;
            }
        }
    }

    UnifiedSettings::getInstance()->uploadViewSettings(settings);
    if ( isChanged ) UnifiedSettings::getInstance()->signalViewLimitedChanged();

//    UnifiedSettings::getInstance()->signalViewSettingChanged(INPUT3DMODEL);
}

void Input3DSettingDialog::createAllWidgets()
{
    // 優先
    priorityCheckbox = new QCheckBox(u8"符号別の表示設定を優先.", this);
    QHBoxLayout *hlay_priority = new QHBoxLayout;
    hlay_priority->setContentsMargins(0, 0, 0, 0);
    hlay_priority->addWidget(priorityCheckbox);
    hlay_priority->addStretch();

    // 階・通りの指定
    QLabel *floorLabel = new QLabel(u8"・階所属の制限", this);
    floorLabel->setFixedWidth(110);
    QRadioButton *fl_noneButton = new QRadioButton(u8"制限なし", this);
    QRadioButton *fl_containButton = new QRadioButton(u8"全体所属", this);
    QRadioButton *fl_touchButton = new QRadioButton(u8"部分所属", this);
    limitedFloorGroup = new QButtonGroup;
    limitedFloorGroup->addButton(fl_noneButton, 0);
    limitedFloorGroup->addButton(fl_containButton, 1);
    limitedFloorGroup->addButton(fl_touchButton, 2);

    QPushButton *selectedFloorButton = new QPushButton(u8"所属階", this);
    selectedFloorButton->setFixedSize(60, 25);
    limitedFloorLabel = new ElidedLabel("", this);
    limitedFloorLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    limitedFloorLabel->setTextFormat(Qt::AutoText);

    connect(selectedFloorButton, &QAbstractButton::clicked, this, &Input3DSettingDialog::showSelectFloorDialog);

    QLabel *frameLabel = new QLabel(u8"・通り所属の制限", this);
    frameLabel->setFixedWidth(110);
    QRadioButton *fr_noneButton = new QRadioButton(u8"制限なし", this);
    QRadioButton *fr_containButton = new QRadioButton(u8"全体所属", this);
    QRadioButton *fr_touchButton = new QRadioButton(u8"部分所属", this);
    limitedFrameGroup = new QButtonGroup;
    limitedFrameGroup->addButton(fr_noneButton, 0);
    limitedFrameGroup->addButton(fr_containButton, 1);
    limitedFrameGroup->addButton(fr_touchButton, 2);

    QPushButton *selectedFrameButton = new QPushButton(u8"所属通り", this);
    selectedFrameButton->setFixedSize(60, 25);
    limitedFrameLabel = new ElidedLabel("", this);
    limitedFrameLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    limitedFrameLabel->setTextFormat(Qt::AutoText);

    connect(selectedFrameButton, &QAbstractButton::clicked, this, &Input3DSettingDialog::showSelectFrameDialog);

    QHBoxLayout *hlay_floor = new QHBoxLayout;
    hlay_floor->setContentsMargins(0, 0, 0, 0);
    hlay_floor->addWidget(floorLabel);
    hlay_floor->addSpacing(10);
    hlay_floor->addWidget(fl_noneButton);
    hlay_floor->addSpacing(10);
    hlay_floor->addWidget(fl_containButton);
    hlay_floor->addSpacing(10);
    hlay_floor->addWidget(fl_touchButton);
    hlay_floor->addStretch();
    QHBoxLayout *hlay_sfloor = new QHBoxLayout;
    hlay_sfloor->setContentsMargins(0, 0, 0, 0);
    hlay_sfloor->addSpacing(130);
    hlay_sfloor->addWidget(selectedFloorButton);
    hlay_sfloor->addWidget(limitedFloorLabel);

    QHBoxLayout *hlay_frame = new QHBoxLayout;
    hlay_frame->setContentsMargins(0, 0, 0, 0);
    hlay_frame->addWidget(frameLabel);
    hlay_frame->addSpacing(10);
    hlay_frame->addWidget(fr_noneButton);
    hlay_frame->addSpacing(10);
    hlay_frame->addWidget(fr_containButton);
    hlay_frame->addSpacing(10);
    hlay_frame->addWidget(fr_touchButton);
    hlay_frame->addStretch();
    QHBoxLayout *hlay_sframe = new QHBoxLayout;
    hlay_sframe->setContentsMargins(0, 0, 0, 0);
    hlay_sframe->addSpacing(130);
    hlay_sframe->addWidget(selectedFrameButton);
    hlay_sframe->addWidget(limitedFrameLabel);

    // 階・通りスケール
    scaleColorCombobox = new TypicalColorComboBox(this);

    frscaleCheck = new QCheckBox(u8"通りスケールの表示", this);
    frscaleCheck->setFixedWidth(150);
    frscaleFloorCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::Input, this);
    frscaleFloorCombobox->setDefaultName(u8"（表示しない）");

    connect(frscaleCheck, &QAbstractButton::toggled, frscaleFloorCombobox, &QWidget::setEnabled );

    flscaleCheck = new QCheckBox(u8"階スケールの表示", this);
    flscaleCheck->setFixedWidth(150);
    flscaleXLine = new QLineEdit(this);
    flscaleXLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    flscaleXLine->setAlignment(Qt::AlignRight);
    flscaleXLine->setFixedWidth(70);
    flscaleYLine = new QLineEdit(this);
    flscaleYLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    flscaleYLine->setAlignment(Qt::AlignRight);
    flscaleYLine->setFixedWidth(70);

    connect(flscaleCheck, &QAbstractButton::toggled, flscaleXLine, &QWidget::setEnabled );
    connect(flscaleCheck, &QAbstractButton::toggled, flscaleYLine, &QWidget::setEnabled );

    QHBoxLayout *hlay_scale = new QHBoxLayout;
    hlay_scale->setContentsMargins(0, 0, 0, 0);
    hlay_scale->addWidget(new QLabel(u8"・スケールの表示", this));
    hlay_scale->addSpacing(40);
    hlay_scale->addWidget(new QLabel(u8"線色：", this));
    hlay_scale->addWidget(scaleColorCombobox);
    hlay_scale->addStretch();
    QHBoxLayout *hlay_frscale = new QHBoxLayout;
    hlay_frscale->setContentsMargins(0, 0, 0, 0);
    hlay_frscale->addSpacing(15);
    hlay_frscale->addWidget(frscaleCheck);
    hlay_frscale->addWidget(new QLabel(u8"表示階：", this));
    hlay_frscale->addWidget(frscaleFloorCombobox);
    hlay_frscale->addStretch();
    QHBoxLayout *hlay_flscale = new QHBoxLayout;
    hlay_flscale->setContentsMargins(0, 0, 0, 0);
    hlay_flscale->addSpacing(15);
    hlay_flscale->addWidget(flscaleCheck);
    hlay_flscale->addWidget(new QLabel(u8"X座標(m)：", this));
    hlay_flscale->addWidget(flscaleXLine);
    hlay_flscale->addSpacing(20);
    hlay_flscale->addWidget(new QLabel(u8"Y座標(m)：", this));
    hlay_flscale->addWidget(flscaleYLine);
    hlay_flscale->addStretch();

    // 文字サイズ
    jointSizeCombobox = new FiveSizeComboBox(this);
    jointColorCombobox = new TypicalColorComboBox(this);

    QHBoxLayout *hlay_joint = new QHBoxLayout;
    hlay_joint->setContentsMargins(0, 0, 0, 0);
    hlay_joint->addWidget(new QLabel(u8"・節点番号", this));
    hlay_joint->addSpacing(30);
    hlay_joint->addWidget(new QLabel(u8"文字サイズ：", this));
    hlay_joint->addWidget(jointSizeCombobox);
    hlay_joint->addSpacing(30);
    hlay_joint->addWidget(new QLabel(u8"文字色：", this));
    hlay_joint->addWidget(jointColorCombobox);
    hlay_joint->addStretch();

    memberSizeCombobox = new FiveSizeComboBox(this);
    memberColorCombobox = new TypicalColorComboBox(this);

    QHBoxLayout *hlay_member = new QHBoxLayout;
    hlay_member->setContentsMargins(0, 0, 0, 0);
    hlay_member->addWidget(new QLabel(u8"・部材符号", this));
    hlay_member->addSpacing(30);
    hlay_member->addWidget(new QLabel(u8"文字サイズ：", this));
    hlay_member->addWidget(memberSizeCombobox);
    hlay_member->addSpacing(30);
    hlay_member->addWidget(new QLabel(u8"文字色：", this));
    hlay_member->addWidget(memberColorCombobox);
    hlay_member->addStretch();

    ElementStateWidgetGroup *gg_dummy = new ElementStateWidgetGroup(u8"・ダミー要素の表示", this);
    ElementStateWidgetGroup *gg_error = new ElementStateWidgetGroup(u8"・エラー要素の表示", this);
    ElementStateWidgetGroup *gg_select = new ElementStateWidgetGroup(u8"・選択要素の表示", this);

    elementStateWidgets.insert(ELDUMMY, gg_dummy);
    elementStateWidgets.insert(ELERROR, gg_error);
    elementStateWidgets.insert(ELSELECT, gg_select);

    ElementWidgetGroup *eg_joint = new ElementWidgetGroup(u8"節点の表示", this);
    ElementWidgetGroup *eg_column = new ElementWidgetGroup(u8"柱の表示", this);
    ElementWidgetGroup *eg_girder = new ElementWidgetGroup(u8"大梁の表示", this);
    ElementWidgetGroup *eg_brace = new ElementWidgetGroup(u8"ブレースの表示", this);
    ElementWidgetGroup *eg_beam = new ElementWidgetGroup(u8"小梁の表示", this);
    ElementWidgetGroup *eg_slab = new ElementWidgetGroup(u8"スラブの表示", this);
    ElementWidgetGroup *eg_wall = new ElementWidgetGroup(u8"壁の表示", this);
    ElementWidgetGroup *eg_uwall = new ElementWidgetGroup(u8"地下外壁の表示", this);
    ElementWidgetGroup *eg_damper = new ElementWidgetGroup(u8"制振要素の表示", this);
    ElementWidgetGroup *eg_iso = new ElementWidgetGroup(u8"免震要素の表示", this);
    elementWidgets.insert(ELJOINT, eg_joint);
    elementWidgets.insert(ELCOLUMN, eg_column);
    elementWidgets.insert(ELGIRDER, eg_girder);
    elementWidgets.insert(ELBRACE, eg_brace);
    elementWidgets.insert(ELBEAM, eg_beam);
    elementWidgets.insert(ELSLAB, eg_slab);
    elementWidgets.insert(ELWALL, eg_wall);
    elementWidgets.insert(ELUWALL, eg_uwall);
    elementWidgets.insert(ELDAMPER, eg_damper);
    elementWidgets.insert(ELISO, eg_iso);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 10, 5, 10);
    vlay->setSpacing(3);
    vlay->addLayout(hlay_priority);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_floor);
    vlay->addLayout(hlay_sfloor);
    vlay->addLayout(hlay_frame);
    vlay->addLayout(hlay_sframe);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_scale);
    vlay->addLayout(hlay_frscale);
    vlay->addLayout(hlay_flscale);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_joint);
    vlay->addLayout(hlay_member);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addWidget(gg_dummy);
    vlay->addWidget(gg_error);
    vlay->addWidget(gg_select);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addWidget(eg_joint);
    vlay->addWidget(eg_column);
    vlay->addWidget(eg_girder);
    vlay->addWidget(eg_brace);
    vlay->addWidget(eg_beam);
    vlay->addWidget(eg_slab);
    vlay->addWidget(eg_wall);
    vlay->addWidget(eg_uwall);
    vlay->addWidget(eg_damper);
    vlay->addWidget(eg_iso);
    vlay->addStretch();

    persAllWidget = new QWidget(this);
    persAllWidget->setLayout(vlay);
}

void Input3DSettingDialog::createIndividualWidgets()
{
    IndividualSettingType a_type;
    persIndividualWidget = new IndividualSettingTableWidget(a_type, this);
}

Input3DViewSettings Input3DSettingDialog::makeSettings() const
{
    QHash<ELEMENTSTATE, COLORBRUSHSETTING> state;
    QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT};
    foreach(auto s, stateList){
        state.insert(s, elementStateWidgets.value(s)->getElementSetting());
    }

    QHash<ELEMENTTYPE, ELEMENTVIEW> type;
    QList<ELEMENTTYPE> elementList{
        ELJOINT,
        ELCOLUMN,
        ELGIRDER,
        ELBRACE,
        ELBEAM,
        ELSLAB,
        ELWALL,
        ELUWALL,
        ELDAMPER,
        ELISO
    };
    foreach(auto e, elementList){
        type.insert(e, elementWidgets.value(e)->getElementSetting());
    }

    QMap<QString, COLORBRUSHSETTING> individual;
    foreach(auto n, persIndividualWidget->getIndividualData()){
        QString name = n.at(1).toString();
        QColor line = TypicalColorTable::nameToColor(n.at(2).toString());
        QColor brush = TypicalColorTable::nameToColor(n.at(3).toString());
        brush.setAlpha(n.at(4).toInt());
        individual.insert(name, COLORBRUSHSETTING(line, brush));
    }

    QList<QUuid> floors;
    foreach(auto fl, limitedFloors){
        floors.append(UnifiedInputData::getInInstance()->nameToID(fl, DATATYPE::TPFLOOR));
    }

    QList<QUuid> frames;
    foreach(auto fr, limitedFrames){
        frames.append(UnifiedInputData::getInInstance()->nameToID(fr, DATATYPE::TPFRAMEP));
    }

    auto setting = Input3DViewSettings(
                priorityCheckbox->isChecked(),
                static_cast<ELEMENTLIMITED>(limitedFloorGroup->checkedId()),
                floors,
                static_cast<ELEMENTLIMITED>(limitedFrameGroup->checkedId()),
                frames,
                scaleColorCombobox->color(),
                frscaleCheck->isChecked(),
                UnifiedInputData::getInInstance()->nameToID(frscaleFloorCombobox->currentText(), DATATYPE::TPFRAMEP),
                flscaleCheck->isChecked(),
                QPointF(flscaleXLine->text().toDouble(), flscaleYLine->text().toDouble()),
                jointSizeCombobox->currentIndex(),
                jointColorCombobox->color(),
                memberSizeCombobox->currentIndex(),
                memberColorCombobox->color(),
                state,
                type,
                individual,
                true
                );

    return setting;
}

void Input3DSettingDialog::applySettings(const Input3DViewSettings &setting)
{
    priorityCheckbox->setChecked(setting.getIsPriorIndividual());
    limitedFloorGroup->button(static_cast<int>(setting.getFloorLimitedState()))->setChecked(true);
    QStringList floors;
    foreach(auto fl, setting.getLimitedFloors()){
        QString name = UnifiedInputData::getInInstance()->idToName(fl, DATATYPE::TPFLOOR);
        if (name != QString{})
            floors.append(name);
    }
    limitedFloors = floors;
    limitedFloorLabel->setText(limitedFloors.join(','));
    limitedFrameGroup->button(static_cast<int>(setting.getFrameLimitedState()))->setChecked(true);
    QStringList frames;
    foreach(auto fr, setting.getLimitedFrames()){
        QString name = UnifiedInputData::getInInstance()->idToName(fr, DATATYPE::TPFRAMEP);
        if (name != QString{})
            frames.append(name);
    }
    limitedFrames = frames;
    limitedFrameLabel->setText(limitedFrames.join(','));
    scaleColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getScaleColor()));
    frscaleCheck->setChecked(setting.getFrameScaleIsChecked());
    QString floorName = UnifiedInputData::getInInstance()->idToName(setting.getFrameScaleFloorID(), DATATYPE::TPFLOOR);
    int index = (frscaleFloorCombobox->findText(floorName) < 0) ? 0 : frscaleFloorCombobox->findText(floorName);
    frscaleFloorCombobox->setCurrentIndex(index);
    flscaleCheck->setChecked(setting.getFloorScaleIsChecked());
    flscaleXLine->setText(QString::number(setting.getFloorScaleXY().x()));
    flscaleYLine->setText(QString::number(setting.getFloorScaleXY().y()));
    jointSizeCombobox->setCurrentIndex(setting.getJointNameSize());
    jointColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getJointNameColor()));
    memberSizeCombobox->setCurrentIndex(setting.getMemberNameSize());
    memberColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getMemberNameColor()));

    const QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT};
    foreach(auto s, stateList){
        elementStateWidgets.value(s)->setElementSetting(setting.getElementStateView(s));
    }

    const QList<ELEMENTTYPE> elementList{ELJOINT, ELCOLUMN, ELGIRDER, ELBRACE, ELBEAM, ELSLAB, ELWALL, ELUWALL, ELDAMPER, ELISO};
    foreach(auto e, elementList){
        elementWidgets.value(e)->setElementSetting(setting.getElementView(e));
    }

    QList<QList<QVariant>> idvList;
    auto keys = setting.getNameToViewSettingsKeys();
    for (int i = 0; i < keys.count(); i++){
        QString no = QString::number(i + 1);
        QString label = keys.at(i);
        COLORBRUSHSETTING brush = setting.getIndividualSetting(keys.at(i));
        QString lineColor = TypicalColorTable::colorToName(brush.lineColor.rgb());
        QString fillColor = TypicalColorTable::colorToName(brush.paintBrush.color().rgb());
        int alpha = brush.paintBrush.color().alpha();
        QList<QVariant> idList{no, label, lineColor, fillColor, alpha};
        idvList.append(idList);
    }
    persIndividualWidget->setIndividualData(idvList);
}

void Input3DSettingDialog::showSelectFloorDialog()
{
    QStringList fllist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFLOOR);
    NameSelectDialog dialog(fllist, limitedFloors, this);
    dialog.setWindowTitle(u8"階の選択");
    if ( dialog.exec() ) {
        limitedFloors = dialog.selectedNames();
        QString str = ( limitedFloors.isEmpty() ) ? QString("") : limitedFloors.join(",") ;
        limitedFloorLabel->setText(str);
    }
}

void Input3DSettingDialog::showSelectFrameDialog()
{
    QStringList frlist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFRAMEP);
    NameSelectDialog dialog(frlist, limitedFrames, this);
    dialog.setWindowTitle(u8"通りの選択");
    if ( dialog.exec() ) {
        limitedFrames = dialog.selectedNames();
        QString str = ( limitedFrames.isEmpty() ) ? QString("") : limitedFrames.join(",") ;
        limitedFrameLabel->setText(str);
    }
}

void Input3DSettingDialog::updateSelectedFloorFrame(DATATYPE dtype)
{
    if ( dtype == DATATYPE::TPFLOOR ) {
        QStringList fllist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFLOOR);
        for ( int i = limitedFloors.count() - 1; i >= 0; i-- ) {
            if ( !fllist.contains( limitedFloors.at(i) ) ) limitedFloors.removeAt(i);
        }
        QString str = limitedFloors.join(",") ;
        limitedFloorLabel->setText(str);
    } else if ( dtype == DATATYPE::TPFRAMEP ) {
        QStringList frlist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFRAMEP);
        for ( int i = limitedFrames.count() - 1; i >= 0; i-- ) {
            if ( !frlist.contains( limitedFrames.at(i) ) ) limitedFrames.removeAt(i);
        }
        QString str = limitedFrames.join(",") ;
        limitedFrameLabel->setText(str);
    }
}

void Input3DSettingDialog::slotFileSave()
{
    QString filepath =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    saveSettings(filepath);
}

void Input3DSettingDialog::saveSettings(const QString &filepath) const
{
    auto setting = makeSettings();
    setting.saveSettings(filepath);
    /*
    QSettings settings(filepath, QSettings::IniFormat);

    settings.setValue("i3d_priority", priorityCheckbox->isChecked() );
    settings.setValue("i3d_scaleColor",   scaleColorCombobox->currentIndex());
    settings.setValue("i3d_frscaleCheck", frscaleCheck->isChecked());
    settings.setValue("i3d_flscaleCheck", flscaleCheck->isChecked());
    settings.setValue("i3d_flscaleX", flscaleXLine->text());
    settings.setValue("i3d_flscaleY", flscaleYLine->text());
    settings.setValue("i3d_jointSize",   jointSizeCombobox->currentIndex());
    settings.setValue("i3d_jointColor",  jointColorCombobox->currentIndex());
    settings.setValue("i3d_memberSize",  memberSizeCombobox->currentIndex());
    settings.setValue("i3d_memberColor", memberColorCombobox->currentIndex());

    const QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT};
    settings.beginWriteArray("i3d_states");
    for (int i = 0; i < stateList.count(); ++i) {
        ELEMENTSTATE state = stateList.at(i);
        settings.setArrayIndex(i);
                settings.setValue("line",  elementStateWidgets.value(state)->lineComboIndex());
                settings.setValue("brush", elementStateWidgets.value(state)->brushComboIndex());
                settings.setValue("alpha", elementStateWidgets.value(state)->alphaValue());
        COLORBRUSHSETTING colb = elementStateWidgets.value(state)->getElementSetting();
        settings.setValue("line", colb.lineColor);
        settings.setValue("brush", colb.paintBrush);
    }
    settings.endArray();

    const QList<ELEMENTTYPE> elementList{ELJOINT, ELCOLUMN, ELGIRDER, ELBRACE, ELBEAM, ELSLAB, ELWALL, ELUWALL, ELDAMPER, ELISO};
    settings.beginWriteArray("i3d_elements");
    for (int i = 0; i < elementList.count(); ++i) {
        ELEMENTTYPE el = elementList.at(i);
        settings.setArrayIndex(i);
                settings.setValue("check",elementWidgets.value(el)->isElementChecked());
                settings.setValue("nameCheck",elementWidgets.value(el)->isNameChecked());
                settings.setValue("line",elementWidgets.value(el)->lineComboIndex());
                settings.setValue("brush", elementWidgets.value(el)->brushComboIndex());
                settings.setValue("alpha", elementWidgets.value(el)->alphaValue());
        ELEMENTVIEW ev = elementWidgets.value(el)->getElementSetting();
        settings.setValue("check", ev.isVisible);
        settings.setValue("nameCheck", ev.isNameVisible);
        settings.setValue("line", ev.lineColor);
        settings.setValue("brush", ev.paintBrush);
    }
    settings.endArray();

    QList<QList<QVariant> > idvList = persIndividualWidget->getIndividualData();
    settings.beginWriteArray("i3d_individual");
    for (int i = 0; i < idvList.count(); ++i) {
        QList<QVariant> list = idvList.at(i);
        settings.setArrayIndex(i);
        settings.setValue("No.", list.at(0));
        settings.setValue("name", list.at(1));
        settings.setValue("line", list.at(2));
        settings.setValue("brush", list.at(3));
        settings.setValue("alpha", list.at(4));
    }
    settings.endArray();
    */

}

void Input3DSettingDialog::slotFileOpen()
{
    QString filepath =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    readSettings(filepath);
}

void Input3DSettingDialog::readSettings(const QString &filepath)
{
    auto setting = Input3DViewSettings::readSettings(filepath);
    applySettings(setting);
    /*
    QSettings settings(filepath, QSettings::IniFormat);

    priorityCheckbox->setChecked(settings.value("i3d_priority").toBool());
    limitedFloorGroup->button(0)->setChecked(true);
    limitedFrameGroup->button(0)->setChecked(true);
    frscaleFloorCombobox->setCurrentIndex(0);
    limitedFloors.clear();
    limitedFrames.clear();
    limitedFloorLabel->setText(QString());
    limitedFrameLabel->setText(QString());

    scaleColorCombobox->setCurrentIndex(settings.value("i3d_scaleColor").toInt());
    frscaleCheck->setChecked(settings.value("i3d_frscaleCheck").toBool());
    flscaleCheck->setChecked(settings.value("i3d_flscaleCheck").toBool());
    flscaleXLine->setText(settings.value("i3d_flscaleX").toString());
    flscaleYLine->setText(settings.value("i3d_flscaleY").toString());

    jointSizeCombobox->setCurrentIndex(settings.value("i3d_jointSize").toInt());
    jointColorCombobox->setCurrentIndex(settings.value("i3d_jointColor").toInt());
    memberSizeCombobox->setCurrentIndex(settings.value("i3d_memberSize").toInt());
    memberColorCombobox->setCurrentIndex(settings.value("i3d_memberColor").toInt());

    const QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT};
    settings.beginReadArray("i3d_states");
    for (int i = 0; i < stateList.count(); ++i) {
        ELEMENTSTATE state = stateList.at(i);
        settings.setArrayIndex(i);
                elementStateWidgets.value(state)->setLineComboIndex(settings.value("line").toInt());
                elementStateWidgets.value(state)->setBrushComboIndex(settings.value("brush").toInt());
                elementStateWidgets.value(state)->setAlphaValue(settings.value("alpha").toInt());
        COLORBRUSHSETTING colb(settings.value("line").value<QColor>(),
                               settings.value("brush").value<QBrush>());
        elementStateWidgets.value(state)->setElementSetting(colb);
    }
    settings.endArray();

    const QList<ELEMENTTYPE> elementList{ELJOINT, ELCOLUMN, ELGIRDER, ELBRACE, ELBEAM, ELSLAB, ELWALL, ELUWALL, ELDAMPER, ELISO};
    settings.beginWriteArray("i3d_elements");
    for (int i = 0; i < elementList.count(); ++i) {
        ELEMENTTYPE el = elementList.at(i);
        settings.setArrayIndex(i);
                elementWidgets.value(el)->setElementCheck(settings.value("check").toBool());
                elementWidgets.value(el)->setNameCheck(settings.value("nameCheck").toBool());
                elementWidgets.value(el)->setLineComboIndex(settings.value("line").toInt());
                elementWidgets.value(el)->setBrushComboIndex(settings.value("brush").toInt());
                elementWidgets.value(el)->setAlphaValue(settings.value("alpha").toInt());
        ELEMENTVIEW ev(settings.value("check").toBool(), settings.value("nameCheck").toBool(),
                       settings.value("line").value<QColor>(),
                       settings.value("brush").value<QBrush>());
        elementWidgets.value(el)->setElementSetting(ev);

    }
    settings.endArray();

    QList<QList<QVariant> > idvList;
    int size = settings.beginReadArray("i3d_individual");
    for (int i = 0; i < size; ++i) {
        QList<QVariant> list;
        settings.setArrayIndex(i);
        list.append(settings.value("No."));
        list.append(settings.value("name"));
        list.append(settings.value("line"));
        list.append(settings.value("brush"));
        list.append(settings.value("alpha"));
        idvList.append(list);
    }
    settings.endArray();
    persIndividualWidget->setIndividualData(idvList);
    */

}
} // namespace post3dapp

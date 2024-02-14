#include "calculationload3dsetting_dialog.h"

#include "define_calculationload3dsettings.h"
#include "reference_combobox.h"
#include "typicalcolor_combobox.h"
#include "fixed_data.h"
#include "unified_inputdata.h"
#include "unified_settings.h"
#include "vhline.h"
#include "viewsetting_partswidget.h"


namespace post3dapp{

CalculationLoad3DSettingDialog::CalculationLoad3DSettingDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setFont(QFont(tr("Microsoft JhengHei"), 9));


    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(pal);

    QPushButton *fileOpenButton = new QPushButton(u8"ファイル読込", this);
    QPushButton *fileSaveButton = new QPushButton(u8"ファイル保存", this);
    QPushButton *defaultButton = new QPushButton(u8"デフォルト値", this);
    QPushButton *uploadButton = new QPushButton(u8"画面更新", this);
    fileOpenButton->setFixedWidth(90);
    fileSaveButton->setFixedWidth(90);
    defaultButton->setFixedWidth(90);
    uploadButton->setFixedWidth(90);

    connect( fileSaveButton, &QPushButton::clicked, this, &CalculationLoad3DSettingDialog::slotFileSave);
    connect( fileOpenButton, &QPushButton::clicked, this, &CalculationLoad3DSettingDialog::slotFileOpen);
    connect( defaultButton,  &QPushButton::clicked, this, &CalculationLoad3DSettingDialog::initializeSettings);
    connect( uploadButton,   &QPushButton::clicked, this, &CalculationLoad3DSettingDialog::uploadViewSettings);

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
    createWidget(vlay);
    vlay->addStretch();
    this->setLayout(vlay);

    this->setWindowTitle(u8"荷重拾い結果（３Ｄパース）　表示設定");
    this->setWindowIcon(QIcon(QPixmap(":/icons/color_settings.png")));
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(800, 650));


    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    ( QFile(filepath).exists() ) ? readSettings(filepath) : initializeSettings() ;

    uploadViewSettings();
}

CalculationLoad3DSettingDialog::~CalculationLoad3DSettingDialog()
{
    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    saveSettings(filepath);
}

void CalculationLoad3DSettingDialog::createWidget(QVBoxLayout* vlay)
{
    // 解析節点
    QLabel *jointLabel = new QLabel(u8"・解析節点", this);
    jointLabel->setFixedWidth(150);
    jointCheck = new QCheckBox(u8"表示する", this);
    jointCheck->setFixedWidth(80);
    QLabel *jointColorLabel = new QLabel(u8"色：", this);
    jointColorCombobox = new TypicalColorComboBox(this);
    QHBoxLayout *hlay_joint = new QHBoxLayout;
    hlay_joint->setContentsMargins(0, 0, 0, 0);
    hlay_joint->addWidget(jointLabel);
    hlay_joint->addWidget(jointCheck);
    hlay_joint->addSpacing(30);
    hlay_joint->addWidget(jointColorLabel);
    hlay_joint->addWidget(jointColorCombobox);
    hlay_joint->addStretch();

    connect( jointCheck, &QAbstractButton::toggled, jointColorLabel,    &QWidget::setEnabled );
    connect( jointCheck, &QAbstractButton::toggled, jointColorCombobox, &QWidget::setEnabled );
    jointCheck->setChecked(true);

    // 解析部材（線材）
    QLabel *lineLabel = new QLabel(u8"・解析部材（線材）", this);
    lineLabel->setFixedWidth(150);
    lineCheck = new QCheckBox(u8"表示する", this);
    lineCheck->setFixedWidth(80);
    QLabel *lineColorLabel = new QLabel(u8"色：", this);
    lineColorCombobox = new TypicalColorComboBox(this);
    QHBoxLayout *hlay_line = new QHBoxLayout;
    hlay_line->setContentsMargins(0, 0, 0, 0);
    hlay_line->addWidget(lineLabel);
    hlay_line->addWidget(lineCheck);
    hlay_line->addSpacing(30);
    hlay_line->addWidget(lineColorLabel);
    hlay_line->addWidget(lineColorCombobox);
    hlay_line->addStretch();

    connect( lineCheck, &QAbstractButton::toggled, lineColorLabel,    &QWidget::setEnabled );
    connect( lineCheck, &QAbstractButton::toggled, lineColorCombobox, &QWidget::setEnabled );
    lineCheck->setChecked(true);

    // 解析部材（面材）
    QLabel *planeLabel = new QLabel(u8"・解析部材（面材）", this);
    planeLabel->setFixedWidth(150);
    planeCheck = new QCheckBox(u8"表示する", this);
    planeCheck->setFixedWidth(80);
    QLabel *planeColorLabel = new QLabel(u8"色：", this);
    planeColorCombobox = new TypicalColorComboBox(this);
    QHBoxLayout *hlay_plane = new QHBoxLayout;
    hlay_plane->setContentsMargins(0, 0, 0, 0);
    hlay_plane->addWidget(planeLabel);
    hlay_plane->addWidget(planeCheck);
    hlay_plane->addSpacing(30);
    hlay_plane->addWidget(planeColorLabel);
    hlay_plane->addWidget(planeColorCombobox);
    hlay_plane->addStretch();

    connect( planeCheck, &QAbstractButton::toggled, planeColorLabel,    &QWidget::setEnabled );
    connect( planeCheck, &QAbstractButton::toggled, planeColorCombobox, &QWidget::setEnabled );
    planeCheck->setChecked(true);

    // 節点番号
    QLabel *jnoLabel = new QLabel(u8"・節点番号", this);
    jnoLabel->setFixedWidth(150);
    jnoCheck = new QCheckBox(u8"表示する", this);
    jnoCheck->setFixedWidth(80);
    QLabel *jnoColorLabel = new QLabel(u8"色：", this);
    jnoColorCombobox = new TypicalColorComboBox(this);
    QLabel *jnoSizeLabel = new QLabel(u8"サイズ：", this);
    jnoSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *hlay_jno = new QHBoxLayout;
    hlay_jno->setContentsMargins(0, 0, 0, 0);
    hlay_jno->addWidget(jnoLabel);
    hlay_jno->addWidget(jnoCheck);
    hlay_jno->addSpacing(30);
    hlay_jno->addWidget(jnoColorLabel);
    hlay_jno->addWidget(jnoColorCombobox);
    hlay_jno->addSpacing(30);
    hlay_jno->addWidget(jnoSizeLabel);
    hlay_jno->addWidget(jnoSizeCombobox);
    hlay_jno->addStretch();

    connect( jnoCheck, &QAbstractButton::toggled, jnoColorLabel,    &QWidget::setEnabled );
    connect( jnoCheck, &QAbstractButton::toggled, jnoColorCombobox, &QWidget::setEnabled );
    connect( jnoCheck, &QAbstractButton::toggled, jnoSizeLabel,     &QWidget::setEnabled );
    connect( jnoCheck, &QAbstractButton::toggled, jnoSizeCombobox,  &QWidget::setEnabled );
    jnoCheck->setChecked(true);

    // 部材符号
    QLabel *nameLabel = new QLabel(u8"・部材符号", this);
    nameLabel->setFixedWidth(150);
    nameCheck = new QCheckBox(u8"表示する", this);
    nameCheck->setFixedWidth(80);
    QLabel *nameColorLabel = new QLabel(u8"色：", this);
    nameColorCombobox = new TypicalColorComboBox(this);
    QLabel *nameSizeLabel = new QLabel(u8"サイズ：", this);
    nameSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *hlay_name = new QHBoxLayout;
    hlay_name->setContentsMargins(0, 0, 0, 0);
    hlay_name->addWidget(nameLabel);
    hlay_name->addWidget(nameCheck);
    hlay_name->addSpacing(30);
    hlay_name->addWidget(nameColorLabel);
    hlay_name->addWidget(nameColorCombobox);
    hlay_name->addSpacing(30);
    hlay_name->addWidget(nameSizeLabel);
    hlay_name->addWidget(nameSizeCombobox);
    hlay_name->addStretch();

    connect( nameCheck, &QAbstractButton::toggled, nameColorLabel,    &QWidget::setEnabled );
    connect( nameCheck, &QAbstractButton::toggled, nameColorCombobox, &QWidget::setEnabled );
    connect( nameCheck, &QAbstractButton::toggled, nameSizeLabel,     &QWidget::setEnabled );
    connect( nameCheck, &QAbstractButton::toggled, nameSizeCombobox,  &QWidget::setEnabled );
    nameCheck->setChecked(true);

    // カラーバー
    QLabel *colorBarLabel = new QLabel(u8"・カラーバー", this);
    colorBarLabel->setFixedWidth(150);
    colorBarCheck = new QCheckBox(u8"表示する", this);
    colorBarCheck->setFixedWidth(80);
    QLabel *colorBarColorLabel = new QLabel(u8"色：", this);
    colorBarColorCombobox = new TypicalColorComboBox(this);
    QLabel *colorBarSizeLabel = new QLabel(u8"サイズ：", this);
    colorBarSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *hlay_bar = new QHBoxLayout;
    hlay_bar->setContentsMargins(0, 0, 0, 0);
    hlay_bar->addWidget(colorBarLabel);
    hlay_bar->addWidget(colorBarCheck);
    hlay_bar->addSpacing(30);
    hlay_bar->addWidget(colorBarColorLabel);
    hlay_bar->addWidget(colorBarColorCombobox);
    hlay_bar->addSpacing(30);
    hlay_bar->addWidget(colorBarSizeLabel);
    hlay_bar->addWidget(colorBarSizeCombobox);
    hlay_bar->addStretch();

    connect( colorBarCheck, &QAbstractButton::toggled, colorBarColorLabel,    &QWidget::setEnabled );
    connect( colorBarCheck, &QAbstractButton::toggled, colorBarColorCombobox, &QWidget::setEnabled );
    connect( colorBarCheck, &QAbstractButton::toggled, colorBarSizeLabel,     &QWidget::setEnabled );
    connect( colorBarCheck, &QAbstractButton::toggled, colorBarSizeCombobox,  &QWidget::setEnabled );
    colorBarCheck->setChecked(true);

    // 表示値の透過度
    QLabel *valueAlphaLabel = new QLabel(u8"・表示値の透過度", this);
    valueAlphaLabel->setFixedWidth(150);
    valueAlphaSpinbox = new QSpinBox(this);
    valueAlphaSpinbox->setRange(0, 255);
    QHBoxLayout *hlay_alpha = new QHBoxLayout;
    hlay_alpha->setContentsMargins(0, 0, 0, 0);
    hlay_alpha->addWidget(valueAlphaLabel);
    hlay_alpha->addWidget(valueAlphaSpinbox);
    hlay_alpha->addStretch();

    //　選択時の色
    QLabel *selectColorLabel = new QLabel(u8"・選択時の色", this);
    selectColorLabel->setFixedWidth(150);
    selectColorCombobox = new TypicalColorComboBox(this);
    QHBoxLayout *hlay_select = new QHBoxLayout;
    hlay_select->setContentsMargins(0, 0, 0, 0);
    hlay_select->addWidget(selectColorLabel);
    hlay_select->addWidget(selectColorCombobox);
    hlay_select->addStretch();

    // 階・通りスケール
    QLabel *scaleLabel = new QLabel(u8"・階／通りスケール", this);
    scaleLabel->setFixedWidth(150);
    QLabel *scaleColorLabel = new QLabel(u8"線色：", this);
    scaleColorCombobox = new TypicalColorComboBox(this);

    frscaleCheck = new QCheckBox(u8"通りスケールの表示", this);
    frscaleCheck->setFixedWidth(135);
    QLabel *frscaleFloorLabel = new QLabel(u8"表示階：", this);
    frscaleFloorCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::FixedInput, this);
    frscaleFloorCombobox->setDefaultName(u8"（表示しない）");

    connect(frscaleCheck, &QAbstractButton::toggled, frscaleFloorLabel, &QWidget::setEnabled );
    connect(frscaleCheck, &QAbstractButton::toggled, frscaleFloorCombobox, &QWidget::setEnabled );

    flscaleCheck = new QCheckBox(u8"階スケールの表示", this);
    flscaleCheck->setFixedWidth(135);
    QLabel *flscaleXLabel = new QLabel(u8"X座標(m)：", this);
    flscaleXLine = new QLineEdit(this);
    flscaleXLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    flscaleXLine->setAlignment(Qt::AlignRight);
    flscaleXLine->setFixedWidth(70);
    QLabel *flscaleYLabel = new QLabel(u8"Y座標(m)：", this);
    flscaleYLine = new QLineEdit(this);
    flscaleYLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    flscaleYLine->setAlignment(Qt::AlignRight);
    flscaleYLine->setFixedWidth(70);

    connect(flscaleCheck, &QAbstractButton::toggled, flscaleXLine, &QWidget::setEnabled );
    connect(flscaleCheck, &QAbstractButton::toggled, flscaleXLabel, &QWidget::setEnabled );
    connect(flscaleCheck, &QAbstractButton::toggled, flscaleYLine, &QWidget::setEnabled );
    connect(flscaleCheck, &QAbstractButton::toggled, flscaleYLabel, &QWidget::setEnabled );

    QHBoxLayout *hlay_scale = new QHBoxLayout;
    hlay_scale->setContentsMargins(0, 0, 0, 0);
    hlay_scale->addWidget(scaleLabel);
    hlay_scale->addWidget(scaleColorLabel);
    hlay_scale->addWidget(scaleColorCombobox);
    hlay_scale->addStretch();
    QHBoxLayout *hlay_frscale = new QHBoxLayout;
    hlay_frscale->setContentsMargins(0, 0, 0, 0);
    hlay_frscale->addSpacing(15);
    hlay_frscale->addWidget(frscaleCheck);
    hlay_frscale->addWidget(frscaleFloorLabel);
    hlay_frscale->addWidget(frscaleFloorCombobox);
    hlay_frscale->addStretch();
    QHBoxLayout *hlay_flscale = new QHBoxLayout;
    hlay_flscale->setContentsMargins(0, 0, 0, 0);
    hlay_flscale->addSpacing(15);
    hlay_flscale->addWidget(flscaleCheck);
    hlay_flscale->addWidget(flscaleXLabel);
    hlay_flscale->addWidget(flscaleXLine);
    hlay_flscale->addSpacing(20);
    hlay_flscale->addWidget(flscaleYLabel);
    hlay_flscale->addWidget(flscaleYLine);
    hlay_flscale->addStretch();

    QLabel *jloadSizeLabel = new QLabel(u8"節点サイズ：", this);
    jloadSizeLabel->setFixedWidth(110);
    jloadSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *lay_jsize = new QHBoxLayout;
    lay_jsize->addWidget(jloadSizeLabel);
    lay_jsize->addWidget(jloadSizeCombobox);
    lay_jsize->addStretch();

    QLabel *mloadSizeLabel = new QLabel(u8"分布荷重サイズ：", this);
    mloadSizeLabel->setFixedWidth(110);
    mloadSizeSpin = new QDoubleSpinBox(this);
    mloadSizeSpin->setRange(0.01, 999.99);
    QLabel *mloadSize2Label = new QLabel(" kN/m = 1.0m", this);
    QHBoxLayout *lay_msize = new QHBoxLayout;
    lay_msize->addWidget(mloadSizeLabel);
    lay_msize->addWidget(mloadSizeSpin);
    lay_msize->addWidget(mloadSize2Label);
    lay_msize->addStretch();

    jloadCheck = new QCheckBox(u8"節点荷重の表示", this);
    jloadCheck->setFixedWidth(140);
    QLabel *jloadMinLabel = new QLabel(u8"最小色：", this);
    jloadMinColor = new TypicalColorComboBox(this);
    jloadMinSpin = new QDoubleSpinBox(this);
    jloadMinSpin->setRange(0.00, 999.99);
    QLabel *jloadMin2Label = new QLabel(" kN", this);
    jloadMin2Label->setFixedWidth(35);
    QLabel *jloadMaxLabel = new QLabel(u8"最大色：", this);
    jloadMaxColor = new TypicalColorComboBox(this);
    jloadMaxSpin = new QDoubleSpinBox(this);
    jloadMaxSpin->setRange(0.00, 999.99);
    QLabel *jloadMax2Label = new QLabel(" kN", this);
    jloadMax2Label->setFixedWidth(35);
    QHBoxLayout *lay_jload = new QHBoxLayout;
    lay_jload->addWidget(jloadCheck);
    lay_jload->addWidget(jloadMinLabel);
    lay_jload->addWidget(jloadMinColor);
    lay_jload->addWidget(jloadMinSpin);
    lay_jload->addWidget(jloadMin2Label);
    lay_jload->addSpacing(10);
    lay_jload->addWidget(jloadMaxLabel);
    lay_jload->addWidget(jloadMaxColor);
    lay_jload->addWidget(jloadMaxSpin);
    lay_jload->addWidget(jloadMax2Label);
    lay_jload->addStretch();

    connect( jloadCheck, &QAbstractButton::toggled, jloadMinLabel,  &QWidget::setEnabled );
    connect( jloadCheck, &QAbstractButton::toggled, jloadMinSpin,   &QWidget::setEnabled );
    connect( jloadCheck, &QAbstractButton::toggled, jloadMin2Label, &QWidget::setEnabled );
    connect( jloadCheck, &QAbstractButton::toggled, jloadMinColor,  &QWidget::setEnabled );
    connect( jloadCheck, &QAbstractButton::toggled, jloadMaxLabel,  &QWidget::setEnabled );
    connect( jloadCheck, &QAbstractButton::toggled, jloadMaxSpin,   &QWidget::setEnabled );
    connect( jloadCheck, &QAbstractButton::toggled, jloadMax2Label, &QWidget::setEnabled );
    connect( jloadCheck, &QAbstractButton::toggled, jloadMaxColor,  &QWidget::setEnabled );
    jloadCheck->setChecked(true);

    ploadCheck = new QCheckBox(u8"集中荷重の表示", this);
    ploadCheck->setFixedWidth(140);
    QLabel *ploadMinLabel = new QLabel(u8"最小色：", this);
    ploadMinColor = new TypicalColorComboBox(this);
    ploadMinSpin = new QDoubleSpinBox(this);
    ploadMinSpin->setRange(0.00, 999.99);
    QLabel *ploadMin2Label = new QLabel(" kN", this);
    ploadMin2Label->setFixedWidth(35);
    QLabel *ploadMaxLabel = new QLabel(u8"最大色：", this);
    ploadMaxColor = new TypicalColorComboBox(this);
    ploadMaxSpin = new QDoubleSpinBox(this);
    ploadMaxSpin->setRange(0.00, 999.99);
    QLabel *ploadMax2Label = new QLabel(" kN", this);
    ploadMax2Label->setFixedWidth(35);
    QHBoxLayout *lay_pload = new QHBoxLayout;
    lay_pload->addWidget(ploadCheck);
    lay_pload->addWidget(ploadMinLabel);
    lay_pload->addWidget(ploadMinColor);
    lay_pload->addWidget(ploadMinSpin);
    lay_pload->addWidget(ploadMin2Label);
    lay_pload->addSpacing(10);
    lay_pload->addWidget(ploadMaxLabel);
    lay_pload->addWidget(ploadMaxColor);
    lay_pload->addWidget(ploadMaxSpin);
    lay_pload->addWidget(ploadMax2Label);
    lay_pload->addStretch();

    connect( ploadCheck, &QAbstractButton::toggled, ploadMinLabel,  &QWidget::setEnabled );
    connect( ploadCheck, &QAbstractButton::toggled, ploadMinSpin,   &QWidget::setEnabled );
    connect( ploadCheck, &QAbstractButton::toggled, ploadMin2Label, &QWidget::setEnabled );
    connect( ploadCheck, &QAbstractButton::toggled, ploadMinColor,  &QWidget::setEnabled );
    connect( ploadCheck, &QAbstractButton::toggled, ploadMaxLabel,  &QWidget::setEnabled );
    connect( ploadCheck, &QAbstractButton::toggled, ploadMaxSpin,   &QWidget::setEnabled );
    connect( ploadCheck, &QAbstractButton::toggled, ploadMax2Label, &QWidget::setEnabled );
    connect( ploadCheck, &QAbstractButton::toggled, ploadMaxColor,  &QWidget::setEnabled );
    ploadCheck->setChecked(true);

    wloadCheck = new QCheckBox(u8"分布荷重（自重）の表示", this);
    wloadCheck->setFixedWidth(140);
    QLabel *wloadMinLabel = new QLabel(u8"最小色：", this);
    wloadMinColor = new TypicalColorComboBox(this);
    wloadMinSpin = new QDoubleSpinBox(this);
    wloadMinSpin->setRange(0.00, 999.99);
    QLabel *wloadMin2Label = new QLabel(" kN/m", this);
    wloadMin2Label->setFixedWidth(35);
    QLabel *wloadMaxLabel = new QLabel(u8"最大色：", this);
    wloadMaxColor = new TypicalColorComboBox(this);
    wloadMaxSpin = new QDoubleSpinBox(this);
    wloadMaxSpin->setRange(0.00, 999.99);
    QLabel *wloadMax2Label = new QLabel(" kN/m", this);
    wloadMax2Label->setFixedWidth(35);
    QHBoxLayout *lay_wload = new QHBoxLayout;
    lay_wload->addWidget(wloadCheck);
    lay_wload->addWidget(wloadMinLabel);
    lay_wload->addWidget(wloadMinColor);
    lay_wload->addWidget(wloadMinSpin);
    lay_wload->addWidget(wloadMin2Label);
    lay_wload->addSpacing(10);
    lay_wload->addWidget(wloadMaxLabel);
    lay_wload->addWidget(wloadMaxColor);
    lay_wload->addWidget(wloadMaxSpin);
    lay_wload->addWidget(wloadMax2Label);
    lay_wload->addStretch();

    connect( wloadCheck, &QAbstractButton::toggled, wloadMinLabel,  &QWidget::setEnabled );
    connect( wloadCheck, &QAbstractButton::toggled, wloadMinSpin,   &QWidget::setEnabled );
    connect( wloadCheck, &QAbstractButton::toggled, wloadMin2Label, &QWidget::setEnabled );
    connect( wloadCheck, &QAbstractButton::toggled, wloadMinColor,  &QWidget::setEnabled );
    connect( wloadCheck, &QAbstractButton::toggled, wloadMaxLabel,  &QWidget::setEnabled );
    connect( wloadCheck, &QAbstractButton::toggled, wloadMaxSpin,   &QWidget::setEnabled );
    connect( wloadCheck, &QAbstractButton::toggled, wloadMax2Label, &QWidget::setEnabled );
    connect( wloadCheck, &QAbstractButton::toggled, wloadMaxColor,  &QWidget::setEnabled );
    wloadCheck->setChecked(true);


    dloadCheck = new QCheckBox(u8"分布荷重（床）の表示", this);
    dloadCheck->setFixedWidth(140);
    QLabel *dloadMinLabel = new QLabel(u8"最小色：", this);
    dloadMinColor = new TypicalColorComboBox(this);
    dloadMinSpin = new QDoubleSpinBox(this);
    dloadMinSpin->setRange(0.00, 999.99);
    QLabel *dloadMin2Label = new QLabel(" kN/m", this);
    dloadMin2Label->setFixedWidth(35);
    QLabel *dloadMaxLabel = new QLabel(u8"最大色：", this);
    dloadMaxColor = new TypicalColorComboBox(this);
    dloadMaxSpin = new QDoubleSpinBox(this);
    dloadMaxSpin->setRange(0.00, 999.99);
    QLabel *dloadMax2Label = new QLabel(" kN/m", this);
    dloadMax2Label->setFixedWidth(35);
    QHBoxLayout *lay_dload = new QHBoxLayout;
    lay_dload->addWidget(dloadCheck);
    lay_dload->addWidget(dloadMinLabel);
    lay_dload->addWidget(dloadMinColor);
    lay_dload->addWidget(dloadMinSpin);
    lay_dload->addWidget(dloadMin2Label);
    lay_dload->addSpacing(10);
    lay_dload->addWidget(dloadMaxLabel);
    lay_dload->addWidget(dloadMaxColor);
    lay_dload->addWidget(dloadMaxSpin);
    lay_dload->addWidget(dloadMax2Label);
    lay_dload->addStretch();

    connect( dloadCheck, &QAbstractButton::toggled, dloadMinLabel,  &QWidget::setEnabled );
    connect( dloadCheck, &QAbstractButton::toggled, dloadMinSpin,   &QWidget::setEnabled );
    connect( dloadCheck, &QAbstractButton::toggled, dloadMin2Label, &QWidget::setEnabled );
    connect( dloadCheck, &QAbstractButton::toggled, dloadMinColor,  &QWidget::setEnabled );
    connect( dloadCheck, &QAbstractButton::toggled, dloadMaxLabel,  &QWidget::setEnabled );
    connect( dloadCheck, &QAbstractButton::toggled, dloadMaxSpin,   &QWidget::setEnabled );
    connect( dloadCheck, &QAbstractButton::toggled, dloadMax2Label, &QWidget::setEnabled );
    connect( dloadCheck, &QAbstractButton::toggled, dloadMaxColor,  &QWidget::setEnabled );
    dloadCheck->setChecked(true);

    vlay->addLayout(hlay_joint);
    vlay->addLayout(hlay_line);
    vlay->addLayout(hlay_plane);
    vlay->addLayout(hlay_jno);
    vlay->addLayout(hlay_name);
    vlay->addLayout(hlay_bar);
    vlay->addLayout(hlay_alpha);
    vlay->addLayout(hlay_select);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_scale);
    vlay->addLayout(hlay_frscale);
    vlay->addLayout(hlay_flscale);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(lay_jsize);
    vlay->addLayout(lay_msize);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(lay_jload);
    vlay->addLayout(lay_pload);
    vlay->addLayout(lay_wload);
    vlay->addLayout(lay_dload);
}

CalculationLoad3DViewSettings CalculationLoad3DSettingDialog::makeSetting() const
{
    CalculationLoad3DViewSettings settings(
                jointCheck->isChecked(),
                jointColorCombobox->color(),
                lineCheck->isChecked(),
                lineColorCombobox->color(),
                planeCheck->isChecked(),
                planeColorCombobox->color(),
                jnoCheck->isChecked(),
                jnoColorCombobox->color(),
                jnoSizeCombobox->currentIndex(),
                nameCheck->isChecked(),
                nameColorCombobox->color(),
                nameSizeCombobox->currentIndex(),
                colorBarCheck->isChecked(),
                colorBarColorCombobox->color(),
                colorBarSizeCombobox->currentIndex(),
                valueAlphaSpinbox->value(),
                selectColorCombobox->color(),
                scaleColorCombobox->color(),
                frscaleCheck->isChecked(),
                frscaleFloorCombobox->currentUuid(),
                flscaleCheck->isChecked(),
                QPointF(flscaleXLine->text().toDouble(), flscaleYLine->text().toDouble()),
                jloadSizeCombobox->currentIndex(),
                mloadSizeSpin->value(),
                jloadCheck->isChecked(),
                jloadMinSpin->value(),
                jloadMinColor->color(),
                jloadMaxColor->color(),
                jloadMaxSpin->value(),
                ploadCheck->isChecked(),
                ploadMinSpin->value(),
                ploadMinColor->color(),
                ploadMaxColor->color(),
                ploadMaxSpin->value(),
                wloadCheck->isChecked(),
                wloadMinSpin->value(),
                wloadMinColor->color(),
                wloadMaxColor->color(),
                wloadMaxSpin->value(),
                dloadCheck->isChecked(),
                dloadMinSpin->value(),
                dloadMinColor->color(),
                dloadMaxColor->color(),
                dloadMaxSpin->value(),
                true);
    return settings;
}

void CalculationLoad3DSettingDialog::applySettings(const CalculationLoad3DViewSettings &set)
{
    jointCheck->setChecked(set.getIsJointVisible());
    jointColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getJointColor()));
    lineCheck->setChecked(set.getIsLineVisible());
    lineColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getLineColor()));
    planeCheck->setChecked(set.getIsPlaneVisible());
    planeColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getPlaneColor()));
    jnoCheck->setChecked(set.getIsJnoVisible());
    jnoColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getJnoColor()));
    jnoSizeCombobox->setCurrentIndex(set.getJnoSize());
    nameCheck->setChecked(set.getIsNameVisible());
    nameColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getNameColor()));
    nameSizeCombobox->setCurrentIndex(set.getNameSize());
    colorBarCheck->setChecked(set.getIsColorBarVisible());
    colorBarColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getColorBarTextColor()));
    colorBarSizeCombobox->setCurrentIndex(set.getColorBarTextSize());
    valueAlphaSpinbox->setValue(set.getValueAlpha());
    selectColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getSelectColor()));
    scaleColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getScaleColor()));
    frscaleCheck->setChecked(set.getIsFrScaleVisible());
    frscaleFloorCombobox->setCurrentText(UnifiedFixedData::getInstance()->getInputData()->idToName(set.getFrScaleFloor(), DATATYPE::TPFLOOR));
    flscaleCheck->setChecked(set.getIsFlScaleVisible());
    flscaleXLine->setText(QString::number(set.getFloorScaleXY().x()));
    flscaleYLine->setText(QString::number(set.getFloorScaleXY().y()));
    jloadSizeCombobox->setCurrentIndex(set.getJloadSize());
    mloadSizeSpin->setValue(set.getMloadSize());
    jloadCheck->setChecked(set.getIsJloadVisible());
    jloadMinSpin->setValue(set.getJloadMinValue());
    jloadMinColor->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getJloadMinColor()));
    jloadMaxColor->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getJloadMaxColor()));
    jloadMaxSpin->setValue(set.getJloadMaxValue());
    ploadCheck->setChecked(set.getIsPloadVisible());
    ploadMinSpin->setValue(set.getPloadMinValue());
    ploadMinColor->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getPloadMinColor()));
    ploadMaxColor->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getPloadMaxColor()));
    ploadMaxSpin->setValue(set.getPloadMaxValue());
    wloadCheck->setChecked(set.getIsWloadVisible());
    wloadMinSpin->setValue(set.getWloadMinValue());
    wloadMinColor->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getWloadMinColor()));
    wloadMaxColor->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getWloadMaxColor()));
    wloadMaxSpin->setValue(set.getWloadMaxValue());
    dloadCheck->setChecked(set.getIsDloadVisible());
    dloadMinSpin->setValue(set.getDloadMinValue());
    dloadMinColor->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getDloadMinColor()));
    dloadMaxColor->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getDloadMaxColor()));
    dloadMaxSpin->setValue(set.getDloadMaxValue());
}

void CalculationLoad3DSettingDialog::initializeSettings()
{
    applySettings(CalculationLoad3DViewSettings::getDefault());
}

void CalculationLoad3DSettingDialog::uploadViewSettings()
{
    UnifiedSettings::getInstance()->uploadViewSettings(makeSetting());
    UnifiedSettings::getInstance()->signalViewSettingChanged(CALC3DMODEL);
}

void CalculationLoad3DSettingDialog::slotFileSave()
{
    QString filepath =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    saveSettings(filepath);
}

void CalculationLoad3DSettingDialog::slotFileOpen()
{
    QString filepath =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    readSettings(filepath);
}

void CalculationLoad3DSettingDialog::saveSettings(const QString &filepath) const{
    auto setting = makeSetting();
    setting.saveSettings(filepath);
}
void CalculationLoad3DSettingDialog::readSettings(const QString &filepath){
    auto setting = CalculationLoad3DViewSettings::readSettings(filepath);
    applySettings(setting);
}
} // namespace post3dapp

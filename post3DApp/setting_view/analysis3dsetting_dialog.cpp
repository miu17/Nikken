#include "analysis3dsetting_dialog.h"

#include "define_analysis3dsettings.h"
#include "fixed_data.h"
#include "reference_combobox.h"
#include "typicalcolor_combobox.h"
#include "unified_analysisdata.h"
#include "unified_settings.h"
#include "vhline.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{

Analysis3DSettingDialog::Analysis3DSettingDialog(QWidget *parent)
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

    connect( fileSaveButton, &QPushButton::clicked, this, &Analysis3DSettingDialog::slotFileSave);
    connect( fileOpenButton, &QPushButton::clicked, this, &Analysis3DSettingDialog::slotFileOpen);
    connect( defaultButton,  &QPushButton::clicked, this, &Analysis3DSettingDialog::initializeSettings);
    connect( uploadButton,   &QPushButton::clicked, this, &Analysis3DSettingDialog::uploadViewSettings);

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

    this->setWindowTitle(u8"解析結果（３Ｄパース）　表示設定");
    this->setWindowIcon(QIcon(QPixmap(":/icons/color_settings.png")));
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(800, 650));


    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    ( QFile(filepath).exists() ) ? readSettings(filepath) : initializeSettings() ;

    uploadViewSettings();
}

Analysis3DSettingDialog::~Analysis3DSettingDialog()
{
    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    saveSettings(filepath);
}

void Analysis3DSettingDialog::createWidget(QVBoxLayout* vlay)
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
    frscaleFloorCombobox = new ReferenceCombobox(DATATYPE::TPAFLOOR, UnifiedDataType::FixedInput, this);
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
}

Analysis3DViewSettings Analysis3DSettingDialog::makeSettings() const
{
    return Analysis3DViewSettings{
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
        UnifiedFixedData::getInstance()->getAnalysisData()->idToName(frscaleFloorCombobox->currentText(), DATATYPE::TPFLOOR),
        flscaleCheck->isChecked(),
        QPointF(flscaleXLine->text().toDouble(), flscaleYLine->text().toDouble()),
        true
    };
}

void Analysis3DSettingDialog::applySettings(const Analysis3DViewSettings &setting)
{
    jointCheck->setChecked(setting.getIsJointVisible());
    jointColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getJointColor()));
    lineCheck->setChecked(setting.getIsLineVisible());
    lineColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getLineColor()));
    planeCheck->setChecked(setting.getIsPlaneVisible());
    planeColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getPlaneColor()));
    jnoCheck->setChecked(setting.getIsJnoVisible());
    jnoColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getJnoColor()));
    jnoSizeCombobox->setCurrentIndex(setting.getJnoSize());
    nameCheck->setChecked(setting.getIsMemberNameVisible());
    nameColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getMemberNameColor()));
    nameSizeCombobox->setCurrentIndex(setting.getMemberNameSize());
    colorBarCheck->setChecked(setting.getIsColorBarVisible());
    colorBarColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getColorBarTextColor()));
    colorBarSizeCombobox->setCurrentIndex(setting.getColorBarTextSize());
    valueAlphaSpinbox->setValue(setting.getValueAlpha());
    selectColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getSelectColor()));
    scaleColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getScaleColor()));
    frscaleCheck->setChecked(setting.getIsFrScaleVisible());
    frscaleFloorCombobox->setCurrentText(UnifiedFixedData::getInstance()->getAnalysisData()->idToName(setting.getFrScaleFloor(), DATATYPE::TPFLOOR));
    flscaleCheck->setChecked(setting.getIsFlScaleVisible());
    flscaleXLine->setText(QString::number(setting.getFloorScaleXY().x()));
    flscaleYLine->setText(QString::number(setting.getFloorScaleXY().y()));
}

void Analysis3DSettingDialog::initializeSettings()
{
    auto setting = Analysis3DViewSettings::getDefault();
    applySettings(setting);
}

void Analysis3DSettingDialog::uploadViewSettings()
{
    UnifiedSettings::getInstance()->uploadViewSettings(makeSettings());
    UnifiedSettings::getInstance()->signalViewSettingChanged(OUTPUT3DMODEL);
}

void Analysis3DSettingDialog::slotFileSave()
{
    QString filepath =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    saveSettings(filepath);
}

void Analysis3DSettingDialog::slotFileOpen()
{
    QString filepath =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    readSettings(filepath);
}

void Analysis3DSettingDialog::saveSettings(const QString &filepath) const{
    auto setting = makeSettings();
    setting.saveSettings(filepath);
}
void Analysis3DSettingDialog::readSettings(const QString &filepath){
    auto setting = Analysis3DViewSettings::readSettings(filepath);
    applySettings(setting);
}
} // namespace post3dapp

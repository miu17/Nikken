#include "calculationload2dsetting_dialog.h"

#include "define_calculationload2dsettings.h"
#include "typicalcolor_combobox.h"
#include "unified_settings.h"
#include "vhline.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{

CalculationLoad2DSettingDialog::CalculationLoad2DSettingDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setFont(QFont(QString::fromLocal8Bit("Microsoft JhengHei"), 9));

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

    connect( fileSaveButton, &QPushButton::clicked, this, &CalculationLoad2DSettingDialog::slotFileSave );
    connect( fileOpenButton, &QPushButton::clicked, this, &CalculationLoad2DSettingDialog::slotFileOpen );
    connect( defaultButton, &QPushButton::clicked, this, &CalculationLoad2DSettingDialog::initializeSettings );
    connect( uploadButton,  &QPushButton::clicked, this, &CalculationLoad2DSettingDialog::uploadViewSettings );

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
    createWidgets(vlay);
    this->setLayout(vlay);

    this->setWindowTitle(u8"荷重拾い2D表示設定");
    this->setWindowIcon(QIcon(QPixmap(":/icons/color_settings.png")));
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(800, 650));

    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    ( QFile(filepath).exists() ) ? readSettings(filepath) : initializeSettings() ;
    uploadViewSettings();
}

CalculationLoad2DSettingDialog::~CalculationLoad2DSettingDialog()
{
    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    saveSettings(filepath);
}

void CalculationLoad2DSettingDialog::createWidgets(QVBoxLayout* vlay){
    // 方向
    directionCheckbox = new QCheckBox(u8"線材の入力方向（i端→j端）を示す記号を表示.", this);
    QHBoxLayout *hlay_direction = new QHBoxLayout;
    hlay_direction->setContentsMargins(0, 0, 0, 0);
    hlay_direction->addWidget(directionCheckbox);
    hlay_direction->addStretch();


    // 解析節点
    QLabel *jointLabel = new QLabel(u8"・解析節点", this);
    jointLabel->setFixedWidth(120);
    jointColorCombobox = new TypicalColorComboBox(this);
    jointStringCheck = new QCheckBox(u8"番号を表示:", this);
    jointStringCheck->setFixedWidth(100);
    QLabel *jointSizeLabel = new QLabel(u8"文字サイズ：", this);
    jointSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *hlay_joint = new QHBoxLayout;
    hlay_joint->setContentsMargins(0, 0, 0, 0);
    hlay_joint->addWidget(jointLabel);
    hlay_joint->addWidget(new QLabel(u8"色：", this));
    hlay_joint->addWidget(jointColorCombobox);
    hlay_joint->addSpacing(30);
    hlay_joint->addWidget(jointStringCheck);
    hlay_joint->addWidget(jointSizeLabel);
    hlay_joint->addWidget(jointSizeCombobox);
    hlay_joint->addStretch();

    connect( jointStringCheck, &QAbstractButton::toggled, jointSizeLabel,    &QWidget::setEnabled );
    connect( jointStringCheck, &QAbstractButton::toggled, jointSizeCombobox, &QWidget::setEnabled );

    jointStringCheck->setChecked(true);

    // 解析部材（線）
    QLabel *lineLabel = new QLabel(u8"・解析部材（線材）", this);
    lineLabel->setFixedWidth(120);
    QLabel *lineColorLabel = new QLabel(u8"色：", this);
    lineColorCombobox = new TypicalColorComboBox(this);
    lineStringCheck = new QCheckBox(u8"符号を表示:", this);
    lineStringCheck->setFixedWidth(100);
    QLabel *lineSizeLabel = new QLabel(u8"文字サイズ：", this);
    lineSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *hlay_line = new QHBoxLayout;
    hlay_line->setContentsMargins(0, 0, 0, 0);
    hlay_line->addWidget(lineLabel);
    hlay_line->addWidget(lineColorLabel);
    hlay_line->addWidget(lineColorCombobox);
    hlay_line->addSpacing(30);
    hlay_line->addWidget(lineStringCheck);
    hlay_line->addWidget(lineSizeLabel);
    hlay_line->addWidget(lineSizeCombobox);
    hlay_line->addStretch();

    connect( lineStringCheck, &QAbstractButton::toggled, lineSizeLabel,    &QWidget::setEnabled );
    connect( lineStringCheck, &QAbstractButton::toggled, lineSizeCombobox, &QWidget::setEnabled );

    lineStringCheck->setChecked(true);

    // 解析部材（面）
    QLabel *planeLabel = new QLabel(u8"・解析部材（面材）", this);
    planeLabel->setFixedWidth(120);
    QLabel *planeColorLabel = new QLabel(u8"色：", this);
    planeColorCombobox = new TypicalColorComboBox(this);
    planeStringCheck = new QCheckBox(u8"符号を表示:", this);
    planeStringCheck->setFixedWidth(100);
    QLabel *planeSizeLabel = new QLabel(u8"文字サイズ：", this);
    planeSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *hlay_plane = new QHBoxLayout;
    hlay_plane->setContentsMargins(0, 0, 0, 0);
    hlay_plane->addWidget(planeLabel);
    hlay_plane->addWidget(planeColorLabel);
    hlay_plane->addWidget(planeColorCombobox);
    hlay_plane->addSpacing(30);
    hlay_plane->addWidget(planeStringCheck);
    hlay_plane->addWidget(planeSizeLabel);
    hlay_plane->addWidget(planeSizeCombobox);
    hlay_plane->addStretch();

    connect( planeStringCheck, &QAbstractButton::toggled, planeSizeLabel,    &QWidget::setEnabled );
    connect( planeStringCheck, &QAbstractButton::toggled, planeSizeCombobox, &QWidget::setEnabled );
    planeStringCheck->setChecked(true);

    //　選択時の色
    QLabel *selectColorLabel = new QLabel(u8"選択カラー:", this);
    selectColorLabel->setFixedWidth(120);
    selectColorCombobox = new TypicalColorComboBox(this);
    QHBoxLayout *hlay_select = new QHBoxLayout;
    hlay_select->setContentsMargins(0, 0, 0, 0);
    hlay_select->addWidget(selectColorLabel);
    hlay_select->addWidget(selectColorCombobox);
    hlay_select->addStretch();

    // 階・通りのライン
    QLabel *scaleLineLabel = new QLabel(u8"階／通りスケールの表示カラー:", this);
    scaleColorCombobox = new TypicalColorComboBox(this);
    QHBoxLayout *hlay_scale = new QHBoxLayout;
    hlay_scale->setContentsMargins(0, 0, 0, 0);
    hlay_scale->addWidget(scaleLineLabel);
    hlay_scale->addSpacing(30);
    hlay_scale->addWidget(scaleColorCombobox);
    hlay_scale->addStretch();

    // 荷重分布図
    QLabel *stringSizeLabel = new QLabel(u8"フォントサイズ：", this);
    stringSizeLabel->setFixedWidth(100);
    stringSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *lay_load = new QHBoxLayout;
    lay_load->addWidget(stringSizeLabel);
    lay_load->addWidget(stringSizeCombobox);
    lay_load->addStretch();

    jCheck = new QCheckBox(u8"節点荷重の表示", this);
    jCheck->setFixedWidth(140);
    QLabel *jColorLabel = new QLabel(u8"色：", this);
    jColorCombobox = new TypicalColorComboBox(this);
    QLabel *jSizeLabel = new QLabel(u8"サイズ：", this);
    jSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *lay_jload = new QHBoxLayout;
    lay_jload->addWidget(jCheck);
    lay_jload->addWidget(jColorLabel);
    lay_jload->addWidget(jColorCombobox);
    lay_jload->addSpacing(20);
    lay_jload->addWidget(jSizeLabel);
    lay_jload->addWidget(jSizeCombobox);
    lay_jload->addStretch();

    connect( jCheck, &QAbstractButton::toggled, jColorLabel,    &QWidget::setEnabled );
    connect( jCheck, &QAbstractButton::toggled, jColorCombobox, &QWidget::setEnabled );
    connect( jCheck, &QAbstractButton::toggled, jSizeLabel,     &QWidget::setEnabled );
    connect( jCheck, &QAbstractButton::toggled, jSizeCombobox,  &QWidget::setEnabled );
    jCheck->setChecked(true);

    pCheck = new QCheckBox(u8"集中荷重の表示", this);
    pCheck->setFixedWidth(140);
    QLabel *pColorLabel = new QLabel(u8"色：", this);
    pColorCombobox = new TypicalColorComboBox(this);
    QLabel *pSizeLabel = new QLabel(u8"サイズ：", this);
    pSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *lay_pload = new QHBoxLayout;
    lay_pload->addWidget(pCheck);
    lay_pload->addWidget(pColorLabel);
    lay_pload->addWidget(pColorCombobox);
    lay_pload->addSpacing(20);
    lay_pload->addWidget(pSizeLabel);
    lay_pload->addWidget(pSizeCombobox);
    lay_pload->addStretch();

    connect( pCheck, &QAbstractButton::toggled, pColorLabel,    &QWidget::setEnabled );
    connect( pCheck, &QAbstractButton::toggled, pColorCombobox, &QWidget::setEnabled );
    connect( pCheck, &QAbstractButton::toggled, pSizeLabel,     &QWidget::setEnabled );
    connect( pCheck, &QAbstractButton::toggled, pSizeCombobox,  &QWidget::setEnabled );
    pCheck->setChecked(true);

    wCheck = new QCheckBox(u8"分布荷重(自重)の表示", this);
    wCheck->setFixedWidth(140);
    QLabel *wColorLabel = new QLabel(u8"色：", this);
    wColorCombobox = new TypicalColorComboBox(this);
    QLabel *wScale1Label = new QLabel(u8"スケール：", this);
    wScaleSpin = new QDoubleSpinBox(this);
    wScaleSpin->setRange(0.01, 999.99);
    QLabel *wScale2Label = new QLabel(" kN/m = 1.0m", this);
    QHBoxLayout *lay_wload = new QHBoxLayout;
    lay_wload->addWidget(wCheck);
    lay_wload->addWidget(wColorLabel);
    lay_wload->addWidget(wColorCombobox);
    lay_wload->addSpacing(20);
    lay_wload->addWidget(wScale1Label);
    lay_wload->addWidget(wScaleSpin);
    lay_wload->addWidget(wScale2Label);
    lay_wload->addStretch();

    connect( wCheck, &QAbstractButton::toggled, wColorLabel,    &QWidget::setEnabled );
    connect( wCheck, &QAbstractButton::toggled, wColorCombobox, &QWidget::setEnabled );
    connect( wCheck, &QAbstractButton::toggled, wScale1Label,   &QWidget::setEnabled );
    connect( wCheck, &QAbstractButton::toggled, wScaleSpin,     &QWidget::setEnabled );
    connect( wCheck, &QAbstractButton::toggled, wScale2Label,   &QWidget::setEnabled );
    wCheck->setChecked(true);

    dCheck = new QCheckBox(u8"分布荷重（床）の表示", this);
    dCheck->setFixedWidth(140);
    QLabel *dColorLabel = new QLabel(u8"色：", this);
    dColorCombobox = new TypicalColorComboBox(this);
    QLabel *dScale1Label = new QLabel(u8"スケール：", this);
    dScaleSpin = new QDoubleSpinBox(this);
    dScaleSpin->setRange(0.01, 999.99);
    QLabel *dScale2Label = new QLabel(" kN/m = 1.0m", this);
    QHBoxLayout *lay_dload = new QHBoxLayout;
    lay_dload->addWidget(dCheck);
    lay_dload->addWidget(dColorLabel);
    lay_dload->addWidget(dColorCombobox);
    lay_dload->addSpacing(20);
    lay_dload->addWidget(dScale1Label);
    lay_dload->addWidget(dScaleSpin);
    lay_dload->addWidget(dScale2Label);
    lay_dload->addStretch();

    connect( dCheck, &QAbstractButton::toggled, dColorLabel,    &QWidget::setEnabled );
    connect( dCheck, &QAbstractButton::toggled, dColorCombobox, &QWidget::setEnabled );
    connect( dCheck, &QAbstractButton::toggled, dScale1Label,   &QWidget::setEnabled );
    connect( dCheck, &QAbstractButton::toggled, dScaleSpin,     &QWidget::setEnabled );
    connect( dCheck, &QAbstractButton::toggled, dScale2Label,   &QWidget::setEnabled );
    dCheck->setChecked(true);

    vlay->addLayout(hlay_direction);
    //vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_joint);
    vlay->addLayout(hlay_line);
    vlay->addLayout(hlay_plane);
    vlay->addLayout(hlay_select);
    vlay->addLayout(hlay_scale);
    vlay->addLayout(lay_load);
    vlay->addLayout(lay_jload);
    vlay->addLayout(lay_pload);
    vlay->addLayout(lay_wload);
    vlay->addLayout(lay_dload);
    vlay->addStretch();
    //vlay->addWidget(new HFLine(Qt::lightGray, this));
}


void CalculationLoad2DSettingDialog::initializeSettings()
{
    applySettings(CalculationLoad2DViewSettings::getDefault());
}

void CalculationLoad2DSettingDialog::uploadViewSettings()
{
    UnifiedSettings::getInstance()->uploadViewSettings(makeSettings());
}

CalculationLoad2DViewSettings CalculationLoad2DSettingDialog::makeSettings() const{
    CalculationLoad2DViewSettings setting(
                directionCheckbox->isChecked(),
                jointColorCombobox->color(),
                jointStringCheck->isChecked(),
                jointSizeCombobox->size(),
                lineColorCombobox->color(),
                lineStringCheck->isChecked(),
                lineSizeCombobox->size(),
                planeColorCombobox->color(),
                planeStringCheck->isChecked(),
                planeSizeCombobox->size(),
                selectColorCombobox->color(),
                scaleColorCombobox->color(),
                stringSizeCombobox->size(),
                jCheck->isChecked(),
                jColorCombobox->color(),
                jSizeCombobox->size(),
                pCheck->isChecked(),
                pColorCombobox->color(),
                pSizeCombobox->size(),
                wCheck->isChecked(),
                wColorCombobox->color(),
                wScaleSpin->value(),
                dCheck->isChecked(),
                dColorCombobox->color(),
                dScaleSpin->value(),
                true);
    return setting;
}

void CalculationLoad2DSettingDialog::applySettings(const CalculationLoad2DViewSettings &set)
{
    directionCheckbox->setChecked(set.getIsDirectionVisible());
    jointColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getJointColor()));
    jointStringCheck->setChecked(set.getIsJointStringVisible());
    jointSizeCombobox->setCurrentIndex(set.getJointSize());
    lineColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getLineColor()));
    lineStringCheck->setChecked(set.getIsLineStringVisible());
    lineSizeCombobox->setCurrentIndex(set.getLineSize());
    planeColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getPlaneColor()));
    planeStringCheck->setChecked(set.getIsPlaneStringVisible());
    planeSizeCombobox->setCurrentIndex(set.getPlaneSize());
    selectColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getSelectColor()));
    scaleColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getScaleColor()));
    stringSizeCombobox->setCurrentIndex(set.getStringSize());
    jCheck->setChecked(set.getJLoadVisible());
    jColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getJLoadColor()));
    jSizeCombobox->setCurrentIndex(set.getJLoadSize());
    pCheck->setChecked(set.getIsPLoadVisible());
    pColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getPLoadColor()));
    pSizeCombobox->setCurrentIndex(set.getPLoadSize());
    wCheck->setChecked(set.getIsWloadVisible());
    wColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getWLoadColor()));
    wScaleSpin->setValue(set.getWLoadSize());
    dCheck->setChecked(set.getIsDloadVisible());
    dColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(set.getDloadColor()));
    dScaleSpin->setValue(set.getDloadSize());
}

void CalculationLoad2DSettingDialog::slotFileSave()
{
    QString filepath =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    saveSettings(filepath);
}

void CalculationLoad2DSettingDialog::slotFileOpen()
{
    QString filepath =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    readSettings(filepath);
}

void CalculationLoad2DSettingDialog::saveSettings(const QString &filepath) const{
    auto setting = makeSettings();
    setting.saveSettings(filepath);
}

void CalculationLoad2DSettingDialog::readSettings(const QString &filepath){
    CalculationLoad2DViewSettings set(CalculationLoad2DViewSettings::readSettings(filepath));
    applySettings(set);
}

} // namespace post3dapp

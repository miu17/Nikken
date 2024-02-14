#include "analysis2dsetting_dialog.h"

#include "analysis2dsetting_tabwidget.h"
#include "unified_settings.h"
#include "vhline.h"
namespace post3dapp{

Analysis2DSettingDialog::Analysis2DSettingDialog(QWidget *parent)
    :QDialog(parent)
{
    this->setFont(QFont(QString::fromLocal8Bit("Microsoft JhengHei"), 9));


    tabWidget = new Analysis2DSettingTabWidget();

    QPushButton *fileOpenButton = new QPushButton(u8"ファイル読込", this);
    QPushButton *fileSaveButton = new QPushButton(u8"ファイル保存", this);
    QPushButton *defaultButton = new QPushButton(u8"デフォルト値", this);
    QPushButton *uploadButton = new QPushButton(u8"画面更新", this);
    fileOpenButton->setFixedWidth(90);
    fileSaveButton->setFixedWidth(90);
    defaultButton->setFixedWidth(90);
    uploadButton->setFixedWidth(90);

    connect( fileSaveButton, &QPushButton::clicked, this, &Analysis2DSettingDialog::slotFileSave);
    connect( fileOpenButton, &QPushButton::clicked, this, &Analysis2DSettingDialog::slotFileOpen);
    connect( defaultButton, &QPushButton::clicked, tabWidget, &Analysis2DSettingTabWidget::initializeSettings);
    connect( uploadButton,  &QPushButton::clicked, this, &Analysis2DSettingDialog::uploadViewSettings);

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

    this->setWindowTitle(u8"解析モデル(2D)表示設定");
    this->setWindowIcon(QIcon(QPixmap(":/icons/color_settings.png")));
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(800, 650));

    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    ( QFile(filepath).exists() ) ? readSettings(filepath) : tabWidget->initializeSettings() ;
    uploadViewSettings();
}

Analysis2DSettingDialog::~Analysis2DSettingDialog()
{
    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    saveSettings(filepath);
}

void Analysis2DSettingDialog::uploadViewSettings()
{
    UnifiedSettings::getInstance()->uploadViewSettings(makeSetting());
}

void Analysis2DSettingDialog::slotFileSave()
{
    QString filepath =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    saveSettings(filepath);
}

void Analysis2DSettingDialog::slotFileOpen()
{
    QString filepath =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    readSettings(filepath);
}

Analysis2DViewSettings Analysis2DSettingDialog::makeSetting() const
{
    return tabWidget->makeSettings();
}

void Analysis2DSettingDialog::applySettings(const Analysis2DViewSettings &settings)
{
    tabWidget->applySettings(settings);
}

void Analysis2DSettingDialog::saveSettings(const QString &filepath) const
{
    auto settings = makeSetting();
    settings.saveSettings(filepath);
}
void Analysis2DSettingDialog::readSettings(const QString &filepath)
{
    Analysis2DViewSettings settings = Analysis2DViewSettings::readSettings(filepath);
    applySettings(settings);
}

} // namespace post3dapp

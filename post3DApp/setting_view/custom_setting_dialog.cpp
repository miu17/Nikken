#include "custom_setting_dialog.h"

#include <QtWidgets>

#include "setting_widget_factory.h"
#include "unified_settings.h"
#include "vhline.h"

namespace post3dapp{
/*
CustomSettingDialog::CustomSettingDialog(const SETTINGDIALOGSETTINGS& s,QWidget *parent)
    :QDialog(parent),myDialogSetting(s)
{
    this->setFont(QFont(QString::fromLocal8Bit("Microsoft JhengHei"), 9));

//FIGURE_TYPE_PROPERTY type;
//type.viewtype = s.modelViewType;
//type.type = s.type;
    myWidget = SettingWidgetFactory::createSettingWidget(s);
//    tabWidget = new Analysis2DSettingTabWidget();

    QPushButton *fileOpenButton = new QPushButton(u8"ファイル読込", this);
    QPushButton *fileSaveButton = new QPushButton(u8"ファイル保存", this);
    QPushButton *defaultButton = new QPushButton(u8"デフォルト値", this);
    QPushButton *uploadButton = new QPushButton(u8"画面更新", this);
    fileOpenButton->setFixedWidth(90);
    fileSaveButton->setFixedWidth(90);
    defaultButton->setFixedWidth(90);
    uploadButton->setFixedWidth(90);

    connect( fileSaveButton, &QPushButton::clicked, this, &CustomSettingDialog::slotFileSave);
    connect( fileOpenButton, &QPushButton::clicked, this, &CustomSettingDialog::slotFileOpen);
//    connect( defaultButton, &QPushButton::clicked, tabWidget, &Analysis2DSettingTabWidget::initializeSettings);
    connect( uploadButton,  &QPushButton::clicked, this, &CustomSettingDialog::uploadViewSettings);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setSpacing(5);
    hlay->addWidget(fileOpenButton);
    hlay->addWidget(fileSaveButton);
    hlay->addWidget(defaultButton);
    hlay->addStretch();
    hlay->addWidget(uploadButton);
QComboBox* combo = new QComboBox;

QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(hlay);
//    vlay->addWidget(tabWidget);
    this->setLayout(vlay);

    this->setWindowTitle(u8"解析モデル(2D)表示設定");
    this->setWindowIcon(QIcon(QPixmap(":/icons/color_settings.png")));
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(800, 650));

    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
//    ( QFile(filepath).exists() ) ? readSettings(filepath) : tabWidget->initializeSettings() ;
    uploadViewSettings();
}

CustomSettingDialog::~CustomSettingDialog()
{
    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    saveSettings(filepath);
}

void CustomSettingDialog::uploadViewSettings()
{
    qDebug()<<"call CustomSettingDialog::uploadViewSettings()";
//    UnifiedSettings::getInstance()->uploadViewSettings(createOutput2dViewSetting());
}

void CustomSettingDialog::slotFileSave()
{
    QString filepath =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    saveSettings(filepath);
}

void CustomSettingDialog::slotFileOpen()
{
    QString filepath =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    readSettings(filepath);
}
//OUTPUT2DVIEWSETTINGS CustomSettingDialog::createOutput2dViewSetting() const{
//    qDebug()<<"call createOutput2dViewSetting()";
////    return tabWidget->createOutput2dViewSetting();
//}

void CustomSettingDialog::saveSettings(const QString &filepath) const{
    QSettings settings(filepath, QSettings::IniFormat);
//    tabWidget->saveSettings(settings);
}
void CustomSettingDialog::readSettings(const QString &filepath){
    QSettings settings(filepath, QSettings::IniFormat);
//    tabWidget->readSettings(settings);
}
*/

} // namespace post3dapp

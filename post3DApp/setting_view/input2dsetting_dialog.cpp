#include "input2dsetting_dialog.h"

#include <QtGui>

#include "define_individualsetting.h"
#include "define_input2dsettings.h"
#include "elided_label.h"
#include "individualsetting_tablewidget.h"
#include "nameselect_dialog.h"
#include "reference_combobox.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
#include "unified_settings.h"
#include "vhline.h"
#include "viewsetting_partswidget.h"


namespace post3dapp{
Input2DSettingDialog::Input2DSettingDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setFont(QFont(tr("Microsoft JhengHei"), 9));

    createAllWidgets();
    createIndividualWidgets();

    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(planAllWidget, u8"全　体");
    tabWidget->addTab(planIndividualWidget, u8"符号別");
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

    connect( fileSaveButton, &QAbstractButton::clicked, this, &Input2DSettingDialog::slotFileSave );
    connect( fileOpenButton, &QAbstractButton::clicked, this, &Input2DSettingDialog::slotFileOpen );
    connect( defaultButton, &QAbstractButton::clicked, this, &Input2DSettingDialog::initializeSettings );
    connect( uploadButton,  &QAbstractButton::clicked, this, &Input2DSettingDialog::uploadViewSettings );

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

    this->setWindowTitle(u8"形状モデル（伏図・軸組図）　表示設定");
    this->setWindowIcon(QIcon(QPixmap(":/icons/color_settings.png")));
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(800, 650));

    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    ( QFile(filepath).exists() ) ? readSettings(filepath) : initializeSettings() ;
    uploadViewSettings();
}

Input2DSettingDialog::~Input2DSettingDialog()
{
    QString filepath = QDir::currentPath() + "/lastViewSetting.ini";
    saveSettings(filepath);
}

void Input2DSettingDialog::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    QDialog::closeEvent(event);
}

void Input2DSettingDialog::initializeSettings()
{
    applySettings(Input2DViewSettings::getDefault());
}

void Input2DSettingDialog::uploadViewSettings()
{
    UnifiedSettings::getInstance()->uploadViewSettings(makeSettings());
    UnifiedSettings::getInstance()->signalViewSettingChanged(INPUT2DMODEL);
}

void Input2DSettingDialog::createAllWidgets()
{
    // 優先
    priorityCheckbox = new QCheckBox(u8"符号別の表示設定を優先.", this);
    QHBoxLayout *hlay_priority = new QHBoxLayout;
    hlay_priority->setContentsMargins(0, 0, 0, 0);
    hlay_priority->addWidget(priorityCheckbox);
    hlay_priority->addStretch();

    // 入力方向
    directionCheckbox = new QCheckBox(u8"線材の入力方向（i端→j端）を示す記号を表示.", this);
    QHBoxLayout *hlay_direction = new QHBoxLayout;
    hlay_direction->setContentsMargins(0, 0, 0, 0);
    hlay_direction->addWidget(directionCheckbox);
    hlay_direction->addStretch();

    // 階・通りのライン
    QLabel *frameLineLabel = new QLabel(u8"・階／通りスケールの表示色", this);
    QLabel *frameNormalLabel = new QLabel(u8"通常時の線色：", this);
    QLabel *frameFocusLabel = new QLabel(u8"フォーカス時の線色：", this);
    frlineNormalCombobox = new TypicalColorComboBox(this);
    frlineFocusCombobox = new TypicalColorComboBox(this);

    QHBoxLayout *hlay_frame = new QHBoxLayout;
    hlay_frame->setContentsMargins(0, 0, 0, 0);
    hlay_frame->addWidget(frameLineLabel);
    hlay_frame->addSpacing(30);
    hlay_frame->addWidget(frameNormalLabel);
    hlay_frame->addWidget(frlineNormalCombobox);
    hlay_frame->addSpacing(30);
    hlay_frame->addWidget(frameFocusLabel);
    hlay_frame->addWidget(frlineFocusCombobox);
    hlay_frame->addStretch();

    // 文字サイズ
    QLabel *sizeLabel = new QLabel(u8"・文字サイズ", this);
    QLabel *jointSizeLabel = new QLabel(u8"節点番号：", this);
    QLabel *memberSizeLabel = new QLabel(u8"部材符号：", this);
    QLabel *loadSizeLabel = new QLabel(u8"荷重符号：", this);
    jointSizeCombobox = new FiveSizeComboBox(this);
    memberSizeCombobox = new FiveSizeComboBox(this);
    loadSizeCombobox = new FiveSizeComboBox(this);
    QHBoxLayout *hlay_size = new QHBoxLayout;
    hlay_size->setContentsMargins(0, 0, 0, 0);
    hlay_size->addWidget(sizeLabel);
    hlay_size->addSpacing(30);
    hlay_size->addWidget(jointSizeLabel);
    hlay_size->addWidget(jointSizeCombobox);
    hlay_size->addSpacing(30);
    hlay_size->addWidget(memberSizeLabel);
    hlay_size->addWidget(memberSizeCombobox);
    hlay_size->addSpacing(30);
    hlay_size->addWidget(loadSizeLabel);
    hlay_size->addWidget(loadSizeCombobox);
    hlay_size->addStretch();

    ElementStateWidgetGroup *gg_dummy = new ElementStateWidgetGroup(u8"・ダミー要素の表示", this);
    ElementStateWidgetGroup *gg_error = new ElementStateWidgetGroup(u8"・エラー要素の表示", this);
    ElementStateWidgetGroup *gg_select = new ElementStateWidgetGroup(u8"・選択要素の表示", this);
    ElementStateWidgetGroup *gg_input = new ElementStateWidgetGroup(u8"・入力補助要素の表示", this);
    elementStateWidgets.insert(ELDUMMY, gg_dummy);
    elementStateWidgets.insert(ELERROR, gg_error);
    elementStateWidgets.insert(ELSELECT, gg_select);
    elementStateWidgets.insert(ELINPUT, gg_input);

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
    ElementWidgetGroup *eg_jload = new ElementWidgetGroup(u8"節点荷重の表示", this);
    ElementWidgetGroup *eg_mload = new ElementWidgetGroup(u8"部材荷重の表示", this);
    ElementWidgetGroup *eg_lload = new ElementWidgetGroup(u8"床荷重の表示", this);
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
    elementWidgets.insert(ELJOINTLOAD, eg_jload);
    elementWidgets.insert(ELMEMBERLOAD, eg_mload);
    elementWidgets.insert(ELSLABLOAD, eg_lload);

    crossElementWidget = new ElementWidgetGroup(u8"(軸)直交部材の表示", this);
    downWidgetGroup = new DownWidgetGroup(u8"・(伏)下階柱の表示", this);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 10, 5, 10);
    vlay->setSpacing(3);
    vlay->addLayout(hlay_priority);
    vlay->addLayout(hlay_direction);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_frame);
    vlay->addLayout(hlay_size);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addWidget(gg_dummy);
    vlay->addWidget(gg_error);
    vlay->addWidget(gg_select);
    vlay->addWidget(gg_input);
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
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addWidget(eg_jload);
    vlay->addWidget(eg_mload);
    vlay->addWidget(eg_lload);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addWidget(crossElementWidget);
    vlay->addWidget(downWidgetGroup);
    vlay->addStretch();

    planAllWidget = new QWidget(this);
    planAllWidget->setLayout(vlay);
}

void Input2DSettingDialog::createIndividualWidgets()
{
    IndividualSettingType a_type;
    planIndividualWidget = new IndividualSettingTableWidget(a_type, this);
}

Input2DViewSettings Input2DSettingDialog::makeSettings() const
{
    QHash<ELEMENTSTATE, COLORBRUSHSETTING> state;
    QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT, ELINPUT};
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
        ELISO,
        ELJOINTLOAD,
        ELMEMBERLOAD,
        ELSLABLOAD
    };
    foreach(auto e, elementList){
        type.insert(e, elementWidgets.value(e)->getElementSetting());
    }

    QMap<QString, COLORBRUSHSETTING> individual;
    foreach(auto n, planIndividualWidget->getIndividualData()){
        QString name = n.at(1).toString();
        QColor line = TypicalColorTable::nameToColor(n.at(2).toString());
        QColor brush = TypicalColorTable::nameToColor(n.at(3).toString());
        brush.setAlpha(n.at(4).toInt());
        individual.insert(name, COLORBRUSHSETTING(line, brush));
    }

    auto setting = Input2DViewSettings(
                priorityCheckbox->isChecked(),
                directionCheckbox->isChecked(),
                frlineNormalCombobox->color(),
                frlineFocusCombobox->color(),
                jointSizeCombobox->currentIndex(),
                memberSizeCombobox->currentIndex(),
                loadSizeCombobox->currentIndex(),
                state,
                type,
                crossElementWidget->getElementSetting(),
                downWidgetGroup->getElementSetting(),
                individual,
                true,
                3
                );
    return setting;
}

void Input2DSettingDialog::applySettings(const Input2DViewSettings &setting)
{
    priorityCheckbox->setChecked(setting.getIsPriorIndividual());
    directionCheckbox->setChecked(setting.getIsLineDirVisible());
    frlineNormalCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getScaleColor()));
    frlineFocusCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(setting.getScaleFocusColor()));
    jointSizeCombobox->setCurrentIndex(setting.getJointNameSize());
    memberSizeCombobox->setCurrentIndex(setting.getMemberNameSize());
    loadSizeCombobox->setCurrentIndex(setting.getLoadNameSize());
    QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT, ELINPUT};
    foreach(auto s, stateList){
        elementStateWidgets.value(s)->setElementSetting(setting.getElementStateView(s));
    }

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
        ELISO,
        ELJOINTLOAD,
        ELMEMBERLOAD,
        ELSLABLOAD
    };
    foreach(auto e, elementList){
        elementWidgets.value(e)->setElementSetting(setting.getElementView(e));
    }

    crossElementWidget->setElementSetting(setting.getCrossElementView());
    downWidgetGroup->setElementSetting(setting.getDownColorBrushSetting());
    QList<QList<QVariant>> idvList;
    auto keys = setting.getNameKeys();
    for (int i = 0; i < keys.count(); i++){
        QString no = QString::number(i + 1);
        auto colorBrush = setting.getIndividualSetting(keys.at(i));
        QString label = keys.at(i);
        QString lineColor = TypicalColorTable::colorToName(colorBrush.lineColor.rgb());
        QString fillColor = TypicalColorTable::colorToName(colorBrush.paintBrush.color().rgb());
        int alpha = colorBrush.paintBrush.color().alpha();
        QList<QVariant> idList{no, label, lineColor, fillColor, alpha};
        idvList.append(idList);
    }
    planIndividualWidget->setIndividualData(idvList);
}

void Input2DSettingDialog::slotFileSave()
{
    QString filepath =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    saveSettings(filepath);
}

void Input2DSettingDialog::saveSettings(const QString &filepath) const
{
    auto settings = makeSettings();
    settings.saveSettings(filepath);
}

void Input2DSettingDialog::slotFileOpen()
{
    QString filepath =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    readSettings(filepath);
}

void Input2DSettingDialog::readSettings(const QString &filepath)
{
    applySettings(Input2DViewSettings::readSettings(filepath));
}
} // namespace post3dapp

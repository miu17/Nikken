#include "operation_editall_widget.h"

#include <QtGui>

#include "elided_label.h"
#include "nameselect_dialog.h"
#include "operation_widget_parts.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "vhline.h"

namespace post3dapp{
OperationEditAllWidget::OperationEditAllWidget( QWidget *parent )
    : QWidget( parent )
{
    limitDialog = new LimitSelectDialog(this);

    limitCheckBox = new QCheckBox(u8"選択対象を制限する", this);
    limitButton = new QPushButton(u8"制 限", this);
    limitButton->setFixedSize(50, 20);

    QHBoxLayout *hlay_limit = new QHBoxLayout;
    hlay_limit->addWidget(limitCheckBox);
    hlay_limit->addStretch();
    hlay_limit->addWidget(limitButton);

    limitedLabel = new ElidedLabel("", this);
    limitedLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    limitedLabel->setTextFormat(Qt::AutoText);
    limitedLabel->setMinimumHeight(25);

    connect( limitButton, &QAbstractButton::clicked, this, &OperationEditAllWidget::showLimitDialog );

    QHBoxLayout *hlay_label = new QHBoxLayout;
    hlay_label->addSpacing(12);
    hlay_label->addWidget(limitedLabel);

    limitCheckBox->setChecked(false);
    slotEnableLimitParts(false);
    connect( limitCheckBox, &QAbstractButton::toggled, this, &OperationEditAllWidget::slotEnableLimitParts );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay_limit);
    vlay->addLayout(hlay_label);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addWidget(new SnapJointWidget(this));
    vlay->addSpacing(10);
    vlay->addWidget(new AddMoveWidget(this));
    vlay->addSpacing(10);
    vlay->addWidget(new AddedMovedWidget(this));
    vlay->addSpacing(10);
    vlay->addWidget(new DeleteWidget(this));
    vlay->addStretch();

    this->setLayout(vlay);
}

void OperationEditAllWidget::slotEnableLimitParts(bool on)
{
    limitButton->setEnabled(on);
    limitedLabel->setEnabled(on);

    LIMITSELECTTERM selectTerm = limitDialog->currentLimitSettings();
    selectTerm.isSelectLimited = on;
    UnifiedEditingData::getInstance()->uploadLimitSelectTerm(selectTerm);
}

void OperationEditAllWidget::showLimitDialog()
{
    limitDialog->setCurrentValues();
    if ( limitDialog->exec() ) {
        LIMITSELECTTERM settings = limitDialog->currentLimitSettings();
        QString msg;
        if ( !settings.isSelectLimited ) {
            msg = u8"（制限なし）";
        } else {
            if ( settings.isLimitFloor ) msg += u8"階";
            if ( settings.isLimitFrame ) {
                if ( !msg.isEmpty() ) msg += "/";
                msg += u8"通り";
            }
            if ( settings.isLimitDataType ) {
                if ( !msg.isEmpty() ) msg += "/";
                msg += u8"要素";
            }
            if ( settings.isLimitName ) {
                if ( !msg.isEmpty() ) msg += "/";
                msg += u8"符号";
            }
            msg += u8" の制限あり";
        }
        limitedLabel->setText(msg);
    }
}


/* 制限ダイアログ */
LimitSelectDialog::LimitSelectDialog(QWidget *parent)
    : QDialog(parent)
{
    createLayout();

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle( u8"選択要素の制限" );
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void LimitSelectDialog::setCurrentValues()
{
    limitFloorCheckBox->setChecked(mySettings.isLimitFloor);
    limitFloorAllButton->setChecked(mySettings.isFloorContained);
    limitedFloors.clear();
    Q_FOREACH ( QUuid id, mySettings.limitedFloors ) {
        QString str = UnifiedInputData::getInInstance()->idToName(id, DATATYPE::TPFLOOR);
        if ( !str.isEmpty() ) limitedFloors.append(str);
    }
    QString str = ( limitedFloors.isEmpty() ) ? QString("") : limitedFloors.join(",") ;
    limitFloorLabel->setText(str);

    limitFrameCheckBox->setChecked(mySettings.isLimitFrame);
    limitFrameAllButton->setChecked(mySettings.isFrameContained);
    limitedFrames.clear();
    Q_FOREACH ( QUuid id, mySettings.limitedFrames ) {
        str = UnifiedInputData::getInInstance()->idToName(id, DATATYPE::TPFRAMEP);
        if ( !str.isEmpty() ) limitedFrames.append(str);
    }
    str = ( limitedFrames.isEmpty() ) ? QString("") : limitedFrames.join(",") ;
    limitFrameLabel->setText(str);

    limitDataTypeCheckBox->setChecked(mySettings.isLimitDataType);
    limitedDataTypes = mySettings.limitedDataTypes;
    QStringList strlist;
    Q_FOREACH ( DATATYPE type, limitedDataTypes ) {
        strlist.append( UnifiedInputData::getInInstance()->dataTypeToJapanese(type) );
    }
    str = ( strlist.isEmpty() ) ? QString("") : strlist.join(",") ;
    limitDataTypeLabel->setText(str);

    limitNameCheckBox->setChecked(mySettings.isLimitName);
    str = ( mySettings.limitedNames.isEmpty() ) ? QString("") : mySettings.limitedNames.join(",") ;
    limitNameEdit->setText(str);
}

void LimitSelectDialog::createLayout()
{
    // 階の制限
    limitFloorCheckBox = new QCheckBox(u8"対象階を制限する", this);

    QHBoxLayout *hlay_floorcheck = new QHBoxLayout;
    hlay_floorcheck->addWidget(limitFloorCheckBox);
    hlay_floorcheck->addStretch();

    limitFloorAllButton = new QRadioButton(u8"全体所属", this);
    limitFloorPartButton = new QRadioButton(u8"部分所属", this);
    QButtonGroup *limitFloorGroup = new QButtonGroup;
    limitFloorGroup->addButton(limitFloorAllButton, 0);
    limitFloorGroup->addButton(limitFloorPartButton, 1);
    limitFloorAllButton->setChecked(true);
    limitFloorAllButton->setFixedWidth(120);
    limitFloorPartButton->setFixedWidth(120);

    QHBoxLayout *hlay_floorbutton = new QHBoxLayout;
    hlay_floorbutton->addSpacing(15);
    hlay_floorbutton->addWidget(limitFloorAllButton);
    hlay_floorbutton->addSpacing(10);
    hlay_floorbutton->addWidget(limitFloorPartButton);
    hlay_floorbutton->addStretch();

    limitFloorButton = new QPushButton(u8"対象階", this);
    limitFloorButton->setFixedSize(60, 23);
    limitFloorLabel = new ElidedLabel("", this);
    limitFloorLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    limitFloorLabel->setTextFormat(Qt::AutoText);

    connect( limitFloorButton, &QAbstractButton::clicked, this, &LimitSelectDialog::showSelectFloorDialog );

    QHBoxLayout *hlay_floorlabel = new QHBoxLayout;
    hlay_floorlabel->addSpacing(15);
    hlay_floorlabel->addWidget(limitFloorButton);
    hlay_floorlabel->addWidget(limitFloorLabel);

    limitFloorCheckBox->setChecked(false);
    slotEnableFloorParts(false);
    connect( limitFloorCheckBox, &QAbstractButton::toggled, this, &LimitSelectDialog::slotEnableFloorParts );

    // 通りの制限
    limitFrameCheckBox = new QCheckBox(u8"対象通りを制限する", this);

    QHBoxLayout *hlay_framecheck = new QHBoxLayout;
    hlay_framecheck->addWidget(limitFrameCheckBox);
    hlay_framecheck->addStretch();

    limitFrameAllButton = new QRadioButton(u8"全体所属", this);
    limitFramePartButton = new QRadioButton(u8"部分所属", this);
    QButtonGroup *limitFrameGroup = new QButtonGroup;
    limitFrameGroup->addButton(limitFrameAllButton, 0);
    limitFrameGroup->addButton(limitFramePartButton, 1);
    limitFrameAllButton->setChecked(true);
    limitFrameAllButton->setFixedWidth(120);
    limitFramePartButton->setFixedWidth(120);

    QHBoxLayout *hlay_framebutton = new QHBoxLayout;
    hlay_framebutton->addSpacing(15);
    hlay_framebutton->addWidget(limitFrameAllButton);
    hlay_framebutton->addSpacing(10);
    hlay_framebutton->addWidget(limitFramePartButton);
    hlay_framebutton->addStretch();

    limitFrameButton = new QPushButton(u8"対象通り", this);
    limitFrameButton->setFixedSize(60, 23);
    limitFrameLabel = new ElidedLabel("", this);
    limitFrameLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    limitFrameLabel->setTextFormat(Qt::AutoText);

    connect( limitFrameButton, &QAbstractButton::clicked, this, &LimitSelectDialog::showSelectFrameDialog );

    QHBoxLayout *hlay_framelabel = new QHBoxLayout;
    hlay_framelabel->addSpacing(15);
    hlay_framelabel->addWidget(limitFrameButton);
    hlay_framelabel->addWidget(limitFrameLabel);

    limitFrameCheckBox->setChecked(false);
    slotEnableFrameParts(false);
    connect( limitFrameCheckBox, &QAbstractButton::toggled, this, &LimitSelectDialog::slotEnableFrameParts );

    // 要素の制限
    limitDataTypeCheckBox = new QCheckBox(u8"対象要素を制限する", this);

    QHBoxLayout *hlay_elementcheck = new QHBoxLayout;
    hlay_elementcheck->addWidget(limitDataTypeCheckBox);
    hlay_elementcheck->addStretch();

    limitDataTypeButton = new QPushButton(u8"対象要素", this);
    limitDataTypeButton->setFixedSize(60, 23);
    limitDataTypeLabel = new ElidedLabel("", this);
    limitDataTypeLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    limitDataTypeLabel->setTextFormat(Qt::AutoText);

    connect( limitDataTypeButton, &QAbstractButton::clicked, this, &LimitSelectDialog::showSelectElementDialog );

    QHBoxLayout *hlay_elementlabel = new QHBoxLayout;
    hlay_elementlabel->addSpacing(15);
    hlay_elementlabel->addWidget(limitDataTypeButton);
    hlay_elementlabel->addWidget(limitDataTypeLabel);

    limitDataTypeCheckBox->setChecked(false);
    slotEnableElementParts(false);
    connect( limitDataTypeCheckBox, &QAbstractButton::toggled, this, &LimitSelectDialog::slotEnableElementParts );

    // 符号の制限
    limitNameCheckBox = new QCheckBox(u8"対象符号を制限する（※カンマ区切りで下に列記）", this);

    QHBoxLayout *hlay_namecheck = new QHBoxLayout;
    hlay_namecheck->addWidget(limitNameCheckBox);
    hlay_namecheck->addStretch();

    limitNameEdit = new QLineEdit(this);
    limitNameEdit->setFixedHeight(30);

    QHBoxLayout *hlay_nameedit = new QHBoxLayout;
    hlay_nameedit->addSpacing(15);
    hlay_nameedit->addWidget(limitNameEdit);

    limitNameCheckBox->setChecked(false);
    limitNameEdit->setEnabled(false);
    connect( limitNameCheckBox, &QAbstractButton::toggled, limitNameEdit, &QWidget::setEnabled );

    // OK Cancel
    QPushButton *okButton = new QPushButton(u8"設　定", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &LimitSelectDialog::accept );
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    QHBoxLayout *hlay_okcancel = new QHBoxLayout;
    hlay_okcancel->addStretch();
    hlay_okcancel->addWidget(okButton);
    hlay_okcancel->addWidget(cancelButton);

    /* レイアウト */
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setSpacing(5);
    vlay->addLayout(hlay_floorcheck);
    vlay->addLayout(hlay_floorbutton);
    vlay->addLayout(hlay_floorlabel);
    vlay->addSpacing(10);
    vlay->addLayout(hlay_framecheck);
    vlay->addLayout(hlay_framebutton);
    vlay->addLayout(hlay_framelabel);
    vlay->addSpacing(10);
    vlay->addLayout(hlay_elementcheck);
    vlay->addLayout(hlay_elementlabel);
    vlay->addSpacing(10);
    vlay->addLayout(hlay_namecheck);
    vlay->addLayout(hlay_nameedit);
    vlay->addStretch();
    vlay->addLayout(hlay_okcancel);

    this->setLayout(vlay);
}

void LimitSelectDialog::slotEnableFloorParts(bool onoff)
{
    limitFloorAllButton->setEnabled(onoff);
    limitFloorPartButton->setEnabled(onoff);
    limitFloorButton->setEnabled(onoff);
    limitFloorLabel->setEnabled(onoff);
}

void LimitSelectDialog::slotEnableFrameParts(bool onoff)
{
    limitFrameAllButton->setEnabled(onoff);
    limitFramePartButton->setEnabled(onoff);
    limitFrameButton->setEnabled(onoff);
    limitFrameLabel->setEnabled(onoff);
}

void LimitSelectDialog::slotEnableElementParts(bool onoff)
{
    limitDataTypeButton->setEnabled(onoff);
    limitDataTypeLabel->setEnabled(onoff);
}


void LimitSelectDialog::showSelectFloorDialog()
{
    QStringList fllist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFLOOR);
    NameSelectDialog dialog(fllist, limitedFloors, this);
    dialog.setWindowTitle(u8"対象階の選択");
    if ( dialog.exec() ) {
        limitedFloors = dialog.selectedNames();
        QString str = ( limitedFloors.isEmpty() ) ? QString("") : limitedFloors.join(",") ;
        limitFloorLabel->setText(str);
    }
}

void LimitSelectDialog::showSelectFrameDialog()
{
    QStringList frlist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFRAMEP);
    NameSelectDialog dialog(frlist, limitedFrames, this);
    dialog.setWindowTitle(u8"対象通りの選択");
    if ( dialog.exec() ) {
        limitedFrames = dialog.selectedNames();
        QString str = ( limitedFrames.isEmpty() ) ? QString() : limitedFrames.join(",") ;
        limitFrameLabel->setText(str);
    }
}

void LimitSelectDialog::showSelectElementDialog()
{
    QStringList namelist;
    QStringList selected_names;
    int st_id = static_cast<int>(DATATYPE::TPJOINT);
    int en_id = static_cast<int>(DATATYPE::TPSLABLOAD);
    for ( int i = st_id; i <= en_id ; i++ ) {
        DATATYPE type = static_cast<DATATYPE>(i);
        QString str = UnifiedInputData::getInInstance()->dataTypeToJapanese(type);
        namelist.append(str);
        if ( limitedDataTypes.contains(type) ) selected_names.append(str);
    }

    NameSelectDialog dialog(namelist, selected_names, this);
    dialog.setWindowTitle(u8"対象要素の選択");
    if ( dialog.exec() ) {
        selected_names = dialog.selectedNames();
        QString str = ( selected_names.isEmpty() ) ? QString() : selected_names.join(",") ;
        limitDataTypeLabel->setText(str);
        limitedDataTypes.clear();
        Q_FOREACH ( QString name, selected_names ) {
            int idx = namelist.indexOf(name);
            DATATYPE type = static_cast<DATATYPE>(idx + st_id);
            limitedDataTypes.append(type);
        }
    }
}

void LimitSelectDialog::accept()
{
    bool islimitfloor = limitFloorCheckBox->isChecked();
    bool isfloorcontain = limitFloorAllButton->isChecked();
    QList<QUuid> floors;
    Q_FOREACH ( QString str, limitedFloors ) {
        QUuid id = UnifiedInputData::getInInstance()->nameToID(str, DATATYPE::TPFLOOR);
        if ( !id.isNull() ) floors.append(id);
    }

    bool islimitframe = limitFrameCheckBox->isChecked();
    bool isframecontain = limitFrameAllButton->isChecked();
    QList<QUuid> frames;
    Q_FOREACH ( QString str, limitedFrames ) {
        QUuid id = UnifiedInputData::getInInstance()->nameToID(str, DATATYPE::TPFRAMEP);
        if ( !id.isNull() ) frames.append(id);
    }

    bool islimitdata = limitDataTypeCheckBox->isChecked();
    bool islimitname = limitNameCheckBox->isChecked();
    QString str = limitNameEdit->text();
    QStringList limitnames = ( !str.isEmpty() ) ? str.split(',') : QStringList();

    bool isSelectLimited = ( islimitfloor || islimitframe || islimitdata || islimitname );
    mySettings = LIMITSELECTTERM(isSelectLimited,
                                 islimitfloor, isfloorcontain, floors,
                                 islimitframe, isframecontain, frames,
                                 islimitdata, limitedDataTypes, islimitname, limitnames);

    UnifiedEditingData::getInstance()->uploadLimitSelectTerm(mySettings);
    QDialog::accept();
}
} // namespace post3dapp

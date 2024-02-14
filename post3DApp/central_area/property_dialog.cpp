#include "property_dialog.h"

#include <QtGui>
#include <QtWidgets>

#include "fixed_data.h"
#include "manage_memberdata.h"
#include "manage_jointdata.h"
#include "unified_analysisdata.h"
#include "unified_editingdata.h"
#include "vhline.h"

namespace post3dapp{

PropertyDialog::PropertyDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setMinimumSize(100, 100);
    myUnifiedData = UnifiedInputData::getInInstance();

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(pal);

    createDefineCountProperty();
    createElementCountProperty();
    createSelectElementProperty();

    i_jointWidget->setVisible(false);
    i_colWidget->setVisible(false);
    i_girWidget->setVisible(false);
    i_braWidget->setVisible(false);
    i_beamWidget->setVisible(false);
    i_slabWidget->setVisible(false);
    i_wallWidget->setVisible(false);
    i_uwalWidget->setVisible(false);
    i_dampWidget->setVisible(false);
    i_isoWidget->setVisible(false);
    i_jloadWidget->setVisible(false);
    i_mloadWidget->setVisible(false);
    i_sloadWidget->setVisible(false);

    s_jointWidget->setVisible(false);
    s_colWidget->setVisible(false);
    s_girWidget->setVisible(false);
    s_braWidget->setVisible(false);
    s_beamWidget->setVisible(false);
    s_slabWidget->setVisible(false);
    s_wallWidget->setVisible(false);
    s_uwalWidget->setVisible(false);
    s_dampWidget->setVisible(false);
    s_isoWidget->setVisible(false);
    s_jloadWidget->setVisible(false);
    s_mloadWidget->setVisible(false);
    s_sloadWidget->setVisible(false);

    tabWidget = new QTabWidget(this);
    tabWidget->setFont(QFont(tr("Microsoft JhengHei"), 9));
    tabWidget->addTab(defineCountProperty, u8"定  義");
    tabWidget->addTab(elementCountProperty, u8"入力要素");
    tabWidget->addTab(selectElementProperty, u8"選択要素");

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->addWidget(tabWidget);
    this->setLayout(vlay);

    this->setWindowTitle(u8"プロパティ");
    this->setWindowIcon(QIcon(QPixmap(":/icons/property.png")));
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(380, 480));

    connect(UnifiedInputData::getInInstance(), &UnifiedData::dataUpdated,
            this, &PropertyDialog::resetDefinePropertyContents);
    connect(UnifiedInputData::getInInstance(), &UnifiedData::elementCountChanged,
            this, &PropertyDialog::resetElementPropertyContents);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::activeContentsChanged,
            this, &PropertyDialog::resetSelectPropertyContents);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData:: activeMdiWindowTypeChanged,
            this, &PropertyDialog::changeMode);
}

void PropertyDialog::changeMode(MDITYPE type)
{
switch(type){
case MDITYPE::MDIINPUT:
    tabWidget->clear();
    tabWidget->addTab(defineCountProperty, u8"定  義");
    tabWidget->addTab(elementCountProperty, u8"入力要素");
    tabWidget->addTab(selectElementProperty, u8"選択要素");
    myUnifiedData = UnifiedInputData::getInInstance();
    break;
case MDITYPE::MDIOUTPUT:
    tabWidget->clear();
    tabWidget->addTab(selectElementProperty, u8"選択要素");
    myUnifiedData = UnifiedFixedData::getInstance()->getAnalysisData();
    break;
case MDITYPE::MDICALCLOAD:
    tabWidget->clear();
    tabWidget->addTab(defineCountProperty, u8"定  義");
    tabWidget->addTab(elementCountProperty, u8"入力要素");
    tabWidget->addTab(selectElementProperty, u8"選択要素");
    myUnifiedData = UnifiedFixedData::getInstance()->getInputData();
    break;
case MDITYPE::MDICOPY:
    tabWidget->clear();
    tabWidget->addTab(defineCountProperty, u8"定  義");
    tabWidget->addTab(elementCountProperty, u8"入力要素");
    tabWidget->addTab(selectElementProperty, u8"選択要素");
    myUnifiedData = UnifiedFixedData::getInstance()->getInputData();
    break;
default:
    break;
}
return;
}

void PropertyDialog::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    QDialog::closeEvent(event);
}

void PropertyDialog::createDefineCountProperty()
{
    QLabel *d_flLabel = new QLabel(u8"階:", this);
    d_flLabel->setFixedWidth(100);
    d_flLine = new QLineEdit(this);
    d_flLine->setAlignment(Qt::AlignLeft);
    d_flLine->setReadOnly(true);
    d_flLine->setText(u8"0個");
    QHBoxLayout *hlay_fl = new QHBoxLayout;
    hlay_fl->addWidget(d_flLabel);
    hlay_fl->addWidget(d_flLine, 1);
    /*
        QLabel *d_frgLabel=new QLabel(tr("通りグループ:"),this);
        d_frgLabel->setFixedWidth(100);
        d_frgLine=new QLineEdit(this);
        d_frgLine->setAlignment(Qt::AlignLeft);
        d_frgLine->setReadOnly(true);
        d_frgLine->setText(tr("0個"));
        QHBoxLayout *hlay_frg=new QHBoxLayout;
        hlay_frg->addWidget(d_frgLabel);
        hlay_frg->addWidget(d_frgLine,1);
    */
    QLabel *d_frpLabel = new QLabel(u8"通り:", this);
    d_frpLabel->setFixedWidth(100);
    d_frpLine = new QLineEdit(this);
    d_frpLine->setAlignment(Qt::AlignLeft);
    d_frpLine->setReadOnly(true);
    d_frpLine->setText(u8"0個");
    QHBoxLayout *hlay_frp = new QHBoxLayout;
    hlay_frp->addWidget(d_frpLabel);
    hlay_frp->addWidget(d_frpLine, 1);
    /*
        QLabel *d_maLabel=new QLabel(tr("材料リスト:"),this);
        d_maLabel->setFixedWidth(100);
        d_maLine=new QLineEdit(this);
        d_maLine->setAlignment(Qt::AlignLeft);
        d_maLine->setReadOnly(true);
        d_maLine->setText(tr("0個"));
        QHBoxLayout *hlay_ma=new QHBoxLayout;
        hlay_ma->addWidget(d_maLabel);
        hlay_ma->addWidget(d_maLine,1);
    */
    QLabel *d_macLabel = new QLabel(u8"コンクリート材種:", this);
    d_macLabel->setFixedWidth(100);
    d_macLine = new QLineEdit(this);
    d_macLine->setAlignment(Qt::AlignLeft);
    d_macLine->setReadOnly(true);
    d_macLine->setText(u8"0個");
    QHBoxLayout *hlay_mac = new QHBoxLayout;
    hlay_mac->addWidget(d_macLabel);
    hlay_mac->addWidget(d_macLine, 1);

    QLabel *d_masLabel = new QLabel(u8"鉄骨材種:", this);
    d_masLabel->setFixedWidth(100);
    d_masLine = new QLineEdit(this);
    d_masLine->setAlignment(Qt::AlignLeft);
    d_masLine->setReadOnly(true);
    d_masLine->setText(u8"0個");
    QHBoxLayout *hlay_mas = new QHBoxLayout;
    hlay_mas->addWidget(d_masLabel);
    hlay_mas->addWidget(d_masLine, 1);

    QLabel *d_marLabel = new QLabel(u8"鉄筋材種:", this);
    d_marLabel->setFixedWidth(100);
    d_marLine = new QLineEdit(this);
    d_marLine->setAlignment(Qt::AlignLeft);
    d_marLine->setReadOnly(true);
    d_marLine->setText(u8"0個");
    QHBoxLayout *hlay_mar = new QHBoxLayout;
    hlay_mar->addWidget(d_marLabel);
    hlay_mar->addWidget(d_marLine, 1);

    QLabel *d_colLabel = new QLabel(u8"柱リスト:", this);
    d_colLabel->setFixedWidth(100);
    d_colLine = new QLineEdit(this);
    d_colLine->setAlignment(Qt::AlignLeft);
    d_colLine->setReadOnly(true);
    d_colLine->setText(u8"0個");
    QHBoxLayout *hlay_col = new QHBoxLayout;
    hlay_col->addWidget(d_colLabel);
    hlay_col->addWidget(d_colLine, 1);

    QLabel *d_girLabel = new QLabel(u8"大梁リスト:", this);
    d_girLabel->setFixedWidth(100);
    d_girLine = new QLineEdit(this);
    d_girLine->setAlignment(Qt::AlignLeft);
    d_girLine->setReadOnly(true);
    d_girLine->setText(u8"0個");
    QHBoxLayout *hlay_gir = new QHBoxLayout;
    hlay_gir->addWidget(d_girLabel);
    hlay_gir->addWidget(d_girLine, 1);

    QLabel *d_braLabel = new QLabel(u8"ブレースリスト:", this);
    d_braLabel->setFixedWidth(100);
    d_braLine = new QLineEdit(this);
    d_braLine->setAlignment(Qt::AlignLeft);
    d_braLine->setReadOnly(true);
    d_braLine->setText(u8"0個");
    QHBoxLayout *hlay_bra = new QHBoxLayout;
    hlay_bra->addWidget(d_braLabel);
    hlay_bra->addWidget(d_braLine, 1);

    QLabel *d_beamLabel = new QLabel(u8"小梁リスト:", this);
    d_beamLabel->setFixedWidth(100);
    d_beamLine = new QLineEdit(this);
    d_beamLine->setAlignment(Qt::AlignLeft);
    d_beamLine->setReadOnly(true);
    d_beamLine->setText(u8"0個");
    QHBoxLayout *hlay_beam = new QHBoxLayout;
    hlay_beam->addWidget(d_beamLabel);
    hlay_beam->addWidget(d_beamLine, 1);

    QLabel *d_slabLabel = new QLabel(u8"スラブリスト:", this);
    d_slabLabel->setFixedWidth(100);
    d_slabLine = new QLineEdit(this);
    d_slabLine->setAlignment(Qt::AlignLeft);
    d_slabLine->setReadOnly(true);
    d_slabLine->setText(u8"0個");
    QHBoxLayout *hlay_slab = new QHBoxLayout;
    hlay_slab->addWidget(d_slabLabel);
    hlay_slab->addWidget(d_slabLine, 1);

    QLabel *d_wallLabel = new QLabel(u8"壁リスト:", this);
    d_wallLabel->setFixedWidth(100);
    d_wallLine = new QLineEdit(this);
    d_wallLine->setAlignment(Qt::AlignLeft);
    d_wallLine->setReadOnly(true);
    d_wallLine->setText(u8"0個");
    QHBoxLayout *hlay_wall = new QHBoxLayout;
    hlay_wall->addWidget(d_wallLabel);
    hlay_wall->addWidget(d_wallLine, 1);

    QLabel *d_uwalLabel = new QLabel(u8"地下外壁リスト:", this);
    d_uwalLabel->setFixedWidth(100);
    d_uwalLine = new QLineEdit(this);
    d_uwalLine->setAlignment(Qt::AlignLeft);
    d_uwalLine->setReadOnly(true);
    d_uwalLine->setText(u8"0個");
    QHBoxLayout *hlay_uwall = new QHBoxLayout;
    hlay_uwall->addWidget(d_uwalLabel);
    hlay_uwall->addWidget(d_uwalLine, 1);

    QLabel *d_wopenLabel = new QLabel(u8"開口リスト:", this);
    d_wopenLabel->setFixedWidth(100);
    d_wopenLine = new QLineEdit(this);
    d_wopenLine->setAlignment(Qt::AlignLeft);
    d_wopenLine->setReadOnly(true);
    d_wopenLine->setText(u8"0個");
    QHBoxLayout *hlay_wopen = new QHBoxLayout;
    hlay_wopen->addWidget(d_wopenLabel);
    hlay_wopen->addWidget(d_wopenLine, 1);

    QLabel *d_baseLabel = new QLabel(u8"柱脚リスト:", this);
    d_baseLabel->setFixedWidth(100);
    d_baseLine = new QLineEdit(this);
    d_baseLine->setAlignment(Qt::AlignLeft);
    d_baseLine->setReadOnly(true);
    d_baseLine->setText(u8"0個");
    QHBoxLayout *hlay_base = new QHBoxLayout;
    hlay_base->addWidget(d_baseLabel);
    hlay_base->addWidget(d_baseLine, 1);

    QLabel *d_dampLabel = new QLabel(u8"制振要素リスト:", this);
    d_dampLabel->setFixedWidth(100);
    d_dampLine = new QLineEdit(this);
    d_dampLine->setAlignment(Qt::AlignLeft);
    d_dampLine->setReadOnly(true);
    d_dampLine->setText(u8"0個");
    QHBoxLayout *hlay_damp = new QHBoxLayout;
    hlay_damp->addWidget(d_dampLabel);
    hlay_damp->addWidget(d_dampLine, 1);

    QLabel *d_isoLabel = new QLabel(u8"免震要素リスト:", this);
    d_isoLabel->setFixedWidth(100);
    d_isoLine = new QLineEdit(this);
    d_isoLine->setAlignment(Qt::AlignLeft);
    d_isoLine->setReadOnly(true);
    d_isoLine->setText(u8"0個");
    QHBoxLayout *hlay_iso = new QHBoxLayout;
    hlay_iso->addWidget(d_isoLabel);
    hlay_iso->addWidget(d_isoLine, 1);

    QLabel *d_jloadLabel = new QLabel(u8"節点荷重リスト:", this);
    d_jloadLabel->setFixedWidth(100);
    d_jloadLine = new QLineEdit(this);
    d_jloadLine->setAlignment(Qt::AlignLeft);
    d_jloadLine->setReadOnly(true);
    d_jloadLine->setText(u8"0個");
    QHBoxLayout *hlay_jload = new QHBoxLayout;
    hlay_jload->addWidget(d_jloadLabel);
    hlay_jload->addWidget(d_jloadLine, 1);

    QLabel *d_mloadLabel = new QLabel(u8"部材荷重リスト:", this);
    d_mloadLabel->setFixedWidth(100);
    d_mloadLine = new QLineEdit(this);
    d_mloadLine->setAlignment(Qt::AlignLeft);
    d_mloadLine->setReadOnly(true);
    d_mloadLine->setText(u8"0個");
    QHBoxLayout *hlay_mload = new QHBoxLayout;
    hlay_mload->addWidget(d_mloadLabel);
    hlay_mload->addWidget(d_mloadLine, 1);

    QLabel *d_sloadLabel = new QLabel(u8"床荷重リスト:", this);
    d_sloadLabel->setFixedWidth(100);
    d_sloadLine = new QLineEdit(this);
    d_sloadLine->setAlignment(Qt::AlignLeft);
    d_sloadLine->setReadOnly(true);
    d_sloadLine->setText(u8"0個");
    QHBoxLayout *hlay_sload = new QHBoxLayout;
    hlay_sload->addWidget(d_sloadLabel);
    hlay_sload->addWidget(d_sloadLine, 1);


    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(2);
    vlay->addLayout(hlay_fl);
//    vlay->addLayout(hlay_frg);
    vlay->addLayout(hlay_frp);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
//    vlay->addLayout(hlay_ma);
    vlay->addLayout(hlay_mac);
    vlay->addLayout(hlay_mas);
    vlay->addLayout(hlay_mar);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_col);
    vlay->addLayout(hlay_gir);
    vlay->addLayout(hlay_bra);
    vlay->addLayout(hlay_beam);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_slab);
    vlay->addLayout(hlay_wall);
    vlay->addLayout(hlay_uwall);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_wopen);
    vlay->addLayout(hlay_base);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_damp);
    vlay->addLayout(hlay_iso);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay_jload);
    vlay->addLayout(hlay_mload);
    vlay->addLayout(hlay_sload);
    vlay->addStretch();

    defineCountProperty = new QWidget(this);
    defineCountProperty->setLayout(vlay);
}

void PropertyDialog::createElementCountProperty()
{
    i_jointWidget = new PropertyInputWidget(u8"節点", this);
    i_colWidget = new PropertyInputWidget(u8"柱", this);
    i_girWidget = new PropertyInputWidget(u8"大梁", this);
    i_braWidget = new PropertyInputWidget(u8"ブレース", this);
    i_beamWidget = new PropertyInputWidget(u8"小梁", this);
    i_slabWidget = new PropertyInputWidget(u8"スラブ", this);
    i_wallWidget = new PropertyInputWidget(u8"壁", this);
    i_uwalWidget = new PropertyInputWidget(u8"地下外壁", this);
    i_dampWidget = new PropertyInputWidget(u8"制振要素", this);
    i_isoWidget = new PropertyInputWidget(u8"免震要素", this);
    i_jloadWidget = new PropertyInputWidget(u8"節点荷重", this);
    i_mloadWidget = new PropertyInputWidget(u8"部材荷重", this);
    i_sloadWidget = new PropertyInputWidget(u8"床荷重", this);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(2);
    vlay->addWidget(i_jointWidget);
    vlay->addWidget(i_colWidget);
    vlay->addWidget(i_girWidget);
    vlay->addWidget(i_braWidget);
    vlay->addWidget(i_beamWidget);
    vlay->addWidget(i_slabWidget);
    vlay->addWidget(i_wallWidget);
    vlay->addWidget(i_uwalWidget);
    vlay->addWidget(i_dampWidget);
    vlay->addWidget(i_isoWidget);
    vlay->addWidget(i_jloadWidget);
    vlay->addWidget(i_mloadWidget);
    vlay->addWidget(i_sloadWidget);
    vlay->addStretch();

    elementCountProperty = new QWidget(this);
    elementCountProperty->setLayout(vlay);
}

void PropertyDialog::createSelectElementProperty()
{
    s_jointWidget = new PropertySelectWidget(u8"節点", this);
    s_colWidget = new PropertySelectWidget(u8"柱", this);
    s_girWidget = new PropertySelectWidget(u8"大梁", this);
    s_braWidget = new PropertySelectWidget(u8"ブレース", this);
    s_beamWidget = new PropertySelectWidget(u8"小梁", this);
    s_slabWidget = new PropertySelectWidget(u8"スラブ", this);
    s_wallWidget = new PropertySelectWidget(u8"壁", this);
    s_uwalWidget = new PropertySelectWidget(u8"地下外壁", this);
    s_dampWidget = new PropertySelectWidget(u8"制振要素", this);
    s_isoWidget = new PropertySelectWidget(u8"免震要素", this);
    s_jloadWidget = new PropertySelectWidget(u8"節点荷重", this);
    s_mloadWidget = new PropertySelectWidget(u8"部材荷重", this);
    s_sloadWidget = new PropertySelectWidget(u8"床荷重", this);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(2);
    vlay->addWidget(s_jointWidget);
    vlay->addWidget(s_colWidget);
    vlay->addWidget(s_girWidget);
    vlay->addWidget(s_braWidget);
    vlay->addWidget(s_beamWidget);
    vlay->addWidget(s_slabWidget);
    vlay->addWidget(s_wallWidget);
    vlay->addWidget(s_uwalWidget);
    vlay->addWidget(s_dampWidget);
    vlay->addWidget(s_isoWidget);
    vlay->addWidget(s_jloadWidget);
    vlay->addWidget(s_mloadWidget);
    vlay->addWidget(s_sloadWidget);
    vlay->addStretch(2);

    selectElementProperty = new QWidget(this);
    selectElementProperty->setLayout(vlay);
}

void PropertyDialog::resetDefinePropertyContents(DATATYPE dtype)
{
    int count = 0;
    QString str;

    switch ( dtype ) {

    case DATATYPE::TPFLOOR:
        count = myUnifiedData->getDataCount(DATATYPE::TPFLOOR);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPFLOOR).join(",");
        d_flLine->setText(str);
        d_flLine->setCursorPosition(0);
        break;
    /*
        case TPFRAMEG:
            count=FactoryUnifiedData::getInstance(isOutput)->getDataCount(TPFRAMEG);
            str=QString(tr("%1個")).arg(count);
            if( count>0 ) str+=";"+FactoryUnifiedData::getInstance(isOutput)->getNameList(TPFRAMEG).join(",");
            d_frgLine->setText(str);
            d_frgLine->setCursorPosition(0);
            break;
    */
    case DATATYPE::TPFRAMEP:
        count = myUnifiedData->getDataCount(DATATYPE::TPFRAMEP);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPFRAMEP).join(",");
        d_frpLine->setText(str);
        d_frpLine->setCursorPosition(0);
        break;
    case DATATYPE::TPMACONC:
        count = myUnifiedData->getDataCount(DATATYPE::TPMACONC);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPMACONC).join(",");
        d_macLine->setText(str);
        d_macLine->setCursorPosition(0);
        break;

    case DATATYPE::TPMASTEEL:
        count = myUnifiedData->getDataCount(DATATYPE::TPMASTEEL);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPMASTEEL).join(",");
        d_masLine->setText(str);
        d_masLine->setCursorPosition(0);
        break;

    case DATATYPE::TPMAREIN:
        count = myUnifiedData->getDataCount(DATATYPE::TPMAREIN);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getReinforceNameList().join(",");
        d_marLine->setText(str);
        d_marLine->setCursorPosition(0);
        break;

    case DATATYPE::TPCOLUMN:
        count = myUnifiedData->getDataCount(DATATYPE::TPCOLUMN, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPCOLUMN).join(",");
        d_colLine->setText(str);
        d_colLine->setCursorPosition(0);
        break;

    case DATATYPE::TPGIRDER:
        count = myUnifiedData->getDataCount(DATATYPE::TPGIRDER, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPGIRDER).join(",");
        d_girLine->setText(str);
        d_girLine->setCursorPosition(0);
        break;

    case DATATYPE::TPBRACE:
        count = myUnifiedData->getDataCount(DATATYPE::TPBRACE, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPBRACE).join(",");
        d_braLine->setText(str);
        d_braLine->setCursorPosition(0);
        break;

    case DATATYPE::TPBEAM:
        count = myUnifiedData->getDataCount(DATATYPE::TPBEAM, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPBEAM).join(",");
        d_beamLine->setText(str);
        d_beamLine->setCursorPosition(0);
        break;

    case DATATYPE::TPSLAB:
        count = myUnifiedData->getDataCount(DATATYPE::TPSLAB, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPSLAB).join(",");
        d_slabLine->setText(str);
        d_slabLine->setCursorPosition(0);
        break;

    case DATATYPE::TPWALL:
        count = myUnifiedData->getDataCount(DATATYPE::TPWALL, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPWALL).join(",");
        d_wallLine->setText(str);
        d_wallLine->setCursorPosition(0);
        break;
    case DATATYPE::TPUWALL:
        count = myUnifiedData->getDataCount(DATATYPE::TPUWALL, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPUWALL).join(",");
        d_uwalLine->setText(str);
        d_uwalLine->setCursorPosition(0);
        break;

    case DATATYPE::TPWALLOPEN:
        count = myUnifiedData->getDataCount(DATATYPE::TPWALLOPEN, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPWALLOPEN).join(",");
        d_wopenLine->setText(str);
        d_wopenLine->setCursorPosition(0);
        break;

    case DATATYPE::TPBASE:
        count = myUnifiedData->getDataCount(DATATYPE::TPBASE, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPBASE).join(",");
        d_baseLine->setText(str);
        d_baseLine->setCursorPosition(0);
        break;

    case DATATYPE::TPDAMPER:
        count = myUnifiedData->getDataCount(DATATYPE::TPDAMPER, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPDAMPER).join(",");
        d_dampLine->setText(str);
        d_dampLine->setCursorPosition(0);
        break;

    case DATATYPE::TPISO:
        count = myUnifiedData->getDataCount(DATATYPE::TPISO, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPISO).join(",");
        d_isoLine->setText(str);
        d_isoLine->setCursorPosition(0);
        break;

    case DATATYPE::TPJOINTLOAD:
        count = myUnifiedData->getDataCount(DATATYPE::TPJOINTLOAD, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPJOINTLOAD).join(",");
        d_jloadLine->setText(str);
        d_jloadLine->setCursorPosition(0);
        break;

    case DATATYPE::TPMEMBERLOAD:
        count = myUnifiedData->getDataCount(DATATYPE::TPMEMBERLOAD, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPMEMBERLOAD).join(",");
        d_mloadLine->setText(str);
        d_mloadLine->setCursorPosition(0);
        break;

    case DATATYPE::TPSLABLOAD:
        count = myUnifiedData->getDataCount(DATATYPE::TPSLABLOAD, true);
        str = QString(u8"%1個").arg(count);
        if ( count > 0 ) str += ";" + myUnifiedData->getNameList(DATATYPE::TPSLABLOAD).join(",");
        d_sloadLine->setText(str);
        d_sloadLine->setCursorPosition(0);
        break;

    default:
        break;

    }

    tabWidget->setCurrentIndex(0);
}

void PropertyDialog::resetElementPropertyContents()
{
    setInputCountText(DATATYPE::TPJOINT);
    setInputCountText(DATATYPE::TPCOLUMN);
    setInputCountText(DATATYPE::TPGIRDER);
    setInputCountText(DATATYPE::TPBRACE);
    setInputCountText(DATATYPE::TPBEAM);
    setInputCountText(DATATYPE::TPSLAB);
    setInputCountText(DATATYPE::TPWALL);
    setInputCountText(DATATYPE::TPUWALL);
    setInputCountText(DATATYPE::TPDAMPER);
    setInputCountText(DATATYPE::TPISO);
    setInputCountText(DATATYPE::TPJOINTLOAD);
    setInputCountText(DATATYPE::TPMEMBERLOAD);
    setInputCountText(DATATYPE::TPSLABLOAD);

    tabWidget->setCurrentIndex(1);
}

void PropertyDialog::setInputCountText(DATATYPE dtype)
{
    PropertyInputWidget *myWidget = NULL;
    switch (dtype) {
    case DATATYPE::TPJOINT:
        myWidget = i_jointWidget;
        break;
    case DATATYPE::TPCOLUMN:
        myWidget = i_colWidget;
        break;
    case DATATYPE::TPGIRDER:
        myWidget = i_girWidget;
        break;
    case DATATYPE::TPBRACE:
        myWidget = i_braWidget;
        break;
    case DATATYPE::TPBEAM:
        myWidget = i_beamWidget;
        break;
    case DATATYPE::TPSLAB:
        myWidget = i_slabWidget;
        break;
    case DATATYPE::TPWALL:
        myWidget = i_wallWidget;
        break;
    case DATATYPE::TPUWALL:
        myWidget = i_uwalWidget;
        break;
    case DATATYPE::TPDAMPER:
        myWidget = i_dampWidget;
        break;
    case DATATYPE::TPISO:
        myWidget = i_isoWidget;
        break;
    case DATATYPE::TPJOINTLOAD:
        myWidget = i_jloadWidget;
        break;
    case DATATYPE::TPMEMBERLOAD:
        myWidget = i_mloadWidget;
        break;
    case DATATYPE::TPSLABLOAD:
        myWidget = i_sloadWidget;
        break;
    default:
        break;
    }
    if ( myWidget == NULL ) return;

    int tcount = myUnifiedData->getDataCount(dtype);
    if ( tcount < 1 ) {
        myWidget->setVisible(false);
        return;
    }
    myWidget->setVisible(true);

    QString str = QString(u8"計%1個").arg(tcount);

    if ( dtype == DATATYPE::TPJOINT ) {
        myWidget->setDetailText(str);
        return;
    }

    QHash<QString, int> countTable = myUnifiedData->getMemberSectionCount(dtype);
    QStringList nameList = myUnifiedData->getNameList(dtype);
    if ( countTable.contains("(dummy)") ) {
        int cnt = countTable.value("(dummy)");
        if ( cnt > 0 ) str += "," + QString(u8"ダミー:%1個").arg(cnt);
    }
    Q_FOREACH ( QString name, nameList ) {
        int cnt = ( countTable.contains(name) ) ? countTable.value(name) : 0 ;
        if ( cnt > 0 ) str += "," + name + QString(u8":%1個").arg(cnt);
    }

    myWidget->setDetailText(str);
}

void PropertyDialog::resetSelectPropertyContents()
{
    if ( !UnifiedEditingData::getInstance()->isPropertyEnable() ) return;

    QList<JointData *> jointList = UnifiedEditingData::getInstance()->getActiveJoints();
    QList<MemberData *> memberList = UnifiedEditingData::getInstance()->getActiveMembers();

    QList<MemberData *> col_list;
    QList<MemberData *> gir_list;
    QList<MemberData *> bra_list;
    QList<MemberData *> bea_list;
    QList<MemberData *> sla_list;
    QList<MemberData *> wal_list;
    QList<MemberData *> uwa_list;
    QList<MemberData *> dam_list;
    QList<MemberData *> iso_list;
    QList<MemberData *> jlo_list;
    QList<MemberData *> mlo_list;
    QList<MemberData *> slo_list;

    Q_FOREACH ( MemberData *md, memberList ) {
        switch ( md->dataType() ) {
        case DATATYPE::TPCOLUMN:
            col_list.append(md);
            break;
        case DATATYPE::TPGIRDER:
            gir_list.append(md);
            break;
        case DATATYPE::TPBRACE:
            bra_list.append(md);
            break;
        case DATATYPE::TPBEAM:
            bea_list.append(md);
            break;
        case DATATYPE::TPSLAB:
            sla_list.append(md);
            break;
        case DATATYPE::TPWALL:
            wal_list.append(md);
            break;
        case DATATYPE::TPUWALL:
            uwa_list.append(md);
            break;
        case DATATYPE::TPDAMPER:
            dam_list.append(md);
            break;
        case DATATYPE::TPISO:
            iso_list.append(md);
            break;
        case DATATYPE::TPJOINTLOAD:
            jlo_list.append(md);
            break;
        case DATATYPE::TPMEMBERLOAD:
            mlo_list.append(md);
            break;
        case DATATYPE::TPSLABLOAD:
            slo_list.append(md);
            break;
        default:
            break;
        }
    }

    setJointProperty(jointList);
    setLineMemberProperty(DATATYPE::TPCOLUMN, col_list);
    setLineMemberProperty(DATATYPE::TPGIRDER, gir_list);
    setLineMemberProperty(DATATYPE::TPBRACE, bra_list);
    setLineMemberProperty(DATATYPE::TPBEAM, bea_list);
    setLineMemberProperty(DATATYPE::TPDAMPER, dam_list);
    setLineMemberProperty(DATATYPE::TPISO, iso_list);
    setPlaneMemberProperty(DATATYPE::TPSLAB, sla_list);
    setPlaneMemberProperty(DATATYPE::TPWALL, wal_list);
    setPlaneMemberProperty(DATATYPE::TPUWALL, uwa_list);
    setLoadMemberProperty(DATATYPE::TPJOINTLOAD, jlo_list);
    setLoadMemberProperty(DATATYPE::TPMEMBERLOAD, mlo_list);
    setLoadMemberProperty(DATATYPE::TPSLABLOAD, slo_list);

    if ( jointList.count() + memberList.count() > 0 ){
        tabWidget->setCurrentIndex(2);
    }else{
        tabWidget->setCurrentIndex(0);
    }
}

void PropertyDialog::setJointProperty( const QList<JointData *> &jdlist )
{
    int count = jdlist.count();
    QString str_outline = QString(u8"計%1個").arg(count);
    if ( count <= 20 ) {
        Q_FOREACH ( JointData *jd, jdlist ) {
            str_outline += QString(",No.%1").arg(myUnifiedData->indexOfJoint(jd) + 1);
        }
    }
    if ( count < 1 ) {
        s_jointWidget->clearText();
        s_jointWidget->setVisible(false);
        return;
    }
    s_jointWidget->setCountText(str_outline);

    QString str_detail;
    JointData *jfirst = jdlist.first();
    qreal xc = jfirst->xPos();
    qreal yc = jfirst->yPos();
    qreal zc = jfirst->zPos();
    QList<QUuid> fllist = jfirst->getAttachedFloorList();
    QList<QUuid> frlist = jfirst->getAttachedFrameList();
    bool isNoFloor = fllist.isEmpty();
    bool isNoFrame = frlist.isEmpty();
    bool xok = true, yok = true, zok = true;
    qreal eps = 1.0e-3;

    Q_FOREACH (JointData *jd, jdlist) {
        if ( jd == jfirst ) continue;
        if ( xok ) xok = ( qAbs( xc - jd->xPos() ) < eps ) ;
        if ( yok ) yok = ( qAbs( yc - jd->yPos() ) < eps ) ;
        if ( zok ) zok = ( qAbs( zc - jd->zPos() ) < eps ) ;
        if ( isNoFloor ) {
            isNoFloor = jd->getAttachedFloorList().isEmpty();
        } else if ( !fllist.isEmpty() ) {
            Q_FOREACH (QUuid id, fllist) {
                if ( !jd->getAttachedFloorList().contains(id) ) {
                    fllist.removeOne(id);
                }
            }
        }
        if ( isNoFrame ) {
            isNoFrame = jd->getAttachedFrameList().isEmpty();
        } else if ( !frlist.isEmpty() ) {
            Q_FOREACH (QUuid id, frlist) {
                if ( !jd->getAttachedFrameList().contains(id) ) {
                    frlist.removeOne(id);
                }
            }
        }
        if ( !xok && !yok && !zok && fllist.isEmpty() && frlist.isEmpty() ) break;
    }

    str_detail += u8"X座標:";
    str_detail += ( xok ) ? QString("%1m").arg(xc, 0, 'f', 3) : u8"(不定)" ;
    str_detail += u8", Y座標:";
    str_detail += ( yok ) ? QString("%1m").arg(yc, 0, 'f', 3) : u8"(不定)" ;
    str_detail += u8", Z座標:";
    str_detail += ( zok ) ? QString("%1m").arg(zc, 0, 'f', 3) : u8"(不定)" ;
    str_detail += ",\n";

    str_detail += u8"所属階:";
    if ( isNoFloor ) {
        str_detail += u8"(無し),";
    } else if ( fllist.isEmpty() ) {
        str_detail += u8"(不定),";
    } else {
        Q_FOREACH (QUuid id, fllist) {
            str_detail += myUnifiedData->idToName(id, DATATYPE::TPFLOOR);
            str_detail += ",";
        }
    }
    str_detail += u8"所属階:";
    if ( isNoFrame ) {
        str_detail += u8"(無し),";
    } else if ( frlist.isEmpty() ) {
        str_detail += u8"(不定),";
    } else {
        Q_FOREACH (QUuid id, frlist) {
            str_detail += myUnifiedData->idToName(id, DATATYPE::TPFRAMEP);
            str_detail += ",";
        }
    }
    str_detail += "\n";


    int rl_counter[static_cast<int>(DATATYPE::TPSLABLOAD) + 1] = {0};
    QList<MemberData *> rl_md;
    Q_FOREACH (JointData *jd, jdlist) {
        Q_FOREACH (MemberData *md, jd->getRelatedMembers()) {
            if ( !rl_md.contains(md) ) {
                rl_counter[static_cast<int>(md->dataType())]++;
                rl_md.append(md);
            }
        }
    }

    str_detail += u8"従属要素:";
    if ( rl_counter[static_cast<int>(DATATYPE::TPCOLUMN)] > 0 ) str_detail += QString(u8"柱%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPCOLUMN)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPGIRDER)] > 0 ) str_detail += QString(u8"大梁%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPGIRDER)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPBRACE)] > 0 ) str_detail += QString(u8"ブレース%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPBRACE)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPBEAM)] > 0 ) str_detail += QString(u8"小梁%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPBEAM)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPSLAB)] > 0 ) str_detail += QString(u8"スラブ%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPSLAB)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPWALL)] > 0 ) str_detail += QString(u8"壁%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPWALL)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPUWALL)] > 0 ) str_detail += QString(u8"地下外壁%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPUWALL)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPDAMPER)] > 0 ) str_detail += QString(u8"制振要素%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPDAMPER)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPISO)] > 0 ) str_detail += QString(u8"免震要素%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPISO)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPJOINTLOAD)] > 0 ) str_detail += QString(u8"節点荷重%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPJOINTLOAD)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPMEMBERLOAD)] > 0 ) str_detail += QString(u8"部材荷重%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPMEMBERLOAD)]);
    if ( rl_counter[static_cast<int>(DATATYPE::TPSLABLOAD)] > 0 ) str_detail += QString(u8"床荷重%1個,").arg(rl_counter[static_cast<int>(DATATYPE::TPSLABLOAD)]);

    s_jointWidget->setDetailText(str_detail);
    s_jointWidget->setVisible(true);
}

void PropertyDialog::setLineMemberProperty( DATATYPE dtype, const QList<MemberData *> &mdlist )
{
    qDebug()<<"PropertyDialog::setLineMemberProperty"<<(int)dtype;
    PropertySelectWidget *myWidget;
    if ( dtype == DATATYPE::TPCOLUMN ) {
        myWidget = s_colWidget;
    } else if ( dtype == DATATYPE::TPGIRDER ) {
        myWidget = s_girWidget;
    } else if ( dtype == DATATYPE::TPBRACE ) {
        myWidget = s_braWidget;
    } else if ( dtype == DATATYPE::TPBEAM ) {
        myWidget = s_beamWidget;
    } else if ( dtype == DATATYPE::TPDAMPER ) {
        myWidget = s_dampWidget;
    } else if ( dtype == DATATYPE::TPISO ) {
        myWidget = s_isoWidget;
    } else {
        return;
    }

    int count = mdlist.count();
    QString str_outline = QString(u8"計%1個").arg(count);
    QStringList nameList = myUnifiedData->getNameList(dtype);
    int nameCount = nameList.count();
    int *counter = new int[nameCount + 1];
    for ( int i = 0; i < nameCount + 1; i++ ) counter[i] = 0;
    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( md->sectionName().isEmpty() ) {
            counter[nameCount]++;
            continue;
        }
        int idx = nameList.indexOf(md->sectionName());
        counter[idx]++;
    }
    if ( counter[nameCount] > 0 ) {
        str_outline += QString(u8",ダミー：%1個").arg(counter[nameCount]);
    }
//    QStringList existedNameList;
    for ( int i = 0; i < nameCount; i++ ) {
        if ( counter[i] > 0 ) {
            str_outline += "," + nameList.at(i) + QString(u8":%1個").arg(counter[i]);
//            existedNameList.append( nameList.at(i) );
        }
    }
    delete [] counter;

    if ( count < 1 ) {
        myWidget->clearText();
        myWidget->setVisible(false);
        return;
    }
    myWidget->setCountText(str_outline);

    QString str_detail;
    MemberData *mfirst = mdlist.first();
    // QList<QUuid> fl_uid=mfirst->getAttachedFloorList();
    QUuid fl_uid = mfirst->getAttachedFloor();
    QList<QUuid> frlist = mfirst->getAttachedFrameList();
    //bool isNoFloor=fl_uid.isEmpty();
    bool isNoFloor = fl_uid.isNull();
    bool isNoFrame = frlist.isEmpty();

    qreal joint_len = mfirst->getTotalLength();
    qreal internal_len = mfirst->getInternalLength();
    qreal angle = mfirst->codeAngle();

    MEMBERPARTITION mpart = mfirst->getMemberPartition();
    qreal len_fi = mpart.lenFi;
    qreal len_i = mpart.lenI;
    qreal len_fj = mpart.lenSum - mpart.lenFj;
    qreal len_j = mpart.lenSum - mpart.lenJ;

    bool ok_jointlen = true, ok_internallen = true;
    bool ok_angle = true;
    bool ok_lenfi = true, ok_leni = true, ok_lenfj = true, ok_lenj = true;

    qreal eps = 1.0e-3;

    Q_FOREACH (MemberData *md, mdlist) {

        if ( md == mfirst ) continue;

        if ( isNoFloor ) {
//            isNoFloor=md->getAttachedFloorList().isEmpty();
            isNoFloor = md->getAttachedFloor().isNull();
        } else if ( !fl_uid.isNull() ) {
            if ( !md->containsFloor(fl_uid) ) fl_uid = QUuid();
//            Q_FOREACH( QUuid id, fl_uid )
//                if( !md->containsFloor(id) ) fl_uid.removeOne(id);
        }

        if ( isNoFrame ) {
            isNoFrame = md->getAttachedFrameList().isEmpty();
        } else if ( !frlist.isEmpty() ) {
            Q_FOREACH ( QUuid id, frlist )
                if ( !md->containsFrame(id) ) frlist.removeOne(id);
        }

        if ( ok_jointlen ) ok_jointlen = ( qAbs( joint_len - md->getTotalLength() ) < eps ) ;
        if ( ok_internallen ) ok_internallen = ( qAbs( internal_len - md->getInternalLength() ) < eps ) ;

        MEMBERPARTITION partition = md->getMemberPartition();
        if ( ok_lenfi ) ok_lenfi = ( qAbs( len_fi - partition.lenFi ) < eps ) ;
        if ( ok_leni ) ok_leni = ( qAbs( len_i - partition.lenI ) < eps ) ;
        if ( ok_lenfj ) ok_lenfj = ( qAbs( len_fj - partition.lenSum + partition.lenFj ) < eps ) ;
        if ( ok_lenj ) ok_lenj = ( qAbs( len_j - partition.lenSum + partition.lenJ ) < eps ) ;

        if ( ok_angle ) ok_angle = ( qAbs( angle - md->codeAngle() ) < eps ) ;

    }

    if ( count == 1 ) {
        str_detail += QString(u8"節点番号:");
        Q_FOREACH ( JointData *jd, mfirst->getJointList() ) {
            str_detail += QString("No.%1,").arg(myUnifiedData->indexOfJoint(jd) + 1);
        }
        str_detail += "\n";
    }

    str_detail += QString(u8"所属階:");
    if ( isNoFloor ) {
        str_detail += u8"(無し),";
    } else if ( fl_uid.isNull() ) {
        str_detail += u8"(不定),";
    } else {
        str_detail += myUnifiedData->idToName(fl_uid, DATATYPE::TPFLOOR);
        str_detail += ",";
        /*
               Q_FOREACH(QUuid id, fl_uid){
                   str_detail+=FactoryUnifiedData::getInstance(isOutput)->idToName(id,TPFLOOR);
                   str_detail+=",";
               }
        */
    }
    str_detail += u8"所属通り:";
    if ( isNoFrame) {
        str_detail += u8"(無し),";
    } else if ( frlist.isEmpty() ) {
        str_detail += u8"(不定),";
    } else {
        Q_FOREACH (QUuid id, frlist) {
            str_detail += myUnifiedData->idToName(id, DATATYPE::TPFRAMEP);
            str_detail += ",";
        }
    }
    str_detail += "\n";

    str_detail += u8"節点間距離:";
    str_detail += ( ok_jointlen ) ? QString("%L1mm,").arg(qRound(joint_len)) : u8"(不定),";

    if ( dtype == DATATYPE::TPDAMPER || dtype == DATATYPE::TPISO ) {
        str_detail += "\n";

        if ( count == 1 ) {
            XYZ xyz = mfirst->iXYZ();
            str_detail += u8"i端座標: (X,Y,Z) = ";
            str_detail += QString("(%1m,%2m,%3m),").arg(xyz.x, 0, 'f', 3).arg(xyz.y, 0, 'f', 3).arg(xyz.z, 0, 'f', 3);
            str_detail += "\n";
            xyz = mfirst->jXYZ();
            str_detail += u8"j端座標: (X,Y,Z) = ";
            str_detail += QString("(%1m,%2m,%3m),")
                          .arg(xyz.x, 0, 'f', 3).arg(xyz.y, 0, 'f', 3).arg(xyz.z, 0, 'f', 3);
            str_detail += "\n";
        }

        myWidget->setDetailText(str_detail);
        myWidget->setVisible(true);
        return;
    }

    str_detail += u8"内法長さ:";
    str_detail += ( ok_jointlen ) ? QString("%L1mm,").arg(qRound(internal_len)) : u8"(不定),";
    str_detail += u8"コードアングル:";
    str_detail += ( ok_angle ) ? QString(u8"%1度,").arg(qRound(angle * 180.0 / M_PI)) :  u8"(不定),";
    str_detail += "\n";

    str_detail += u8"端部長さ: i端";
    str_detail += ( ok_leni ) ? QString("%1mm,").arg(qRound(len_i)) : u8"(不定),";
    str_detail += u8"j端";
    str_detail += ( ok_lenj ) ? QString("%1mm,").arg(qRound(len_j)) : u8"(不定),";
    str_detail += "\n";

    str_detail += u8"フェイス長さ: i端";
    str_detail += ( ok_lenfi ) ? QString("%1mm,").arg(qRound(len_fi)) : u8"(不定),";
    str_detail += u8"j端";
    str_detail += ( ok_lenfj ) ? QString("%1mm,").arg(qRound(len_fj)) : u8"(不定),";
    str_detail += "\n";

    if ( count == 1 ) {

        XYZ xyz = mfirst->iXYZ();
        str_detail += u8"i端座標: (X,Y,Z) = ";
        str_detail += QString("(%1m,%2m,%3m),").arg(xyz.x, 0, 'f', 3).arg(xyz.y, 0, 'f', 3).arg(xyz.z, 0, 'f', 3);
        str_detail += "\n";
        xyz = mfirst->jXYZ();
        str_detail += u8"j端座標: (X,Y,Z) = ";
        str_detail += QString("(%1m,%2m,%3m),").arg(xyz.x, 0, 'f', 3).arg(xyz.y, 0, 'f', 3).arg(xyz.z, 0, 'f', 3);
        str_detail += "\n";

        LINEPROPERTY lv = mfirst->getLineProperty(*myUnifiedData);

        str_detail += u8"断面種別:" + structureTypeToString(lv.structType) + QString(",");
        if ( dtype == DATATYPE::TPCOLUMN && !lv.baseName.isEmpty() ) {
            str_detail += u8"柱脚符号:" + lv.baseName + QString(",");
        }
        str_detail += "\n";
        str_detail += u8"断面(i): " + lv.i_material + QString(", ");
        qDebug()<<"i_renderType"<<(int)lv.i_renderType;
        str_detail += renderShapeToString(lv.structType, lv.i_renderType,
                                          lv.i_B, lv.i_H, lv.i_t1, lv.i_t2) + QString(",");
        str_detail += "\n";
        if ( dtype != DATATYPE::TPCOLUMN ) {
            str_detail += u8"断面(c): " + lv.c_material + QString(", ");
            str_detail += renderShapeToString(lv.structType, lv.c_renderType,
                                              lv.c_B, lv.c_H, lv.c_t1, lv.c_t2) + QString(",");
            str_detail += "\n";
        }
        str_detail += u8"断面(j): " + lv.j_material + QString(", ");
        str_detail += renderShapeToString(lv.structType, lv.j_renderType,
                                          lv.j_B, lv.j_H, lv.j_t1, lv.j_t2) + QString(",");
        str_detail += "\n";

    }

    myWidget->setDetailText(str_detail);
    myWidget->setVisible(true);
}

void PropertyDialog::setPlaneMemberProperty( DATATYPE dtype, const QList<MemberData *> &mdlist )
{
    qDebug()<<"PropertyDialog::setPlaneMemberProperty"<<(int)dtype;
    PropertySelectWidget *myWidget;
    if ( dtype == DATATYPE::TPSLAB ) {
        myWidget = s_slabWidget;
    } else if ( dtype == DATATYPE::TPWALL ) {
        myWidget = s_wallWidget;
    } else if ( dtype == DATATYPE::TPUWALL ) {
        myWidget = s_uwalWidget;
    } else {
        return;
    }

    int count = mdlist.count();
    QString str_outline = QString(u8"計%1個").arg(count);
    QStringList nameList = myUnifiedData->getNameList(dtype);
    int nameCount = nameList.count();
    int *counter = new int[nameCount + 1];
    for ( int i = 0; i < nameCount + 1; i++ ) counter[i] = 0;
    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( md->sectionName().isEmpty() ) {
            counter[nameCount]++;
            continue;
        }
        int idx = nameList.indexOf(md->sectionName());
        counter[idx]++;
    }
    if ( counter[nameCount] > 0 ) {
        str_outline += QString(u8",ダミー：%1個").arg(counter[nameCount]);
    }
    for ( int i = 0; i < nameCount; i++ ) {
        if ( counter[i] > 0 ) str_outline += "," + nameList.at(i) + QString(u8":%1個").arg(counter[i]);
    }
    delete [] counter;

    if ( count < 1 ) {
        myWidget->clearText();
        myWidget->setVisible(false);
        return;
    }
    myWidget->setCountText(str_outline);

    QString str_detail;
    MemberData *mfirst = mdlist.first();
    QUuid fl_uid = mfirst->getAttachedFloor();
    QList<QUuid> frlist = mfirst->getAttachedFrameList();
    bool isNoFloor = fl_uid.isNull();
    bool isNoFrame = frlist.isEmpty();

    qreal thick = mfirst->getPlaneThickness();
    PLANEMODEL ptype = mfirst->getPlaneModel();
    PLANESUBMODEL psubtype = mfirst->getPlaneSubModel();
    QString opname = mfirst->wallOpenName();

    bool ok_thick = true;
    bool ok_model = true;
    bool ok_submodel = true;
    bool ok_op = true;

//    qreal shz=mfirst->getShiftZ();
//    bool ok_shz=true;
    qreal eps = 1.0e-3;

    Q_FOREACH (MemberData *md, mdlist) {

        if ( md == mfirst ) continue;

        if ( isNoFloor ) {
            isNoFloor = md->getAttachedFloor().isNull();
        } else if ( !fl_uid.isNull() ) {
            if ( !md->containsFloor(fl_uid) ) fl_uid = QUuid();
//            Q_FOREACH( QUuid id, fl_uid )
//                if( !md->containsFloor(id) ) fl_uid.removeOne(id);
        }

        if ( isNoFrame ) {
            isNoFrame = md->getAttachedFrameList().isEmpty();
        } else if ( !frlist.isEmpty() ) {
            Q_FOREACH ( QUuid id, frlist )
                if ( !md->containsFrame(id) ) frlist.removeOne(id);
        }

        if ( ok_thick ) ok_thick = ( qAbs( thick - md->getPlaneThickness() ) < eps ) ;
        if ( ok_model ) ok_model = ( ptype == md->getPlaneModel() ) ;
        if ( ok_submodel ) ok_submodel = ( psubtype == md->getPlaneSubModel() ) ;
        if ( ok_op ) ok_op = ( opname == md->wallOpenName() ) ;
    }

    if ( count == 1 ) {
        str_detail += u8"節点番号:";
        Q_FOREACH ( JointData *jd, mfirst->getJointList() ) {
            str_detail += QString("No.%1,").arg(myUnifiedData->indexOfJoint(jd) + 1);
        }
        str_detail += "\n";
    }

    str_detail += u8"所属階:";
    if ( isNoFloor ) {
        str_detail += u8"(無し),";
    } else if ( fl_uid.isNull() ) {
        str_detail += u8"(不定),";
    } else {
//        Q_FOREACH(QUuid id, fl_uid){
        str_detail += myUnifiedData->idToName(fl_uid, DATATYPE::TPFLOOR);
        str_detail += ",";
//        }
    }
    str_detail += u8"所属階:";
    if ( isNoFrame ) {
        str_detail += u8"(無し),";
    } else if ( frlist.isEmpty() ) {
        str_detail += u8"(不定),";
    } else {
        Q_FOREACH (QUuid id, frlist) {
            str_detail += myUnifiedData->idToName(id, DATATYPE::TPFRAMEP);
            str_detail += ",";
        }
    }
    str_detail += "\n";

    str_detail += u8"厚さ:";
    str_detail += ( ok_thick ) ? QString("%1mm,").arg(qRound(thick)) : u8"(不定),";
    if ( dtype == DATATYPE::TPWALL ) {
        str_detail += u8"開口符号:";
        str_detail += ( ok_op ) ? opname + "," : u8"(不定),";
    }
    str_detail += "\n";

    str_detail += u8"モデル化:";
    str_detail += ( ok_model ) ? planeModelToString(ptype,psubtype) : u8"(不定),";
    str_detail += "\n";

    myWidget->setDetailText(str_detail);
    myWidget->setVisible(true);
}

void PropertyDialog::setLoadMemberProperty( DATATYPE dtype, const QList<MemberData *> &mdlist )
{
    qDebug()<<"PropertyDialog::setLoadMemberProperty"<<(int)dtype;
    PropertySelectWidget *myWidget;
    if ( dtype == DATATYPE::TPJOINTLOAD ) {
        myWidget = s_jloadWidget;
    } else if ( dtype == DATATYPE::TPMEMBERLOAD ) {
        myWidget = s_mloadWidget;
    } else if ( dtype == DATATYPE::TPSLABLOAD ) {
        myWidget = s_sloadWidget;
    } else {
        return;
    }

    int count = mdlist.count();
    QString str_outline = QString(u8"計%1個").arg(count);
    QStringList nameList = myUnifiedData->getNameList(dtype);
    qDebug()<<"PropertyDialog::namelist:"<<nameList;
    int nameCount = nameList.count();
    int *counter = new int[nameCount + 1];
    for ( int i = 0; i < nameCount + 1; i++ ) counter[i] = 0;
    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( md->sectionName().isEmpty() ) {
            counter[nameCount]++;
            continue;
        }
        int idx = nameList.indexOf( md->sectionName() );
        counter[idx]++;
    }
    if ( counter[nameCount] > 0 ) {
        str_outline += QString(u8",ダミー：%1個").arg(counter[nameCount]);
    }
    for ( int i = 0; i < nameCount; i++ ) {
        if ( counter[i] > 0 ) str_outline += "," + nameList.at(i) + QString(u8":%1個").arg(counter[i]);
    }
    delete [] counter;

    if ( count < 1 ) {
        myWidget->clearText();
        myWidget->setVisible(false);
        return;
    }
    myWidget->setCountText(str_outline);

    QString str_detail;
    MemberData *mfirst = mdlist.first();
    QUuid fl_uid = mfirst->getAttachedFloor();
    QList<QUuid> frlist = mfirst->getAttachedFrameList();
    bool isNoFloor = fl_uid.isNull();
    bool isNoFrame = frlist.isEmpty();

    Q_FOREACH (MemberData *md, mdlist) {

        if ( md == mfirst ) continue;

        if ( isNoFloor ) {
            isNoFloor = md->getAttachedFloor().isNull();
        } else if ( !fl_uid.isNull() ) {
            if ( !md->containsFloor(fl_uid) ) fl_uid = QUuid();
//            Q_FOREACH( QUuid id, fl_uid )
//                if( !md->containsFloor(id) ) fl_uid.removeOne(id);
        }

        if ( isNoFrame ) {
            isNoFrame = md->getAttachedFrameList().isEmpty();
        } else if ( !frlist.isEmpty() ) {
            Q_FOREACH ( QUuid id, frlist )
                if ( !md->containsFrame(id) ) frlist.removeOne(id);
        }

    }

    if ( count == 1 ) {
        str_detail += u8"節点番号:";
        Q_FOREACH ( JointData *jd, mfirst->getJointList() ) {
            str_detail += QString("No.%1,").arg(myUnifiedData->indexOfJoint(jd) + 1);
        }
        str_detail += "\n";
    }

    str_detail += u8"所属階:";
    if ( isNoFloor ) {
        str_detail += u8"(無し),";
    } else if ( fl_uid.isNull() ) {
        str_detail += u8"(不定),";
    } else {
//        Q_FOREACH(QUuid id, fl_uid){
        str_detail += myUnifiedData->idToName(fl_uid, DATATYPE::TPFLOOR);
        str_detail += ",";
//        }
    }
    str_detail += u8"所属通り:";
    if ( isNoFrame ) {
        str_detail += u8"(無し),";
    } else if ( frlist.isEmpty() ) {
        str_detail +=  u8"(不定),";
    } else {
        Q_FOREACH (QUuid id, frlist) {
            str_detail += myUnifiedData->idToName(id, DATATYPE::TPFRAMEP);
            str_detail += ",";
        }
    }
    str_detail += "\n";

    if ( count == 1 ) {
        str_detail += myUnifiedData->loadNameToLoadProperty(dtype, mfirst->sectionName());
        str_detail += "\n";
    }

    myWidget->setDetailText(str_detail);
    myWidget->setVisible(true);
}

QString PropertyDialog::structureTypeToString( STRUCTTYPE stype ) const
{
    if ( stype == STRUCTTYPE::STP_RC ) {
        return QString("RC");
    } else if ( stype == STRUCTTYPE::STP_S ) {
        return QString("S");
    } else if ( stype == STRUCTTYPE::STP_SRC ) {
        return QString("SRC");
    } else if ( stype == STRUCTTYPE::STP_CFT ) {
        return QString("CFT");
    }
    return QString("(none)");
}

QString PropertyDialog::planeModelToString( PLANEMODEL pmodel ,PLANESUBMODEL psubmodel) const
{
    if ( pmodel == PLANEMODEL::NONSTRUCT_MODEL ) {
        return u8"非構造モデル";
    } else if ( pmodel == PLANEMODEL::WALL_MODEL ) {
        if(psubmodel == PLANESUBMODEL::MEMBRANE_MODEL)return u8"平面応力要素";
        if(psubmodel == PLANESUBMODEL::PLATE_MODEL)return u8"平板シェル要素";
        if(psubmodel == PLANESUBMODEL::WH_MODEL)return u8"耐力壁（WH）";
        if(psubmodel == PLANESUBMODEL::WI_MODEL)return u8"耐力壁（WI）";
        if(psubmodel == PLANESUBMODEL::SH_MODEL)return u8"鋼板壁（SH）";
        if(psubmodel == PLANESUBMODEL::SI_MODEL)return u8"鋼板壁（SI）";
        return u8"耐力壁";
    } else if ( pmodel == PLANEMODEL::ZWALL_MODEL ) {
        return u8"雑壁";
    } else if ( pmodel == PLANEMODEL::SLAB_MODEL ) {
        if(psubmodel == PLANESUBMODEL::MEMBRANE_MODEL)return u8"平面応力要素";
        if(psubmodel == PLANESUBMODEL::PLATE_MODEL)return u8"平板シェル要素";
        return u8"スラブ";
    }
    return u8"非構造モデル";
}

QString PropertyDialog::renderShapeToString(STRUCTTYPE stype, RENDERSHAPE rtype,
                                             qreal s1, qreal s2, qreal s3, qreal s4) const
{
    qDebug()<<"PropertyDialog::renderShapeToString"<<(int)stype<<(int)rtype;
    QString str;

    if ( stype == STRUCTTYPE::STP_RC || stype == STRUCTTYPE::STP_SRC ) {
        if ( rtype == RENDERSHAPE::SHAPE_PIPE ) {
            str += QString("D:%1mm").arg(qRound(s1));
        } else {
            str += QString("Dx:%1mm,Dy:%2mm").arg(qRound(s1)).arg(qRound(s2));
        }
    } else {
        if ( rtype == RENDERSHAPE::SHAPE_BOX ) {
            str += "BX-";
            str += QString("%1x%2x%3x%4")
                   .arg(qRound(s2)).arg(qRound(s1)).arg(qRound(s3)).arg(qRound(s4));
        } else if ( rtype == RENDERSHAPE::SHAPE_H ) {
            str += "H-";
            str += QString("%1x%2x%3x%4")
                   .arg(qRound(s2)).arg(qRound(s1)).arg(qRound(s3)).arg(qRound(s4));
        } else if ( rtype == RENDERSHAPE::SHAPE_I ) {
            str += "I-";
            str += QString("%1x%2x%3x%4")
                   .arg(qRound(s2)).arg(qRound(s1)).arg(qRound(s3)).arg(qRound(s4));
        } else if ( rtype == RENDERSHAPE::SHAPE_T1 ) {
            str += "T1-";
            str += QString("%1x%2x%3x%4")
                   .arg(qRound(s2)).arg(qRound(s1)).arg(qRound(s3)).arg(qRound(s4));
        } else if ( rtype == RENDERSHAPE::SHAPE_T2 ) {
            str += "T2-";
            str += QString("%1x%2x%3x%4")
                   .arg(qRound(s2)).arg(qRound(s1)).arg(qRound(s3)).arg(qRound(s4));
        } else if ( rtype == RENDERSHAPE::SHAPE_C1 ) {
            str += "C-";
            str += QString("%1x%2x%3x%4")
                   .arg(qRound(s2)).arg(qRound(s1)).arg(qRound(s3)).arg(qRound(s4));
        } else if ( rtype == RENDERSHAPE::SHAPE_L ) {
            str += "L-";
            str += QString("%1x%2x%3x%4")
                   .arg(qRound(s2)).arg(qRound(s1)).arg(qRound(s3)).arg(qRound(s4));
        } else if ( rtype == RENDERSHAPE::SHAPE_PIPE ) {
            str += "P-";
            str += QString("%1x%2")
                   .arg(qRound(s1)).arg(qRound(s3));
        } else if ( rtype == RENDERSHAPE::SHAPE_CROSS ) {
            str += "CRS-";
            str += QString("%1x%2x%3x%4")
                   .arg(qRound(s2)).arg(qRound(s1)).arg(qRound(s3)).arg(qRound(s4));
        } else if ( rtype == RENDERSHAPE::SHAPE_CROSSH ) {
            str += QString(u8"CROSS-H-SECTION");
        }
    }
    return str;
}


/* PropertyInputWidget */

PropertyInputWidget::PropertyInputWidget(const QString &name, QWidget *parent)
    : QWidget(parent)
{
    QLabel *myLabel = new QLabel(u8"入力済の" + name + ":", this);
    myLabel->setFixedWidth(100);

    textEdit = new QTextEdit(this);
    textEdit->setContentsMargins(2, 2, 2, 2);
    textEdit->setReadOnly(true);

    QGridLayout *glay = new QGridLayout;
    glay->addWidget(myLabel, 0, 0, 1, 1);
    glay->addWidget(textEdit, 0, 1, 2, 1);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->addLayout(glay);
    vlay->addWidget(new HFLine(Qt::lightGray, this));

    this->setLayout(vlay);
}

void PropertyInputWidget::clearText()
{
    textEdit->clear();
}

void PropertyInputWidget::setDetailText(const QString &text)
{
    textEdit->setText(text);
}

/* PropertySelectWidget */

PropertySelectWidget::PropertySelectWidget(const QString &name, QWidget *parent)
    : QWidget(parent)
{
    QIcon icon;
    icon.addPixmap(QPixmap(":/icons/expand.png"), QIcon::Normal, QIcon::On);
    icon.addPixmap(QPixmap(":/icons/contract.png"), QIcon::Normal, QIcon::Off);
    QToolButton *toolButton = new QToolButton(this);
    toolButton->setIcon(icon);
    toolButton->setCheckable(true);
    toolButton->setChecked(true);
    toolButton->setAutoRaise(true);
    toolButton->setIconSize(QSize(15, 15));
    toolButton->setFixedSize(20, 20);

    QLabel *myLabel = new QLabel(u8"選択された" + name + ":", this);
    myLabel->setFixedWidth(130);

    lineEdit = new QLineEdit(this);
    lineEdit->setReadOnly(true);
    textEdit = new QTextEdit(this);
    textEdit->setContentsMargins(2, 2, 2, 2);
    textEdit->setMinimumHeight(1000);
    textEdit->setReadOnly(true);

    connect(toolButton, &QAbstractButton::toggled, textEdit, &QWidget::setVisible);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(toolButton);
    hlay->addWidget(myLabel);
    hlay->addWidget(lineEdit, 1);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(2);
    vlay->addLayout(hlay);
    vlay->addWidget(textEdit, 1);
    vlay->addWidget(new HFLine(Qt::lightGray, this));

    this->setLayout(vlay);
}

void PropertySelectWidget::clearText()
{
    lineEdit->clear();
    textEdit->clear();
}

void PropertySelectWidget::setCountText(const QString &text)
{
    lineEdit->setText(text);
    lineEdit->setCursorPosition(0);
}

void PropertySelectWidget::setDetailText(const QString &text)
{
    textEdit->setText(text);
}
} // namespace post3dapp

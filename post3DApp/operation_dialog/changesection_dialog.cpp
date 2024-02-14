#include "changesection_dialog.h"

#include <QtGui>

#include "factory_unifieddata.h"
#include "unified_data.h"

namespace post3dapp{
ChangeSectionDialog::ChangeSectionDialog(QWidget *parent,UnifiedDataType isout)
    : QDialog(parent)
{
    isOutput = isout;
    dataTypeSrings << u8"柱" << u8"大梁" << u8"ブレース" << u8"小梁" << u8"スラブ"
                   << u8"壁" << u8"地下外壁" << u8"制振要素" << u8"免震要素"
                   << u8"節点荷重" << u8"部材荷重" << u8"床荷重" ;
    dataTypes << DATATYPE::TPCOLUMN << DATATYPE::TPGIRDER << DATATYPE::TPBRACE << DATATYPE::TPBEAM << DATATYPE::TPSLAB
              << DATATYPE::TPWALL << DATATYPE::TPUWALL << DATATYPE::TPDAMPER << DATATYPE::TPISO
              << DATATYPE::TPJOINTLOAD << DATATYPE::TPMEMBERLOAD << DATATYPE::TPSLABLOAD ;

    createLayout();

    elementComboBox->setCurrentIndex(0);
    slotChangeSectionComboBox(0);

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"符号の変更");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void ChangeSectionDialog::setCurrentDataType(DATATYPE dtype)
{
    int idx = dataTypes.indexOf(dtype);
    if ( idx > -1 ) elementComboBox->setCurrentIndex(idx);
}

void ChangeSectionDialog::createLayout()
{
    elementComboBox = new QComboBox(this);
    elementComboBox->addItems(dataTypeSrings);
    connect( elementComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
             this, &ChangeSectionDialog::slotChangeSectionComboBox );

    sectionComboBox = new QComboBox(this);
    QGridLayout *glay = new QGridLayout;
    glay->setSpacing(5);
    glay->addWidget(new QLabel(u8"対象要素 ：", this), 0, 0, 1, 1);
    glay->addWidget(elementComboBox, 0, 1, 1, 2);
    glay->addWidget(new QLabel(u8"変更後の符号 ：", this), 1, 0, 1, 1);
    glay->addWidget(sectionComboBox, 1, 1, 1, 2);

    QPushButton *okButton = new QPushButton(u8"変　更", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &QDialog::accept );
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addStretch();
    hlay->addWidget(okButton);
    hlay->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(glay);
    vlay->addLayout(hlay);
    vlay->addStretch();

    this->setLayout(vlay);
}

void ChangeSectionDialog::slotChangeSectionComboBox(int idx)
{
    sectionComboBox->clear();
    QStringList names = FactoryUnifiedData::getInstance(isOutput)->getNameList( dataTypes.at(idx) );
    names.prepend(QString(""));
    sectionComboBox->addItems(names);
}

DATATYPE ChangeSectionDialog::currentDataType() const
{
    int idx = elementComboBox->currentIndex();
    return dataTypes.at(idx);
}

QString ChangeSectionDialog::currentName() const
{
    return sectionComboBox->currentText();
}
} // namespace post3dapp

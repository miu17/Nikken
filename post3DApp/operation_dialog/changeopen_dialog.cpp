#include "changeopen_dialog.h"

#include <QtGui>

#include "factory_unifieddata.h"
#include "unified_data.h"

namespace post3dapp{
ChangeOpenDialog::ChangeOpenDialog(QWidget *parent , UnifiedDataType isout)
    : QDialog(parent), isOutput(isout)
{
    createLayout();

    slotChangeSectionComboBox();

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"開口符号の追加・変更");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void ChangeOpenDialog::createLayout()
{
    QLabel *changedLabel = new QLabel(u8"変更後の開口符号 ：", this);
    sectionComboBox = new QComboBox(this);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(changedLabel, 0);
    hlay1->addWidget(sectionComboBox, 1);

    QPushButton *okButton = new QPushButton(u8"変　更", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &QDialog::accept );
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->addWidget(okButton);
    hlay2->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(hlay1);
    vlay->addLayout(hlay2);

    this->setLayout(vlay);
}

void ChangeOpenDialog::slotChangeSectionComboBox()
{
    sectionComboBox->clear();
    QStringList names = FactoryUnifiedData::getInstance(isOutput)->getNameList( DATATYPE::TPWALLOPEN );
    names.prepend(QString(""));
    sectionComboBox->addItems(names);
}

QString ChangeOpenDialog::currentName() const
{
    return sectionComboBox->currentText();
}
} // namespace post3dapp

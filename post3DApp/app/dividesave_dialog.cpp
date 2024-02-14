#include <QtGui>
#include <QtWidgets>

#include "dividesave_dialog.h"
#include "nameselect_dialog.h"
#include "unified_inputdata.h"
#include "utility.h"
#include "vhline.h"

namespace post3dapp{

DivideSaveDialog::DivideSaveDialog( QWidget *parent )
    : QDialog(parent)
{
    createLayout();

    setFont( QFont(tr("Microsoft JhengHei"), 9) );
    setWindowTitle(u8"ファイルの分割保存");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint);
}

void DivideSaveDialog::createLayout()
{
    // 階範囲

    QPushButton *button_range = new QPushButton(u8"階範囲", this);
    button_range->setFixedWidth(85);
    label_range = new QLabel(this);
    label_range->setFrameStyle(QFrame::Box | QFrame::Raised);
    QHBoxLayout *hlay_range = new QHBoxLayout;
    hlay_range->addWidget(button_range);
    hlay_range->addWidget(label_range);

    connect( button_range, &QAbstractButton::clicked, this, &DivideSaveDialog::slotShowRangeDialog );

    // 保存ファイル

    button_fileA = new QPushButton(u8"保存ファイル", this);
    button_fileA->setFixedWidth(85);
    label_fileA = new QLabel(this);
    label_fileA->setFrameStyle(QFrame::Box | QFrame::Raised);
    QHBoxLayout *hlay_fileA = new QHBoxLayout;
    hlay_fileA->addWidget(button_fileA);
    hlay_fileA->addWidget(label_fileA);

    connect( button_fileA, &QAbstractButton::clicked, this, &DivideSaveDialog::slotOpenFileA );

    // 範囲外

    check_outrange = new QCheckBox(u8"上記範囲に含まれないデータを別ファイルに保存する。", this);

    connect( check_outrange, &QAbstractButton::toggled, this, &DivideSaveDialog::slotSetOutRangeStatus );

    // 保存ファイル

    button_fileB = new QPushButton(u8"保存ファイル", this);
    button_fileB->setFixedWidth(80);
    label_fileB = new QLabel(this);
    label_fileB->setFrameStyle(QFrame::Box | QFrame::Raised);
    QHBoxLayout *hlay_fileB = new QHBoxLayout;
    hlay_fileB->addWidget(button_fileB);
    hlay_fileB->addWidget(label_fileB);

    connect( button_fileB, &QAbstractButton::clicked, this, &DivideSaveDialog::slotOpenFileB );

    // execute

    QPushButton *okButton = new QPushButton(u8"実　行", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &DivideSaveDialog::slotCheckButtonStatus );
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    // layout

    QHBoxLayout *hlay_ok = new QHBoxLayout;
    hlay_ok->addStretch();
    hlay_ok->addWidget(okButton);
    hlay_ok->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setSpacing(5);
    vlay->addLayout(hlay_range);
    vlay->addLayout(hlay_fileA);
    vlay->addSpacing(3);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(3);
    vlay->addWidget(check_outrange);
    vlay->addLayout(hlay_fileB);
    vlay->addStretch();
    vlay->addSpacing(3);
    vlay->addLayout(hlay_ok);

    slotSetOutRangeStatus(false);

    this->setLayout(vlay);
}

void DivideSaveDialog::slotShowRangeDialog()
{
    QStringList rangeFloors = ( label_range->text().isEmpty() ) ? QStringList()
                              : label_range->text().split(",");

    QStringList fllist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFLOOR);
    NameSelectDialog dialog(fllist, rangeFloors, this);
    dialog.setWindowTitle(QString(u8"階の選択"));
    if ( dialog.exec() ) {
        rangeFloors = dialog.selectedNames();
        QString str = ( rangeFloors.isEmpty() ) ? QString("") : rangeFloors.join(",") ;
        label_range->setText(str);
    }
}

void DivideSaveDialog::slotSetOutRangeStatus(bool on)
{
    if ( on ) {
        button_fileB->setEnabled(true);
        label_fileB->setEnabled(true);
    } else {
        button_fileB->setEnabled(false);
        label_fileB->setEnabled(false);
    }
}

void DivideSaveDialog::slotOpenFileA()
{
    QString opendir = ( label_fileA->text().isEmpty() ) ? QDir::currentPath() : label_fileA->text() ;
    QString filename = QFileDialog::getSaveFileName(0, QString(), opendir, "File(*.p3d)");
    label_fileA->setText(filename);
}

void DivideSaveDialog::slotOpenFileB()
{
    QString opendir = ( label_fileB->text().isEmpty() ) ? QDir::currentPath() : label_fileB->text() ;
    QString filename = QFileDialog::getSaveFileName(0, QString(), opendir, "File(*.p3d)");
    label_fileB->setText(filename);
}

void DivideSaveDialog::slotCheckButtonStatus()
{
    QString fileA = label_fileA->text();
    QString fileB = ( check_outrange->isChecked() ) ? label_fileB->text() : QString();

    if ( fileA.isEmpty() ) {
        Utility::showErrorMessage(this, u8"保存ファイルが指定されていません。");
        return;
    }

    if ( check_outrange->isChecked() && fileB.isEmpty() ) {
        Utility::showErrorMessage(this, u8"範囲外データの保存ファイルが指定されていません。");
        return;
    }

    if ( fileA == fileB ) {
        Utility::showErrorMessage(this, u8"保存ファイル名が重複しています。");
        return;
    }

    accept();
}

FILESAVEPATTERN DivideSaveDialog::getFileSavePattern() const
{
    QString fileA = label_fileA->text();
    QString fileB = ( check_outrange->isChecked() ) ? label_fileB->text() : QString();

    QStringList floors = ( label_range->text().isEmpty() ) ? QStringList()
                         : label_range->text().split(",");
    QList<QUuid> idlist = UnifiedInputData::getInInstance()->namesToIDs(floors, DATATYPE::TPFLOOR);

    return FILESAVEPATTERN(fileA, fileB, idlist);
}
} // namespace post3dapp

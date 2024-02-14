#include "additionalopen_dialog.h"

#include <QtGui>
#include <QtWidgets>

#include "define_unifieddata.h"
#include "reference_combobox.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "utility.h"
#include "vhline.h"

namespace post3dapp{
AdditionalOpenDialog::AdditionalOpenDialog( QWidget *parent )
    : QDialog(parent)
{
    createLayout();

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"ファイルの追加読込");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void AdditionalOpenDialog::createLayout()
{
    // read pattern

    QRadioButton *button_newread = new QRadioButton(u8"現在開かれているデータ（A）に新たなファイル（B）を追加で読込む。", this);
    QRadioButton *button_sameread = new QRadioButton(u8"現在開かれているデータを破棄して別々の２ファイル（A,B）を同時に読込む。", this);
    group_read = new QButtonGroup(this);
    group_read->addButton(button_newread, 0);
    group_read->addButton(button_sameread, 1);
    button_newread->setChecked(true);

    connect( button_newread, &QAbstractButton::toggled, this, &AdditionalOpenDialog::slotSetAddReadStatus );

    // fileA,fileB

    button_fileA = new QPushButton(u8"Aファイル", this);
    label_fileA = new QLabel(this);
    label_fileA->setFrameStyle(QFrame::Box | QFrame::Raised);
    button_fileA->setFixedWidth(75);
    QHBoxLayout *hlay_fileA = new QHBoxLayout;
    hlay_fileA->addWidget(button_fileA);
    hlay_fileA->addWidget(label_fileA);

    connect( button_fileA, &QAbstractButton::clicked, this, &AdditionalOpenDialog::slotOpenFileA );

    button_fileB = new QPushButton(u8"Bファイル", this);
    label_fileB = new QLabel(this);
    label_fileB->setFrameStyle(QFrame::Box | QFrame::Raised);
    button_fileB->setFixedWidth(75);
    QHBoxLayout *hlay_fileB = new QHBoxLayout;
    hlay_fileB->addWidget(button_fileB);
    hlay_fileB->addWidget(label_fileB);

    connect( button_fileB, &QAbstractButton::clicked, this, &AdditionalOpenDialog::slotOpenFileB );

    // add Floor

    check_addFloor = new QCheckBox(u8"新たに追加するデータ（B）の全節点に階属性を付加する。", this);
    combo_addFloor = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::Input, this);
    combo_addFloor->setFixedWidth(100);
    QHBoxLayout *hlay_add = new QHBoxLayout;
    hlay_add->addWidget(check_addFloor);
    hlay_add->addWidget(combo_addFloor);

    // overlap
    QPushButton *button_allA = new QPushButton(u8"全てA優先", this);
    button_allA->setFixedWidth(75);
    QPushButton *button_allB = new QPushButton(u8"全てB優先", this);
    button_allB->setFixedWidth(75);

    connect( button_allA, &QAbstractButton::clicked, this, &AdditionalOpenDialog::slotSelectAllFileA );
    connect( button_allB, &QAbstractButton::clicked, this, &AdditionalOpenDialog::slotSelectAllFileB );

    QHBoxLayout *hlay_overlap = new QHBoxLayout;
    hlay_overlap->addWidget(new QLabel(u8"【データの重複に対する扱い】", this));
    hlay_overlap->addStretch();
    hlay_overlap->addWidget(button_allA);
    hlay_overlap->addWidget(button_allB);


    QLabel *label_1 = new QLabel(u8"・定義リスト（階,通り,断面など）", this);
    QLabel *label_2 = new QLabel(u8"・IDが重複する節点データ", this);
    QLabel *label_3 = new QLabel(u8"・IDが重複する部材・荷重データ", this);
    QLabel *label_4 = new QLabel(u8"・座標が重複する節点データ", this);
    QLabel *label_5 = new QLabel(u8"・座標が重複する部材・荷重データ", this);

    label_1->setFixedWidth(200);
    label_2->setFixedWidth(200);
    label_3->setFixedWidth(200);
    label_4->setFixedWidth(200);
    label_5->setFixedWidth(200);

    QRadioButton *bt_1A = new QRadioButton(u8"Aファイル優先", this);
    QRadioButton *bt_1B = new QRadioButton(u8"Bファイル優先", this);
    overlap_list = new QButtonGroup(this);
    overlap_list->addButton(bt_1A, 0);
    overlap_list->addButton(bt_1B, 1);
    bt_1A->setChecked(true);
    bt_1A->setFixedWidth(110);
    bt_1B->setFixedWidth(110);

    QRadioButton *bt_2A = new QRadioButton(u8"Aファイル優先", this);
    QRadioButton *bt_2B = new QRadioButton(u8"Bファイル優先", this);
    overlap_jID = new QButtonGroup(this);
    overlap_jID->addButton(bt_2A, 0);
    overlap_jID->addButton(bt_2B, 1);
    bt_2A->setChecked(true);
    bt_2A->setFixedWidth(110);
    bt_2B->setFixedWidth(110);

    QRadioButton *bt_3A = new QRadioButton(u8"Aファイル優先", this);
    QRadioButton *bt_3B = new QRadioButton(u8"Bファイル優先", this);
    overlap_mID = new QButtonGroup(this);
    overlap_mID->addButton(bt_3A, 0);
    overlap_mID->addButton(bt_3B, 1);
    bt_3A->setChecked(true);
    bt_3A->setFixedWidth(110);
    bt_3B->setFixedWidth(110);

    QRadioButton *bt_4A = new QRadioButton(u8"Aファイル優先", this);
    QRadioButton *bt_4B = new QRadioButton(u8"Bファイル優先", this);
    QRadioButton *bt_4C = new QRadioButton(u8"重複させたまま", this);
    overlap_jXYZ = new QButtonGroup(this);
    overlap_jXYZ->addButton(bt_4A, 0);
    overlap_jXYZ->addButton(bt_4B, 1);
    overlap_jXYZ->addButton(bt_4C, 2);
    bt_4A->setChecked(true);
    bt_4A->setFixedWidth(110);
    bt_4B->setFixedWidth(110);
    bt_4C->setFixedWidth(110);

    QRadioButton *bt_5A = new QRadioButton(u8"Aファイル優先", this);
    QRadioButton *bt_5B = new QRadioButton(u8"Bファイル優先", this);
    QRadioButton *bt_5C = new QRadioButton(u8"重複させたまま", this);
    overlap_mXYZ = new QButtonGroup(this);
    overlap_mXYZ->addButton(bt_5A, 0);
    overlap_mXYZ->addButton(bt_5B, 1);
    overlap_mXYZ->addButton(bt_5C, 2);
    bt_5A->setChecked(true);
    bt_5A->setFixedWidth(110);
    bt_5B->setFixedWidth(110);
    bt_5C->setFixedWidth(110);


    connect( button_sameread, &QAbstractButton::toggled, label_1, &QWidget::setEnabled );
    label_1->setEnabled(false);


    QHBoxLayout *hlay_1A = new QHBoxLayout;
    hlay_1A->addWidget(label_1);
    hlay_1A->addWidget(bt_1A);
    hlay_1A->addWidget(bt_1B);
    hlay_1A->addStretch();

    QHBoxLayout *hlay_2A = new QHBoxLayout;
    hlay_2A->addWidget(label_2);
    hlay_2A->addWidget(bt_2A);
    hlay_2A->addWidget(bt_2B);
    hlay_2A->addStretch();

    QHBoxLayout *hlay_3A = new QHBoxLayout;
    hlay_3A->addWidget(label_3);
    hlay_3A->addWidget(bt_3A);
    hlay_3A->addWidget(bt_3B);
    hlay_3A->addStretch();

    QHBoxLayout *hlay_4A = new QHBoxLayout;
    hlay_4A->addWidget(label_4);
    hlay_4A->addWidget(bt_4A);
    hlay_4A->addWidget(bt_4B);
    hlay_4A->addWidget(bt_4C);
    hlay_4A->addStretch();

    QHBoxLayout *hlay_5A = new QHBoxLayout;
    hlay_5A->addWidget(label_5);
    hlay_5A->addWidget(bt_5A);
    hlay_5A->addWidget(bt_5B);
    hlay_5A->addWidget(bt_5C);
    hlay_5A->addStretch();

    // execute

    QPushButton *okButton = new QPushButton(u8"実　行", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &AdditionalOpenDialog::slotCheckButtonStatus );
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    // layout

    QHBoxLayout *hlay_ok = new QHBoxLayout;
    hlay_ok->addStretch();
    hlay_ok->addWidget(okButton);
    hlay_ok->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setSpacing(5);
    vlay->addWidget(button_newread);
    vlay->addWidget(button_sameread);
    vlay->addSpacing(3);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(3);
    vlay->addLayout(hlay_fileA);
    vlay->addLayout(hlay_fileB);
    vlay->addSpacing(3);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(3);
    vlay->addLayout(hlay_add);
    vlay->addSpacing(3);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(3);
    vlay->addLayout(hlay_overlap);
    vlay->addSpacing(3);
    vlay->addLayout(hlay_1A);
    vlay->addLayout(hlay_2A);
    vlay->addLayout(hlay_3A);
    vlay->addLayout(hlay_4A);
    vlay->addLayout(hlay_5A);
    vlay->addStretch();
    vlay->addSpacing(3);
    vlay->addLayout(hlay_ok);

    slotSetAddReadStatus(true);

    this->setLayout(vlay);
}

void AdditionalOpenDialog::slotSetAddReadStatus(bool on)
{
    if ( on ) {

        button_fileA->setEnabled(false);
        label_fileA->setText(UnifiedEditingData::getInstance()->currentFilePath() );
        label_fileA->setEnabled(false);

        check_addFloor->setEnabled(true);
        combo_addFloor->setEnabled(true);

        overlap_list->button(0)->setChecked(true);
        overlap_list->button(0)->setEnabled(false);
        overlap_list->button(1)->setEnabled(false);

    } else {

        button_fileA->setEnabled(true);
        label_fileA->setEnabled(true);

        check_addFloor->setEnabled(false);
        combo_addFloor->setEnabled(false);

        overlap_list->button(0)->setEnabled(true);
        overlap_list->button(1)->setEnabled(true);

    }
}

void AdditionalOpenDialog::slotOpenFileA()
{
    QString opendir = ( label_fileA->text().isEmpty() ) ? QDir::currentPath() : label_fileA->text() ;
    QString filename = QFileDialog::getOpenFileName(nullptr, QString(), opendir, "File(*.p3d)");
    label_fileA->setText(filename);
}

void AdditionalOpenDialog::slotOpenFileB()
{
    QString opendir = ( label_fileB->text().isEmpty() ) ? QDir::currentPath() : label_fileB->text() ;
    QString filename = QFileDialog::getOpenFileName(nullptr, QString(), opendir, "File(*.p3d)");
    label_fileB->setText(filename);
}

void AdditionalOpenDialog::slotSelectAllFileA()
{
    overlap_list->button(0)->setChecked(true);
    overlap_jID->button(0)->setChecked(true);
    overlap_mID->button(0)->setChecked(true);
    overlap_jXYZ->button(0)->setChecked(true);
    overlap_mXYZ->button(0)->setChecked(true);
}

void AdditionalOpenDialog::slotSelectAllFileB()
{
    if ( group_read->button(1)->isChecked() ) overlap_list->button(1)->setChecked(true);
    overlap_jID->button(1)->setChecked(true);
    overlap_mID->button(1)->setChecked(true);
    overlap_jXYZ->button(1)->setChecked(true);
    overlap_mXYZ->button(1)->setChecked(true);
}

void AdditionalOpenDialog::slotCheckButtonStatus()
{
    if ( label_fileA->text() == label_fileB->text() ) {
        Utility::showErrorMessage(this, u8"AファイルとBファイルが重複しています。");
        return;
    }

    if ( group_read->button(0)->isChecked() ) {
        if ( !QFileInfo( label_fileB->text() ).exists() ) {
            Utility::showErrorMessage(this, u8"読込ファイル（B）が指定されていません。");
            return;
        }
        if ( check_addFloor->isChecked() && combo_addFloor->currentUuid().isNull() ) {
            Utility::showErrorMessage(this, u8"付加する階属性が指定されていません。");
            return;
        }
    } else {
        if ( !QFileInfo( label_fileA->text() ).exists() ) {
            Utility::showErrorMessage(this, u8"読込ファイル（A）が指定されていません");
            return;
        }
        if ( !QFileInfo( label_fileB->text() ).exists() ) {
            Utility::showErrorMessage(this, u8"読込ファイル（B）が指定されていません");
            return;
        }
    }
    accept();
}

FILEREADPATTERN AdditionalOpenDialog::getFileReadPattern() const
{
    QString fA = ( group_read->button(0)->isChecked() ) ? QString() : label_fileA->text();
    QString fB = label_fileB->text();
    QUuid uid = ( check_addFloor->isChecked() ) ? combo_addFloor->currentUuid() : QUuid();
    FILEPRIOR list = static_cast<FILEPRIOR>( overlap_list->checkedId() );
    FILEPRIOR jid = static_cast<FILEPRIOR>( overlap_jID->checkedId() );
    FILEPRIOR mid = static_cast<FILEPRIOR>( overlap_mID->checkedId() );
    FILEPRIOR jxyz = static_cast<FILEPRIOR>( overlap_jXYZ->checkedId() );
    FILEPRIOR mxyz = static_cast<FILEPRIOR>( overlap_mXYZ->checkedId() );
    return FILEREADPATTERN( fA, fB, uid, list, jid, mid, jxyz, mxyz );
}
} // namespace post3dapp

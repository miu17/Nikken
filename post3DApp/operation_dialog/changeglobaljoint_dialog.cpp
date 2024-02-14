#include "changeglobaljoint_dialog.h"

#include <QtGui>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QMessageBox>
#include <QPushButton>

#include "persUI_widget.h"
#include "vhline.h"

namespace post3dapp{
ChangeGlobalJointDialog::ChangeGlobalJointDialog(const QString &str_x, const QString &str_y,
                                                 const QString &str_z,
                                                 PersInterfaceWidget *parent)
    : QDialog(parent), parentWidget(parent)
{
    createLayout();

    loadValues = GLOBALJOINTVALUES(str_x, str_y, str_z);
    globalXEdit->setText(str_x);
    globalYEdit->setText(str_y);
    globalZEdit->setText(str_z);
    globalDXEdit->setText("0.000");
    globalDYEdit->setText("0.000");
    globalDZEdit->setText("0.000");
    globalXYZButton->setEnabled(false);
    globalDeltaButton->setEnabled(false);
    appendFloorButton->setEnabled(false);
    appendFrameButton->setEnabled(false);

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"節点座標・属性の変更");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void ChangeGlobalJointDialog::createLayout()
{
    /*　全体座標系の座標変更 */

    QLabel *globalLabel = new QLabel(u8"【全体座標系の座標変更】", this);
    QLabel *globalCLabel = new QLabel(u8"※階・通り属性は変更されません", this);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(globalLabel);
    hlay1->addStretch();
    hlay1->addWidget(globalCLabel);

    QLabel *globalXLabel = new QLabel(u8"X座標（m）：", this);
    QLabel *globalYLabel = new QLabel(u8"Y座標（m）：", this);
    QLabel *globalZLabel = new QLabel(u8"Z座標（m）：", this);

    globalXEdit = new QLineEdit(this);
    globalYEdit = new QLineEdit(this);
    globalZEdit = new QLineEdit(this);
    globalXEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalYEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalZEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalXEdit->setAlignment(Qt::AlignRight);
    globalYEdit->setAlignment(Qt::AlignRight);
    globalZEdit->setAlignment(Qt::AlignRight);

    connect(globalXEdit, &QLineEdit::textEdited, this, &ChangeGlobalJointDialog::checkGlobalXYZModified);
    connect(globalYEdit, &QLineEdit::textEdited, this, &ChangeGlobalJointDialog::checkGlobalXYZModified);
    connect(globalZEdit, &QLineEdit::textEdited, this, &ChangeGlobalJointDialog::checkGlobalXYZModified);

    QGridLayout *glay1 = new QGridLayout;
    glay1->setSpacing(5);
    glay1->addWidget(globalXLabel, 0, 0, 1, 1);
    glay1->addWidget(globalXEdit, 0, 1, 1, 2);
    glay1->addWidget(globalYLabel, 1, 0, 1, 1);
    glay1->addWidget(globalYEdit, 1, 1, 1, 2);
    glay1->addWidget(globalZLabel, 2, 0, 1, 1);
    glay1->addWidget(globalZEdit, 2, 1, 1, 2);

    globalXYZButton = new QPushButton(u8"座標値の変更", this);
    connect( globalXYZButton, &QAbstractButton::clicked, this, &ChangeGlobalJointDialog::slotChangeGlobalXYZ );

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->addLayout(glay1, 1);
    vlay1->addWidget(globalXYZButton, 0);

    QLabel *globalDXLabel = new QLabel(u8"ΔX（m）：", this);
    QLabel *globalDYLabel = new QLabel(u8"ΔY（m）：", this);
    QLabel *globalDZLabel = new QLabel(u8"ΔZ（m）：", this);

    globalDXEdit = new QLineEdit(this);
    globalDYEdit = new QLineEdit(this);
    globalDZEdit = new QLineEdit(this);
    globalDXEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalDYEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalDZEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalDXEdit->setAlignment(Qt::AlignRight);
    globalDYEdit->setAlignment(Qt::AlignRight);
    globalDZEdit->setAlignment(Qt::AlignRight);

    connect(globalDXEdit, &QLineEdit::textEdited, this, &ChangeGlobalJointDialog::checkGlobalDXYZModified);
    connect(globalDYEdit, &QLineEdit::textEdited, this, &ChangeGlobalJointDialog::checkGlobalDXYZModified);
    connect(globalDZEdit, &QLineEdit::textEdited, this, &ChangeGlobalJointDialog::checkGlobalDXYZModified);

    QGridLayout *glay2 = new QGridLayout;
    glay2->setSpacing(5);
    glay2->addWidget(globalDXLabel, 0, 0, 1, 1);
    glay2->addWidget(globalDXEdit, 0, 1, 1, 2);
    glay2->addWidget(globalDYLabel, 1, 0, 1, 1);
    glay2->addWidget(globalDYEdit, 1, 1, 1, 2);
    glay2->addWidget(globalDZLabel, 2, 0, 1, 1);
    glay2->addWidget(globalDZEdit, 2, 1, 1, 2);

    globalDeltaButton = new QPushButton(u8"移動量の反映", this);
    connect( globalDeltaButton, &QAbstractButton::clicked, this, &ChangeGlobalJointDialog::slotChangeGlobalDXYZ );

    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->addLayout(glay2, 1);
    vlay2->addWidget(globalDeltaButton, 0);


    QHBoxLayout *hlay12 = new QHBoxLayout;
    hlay12->addLayout(vlay1);
    hlay12->addWidget(new VFLine(Qt::lightGray, this));
    hlay12->addLayout(vlay2);

    /*　階・通りの属性の変更 */

    QLabel *floorFrameLabel = new QLabel(u8"【階・通りの属性の変更】", this);

    clearFloorButton = new QPushButton(u8"階属性のクリア", this);
    appendFloorButton = new QPushButton(u8"階属性の追加", this);
    clearFrameButton = new QPushButton(u8"通り属性のクリア", this);
    appendFrameButton = new QPushButton(u8"通り属性の追加", this);

    clearFloorButton->setFixedWidth(100);
    appendFloorButton->setFixedWidth(100);
    clearFrameButton->setFixedWidth(100);
    appendFrameButton->setFixedWidth(100);

    connect( clearFloorButton, &QAbstractButton::clicked, this, &ChangeGlobalJointDialog::slotClearFloorAttachment );
    connect( appendFloorButton, &QAbstractButton::clicked, this, &ChangeGlobalJointDialog::slotAppendFloorAttachment );
    connect( clearFrameButton, &QAbstractButton::clicked, this, &ChangeGlobalJointDialog::slotClearFrameAttachment );
    connect( appendFrameButton, &QAbstractButton::clicked, this, &ChangeGlobalJointDialog::slotAppendFrameAttachment );

    floorEdit = new QLineEdit(this);
    frameEdit = new QLineEdit(this);

    connect(floorEdit, &QLineEdit::textEdited, this, &ChangeGlobalJointDialog::checkAppendFloorModified);
    connect(frameEdit, &QLineEdit::textEdited, this, &ChangeGlobalJointDialog::checkAppendFrameModified);

    QHBoxLayout *hlay3 = new QHBoxLayout;
    hlay3->addWidget(clearFloorButton);
    hlay3->addWidget(new VFLine(Qt::lightGray, this));
    hlay3->addWidget(floorEdit);
    hlay3->addWidget(appendFloorButton);

    QHBoxLayout *hlay4 = new QHBoxLayout;
    hlay4->addWidget(clearFrameButton);
    hlay4->addWidget(new VFLine(Qt::lightGray, this));
    hlay4->addWidget(frameEdit);
    hlay4->addWidget(appendFrameButton);

    QHBoxLayout *hlay5 = new QHBoxLayout;
    hlay5->addWidget(floorFrameLabel);
    hlay5->addStretch();

    /*　全体レイアウト */

    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    QHBoxLayout *hlay6 = new QHBoxLayout;
    hlay6->addStretch();
    hlay6->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(hlay1);
    vlay->addLayout(hlay12);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay5);
    vlay->addLayout(hlay3);
    vlay->addLayout(hlay4);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addStretch();
    vlay->addLayout(hlay6);

    this->setLayout(vlay);
}

void ChangeGlobalJointDialog::slotChangeGlobalXYZ()
{
    bool ok_x = false, ok_y = false, ok_z = false;
    if ( loadValues.gX != globalXEdit->text() ) ok_x = true;
    if ( loadValues.gY != globalYEdit->text() ) ok_y = true;
    if ( loadValues.gZ != globalZEdit->text() ) ok_z = true;
    qreal xx = 0.0, yy = 0.0, zz = 0.0;
    bool ok1 = true, ok2 = true, ok3 = true;
    if ( ok_x ) xx = globalXEdit->text().toDouble(&ok1);
    if ( ok_y ) yy = globalYEdit->text().toDouble(&ok2);
    if ( ok_z ) zz = globalZEdit->text().toDouble(&ok3);

    if ( ok_x && !ok1 ) {
        warningInvalidValue();
        return;
    }
    if ( ok_y && !ok2 ) {
        warningInvalidValue();
        return;
    }
    if ( ok_z && !ok3 ) {
        warningInvalidValue();
        return;
    }

    if ( !parentWidget->changeJointsGlobalXYZ(ok_x, ok_y, ok_z, xx, yy, zz) ) {
        warningDisableChangeValue();
        return;
    }
    accept();
}

void ChangeGlobalJointDialog::slotChangeGlobalDXYZ()
{
    bool ok_x = false, ok_y = false, ok_z = false;
    if ( loadValues.gdX != globalDXEdit->text() ) ok_x = true;
    if ( loadValues.gdY != globalDYEdit->text() ) ok_y = true;
    if ( loadValues.gdZ != globalDZEdit->text() ) ok_z = true;
    qreal xx = 0.0, yy = 0.0, zz = 0.0;
    bool ok1 = true, ok2 = true, ok3 = true;
    if ( ok_x ) xx = globalDXEdit->text().toDouble(&ok1);
    if ( ok_y ) yy = globalDYEdit->text().toDouble(&ok2);
    if ( ok_z ) zz = globalDZEdit->text().toDouble(&ok3);

    if ( ok_x && !ok1 ) {
        warningInvalidValue();
        return;
    }
    if ( ok_y && !ok2 ) {
        warningInvalidValue();
        return;
    }
    if ( ok_z && !ok3 ) {
        warningInvalidValue();
        return;
    }

    if ( !parentWidget->changJointsGlobalDelta(ok_x, ok_y, ok_z, xx, yy, zz) ) {
        warningDisableChangeValue();
        return;
    }
    accept();
}

void ChangeGlobalJointDialog::slotClearFloorAttachment()
{
    parentWidget->clearFloorAttachment();
    accept();
}

void ChangeGlobalJointDialog::slotAppendFloorAttachment()
{
    if ( !parentWidget->appendFloorAttachment(floorEdit->text()) ) {
        warningDisableChangeFloorFrame();
        return;
    }
    accept();
}

void ChangeGlobalJointDialog::slotClearFrameAttachment()
{
    parentWidget->clearFrameAttachment();
    accept();
}

void ChangeGlobalJointDialog::slotAppendFrameAttachment()
{
    if ( !parentWidget->appendFrameAttachment(frameEdit->text()) ) {
        warningDisableChangeFloorFrame();
        return;
    }
    accept();
}

void ChangeGlobalJointDialog::checkGlobalXYZModified()
{
    bool modified = false;
    if ( loadValues.gX != globalXEdit->text() ) modified = true;
    if ( loadValues.gY != globalYEdit->text() ) modified = true;
    if ( loadValues.gZ != globalZEdit->text() ) modified = true;
    globalXYZButton->setEnabled(modified);
}

void ChangeGlobalJointDialog::checkGlobalDXYZModified()
{
    bool modified = false;
    if ( loadValues.gdX != globalDXEdit->text() ) modified = true;
    if ( loadValues.gdY != globalDYEdit->text() ) modified = true;
    if ( loadValues.gdZ != globalDZEdit->text() ) modified = true;
    globalDeltaButton->setEnabled(modified);
}

void ChangeGlobalJointDialog::checkAppendFloorModified()
{
    bool modified = false;
    if ( loadValues.floors != floorEdit->text() ) modified = true;
    appendFloorButton->setEnabled(modified);
}

void ChangeGlobalJointDialog::checkAppendFrameModified()
{
    bool modified = false;
    if ( loadValues.frames != frameEdit->text() ) modified = true;
    appendFrameButton->setEnabled(modified);
}

void ChangeGlobalJointDialog::warningInvalidValue()
{
    QMessageBox::warning( this,
                          "WARNING",
                          u8"入力された値が不正です。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}

void ChangeGlobalJointDialog::warningDisableChangeValue()
{
    QMessageBox::warning( this,
                          "WARNING",
                          u8"移動後の座標が他の節点と重複しています。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}

void ChangeGlobalJointDialog::warningDisableChangeFloorFrame()
{
    QMessageBox::warning( this, "WARNING",
                          u8"入力された階名・通り名が存在しません。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}
} // namespace post3dapp

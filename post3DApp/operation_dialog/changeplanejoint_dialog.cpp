#include "changeplanejoint_dialog.h"

#include <QtGui>

#include "eventoperation_point.h"
#include "vhline.h"

namespace post3dapp{
ChangePlaneJointDialog::ChangePlaneJointDialog(AbstractEventOperation *parentop, QWidget *parent)
    : parentOperation(parentop), QDialog(parent)
{
    createLayout();

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"節点座標・属性の変更");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void ChangePlaneJointDialog::createLayout()
{
    /*　全体座標系の座標変更 */

    QLabel *globalLabel = new QLabel(u8"【全体座標系の座標変更】", this);
    QLabel *globalCLabel = new QLabel(u8"※階・通り属性は変更されません", this);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(globalLabel);
    hlay1->addStretch();
    hlay1->addWidget(globalCLabel);

    QLabel *globalXLabel = new QLabel(u8"X座標(m) : ", this);
    QLabel *globalYLabel = new QLabel(u8"Y座標(m) : ", this);
    QLabel *globalZLabel = new QLabel(u8"Z座標(m) : ", this);

    globalXEdit = new QLineEdit(this);
    globalYEdit = new QLineEdit(this);
    globalZEdit = new QLineEdit(this);
    globalXEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalYEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalZEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalXEdit->setAlignment(Qt::AlignRight);
    globalYEdit->setAlignment(Qt::AlignRight);
    globalZEdit->setAlignment(Qt::AlignRight);

    connect(globalXEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkGlobalXYZModified);
    connect(globalYEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkGlobalXYZModified);
    connect(globalZEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkGlobalXYZModified);

    QGridLayout *glay1 = new QGridLayout;
    glay1->setSpacing(5);
    glay1->addWidget(globalXLabel, 0, 0, 1, 1);
    glay1->addWidget(globalXEdit, 0, 1, 1, 2);
    glay1->addWidget(globalYLabel, 1, 0, 1, 1);
    glay1->addWidget(globalYEdit, 1, 1, 1, 2);
    glay1->addWidget(globalZLabel, 2, 0, 1, 1);
    glay1->addWidget(globalZEdit, 2, 1, 1, 2);

    globalXYZButton = new QPushButton(u8"座標値の変更", this);
    connect( globalXYZButton, &QAbstractButton::clicked, this, &ChangePlaneJointDialog::slotChangeGlobalXYZ );

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->addLayout(glay1, 1);
    vlay1->addWidget(globalXYZButton, 0);

    QLabel *globalDXLabel = new QLabel(u8"ΔX(m) : ", this);
    QLabel *globalDYLabel = new QLabel(u8"ΔY(m) : ", this);
    QLabel *globalDZLabel = new QLabel(u8"ΔZ(m) : ", this);

    globalDXEdit = new QLineEdit(this);
    globalDYEdit = new QLineEdit(this);
    globalDZEdit = new QLineEdit(this);
    globalDXEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalDYEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalDZEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    globalDXEdit->setAlignment(Qt::AlignRight);
    globalDYEdit->setAlignment(Qt::AlignRight);
    globalDZEdit->setAlignment(Qt::AlignRight);

    connect(globalDXEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkGlobalDXYZModified);
    connect(globalDYEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkGlobalDXYZModified);
    connect(globalDZEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkGlobalDXYZModified);

    QGridLayout *glay2 = new QGridLayout;
    glay2->setSpacing(5);
    glay2->addWidget(globalDXLabel, 0, 0, 1, 1);
    glay2->addWidget(globalDXEdit, 0, 1, 1, 2);
    glay2->addWidget(globalDYLabel, 1, 0, 1, 1);
    glay2->addWidget(globalDYEdit, 1, 1, 1, 2);
    glay2->addWidget(globalDZLabel, 2, 0, 1, 1);
    glay2->addWidget(globalDZEdit, 2, 1, 1, 2);

    globalDeltaButton = new QPushButton(u8"移動量の反映", this);
    connect( globalDeltaButton, &QAbstractButton::clicked, this, &ChangePlaneJointDialog::slotChangeGlobalDXYZ );

    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->addLayout(glay2, 1);
    vlay2->addWidget(globalDeltaButton, 0);


    QHBoxLayout *hlay12 = new QHBoxLayout;
    hlay12->addLayout(vlay1);
    hlay12->addWidget(new VFLine(Qt::lightGray, this));
    hlay12->addLayout(vlay2);

    /*　伏図・軸組図座標系の座標変更 */

    QLabel *localLabel = new QLabel(u8"【伏図・軸組図座標系の座標変更】", this);
    changeAttachButton = new QButtonGroup;
    QRadioButton *localButton1 = new QRadioButton(u8"座標変更後も元の階・通り属性を保持する", this);
    QRadioButton *localButton2 = new QRadioButton(u8"座標変更後の位置に階・通り属性を変更する", this);
    changeAttachButton->addButton(localButton1, 0);
    changeAttachButton->addButton(localButton2, 1);
    localButton1->setChecked(true);

    QVBoxLayout *vlay3 = new QVBoxLayout;
    vlay3->setSpacing(0);
    vlay3->addWidget(localButton1);
    vlay3->addWidget(localButton2);

    QLabel *localXLabel = new QLabel(u8"x座標(m) : ", this);
    QLabel *localYLabel = new QLabel(u8"y座標(m) : ", this);
    QLabel *localZLabel = new QLabel(u8"z座標(m) : ", this);

    localXEdit = new QLineEdit(this);
    localYEdit = new QLineEdit(this);
    localZEdit = new QLineEdit(this);
    localXEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    localYEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    localZEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    localXEdit->setAlignment(Qt::AlignRight);
    localYEdit->setAlignment(Qt::AlignRight);
    localZEdit->setAlignment(Qt::AlignRight);

    connect(localXEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkLocalXYZModified);
    connect(localYEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkLocalXYZModified);
    connect(localZEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkLocalXYZModified);

    QGridLayout *glay3 = new QGridLayout;
    glay3->setSpacing(5);
    glay3->addWidget(localXLabel, 0, 0, 1, 1);
    glay3->addWidget(localXEdit, 0, 1, 1, 2);
    glay3->addWidget(localYLabel, 1, 0, 1, 1);
    glay3->addWidget(localYEdit, 1, 1, 1, 2);
    glay3->addWidget(localZLabel, 2, 0, 1, 1);
    glay3->addWidget(localZEdit, 2, 1, 1, 2);

    localXYZButton = new QPushButton(u8"座標値・属性の変更", this);
    connect( localXYZButton, &QAbstractButton::clicked, this, &ChangePlaneJointDialog::slotChangeLocalXYZ );

    QVBoxLayout *vlay4 = new QVBoxLayout;
    vlay4->addLayout(glay3, 1);
    vlay4->addWidget(localXYZButton, 0);

    QLabel *localDXLabel = new QLabel(u8"Δx(m) : ", this);
    QLabel *localDYLabel = new QLabel(u8"Δy(m) : ", this);
    QLabel *localDZLabel = new QLabel(u8"Δz(m) : ", this);

    localDXEdit = new QLineEdit(this);
    localDYEdit = new QLineEdit(this);
    localDZEdit = new QLineEdit(this);
    localDXEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    localDYEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    localDZEdit->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    localDXEdit->setAlignment(Qt::AlignRight);
    localDYEdit->setAlignment(Qt::AlignRight);
    localDZEdit->setAlignment(Qt::AlignRight);

    connect(localDXEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkLocalDXYZModified);
    connect(localDYEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkLocalDXYZModified);
    connect(localDZEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkLocalDXYZModified);

    QGridLayout *glay4 = new QGridLayout;
    glay4->setSpacing(5);
    glay4->addWidget(localDXLabel, 0, 0, 1, 1);
    glay4->addWidget(localDXEdit, 0, 1, 1, 2);
    glay4->addWidget(localDYLabel, 1, 0, 1, 1);
    glay4->addWidget(localDYEdit, 1, 1, 1, 2);
    glay4->addWidget(localDZLabel, 2, 0, 1, 1);
    glay4->addWidget(localDZEdit, 2, 1, 1, 2);

    localDeltaButton = new QPushButton(u8"移動量の反映", this);
    connect( localDeltaButton, &QAbstractButton::clicked, this, &ChangePlaneJointDialog::slotChangeLocalDXYZ );

    QVBoxLayout *vlay5 = new QVBoxLayout;
    vlay5->addLayout(glay4, 1);
    vlay5->addWidget(localDeltaButton, 0);

    QHBoxLayout *hlay34 = new QHBoxLayout;
    hlay34->addLayout(vlay4);
    hlay34->addWidget(new VFLine(Qt::lightGray, this));
    hlay34->addLayout(vlay5);

    /*　階・通りの属性の変更 */

    QLabel *floorFrameLabel = new QLabel(u8"＜階・通りの属性の変更＞", this);

    QLabel *floorLabel = new QLabel(u8"所属階:", this);
    QLabel *frameLabel = new QLabel(u8"所属通り:", this);

    floorEdit = new QLineEdit(this);
    frameEdit = new QLineEdit(this);

    connect(floorEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkFloorFrameModified);
    connect(frameEdit, &QLineEdit::textEdited, this, &ChangePlaneJointDialog::checkFloorFrameModified);

    QGridLayout *glay5 = new QGridLayout;
    glay5->setSpacing(5);
    glay5->addWidget(floorLabel, 0, 0, 1, 1);
    glay5->addWidget(floorEdit, 0, 1, 1, 5);
    glay5->addWidget(frameLabel, 1, 0, 1, 1);
    glay5->addWidget(frameEdit, 1, 1, 1, 5);

    floorFrameButton = new QPushButton(u8"属性の変更", this);
    connect( floorFrameButton, &QAbstractButton::clicked, this, &ChangePlaneJointDialog::slotChangeFloorFrame );

    QHBoxLayout *hlay5 = new QHBoxLayout;
    hlay5->addWidget(floorFrameLabel);
    hlay5->addStretch();
    hlay5->addWidget(floorFrameButton);

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
    vlay->addWidget(localLabel);
    vlay->addLayout(vlay3);
    vlay->addLayout(hlay34);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addLayout(hlay5);
    vlay->addLayout(glay5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addStretch();
    vlay->addLayout(hlay6);

    this->setLayout(vlay);
}

void ChangePlaneJointDialog::setValues(const PLANEJOINTVALUES &values)
{
    loadValues = values;
    globalXEdit->setText(loadValues.gX);
    globalYEdit->setText(loadValues.gY);
    globalZEdit->setText(loadValues.gZ);
    globalDXEdit->setText(loadValues.gdX);
    globalDYEdit->setText(loadValues.gdY);
    globalDZEdit->setText(loadValues.gdZ);
    localXEdit->setText(loadValues.lX);
    localYEdit->setText(loadValues.lY);
    localZEdit->setText(loadValues.lZ);
    localDXEdit->setText(loadValues.ldX);
    localDYEdit->setText(loadValues.ldY);
    localDZEdit->setText(loadValues.ldZ);
    floorEdit->setText(loadValues.floors);
    frameEdit->setText(loadValues.frames);
    globalXYZButton->setEnabled(false);
    globalDeltaButton->setEnabled(false);
    localXYZButton->setEnabled(false);
    localDeltaButton->setEnabled(false);
    floorFrameButton->setEnabled(false);
}

void ChangePlaneJointDialog::slotChangeGlobalXYZ()
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

    if ( !parentOperation->changeSelectedJointsGlobalXYZ(ok_x, ok_y, ok_z, xx, yy, zz) ) {
        warningDisableChangeValue();
    }
}

void ChangePlaneJointDialog::slotChangeGlobalDXYZ()
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

    if ( !parentOperation->changeSelectedJointsGlobalDelta(ok_x, ok_y, ok_z, xx, yy, zz) ) {
        warningDisableChangeValue();
    }
}

void ChangePlaneJointDialog::slotChangeLocalXYZ()
{
    bool ok_x = false, ok_y = false, ok_z = false;
    if ( loadValues.lX != localXEdit->text() ) ok_x = true;
    if ( loadValues.lY != localYEdit->text() ) ok_y = true;
    if ( loadValues.lZ != localZEdit->text() ) ok_z = true;
    qreal xx = 0.0, yy = 0.0, zz = 0.0;
    bool ok1 = true, ok2 = true, ok3 = true;
    if ( ok_x ) xx = localXEdit->text().toDouble(&ok1);
    if ( ok_y ) yy = localYEdit->text().toDouble(&ok2);
    if ( ok_z ) zz = localZEdit->text().toDouble(&ok3);

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

    if ( !parentOperation->changeSelectedJointsLocalXYZ(changeAttachButton->checkedId() == 1,
                                                        ok_x, ok_y, ok_z, xx, yy, zz) ) {
        warningDisableChangeValue();
    }
}

void ChangePlaneJointDialog::slotChangeLocalDXYZ()
{
    bool ok_x = false, ok_y = false, ok_z = false;
    if ( loadValues.ldX != localDXEdit->text() ) ok_x = true;
    if ( loadValues.ldY != localDYEdit->text() ) ok_y = true;
    if ( loadValues.ldZ != localDZEdit->text() ) ok_z = true;
    qreal xx = 0.0, yy = 0.0, zz = 0.0;
    bool ok1 = true, ok2 = true, ok3 = true;
    if ( ok_x ) xx = localDXEdit->text().toDouble(&ok1);
    if ( ok_y ) yy = localDYEdit->text().toDouble(&ok2);
    if ( ok_z ) zz = localDZEdit->text().toDouble(&ok3);

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

    if ( !parentOperation->changeSelectedJointsLocalDelta(changeAttachButton->checkedId() == 1,
                                                          ok_x, ok_y, ok_z, xx, yy, zz) ) {
        warningDisableChangeValue();
    }
}

void ChangePlaneJointDialog::slotChangeFloorFrame()
{
    bool ok_fl = false, ok_fr = false;
    if ( loadValues.floors != floorEdit->text() ) ok_fl = true;
    if ( loadValues.frames != frameEdit->text() ) ok_fr = true;

    if ( !parentOperation->changeSelectedJointsAttachment(ok_fl, ok_fr, floorEdit->text(),
                                                          frameEdit->text()) ) {
        warningDisableChangeFloorFrame();
    }
}

void ChangePlaneJointDialog::checkGlobalXYZModified()
{
    bool modified = false;
    if ( loadValues.gX != globalXEdit->text() ) modified = true;
    if ( loadValues.gY != globalYEdit->text() ) modified = true;
    if ( loadValues.gZ != globalZEdit->text() ) modified = true;
    globalXYZButton->setEnabled(modified);
}

void ChangePlaneJointDialog::checkGlobalDXYZModified()
{
    bool modified = false;
    if ( loadValues.gdX != globalDXEdit->text() ) modified = true;
    if ( loadValues.gdY != globalDYEdit->text() ) modified = true;
    if ( loadValues.gdZ != globalDZEdit->text() ) modified = true;
    globalDeltaButton->setEnabled(modified);
}

void ChangePlaneJointDialog::checkLocalXYZModified()
{
    bool modified = false;
    if ( loadValues.lX != localXEdit->text() ) modified = true;
    if ( loadValues.lY != localYEdit->text() ) modified = true;
    if ( loadValues.lZ != localZEdit->text() ) modified = true;
    localXYZButton->setEnabled(modified);
}

void ChangePlaneJointDialog::checkLocalDXYZModified()
{
    bool modified = false;
    if ( loadValues.ldX != localDXEdit->text() ) modified = true;
    if ( loadValues.ldY != localDYEdit->text() ) modified = true;
    if ( loadValues.ldZ != localDZEdit->text() ) modified = true;
    localDeltaButton->setEnabled(modified);
}

void ChangePlaneJointDialog::checkFloorFrameModified()
{
    bool modified = false;
    if ( loadValues.floors != floorEdit->text() ) modified = true;
    if ( loadValues.frames != frameEdit->text() ) modified = true;
    floorFrameButton->setEnabled(modified);
}


void ChangePlaneJointDialog::warningInvalidValue()
{
    QMessageBox::warning( this,
                          "WARNING",
                          u8"入力された値が不正です。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}

void ChangePlaneJointDialog::warningDisableChangeValue()
{
    QMessageBox::warning( this,
                          "WARNING",
                          u8"移動後の座標が他の節点と重複しています。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}

void ChangePlaneJointDialog::warningDisableChangeFloorFrame()
{
    QMessageBox::warning( this,
                          "WARNING",
                          u8"入力された階名・通り名が存在しません。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}
} // namespace post3dapp

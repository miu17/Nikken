#include "persCL_settingdialog.h"

#include <QtGui>

#include "typicalcolor_combobox.h"
#include "utility.h"
namespace post3dapp{
PersCalcLoadSettingDialog::PersCalcLoadSettingDialog(QWidget *parent)
    : QDialog(parent)
{
    createLayout();

    setFont( QFont(tr("Microsoft JhengHei"), 9) );
    setWindowTitle(u8"個別ウィンドウの表示設定");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void PersCalcLoadSettingDialog::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    QDialog::closeEvent(event);
}

void PersCalcLoadSettingDialog::setCenterPosition( qreal xx, qreal yy, qreal zz )
{
    mySettings.centerX = xx;
    mySettings.centerY = yy;
    mySettings.centerZ = zz;
}

void PersCalcLoadSettingDialog::setCurrentValues()
{
    backColorComboBox->setColor(mySettings.backColor);

    oxLine->setText(QString("%1").arg(mySettings.centerX / 1000.0, 0, 'f', 2));
    oyLine->setText(QString("%1").arg(mySettings.centerY / 1000.0, 0, 'f', 2));
    ozLine->setText(QString("%1").arg(mySettings.centerZ / 1000.0, 0, 'f', 2));

    textCheck->setChecked(mySettings.isTextOn);
    jointCheck->setChecked(mySettings.isJointOn);
    axisCheck->setChecked(mySettings.isAxisOn);
    slotAxisValuesEnable(mySettings.isAxisOn);
    axisLine->setText(QString("%1").arg(mySettings.axisLength / 1000.0, 0, 'f', 2));
}

void PersCalcLoadSettingDialog::createLayout()
{
    /* 背景 */
    QGroupBox *backGroundGroup = new QGroupBox(u8"背景", this);

    QLabel *colorLabel = new QLabel(u8"背景の色:", this);
    backColorComboBox = new TypicalColorComboBox(this);

    QHBoxLayout *hlay_color = new QHBoxLayout;
    hlay_color->addWidget(colorLabel);
    hlay_color->addWidget(backColorComboBox);
    hlay_color->addStretch();

    backGroundGroup->setLayout(hlay_color);

    /* パースの中心位置 */
    QGroupBox *originGroup = new QGroupBox(u8"パースの中心位置", this);

    oxLine = new QLineEdit(this);
    oxLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    oxLine->setAlignment(Qt::AlignRight);
    oxLine->setFixedWidth(80);
    oyLine = new QLineEdit(this);
    oyLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    oyLine->setAlignment(Qt::AlignRight);
    oyLine->setFixedWidth(80);
    ozLine = new QLineEdit(this);
    ozLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    ozLine->setAlignment(Qt::AlignRight);
    ozLine->setFixedWidth(80);

    QHBoxLayout *hlay_origin = new QHBoxLayout;
    hlay_origin->addWidget(new QLabel(u8"X座標(m):", this));
    hlay_origin->addWidget(oxLine);
    hlay_origin->addSpacing(10);
    hlay_origin->addWidget(new QLabel(u8"Y座標(m):", this));
    hlay_origin->addWidget(oyLine);
    hlay_origin->addSpacing(10);
    hlay_origin->addWidget(new QLabel(u8"Z座標(m):", this));
    hlay_origin->addWidget(ozLine);
    hlay_origin->addStretch();

    originGroup->setLayout(hlay_origin);

    /* 表示・非表示 */

    QGroupBox *onoffGroup = new QGroupBox(u8"表示・非表示", this);

    textCheck = new QCheckBox(u8"符号を表示する。", this);
    jointCheck = new QCheckBox(u8"節点を表示する。", this);
    axisCheck = new QCheckBox(u8"XYZ軸を表示する。", this);
    axisLine = new QLineEdit(this);
    axisLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    axisLine->setAlignment(Qt::AlignRight);
    axisLine->setFixedWidth(80);

    QHBoxLayout *hlay_fugoCheck = new QHBoxLayout;
    hlay_fugoCheck->addWidget(textCheck);
    hlay_fugoCheck->addStretch();

    QHBoxLayout *hlay_jointCheck = new QHBoxLayout;
    hlay_jointCheck->addWidget(jointCheck);
    hlay_jointCheck->addStretch();

    QHBoxLayout *hlay_axisCheck = new QHBoxLayout;
    hlay_axisCheck->addWidget(axisCheck);
    hlay_axisCheck->addSpacing(10);
    hlay_axisCheck->addWidget(new QLabel(u8"軸の長さ(m):", this));
    hlay_axisCheck->addWidget(axisLine);
    hlay_axisCheck->addStretch();

    QVBoxLayout *vlay_axis = new QVBoxLayout;
    vlay_axis->addLayout(hlay_fugoCheck);
    vlay_axis->addLayout(hlay_jointCheck);
    vlay_axis->addLayout(hlay_axisCheck);

    onoffGroup->setLayout(vlay_axis);

    connect( axisCheck, &QAbstractButton::toggled, this, &PersCalcLoadSettingDialog::slotAxisValuesEnable );

    /* OK Cancel */
    QPushButton *okButton = new QPushButton(u8"変　更", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &PersCalcLoadSettingDialog::slotChangeViewSetting );
    connect( cancelButton, &QAbstractButton::clicked, this, &PersCalcLoadSettingDialog::reject );

    QHBoxLayout *hlay_okcancel = new QHBoxLayout;
    hlay_okcancel->addStretch();
    hlay_okcancel->addWidget(okButton);
    hlay_okcancel->addWidget(cancelButton);

    /* レイアウト */
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setSpacing(5);
    vlay->addWidget(backGroundGroup);
    vlay->addWidget(originGroup);
    vlay->addWidget(onoffGroup);
    vlay->addStretch();
    vlay->addLayout(hlay_okcancel);

    this->setLayout(vlay);
}

void PersCalcLoadSettingDialog::slotAxisValuesEnable(bool onoff)
{
    axisLine->setEnabled(onoff);
}

void PersCalcLoadSettingDialog::slotChangeViewSetting()
{
    bool ok = true;

    QColor _color = backColorComboBox->color();
    qreal _ox = oxLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _oy = oyLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _oz = ozLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    bool _fugoon = textCheck->isChecked();
    bool _jointon = jointCheck->isChecked();
    bool _axison = axisCheck->isChecked();
    qreal _alen = axisLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }

    mySettings = PERSSETTINGVALUES();
    mySettings.backColor = _color;
    mySettings.centerX = _ox;
    mySettings.centerY = _oy;
    mySettings.centerZ = _oz;
    mySettings.isTextOn = _fugoon;
    mySettings.isJointOn = _jointon;
    mySettings.isAxisOn = _axison;
    mySettings.axisLength = _alen;

    emit windowClosed();
    emit viewSettingChanged();
    this->accept();
}

} // namespace post3dapp

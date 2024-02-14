#include "persUI_settingdialog.h"

#include <QtGui>
#include "typicalcolor_combobox.h"
#include "utility.h"
namespace post3dapp{
PersInterfaceSettingDialog::PersInterfaceSettingDialog(QWidget *parent)
    : QDialog(parent)
{
    createLayout();

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle( u8"個別ウィンドウの表示設定");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void PersInterfaceSettingDialog::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    QDialog::closeEvent(event);
}

void PersInterfaceSettingDialog::setCenterPosition( qreal xx, qreal yy, qreal zz )
{
    mySettings.centerX = xx;
    mySettings.centerY = yy;
    mySettings.centerZ = zz;
}

void PersInterfaceSettingDialog::setCurrentValues()
{
    drawingStyleButtons->button(mySettings.drawingStyle)->setChecked(true);

    backColorComboBox->setColor(mySettings.backColor);

    oxLine->setText(QString("%1").arg(mySettings.centerX / 1000.0, 0, 'f', 2));
    oyLine->setText(QString("%1").arg(mySettings.centerY / 1000.0, 0, 'f', 2));
    ozLine->setText(QString("%1").arg(mySettings.centerZ / 1000.0, 0, 'f', 2));

    light1Check->setChecked(mySettings.isLight1On);
    slotLight1Enable(mySettings.isLight1On);
    l1xLine->setText(QString("%1").arg(mySettings.light1X, 0, 'f', 2));
    l1yLine->setText(QString("%1").arg(mySettings.light1Y, 0, 'f', 2));
    l1zLine->setText(QString("%1").arg(mySettings.light1Z, 0, 'f', 2));
    light2Check->setChecked(mySettings.isLight2On);
    slotLight2Enable(mySettings.isLight2On);
    l2xLine->setText(QString("%1").arg(mySettings.light2X, 0, 'f', 2));
    l2yLine->setText(QString("%1").arg(mySettings.light2Y, 0, 'f', 2));
    l2zLine->setText(QString("%1").arg(mySettings.light2Z, 0, 'f', 2));
    light3Check->setChecked(mySettings.isLight3On);
    slotLight3Enable(mySettings.isLight3On);
    l3xLine->setText(QString("%1").arg(mySettings.light3X, 0, 'f', 2));
    l3yLine->setText(QString("%1").arg(mySettings.light3Y, 0, 'f', 2));
    l3zLine->setText(QString("%1").arg(mySettings.light3Z, 0, 'f', 2));

    clippingCheck->setChecked(mySettings.isClippingOn);
    slotClippingValuesEnable(mySettings.isClippingOn);
    caLine->setText(QString("%1").arg(mySettings.clippingCorA, 0, 'f', 2));
    cbLine->setText(QString("%1").arg(mySettings.clippingCorB, 0, 'f', 2));
    ccLine->setText(QString("%1").arg(mySettings.clippingCorC, 0, 'f', 2));
    cdLine->setText(QString("%1").arg(mySettings.clippingCorD / 1000.0, 0, 'f', 2));

    gridCheck->setChecked(mySettings.isGridOn);
    slotGridValuesEnable(mySettings.isGridOn);
    gxLine->setText(QString("%1").arg(mySettings.gridX / 1000.0, 0, 'f', 2));
    gyLine->setText(QString("%1").arg(mySettings.gridY / 1000.0, 0, 'f', 2));
    gzLine->setText(QString("%1").arg(mySettings.gridZ / 1000.0, 0, 'f', 2));
    ptLine->setText(QString("%1").arg(mySettings.gridPitch / 1000.0, 0, 'f', 2));
    ixSpin->setValue(mySettings.gridCountX);
    iySpin->setValue(mySettings.gridCountY);

    textCheck->setChecked(mySettings.isTextOn);
    jointCheck->setChecked(mySettings.isJointOn);
    axisCheck->setChecked(mySettings.isAxisOn);
    slotAxisValuesEnable(mySettings.isAxisOn);
    axisLine->setText(QString("%1").arg(mySettings.axisLength / 1000.0, 0, 'f', 2));
}

void PersInterfaceSettingDialog::createLayout()
{
    /* 表現 */
    QGroupBox *drawingStyleGroup = new QGroupBox(u8"部材の表現方法", this);

    QRadioButton *polygonButton = new QRadioButton(u8"ポリゴン", this);
    QRadioButton *wireButton = new QRadioButton(u8"ワイヤー", this);
    QRadioButton *lineButton = new QRadioButton(u8"ライン", this);
    drawingStyleButtons = new QButtonGroup;
    drawingStyleButtons->addButton(polygonButton, 0);
    drawingStyleButtons->addButton(wireButton, 1);
    drawingStyleButtons->addButton(lineButton, 2);

    QHBoxLayout *hlay_style = new QHBoxLayout;
    hlay_style->addWidget(polygonButton);
    hlay_style->addSpacing(10);
    hlay_style->addWidget(wireButton);
    hlay_style->addSpacing(10);
    hlay_style->addWidget(lineButton);
    hlay_style->addStretch();

    drawingStyleGroup->setLayout(hlay_style);

    /* 背景 */
    QGroupBox *backGroundGroup = new QGroupBox(u8"背景", this);
    backColorComboBox = new TypicalColorComboBox(this);

    QHBoxLayout *hlay_color = new QHBoxLayout;
    hlay_color->addWidget(new QLabel(u8"背景の色:", this));
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

    /* 光源 */
    QGroupBox *lightGroup = new QGroupBox(u8"光源ベクトル", this);

    light1Check = new QCheckBox(u8"光源1をONにする。", this);
    l1xLine = new QLineEdit(this);
    l1xLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    l1xLine->setAlignment(Qt::AlignRight);
    l1xLine->setFixedWidth(70);
    l1yLine = new QLineEdit(this);
    l1yLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    l1yLine->setAlignment(Qt::AlignRight);
    l1yLine->setFixedWidth(70);
    l1zLine = new QLineEdit(this);
    l1zLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    l1zLine->setAlignment(Qt::AlignRight);
    l1zLine->setFixedWidth(70);

    QHBoxLayout *hlay_light1 = new QHBoxLayout;
    hlay_light1->addWidget(light1Check);
    hlay_light1->addSpacing(20);
    hlay_light1->addWidget(new QLabel("ex:", this));
    hlay_light1->addWidget(l1xLine);
    hlay_light1->addSpacing(10);
    hlay_light1->addWidget(new QLabel("ey:", this));
    hlay_light1->addWidget(l1yLine);
    hlay_light1->addSpacing(10);
    hlay_light1->addWidget(new QLabel("ez:", this));
    hlay_light1->addWidget(l1zLine);
    hlay_light1->addStretch();

    light2Check = new QCheckBox(u8"光源2をONにする。", this);
    l2xLine = new QLineEdit(this);
    l2xLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    l2xLine->setAlignment(Qt::AlignRight);
    l2xLine->setFixedWidth(70);
    l2yLine = new QLineEdit(this);
    l2yLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    l2yLine->setAlignment(Qt::AlignRight);
    l2yLine->setFixedWidth(70);
    l2zLine = new QLineEdit(this);
    l2zLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    l2zLine->setAlignment(Qt::AlignRight);
    l2zLine->setFixedWidth(70);

    QHBoxLayout *hlay_light2 = new QHBoxLayout;
    hlay_light2->addWidget(light2Check);
    hlay_light2->addSpacing(20);
    hlay_light2->addWidget(new QLabel("ex:", this));
    hlay_light2->addWidget(l2xLine);
    hlay_light2->addSpacing(10);
    hlay_light2->addWidget(new QLabel("ey:", this));
    hlay_light2->addWidget(l2yLine);
    hlay_light2->addSpacing(10);
    hlay_light2->addWidget(new QLabel("ez:", this));
    hlay_light2->addWidget(l2zLine);
    hlay_light2->addStretch();

    light3Check = new QCheckBox(u8"光源3をONにする。", this);
    l3xLine = new QLineEdit(this);
    l3xLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    l3xLine->setAlignment(Qt::AlignRight);
    l3xLine->setFixedWidth(70);
    l3yLine = new QLineEdit(this);
    l3yLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    l3yLine->setAlignment(Qt::AlignRight);
    l3yLine->setFixedWidth(70);
    l3zLine = new QLineEdit(this);
    l3zLine->setValidator(new QDoubleValidator(-999.99, 999.99, 2, this));
    l3zLine->setAlignment(Qt::AlignRight);
    l3zLine->setFixedWidth(70);

    QHBoxLayout *hlay_light3 = new QHBoxLayout;
    hlay_light3->addWidget(light3Check);
    hlay_light3->addSpacing(20);
    hlay_light3->addWidget(new QLabel("ex:", this));
    hlay_light3->addWidget(l3xLine);
    hlay_light3->addSpacing(10);
    hlay_light3->addWidget(new QLabel("ey:", this));
    hlay_light3->addWidget(l3yLine);
    hlay_light3->addSpacing(10);
    hlay_light3->addWidget(new QLabel("ez:", this));
    hlay_light3->addWidget(l3zLine);
    hlay_light3->addStretch();

    QVBoxLayout *vlay_light = new QVBoxLayout;
    vlay_light->addLayout(hlay_light1);
    vlay_light->addLayout(hlay_light2);
    vlay_light->addLayout(hlay_light3);

    lightGroup->setLayout(vlay_light);

    connect( light1Check, &QAbstractButton::toggled, this, &PersInterfaceSettingDialog::slotLight1Enable );
    connect( light2Check, &QAbstractButton::toggled, this, &PersInterfaceSettingDialog::slotLight2Enable );
    connect( light3Check, &QAbstractButton::toggled, this, &PersInterfaceSettingDialog::slotLight3Enable );

    /* クリッピング */
    QGroupBox *clippingGroup = new QGroupBox(u8"クリッピング", this);

    clippingCheck = new QCheckBox(u8"クリッピングを行う。ax + by + cz + d >= 0  （m単位）", this);
    caLine = new QLineEdit(this);
    caLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    caLine->setAlignment(Qt::AlignRight);
    caLine->setFixedWidth(80);
    cbLine = new QLineEdit(this);
    cbLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    cbLine->setAlignment(Qt::AlignRight);
    cbLine->setFixedWidth(80);
    ccLine = new QLineEdit(this);
    ccLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    ccLine->setAlignment(Qt::AlignRight);
    ccLine->setFixedWidth(80);
    cdLine = new QLineEdit(this);
    cdLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    cdLine->setAlignment(Qt::AlignRight);
    cdLine->setFixedWidth(80);

    QHBoxLayout *hlay_clipcheck = new QHBoxLayout;
    hlay_clipcheck->addWidget(clippingCheck);
    hlay_clipcheck->addStretch();

    QHBoxLayout *hlay_abcd = new QHBoxLayout;
    hlay_abcd->addWidget(new QLabel("a:", this));
    hlay_abcd->addWidget(caLine);
    hlay_abcd->addSpacing(10);
    hlay_abcd->addWidget(new QLabel("b:", this));
    hlay_abcd->addWidget(cbLine);
    hlay_abcd->addSpacing(10);
    hlay_abcd->addWidget(new QLabel("c:", this));
    hlay_abcd->addWidget(ccLine);
    hlay_abcd->addSpacing(10);
    hlay_abcd->addWidget(new QLabel("d:", this));
    hlay_abcd->addWidget(cdLine);
    hlay_abcd->addStretch();

    QVBoxLayout *vlay_clipping = new QVBoxLayout;
    vlay_clipping->addLayout(hlay_clipcheck);
    vlay_clipping->addLayout(hlay_abcd);

    clippingGroup->setLayout(vlay_clipping);
    connect( clippingCheck, &QAbstractButton::toggled, this, &PersInterfaceSettingDialog::slotClippingValuesEnable );


    /* グリッド */
    QGroupBox *gridGroup = new QGroupBox(u8"XYグリッド", this);

    gridCheck = new QCheckBox(u8"XYグリッドを表示する。", this);
    gxLine = new QLineEdit(this);
    gxLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    gxLine->setAlignment(Qt::AlignRight);
    gxLine->setFixedWidth(80);
    gyLine = new QLineEdit(this);
    gyLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    gyLine->setAlignment(Qt::AlignRight);
    gyLine->setFixedWidth(80);
    gzLine = new QLineEdit(this);
    gzLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    gzLine->setAlignment(Qt::AlignRight);
    gzLine->setFixedWidth(80);

    ptLine = new QLineEdit(this);
    ptLine->setValidator(new QDoubleValidator(-9999.99, 9999.99, 2, this));
    ptLine->setAlignment(Qt::AlignRight);
    ptLine->setFixedWidth(80);
    ixSpin = new QSpinBox(this);
    ixSpin->setRange(1, 999);
    iySpin = new QSpinBox(this);
    iySpin->setRange(1, 999);

    QHBoxLayout *hlay_gridcheck = new QHBoxLayout;
    hlay_gridcheck->addWidget(gridCheck);
    hlay_gridcheck->addStretch();

    QGridLayout *glay_grid = new QGridLayout;
    glay_grid->addWidget(new QLabel(u8"X座標(m):", this), 0, 0, 1, 1);
    glay_grid->addWidget(gxLine, 0, 1, 1, 1);
    glay_grid->addWidget(new QLabel(u8"  Y座標(m):", this), 0, 2, 1, 1);
    glay_grid->addWidget(gyLine, 0, 3, 1, 1);
    glay_grid->addWidget(new QLabel(u8"  Z座標(m):", this), 0, 4, 1, 1);
    glay_grid->addWidget(gzLine, 0, 5, 1, 1);
    glay_grid->addWidget(new QLabel(u8"ピッチ(m):", this), 1, 0, 1, 1);
    glay_grid->addWidget(ptLine, 1, 1, 1, 1);
    glay_grid->addWidget(new QLabel(u8"X方向個数:", this), 1, 2, 1, 1);
    glay_grid->addWidget(ixSpin, 1, 3, 1, 1);
    glay_grid->addWidget(new QLabel(u8"  Y方向個数:", this), 1, 4, 1, 1);
    glay_grid->addWidget(iySpin, 1, 5, 1, 1);
    glay_grid->setColumnStretch(6, 2);

    QVBoxLayout *vlay_grid = new QVBoxLayout;
    vlay_grid->addLayout(hlay_gridcheck);
    vlay_grid->addLayout(glay_grid);

    gridGroup->setLayout(vlay_grid);

    connect( gridCheck, &QAbstractButton::toggled, this, &PersInterfaceSettingDialog::slotGridValuesEnable );


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

    connect( axisCheck, &QAbstractButton::toggled, this, &PersInterfaceSettingDialog::slotAxisValuesEnable );

    /* OK Cancel */
    QPushButton *okButton = new QPushButton(u8"変　更", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &PersInterfaceSettingDialog::slotChangeViewSetting );
    connect( cancelButton, &QAbstractButton::clicked, this, &PersInterfaceSettingDialog::reject );

    QHBoxLayout *hlay_okcancel = new QHBoxLayout;
    hlay_okcancel->addStretch();
    hlay_okcancel->addWidget(okButton);
    hlay_okcancel->addWidget(cancelButton);

    /* レイアウト */
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setSpacing(5);
    vlay->addWidget(backGroundGroup);
    vlay->addWidget(drawingStyleGroup);
    vlay->addWidget(originGroup);
    vlay->addWidget(lightGroup);
    vlay->addWidget(clippingGroup);
    vlay->addWidget(gridGroup);
    vlay->addWidget(onoffGroup);
    vlay->addStretch();
    vlay->addLayout(hlay_okcancel);

    this->setLayout(vlay);
}
void PersInterfaceSettingDialog::slotLight1Enable(bool onoff)
{
    l1xLine->setEnabled(onoff);
    l1yLine->setEnabled(onoff);
    l1zLine->setEnabled(onoff);
}

void PersInterfaceSettingDialog::slotLight2Enable(bool onoff)
{
    l2xLine->setEnabled(onoff);
    l2yLine->setEnabled(onoff);
    l2zLine->setEnabled(onoff);
}

void PersInterfaceSettingDialog::slotLight3Enable(bool onoff)
{
    l3xLine->setEnabled(onoff);
    l3yLine->setEnabled(onoff);
    l3zLine->setEnabled(onoff);
}

void PersInterfaceSettingDialog::slotClippingValuesEnable(bool onoff)
{
    caLine->setEnabled(onoff);
    cbLine->setEnabled(onoff);
    ccLine->setEnabled(onoff);
    cdLine->setEnabled(onoff);
}

void PersInterfaceSettingDialog::slotGridValuesEnable(bool onoff)
{
    gxLine->setEnabled(onoff);
    gyLine->setEnabled(onoff);
    gzLine->setEnabled(onoff);
    ptLine->setEnabled(onoff);
    ixSpin->setEnabled(onoff);
    iySpin->setEnabled(onoff);
}

void PersInterfaceSettingDialog::slotAxisValuesEnable(bool onoff)
{
    axisLine->setEnabled(onoff);
}

void PersInterfaceSettingDialog::slotChangeViewSetting()
{
    bool ok = true;

    int _style = drawingStyleButtons->checkedId();
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
    bool _l1on = light1Check->isChecked();
    qreal _l1x = l1xLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _l1y = l1yLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _l1z = l1zLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    bool _l2on = light2Check->isChecked();
    qreal _l2x = l2xLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _l2y = l2yLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _l2z = l2zLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    bool _l3on = light3Check->isChecked();
    qreal _l3x = l3xLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _l3y = l3yLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _l3z = l3zLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    bool _clipon = clippingCheck->isChecked();
    qreal _ca = caLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _cb = cbLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _cc = ccLine->text().toDouble(&ok);
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _cd = cdLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    bool _gridon = gridCheck->isChecked();
    qreal _gx = gxLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _gy = gyLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _gz = gzLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    qreal _pt = ptLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }
    int _ix = ixSpin->value();
    int _iy = iySpin->value();
    bool _fugoon = textCheck->isChecked();
    bool _jointon = jointCheck->isChecked();
    bool _axison = axisCheck->isChecked();
    qreal _alen = axisLine->text().toDouble(&ok) * 1000.0;
    if ( !ok ) {
        Utility::messageInvalidValue(this);
        return;
    }

    mySettings = PERSSETTINGVALUES( _style, _color, _ox, _oy, _oz, _l1on, _l1x, _l1y, _l1z,
                                    _l2on, _l2x, _l2y, _l2z, _l3on, _l3x, _l3y, _l3z,
                                    _clipon, _ca, _cb, _cc, _cd, _gridon, _gx, _gy, _gz, _pt, _ix, _iy,
                                    _fugoon, _jointon, _axison, _alen );
    emit windowClosed();
    emit viewSettingChanged();
    this->accept();
}


} // namespace post3dapp

#include "viewsetting_partswidget.h"

#include <QtGui>

#include "typicalcolor_combobox.h"
#include "unified_editingdata.h"
#include "vhline.h"

namespace post3dapp{
/* 要素の設定ウィジェット  */
ElementWidgetGroup::ElementWidgetGroup( const QString &str, QWidget *parent )
    : QWidget(parent)
{
    elementCheckBox = new QCheckBox( str, this );
    elementCheckBox->setFixedWidth(120);
    nameCheckBox = new QCheckBox(u8"符号の表示", this);
    lineLabel = new QLabel(u8"線色：", this);
    lineComboBox = new TypicalColorComboBox(this);
    brushLabel = new QLabel(u8"塗りつぶし：", this);
    brushComboBox = new TypicalColorComboBox(this);
    transLabel = new QLabel(u8"透過度：", this);
    transSpinBox = new QSpinBox(this);
    transSpinBox->setRange(0, 255);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setContentsMargins(0, 0, 0, 0);
    hlay->addWidget(elementCheckBox);
    hlay->addStretch();
    hlay->addWidget(nameCheckBox);
    hlay->addStretch();
    hlay->addWidget(lineLabel);
    hlay->addWidget(lineComboBox);
    hlay->addStretch();
    hlay->addWidget(brushLabel);
    hlay->addWidget(brushComboBox);
    hlay->addStretch();
    hlay->addWidget(transLabel);
    hlay->addWidget(transSpinBox);

    this->setLayout(hlay);
    connect( elementCheckBox, &QAbstractButton::toggled, this, &ElementWidgetGroup::slotEnableAllWidgets );
}

void ElementWidgetGroup::slotEnableAllWidgets(bool onoff)
{
    nameCheckBox->setEnabled(onoff);
    lineLabel->setEnabled(onoff);
    lineComboBox->setEnabled(onoff);
    brushLabel->setEnabled(onoff);
    brushComboBox->setEnabled(onoff);
    transLabel->setEnabled(onoff);
    transSpinBox->setEnabled(onoff);
}

ELEMENTVIEW ElementWidgetGroup::getElementSetting() const
{
    QColor bcolor = brushComboBox->color();
    if ( bcolor != Qt::transparent ) bcolor.setAlpha( transSpinBox->value() );
    QBrush brush = ( bcolor == Qt::transparent ) ? Qt::NoBrush : QBrush(bcolor);
    return ELEMENTVIEW(elementCheckBox->isChecked(), nameCheckBox->isChecked(),
                       lineComboBox->color(), brush);
}

void ElementWidgetGroup::setElementSetting( const ELEMENTVIEW &setting )
{
    elementCheckBox->setChecked(setting.isVisible);
    nameCheckBox->setChecked(setting.isNameVisible);
    lineComboBox->setColor(setting.lineColor);
    if ( setting.paintBrush == Qt::NoBrush ) {
        brushComboBox->setColor( Qt::transparent );
        transSpinBox->setValue(0);
    } else {
        QColor bcolor = setting.paintBrush.color();
        bcolor.setAlpha(255);
        brushComboBox->setColor( bcolor );
        transSpinBox->setValue( setting.paintBrush.color().alpha() );
    }
}

/* 要素状態の設定ウィジェット  */
ElementStateWidgetGroup::ElementStateWidgetGroup( const QString &str, QWidget *parent )
    : QWidget(parent)
{
    QLabel *elementStateLabel = new QLabel(str, this);
    elementStateLabel->setFixedWidth(120);
    QLabel *lineLabel = new QLabel(u8"線色：", this);
    lineComboBox = new TypicalColorComboBox(this);
    QLabel *brushLabel = new QLabel(u8"塗りつぶし：", this);
    brushComboBox = new TypicalColorComboBox(this);
    QLabel *transLabel = new QLabel(u8"透過度：", this);
    transSpinBox = new QSpinBox(this);
    transSpinBox->setRange(0, 255);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setContentsMargins(0, 0, 0, 0);
    hlay->addWidget(elementStateLabel);
    hlay->addSpacing(20);
    hlay->addWidget(lineLabel);
    hlay->addWidget(lineComboBox);
    hlay->addSpacing(20);
    hlay->addWidget(brushLabel);
    hlay->addWidget(brushComboBox);
    hlay->addSpacing(20);
    hlay->addWidget(transLabel);
    hlay->addWidget(transSpinBox);
    hlay->addStretch();

    this->setLayout(hlay);
}

COLORBRUSHSETTING ElementStateWidgetGroup::getElementSetting() const
{
    QColor bcolor = brushComboBox->color();
    if ( bcolor != Qt::transparent ) bcolor.setAlpha( transSpinBox->value() );
    QBrush brush = ( bcolor == Qt::transparent ) ? Qt::NoBrush : QBrush(bcolor);
    return COLORBRUSHSETTING(lineComboBox->color(), brush);
}

void ElementStateWidgetGroup::setElementSetting( const COLORBRUSHSETTING &setting )
{
    lineComboBox->setColor(setting.lineColor);
    if ( setting.paintBrush == Qt::NoBrush ) {
        brushComboBox->setColor( Qt::transparent );
        transSpinBox->setValue(0);
    } else {
        QColor bcolor = setting.paintBrush.color();
        bcolor.setAlpha(255);
        brushComboBox->setColor( bcolor );
        transSpinBox->setValue( setting.paintBrush.color().alpha() );
    }
}

/* ５サイズコンボボックス  */
FiveSizeComboBox::FiveSizeComboBox(QWidget *widget)
    : QComboBox(widget)
{
    this->addItems({u8"最小サイズ", u8"小さいサイズ", u8"標準サイズ", u8"大きめサイズ", u8"最大サイズ"});
    setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
}

LineStyleWidget::LineStyleWidget(const QString &str, QWidget *widget): QWidget(widget)
{
    chk_visible = new QCheckBox;
//    chk_visible->setEnabled(false);

    lineColorComboBox = new TypicalColorComboBox;
    lineStyleComboBox = new QComboBox;

    QLabel *label = new QLabel(str);
    lineStyleComboBox->addItem(u8"SolidLine");
    lineStyleComboBox->addItem(u8"DashLine");
    lineStyleComboBox->addItem(u8"DashDotLine");
    lineStyleComboBox->addItem(u8"DashDotDotLine");
    lineStyleComboBox->addItem(u8"DotLine");
    QHBoxLayout* hlay=new QHBoxLayout;
    hlay->addWidget(chk_visible);
    hlay->addWidget(label);
    hlay->addStretch();
    hlay->addWidget(lineColorComboBox);
    hlay->addWidget(lineStyleComboBox);
    this->setLayout(hlay);
    connect( chk_visible, &QCheckBox::toggled, this, &LineStyleWidget::slotEnableMinMax );
}
bool LineStyleWidget::getLineVisible() const
{
    return chk_visible->isChecked();
}
QColor LineStyleWidget::getLineColor() const
{
    return lineColorComboBox->color();
}
Qt::PenStyle LineStyleWidget::getLineStyle() const
{
    switch(lineStyleComboBox->currentIndex()){
    case 0:
        return Qt::SolidLine;
        break;
    case 1:
        return Qt::DashLine;
        break;
    case 2:
        return Qt::DashDotLine;
        break;
    case 3:
        return Qt::DashDotDotLine;
        break;
    case 4:
        return Qt::DotLine;
        break;
    default:
        return Qt::SolidLine;
    }
}
void LineStyleWidget::setSetting( bool visible,const QColor& color,Qt::PenStyle pen)
{
    chk_visible->setChecked(visible);
    lineColorComboBox->setColor(color);
    switch(pen){
    case Qt::SolidLine:
        lineStyleComboBox->setCurrentIndex(0);
        break;
    case Qt::DashLine:
        lineStyleComboBox->setCurrentIndex(1);
        break;
    case Qt::DashDotLine:
        lineStyleComboBox->setCurrentIndex(2);
        break;
    case Qt::DashDotDotLine:
        lineStyleComboBox->setCurrentIndex(3);
        break;
    case Qt::DotLine:
        lineStyleComboBox->setCurrentIndex(4);
        break;
    default:
        lineStyleComboBox->setCurrentIndex(5);
    }

}
void LineStyleWidget::slotEnableMinMax(bool onoff)
{
    lineColorComboBox->setEnabled(onoff);
    lineStyleComboBox->setEnabled(onoff);
}
Qt::PenStyle LineStyleWidget::toPenStyle(QString str)
{
    if(str == QString(u8"SolidLine"))return Qt::SolidLine;
    else if(str == QString(u8"DashLine"))return Qt::DashLine;
    else if(str == QString(u8"DashDotLine"))return Qt::DashDotLine;
    else if(str == QString(u8"DashDotDotLine"))return Qt::DashDotDotLine;
    else if(str == QString(u8"DotLine"))return Qt::DotLine;
    else {return Qt::SolidLine;}
}
QString LineStyleWidget::toString(Qt::PenStyle style)
{
    switch(style){
    case Qt::SolidLine:
        return QString(u8"SolidLine");
        break;
    case Qt::DashLine:
        return QString(u8"DashLine");
        break;
    case Qt::DashDotLine:
        return QString(u8"DashDotLine");
        break;
    case Qt::DashDotDotLine:
        return QString(u8"DashDotDotLine");
        break;
    case Qt::DotLine:
        return QString(u8"DotLine");
        break;
    default:
        return QString(u8"SolidLine");
    }
}

/*最大最小値の指定ウィジェット*/

AxisMinMaxWidget::AxisMinMaxWidget(const QString &str, QWidget *widget): QWidget(widget)
{
    QLabel* label = new QLabel(str);
    QRadioButton *radio_auto=new QRadioButton(u8"自動");
    QRadioButton *radio_manual=new QRadioButton(u8"指定");
    radio_autoMinMax = new QButtonGroup;
    radio_autoMinMax->addButton(radio_auto,0);
    radio_autoMinMax->addButton(radio_manual,1);
    QLabel* minlabel = new QLabel(u8"最小：");
    QLabel* maxlabel = new QLabel(u8"最大：");
    minSpinBox = new QDoubleSpinBox;
    minSpinBox->setMinimum(-1000000);
    minSpinBox->setMaximum(1000000);
    maxSpinBox = new QDoubleSpinBox;
    maxSpinBox->setMinimum(-1000000);
    maxSpinBox->setMaximum(1000000);

    QHBoxLayout* hlay = new QHBoxLayout;
    hlay->addWidget(label);
    hlay->addStretch();
    hlay->addWidget(radio_auto);
    hlay->addWidget(radio_manual);
    hlay->addWidget(minlabel);
    hlay->addWidget(minSpinBox);
    hlay->addWidget(maxlabel);
    hlay->addWidget(maxSpinBox);

    connect( radio_auto, &QRadioButton::toggled, this, &AxisMinMaxWidget::slotDisableMinMax );
    connect( radio_manual, &QRadioButton::toggled, this, &AxisMinMaxWidget::slotEnableMinMax );
    this->setLayout(hlay);
}

bool AxisMinMaxWidget::isAutoScale() const
{
    return radio_autoMinMax->checkedId()==0;
}
qreal AxisMinMaxWidget::minValue() const
{
    return minSpinBox->value();
}
qreal AxisMinMaxWidget::maxValue() const
{
    return maxSpinBox->value();

}
void AxisMinMaxWidget::setSetting(bool isAuto,qreal min,qreal max)
{
    minSpinBox->setValue(min);
    maxSpinBox->setValue(max);
    if(isAuto){
        radio_autoMinMax->button(0)->setChecked(true);
    }else{
        radio_autoMinMax->button(1)->setChecked(true);
    }
}
void AxisMinMaxWidget::slotEnableMinMax(bool onoff)
{
    maxSpinBox->setEnabled(onoff);
    minSpinBox->setEnabled(onoff);
}
void AxisMinMaxWidget::slotDisableMinMax(bool onoff)
{
    maxSpinBox->setEnabled(!onoff);
    minSpinBox->setEnabled(!onoff);
}

DownWidgetGroup::DownWidgetGroup(const QString &str, QWidget *widget)
    :QWidget(widget)
{
    QLabel *downLabel = new QLabel(str, this);
    downLabel->setFixedWidth(120);
    nameCheckBox = new QCheckBox(u8"符号の表示", this);
    QLabel *lineLabel = new QLabel(u8"線色：", this);
    lineComboBox = new TypicalColorComboBox(this);
    QLabel *brushLabel = new QLabel(u8"塗りつぶし：", this);
    brushComboBox = new TypicalColorComboBox(this);
    QLabel *transLabel = new QLabel(u8"透過度：", this);
    transSpinBox = new QSpinBox(this);
    transSpinBox->setRange(0, 255);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setContentsMargins(0, 0, 0, 0);
    hlay->addWidget(downLabel);
    hlay->addStretch();
    hlay->addWidget(nameCheckBox);
    hlay->addStretch();
    hlay->addWidget(lineLabel);
    hlay->addWidget(lineComboBox);
    hlay->addStretch();
    hlay->addWidget(brushLabel);
    hlay->addWidget(brushComboBox);
    hlay->addStretch();
    hlay->addWidget(transLabel);
    hlay->addWidget(transSpinBox);

    this->setLayout(hlay);
}

DOWNSETTING DownWidgetGroup::getElementSetting() const
{
    QColor bcolor = brushComboBox->color();
    if ( bcolor != Qt::transparent ) bcolor.setAlpha( transSpinBox->value() );
    QBrush brush = ( bcolor == Qt::transparent ) ? Qt::NoBrush : QBrush(bcolor);
    return DOWNSETTING(nameCheckBox->isChecked(), lineComboBox->color(), brush);
}

void DownWidgetGroup::setElementSetting(const DOWNSETTING &setting)
{
    nameCheckBox->setChecked(setting.isNameVisible);
    lineComboBox->setColor(setting.lineColor);
    if ( setting.paintBrush == Qt::NoBrush ) {
        brushComboBox->setColor( Qt::transparent );
        transSpinBox->setValue(0);
    } else {
        QColor bcolor = setting.paintBrush.color();
        bcolor.setAlpha(255);
        brushComboBox->setColor( bcolor );
        transSpinBox->setValue( setting.paintBrush.color().alpha() );
    }
}

} // namespace post3dapp

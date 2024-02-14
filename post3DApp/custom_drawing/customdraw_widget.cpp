#include "customdraw_widget.h"

#include <QtGui>
#include "customdraw_grview.h"

namespace post3dapp{
CustomDrawWidget::CustomDrawWidget(QWidget *parent)
    : InterfaceWidget(parent)
{
//    setFrameStyle(QFrame::Box|QFrame::Raised);
}

void CustomDrawWidget::createButtons()
{
    zoomInIcon = new QToolButton(this);
    zoomInIcon->setIcon(QIcon(QPixmap(":/icons/zoomin.png")));
    zoomInIcon->setIconSize(QSize(25, 25));
    zoomInIcon->setAutoRaise(true);
    zoomInIcon->setFixedSize(30, 30);
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);

    zoomOutIcon = new QToolButton(this);
    zoomOutIcon->setIcon(QIcon(QPixmap(":/icons/zoomout.png")));
    zoomOutIcon->setIconSize(QSize(25, 25));
    zoomOutIcon->setAutoRaise(true);
    zoomOutIcon->setFixedSize(30, 30);
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);

    zoomSlider = new QSlider(this);
    zoomSlider->setFixedSize(120, 30);
    zoomSlider->setOrientation(Qt::Horizontal);
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(400);
    zoomSlider->setValue(200);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    connect ( zoomSlider, &QAbstractSlider::valueChanged, this, &CustomDrawWidget::slotChangeViewScale );
    connect ( zoomInIcon, &QAbstractButton::clicked, this, &CustomDrawWidget::zoomIn );
    connect ( zoomOutIcon, &QAbstractButton::clicked, this, &CustomDrawWidget::zoomOut );

    fitButton = new QToolButton(this);
    fitButton->setIcon(QIcon(QPixmap(":/icons/fit.png")));
    fitButton->setAutoRaise(true);
    fitButton->setFixedSize(30, 30);
    fitButton->setIconSize(QSize(25, 25));

    connect(fitButton, &QToolButton::clicked, getView(), &CustomDrawGrView::fitWindow);
}

void CustomDrawWidget::createLayout()
{
    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addStretch();
    hlay1->addSpacing(10);
    hlay1->addWidget(zoomOutIcon, 0);
    hlay1->addWidget(zoomSlider, 0);
    hlay1->addWidget(zoomInIcon, 0);
    hlay1->addSpacing(5);
    hlay1->addWidget(fitButton, 0);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->addWidget(getView(), 1);
    vlay->addLayout(hlay1, 0);

    setLayout(vlay);
}

void CustomDrawWidget::slotChangeViewScale()
{
    qDebug()<<"CustomDrawWidget::slotChangeViewScale : in";
    qreal scale = qPow(2.0, qreal(zoomSlider->value() - 200) / 50.0) / 100.0;
    if ( scale < 1.0 / 16.0 / 100.0 || scale > 16.0 / 100.0 ) return;
    getView()->setTransform(QTransform(scale, 0, 0, -scale, 0, 0));
    qDebug()<<"CustomDrawWidget::slotChangeViewScale : out";
}

void CustomDrawWidget::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void CustomDrawWidget::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void CustomDrawWidget::slotChangeSliderValue(qreal factor)
{
    qreal value = qLn(factor * 100.0) / qLn(2.0) * 50.0 + 200.0;
    zoomSlider->setValue(int(value));
}

} // namespace post3dapp

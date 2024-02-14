#include "elided_label.h"

#include<QDebug>
#include <QFontMetricsF>
#include <QPainter>
#include <QPaintEvent>

namespace post3dapp{
ElidedLabel::ElidedLabel( QWidget *parent )
    : QLabel(parent)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}

ElidedLabel::ElidedLabel( const QString &text, QWidget *parent )
    : QLabel(text, parent), myText(text)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}

void ElidedLabel::setText( const QString &str )
{
    myText = str;
    QLabel::setText(str);
}

void ElidedLabel::paintEvent( QPaintEvent * )
{
    QFontMetrics fm = this->fontMetrics();
    if ( fm.width(myText) > width() - 10 ) recalcMyText();

    QPainter painter( this );
    /*
        painter.setRenderHint(QPainter::Antialiasing);
        ( isEnabled() )? painter.setPen(QColor(255,255,255,200))
                       : painter.setPen(QColor(255,255,255,50));
        painter.drawRect( this->rect() );

        if(text().isEmpty())return;
        QColor col=txcol;
        col.setAlpha(txcol.alpha()-150);
        ( isEnabled() )? painter.setPen(txcol)
                       : painter.setPen(col);
    */

    QColor col = this->palette().color(QPalette::WindowText);
    painter.setPen(col);
    painter.drawText(this->rect().translated(5, 0), Qt::AlignLeft | Qt::AlignVCenter, myText);
    col = this->palette().color(QPalette::Mid);
    painter.setPen(col);
    painter.drawRect(this->rect().adjusted(1, 1, -2, -2));
}

void ElidedLabel::resizeEvent( QResizeEvent * )
{
    recalcMyText();
    update();
}

void ElidedLabel::recalcMyText()
{
    QString str = text();
    int ww = width() - 10;
    QFontMetrics fm = this->fontMetrics();
    myText = fm.elidedText(str, Qt::ElideRight, ww);
}
} // namespace post3dapp

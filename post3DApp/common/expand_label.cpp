#include "expand_label.h"

#include <QDebug>
#include <QFontMetricsF>
#include <QPainter>
#include <QPaintEvent>

namespace post3dapp{
ExpandLabel::ExpandLabel( QWidget *parent )
    : QLabel(parent)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}

ExpandLabel::ExpandLabel( const QString &text, QWidget *parent )
    : QLabel(text, parent)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}

void ExpandLabel::paintEvent( QPaintEvent * )
{
    QPainter painter( this );

    QColor col = this->palette().color(QPalette::WindowText);
    painter.setPen(col);

    QRect rect = this->rect();
    QString str = this->text();
    int ww = painter.fontMetrics().width(str);

    if ( ww > rect.width() ) {
        QFont curFont = painter.font();
        qreal factor = qreal(rect.width()) / qreal(ww) * 100.0;
        curFont.setStretch( qRound(factor) );
        painter.setFont(curFont);
        for ( int i = 0; i < str.count(); i++ ) {
            int x1 = qRound( qreal(rect.width()) * qreal(i) / qreal(str.count()) );
            painter.drawText(rect.adjusted(x1, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, str.at(i));
        }
        curFont.setStretch(100);
        painter.setFont(curFont);
    } else {
        painter.drawText(rect, Qt::AlignVCenter | Qt::AlignLeft, str);
    }
}
} // namespace post3dapp

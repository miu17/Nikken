#include "menu_button.h"

#include <QDebug>
#include <QToolButton>
#include <QVBoxLayout>
#include "expand_label.h"

namespace post3dapp{
MenuButton::MenuButton( const QString &text, CBSTATUS status, QWidget *parent )
    : currentStatus(status), QWidget(parent)
{
    textLabel = new ExpandLabel(text, this);
    textLabel->setFixedSize(75, 15);

    toolButton = new QToolButton(this);
    toolButton->setAutoRaise(true);
    toolButton->setIcon(QIcon(QPixmap(":/icons/menu_normal.png")));
    toolButton->setIconSize(QSize(80, 28));
    toolButton->setFixedSize(80, 28);

    connect(toolButton, &QAbstractButton::clicked, this, &MenuButton::signalButtonClicked);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->setContentsMargins(0, 0, 0, 0);
    hlay1->addSpacing(4);
    hlay1->addWidget(textLabel);
    hlay1->addStretch();

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setContentsMargins(0, 0, 0, 0);
    hlay->addWidget(toolButton);
    hlay->addStretch();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addLayout(hlay1);
    vlay->addLayout(hlay);
    vlay->addStretch();

    this->setLayout(vlay);

    resetButtonIcon();
}

void MenuButton::changeButtonColor( CBSTATUS status )
{
    currentStatus = status;
    resetButtonIcon();
}

void MenuButton::resetButtonIcon()
{
    QPalette pal = textLabel->palette();
    QIcon icon;
    switch ( currentStatus ) {
    case CBSTATUS::CB_NORMAL:
        pal.setColor(QPalette::WindowText, Qt::darkBlue);
        icon = QIcon(QPixmap(":/icons/menu_normal.png"));
        break;
    case CBSTATUS::CB_ACTIVE:
        pal.setColor(QPalette::WindowText, Qt::darkGreen);
        icon = QIcon(QPixmap(":/icons/menu_active.png"));
        break;
    case CBSTATUS::CB_DEFINE:
        pal.setColor(QPalette::WindowText, Qt::darkRed);
        icon = QIcon(QPixmap(":/icons/menu_define.png"));
        break;
    case CBSTATUS::CB_DISABLE:
        pal.setColor(QPalette::WindowText, Qt::darkGray);
        icon = QIcon(QPixmap(":/icons/menu_disable.png"));
        break;
    case CBSTATUS::CB_COLOR:
        pal.setColor(QPalette::WindowText, Qt::black);
        icon = QIcon(QPixmap(":/icons/menu.png"));
        break;
    case CBSTATUS::CB_PLAY:
        pal.setColor(QPalette::WindowText, Qt::darkBlue);
        icon = QIcon(QPixmap(":/icons/play_blue.png").scaled(50, 20, Qt::IgnoreAspectRatio,
                                                             Qt::SmoothTransformation));
        break;
    case CBSTATUS::CB_PLAYDONE:
        pal.setColor(QPalette::WindowText, Qt::darkRed);
        icon = QIcon(QPixmap(":/icons/play_red.png").scaled(50, 20, Qt::IgnoreAspectRatio,
                                                            Qt::SmoothTransformation));
        break;
    case CBSTATUS::CB_DEFAULT:
        pal.setColor(QPalette::WindowText, Qt::black);
        icon = QIcon(QPixmap(":/icons/menu_default.png"));
        break;
    default:
        pal.setColor(QPalette::WindowText, Qt::darkBlue);
        icon = QIcon(QPixmap(":/icons/menu_normal.png"));
        break;
    }
    toolButton->setIcon(icon);

//    ( currentStatus==CB_DISABLE ) ? pal.setColor(QPalette::WindowText,Qt::darkGray)
//                                  : pal.setColor(QPalette::WindowText,Qt::black);
    textLabel->setPalette(pal);

    this->update();
}

void MenuButton::signalButtonClicked()
{
    emit clicked();
}
} // namespace post3dapp

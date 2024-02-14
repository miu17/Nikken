#ifndef EXPANDLABEL_H
#define EXPANDLABEL_H

#include <QLabel>
namespace post3dapp{
class ExpandLabel : public QLabel
{
    Q_OBJECT

public:

    ExpandLabel( QWidget *parent = 0 );
    ExpandLabel( const QString &text, QWidget *parent = 0 );

protected:

    void paintEvent( QPaintEvent * );

private:

};
} // namespace post3dapp
#endif

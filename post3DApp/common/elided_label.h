#ifndef ELIDEDLABEL_H
#define ELIDEDLABEL_H

#include <QLabel>

namespace post3dapp{
class ElidedLabel : public QLabel
{
    Q_OBJECT

public:

    ElidedLabel( QWidget *parent = 0 );
    ElidedLabel( const QString &text, QWidget *parent = 0 );

    void setText(const QString &str);

protected:

    void paintEvent( QPaintEvent * ) override;
    void resizeEvent( QResizeEvent * ) override;

private:

    void recalcMyText();
    QString myText;

};
} // namespace post3dapp
#endif

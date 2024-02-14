#ifndef TYPICALCOLORCOMBOBOX_H
#define TYPICALCOLORCOMBOBOX_H

#include <QComboBox>

namespace post3dapp{

class TypicalColorComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor USER true)

public:
    TypicalColorComboBox(QWidget *widget = nullptr);

    QColor color() const;
    QString name() const;
    void setColor(QColor c);
    static int nameToIndex(const QString&);
    static int colorToIndex(const QColor &);

private:
    void populateList();
};
} // namespace post3dapp
#endif

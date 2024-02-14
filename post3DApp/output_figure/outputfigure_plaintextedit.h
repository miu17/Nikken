#ifndef OUTPUTFIGUREPLAINTEXTEDIT_H
#define OUTPUTFIGUREPLAINTEXTEDIT_H

#include "custom_plaintextedit.h"
namespace post3dapp{
struct BASE_TYPE_PROPERTY;
struct STRESS;

class OutputFigurePlainTextEdit : public CustomPlainTextEdit
{
    Q_OBJECT
public:
    OutputFigurePlainTextEdit(QWidget *parent = nullptr);
    bool hasFormatError() const override;
    QString getStatusMessage() const override;
    QList<BASE_TYPE_PROPERTY*> getTypeProperties(QString *errorMessage) const;

protected:
    bool eventFilter(QObject *obj,QEvent *event) override;

private:
    void decorateCells() const override;
};
} // namespace post3dapp


#endif // OUTPUTFIGUREPLAINTEXTEDIT_H

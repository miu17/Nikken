#ifndef RESORCEPLAINTEXTEDIT_H
#define RESORCEPLAINTEXTEDIT_H

#include <QPlainTextEdit>
namespace post3dapp{

class ResorcePlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    ResorcePlainTextEdit(QWidget *parent = nullptr);
};

} // namespace post3dapp
#endif // RESORCEPLAINTEXTEDIT_H

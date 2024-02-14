#ifndef INPUTDATACHECK_H
#define INPUTDATACHECK_H

#include <QObject>
class QProgressDialog;
namespace post3dapp{

class InputDataCheck : public QObject
{
    Q_OBJECT
public:
    InputDataCheck(QObject *parent = nullptr);
    QString executeCheck(QProgressDialog* dialog = nullptr);

private:
    QString checkWall();
};

} // namespace post3dapp

#endif // INPUTDATACHECK_H

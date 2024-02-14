#ifndef BUILDINGINFORMATIONWIDGET_H
#define BUILDINGINFORMATIONWIDGET_H

#include <QLineEdit>
#include <QWidget>

namespace post3dapp {
class AppVersion;

class BuildingInformationWidget : public QWidget
{
    Q_OBJECT
public:
    BuildingInformationWidget(QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

private:
    QLineEdit* name01;
    QLineEdit* place02;
    QLineEdit* use03;
    QLineEdit* category04;
    QLineEdit* future05;
    QLineEdit* totalarea06;
    QLineEdit* buildingarea07;
    QLineEdit* undergroundfloor08;
    QLineEdit* floor09;
    QLineEdit* roof10;
    QLineEdit* height11;
    QLineEdit* eavesheight12;
    QLineEdit* structureX13;
    QLineEdit* frameX14;
    QLineEdit* frameY15;
    QLineEdit* spanX16;
    QLineEdit* spanY17;
    QLineEdit* foundation18;
    QLineEdit* finishslab19;
    QLineEdit* finishroof20;
    QLineEdit* finishwall21;
    QLineEdit* accessories22;
};
} // namespace post3dapp

#endif // BUILDINGINFORMATIONWIDGET_H

#ifndef CALCULATIONTABWIDGET_H
#define CALCULATIONTABWIDGET_H

#include <QTabWidget>


namespace post3dapp {
class AppVersion;
class BuildingInformationWidget;
class CalculationPaperSettingWidget;
class PoskPlainTextEdit;
class ResorcePlainTextEdit;

class CalculationTabWidget :public QTabWidget
{
    Q_OBJECT
public:
    CalculationTabWidget(QWidget *parent = 0);    

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

    PoskPlainTextEdit* posk;
    BuildingInformationWidget* binfo;
    CalculationPaperSettingWidget* paperWidget;
    ResorcePlainTextEdit* resorce;

};
} // namespace post3dapp

#endif // CALCULATIONTABWIDGET_H

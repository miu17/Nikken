#ifndef OUTPUTFIGURE_TABWIDGET_H
#define OUTPUTFIGURE_TABWIDGET_H

#include <QTabWidget>
namespace post3dapp{
struct BASE_TYPE_PROPERTY;
class AppVersion;
class OutputFigurePlainTextEdit;
class OutputFigureViewSettingWidget;
class OutputPaperSetting;

class OutputFigureTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    OutputFigureTabWidget(QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

    QPair<OutputPaperSetting,QList<BASE_TYPE_PROPERTY*>> GetSetting(QString *errorMessage) const;
    bool hasFormatError() const;

private:
    OutputFigurePlainTextEdit *textEdit;
    OutputFigureViewSettingWidget *settingWidget;
};


} // namespace post3dapp

#endif // OUTPUTFIGURE_TABWIDGET_H

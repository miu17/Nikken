#include "outputfigure_tabwidget.h"

#include "app_version.h"
#include "define_draw_figure.h"
#include "outputfigure_plaintextedit.h"
#include "outputfigure_viewsettingwidget.h"
namespace post3dapp{

OutputFigureTabWidget::OutputFigureTabWidget(QWidget *parent)
    :QTabWidget(parent)
{
    this->setFixedSize(900, 900);
    textEdit = new OutputFigurePlainTextEdit();
    settingWidget = new OutputFigureViewSettingWidget();
    this->addTab(textEdit, "text");
    this->addTab(settingWidget, "view setting");
}

void OutputFigureTabWidget::writeBinaryFile(QDataStream &outfile) const{
    outfile << textEdit->toPlainText();
    settingWidget->writeBinaryFile(outfile);
}
void OutputFigureTabWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    QString temp;
    infile >> temp;
    textEdit->setPlainText(temp);
    settingWidget->readBinaryFile(infile, version);
}

bool OutputFigureTabWidget::hasFormatError() const{
    return this->textEdit->hasFormatError();
}

QPair<OutputPaperSetting,QList<BASE_TYPE_PROPERTY*>> OutputFigureTabWidget::GetSetting(QString *errorMessage) const{
    QList<BASE_TYPE_PROPERTY*> properties = this->textEdit->getTypeProperties(errorMessage);
    OutputPaperSetting setting = this->settingWidget->GetPaperSetting();
    return QPair<OutputPaperSetting,QList<BASE_TYPE_PROPERTY*>>{setting, properties};
}


} // namespace post3dapp

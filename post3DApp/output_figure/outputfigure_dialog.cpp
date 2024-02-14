#include "outputfigure_dialog.h"

#include <QBoxLayout>
#include <QPrintDialog>
#include <QPrinter>

#include "app_version.h"
#include "define_draw_figure.h"
#include "draw_figure.h"
#include "outputfigure_plaintextedit.h"
#include "outputfigure_tabwidget.h"
#include "unified_data.h"
#include "unified_inputdata.h"
#include "utility.h"

namespace post3dapp{

OutputFigureDialog::OutputFigureDialog(QWidget *parent)
    :QDialog(parent)
{
    this->setWindowTitle(u8"任意図出力");
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    this->setGeometry(0, 0, 500, 500);
    QVBoxLayout *lay = new QVBoxLayout();
    tabWidget = new OutputFigureTabWidget();
    lay->addWidget(tabWidget);
    QHBoxLayout* runButtonsLayout = new QHBoxLayout();
    runButtonsLayout->setSpacing(5);
    run_button = new QPushButton(u8"実行");
    runButtonsLayout->addWidget(run_button);
    lay->addLayout(runButtonsLayout);
    this->setLayout(lay);

    connect(run_button, &QAbstractButton::clicked,
            this, &OutputFigureDialog::setPrinter);
}


void OutputFigureDialog::setPrinter(){
    if (tabWidget->hasFormatError()){
        Utility::showErrorMessage(this, u8"任意出図テキストにエラーがあります。");
        return;
    }
    QString errorMessage;
    auto setting = tabWidget->GetSetting(&errorMessage);
    if (errorMessage != QString{}){
        Utility::showErrorMessage(this, errorMessage);
        return;
    }
    QPrintDialog printDialog;
    if (printDialog.exec() == QDialog::Accepted){
        QPainter* painter = new QPainter();
        QPrinter* printer = new QPrinter(QPrinter::ScreenResolution);
        printer->setOutputFileName(printDialog.objectName());
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setFullPage(true);
        OutputPaperSetting paperSetting = setting.first;
        for(int i = 0; i < setting.second.count(); i++){
            BASE_TYPE_PROPERTY* type = setting.second.at(i);
            if (typeid (type) == typeid(FIGURE_TYPE_PROPERTY*)){
                FIGURE_TYPE_PROPERTY* figureType = static_cast<FIGURE_TYPE_PROPERTY*>(type);
                CustomDrawGrScene* scene = figureType->makeScene(paperSetting.input2DViewSetting,
                                                          paperSetting.input3DViewSetting,
                                                          paperSetting.calcLoad2DViewSetting,
                                                          paperSetting.calcLoad3DViewSetting,
                                                          paperSetting.analysis2DViewSetting,
                                                          paperSetting.analysis3DViewSetting);
                auto result = DrawFigure::calcScaleAndDivision(scene, paperSetting.figure_size_policy);
                result.first.unitPaper.setPrinter(printer);
                if (i == 0){
                    painter->begin(printer);
                }else{
                    printer->newPage();
                }
                DrawFigure::drawForOutputFigure(painter, scene, result.second, result.first);
            }
        }
        painter->end();
    }
}


void OutputFigureDialog::writeBinaryFile(QDataStream &outfile) const{
    tabWidget->writeBinaryFile(outfile);
}

void OutputFigureDialog::readBinaryFile(QDataStream &infile, const AppVersion &version){
    tabWidget->readBinaryFile(infile, version);
}

} // namespace post3dapp

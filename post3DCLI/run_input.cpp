#include "run_input.h"

#include <QPainter>
#include <QPrinter>
#include "draw_figure.h"
#include "customdraw_grscene.h"
#include "outputfigure_reader.h"
#include "read_from_psv.h"
#include "unified_inputdata.h"
RunInput::RunInput(const ParsedArguments &args, QObject *parent)
    :QObject(parent), arguments(args)
{

}

bool RunInput::run(QString& out)
{
    // UnifiedInputDataの取り込み
    post3dapp::ReadFromPsv psv;
    QFile psvFile(arguments.psv);
    if (!psvFile.open(QIODevice::ReadOnly)){
        out += u8"ファイルが開けません";
        return false;
    }

    psv.ReadPsv(&psvFile);
    psvFile.close();

    // カラー設定
    post3dapp::Input2DViewSettings input2D = post3dapp::Input2DViewSettings::getDefault();
    post3dapp::Input3DViewSettings input3D = post3dapp::Input3DViewSettings::getDefault();
    post3dapp::CalculationLoad2DViewSettings load2D = post3dapp::CalculationLoad2DViewSettings::getDefault();
    post3dapp::CalculationLoad3DViewSettings load3D = post3dapp::CalculationLoad3DViewSettings::getDefault();
    post3dapp::Analysis2DViewSettings analysis2D = post3dapp::Analysis2DViewSettings::getDefault();
    post3dapp::Analysis3DViewSettings analysis3D = post3dapp::Analysis3DViewSettings::getDefault();
    if (arguments.ini != QString{}){
        post3dapp::UnifiedSettings::getInstance()->read(arguments.ini);
        input2D = post3dapp::UnifiedSettings::getInstance()->getInput2DViewSettings();
        input3D = post3dapp::UnifiedSettings::getInstance()->getInput3DViewSettings();
        load2D = post3dapp::UnifiedSettings::getInstance()->getCalc2DViewSettings();
        load3D = post3dapp::UnifiedSettings::getInstance()->getCalc3DViewSettings();
        analysis2D = post3dapp::UnifiedSettings::getInstance()->getOutput2DViewSettings();
        analysis3D = post3dapp::UnifiedSettings::getInstance()->getOutput3DViewSettings();
    }
    input2D.SetUseScaledFont(false);
    input3D.SetUseScaledFont(false);
    load2D.SetUseScaledFont(false);
    load3D.SetUseScaledFont(false);
    analysis2D.SetUseScaledFont(false);
    analysis3D.SetScaledFont(false);


    // 出力指定
    QStringList lines;

    if (arguments.setting != QString{}){
        QFile file(arguments.setting);
        if (!file.open(QIODevice::ReadOnly)){
            out += u8"Inputの出力指定ファイルが開けません。";
            return false;
        }
        QTextStream in(&file);
        lines = in.readAll().split('\n');
        file.close();

    }else{
        QStringList floorList;
        QStringList frameList;

        auto floors = post3dapp::UnifiedInputData::getInInstance()->getFloorList();
        for(auto f : floors){
             floorList.append(f.floorName);
        }
        if (floorList.count() != 0){
            lines.append("*OUTGROUPP");
            QString line;
            line.append("ALLFLOORS ");
            for(int i = 0; i < floorList.count(); i++){
                line.append(floorList.at(i).leftJustified(10, ' '));
                if ((i + 1) % 10 == 0 && i != floorList.count() - 1)
                    lines.append(line);
            }
            lines.append(line);
        }

        auto frames = post3dapp::UnifiedInputData::getInInstance()->getFramePointList();
        for (auto f : frames){
            frameList.append(f.frameName);
        }
        if (frameList.count() != 0){
            lines.append(u8"*OUTGROUPS");
            QString line;
            line.append(u8"ALLFRAMES ");
            for (int i = 0; i < frameList.count(); i++){
                line.append(frameList.at(i).leftJustified(10, ' '));
                if ((i + 1) % 10 == 0 && i != frameList.count() - 1)
                    lines.append(line);
            }
            lines.append(line);
        }

        if (floorList.count() != 0){
            lines.append(u8"*INPUTPLAN");
            lines.append(u8"ALLFLOORS");
        }
        if (frameList.count() != 0){
            lines.append(u8"*INPUTSECTION");
            lines.append(u8"ALLFRAMES");
        }
        lines.append("*FINALEND");
    }
    bool hasError = post3dapp::OutputFigureReader::hasFormatError(lines.join('\n'));
    if (hasError){
        out += u8"Inputの出力指定ファイルのフォーマットが異なります。";
        return false;
    }
    post3dapp::OutputFigureReader reader;
    QString errorMessage;
    auto properties = reader.cliParse(lines, &errorMessage, true, false, false, false);
    if (errorMessage != QString{}){
        out += errorMessage;
        return false;
    }
    post3dapp::FIGURE_SIZE_POLICY policy;
    policy.policy = arguments.policy;
    policy.initial_paper = post3dapp::PAPER(arguments.size, arguments.orientation);
    policy.initial_scale = arguments.scale;
    policy.drawHeader = true;
    policy.drawFooter = true;
    QPainter* painter = new QPainter();
    QPrinter* printer = new QPrinter(QPrinter::HighResolution);
    printer->setOutputFileName(arguments.output);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setFullPage(true);
    for (int i = 0; i < properties.count(); i++){
        post3dapp::BASE_TYPE_PROPERTY* type = properties.at(i);
        post3dapp::FIGURE_TYPE_PROPERTY* ftype = static_cast<post3dapp::FIGURE_TYPE_PROPERTY*>(type);
        ftype->filename = arguments.output;
        post3dapp::CustomDrawGrScene* scene = ftype->makeScene(input2D,
                                                             input3D,
                                                             load2D,
                                                             load3D,
                                                             analysis2D,
                                                             analysis3D,
                                                             this
                                                             );

        auto result = post3dapp::DrawFigure::calcScaleAndDivision(scene, policy);
        result.first.unitPaper.setPrinter(printer);
        if (i == 0){
            painter->begin(printer);
        }else{
            printer->newPage();
        }
        post3dapp::DrawFigure::drawForOutputFigure(painter, scene, result.second, result.first);
    }
    painter->end();
    return true;
}

#include "run_figure.h"

#include <QDir>
#include <QPainter>
#include <QPrinter>
#include "calc_load.h"
#include "customdraw_grscene.h"
#include "draw_figure.h"
#include "fixed_data.h"
#include "graph_factory.h"
#include "graph_scene.h"
#include "handle_npstn.h"
#include "line_outputfigure.h"
#include "outputfigure_reader.h"
#include "read_from_psv.h"
#include "unified_calcload.h"
#include "unified_inputdata.h"

RunFigure::RunFigure(const ParsedArguments &args, QObject *parent)
    :QObject(parent), arguments(args)
{

}

QStringList RunFigure::makeDefaultOutput() const
{
    QStringList lines;
    QStringList floorList;
    QStringList frameList;

    auto floors = post3dapp::UnifiedInputData::getInInstance()->getFloorList();
    for(auto f : floors){
         floorList.append(f.floorName);
    }
    if (floorList.count() != 0){
        lines.append(post3dapp::LineOutputFigure::outputFigureStatusToCommand(post3dapp::OutputFigureStatus::OutgroupP));
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
        lines.append(post3dapp::LineOutputFigure::outputFigureStatusToCommand(post3dapp::OutputFigureStatus::OutgroupS));
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
        lines.append(post3dapp::LineOutputFigure::outputFigureStatusToCommand(post3dapp::OutputFigureStatus::PlanInput));
        lines.append(u8"ALLFLOORS");
    }
    if (frameList.count() != 0){
        lines.append(post3dapp::LineOutputFigure::outputFigureStatusToCommand(post3dapp::OutputFigureStatus::SectionInput));
        lines.append(u8"ALLFRAMES");
    }
    lines.append("*FINALEND");
    return lines;
}

bool RunFigure::check(QString& out)
{
    if (!arguments.analysis.endsWith(".npstn")  && arguments.analysis == "nanalysis.dat"){
        out +=  u8"ファイル名が*.npstnまたはnanalysis.datではありません";
        return false;
    }else if (arguments.analysis.endsWith(".npstn")){
        auto absolutePath = QFileInfo(arguments.psv).absolutePath() + QDir::separator() + arguments.analysis;
        if (QFileInfo::exists(arguments.analysis)){
            arguments.analysis = QFileInfo(arguments.analysis).absoluteFilePath();
        }else if (QFileInfo::exists(absolutePath)){
            arguments.analysis = absolutePath;
        }else{
            out += u8".npstnファイルが存在しません。";
            return false;
        }
    }else if (arguments.analysis == "nanalysis.dat"){
        if (QFileInfo::exists(arguments.analysis)){
            arguments.npstn = QFileInfo(arguments.analysis).absoluteFilePath();
        }else{
            out += u8"nanalysis.datが存在しません。";
            return false;
        }
    }

    return true;
}

bool RunFigure::run(QString& out)
{
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
    bool doneReadInput = false;
    if (arguments.setting == QString{}){
        // psvを読み込む
        post3dapp::ReadFromPsv psv;
        QFile psvFile(arguments.psv);
        if (!psvFile.open(QIODevice::ReadOnly)){
            out += u8"ファイルが開けません";
            return false;
        }
        psv.ReadPsv(&psvFile);
        psvFile.close();
        doneReadInput = true;
        lines = makeDefaultOutput();
    }else{
        QFile file(arguments.setting);
        if (!file.open(QIODevice::ReadOnly)){
            QCoreApplication::exit(-1);
            out += u8"出力指定ファイルが開けません。";
            return false;
        }
        QTextStream in(&file);
        lines = in.readAll().split('\n');
        file.close();
    }


    bool hasInput = false;
    bool hasInputCopy = false;
    bool hasCalc = false;
    bool hasOutput = false;
    post3dapp::OutputFigureReader reader;
    QString errorMessage;
    auto dummyProperties = reader.fastParse(lines, &errorMessage);
    if (errorMessage != QString{}){
        out += errorMessage;
        return false;
    }
    for(auto p : dummyProperties){
        switch(p->viewtype){
        case post3dapp::MODELVIEWTYPE::INPUT2DMODEL:
        case post3dapp::MODELVIEWTYPE::INPUT3DMODEL:
            hasInput = true;
            break;
        case post3dapp::MODELVIEWTYPE::INPUT2DMODELCOPY:
        case post3dapp::MODELVIEWTYPE::INPUT3DMODELCOPY:
            hasInputCopy = true;
            break;
        case post3dapp::MODELVIEWTYPE::CALC2DMODEL:
        case post3dapp::MODELVIEWTYPE::CALC3DMODEL:
            hasCalc = true;
            break;
        case post3dapp::MODELVIEWTYPE::OUTPUT2DMODEL:
        case post3dapp::MODELVIEWTYPE::OUTPUT3DMODEL:
        case post3dapp::MODELVIEWTYPE::OUTPUTGRAPH:
            hasOutput = true;
            break;
        }
    }

    if (hasOutput){
        if (!check(out)){
            return false;
        }
    }

    bool doAnalysis = hasOutput && arguments.analysis == "nanalysis.dat";

    if (!doneReadInput){
        if (hasInput ||
            hasInputCopy ||
            hasCalc ||
            doAnalysis){
            // psvを読み込む
            post3dapp::ReadFromPsv psv;
            QFile psvFile(arguments.psv);
            if (!psvFile.open(QIODevice::ReadOnly)){
                out += u8"ファイルが開けません";
                return false;
            }

            psv.ReadPsv(&psvFile);
            psvFile.close();
        }
    }

    if (hasInputCopy || hasCalc || doAnalysis){
        // 荷重拾いを行う
        post3dapp::UnifiedFixedData::setInstance(post3dapp::FixedData::Create());
        QString o_fpath;
        bool writeFile = false;
        if (doAnalysis){
            writeFile = true;
            QFileInfo psv(arguments.psv);
            QDir psvdir(psv.absolutePath());
            QString outputDirName = psv.baseName() + "_outputs";
            if (psvdir.exists(outputDirName)){
                psvdir.rmdir(outputDirName);
            }

            if (!psvdir.mkdir(outputDirName)){
                out += u8"荷重拾いフォルダが作成できませんでした。";
                return false;
            }
            o_fpath = psv.absolutePath() + QDir::separator() + outputDirName + QDir::separator() + "output_loads.csv";
        }
        QString err = post3dapp::UnifiedFixedData::getInstance()->getCalcLoadData()->executeCalculation(o_fpath, nullptr, writeFile);
    }

    if (doAnalysis){
        // 再計算を行う
        // npstn名を書き込む
        throw;
    } else if (hasOutput){
        QFile analysis(arguments.analysis);
        if (!analysis.exists()){
            out += u8"npstnファイルが指定されていません。";
            return false;
        }

        // npstnを読み込む
        int errorCode = 0;
        post3dapp::handleNPstn* npstn = post3dapp::handleNPstn::CreateWithoutDialog(arguments.analysis, &errorCode);
        if (errorCode != 0){
            out += u8"npstnが読み込めませんでした。";
            return false;
        }
    }

    auto properties = reader.cliParse(lines, &errorMessage, hasInput, hasInputCopy, hasCalc, hasOutput);
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
        if (type->viewtype == post3dapp::MODELVIEWTYPE::OUTPUTGRAPH){
            auto gtype = static_cast<post3dapp::GRAPH_TYPE_PROPERTY*>(type);
            post3dapp::GRAPH_PROPERTY graphProperty(policy);
            QList<post3dapp::GRAPH_SUBTYPE_PROPERTY> typeOption = gtype->makeSubType();
            post3dapp::GRAPH_SIZE_OPTION sizeOption;
            //post3dapp::Graph graph = post3dapp::GraphFactory::makeOneGraph(analysis2D, typeOption, sizeOption);
            graphProperty.unitPaper.setPrinter(printer);
            if (i == 0){
                painter->begin(printer);
            }else{
                printer->newPage();
            }
            post3dapp::GraphScene* graphScene = new post3dapp::GraphScene(printer->logicalDpiX(), false);
            // TODO nmint myImage
            graphScene->setValues(typeOption, graphProperty, typeOption.count(), analysis2D);
            graphScene->drawForeground(painter, QRect{});

        }else{
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
    }
    painter->end();
    return true;
}

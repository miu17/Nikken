#include "run_text.h"

#include <QDir>
#include "abstracttext.h"
#include "fixed_data.h"
#include "handle_npstn.h"
#include "line_outputtext.h"
#include "outputtext_reader.h"
#include "outputtextparser.h"
#include "read_from_psv.h"
#include "unified_inputdata.h"

RunText::RunText(const ParsedArguments &args, QObject *parent)
    :QObject(parent), arguments(args)
{

}

bool RunText::run(QString &out)
{
    // 出力指定
    QFile file(arguments.setting);
    if (!file.open(QIODevice::ReadOnly)){
        out += u8"Textの出力指定ファイルが開けません。";
        return false;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    file.close();

    post3dapp::OutputTextReader reader;
    QString errorMessage;
    auto dummyTexts = reader.fastParse(text.split('\n'), &errorMessage);
    if (errorMessage != QString{}){
        out += errorMessage;
        return false;
    }

    bool hasInput = false;
    bool hasOutput = false;
    for (auto p : dummyTexts){
        if (post3dapp::LineOutputText::isInputStatus(p->getStatus())){
            hasInput = true;
        }else{
            hasOutput = true;
        }
    }

    if (hasOutput){
        if (!check(out)){
            return false;
        }
    }

    bool doAnalysis = hasOutput && arguments.analysis == "nanalysis.dat";
    if (hasInput || doAnalysis){
        // UnifiedInputDataの取り込み
        post3dapp::ReadFromPsv psv;
        QFile psvFile(arguments.psv);
        if (!psvFile.open(QIODevice::ReadOnly)){
            out += u8"ファイルが開けません";
            return false;
        }

        psv.ReadPsv(&psvFile);
        psvFile.close();
    }

    if (doAnalysis){
        // 再計算
        throw;

    }else if (hasOutput){
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

    if (hasOutput){
        // o_sub.h
    }

    QFile output(arguments.output);
    if (!output.open(QIODevice::WriteOnly)){
        out += u8"Textのlstファイルが開けません。";
        return false;
    }

    QTextStream lst(&output);
    auto parsed = reader.cliParse(text.split('\n'), &errorMessage, hasInput, hasOutput);
    if (errorMessage != QString{}){
        out += errorMessage;
        return false;
    }

    for (auto p : parsed){
        lst << p->write();
        //lst << post3dapp::AbstractText::CreateAndWrite(*post3dapp::UnifiedFixedData::getInstance(), p).second;
    }
    output.close();
    return true;
}

bool RunText::check(QString &out)
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

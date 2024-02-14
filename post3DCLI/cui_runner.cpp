#include "cui_runner.h"

#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTime>
#include "run_figure.h"
#include "run_input.h"
#include "run_statement.h"
#include "run_text.h"
void CUIRunner::run()
{
    auto startTime = QTime::currentTime();
    QCommandLineParser parser;
    QString error;;
    CommandLineParseResult ret;
    ParsedArguments arguments;
    result(parser, ret, arguments);


    bool isNormal = false;
    switch(ret.statusCode){
    case CommandLineParseResult::Status::Error:
        error += ret.errorString;
        break;
    case CommandLineParseResult::Status::VersionRequrested:
        parser.showVersion();
        isNormal = true;
        break;
    case CommandLineParseResult::Status::HelpRequested:
        parser.showHelp();
        isNormal = true;
        break;
    case CommandLineParseResult::Status::Ok:
        switch(arguments.mode){
        case ParsedArguments::Mode::Text:
        {
            RunText text(arguments, this);
            isNormal = text.run(error);
        }break;
        case ParsedArguments::Mode::Figure:
        {
            RunFigure figure(arguments, this);
            isNormal = figure.run(error);
        }break;
        case ParsedArguments::Mode::Statement:
        {

        }break;
        }

        break;
    }

    error += u8"Time:";
    error += QString::number(startTime.secsTo(QTime::currentTime()));
    error += u8"second\n";
    fprintf(stdout, qPrintable(error));
    if (isNormal)
        QCoreApplication::exit(0);
    else
        QCoreApplication::exit(-1);

}

void CUIRunner::result(QCommandLineParser& parser, CommandLineParseResult& ret, ParsedArguments &arguments)
{
    // 引数にロングワードを使う
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

    parser.addPositionalArgument(".psv", u8"psvファイル");

    // ヘルプを出力したい場合 -h でヘルプ表示
    auto helpOption = parser.addHelpOption();

    // バージョンを出力
    auto versionOption = parser.addVersionOption();

    // -m [mode]
    QCommandLineOption modeOption("m", u8"必須。モードを指定する。Figure/Text/Statement", u8"mode", u8"Figure");
    parser.addOption(modeOption);

    // --policy
    QCommandLineOption policyOption("policy", u8"オプション。紙サイズポリシーを指定する。", u8"policy");
    parser.addOption(policyOption);

    // --size
    QCommandLineOption paperSizeOption("size", u8"オプション。紙のサイズを指定する。", u8"size");
    parser.addOption(paperSizeOption);

    // --orientation
    QCommandLineOption paperOrientationOption("orientation", u8"オプション。紙の向きを指定する。", u8"orientation");
    parser.addOption(paperOrientationOption);

    // --scale
    QCommandLineOption scaleOption("scale", u8"オプション。スケールを指定する。", u8"scale");
    parser.addOption(scaleOption);

    // --setting　[setting] 出力指定オプション
    QCommandLineOption settingOption("setting", u8"Input以外必須。出力設定ファイルを指定する。Inputで指定のない場合は全ての階・通りを出力する。",  u8"setting");
    parser.addOption(settingOption);

    // --ini
    QCommandLineOption colorOption("ini", u8"オプション。カラー設定ファイルを指定する。指定しない場合はデフォルト設定を利用する", u8"ini");
    parser.addOption(colorOption);

    // --output [output] オプション
    QCommandLineOption pdfOption("output", u8"オプション。出力ファイル名を指定する。指定のない場合は<ファイル名>.pdf/<ファイル名>.lst", u8"output");
    parser.addOption(pdfOption);

    // --analysis [analysis] オプション
    QCommandLineOption analysisOption("analysis", u8"オプション。解析条件ファイルまたはpstnファイルを指定する。解析条件ファイルの場合は再解析を行う。", "analysis");
    parser.addOption(analysisOption);


    if (QCoreApplication::arguments().count() <= 1){
        ret.statusCode = CommandLineParseResult::Status::HelpRequested;
        return;
    }

    // 引数のチェック
    if (!parser.parse(QCoreApplication::arguments())){
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = parser.errorText();
        return;
    }

    parser.process(QCoreApplication::arguments());

    if (parser.isSet(versionOption)){
        ret.statusCode = CommandLineParseResult::Status::VersionRequrested;
        return;
    }

    if (parser.isSet(helpOption)){
        ret.statusCode = CommandLineParseResult::Status::HelpRequested;
        return;
    }

    // ファイル名を後で使用したいので先頭に
    psvFileCheck(parser, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

    modeCheck(parser, modeOption, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

    policyCheck(parser, policyOption, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

    sizeCheck(parser, paperSizeOption, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

    orientationCheck(parser, paperOrientationOption, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

    scaleCheck(parser, scaleOption, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

    iniFileCheck(parser, colorOption, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

    outputFileCheck(parser, pdfOption, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

    settingCheck(parser, settingOption, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

    analysisCheck(parser, analysisOption, ret, arguments);
    if (ret.statusCode == CommandLineParseResult::Status::Error){
        return;
    }

}

void CUIRunner::policyCheck(const QCommandLineParser &parser,
                            const QCommandLineOption &option,
                            CommandLineParseResult &ret,
                            ParsedArguments &arguments) const
{
    if (!parser.isSet(option)){
        return;
    }

    auto value = parser.value(option);
    if (value == "AutoScale"){
        arguments.policy = post3dapp::DividePolicy::AutoScale;
    }else if (value == "PSD"){
        arguments.policy = post3dapp::DividePolicy::PAPERSIZE_SCALE_DIVIDE;
    }else if (value == "PDS"){
        arguments.policy = post3dapp::DividePolicy::PAPERSIZE_DIVIDE_SCALE;
    }else if (value == "SDP"){
        arguments.policy = post3dapp::DividePolicy::SCALE_DIVIDE_PAPERSIZE;
    }else if (value == "DSP"){
        arguments.policy = post3dapp::DividePolicy::DIVIDE_SCALE_PAPERSIZE;
    }else if (value == "A4A3Auto"){
        arguments.policy = post3dapp::DividePolicy::A4A3Auto;
    }else if (value == "A4A3Scale"){
        arguments.policy = post3dapp::DividePolicy::A4A3Scale;
    }else if (value == "Manual"){
        arguments.policy = post3dapp::DividePolicy::MANUAL;
    }else{
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8"policyの値が不正です";
    }
}

void CUIRunner::sizeCheck(const QCommandLineParser &parser, const QCommandLineOption &option, CommandLineParseResult &ret, ParsedArguments &arguments) const
{
    if (!parser.isSet(option)){
        return;
    }

    auto value = parser.value(option);
    if (value == "A4"){
        arguments.size = QPagedPaintDevice::A4;
    }else if (value == "A3"){
        arguments.size = QPagedPaintDevice::A3;
    }else{
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8"sizeの値が不正です";
    }
}

void CUIRunner::orientationCheck(const QCommandLineParser &parser, const QCommandLineOption &option, CommandLineParseResult &ret, ParsedArguments &arguments) const
{
    if (!parser.isSet(option)){
        return;
    }

    auto value = parser.value(option);
    if (value == "Portrait"){
        arguments.orientation = QPageLayout::Portrait;
    }else if (value == "Landscape"){
        arguments.orientation = QPageLayout::Landscape;
    }else{
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8"orientationの値が不正です";
    }
}

void CUIRunner::scaleCheck(const QCommandLineParser &parser, const QCommandLineOption &option, CommandLineParseResult &ret, ParsedArguments &arguments) const
{
    if (!parser.isSet(option)){
        return;
    }

    auto value = parser.value(option);
    bool isOk = false;
    double d = value.toDouble(&isOk);
    if (!isOk){
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8"scaleの値が不正です";
    }
    arguments.scale = d;
}

void CUIRunner::psvFileCheck(const QCommandLineParser& parser, CommandLineParseResult &ret, ParsedArguments &arguments) const
{
    // 相対パスを使用するためにも.psvは必須とする。読み込むかどうかはオプション次第。
    if (parser.positionalArguments().count() == 0 || !parser.positionalArguments().at(0).endsWith(u8".psv")){
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8".psvファイルが指定されていません。";
        return;
    }

    auto path = parser.positionalArguments().at(0);
    auto absolutePath = QCoreApplication::applicationDirPath() + QDir::separator() + path;
    if (QFileInfo::exists(path)){
        arguments.psv = QFileInfo(path).absoluteFilePath();
    }else if (QFileInfo::exists(absolutePath)){
        arguments.psv = absolutePath;
    }else{
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8".psvファイルが存在しません。";
        return;
    }
}

void CUIRunner::iniFileCheck(const QCommandLineParser &parser, const QCommandLineOption &option, CommandLineParseResult &ret, ParsedArguments &arguments) const
{
    if (!parser.isSet(option)){
        return;
    }else if (arguments.mode == ParsedArguments::Mode::Text){
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8"不要なiniオプションが設定されています。";
    }

    auto path = parser.value(option);
    auto absolutePath = QFileInfo(arguments.psv).absolutePath() + QDir::separator() + path;
    if (QFileInfo::exists(path)){
        arguments.ini = QFileInfo(path).absoluteFilePath();
    }else if (QFileInfo::exists(absolutePath)){
        arguments.ini = absolutePath;
    }else{
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8".iniファイルが存在しません。";
    }
}

void CUIRunner::outputFileCheck(const QCommandLineParser &parser, const QCommandLineOption &option, CommandLineParseResult &ret, ParsedArguments &arguments) const
{
    if (!parser.isSet(option)){
        QFileInfo psv(arguments.psv);
        QString ext;
        if (arguments.mode == ParsedArguments::Mode::Text){
            ext = ".lst";
        }else{
            ext = ".pdf";
        }
        arguments.output = QFileInfo(arguments.psv).absolutePath() + QDir::separator() + psv.baseName() + ext;
        return;
    }

    auto path = parser.value(option);
    if (arguments.mode == ParsedArguments::Mode::Text){
        if (!path.endsWith(".lst")){
            ret.statusCode = CommandLineParseResult::Status::Error;
            ret.errorString = u8"ファイル名が.lstではありません。";
        }else{
            arguments.output = QFileInfo(path).absoluteFilePath();
        }
    }
    else{
        if (!path.endsWith(".pdf")){
            ret.statusCode = CommandLineParseResult::Status::Error;
            ret.errorString = u8"ファイル名が.pdfではありません。";
        }else{
            arguments.output = QFileInfo(path).absoluteFilePath();
        }
    }
}

void CUIRunner::modeCheck(const QCommandLineParser &parser,
                            const QCommandLineOption &modeOption,
                            CommandLineParseResult &ret,
                            ParsedArguments &arguments) const
{
    if (!parser.isSet(modeOption)){
        /*
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8"mオプションがありません。";
        return;
        */
        arguments.mode = ParsedArguments::Mode::Figure;
    }else{
        auto value = parser.value(modeOption);
        if (value == "figure" || value == "Figure"){
            arguments.mode = ParsedArguments::Mode::Figure;
        }
        else if (value == "text" || value == "Text"){
            arguments.mode = ParsedArguments::Mode::Text;
        }
        else if (value == "statement" || value == "Statement"){
            arguments.mode = ParsedArguments::Mode::Statement;
        }
        else{
            ret.statusCode = CommandLineParseResult::Status::Error;
            ret.errorString = u8"mオプションが正しくありません。";
        }
    }
}

void CUIRunner::settingCheck(const QCommandLineParser &parser,
                             const QCommandLineOption &option,
                             CommandLineParseResult &ret,
                             ParsedArguments &arguments) const
{
    if (!parser.isSet(option)){
        if (arguments.mode == ParsedArguments::Mode::Figure)
            return;
        else{
            ret.statusCode = CommandLineParseResult::Status::Error;
            ret.errorString = u8"出力指定ファイルが指定されていません。";
        }
    }

    auto path = parser.value(option);
    auto absolutePath = QFileInfo(arguments.psv).absolutePath() + QDir::separator() + path;
    if (QFileInfo::exists(path)){
        arguments.setting = QFileInfo(path).absoluteFilePath();
    }else if(QFileInfo::exists(absolutePath)){
        arguments.setting = absolutePath;
    }else{
        ret.statusCode = CommandLineParseResult::Status::Error;
        ret.errorString = u8"出力指定ファイルが存在しません。";
    }
}

void CUIRunner::analysisCheck(const QCommandLineParser &parser,
                              const QCommandLineOption &option,
                              CommandLineParseResult &ret,
                              ParsedArguments &arguments) const
{
    // Inputしか指定がない場合もあるので詳細は内部でチェックし、指定ファイルの有無のみチェックする。
    if (!parser.isSet(option)){
        return;
    }else{
        auto path = parser.value(option);
        auto absolutePath = QFileInfo(arguments.psv).absolutePath() + QDir::separator() + path;
        if (QFileInfo::exists(path)){
            arguments.analysis = QFileInfo(path).absoluteFilePath();
        }else if (QFileInfo::exists(absolutePath)){
            arguments.analysis = absolutePath;
        }else{
            ret.statusCode = CommandLineParseResult::Status::Error;
            ret.errorString = u8"指定されたanalysisオプションが存在しません。";
        }
    }
}



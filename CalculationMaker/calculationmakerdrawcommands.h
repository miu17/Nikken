#ifndef CALCULATIONMAKERCOMMANDS_H
#define CALCULATIONMAKERCOMMANDS_H

#include "calculationmakercommandinterface.h"
#include "define_draw_figure.h"
#include <QMap>

namespace CalculationMakerLib{

class CalculationMakerContext;
class CalculationMaker;

#define BASIC_DRAW_COMMAND(name, command) \
class name : public CalculationMakerDrawCommand \
{ \
public: \
    name() : CalculationMakerDrawCommand(command) {} \
    bool drawContent(CalculationMaker *maker, const QString &arg); \
};

class CalculationMakerDrawCommand : public CalculationMakerCommandInterface
{
public:
    CalculationMakerDrawCommand(const QString &command);
    bool checkPrinting(CalculationMakerContext *context);
    virtual bool preCheck(CalculationMakerLib::CalculationMaker *maker);
    virtual bool isEnabled(CalculationMakerLib::CalculationMaker *maker);
    virtual bool drawContent(CalculationMaker *maker, const QString &arg) = 0;
    bool draw(CalculationMaker *maker, const QString &arg);
};

class CalculationMakerCommonCommand : public CalculationMakerDrawCommand
{
public:
    CalculationMakerCommonCommand();
    bool preCheck(CalculationMakerLib::CalculationMaker *maker);
    bool isEnabled(CalculationMaker *) { return true; }
    bool drawContent(CalculationMaker *maker, const QString &arg);
private:
    QMap<QString, QString> m_commonValues;
};

class CalculationMakerBeginCommand : public CalculationMakerDrawCommand
{
public:
    CalculationMakerBeginCommand();
    bool preCheck(CalculationMakerLib::CalculationMaker *maker);
    bool isEnabled(CalculationMaker *) { return true; }
    bool drawContent(CalculationMaker *maker, const QString &arg);
};

class CalculationMakerEndCommand : public CalculationMakerDrawCommand
{
public:
    CalculationMakerEndCommand();
    bool isEnabled(CalculationMaker *) { return true; }
    bool drawContent(CalculationMaker *maker, const QString &arg);
};

class CalculationMakerStyledTextCommand : public CalculationMakerDrawCommand
{
public:
    CalculationMakerStyledTextCommand(const QString &command);
    bool drawContent(CalculationMaker *maker, const QString &arg);
private:
    QString m_style;
};

BASIC_DRAW_COMMAND(CalculationMakerTextCommand, "\\text")
BASIC_DRAW_COMMAND(CalculationMakerItemCommand, "\\item")
BASIC_DRAW_COMMAND(CalculationMakerNewPageCommand, "\\newpage")

BASIC_DRAW_COMMAND(CalculationMakerFontCommand, "\\font")
BASIC_DRAW_COMMAND(CalculationMakerAlignmentCommand, "\\alignment")
BASIC_DRAW_COMMAND(CalculationMakerIndentLCommand, "\\indentL")
BASIC_DRAW_COMMAND(CalculationMakerIndentRCommand, "\\indentR")
BASIC_DRAW_COMMAND(CalculationMakerLineSpacingCommand, "\\linespacing")
BASIC_DRAW_COMMAND(CalculationMakerHeaderCommand, "\\header")
BASIC_DRAW_COMMAND(CalculationMakerFooterCommand, "\\footer")

BASIC_DRAW_COMMAND(CalculationMakerSetPenCommand, "\\setpen")

BASIC_DRAW_COMMAND(CalculationMakerRectCommand, "\\rect")
BASIC_DRAW_COMMAND(CalculationMakerLineCommand, "\\line")

BASIC_DRAW_COMMAND(CalculationMakerUnderLineCommand, "\\underline")
BASIC_DRAW_COMMAND(CalculationMakerFboxCommand, "\\fbox")

BASIC_DRAW_COMMAND(CalculationMakerSuperScriptCommand, "\\superscript")
BASIC_DRAW_COMMAND(CalculationMakerSubScriptCommand, "\\subscript")

BASIC_DRAW_COMMAND(CalculationMakerNewLineCommand, "\\\\")
BASIC_DRAW_COMMAND(CalculationMakerVSpaceCommand, "\\vspace")
BASIC_DRAW_COMMAND(CalculationMakerHSpaceCommand, "\\hspace")
BASIC_DRAW_COMMAND(CalculationMakerMoveXYCommand, "\\movexy")
BASIC_DRAW_COMMAND(CalculationMakerMoveRXYCommand, "\\moverxy")

BASIC_DRAW_COMMAND(CalculationMakerIncludeFigureCommand, "\\includefigure")
BASIC_DRAW_COMMAND(CalculationMakerIncludeTableCommand, "\\includetable")
BASIC_DRAW_COMMAND(CalculationMakerIncludeTextCommand, "\\includetext")
BASIC_DRAW_COMMAND(CalculationMakerIncludeLineTextCommand, "\\includelinetext")

BASIC_DRAW_COMMAND(CalculationMakerPictureCommand, "\\picture")

class CalculationMakerContentsCommand : public CalculationMakerDrawCommand
{
public:
    CalculationMakerContentsCommand();
    bool preCheck(CalculationMakerLib::CalculationMaker *maker, const post3dapp::OutputPaperSetting& setting);
    bool isEnabled(CalculationMaker *) { return true; }
    bool drawContent(CalculationMaker *maker, const QString &arg);
};
} // namespace CalculationMakerLib

#endif // CALCULATIONMAKERCOMMANDS_H

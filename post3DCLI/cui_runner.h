#ifndef CUIRUNNER_H
#define CUIRUNNER_H

#include <QCommandLineParser>
#include "define_draw_figure.h"

struct CommandLineParseResult{
    enum class Status{
        Ok,
        Error,
        VersionRequrested,
        HelpRequested
    };
    Status statusCode = Status::Ok;
    QString errorString{};
};

struct ParsedArguments{
    enum class Mode{
        Figure,
        Text,
        Statement
    };
    Mode mode = Mode::Figure;
    QString psv = QString{};
    QString ini = QString{};
    QString output = QString{};
    QString setting = QString{};
    QString analysis = QString{};
    QString npstn = QString{};
    post3dapp::DividePolicy policy = post3dapp::DividePolicy::A4A3Auto;
    QPagedPaintDevice::PageSize size = QPagedPaintDevice::PageSize::A4;
    QPageLayout::Orientation orientation = QPageLayout::Orientation::Portrait;
    double scale = 0.005;
};


class CUIRunner : public QObject
{
    Q_OBJECT
public:
    CUIRunner(QObject *parent = nullptr):QObject(parent){}
public slots:
    void run();

private:
    void result(QCommandLineParser&, CommandLineParseResult&, ParsedArguments&);
    void policyCheck(const QCommandLineParser&, const QCommandLineOption&, CommandLineParseResult&, ParsedArguments&) const;
    void sizeCheck(const QCommandLineParser&, const QCommandLineOption&, CommandLineParseResult&, ParsedArguments&) const;
    void orientationCheck(const QCommandLineParser&, const QCommandLineOption&, CommandLineParseResult&, ParsedArguments&) const;
    void scaleCheck(const QCommandLineParser&, const QCommandLineOption&, CommandLineParseResult&, ParsedArguments&) const;
    void psvFileCheck(const QCommandLineParser&, CommandLineParseResult&, ParsedArguments&) const;
    void iniFileCheck(const QCommandLineParser&, const QCommandLineOption&, CommandLineParseResult&, ParsedArguments&) const;
    void outputFileCheck(const QCommandLineParser&, const QCommandLineOption&, CommandLineParseResult&, ParsedArguments&) const;
    void modeCheck(const QCommandLineParser&,
                     const QCommandLineOption&,
                     CommandLineParseResult&,
                     ParsedArguments&) const;
    void settingCheck(const QCommandLineParser&,
                      const QCommandLineOption&,
                      CommandLineParseResult&,
                      ParsedArguments&) const;
    void analysisCheck(const QCommandLineParser&,
                       const QCommandLineOption&,
                       CommandLineParseResult&,
                       ParsedArguments&) const;

};

#endif // CUIRUNNER_H

#ifndef RUNFIGURE_H
#define RUNFIGURE_H

#include <QObject>
#include "cui_runner.h"
#include "define_draw_figure.h"

class RunFigure : public QObject
{
    Q_OBJECT
public:
    RunFigure(const ParsedArguments&, QObject *parent = nullptr);
    bool run(QString&);
private:
    ParsedArguments arguments;
    bool check(QString&);
    QStringList makeDefaultOutput() const;
};

#endif // RUNFIGURE_H

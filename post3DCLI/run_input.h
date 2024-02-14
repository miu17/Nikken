#ifndef RUNINPUT_H
#define RUNINPUT_H

#include <QObject>
#include "cui_runner.h"
#include "define_draw_figure.h"

class RunInput : public QObject
{
    Q_OBJECT
public:
    RunInput(const ParsedArguments&, QObject *parent = nullptr);
    bool run(QString&);
private:
    const ParsedArguments arguments;
};

#endif // RUNINPUT_H

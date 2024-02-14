#ifndef RUNTEXT_H
#define RUNTEXT_H

#include <QObject>
#include "cui_runner.h"


class RunText : public QObject
{
    Q_OBJECT
public:
    RunText(const ParsedArguments&, QObject *parent = nullptr);
    bool run(QString&);
private:
    ParsedArguments arguments;
    bool check(QString&);
};

#endif // RUNTEXT_H

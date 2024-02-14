#include <QApplication>
#include <QTextStream>
#include <QTimer>

#include "cui_runner.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("post3DCLI");
    QCoreApplication::setApplicationVersion("1.0.0");

    CUIRunner runner;
    QTimer::singleShot(0, &runner, &CUIRunner::run);
    return a.exec();
}


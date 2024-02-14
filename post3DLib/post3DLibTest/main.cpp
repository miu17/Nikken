#include <QApplication>
#include <QDebug>
#include <QTest>


int main(int argc, char * argv[])
{
    qSetMessagePattern("%{file}:%{line} => %{message}");
    QApplication app( argc, argv );

    int status = 0;
    //status |= QTest::qExec(new tst_calc_bandPassFilter, argc, argv);
    if (status != 0)
        qFatal("Fatal");
    return status;
}

#include "abstracttext.h"

#include <stdio.h>
#include <io.h>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>

#include "fixed_data.h"
namespace post3dapp{

QString AbstractTextCallFortran::readTempFile(const QString& command) const
{
    QFile temp(tempFile);
    QString second;
    if (!temp.open(QIODevice::ReadOnly))
        second = u8"読み込み失敗:" + command;

    QTextStream in(&temp);
    QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
    in.setCodec(codec);
    while(!in.atEnd()){
        second += in.readLine();
        second += "\n";
    }
    return second;
}

QPair<QPair<QString, QString>, QString> AbstractTextCallFortran::write(const FixedData& fixedData) const
{
    int stdout_dupfd;
    FILE *temp_out;
    int filenumber = _fileno(stdout);
    /* duplicate stdout */
    stdout_dupfd = _dup(filenumber);

    temp_out = fopen(tempFile, "w");

    /* replace stdout with our output fd */
    _dup2(_fileno(temp_out), filenumber);

    /* output something... */
    CallFortran(*fixedData.getPstnData());

    /* flush output so it goes to our file */
    fflush(stdout);
    fclose(temp_out);

    /* Now restore stdout */
    _dup2(stdout_dupfd, filenumber);
    _close(stdout_dupfd);

    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, readTempFile(getCommand())};
}

} // namespace post3dapp

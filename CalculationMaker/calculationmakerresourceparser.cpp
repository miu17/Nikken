#include "calculationmakerresourceparser.h"

#include <QIODevice>
#include <QTextCodec>
#include <QTextStream>
#include <QBuffer>

#include <QDebug>
namespace CalculationMakerLib{

class CalculationMakerResourceParserPrivate
{
public:
    QIODevice *dev;
    QTextCodec *codec;

    void init();
    void clear();

    bool open();
    void close();

    bool atEnd() const;

    QChar currentChar() const;
    QChar readChar();
    void skipSpace();
    QString readCommand();
    bool readLeftBrace();
    QString readArgument();
    bool readRightBrace();

    int bufindex;
    int lineno;
    QString linebuf;
    QTextStream *ts;

    QChar quoteChar;

    CalculationMakerResourceParser::ErrorCode error;
    QString errorString;
};

void CalculationMakerResourceParserPrivate::init()
{
    dev = 0;
    codec = 0;
    ts = 0;
    bufindex = 0;
    lineno = 0;
    linebuf.clear();

    quoteChar = QChar();

    error = CalculationMakerResourceParser::NoError;
    errorString.clear();
}

void CalculationMakerResourceParserPrivate::clear()
{
    delete ts;
    init();
}

bool CalculationMakerResourceParserPrivate::open()
{
    if (!dev)
        return false;
    if (dev->isOpen())
        return true;
    linebuf.clear();
    bufindex = 0;
    bool result = dev->open(QIODevice::ReadOnly);
    if (result) {
        ts = new QTextStream(dev);
        if (codec)
            ts->setCodec(codec);
    }
    return result;
}

void CalculationMakerResourceParserPrivate::close()
{
    if (ts && ts->device() && ts->device()->isOpen())
        ts->device()->close();
    clear();
}

bool CalculationMakerResourceParserPrivate::atEnd() const
{
    return !ts || (ts->atEnd() && linebuf.isEmpty());
}

QChar CalculationMakerResourceParserPrivate::currentChar() const
{
    if (atEnd())
        return QChar();
    if (bufindex >= linebuf.length())
        return QLatin1Char('\n');
    QChar ch = linebuf.at(bufindex);
    if (ch == QLatin1Char('#') && quoteChar.isNull())
        return QLatin1Char('\n');
    return linebuf.at(bufindex);
}

QChar CalculationMakerResourceParserPrivate::readChar()
{
    QChar ch = currentChar();
    if (quoteChar.isNull() && (ch == QLatin1Char('\'') || ch == QLatin1Char('"')))
        quoteChar = ch;
    else if (quoteChar == ch)
        quoteChar = QChar();

//    qDebug("readChar(): %d, %d, %c", lineno, bufindex, ch.unicode());
    if (ch == QLatin1Char('\n')) {
        if (!ts->atEnd())
            linebuf = ts->readLine();
        else
            linebuf.clear();
        bufindex = 0;
        ++lineno;
//        qDebug("ReadLine(%d): %s", lineno, qPrintable(linebuf));
//        qDebug() << ts->atEnd();
    } else if (!ch.isNull())
        ++bufindex;
    return ch;
}

void CalculationMakerResourceParserPrivate::skipSpace()
{
    while (bufindex < linebuf.length() && linebuf.at(bufindex).isSpace())
        bufindex++;
}

/**
 * @brief CalculationMakerResourceParserPrivate::readCommand
 * @return
 *
 * コマンドを読み込む。
 * コマンドは \\ か \[a-zA-z][a-zA-Z0-9]* の形式。
 *
 */
QString CalculationMakerResourceParserPrivate::readCommand()
{
//    qDebug("readCommand()");
    skipSpace();
    while (currentChar() == QLatin1Char('\n')) {
        readChar();
        skipSpace();
    }
    QChar ch = currentChar();
    if (ch != QLatin1Char('\\')) {
        return QString();
    }
    QString token(readChar());
    ch = currentChar();
    if (ch.isLetter()) {
        while (ch.isLetterOrNumber()) {
            token += readChar();
            ch = currentChar();
        }
    } else if (ch == QLatin1Char('\\')) {
        token += readChar();
    }
    return token;
}

/**
 * @brief CalculationMakerResourceParserPrivate::readLeftBrace
 * @return
 *
 * 左カギ括弧を読み込む。
 *
 */
bool CalculationMakerResourceParserPrivate::readLeftBrace()
{
//    qDebug("readLeftBrace()");
    skipSpace();
    while (currentChar() == QLatin1Char('\n')) {
        readChar();
        skipSpace();
    }
    QChar ch = currentChar();
    bool result = ch == QLatin1Char('{');
    if (result)
        readChar();
    return result;
}

QString CalculationMakerResourceParserPrivate::readArgument()
{
//    qDebug("readArgument()");
    //skipSpace();
    QChar ch = currentChar();
    QString text;
    int nestCount = 0;
    while (!ch.isNull()) {
        if (quoteChar.isNull()) {
            if (ch == QLatin1Char('}')) {
                if (nestCount == 0)
                    break;
                else
                    --nestCount;
            } else if (ch == QLatin1Char('{')) {
                ++nestCount;
            }
        }
        text += ch;
        readChar();
        ch = currentChar();
    }
    return text;
}

/**
 * @brief CalculationMakerResourceParserPrivate::readRightBrace
 * @return
 *
 * 右カギ括弧を読み込む。
 *
 */
bool CalculationMakerResourceParserPrivate::readRightBrace()
{
//    qDebug("readRightBrace()");
    skipSpace();
    QChar ch = currentChar();
    bool result = ch == QLatin1Char('}');
    if (result)
        readChar();
    return result;
}

CalculationMakerResourceParser::CalculationMakerResourceParser(QObject *parent) :
    QObject(parent), d_ptr(new CalculationMakerResourceParserPrivate)
{
    Q_D(CalculationMakerResourceParser);
    d->init();
}

CalculationMakerResourceParser::~CalculationMakerResourceParser()
{
    d_ptr->clear();
    delete d_ptr;
}

void CalculationMakerResourceParser::setDevice(QIODevice *dev)
{
    Q_D(CalculationMakerResourceParser);
    d->dev = dev;
}

void CalculationMakerResourceParser::setSource(const QByteArray &source)
{
    QBuffer *buf = new QBuffer(this);
    buf->setData(source);
    setDevice(buf);
}

void CalculationMakerResourceParser::setCodec(QTextCodec *codec)
{
    Q_D(CalculationMakerResourceParser);
    d->codec = codec;
}

int CalculationMakerResourceParser::lineNo() const
{
    Q_D(const CalculationMakerResourceParser);
    return d->lineno;
}

QString CalculationMakerResourceParser::errorString() const
{
    Q_D(const CalculationMakerResourceParser);
    return d->errorString;
}

void CalculationMakerResourceParser::start()
{
    Q_D(CalculationMakerResourceParser);
    if (!d->dev) {
        return;
    }

    bool result = d->open();
    if (!result) {
        d->error = CannotOpen;
        d->errorString = tr("Can not open device.");
        emit parseError(d->error);
        return;
    }

    while (!d->atEnd()) {
        QString token = d->readCommand();
        if (token.isEmpty() && d->atEnd())
            break;

        if (d->atEnd()) {
            d->error = UnexpectedEOF;
            d->errorString = tr("Unexpected End of File.");
        } else if (token.isEmpty()) {
            d->error = BackSlashIsExpected;
            d->errorString = tr("Backslash is expected.");
        } else if (token == u8"\\") {
            d->error = InvalidCommand;
            d->errorString = tr("Invalid Command.");
        }
        if (d->error) {
            emit parseError(d->error);
            break;
        }
//        qDebug() << "Token:" << token;

        result = d->readLeftBrace();
        if (!result) {
            if (token == u8"\\\\") {
                emit parseToken(token, QString(), lineNo());
                continue;
            }
            d->error = LeftBraceNotFound;
            d->errorString = tr("Left Brace is not found.");
            emit parseError(d->error);
            break;
        }

        QString arg = d->readArgument();
//        qDebug() << "Arg:" << arg;
        if (d->atEnd()) {
            d->error = UnexpectedEOF;
            d->errorString = tr("Unexpected End of File.");
            emit parseError(d->error);
            break;
        }

        result = d->readRightBrace();
        if (!result) {
            d->error = RightBraceNotFound;
            d->errorString = tr("Right Brace is not found.");
            emit parseError(d->error);
            break;
        }

        emit parseToken(token, arg, lineNo());
//        qDebug() << d->atEnd();
    }
}
} // namespace CalculationMakerLib

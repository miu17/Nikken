#ifndef CALCULATIONMAKERRESOURCEPARSER_H
#define CALCULATIONMAKERRESOURCEPARSER_H

#include <QObject>

class QIODevice;
class QTextCodec;

namespace CalculationMakerLib{

class CalculationMakerResourceParserPrivate;

/**
 * @brief The CalculationMakerResourceParser class
 *
 * リソースファイルを読み込んで必要なトークンに分解する。
 *
 * @internal
 */
class CalculationMakerResourceParser : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CalculationMakerResourceParser)
public:
    explicit CalculationMakerResourceParser(QObject *parent = 0);
    ~CalculationMakerResourceParser();

    enum ErrorCode { NoError, CannotOpen, UnexpectedEOF, BackSlashIsExpected, InvalidCommand, LeftBraceNotFound, RightBraceNotFound };

    void setDevice(QIODevice *dev);
    void setSource(const QByteArray &source);
    void setCodec(QTextCodec *codec);

    int lineNo() const;
    ErrorCode error() const;
    QString errorString() const;

signals:
    void parseFinished();
    void parseError(int error);
    void parseToken(const QString &command, const QString &arg, int lineno);

public slots:
    void start();

private:
    CalculationMakerResourceParserPrivate *d_ptr;
};
} // namespace CalculationMakerLib

#endif // CALCULATIONMAKERRESOURCEPARSER_H

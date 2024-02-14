#include "calculationmaker.h"

#include "calculationmakercontext.h"

#include "calculationmakerresourceparser.h"

#include <QFont>
#include <QRegExp>
#include <QFile>

#include <QDebug>

namespace CalculationMakerLib{

class CalculationMakerPrivate
{
public:
    CalculationMakerContext *context;
    CalculationMakerResourceParser *parser;
    QTextCodec *codec;
};

/**
 * @brief CalculationMaker::CalculationMaker
 * @param parent
 *
 * コンストラクタ
 *
 */
CalculationMaker::CalculationMaker(const post3dapp::OutputPaperSetting& _setting, QObject *parent) :
    QObject(parent), d_ptr(new CalculationMakerPrivate), setting(_setting)
{
    Q_D(CalculationMaker);

    QFont::insertSubstitution(u8"Minion Pro", u8"メイリオ");
    QFont::insertSubstitution(u8"Century Gothic", u8"ＭＳ 明朝");
    /*
    QFont::insertSubstitution(QString::fromLocal8Bit("Times New Roman"),
                              QString::fromUtf8("\xef\xbc\xad\xef\xbc\xb3 \xe6\x98\x8e\xe6\x9c\x9d")); // ＭＳ 明朝
    QFont::insertSubstitution(QString::fromLocal8Bit("Consolas"),
                              QString::fromLocal8Bit("Meiryo UI"));
    QFont::insertSubstitution(QString::fromLocal8Bit("Arial"),
                              QString::fromUtf8("\xef\xbc\xad\xef\xbc\xb3 \xe3\x82\xb4\xe3\x82\xb7\xe3\x83\x83\xe3\x82\xaf")); // ＭＳ ゴシック
    */
    d->context = new CalculationMakerContext(this, _setting);

    d->parser = new CalculationMakerResourceParser(this);
    connect(d->parser, SIGNAL(parseError(int)), SLOT(handleParseError()));
    connect(d->parser, SIGNAL(parseToken(QString,QString,int)), SLOT(handleCommand(QString,QString,int)));

    d->codec = 0;
}

/**
 * @brief CalculationMaker::~CalculationMaker
 *
 * デストラクタ
 *
 */
CalculationMaker::~CalculationMaker()
{
    Q_D(CalculationMaker);
    delete d->context;
    delete d_ptr;
}

/**
 * @brief CalculationMaker::setDevice
 * @param dev
 *
 * ソースを読み込むデバイスとして dev を指定する。
 *
 */
void CalculationMaker::setDevice(QIODevice *dev)
{
    Q_D(CalculationMaker);
    Q_ASSERT(d->parser);
    d->parser->setDevice(dev);
}

/**
 * @brief CalculationMaker::setSource
 * @param buf
 *
 * ソースを指定する。
 *
 */
void CalculationMaker::setSource(const QByteArray &buf)
{
    Q_D(CalculationMaker);
    Q_ASSERT(d->parser);
    d->parser->setSource(buf);
}

/**
 * @brief CalculationMaker::setCodec
 * @param codec
 *
 * ソースを読み込む際に使用するコーデックを指定する。
 *
 */
void CalculationMaker::setCodec(QTextCodec *codec)
{
    Q_D(CalculationMaker);
    d->codec = codec;
}

/**
 * @brief CalculationMaker::setOutputFileName
 * @param filename
 *
 * 出力する PDF ファイルの名前を指定する。
 */
void CalculationMaker::setOutputFileName(const QString &filename)
{
    Q_D(CalculationMaker);
    d->context->setOutputFilename(filename);
}

/**
 * @brief CalculationMaker::context
 * @return 描画用コンテキスト
 *
 *
 *
 * @internal
 */
CalculationMakerContext *CalculationMaker::context()
{
    Q_D(CalculationMaker);
    return d->context;
}

/**
 * @brief CalculationMaker::setFigureClass
 * @param figure
 *
 * 図描画用クラスを設定する。
 *
 */
void CalculationMaker::setFigureClass(FigureInterface *figure)
{
    Q_D(CalculationMaker);
    d->context->setFigureCommand(figure);
}

/**
 * @brief CalculationMaker::setTableClass
 * @param table
 *
 * テーブル描画用クラスを設定する。
 *
 */
void CalculationMaker::setTableClass(TableInterface *table)
{
    Q_D(CalculationMaker);
    d->context->setTableCommand(table);
}

/**
 * @brief CalculationMaker::parse
 *
 * ソースを読み込んで PDF への出力を行う。
 *
 */
void CalculationMaker::parse()
{
    Q_D(CalculationMaker);
    Q_ASSERT(d->parser);
    Q_ASSERT(d->context);

    QString out = d->context->outputFilename();
    if (out.isEmpty()) {
        qWarning("CalculationMaker: Output filename is not set!");
        return;
    }

    d->parser->start();
    d->context->endPrinting();
}

void CalculationMaker::handleParseError()
{
    Q_D(const CalculationMaker);
    qDebug() << "Error:" << d->parser->errorString() << "at line" << d->parser->lineNo();
}

void CalculationMaker::handleCommand(const QString &command, const QString &arg, int lineno)
{
    Q_D(CalculationMaker);
    d->context->draw(command, arg, lineno);
}
} // namespace CalculationMakerLib

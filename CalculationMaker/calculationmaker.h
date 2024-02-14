#ifndef CALCULATIONMAKER_H
#define CALCULATIONMAKER_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include "define_draw_figure.h"

#ifdef CALCULATIONMAKER_LIB
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QIODevice;
class QTextCodec;

namespace CalculationMakerLib{

class CalculationMakerContext;

class FigureInterface;
class TableInterface;



class CalculationMakerPrivate;

/**
 * @brief The CalculationMaker class
 *
 * PDF作成のユーザーインターフェイスとなるクラス
 *
 */
class CALCULATIONMAKER_CLASS_DLLSPEC CalculationMaker : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CalculationMaker)
public:
    explicit CalculationMaker(const post3dapp::OutputPaperSetting&, QObject *parent = 0);
    ~CalculationMaker();

    void setDevice(QIODevice *dev);
    void setSource(const QByteArray &buf);
    void setCodec(QTextCodec *codec);
    //void setCalculationPaperSetting(const CalculationPaperSetting&);

    void setOutputFileName(const QString &filename);

    CalculationMakerContext *context();

    void setFigureClass(FigureInterface *figure);
    void setTableClass(TableInterface *table);

signals:

public slots:
    void parse();

private slots:
    void handleParseError();
    void handleCommand(const QString &command, const QString &arg, int lineno);

private:

public:
    QString projectName;
    QString structureType;
    QMap<QString, QString> textSettings;
    QMap<QString, bool> sectionSettings;
    QMap<QString, QStringList> outGroups;
    QMap<QString, post3dapp::PAPERSETTING> groupFormats;
    QMap<QString, QString> figureToGroup;
    post3dapp::OutputPaperSetting getSetting() const{return setting;}

private:
    CalculationMakerPrivate *d_ptr;
    post3dapp::OutputPaperSetting setting;

};
} // namespace CalculationMakerLib

#endif // CALCULATIONMAKER_H

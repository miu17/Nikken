#ifndef CALCULATIONMAKERPAGE_H
#define CALCULATIONMAKERPAGE_H

#include <QObject>

#include "calculationmaker.h"

class QPainter;

class CalculationMakerPage : public QObject
{
    Q_OBJECT
public:
    explicit CalculationMakerPage(QObject *parent = 0);

    PAPERSETTING paperSetting() const;
    void setPaperSetting(const PAPERSETTING &setting);

    void render(QPainter *painter);
signals:

public slots:

private:
    PAPERSETTING setting;
};

#endif // CALCULATIONMAKERPAGE_H

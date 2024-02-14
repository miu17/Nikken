#ifndef SECTION_LINECALCULATE_H
#define SECTION_LINECALCULATE_H

#include <QWidget>
#include "define_sectiontable.h"
#include "define_section_struct.h"
namespace post3dapp{
class  SectionLineCalculate: public QWidget
{

    Q_OBJECT

public:

    explicit SectionLineCalculate(const SectionDataType &type, QWidget *parent = 0);

    void setLineValues(const QList<QVariant> &values, const SectionValues &section);

private:

    QList<QVariant> myValues;
    SectionDataType mySectionType;
    SectionValues mySection;
    QList<QList<QStringList> > formatoutput;
    bool hasCalced;
    bool table3input;
    bool table4input;
    bool hasDirectInput;

    //以下、mySectionTypeのデータの一部をコピーするだけのもの
    DATATYPE mtype;
    STRUCTTYPE stype;
    bool isColumn;
    bool isGirder;
    bool isBeam;
    bool isBrace;
    bool isWall;
    bool isUWall;
    bool isSlab;
    bool isIsolator;
    bool isS;
    bool isRC;
    bool isSRC;
    bool isCFT;

protected:

    void paintEvent(QPaintEvent *) override;
    LineOutputData calculateData(const LinePartsParameter &inputdata);
    PlaneOutputData calculateData(const PlaneSectionParameter &inputdata);
    QList<QList<QStringList> > formatResult(LineOutputData outputdata[]);
    QList<QList<QStringList> > formatResult(PlaneOutputData outputdata[]);
    QList<QList<QStringList> > isolatorFormatResult();
    void drawResult(const QList<QList<QStringList> > &foutp, QPainter *resultpaint);

};
} // namespace post3dapp

#endif // SECTION_LINECALCULATE_H

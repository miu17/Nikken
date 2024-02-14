#ifndef ABSTRACTWRITE_H
#define ABSTRACTWRITE_H

#include <QObject>
#include <QStringList>
#include "calculationmaker.h"
#include "calculationpapersettingwidget.h"

namespace post3dapp{
class AbstractWrite : public QObject
{
public:
    AbstractWrite(const QStringList& _list):list(_list){}
    virtual ~AbstractWrite();

    virtual QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&) const = 0;
    virtual QString getTextResult() const {return QString{};}

protected:
    virtual QString getCommand() const = 0;
    virtual QString getSectionName() const = 0;
    QStringList list;
    QString prefix() const {return u8"#" + getCommand() + u8"\n" + begin() + section();}
    QString suffix() const {return newPage() + end() + u8"\n";}
    QString newPage() const {return u8"\\newpage{} \n";}
    QString figure(QString group, qreal x, qreal y, qreal width, qreal height) const
    {return u8"\\includefigure{" + group + u8", " + QString::number(x) + u8", " + QString::number(y) + u8", " + QString::number(width) + u8", " + QString::number(height) + u8"} \n";}
    QString header(QString onoff) const {return u8"\\header{" + onoff + u8"} \n";}
    QString footer(QString onoff) const {return u8"\\footer{" + onoff + u8"} \n";}
    QString chapter(QString name = QString()) const {return u8"\\chapter{" + name + u8"} \n";}
    QString section() const {return u8"\\section{" + getSectionName() + u8"} \n";}
    QString subsection(QString subsection) const {return u8"\\subsection{" + subsection + u8"} \n";}
    QString title(QString title) const { return u8"\\title{" + title + u8"} \n";}
    QString text(QString text) const { return u8"\\text{" + text + u8"} \n";}
    QString font(QString font, QString size) const{return u8"\\font{\"" + font + u8"\"," + size + u8"} \n";}
    QString begin() const{return u8"\\begin{" + getCommand() + u8"} \n";}
    QString end() const{return u8"\\end{" + getCommand() + u8"} \n";}
    QString makeSelfFigure(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting&,  FIGURE_TYPE_PROPERTY) const;
    QString makeStressFigure(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting&,  FIGURE_TYPE_PROPERTY) const;
    QString makeModelFigure(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting&,  FIGURE_TYPE_PROPERTY) const;
private:

};

#define WRITE_COMMAND(classname) \
    class classname : public AbstractWrite \
    { \
    public: \
        classname(const QStringList&); \
        const static QString Command; \
        const static QString Japanese; \
        QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&) const override; \
    protected: \
        QString getCommand() const override {return Command;} \
        QString getSectionName() const override {return Japanese;} \
    };


WRITE_COMMAND(WriteOutgroupp)
WRITE_COMMAND(WriteOutgroups)
WRITE_COMMAND(WriteCover)
WRITE_COMMAND(WriteOrganize)
WRITE_COMMAND(WriteOutline)
WRITE_COMMAND(WritePlan)
WRITE_COMMAND(WriteSection)
WRITE_COMMAND(WriteLsgir)
WRITE_COMMAND(WriteLscol)
WRITE_COMMAND(WriteLswal)
WRITE_COMMAND(WriteLsswa)
WRITE_COMMAND(WriteLsfwa)
WRITE_COMMAND(WriteLsbra)
WRITE_COMMAND(WriteLsbeam)
WRITE_COMMAND(WriteLsslab)
WRITE_COMMAND(WriteLsopen)
WRITE_COMMAND(WriteLsbs)
WRITE_COMMAND(WriteLsbsrc)
WRITE_COMMAND(WriteLsdamp)
WRITE_COMMAND(WriteLsmen)
WRITE_COMMAND(WriteAlist)
WRITE_COMMAND(WriteCalc)
WRITE_COMMAND(WriteStnd)
WRITE_COMMAND(WritePolicy)
WRITE_COMMAND(WriteStt)
WRITE_COMMAND(WriteMatc)
WRITE_COMMAND(WriteMatr)
WRITE_COMMAND(WriteMats)
WRITE_COMMAND(WriteCcs)
WRITE_COMMAND(WritePolicyld)
WRITE_COMMAND(WriteLdl)
WRITE_COMMAND(WriteLdf)
WRITE_COMMAND(WriteLdm)
WRITE_COMMAND(WriteLdj)
WRITE_COMMAND(WriteSnow)
WRITE_COMMAND(WriteWind)
WRITE_COMMAND(WriteOther)
WRITE_COMMAND(WriteLplan)
WRITE_COMMAND(WriteSeisle)
WRITE_COMMAND(WriteLcmqp)
WRITE_COMMAND(WriteLcmqs)
WRITE_COMMAND(WriteJmassp)
WRITE_COMMAND(WriteJmasss)
WRITE_COMMAND(WriteEstim)
WRITE_COMMAND(WritePolicyec)
WRITE_COMMAND(WriteCmodel)
WRITE_COMMAND(WriteEmodel)
WRITE_COMMAND(WriteModelp)
WRITE_COMMAND(WriteModels)
WRITE_COMMAND(WriteMphaip)
WRITE_COMMAND(WriteMphais)
WRITE_COMMAND(WriteMrlp)
WRITE_COMMAND(WriteMrls)
WRITE_COMMAND(WriteMclp)
WRITE_COMMAND(WriteMcls)
WRITE_COMMAND(WriteMblp)
WRITE_COMMAND(WriteMbls)
WRITE_COMMAND(WriteAnacon)
WRITE_COMMAND(WriteRfloor)
WRITE_COMMAND(WriteJspring)
WRITE_COMMAND(WriteMspring)
WRITE_COMMAND(WriteMdetail)
WRITE_COMMAND(WriteAutophai)
WRITE_COMMAND(WriteAutolk)
WRITE_COMMAND(WriteJnt)
WRITE_COMMAND(WriteIndex)
WRITE_COMMAND(WriteFrame)
WRITE_COMMAND(WritePnlmdl)
WRITE_COMMAND(WriteOutcs)
WRITE_COMMAND(WriteVjstrp)
WRITE_COMMAND(WriteVjstrs)
WRITE_COMMAND(WriteVfstrp)
WRITE_COMMAND(WriteVfstrs)
WRITE_COMMAND(WriteHratio)
WRITE_COMMAND(WriteHjstrp)
WRITE_COMMAND(WriteHjstrs)
WRITE_COMMAND(WriteHfstrp)
WRITE_COMMAND(WriteHfstrs)
WRITE_COMMAND(WriteCrez)
WRITE_COMMAND(WriteReact)
WRITE_COMMAND(WriteStstiff)
WRITE_COMMAND(WriteFrstiff)
WRITE_COMMAND(WriteFrdist)
WRITE_COMMAND(WriteQmap)
WRITE_COMMAND(WritePolicych)
WRITE_COMMAND(WriteChcase)
WRITE_COMMAND(WriteChvp)
WRITE_COMMAND(WriteChvs)
WRITE_COMMAND(WriteChhp)
WRITE_COMMAND(WriteChhs)
WRITE_COMMAND(WriteChlistm)
WRITE_COMMAND(WriteChlistq)
WRITE_COMMAND(WriteChcol)
WRITE_COMMAND(WriteChgir)
WRITE_COMMAND(WriteChwal)
WRITE_COMMAND(WriteChbra)
WRITE_COMMAND(WriteChopen)
WRITE_COMMAND(WriteChps2)
WRITE_COMMAND(WriteChprc2)
WRITE_COMMAND(WriteChpsrc2)
WRITE_COMMAND(WriteChpcft2)
WRITE_COMMAND(WriteChps3)
WRITE_COMMAND(WriteChprc3)
WRITE_COMMAND(WriteChpsrc3)
WRITE_COMMAND(WriteChpcft3)
WRITE_COMMAND(WriteChsrcbase)
WRITE_COMMAND(WriteChsbase)
WRITE_COMMAND(WriteChneobase)
WRITE_COMMAND(WriteChnewbase)
WRITE_COMMAND(WriteChbpbase)
WRITE_COMMAND(WriteChcutoff)
WRITE_COMMAND(WriteChbondl1)
WRITE_COMMAND(WriteChbonds1)
WRITE_COMMAND(WriteChbonds2)
WRITE_COMMAND(WritePolicyf)
WRITE_COMMAND(WritePolicystec)
WRITE_COMMAND(WriteMxdfrm)
WRITE_COMMAND(WriteStff)
WRITE_COMMAND(WriteEccf)
WRITE_COMMAND(WritePrestec)
WRITE_COMMAND(WriteGckcp)
WRITE_COMMAND(WriteStecf)
WRITE_COMMAND(WriteCwarea)
WRITE_COMMAND(WriteCgcmp1)
WRITE_COMMAND(WritePolicyquds)
WRITE_COMMAND(WriteQuds)
WRITE_COMMAND(WriteSeislp)
WRITE_COMMAND(WritePolicypc)
WRITE_COMMAND(WritePmodel)
WRITE_COMMAND(WriteDspush)
WRITE_COMMAND(WriteDsgene)
WRITE_COMMAND(WriteDsstep)
WRITE_COMMAND(WriteDscurve)
WRITE_COMMAND(WriteDsstr)
WRITE_COMMAND(WriteDscond)
WRITE_COMMAND(WriteMrnk)
WRITE_COMMAND(WriteRncs)
WRITE_COMMAND(WriteRncrc)
WRITE_COMMAND(WriteRncsrc)
WRITE_COMMAND(WriteRnccft)
WRITE_COMMAND(WriteRngs)
WRITE_COMMAND(WriteRngrc)
WRITE_COMMAND(WriteRngsrc)
WRITE_COMMAND(WriteRnvs)
WRITE_COMMAND(WriteRnwrc)
WRITE_COMMAND(WriteRnwsrc)
WRITE_COMMAND(WriteRnprc)
WRITE_COMMAND(WriteQun)
WRITE_COMMAND(WriteGrbend)
WRITE_COMMAND(WriteGrbondi)
WRITE_COMMAND(WriteGrbondp)
WRITE_COMMAND(WriteYokohogou)
WRITE_COMMAND(WriteDssh)
WRITE_COMMAND(WriteDsmu)
WRITE_COMMAND(WriteQupush)
WRITE_COMMAND(WriteQugene)
WRITE_COMMAND(WriteQustep)
WRITE_COMMAND(WriteQucurve)
WRITE_COMMAND(WriteQucond)
WRITE_COMMAND(WriteQudetrm)
WRITE_COMMAND(WriteQustr)
WRITE_COMMAND(WriteQucrc)
WRITE_COMMAND(WriteQucsrc)
WRITE_COMMAND(WriteQugrc)
WRITE_COMMAND(WriteQugsrc)
WRITE_COMMAND(WriteQuwrc)
WRITE_COMMAND(WriteQuwsrc)
WRITE_COMMAND(WriteQups)
WRITE_COMMAND(WriteQuprc)
WRITE_COMMAND(WriteQupsrc)
WRITE_COMMAND(WriteQupcft)
WRITE_COMMAND(WriteQush)
WRITE_COMMAND(WriteDsqucurve)
WRITE_COMMAND(WriteQuneed)
WRITE_COMMAND(WriteQuneedf)
WRITE_COMMAND(WriteCgcmpm)
WRITE_COMMAND(WriteCgcmpf)
WRITE_COMMAND(WriteMbcpr)
WRITE_COMMAND(WriteQbcpr)
WRITE_COMMAND(WriteMult)
WRITE_COMMAND(WritePrograme)
WRITE_COMMAND(WriteMessage)
WRITE_COMMAND(WriteOpinionc)
WRITE_COMMAND(WriteOpinion)
WRITE_COMMAND(WriteContents)
WRITE_COMMAND(WriteContents2)
WRITE_COMMAND(WriteEnd)


} // namespace post3dapp

#endif // ABSTRACTWRITE_H

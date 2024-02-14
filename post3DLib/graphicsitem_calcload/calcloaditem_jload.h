#ifndef CALCLOADJOINTLOADITEM_H
#define CALCLOADJOINTLOADITEM_H

#include "calcloaditem_joint.h"
namespace post3dapp{
class CalcLoadJointLoadItem : public CalcLoadJointItem
{

public:
    CalcLoadJointLoadItem( DATATYPE dtype,
                           const QUuid &uid,
                           const CalculationLoad2DViewSettings &setting,
                           const FIGURE_TYPE_PROPERTY&,
                           int inum,
                           VEC3D vec,
                           const QList<JLOADSUM *> & );

    void setStatus(const FIGURE_TYPE_PROPERTY&);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

private:

    QPainterPath drawAutoLoadPath;
    QPainterPath drawManualLoadPath;

    const QList<JLOADSUM *> myLoads;
    const VEC3D normVec;

    LoadType::LOADTYPE loadType;
    LOADSUMTYPE loadSumType;
    OUTPUTDIR outputDir;

    int stringSize;
    int valueSize;
    bool jLoadVisible;
    QColor jLoadColor;

    void appendArrow( const QString &, bool, bool isManual = true );
    void appendDoubleArrow( const QString &, bool, bool isManual = true);

};
} // namespace post3dapp
#endif

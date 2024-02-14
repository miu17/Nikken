#include "persOT_partsdraw.h"

#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "persOT_glview.h"
#include "unified_analysisdata.h"
#include "unified_output.h"
#include "unified_settings.h"

namespace post3dapp{
PersOutputPartsDrawing::PersOutputPartsDrawing( QObject *parent )
    : QObject(parent)
{
}

void PersOutputPartsDrawing::drawLineOutput( MemberData *md, OUTFIGTYPE figtype,
                                             int caseIdxA, int caseIdxB, int caseIdxC )
{
    QList<JointData *> jdlist = md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getAnalysisData());
    QList<qreal> lenlist; // 単位 mm
    Q_FOREACH ( JointData *jd, jdlist ) lenlist.append( md->getJointLength(jd) );

    glDisable(GL_LIGHTING);

    glBegin(GL_LINE_STRIP);
    Q_FOREACH ( JointData *jd, jdlist )
        glVertex3d( jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos(true) * 1000.0 );
    glEnd();

    switch ( figtype ) {
    default:
        break;
    }

    glEnable(GL_LIGHTING);
}

void PersOutputPartsDrawing::drawJointOutput( JointData *jd, OUTFIGTYPE figtype,
                                              int caseIdxA, int caseIdxB, int caseIdxC )
{
    glDisable(GL_LIGHTING);

    qreal v_alpha = qreal(UnifiedSettings::getInstance()->getCalc3DViewSettings().getValueAlpha()) / 255.0;


    glEnable(GL_LIGHTING);
}

void PersOutputPartsDrawing::drawColorBar( PersOutputGLViewer *viewer, OUTFIGTYPE figtype )
{
    glDisable(GL_LIGHT0);

    viewer->startScreenCoordinatesSystem();

    int str_size = UnifiedSettings::getInstance()->getCalc3DViewSettings().getColorBarTextSize() + 7;
    QColor str_col = UnifiedSettings::getInstance()->getCalc3DViewSettings().getColorBarTextColor();

    QFont font( tr("ＭＳ ゴシック"), str_size );
    QFontMetrics fm(font);
    int fh = fm.height();
    int width = viewer->width();
    int height = viewer->height();
    int cur_h = height - 10;



    viewer->stopScreenCoordinatesSystem();
}




QColor PersOutputPartsDrawing::valueToColor( qreal var, const QColor &minCol, qreal minVar,
                                             const QColor &maxCol, qreal maxVar )
{
    if ( qAbs( maxVar - minVar ) < 1.0e-5 ) return minCol;
    qreal vr = qAbs( var - minVar ) / qAbs( maxVar - minVar );
    qreal cr = minCol.redF()  + ( maxCol.redF() - minCol.redF() ) * vr;
    qreal cg = minCol.greenF() + ( maxCol.greenF() - minCol.greenF() ) * vr;
    qreal cb = minCol.blueF() + ( maxCol.blueF() - minCol.blueF() ) * vr;
    cr = qMin(cr, 1.0);
    cr = qMax(cr, 0.0);
    cg = qMin(cg, 1.0);
    cg = qMax(cg, 0.0);
    cb = qMin(cb, 1.0);
    cb = qMax(cb, 0.0);
    return QColor( cr * 255.0, cg * 255.0, cb * 255.0 );
}

} // namespace post3dapp

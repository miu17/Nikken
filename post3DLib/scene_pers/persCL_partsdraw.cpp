#include "persCL_partsdraw.h"

#include "define_output.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
//#include "output_control.h"
#include "persCL_glview.h"
#include "unified_calcload.h"
#include "unified_data.h"
#include "unified_inputdata.h"
#include "unified_settings.h"

namespace post3dapp{

PersCalcLoadPartsDrawing::PersCalcLoadPartsDrawing( QObject *parent )
    : QObject(parent)
{
}

void PersCalcLoadPartsDrawing::drawLineOutput( MemberData *md, LoadType::LOADTYPE caseIdxA, LOADSUMTYPE caseIdxB, CALCOUTPUTDIR caseIdxC )
{
    QList<JointData *> jdlist = md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData());
    QList<qreal> lenlist; // unit mm
    Q_FOREACH ( JointData *jd, jdlist ) lenlist.append( md->getJointLength(jd) );

    glDisable(GL_LIGHTING);

    glBegin(GL_LINE_STRIP);
    Q_FOREACH ( JointData *jd, jdlist )
        glVertex3d( jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos(true) * 1000.0 );
    glEnd();

    drawLineLoad( md, jdlist, lenlist, caseIdxA, caseIdxB, caseIdxC );

    glEnable(GL_LIGHTING);
}

void PersCalcLoadPartsDrawing::drawJointOutput( JointData *jd, LoadType::LOADTYPE ltype, LOADSUMTYPE lstype)
//void PersCalcLoadPartsDrawing::drawJointOutput( JointData *jd, int caseIdxA, int caseIdxB, int caseIdxC )
{
    glDisable(GL_LIGHTING);

    qreal v_alpha = qreal(UnifiedSettings::getInstance()->getCalc3DViewSettings().getValueAlpha()) / 255.0;
    if ( lstype == LOADSUMTYPE::LOADSUM_SELF ) return;

    CalculationLoad3DViewSettings loadSetting = UnifiedSettings::getInstance()->getCalc3DViewSettings();
    if ( !loadSetting.getIsJloadVisible() ) return;

    QList<JLOADSUM *> jloads = UnifiedFixedData::getInstance()->getCalcLoadData()->loadOfJoint(jd);

    qreal dsize = qreal(loadSetting.getJloadSize()) * 100.0 + 300.0;
    qreal rsize = dsize / 2.0;

    for ( int i = 0; i < jloads.count(); i++ ) {

        if ( jloads.at(i)->loadType != ltype ) continue;
        if ( jloads.at(i)->loadName.isEmpty() && lstype == LOADSUMTYPE::LOADSUM_MANUAL ) continue;
        if ( !jloads.at(i)->loadName.isEmpty() && lstype == LOADSUMTYPE::LOADSUM_AUTO ) continue;

        qreal cur_load = jloads.at(i)->concentratedLoad;
        QColor col = valueToColor( cur_load, loadSetting.getJloadMinColor(), loadSetting.getJloadMinValue(),
                                   loadSetting.getJloadMaxColor(), loadSetting.getJloadMaxValue() );
        glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );
        VEC3D u1, u2, u3, u4, u5;
        VEC3D u0( jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos(true) * 1000.0 );

        if ( jloads.at(i)->loadDir == LOADDIR::LDIR_X || jloads.at(i)->loadDir == LOADDIR::LDIR_MX ) {
            glNormal3d(1.0, 0.0, 0.0);
            u1 = VEC3D(u0.x, u0.y, u0.z);
            u2 = ( cur_load > 0.0 ) ? VEC3D(u0.x - dsize, u0.y - rsize, u0.z - rsize)
                                    : VEC3D(u0.x + dsize, u0.y - rsize, u0.z - rsize) ;
            u3 = ( cur_load > 0.0 ) ? VEC3D(u0.x - dsize, u0.y + rsize, u0.z - rsize)
                                    : VEC3D(u0.x + dsize, u0.y + rsize, u0.z - rsize) ;
            u4 = ( cur_load > 0.0 ) ? VEC3D(u0.x - dsize, u0.y - rsize, u0.z + rsize)
                                    : VEC3D(u0.x + dsize, u0.y - rsize, u0.z + rsize) ;
            u5 = ( cur_load > 0.0 ) ? VEC3D(u0.x - dsize, u0.y + rsize, u0.z + rsize)
                                    : VEC3D(u0.x + dsize, u0.y + rsize, u0.z + rsize) ;
        } else if ( jloads.at(i)->loadDir == LOADDIR::LDIR_Y || jloads.at(i)->loadDir == LOADDIR::LDIR_MY ) {
            glNormal3d(0.0, 1.0, 0.0);
            u1 = VEC3D(u0.x, u0.y, u0.z);
            u2 = ( cur_load > 0.0 ) ? VEC3D(u0.x - rsize, u0.y - dsize, u0.z - rsize)
                                    : VEC3D(u0.x - rsize, u0.y + dsize, u0.z - rsize) ;
            u3 = ( cur_load > 0.0 ) ? VEC3D(u0.x - rsize, u0.y - dsize, u0.z + rsize)
                                    : VEC3D(u0.x - rsize, u0.y + dsize, u0.z + rsize) ;
            u4 = ( cur_load > 0.0 ) ? VEC3D(u0.x + rsize, u0.y - dsize, u0.z - rsize)
                                    : VEC3D(u0.x + rsize, u0.y + dsize, u0.z - rsize) ;
            u5 = ( cur_load > 0.0 ) ? VEC3D(u0.x + rsize, u0.y - dsize, u0.z + rsize)
                                    : VEC3D(u0.x + rsize, u0.y + dsize, u0.z + rsize) ;
        } else {
            glNormal3d(0.0, 0.0, 1.0);
            u1 = VEC3D(u0.x, u0.y, u0.z);
            u2 = ( cur_load > 0.0 ) ? VEC3D(u0.x - rsize, u0.y - rsize, u0.z - dsize)
                                    : VEC3D(u0.x - rsize, u0.y - rsize, u0.z + dsize) ;
            u3 = ( cur_load > 0.0 ) ? VEC3D(u0.x - rsize, u0.y + rsize, u0.z - dsize)
                                    : VEC3D(u0.x - rsize, u0.y + rsize, u0.z + dsize) ;
            u4 = ( cur_load > 0.0 ) ? VEC3D(u0.x + rsize, u0.y - rsize, u0.z - dsize)
                                    : VEC3D(u0.x + rsize, u0.y - rsize, u0.z + dsize) ;
            u5 = ( cur_load > 0.0 ) ? VEC3D(u0.x + rsize, u0.y + rsize, u0.z - dsize)
                                    : VEC3D(u0.x + rsize, u0.y + rsize, u0.z + dsize) ;
        }

        if ( jloads.at(i)->loadDir == LOADDIR::LDIR_X || jloads.at(i)->loadDir == LOADDIR::LDIR_Y
             || jloads.at(i)->loadDir == LOADDIR::LDIR_Z ) {
            glBegin(GL_TRIANGLE_FAN);
            glVertex3d(u1.x, u1.y, u1.z);
            glVertex3d(u2.x, u2.y, u2.z);
            glVertex3d(u3.x, u3.y, u3.z);
            glVertex3d(u5.x, u5.y, u5.z);
            glVertex3d(u4.x, u4.y, u4.z);
            glEnd();
            glBegin(GL_POLYGON);
            glVertex3d(u2.x, u2.y, u2.z);
            glVertex3d(u3.x, u3.y, u3.z);
            glVertex3d(u5.x, u5.y, u5.z);
            glVertex3d(u4.x, u4.y, u4.z);
            glEnd();
            col = loadSetting.getJloadMinColor();
            glColor3d( col.redF(), col.greenF(), col.blueF() );
            glBegin(GL_LINES);
            glVertex3d(u1.x, u1.y, u1.z);
            glVertex3d(u2.x, u2.y, u2.z);
            glVertex3d(u1.x, u1.y, u1.z);
            glVertex3d(u3.x, u3.y, u3.z);
            glVertex3d(u1.x, u1.y, u1.z);
            glVertex3d(u4.x, u4.y, u4.z);
            glVertex3d(u1.x, u1.y, u1.z);
            glVertex3d(u5.x, u5.y, u5.z);
            glVertex3d(u2.x, u2.y, u2.z);
            glVertex3d(u3.x, u3.y, u3.z);
            glVertex3d(u3.x, u3.y, u3.z);
            glVertex3d(u5.x, u5.y, u5.z);
            glVertex3d(u5.x, u5.y, u5.z);
            glVertex3d(u4.x, u4.y, u4.z);
            glVertex3d(u4.x, u4.y, u4.z);
            glVertex3d(u2.x, u2.y, u2.z);
            glEnd();
        } else {
            glBegin(GL_TRIANGLES);
            glVertex3d(u1.x, u1.y, u1.z);
            glVertex3d(u2.x, u2.y, u2.z);
            glVertex3d(u5.x, u5.y, u5.z);
            glVertex3d(u1.x, u1.y, u1.z);
            glVertex3d(u3.x, u3.y, u3.z);
            glVertex3d(u4.x, u4.y, u4.z);
            glEnd();
            col = loadSetting.getJloadMinColor();
            glColor3d( col.redF(), col.greenF(), col.blueF() );
            glBegin(GL_LINE_LOOP);
            glVertex3d(u1.x, u1.y, u1.z);
            glVertex3d(u2.x, u2.y, u2.z);
            glVertex3d(u5.x, u5.y, u5.z);
            glEnd();
            glBegin(GL_LINE_LOOP);
            glVertex3d(u1.x, u1.y, u1.z);
            glVertex3d(u3.x, u3.y, u3.z);
            glVertex3d(u4.x, u4.y, u4.z);
            glEnd();
        }
    }


    glEnable(GL_LIGHTING);
}

void PersCalcLoadPartsDrawing::drawColorBar( PersCalcLoadGLViewer *viewer)
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

    CalculationLoad3DViewSettings loadSetting = UnifiedSettings::getInstance()->getCalc3DViewSettings();
    int str_wid = fm.width(u8"分布荷重(床)");
    if ( loadSetting.getIsDloadVisible() ) {
        cur_h = cur_h - 40;
        QString str1 = QString("%1kN/m").arg(loadSetting.getDloadMinValue());
        QString str2 = QString("%1kN/m").arg(loadSetting.getDloadMaxValue());
        glColor3f(str_col.redF(), str_col.greenF(), str_col.blueF());
        viewer->drawText(width - 110 - str_wid, cur_h + fh, u8"分布荷重(床)", font);
        viewer->drawText(width - 100, cur_h + 17 + fh, str1, font);
        int cwid = fm.width(str2);
        viewer->drawText(width - 20 - cwid, cur_h + 17 + fh, str2, font);
        QColor col1 = loadSetting.getDloadMinColor();
        QColor col2 = loadSetting.getDloadMaxColor();
        glBegin(GL_QUADS);
        glColor3d( col1.redF(), col1.greenF(), col1.blueF() );
        glVertex2i(width - 100, cur_h);
        glColor3d( col2.redF(), col2.greenF(), col2.blueF() );
        glVertex2i(width - 20, cur_h);
        glVertex2i(width - 20, cur_h + 15);
        glColor3d( col1.redF(), col1.greenF(), col1.blueF() );
        glVertex2i(width - 100, cur_h + 15);
        glEnd();
    }
    if ( loadSetting.getIsWloadVisible() ) {
        cur_h = cur_h - 40;
        QString str1 = QString("%1kN/m").arg(loadSetting.getWloadMinValue());
        QString str2 = QString("%1kN/m").arg(loadSetting.getWloadMaxValue());
        glColor3f(str_col.redF(), str_col.greenF(), str_col.blueF());
        viewer->drawText(width - 110 - str_wid, cur_h + fh, u8"分布荷重(自)", font);
        viewer->drawText(width - 100, cur_h + 17 + fh, str1, font);
        int cwid = fm.width(str2);
        viewer->drawText(width - 20 - cwid, cur_h + 17 + fh, str2, font);
        QColor col1 = loadSetting.getWloadMinColor();
        QColor col2 = loadSetting.getWloadMaxColor();
        glBegin(GL_QUADS);
        glColor3d( col1.redF(), col1.greenF(), col1.blueF() );
        glVertex2i(width - 100, cur_h);
        glColor3d( col2.redF(), col2.greenF(), col2.blueF() );
        glVertex2i(width - 20, cur_h);
        glVertex2i(width - 20, cur_h + 15);
        glColor3d( col1.redF(), col1.greenF(), col1.blueF() );
        glVertex2i(width - 100, cur_h + 15);
        glEnd();
    }
    if ( loadSetting.getIsPloadVisible() ) {
        cur_h = cur_h - 40;
        QString str1 = QString("%1kN").arg(loadSetting.getPloadMinValue());
        QString str2 = QString("%1kN").arg(loadSetting.getPloadMaxValue());
        glColor3f(str_col.redF(), str_col.greenF(), str_col.blueF());
        viewer->drawText(width - 110 - str_wid, cur_h + fh, u8"集中荷重", font);
        viewer->drawText(width - 100, cur_h + 17 + fh, str1, font);
        int cwid = fm.width(str2);
        viewer->drawText(width - 20 - cwid, cur_h + 17 + fh, str2, font);
        QColor col1 = loadSetting.getPloadMinColor();
        QColor col2 = loadSetting.getPloadMaxColor();
        glBegin(GL_QUADS);
        glColor3d( col1.redF(), col1.greenF(), col1.blueF() );
        glVertex2i(width - 100, cur_h);
        glColor3d( col2.redF(), col2.greenF(), col2.blueF() );
        glVertex2i(width - 20, cur_h);
        glVertex2i(width - 20, cur_h + 15);
        glColor3d( col1.redF(), col1.greenF(), col1.blueF() );
        glVertex2i(width - 100, cur_h + 15);
        glEnd();
    }
    if ( loadSetting.getIsJloadVisible() ) {
        cur_h = cur_h - 40;
        QString str1 = QString("%1kN").arg(loadSetting.getJloadMinValue());
        QString str2 = QString("%1kN").arg(loadSetting.getJloadMaxValue());
        glColor3f(str_col.redF(), str_col.greenF(), str_col.blueF());
        viewer->drawText(width - 110 - str_wid, cur_h + fh, u8"節点荷重", font);
        viewer->drawText(width - 100, cur_h + 17 + fh, str1, font);
        int cwid = fm.width(str2);
        viewer->drawText(width - 20 - cwid, cur_h + 17 + fh, str2, font);
        QColor col1 = loadSetting.getJloadMinColor();
        QColor col2 = loadSetting.getJloadMaxColor();
        glBegin(GL_QUADS);
        glColor3d( col1.redF(), col1.greenF(), col1.blueF() );
        glVertex2i(width - 100, cur_h);
        glColor3d( col2.redF(), col2.greenF(), col2.blueF() );
        glVertex2i(width - 20, cur_h);
        glVertex2i(width - 20, cur_h + 15);
        glColor3d( col1.redF(), col1.greenF(), col1.blueF() );
        glVertex2i(width - 100, cur_h + 15);
        glEnd();
    }

    viewer->stopScreenCoordinatesSystem();
}


void PersCalcLoadPartsDrawing::drawLineLoad( MemberData *md, const QList<JointData *> &jdlist,
                                             const QList<qreal> &lenlist,
                                             LoadType::LOADTYPE ltype, LOADSUMTYPE lstype, CALCOUTPUTDIR outdir )
{
    //    glPolygonStipple(hatch);
    qreal v_alpha = qreal(UnifiedSettings::getInstance()->getCalc3DViewSettings().getValueAlpha()) / 255.0;

    qreal tlen = md->getTotalLength(); // 単位 mm
    VEC3D v0 = VEC3D(jdlist.first()->xPos() * 1000.0,
                     jdlist.first()->yPos() * 1000.0,
                     jdlist.first()->zPos(true) * 1000.0);
    VEC3D u_v = md->getUVector();
    VEC3D v_v = md->getVVector();
    VEC3D w_v = md->getWVector();
    qreal mband = 500.0; // 部材の分割幅　50cm

    CalculationLoad3DViewSettings loadSetting = UnifiedSettings::getInstance()->getCalc3DViewSettings();
    QList<MLOADSUM *> mloads = UnifiedFixedData::getInstance()->getCalcLoadData()->loadOfMember(md);

    Q_FOREACH ( MLOADSUM *mload, mloads ) {

        if ( mload->loadType != ltype ) continue;
        if ( lstype == LOADSUMTYPE::LOADSUM_AUTO || lstype == LOADSUMTYPE::LOADSUM_SELF ) {
            if ( !mload->loadName.isEmpty() ) continue;
        } else if ( lstype == LOADSUMTYPE::LOADSUM_MANUAL ) {
            if ( mload->loadName.isEmpty() ) continue;
        }

        if ( outdir == CALCOUTPUTDIR::CALC_ODIR_YZ ) { // yz軸
            if ( mload->loadDir == LOADDIR::LDIR_X || mload->loadDir == LOADDIR::LDIR_MX ) continue;
        } else if ( outdir == CALCOUTPUTDIR::CALC_ODIR_X ) { // x軸
            if ( mload->loadDir != LOADDIR::LDIR_X && mload->loadDir != LOADDIR::LDIR_MX ) continue;
        }

        // 分散荷重　分布荷重(床)
        if ( !mload->sumUpLoad.isEmpty() && lstype != LOADSUMTYPE::LOADSUM_SELF ) {

            if ( !loadSetting.getIsDloadVisible() ) continue;
            //            glEnable(GL_POLYGON_STIPPLE);

            QList<qreal> loads = mload->sumUpLoad;
            qreal medg = ( tlen - qreal(loads.count()) * mband ) / 2.0;
            bool isYdir = ( mload->loadDir == LOADDIR::LDIR_Y || mload->loadDir == LOADDIR::LDIR_X );

            qreal dshift = 100.0; // 荷重描画シフト10cm
            qreal dratio = 1000.0 / loadSetting.getMloadSize();
            qreal cratio;
            VEC3D u1, u2, v1, v2;
            QColor col;
            qreal cur_len = 0.0;
            int pre_idx = -1;
            qreal x0 = 0.0;

            for ( int i = 0; i < loads.count(); i++ ) {

                qreal e_len = cur_len + mband;
                if ( i == 0 || i == loads.count() - 1 ) e_len += medg;
                cratio = 1000.0 / (e_len - cur_len);

                qreal p_len = cur_len + (e_len - cur_len) / 2.0;
                qreal p_load = loads.at(i);

                int idx = 0;
                for ( int j = 0; j < lenlist.count() - 1; j++ ) {
                    if ( lenlist.at(j) > p_len - 1.0e-3 ) break;
                    idx = j;
                }

                if ( idx != pre_idx ) { // 1番目あるいは1本化部材の切り替わるとき

                    if ( i != 0 ) {
                        v1 = ( isYdir ) ? VEC3D(lenlist.at(idx) - x0, -p_load * dratio * cratio + dshift, 10.0)
                                        : VEC3D(lenlist.at(idx) - x0, 10.0, -p_load * dratio * cratio + dshift);
                        v2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v1);
                        col = valueToColor( p_load * cratio,
                                            loadSetting.getDloadMinColor(), loadSetting.getDloadMinValue(),
                                            loadSetting.getDloadMaxColor(), loadSetting.getDloadMaxValue() );
                        glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );

                        glVertex3d(v2.x, v2.y, v2.z);

                        u1 = ( isYdir ) ? VEC3D(lenlist.at(idx) - x0, dshift, 10.0)
                                        : VEC3D(lenlist.at(idx) - x0, 10.0, dshift);
                        u2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u1);
                        glColor4f( loadSetting.getDloadMinColor().redF(), loadSetting.getDloadMinColor().greenF(),
                                   loadSetting.getDloadMinColor().blueF(), v_alpha );
                        glVertex3d(u2.x, u2.y, u2.z);

                        glEnd();
                        glBegin(GL_POLYGON);
                    }

                    glBegin(GL_POLYGON);

                    v0 = VEC3D(jdlist.at(idx)->xPos() * 1000.0,
                               jdlist.at(idx)->yPos() * 1000.0,
                               jdlist.at(idx)->zPos(true) * 1000.0);
                    x0 = lenlist.at(idx);
                    md->calcMidLocalVector(md->indexOfMemberJoint(jdlist.at(idx)),
                                           u_v, v_v, w_v);

                    ( isYdir ) ? glNormal3d(w_v.x, w_v.y, w_v.z) : glNormal3d(v_v.x, v_v.y, v_v.z);

                    u1 = ( isYdir ) ? VEC3D(0.0, dshift, 10.0) : VEC3D(0.0, 10.0, dshift);
                    u2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u1);
                    glColor4f( loadSetting.getDloadMinColor().redF(), loadSetting.getDloadMinColor().greenF(),
                               loadSetting.getDloadMinColor().blueF(), v_alpha );
                    glVertex3d(u2.x, u2.y, u2.z);

                    v1 = ( isYdir ) ? VEC3D(0.0, -p_load * dratio * cratio + dshift, 10.0)
                                    : VEC3D(0.0, 10.0, -p_load * dratio * cratio + dshift);
                    v2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v1);
                    col = valueToColor( p_load * cratio,
                                        loadSetting.getDloadMinColor(), loadSetting.getDloadMinValue(),
                                        loadSetting.getDloadMaxColor(), loadSetting.getDloadMaxValue() );
                    glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );

                    glVertex3d(v2.x, v2.y, v2.z);

                    pre_idx = idx;

                }

                v1 = ( isYdir ) ? VEC3D(p_len - x0, -p_load * dratio * cratio + dshift, 10.0)
                                : VEC3D(p_len - x0, 10.0, -p_load * dratio * cratio + dshift);
                v2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v1);
                col = valueToColor( p_load * cratio,
                                    loadSetting.getDloadMinColor(), loadSetting.getDloadMinValue(),
                                    loadSetting.getDloadMaxColor(), loadSetting.getDloadMaxValue() );
                glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );
                glVertex3d(v2.x, v2.y, v2.z);

                u1 = ( isYdir ) ? VEC3D(p_len - x0, dshift, 10.0) : VEC3D(p_len - x0, 10.0, dshift);
                u2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u1);
                glColor4f( loadSetting.getDloadMinColor().redF(), loadSetting.getDloadMinColor().greenF(),
                           loadSetting.getDloadMinColor().blueF(), v_alpha );
                glVertex3d(u2.x, u2.y, u2.z);

                glEnd();
                glBegin(GL_POLYGON);

                glColor4f( loadSetting.getDloadMinColor().redF(), loadSetting.getDloadMinColor().greenF(),
                           loadSetting.getDloadMinColor().blueF(), v_alpha );
                glVertex3d( u2.x, u2.y, u2.z );
                col = valueToColor( p_load * cratio,
                                    loadSetting.getDloadMinColor(), loadSetting.getDloadMinValue(),
                                    loadSetting.getDloadMaxColor(), loadSetting.getDloadMaxValue() );
                glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );
                glVertex3d( v2.x, v2.y, v2.z );

                cur_len = e_len;
            }

            qreal p_load = loads.last();
            v1 = ( isYdir ) ? VEC3D(tlen - x0, -p_load * dratio * cratio + dshift, 10.0)
                            : VEC3D(tlen - x0, 10.0, -p_load * dratio * cratio + dshift);
            v2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v1);
            col = valueToColor( p_load * cratio,
                                loadSetting.getDloadMinColor(), loadSetting.getDloadMinValue(),
                                loadSetting.getDloadMaxColor(), loadSetting.getDloadMaxValue() );
            glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );
            glVertex3d(v2.x, v2.y, v2.z);

            u1 = ( isYdir ) ? VEC3D(tlen - x0, dshift, 10.0)
                            : VEC3D(tlen - x0, 10.0, dshift);
            u2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u1);
            glColor4f( loadSetting.getDloadMinColor().redF(), loadSetting.getDloadMinColor().greenF(),
                       loadSetting.getDloadMinColor().blueF(), v_alpha );
            glVertex3d(u2.x, u2.y, u2.z);

            glEnd();

            u_v = md->getUVector();
            v_v = md->getVVector();
            w_v = md->getWVector();
        }

        // 分布荷重(自)
        if ( ( qAbs(mload->loadWAB.loadWA.y()) > 1.0e-3
               || qAbs(mload->loadWAB.loadWB.y()) > 1.0e-3 ) && lstype != LOADSUMTYPE::LOADSUM_AUTO ) {

            if ( !loadSetting.getIsWloadVisible() ) continue;
            //            glEnable(GL_POLYGON_STIPPLE);

            WABLOAD wload = mload->loadWAB;
            bool isYdir = ( mload->loadDir == LOADDIR::LDIR_Y || mload->loadDir == LOADDIR::LDIR_X );

            int idx_a = 0, idx_b = 0;
            for ( int i = 0; i < lenlist.count() - 1; i++ ) {
                if ( wload.loadWA.x() * 1000.0 > lenlist.at(i) - 1.0e-3 ) idx_a = i;
                if ( wload.loadWB.x() * 1000.0 > lenlist.at(i) - 1.0e-3 ) idx_b = i;
            }

            qreal dshift = 100.0; // 荷重描画シフト10cm
            qreal dratio = 1000.0 / loadSetting.getMloadSize(); // 仮
            VEC3D v1, v2;

            v0 = VEC3D(jdlist.at(idx_a)->xPos() * 1000.0,
                       jdlist.at(idx_a)->yPos() * 1000.0,
                       jdlist.at(idx_a)->zPos(true) * 1000.0);
            qreal x0 = lenlist.at(idx_a);
            md->calcMidLocalVector( md->indexOfMemberJoint(jdlist.at(idx_a)), u_v, v_v, w_v );

            glBegin(GL_POLYGON);

            qreal p_len = wload.loadWA.x() * 1000.0 - x0;
            qreal p_load = wload.loadWA.y();

            v1 = ( isYdir ) ? VEC3D(p_len, -p_load * dratio + dshift, -10.0)
                            : VEC3D(p_len, -10.0, -p_load * dratio + dshift);
            v1 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v1);
            QColor col = valueToColor( p_load,
                                       loadSetting.getWloadMinColor(), loadSetting.getWloadMinValue(),
                                       loadSetting.getWloadMaxColor(), loadSetting.getWloadMaxValue() );
            glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );
            glVertex3d(v1.x, v1.y, v1.z);

            v2 = ( isYdir ) ? VEC3D(p_len, dshift, -10.0) : VEC3D(p_len, -10.0, dshift);
            v2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v2);
            glColor4f( loadSetting.getWloadMinColor().redF(), loadSetting.getWloadMinColor().greenF(),
                       loadSetting.getWloadMinColor().blueF(), v_alpha );
            glVertex3d(v2.x, v2.y, v2.z);


            for ( int i = idx_a + 1; i <= idx_b; i++ ) {

                p_len = lenlist.at(i) - x0;
                p_load = wload.loadWA.y() + lenlist.at(i) / 1000.0 *
                        (wload.loadWB.y() - wload.loadWA.y()) / (wload.loadWB.x() - wload.loadWA.x());

                v1 = ( isYdir ) ? VEC3D(p_len, dshift, -10.0) : VEC3D(p_len, -10.0, dshift);
                v1 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v1);
                glColor4f( loadSetting.getWloadMinColor().redF(), loadSetting.getWloadMinColor().greenF(),
                           loadSetting.getWloadMinColor().blueF(), v_alpha );
                glVertex3d(v1.x, v1.y, v1.z);

                v2 = ( isYdir ) ? VEC3D(p_len, -p_load * dratio + dshift, -10.0)
                                : VEC3D(p_len, -10.0, -p_load * dratio + dshift);
                v2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v2);
                col = valueToColor( p_load,
                                    loadSetting.getWloadMinColor(), loadSetting.getWloadMinValue(),
                                    loadSetting.getWloadMaxColor(), loadSetting.getWloadMaxValue() );
                glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );
                glVertex3d(v2.x, v2.y, v2.z);
                glEnd();

                if ( qAbs(wload.loadWB.x() * 1000.0 - lenlist.at(i)) < 1.0e-3 ) break;

                v0 = VEC3D(jdlist.at(i)->xPos() * 1000.0,
                           jdlist.at(i)->yPos() * 1000.0,
                           jdlist.at(i)->zPos(true) * 1000.0);
                x0 = lenlist.at(i);
                md->calcMidLocalVector( md->indexOfMemberJoint(jdlist.at(i)), u_v, v_v, w_v );

                glBegin(GL_POLYGON);

                v1 = ( isYdir ) ? VEC3D(0.0, -p_load * dratio + dshift, -10.0)
                                : VEC3D(0.0, -10.0, -p_load * dratio + dshift);
                v1 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v1);
                col = valueToColor( p_load,
                                    loadSetting.getWloadMinColor(), loadSetting.getWloadMinValue(),
                                    loadSetting.getWloadMaxColor(), loadSetting.getWloadMaxValue() );
                glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );
                glVertex3d(v1.x, v1.y, v1.z);

                v2 = ( isYdir ) ? VEC3D(0.0, dshift, -10.0) : VEC3D(0.0, -10.0, dshift);
                v2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v2);
                glColor4f( loadSetting.getWloadMinColor().redF(), loadSetting.getWloadMinColor().greenF(),
                           loadSetting.getWloadMinColor().blueF(), v_alpha );
                glVertex3d(v2.x, v2.y, v2.z);

            }

            if ( qAbs(wload.loadWB.x() * 1000.0 - lenlist.at(idx_b)) > 1.0e-3 ) {

                p_len = wload.loadWB.x() * 1000.0 - x0;
                p_load = wload.loadWB.y();

                v1 = ( isYdir ) ? VEC3D(p_len, dshift, -10.0) : VEC3D(p_len, -10.0, dshift);
                v1 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v1);
                glColor4f( loadSetting.getWloadMinColor().redF(), loadSetting.getWloadMinColor().greenF(),
                           loadSetting.getWloadMinColor().blueF(), v_alpha );
                glVertex3d(v1.x, v1.y, v1.z);

                v2 = ( isYdir ) ? VEC3D(p_len, -p_load * dratio + dshift, -10.0)
                                : VEC3D(p_len, -10.0, -p_load * dratio + dshift);
                v2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, v2);
                col = valueToColor( p_load,
                                    loadSetting.getWloadMinColor(), loadSetting.getWloadMinValue(),
                                    loadSetting.getWloadMaxColor(), loadSetting.getWloadMaxValue() );
                glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );
                glVertex3d(v2.x, v2.y, v2.z);
                glEnd();

            }

            u_v = md->getUVector();
            v_v = md->getVVector();
            w_v = md->getWVector();

        }


        // 集中荷重
        if ( !mload->concentratedLoad.isEmpty() && lstype != LOADSUMTYPE::LOADSUM_SELF ) {

            if ( !loadSetting.getIsPloadVisible() ) continue;
            //            glDisable(GL_POLYGON_STIPPLE);

            QList<QPointF> loads = mload->concentratedLoad;
            VEC3D u1, u2, u3, u4, u5;
            QColor col;
            qreal dsize = qreal(loadSetting.getJloadSize()) * 100.0 + 200.0;
            qreal rsize = dsize / 2.0;

            for ( int i = 0; i < loads.count(); i++ ) {

                qreal cur_len = loads.at(i).x() * 1000.0;
                qreal cur_load = loads.at(i).y();

                int idx = 0;
                for ( int j = 0; j < lenlist.count() - 1; j++ ) {
                    if ( lenlist.at(j) > cur_len - 1.0e-3 ) break;
                    idx = j;
                }
                v0 = VEC3D(jdlist.at(idx)->xPos() * 1000.0,
                           jdlist.at(idx)->yPos() * 1000.0,
                           jdlist.at(idx)->zPos(true) * 1000.0);
                qreal p_len = cur_len - lenlist.at(idx);
                md->calcMidLocalVector(md->indexOfMemberJoint(jdlist.at(idx)), u_v, v_v, w_v);

                col = valueToColor( cur_load, loadSetting.getPloadMinColor(), loadSetting.getPloadMinValue(),
                                    loadSetting.getPloadMaxColor(), loadSetting.getPloadMaxValue() );
                glColor4f( col.redF(), col.greenF(), col.blueF(), v_alpha );

                if ( mload->loadDir == LOADDIR::LDIR_X || mload->loadDir == LOADDIR::LDIR_MX ) {
                    glNormal3d(u_v.x, u_v.y, u_v.z);
                    u1 = VEC3D(p_len, 0.0, 0.0);
                    u1 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u1);
                    u2 = ( cur_load > 0.0 ) ? VEC3D(p_len - dsize, -rsize, -rsize) : VEC3D(p_len + dsize, -rsize,
                                                                                           -rsize) ;
                    u2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u2);
                    u3 = ( cur_load > 0.0 ) ? VEC3D(p_len - dsize, rsize, -rsize) : VEC3D(p_len + dsize, rsize,
                                                                                          -rsize) ;
                    u3 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u3);
                    u4 = ( cur_load > 0.0 ) ? VEC3D(p_len - dsize, -rsize, rsize) : VEC3D(p_len + dsize, -rsize,
                                                                                          rsize) ;
                    u4 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u4);
                    u5 = ( cur_load > 0.0 ) ? VEC3D(p_len - dsize, rsize, rsize) : VEC3D(p_len + dsize, rsize, rsize) ;
                    u5 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u5);
                } else if ( mload->loadDir == LOADDIR::LDIR_Y || mload->loadDir == LOADDIR::LDIR_MY ) {
                    glNormal3d(v_v.x, v_v.y, v_v.z);
                    u1 = VEC3D(p_len, 0.0, 0.0);
                    u1 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u1);
                    u2 = ( cur_load > 0.0 ) ? VEC3D(p_len - rsize, -dsize, -rsize) : VEC3D(p_len - rsize, dsize,
                                                                                           -rsize) ;
                    u2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u2);
                    u3 = ( cur_load > 0.0 ) ? VEC3D(p_len - rsize, -dsize, rsize) : VEC3D(p_len - rsize, dsize, rsize) ;
                    u3 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u3);
                    u4 = ( cur_load > 0.0 ) ? VEC3D(p_len + rsize, -dsize, -rsize) : VEC3D(p_len + rsize, dsize,
                                                                                           -rsize) ;
                    u4 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u4);
                    u5 = ( cur_load > 0.0 ) ? VEC3D(p_len + rsize, -dsize, rsize) : VEC3D(p_len + rsize, dsize, rsize) ;
                    u5 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u5);
                } else {
                    glNormal3d(w_v.x, w_v.y, w_v.z);
                    u1 = VEC3D(p_len, 0.0, 0.0);
                    u1 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u1);
                    u2 = ( cur_load > 0.0 ) ? VEC3D(p_len - rsize, -rsize, -dsize) : VEC3D(p_len - rsize, -rsize,
                                                                                           dsize) ;
                    u2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u2);
                    u3 = ( cur_load > 0.0 ) ? VEC3D(p_len - rsize, rsize, -dsize) : VEC3D(p_len - rsize, rsize, dsize) ;
                    u3 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u3);
                    u4 = ( cur_load > 0.0 ) ? VEC3D(p_len + rsize, -rsize, -dsize) : VEC3D(p_len + rsize, -rsize,
                                                                                           dsize) ;
                    u4 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u4);
                    u5 = ( cur_load > 0.0 ) ? VEC3D(p_len + rsize, rsize, -dsize) : VEC3D(p_len + rsize, rsize, dsize) ;
                    u5 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v0, u5);
                }

                if ( mload->loadDir == LOADDIR::LDIR_X || mload->loadDir == LOADDIR::LDIR_Y || mload->loadDir == LOADDIR::LDIR_Z ) {
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex3d(u1.x, u1.y, u1.z);
                    glVertex3d(u2.x, u2.y, u2.z);
                    glVertex3d(u3.x, u3.y, u3.z);
                    glVertex3d(u5.x, u5.y, u5.z);
                    glVertex3d(u4.x, u4.y, u4.z);
                    glEnd();
                    glBegin(GL_POLYGON);
                    glVertex3d(u2.x, u2.y, u2.z);
                    glVertex3d(u3.x, u3.y, u3.z);
                    glVertex3d(u5.x, u5.y, u5.z);
                    glVertex3d(u4.x, u4.y, u4.z);
                    glEnd();
                    col = loadSetting.getPloadMinColor();
                    glColor3d( col.redF(), col.greenF(), col.blueF() );
                    glBegin(GL_LINES);
                    glVertex3d(u1.x, u1.y, u1.z);
                    glVertex3d(u2.x, u2.y, u2.z);
                    glVertex3d(u1.x, u1.y, u1.z);
                    glVertex3d(u3.x, u3.y, u3.z);
                    glVertex3d(u1.x, u1.y, u1.z);
                    glVertex3d(u4.x, u4.y, u4.z);
                    glVertex3d(u1.x, u1.y, u1.z);
                    glVertex3d(u5.x, u5.y, u5.z);
                    glVertex3d(u2.x, u2.y, u2.z);
                    glVertex3d(u3.x, u3.y, u3.z);
                    glVertex3d(u3.x, u3.y, u3.z);
                    glVertex3d(u5.x, u5.y, u5.z);
                    glVertex3d(u5.x, u5.y, u5.z);
                    glVertex3d(u4.x, u4.y, u4.z);
                    glVertex3d(u4.x, u4.y, u4.z);
                    glVertex3d(u2.x, u2.y, u2.z);
                    glEnd();
                } else {
                    glBegin(GL_TRIANGLES);
                    glVertex3d(u1.x, u1.y, u1.z);
                    glVertex3d(u2.x, u2.y, u2.z);
                    glVertex3d(u5.x, u5.y, u5.z);
                    glVertex3d(u1.x, u1.y, u1.z);
                    glVertex3d(u3.x, u3.y, u3.z);
                    glVertex3d(u4.x, u4.y, u4.z);
                    glEnd();
                    col = loadSetting.getPloadMinColor();
                    glColor3d( col.redF(), col.greenF(), col.blueF() );
                    glBegin(GL_LINE_LOOP);
                    glVertex3d(u1.x, u1.y, u1.z);
                    glVertex3d(u2.x, u2.y, u2.z);
                    glVertex3d(u5.x, u5.y, u5.z);
                    glEnd();
                    glBegin(GL_LINE_LOOP);
                    glVertex3d(u1.x, u1.y, u1.z);
                    glVertex3d(u3.x, u3.y, u3.z);
                    glVertex3d(u4.x, u4.y, u4.z);
                    glEnd();
                }
            }
        }
    }

    //    glDisable(GL_POLYGON_STIPPLE);
}



QColor PersCalcLoadPartsDrawing::valueToColor( qreal var, const QColor &minCol, qreal minVar,
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

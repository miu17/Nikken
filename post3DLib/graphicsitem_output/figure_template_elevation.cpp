#include "figure_template_elevation.h"

#include "calc_vector2d.h"
#include "calc_vector3d.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "output_graphicsitem.h"
#include "output_grscene.h"
#include "unified_analysisdata.h"

namespace post3dapp{

FigureCommonTemplateElevation::FigureCommonTemplateElevation(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : FigureCommonTemplate(_setting, type, uuid)
{

}
void FigureCommonTemplateElevation::createJointItems(OutputGrScene* scene){
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();

    FrameType::FRAMETYPE f_type = uad->getFrameTypeOfFloor(uuid);
    bool isClosedFrame = ( f_type == FrameType::FRAMETYPE::CLCIRCLE || f_type == FrameType::FRAMETYPE::CLVARIABLE ) ;

    // TODO
    int cnt = 0;
    Q_FOREACH ( JointData *jd, uad->jointListOfFrame(uuid) ) {

        if ( !jd->isAnalysisJoint(*uad) ){
            qDebug()<<"not analysis joint! createdJoint="<<cnt<<" / "<<uad->jointListOfFrame(uuid).count();
            continue;
        }

        int ino = uad->indexOfJoint(jd) + 1;
        QPointF pp = jointToPoint(jd);

        createJointItem(scene, jd, ino, pp);
//        qDebug()<<"createdJoint="<<cnt<<" / "<<uad->jointListOfFrame(uuid).count();
        cnt++;

        if ( isClosedFrame && qAbs(pp.x()) < 1.0e-3 ) { // 閉じた通り
            qreal cx = uad->globalZToClosedFrameEndX(uuid, jd->zPos());
            OutputGraphicsItem *item = createOffJointItem(*jd, QPointF(cx * 1000.0, pp.y()));
            scene->addItem(item);
        }
    }
}
void FigureCommonTemplateElevation::createMemberItems(OutputGrScene* scene){
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();

    FrameType::FRAMETYPE f_type = uad->getFrameTypeOfFloor(uuid);
    bool isClosedFrame = ( f_type == FrameType::FRAMETYPE::CLCIRCLE || f_type == FrameType::FRAMETYPE::CLVARIABLE ) ;

//    qDebug() << "createMembers";
    Q_FOREACH ( MemberData *md, uad->memberListOfFrame(uuid) ) {
//        qDebug() << "createMember" << md->getMemberValues().sectionName;

        if ( !md->isAnalysisMember(*uad) ) continue;

        QList<JointData *> jdlist = md->getAnalysisJointList(*uad);

        // 線材の場合
        if ( md->isLineMember(false) ) {

            QList<QList<QPointF> > hlinePoints;

            bool prev_on;
            bool cur_on = false;
            bool next_on = jdlist.first()->containsFrame(uuid);
//            bool next_on = jdlist.first()->containsFloor(uuid);

            for (int i = 0; i < jdlist.count() - 1; i++) {

                JointData *jd0 = jdlist.at(i);
                JointData *jd1 = jdlist.at(i + 1);

                QPointF p0 = jointToPoint(jd0);
                QPointF p1 = jointToPoint(jd1);

                bool isIc = ( isClosedFrame && qAbs(p0.x()) < 1.0e-3 );
                bool isJc = ( isClosedFrame && qAbs(p1.x()) < 1.0e-3 );

                prev_on = cur_on;
                cur_on = next_on;
                next_on = jd1->containsFrame(uuid);
//                next_on = jd1->containsFloor(uuid);

                if ( cur_on ) {
                    if ( next_on ) {
                        if ( prev_on ) {

                            QList<QPointF> poly = hlinePoints.last();
                            QPointF cpos = ( isJc ) ? jointToPoint(jd1, poly.last().x()) : p1 ;
                            QPointF q1 = poly.at( poly.count() - 2 );
                            QPointF q2 = poly.last();
                            q1 = q2 - q1;
                            q2 = p1 - q2;
                            q1 = q1 / CalcVector2D::length(q1);
                            q2 = q2 / CalcVector2D::length(q2);
                            if ( CalcVector2D::dot(q1, q2) > 0.99985 ) {
                                poly.replace(poly.count() - 1, cpos);
                            } else {
                                poly.append(cpos);
                            }
                            hlinePoints.replace(hlinePoints.count() - 1, poly);

                        } else {

                            QList<QPointF> poly;
                            QPointF c1pos, c2pos;
                            if ( isIc && !isJc ) {
                                c2pos = p1;
                                c1pos = jointToPoint(jd0, c2pos.x());
                            } else if ( !isIc && isJc ) {
                                c1pos = p0;
                                c2pos = jointToPoint(jd1, c1pos.x());
                            } else {
                                c1pos = p0;
                                c2pos = p1;
                            }
                            poly.append( c1pos );
                            poly.append( c2pos );
                            hlinePoints.append(poly);

                        }
                    }
                }
            }



            if ( hlinePoints.isEmpty() ) continue;

            for ( int i = 0; i < hlinePoints.count(); i++ ) {
                createLineItem(scene, md, hlinePoints.at(i));
            }

        }


        // 面材の場合
        if ( md->isClosedMember(false) ) {

            bool isOnPlane = false;
            XYZ a1 = jdlist.first()->xyz();
            VEC3D vv(0.0, 0.0, 0.0);
            for ( int i = 1; i < jdlist.count() - 1; i++ ) {
                XYZ a2 = jdlist.at(i)->xyz();
                XYZ a3 = jdlist.at(i + 1)->xyz();
                vv = CalcVector3D::cross(VEC3D(a2.x - a1.x, a2.y - a1.y, a2.z - a1.z),
                                         VEC3D(a3.x - a1.x, a3.y - a1.y, a3.z - a1.z));
                if ( CalcVector3D::norm(vv) > 1.0e-3 ) {
                    isOnPlane = true;
                    break;
                }
            }

            QList<QList<QPointF> > planePoints;
            QList<int> planeIndexes;
            bool prev_on;
            bool cur_on = false;


            // 閉じた通りの場合、基準となる点をサーチ
            QPointF cmpPoint;
            bool isCmpDetect = false; // 通り起点でない点が見つかった
            if ( isClosedFrame ) {
                Q_FOREACH ( JointData *jd, jdlist ) {
                    QPointF pp = jointToPoint(jd);
                    if ( jd->containsFrame(uuid) && qAbs(pp.x()) > 1.0e-3 ) {
                        cmpPoint = jointToPoint(jd);
                        isCmpDetect = true;
                        break;
                    }
                }
            }

            for (int i = 0; i < jdlist.count(); i++) {

                JointData *jd = jdlist.at(i);
                QPointF pp = jointToPoint(jd);
                bool isIc = ( isClosedFrame && qAbs(pp.x()) < 1.0e-3 ); // 閉じた通りの判定
                prev_on = cur_on;
                cur_on = jd->containsFrame(uuid);

                if ( cur_on ) {
                    if ( prev_on ) {
                        QList<QPointF> poly = planePoints.last();
                        QPointF cpos = ( isIc ) ? jointToPoint(jd, poly.last().x()) : pp;
                        poly.append(cpos);
                        planePoints.replace(planePoints.count() - 1, poly);
                    } else {
                        QList<QPointF> poly;
                        QPointF cpos = ( isIc && isCmpDetect ) ? jointToPoint(jd, cmpPoint.x()) : pp;
                        poly.append(cpos);
                        planePoints.append(poly);
                        planeIndexes.append(i);
                    }
                }

                if ( !isOnPlane || i < 3 ) continue;
                XYZ a4 = jd->xyz();
                qreal dot = CalcVector3D::dot(VEC3D(a4.x - a1.x, a4.y - a1.y, a4.z - a1.z), vv);
                isOnPlane = ( qAbs(dot) < 1.0e-3 );
            }

            // 始点と終点がONで,Polygonが2つ以上存在する場合,最初と最後のPolygonを結合する
            if ( cur_on && planePoints.count() > 1 ) {
                if ( jdlist.first()->containsFrame(uuid) ) {
                    QList<QPointF> poly = planePoints.last();
                    poly.append( planePoints.first() );
                    planePoints.replace(planePoints.count() - 1, poly);
                    planePoints.removeFirst();
                    planeIndexes.removeFirst();
                }
            }

            QList<QList<QPointF> > hplanePoints, vplanePoints;
            QList<QPointF> crossPoints;

            for ( int i = 0; i < planePoints.count(); i++ ) {

                QList<QPointF> points = planePoints.at(i);

                if ( points.count() == 1 ) {
                    int insert_idx = 0;
                    for ( int j = 0; j < crossPoints.count(); j++ ) {
                        if ( points.first().x() > crossPoints.at(j).x() ) insert_idx = j + 1;
                    }
                    crossPoints.insert( insert_idx, points.first() );
                    continue;
                }

                bool isOnLine = true;
                if ( points.count() > 2 ) {
                    for ( int j = 0; j < points.count() - 2; j++ ) {
                        if ( CalcVector2D::sideToSegment(points.at(j), points.at(j + 1), points.at(j + 2)) != 0 ) {
                            isOnLine = false;
                            break;
                        }
                    }
                }

                if ( isOnLine ) {

                    int s_idx = planeIndexes.at(i);
                    int e_idx = s_idx + points.count() - 1;
                    if ( e_idx > jdlist.count() - 1 ) e_idx = e_idx - jdlist.count();
                    JointData *pj = md->previousJoint(jdlist.at(s_idx));
                    JointData *nj = md->nextJoint(jdlist.at(e_idx));
                    XYZ p_xyz = uad->globalXYZToFrameXYZ(uuid, pj->xyz());
                    XYZ n_xyz = uad->globalXYZToFrameXYZ(uuid, nj->xyz());

                    if ( p_xyz.z * n_xyz.z < 0.0 ) {
                        int insert_idx = 0;
                        for ( int j = 0; j < crossPoints.count(); j++ ) {
                            if ( points.first().x() > crossPoints.at(j).x() ) insert_idx = i;
                        }
                        if ( p_xyz.z > 0.0 ) {
                            crossPoints.insert( insert_idx, points.first() );
                        } else {
                            crossPoints.insert( insert_idx, points.last() );
                        }
                    } else if ( p_xyz.z > 0.0 && n_xyz.z > 0.0 ) {
                        vplanePoints.append( points );
                    }

                } else {
                    hplanePoints.append(points);
                }

            }

            QList<QPointF> p_crossPoints = crossPoints;

            // 交差点　奇数の場合は無視
            if ( crossPoints.count() > 1 && crossPoints.count() % 2 == 0 ) {

                for ( int i = vplanePoints.count() - 1; i >= 0 ; i-- ) {

                    QList<QPointF> points = vplanePoints.at(i);
                    qreal xx = points.first().x();

                    int idx = 0;
                    for ( int j = 0; j < crossPoints.count(); j++ ) {
                        if ( xx > crossPoints.at(j).x() ) idx = j + 1;
                    }

                    if ( idx > 0 && idx < crossPoints.count() && idx % 2 == 1 ) {
                        int insert_idx = 0;
                        for ( int j = 0; j < p_crossPoints.count(); j++ ) {
                            if ( xx > p_crossPoints.at(j).x() ) insert_idx = j + 1;
                        }

                        if ( points.first().x() < points.last().x() ) {
                            p_crossPoints.insert( insert_idx, points.last() );
                            p_crossPoints.insert( insert_idx, points.first() );
                        } else {
                            p_crossPoints.insert( insert_idx, points.first() );
                            p_crossPoints.insert( insert_idx, points.last() );
                        }
                        vplanePoints.removeAt(i);
                    }

                }
            }

            for (int i = 0; i < p_crossPoints.count(); i++) {
                if ( i % 2 == 1 ) {
                    QList<QPointF> plist;
                    plist.append(p_crossPoints.at(i - 1));
                    plist.append(p_crossPoints.at(i));
                    vplanePoints.append( plist );
                }
            }

            for ( int i = 0; i < hplanePoints.count(); i++ ) {
                qDebug()<<"call hplane";
                createPlaneItem(scene, md, hplanePoints.at(i), false);
            }

            for ( int i = 0; i < vplanePoints.count(); i++ ) {
                qDebug()<<"call vplane";
                createPlaneItem(scene, md, vplanePoints.at(i), true);
            }

        }


    }
}

}

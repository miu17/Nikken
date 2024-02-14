#include "arealinecondition.h"

#include <QtMath>

#include "calc_vector2d.h"
#include "define_unifieddata.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "textdataunit.h"
#include "unified_inputdata.h"

namespace post3dapp {
AbstratAreaLineCondition::AbstratAreaLineCondition(
        const QString& _sign,
        ANALYSISSTEP step,
        const QList<LINESTATUSTYPE>& _premiseTypes,
        const QList<MEMBERCATEGORY>& _mlist)
    :LineCondition(QString{}, step, LINESTATUSTYPE::SKIP), sign(_sign), memberCategory(_mlist){
    premiseTypes = _premiseTypes;
    secondKeywordIndex = -1;
}

BulkAreaLineCondition::BulkAreaLineCondition(
        const QString& _sign,
        ANALYSISSTEP step,
        const QList<LINESTATUSTYPE>& _types,
        const QList<MEMBERCATEGORY>& _mlist)
    :AbstratAreaLineCondition(_sign, step, _types, _mlist){
    key = sign + "BULK";
    dataUnits = QList<TextDataUnit>{
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
            TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 12, false, "", "", QList<QString>{"MC", "MG", "MV", "WW", "WS", "EI", "DP"}},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 13, 20},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80}
        };
}

GridAreaLineCondition::GridAreaLineCondition(
        const QString& _sign,
        ANALYSISSTEP step,
        const QList<LINESTATUSTYPE>& _types,
        const QList<MEMBERCATEGORY>& _mlist)
    :AbstratAreaLineCondition(_sign, step, _types, _mlist){
    key = sign + "GRID";
    dataUnits = QList<TextDataUnit>{
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
            TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 12, false, "", "", QList<QString>{"MC", "MG", "MV", "WW", "WS", "EI", "DP"}},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 13, 20},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80}
        };
}

FloorAreaLineCondition::FloorAreaLineCondition(
        const QString& _sign,
        ANALYSISSTEP step,
        const QList<LINESTATUSTYPE>& _types,
        const QList<MEMBERCATEGORY>& _mlist)
    :AbstratAreaLineCondition(_sign, step, _types, _mlist){
    key = sign + "FLOOR";
    dataUnits = QList<TextDataUnit>{
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 6},
            TextDataUnit{TEXTTYPE::TYPE_BLANK, 7, 10},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 12, false, "", "", QList<QString>{"MC", "MG", "MV", "WW", "WS", "EI", "DP"}},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 13, 20},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80}
        };
}

NodeAreaCondition::NodeAreaCondition(
        const QString& _sign,
        ANALYSISSTEP step,
        const QList<LINESTATUSTYPE>& _types,
        const QList<MEMBERCATEGORY>& _mlist)
    :AbstratAreaLineCondition(_sign, step, _types, _mlist){
    key = sign + "NODE";
    dataUnits = QList<TextDataUnit>{
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
            TextDataUnit{TEXTTYPE::TYPE_INT, 6, 10, true},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 12, false, "", "", QList<QString>{"MC", "MG", "MV", "WW", "WS", "EI", "DP"}},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 13, 20},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, true},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, true},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, true},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, true},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 81, 90, true},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 100, true},
        };
}

CloseAreaCondition::CloseAreaCondition(
        const QString& _sign,
        ANALYSISSTEP step,
        const QList<LINESTATUSTYPE>& _types,
        const QList<MEMBERCATEGORY>& _mlist)
    :AbstratAreaLineCondition(_sign, step, _types, _mlist){
    key = sign + "CLOSE";
    dataUnits = QList<TextDataUnit>{
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 6},
            TextDataUnit{TEXTTYPE::TYPE_BLANK, 7, 10},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 12, false, "", "", QList<QString>{"MC", "MG", "MV", "WW", "WS", "EI", "DP"}},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 13, 20},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, true}
        };
}
CoordinateAreaCondition::CoordinateAreaCondition(
        const QString& _sign,
        ANALYSISSTEP step,
        const QList<LINESTATUSTYPE>& _types,
        const QList<MEMBERCATEGORY>& _mlist)
    :AbstratAreaLineCondition(_sign, step, _types, _mlist){
    key = sign + "COOR";
    dataUnits = QList<TextDataUnit>{
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
            TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 12, false, "", "", QList<QString>{"MC", "MG", "MV", "WW", "WS", "EI", "DP"}},
            TextDataUnit{TEXTTYPE::TYPE_TEXT, 13, 20},
            TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
            TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
            TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
            TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
            TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
            TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true}
        };
}

QString AbstratAreaLineCondition::convert(const QString& txt, QSet<QString>* jointSet, QSet<QString>* memberSet, QString* errorMessage) const{
    QSet<QString> joints;
    QSet<QString> members;
    if (memberCategory.contains(MEMBERCATEGORY::JOINT)){
        QString mc = txt.mid(10, 2).trimmed();
        if (mc != QString{})
            return u8"Joint指定なのに別カテゴリを指定している。";

        joints = getJoints(txt, errorMessage);
            if (errorMessage != QString())
                return QString();
    }else{
        members = getMembers(txt, errorMessage);
        if (errorMessage != QString())
            return QString();
    }

    if (sign == "+"){
        *jointSet += joints;
        *memberSet += members;
    }else{
        *jointSet -= joints;
        *memberSet -= members;
    }
    return QString();
}

FLOORVALUES AbstratAreaLineCondition::getFloor(const QString& fl, QString* errorMessage) const{
    const QList<FLOORVALUES> allFloors = UnifiedFixedData::getInstance()->getInputData()->getFloorList();
    Q_FOREACH(auto floor, allFloors){
        if (floor.floorName == fl){
            return floor;
        }
    }
    *errorMessage = "floor name";
    return FLOORVALUES();
}


QList<FLOORVALUES> AbstratAreaLineCondition::getFloors(const QString& floor1, const QString& floor2, QString* errorMessage) const{
    FLOORVALUES f1, f2;

    if (floor1 != QString()){
        f1 = getFloor(floor1, errorMessage);
        if (errorMessage != QString())
            return QList<FLOORVALUES>{};
    }

    if (floor2 != QString()){
        f2 = getFloor(floor2, errorMessage);
        if (errorMessage != QString())
            return QList<FLOORVALUES>{};
    }

    if (floor1 == QString() && floor2 == QString())
        return QList<FLOORVALUES>{};
    else if (floor1 == QString()){
        return QList<FLOORVALUES>{f2};
    }else if (floor2 == QString()){
        return QList<FLOORVALUES>{f1};
    }else if (floor1 == floor2){
        return QList<FLOORVALUES>{f1};
    }else{
        if (f1.floorHeight > f2.floorHeight){
            FLOORVALUES temp = f1;
            f1 = f2;
            f2 = temp;
        }

        QList<FLOORVALUES> fv;
        bool isWithin = false;
        const QList<FLOORVALUES> allFloors = UnifiedFixedData::getInstance()->getInputData()->getFloorList();
        for (int i = 0; i < allFloors.count(); i++){
            if (f1.UUID == allFloors.at(i).UUID){
                isWithin = true;
            }
            if (isWithin)
                fv.append(allFloors.at(i));

            if (f2.UUID == allFloors.at(i).UUID)
                return fv;
        }
    }

    *errorMessage = "error";
    return QList<FLOORVALUES>{};
}


FRAMEPOINTSVALUE AbstratAreaLineCondition::getFrame(const QString& grid, QString* errorMessage) const{
    const QList<FRAMEPOINTSVALUE> fplist = UnifiedFixedData::getInstance()->getInputData()->getFramePointList();
    Q_FOREACH(auto fp, fplist){
        if (fp.frameName == grid){
            return fp;
        }
    }
    *errorMessage = "grid mismatch";
    return FRAMEPOINTSVALUE();
}

QList<FRAMEPOINTSVALUE> AbstratAreaLineCondition::getFrames(const QString& grid1, const QString& grid2, QString* errorMessage) const{
    FRAMEPOINTSVALUE fp1, fp2;
    if (grid1 != QString()){
        fp1 = getFrame(grid1, errorMessage);
        if (errorMessage != QString())
            return QList<FRAMEPOINTSVALUE>{};
    }
    if (grid2 != QString()){
        fp2 = getFrame(grid2, errorMessage);
        if (errorMessage != QString())
            return QList<FRAMEPOINTSVALUE>{};
    }

    if (grid1 == QString() && grid2 == QString())
        return UnifiedFixedData::getInstance()->getInputData()->getFramePointList();
    else if (grid1 == QString()){
        return QList<FRAMEPOINTSVALUE>{fp2};
    }else if (grid2 == QString()){
        return QList<FRAMEPOINTSVALUE>{fp1};
    }else if (grid1 == grid2){
        return QList<FRAMEPOINTSVALUE>{fp1};
    }else{
        if (fp1.GID != fp2.GID){
            *errorMessage = "grid group of a1 and a2 are different";
            return QList<FRAMEPOINTSVALUE>{};
        }

        QList<FRAMEPOINTSVALUE> fplist;
        QStringList namelist = UnifiedFixedData::getInstance()->getInputData()->getNameList(DATATYPE::TPFRAMEG);
        bool isWithin = false;
        for (int i = 0; i < namelist.count(); i++){
            if (namelist.at(i) == grid1 || namelist.at(i) == grid2){
                if (isWithin == true){
                    fplist.append(getFrame(namelist.at(i), errorMessage));
                    return fplist;
                }else{
                    isWithin = true;
                }
            }

            fplist.append(getFrame(namelist.at(i), errorMessage));
        }
    }

    *errorMessage = "error";
    return QList<FRAMEPOINTSVALUE>{};
}

QSet<QString> AbstratAreaLineCondition::getJoints(const QString& txt, QString* errorMessage) const{
    QList<QSet<JointData*>> joints = getJointList(txt, errorMessage);
    if (*errorMessage != QString()){
        return QSet<QString>{};
    }
    QSet<QString> jointName;
    Q_FOREACH(auto set, joints){
        Q_FOREACH(auto j, set){
            QString name = UnifiedFixedData::getInstance()->getAnalysisJointName(j->getUuid());
            jointName.insert(name);
        }
    }
    return jointName;
}
QSet<QString> AbstratAreaLineCondition::getMembers(const QString& txt, QString* errorMessage) const{
    const QList<MEMBERCATEGORY> category = checkMemberCategory(txt.mid(10, 2).trimmed(), errorMessage);
    if (*errorMessage != QString()){
        return QSet<QString>{};
    }
    const QString sectionName = txt.mid(12, 8).trimmed();

    QList<QSet<JointData*>> joints = getJointList(txt, errorMessage);
    if (*errorMessage != QString()){
        return QSet<QString>{};
    }

    QSet<QString> memberSet;
    Q_FOREACH(auto jointSet, joints){
        if (category.contains(MEMBERCATEGORY::COLUMN)){
            Q_FOREACH(auto column, UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPCOLUMN)){
                if (sectionName != QString() && column->sectionName() != sectionName)
                    continue;

                Q_FOREACH(auto j, jointSet){
                    for(int i = 0; i < column->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).count() - 1; i++){
                        if (j == column->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i)){
                            memberSet.insert(UnifiedFixedData::getInstance()->getAnalysisMemberName(column->getUuid(), i));
                        }
                    }
                }
            }
        }
        if (category.contains(MEMBERCATEGORY::GIRDER)){
            Q_FOREACH(auto girder, UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPGIRDER)){
                if (sectionName != QString() && girder->sectionName() != sectionName)
                    continue;

                for(int i = 0; i < girder->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).count() - 1; i++){
                    bool hasStartJoint = false;
                    bool hasEndJoint = false;
                    Q_FOREACH(auto j, jointSet){
                        if (j == girder->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i))
                            hasStartJoint = true;
                        if (j == girder->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i+1))
                            hasEndJoint = true;
                    }
                    if (hasStartJoint && hasEndJoint){
                        memberSet.insert(UnifiedFixedData::getInstance()->getAnalysisMemberName(girder->getUuid(), i));
                    }
                }
            }
        }
        if (category.contains(MEMBERCATEGORY::BRACE)){
            Q_FOREACH(auto brace, UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPBRACE)){
                if (sectionName != QString() && brace->sectionName() != sectionName)
                    continue;

                for(int i = 0; i < brace->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).count() - 1; i++){
                    bool hasStartJoint = false;
                    bool hasEndJoint = false;
                    Q_FOREACH(auto j, jointSet){
                        if (j == brace->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i))
                            hasStartJoint = true;
                        if (j == brace->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i+1))
                            hasEndJoint = true;
                    }
                    if (hasStartJoint && hasEndJoint){
                        memberSet.insert(UnifiedFixedData::getInstance()->getAnalysisMemberName(brace->getUuid(), i));
                    }
                }
            }

        }
        if (category.contains(MEMBERCATEGORY::DAMPER)){
            Q_FOREACH(auto damper, UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPDAMPER)){
                if (sectionName != QString() && damper->sectionName() != sectionName)
                    continue;

                for(int i = 0; i < damper->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).count() - 1; i++){
                    bool hasStartJoint = false;
                    bool hasEndJoint = false;
                    Q_FOREACH(auto j, jointSet){
                        if (j == damper->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i))
                            hasStartJoint = true;
                        if (j == damper->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i+1))
                            hasEndJoint = true;
                    }
                    if (hasStartJoint && hasEndJoint){
                        memberSet.insert(UnifiedFixedData::getInstance()->getAnalysisMemberName(damper->getUuid(), i));
                    }
                }
            }
        }
        if (category.contains(MEMBERCATEGORY::ISOLATOR)){
            Q_FOREACH(auto iso, UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPISO)){
                if (sectionName != QString() && iso->sectionName() != sectionName)
                    continue;

                Q_FOREACH(auto j, jointSet){
                    for(int i = 0; i < iso->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).count() - 1; i++){
                        if (j == iso->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i)){
                            memberSet.insert(UnifiedFixedData::getInstance()->getAnalysisMemberName(iso->getUuid(), i));
                        }
                    }
                }
            }
        }
        if (category.contains(MEMBERCATEGORY::WALL)){
            Q_FOREACH(auto wall, UnifiedFixedData::getInstance()->getInputData()->memberListOfType(QList<DATATYPE>{DATATYPE::TPWALL, DATATYPE::TPUWALL})){
                if (sectionName != QString() && wall->sectionName() != sectionName)
                    continue;

                bool hasAllJoints = true;
                for(int i = 0; i < wall->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).count(); i++){
                    bool hasOneJoint = false;
                    Q_FOREACH(auto j, jointSet){
                        if (wall->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i) == j){
                            hasOneJoint = true;
                            break;
                        }
                    }
                    if (!hasOneJoint){
                        hasAllJoints = false;
                        break;
                    }
                }

                if (hasAllJoints)
                    memberSet.insert(UnifiedFixedData::getInstance()->getAnalysisMemberName(wall->getUuid(), 0));
            }
        }
        if (category.contains(MEMBERCATEGORY::SLAB)){
            Q_FOREACH(auto slab, UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPSLAB)){
                if (sectionName != QString() && slab->sectionName() != sectionName)
                    continue;

                bool hasAllJoints = true;
                for(int i = 0; i < slab->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).count(); i++){
                    bool hasOneJoint = false;
                    Q_FOREACH(auto j, jointSet){
                        if (slab->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData()).at(i) == j){
                            hasOneJoint = true;
                            break;
                        }
                    }
                    if (!hasOneJoint){
                        hasAllJoints = false;
                        break;
                    }
                }

                if (hasAllJoints){
                    // 解析用スラブかどうかは？
                    memberSet.insert(UnifiedFixedData::getInstance()->getAnalysisMemberName(slab->getUuid(), 0));
                }
            }
        }
    }

    return memberSet;
}

QList<QSet<JointData*>> BulkAreaLineCondition::getJointList(const QString& txt, QString* errorMessage) const
{
    const QString floor1 = txt.mid(20, 10).trimmed();
    const QString floor2 = txt.mid(30, 10).trimmed();
    QList<FLOORVALUES> floors = getFloors(floor1, floor2, errorMessage);
    if (errorMessage != QString())
        return QList<QSet<JointData*>>{};

    const QString gridA1 = txt.mid(40, 10).trimmed();
    const QString gridA2 = txt.mid(50, 10).trimmed();
    QList<FRAMEPOINTSVALUE> frames = getFrames(gridA1, gridA2, errorMessage);
    if (errorMessage != QString())
        return QList<QSet<JointData*>>{};

    const QString gridB = txt.mid(60, 10).trimmed();
    FRAMEPOINTSVALUE gB, gC;
    if (gridB != QString()){
        gB = getFrame(gridB, errorMessage);
        if (errorMessage != QString())
            return QList<QSet<JointData*>>{};
    }

    const QString gridC = txt.mid(70, 10).trimmed();
    if (gridC != QString()){
        gC = getFrame(gridC, errorMessage);
        if (errorMessage != QString())
            return QList<QSet<JointData*>>{};
    }

    if (gridA1 == QString{} && gridA2 == QString{} && (gridB != QString() || gridC != QString())){
        *errorMessage = u8"通芯の設定が不正です。";
        return QList<QSet<JointData*>>{};
    }

    if (floor1 == QString()
            && floor2 == QString()
            && gridA1 == QString()
            && gridA2 == QString()
            && gridB == QString()
            && gridC == QString()){
        QSet<JointData*> jointSet;
        Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfAll()){
            if (j->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData())){
                jointSet.insert(j);
            }
        }
        return QList<QSet<JointData*>>{jointSet};
    }

    if (gridA1 == QString()
            && gridA2 == QString()
            && gridB == QString()
            && gridC == QString()){
        QSet<JointData*> jointSet;
        Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfAll()){
            if (!j->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData())){
                continue;
            }
            Q_FOREACH(auto fl, floors){
                if (j->containsFloor(fl.UUID)){
                    jointSet.insert(j);
                    break;
                }
            }
        }
        return QList<QSet<JointData*>>{jointSet};
    }

    QSet<JointData*> jointSet;
    Q_FOREACH(auto frame, frames){
        Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfFrame(frame.UUID)){
            if (!j->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()))
                continue;

            if (floors.count() == 0){
            }
            else{
                bool containsFloor = false;
                QList<QUuid> uidFloors;
                Q_FOREACH(auto fl, floors){
                    uidFloors.append(fl.UUID);
                }
                Q_FOREACH(auto uid, uidFloors){
                    if (j->containsFloor(uid)){
                        containsFloor = true;
                        break;
                    }
                }
                if (!containsFloor)
                    continue;
            }

            if (gridB == QString() && gridC == QString()){
                jointSet.insert(j);
            }else if (gridB == QString() || gridB == gridC){
                if (j->containsFrame(gC.UUID))
                    jointSet.insert(j);
            }else if (gridC == QString()){
                if (j->containsFrame(gB.UUID))
                    jointSet.insert(j);
            }else{
                // 交点計算
                QList<QPointF> crossB = CalcVector2D::crossPointList(frame.pointList, gB.pointList);
                if (crossB.count() >= 2){
                    *errorMessage = gB.frameName + "has two cross points in" + frame.frameName;
                    return QList<QSet<JointData*>>{};
                } else if (crossB.count() == 0){
                    *errorMessage = gB.frameName + "has no cross point in" + frame.frameName;
                    return QList<QSet<JointData*>>{};
                }
                QList<QPointF> crossC = CalcVector2D::crossPointList(frame.pointList, gC.pointList);
                if (crossC.count() >= 2){
                    *errorMessage = gC.frameName + "has two cross points in" + frame.frameName;
                    return QList<QSet<JointData*>>{};
                } else if (crossC.count() == 0){
                    *errorMessage = gC.frameName + "has no cross point in" + frame.frameName;
                    return QList<QSet<JointData*>>{};
                }

                QPointF bXyz = crossB.first();
                QPointF cXyz = crossC.first();

                if (frame.frameType == FrameType::FRAMETYPE::STRAIGHT
                        || frame.frameType == FrameType::FRAMETYPE::DIAMETER
                        || frame.frameType == FrameType::FRAMETYPE::RADIUS){

                    if (j->containsFrame(gB.UUID)){
                        jointSet.insert(j);
                    }
                    if (j->containsFrame(gC.UUID)){
                        jointSet.insert(j);
                    }
                    if (qAbs(bXyz.x() - cXyz.x()) > qAbs(bXyz.y() - cXyz.y())){
                        if ((bXyz.x() < j->xPos() && j->xPos() < cXyz.x()) || (cXyz.x() < j->xPos() && j->xPos() < bXyz.x()))
                            jointSet.insert(j);
                    }else{
                        if ((bXyz.y() < j->yPos() && j->yPos() < cXyz.y()) || (cXyz.y() < j->yPos() && j->yPos() < bXyz.y()))
                            jointSet.insert(j);
                    }
                }else if (frame.frameType == FrameType::FRAMETYPE::CIRCLE || frame.frameType == FrameType::FRAMETYPE::CLCIRCLE){
                    FRAMEGROUPVALUE fgv;
                    QPointF basePoint;
                    Q_FOREACH(auto fg, UnifiedFixedData::getInstance()->getInputData()->getFrameGroupList()){
                        if (fg.GID == frame.GID){
                            fgv = fg;
                            basePoint = fg.basePoint;
                            break;
                        }
                    }
                    double angleB = bXyz.x() - basePoint.x() == 0 ? 0 : qTan((bXyz.y() - basePoint.y())/ bXyz.x() - basePoint.x());
                    double angleC = cXyz.x() - basePoint.x() == 0 ? 0 : qTan((cXyz.y() - basePoint.y())/ cXyz.x() - basePoint.x());

                    if (j->containsFrame(gB.UUID)){
                        jointSet.insert(j);
                    }
                    if (j->containsFrame(gC.UUID)){
                        jointSet.insert(j);
                    }
                    double angle = j->xyz().x - basePoint.x() == 0 ? 0 : qTan((j->xyz().y - basePoint.y())/ j->xyz().x - basePoint.x());
                    if (angleC > angleB){
                        if (angleB < angle && angle < angleC){
                            jointSet.insert(j);
                        }
                    }else{
                        if (frame.frameType == FrameType::FRAMETYPE::CIRCLE){
                            if (angleC < angle && angle < angleB)
                                jointSet.insert(j);
                        }else{
                            if ((angleB < angle && angle < M_PI * 2.0) || (0.0 < angle && angle < angleC))
                                jointSet.insert(j);
                        }
                    }
                }else if (frame.frameType == FrameType::FRAMETYPE::VARIABLE || frame.frameType == FrameType::FRAMETYPE::CLVARIABLE){
                    QList<QPointF> pointList;
                    int indexB, indexC;
                    for (int i = 0; i < frame.pointList.count() - 1; i++){
                        if (CalcVector2D::between(frame.pointList.at(i), frame.pointList.at(i+1), bXyz)){
                            indexB = i;
                        }
                        if (CalcVector2D::between(frame.pointList.at(i), frame.pointList.at(i+1), cXyz)){
                            indexC = i;
                        }
                    }
                    if (frame.frameType == FrameType::FRAMETYPE::VARIABLE){
                        if (indexB == indexC){
                            pointList.append(bXyz);
                            pointList.append(cXyz);
                        }else if (indexB > indexC){
                            pointList.append(cXyz);
                            pointList.append(frame.pointList.mid(indexC + 1, indexB - indexC));
                            pointList.append(bXyz);
                        }else{
                            pointList.append(bXyz);
                            pointList.append(frame.pointList.mid(indexB + 1, indexC - indexB));
                            pointList.append(cXyz);
                        }
                    }else{
                        if (indexB == indexC){
                            QPointF start = frame.pointList.at(indexB);
                            double distanceB = CalcVector2D::length(start - bXyz);
                            double distanceC = CalcVector2D::length(start - cXyz);
                            if (distanceB <= distanceC){
                                pointList.append(bXyz);
                                pointList.append(cXyz);
                            }else{
                                pointList.append(cXyz);
                                pointList.append(frame.pointList.mid(indexC + 1));
                                pointList.append(frame.pointList.mid(0, indexB));
                                pointList.append(bXyz);
                            }
                        }else if (indexB > indexC){
                            pointList.append(cXyz);
                            pointList.append(frame.pointList.mid(indexC + 1));
                            pointList.append(frame.pointList.mid(0, indexB));
                            pointList.append(bXyz);
                        }else{
                            pointList.append(bXyz);
                            pointList.append(frame.pointList.mid(indexB + 1, indexC - indexB));
                            pointList.append(cXyz);
                        }
                    }

                    if (j->containsFrame(gB.UUID)){
                        jointSet.insert(j);
                    }
                    if (j->containsFrame(gC.UUID)){
                        jointSet.insert(j);
                    }
                    for (int i = 0; i < pointList.count() - 1; i++){
                        if (CalcVector2D::between(pointList.at(i), pointList.at(i+1), QPointF(j->xyz().x, j->xyz().y))){
                            jointSet.insert(j);
                            break;
                        }
                    }
                }
            }
        }
    }

    return QList<QSet<JointData*>>{jointSet};
}



QList<QSet<JointData*>> GridAreaLineCondition::getJointList(const QString& txt, QString* errorMessage) const
{
    const QString floor1 = txt.mid(20, 10).trimmed();
    const QString floor2 = txt.mid(30, 10).trimmed();
    QList<FLOORVALUES> floors = getFloors(floor1, floor2, errorMessage);
    if (errorMessage != QString())
        return QList<QSet<JointData*>>{};

    const QString gridA1 = txt.mid(40, 10).trimmed();
    const QString gridA2 = txt.mid(50, 10).trimmed();
    QList<FRAMEPOINTSVALUE> frames = getFrames(gridA1, gridA2, errorMessage);
    if (errorMessage != QString())
        return QList<QSet<JointData*>>{};

    const QString gridB = txt.mid(60, 10).trimmed();
    FRAMEPOINTSVALUE gB, gC;
    if (gridB != QString()){
        gB = getFrame(gridB, errorMessage);
        if (errorMessage != QString())
            return QList<QSet<JointData*>>{};
    }

    const QString gridC = txt.mid(70, 10).trimmed();
    if (gridC != QString()){
        gC = getFrame(gridC, errorMessage);
        if (errorMessage != QString())
            return QList<QSet<JointData*>>{};
    }

    if (gridA1 == QString{} && gridA2 == QString{} && (gridB != QString() || gridC != QString())){
        *errorMessage = u8"通芯の設定が不正です。";
        return QList<QSet<JointData*>>{};
    }

    if (floor1 == QString()
            && floor2 == QString()
            && gridA1 == QString()
            && gridA2 == QString()
            && gridB == QString()
            && gridC == QString()){
        QList<QSet<JointData*>> joints;
        Q_FOREACH(auto grid, UnifiedFixedData::getInstance()->getInputData()->getFramePointList()){
            QSet<JointData*> jointSet;
            Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfFrame(grid.UUID)){
                if (j->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData())){
                    jointSet.insert(j);
                }
            }
            joints.append(jointSet);
        }
        return joints;
    }

    if (gridA1 == QString()
            && gridA2 == QString()
            && gridB == QString()
            && gridC == QString()){
        QList<QSet<JointData*>> joints;
        Q_FOREACH(auto grid, UnifiedFixedData::getInstance()->getInputData()->getFramePointList()){
            QSet<JointData*> jointSet;
            Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfFrame(grid.UUID)){
                if (!j->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData())){
                    continue;
                }
                Q_FOREACH(auto fl, floors){
                    if (j->containsFloor(fl.UUID)){
                        jointSet.insert(j);
                        break;
                    }
                }
            }
            joints.append(jointSet);
        }

        return joints;
    }

    QList<QSet<JointData*>> joints;
    Q_FOREACH(auto frame, frames){
        QSet<JointData*> jointSet;
        Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfFrame(frame.UUID)){
            if (!j->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()))
                continue;

            if (floors.count() == 0){
            }
            else{
                bool containsFloor = false;
                QList<QUuid> uidFloors;
                Q_FOREACH(auto fl, floors){
                    uidFloors.append(fl.UUID);
                }
                Q_FOREACH(auto uid, uidFloors){
                    if (j->containsFloor(uid)){
                        containsFloor = true;
                        break;
                    }
                }
                if (!containsFloor)
                    continue;
            }

            if (gridB == QString() && gridC == QString()){
                jointSet.insert(j);
            }else if (gridB == QString() || gridB == gridC){
                if (j->containsFrame(gC.UUID))
                    jointSet.insert(j);
            }else if (gridC == QString()){
                if (j->containsFrame(gB.UUID))
                    jointSet.insert(j);
            }else{
                // 交点計算
                QList<QPointF> crossB = CalcVector2D::crossPointList(frame.pointList, gB.pointList);
                if (crossB.count() >= 2){
                    *errorMessage = gB.frameName + "has two cross points in" + frame.frameName;
                    return QList<QSet<JointData*>>{};
                } else if (crossB.count() == 0){
                    *errorMessage = gB.frameName + "has no cross point in" + frame.frameName;
                    return QList<QSet<JointData*>>{};
                }
                QList<QPointF> crossC = CalcVector2D::crossPointList(frame.pointList, gC.pointList);
                if (crossC.count() >= 2){
                    *errorMessage = gC.frameName + "has two cross points in" + frame.frameName;
                    return QList<QSet<JointData*>>{};
                } else if (crossC.count() == 0){
                    *errorMessage = gC.frameName + "has no cross point in" + frame.frameName;
                    return QList<QSet<JointData*>>{};
                }

                QPointF bXyz = crossB.first();
                QPointF cXyz = crossC.first();

                if (frame.frameType == FrameType::FRAMETYPE::STRAIGHT
                        || frame.frameType == FrameType::FRAMETYPE::DIAMETER
                        || frame.frameType == FrameType::FRAMETYPE::RADIUS){

                    if (j->containsFrame(gB.UUID)){
                        jointSet.insert(j);
                    }
                    if (j->containsFrame(gC.UUID)){
                        jointSet.insert(j);
                    }
                    if (qAbs(bXyz.x() - cXyz.x()) > qAbs(bXyz.y() - cXyz.y())){
                        if ((bXyz.x() < j->xPos() && j->xPos() < cXyz.x()) || (cXyz.x() < j->xPos() && j->xPos() < bXyz.x()))
                            jointSet.insert(j);
                    }else{
                        if ((bXyz.y() < j->yPos() && j->yPos() < cXyz.y()) || (cXyz.y() < j->yPos() && j->yPos() < bXyz.y()))
                            jointSet.insert(j);
                    }
                }else if (frame.frameType == FrameType::FRAMETYPE::CIRCLE || frame.frameType == FrameType::FRAMETYPE::CLCIRCLE){
                    FRAMEGROUPVALUE fgv;
                    QPointF basePoint;
                    Q_FOREACH(auto fg, UnifiedFixedData::getInstance()->getInputData()->getFrameGroupList()){
                        if (fg.GID == frame.GID){
                            fgv = fg;
                            basePoint = fg.basePoint;
                            break;
                        }
                    }
                    double angleB = bXyz.x() - basePoint.x() == 0 ? 0 : qAtan2(bXyz.y() - basePoint.y(), bXyz.x() - basePoint.x());
                    double angleC = cXyz.x() - basePoint.x() == 0 ? 0 : qAtan2(cXyz.y() - basePoint.y(), cXyz.x() - basePoint.x());

                    if (j->containsFrame(gB.UUID)){
                        jointSet.insert(j);
                    }
                    if (j->containsFrame(gC.UUID)){
                        jointSet.insert(j);
                    }
                    double angle = j->xyz().x - basePoint.x() == 0 ? 0 : qAtan2(j->xyz().y - basePoint.y(), j->xyz().x - basePoint.x());
                    if (angleC > angleB){
                        if (angleB < angle && angle < angleC){
                            jointSet.insert(j);
                        }
                    }else{
                        if (frame.frameType == FrameType::FRAMETYPE::CIRCLE){
                            if (angleC < angle && angle < angleB)
                                jointSet.insert(j);
                        }else{
                            if ((angleB < angle && angle < M_PI * 2.0) || (0.0 < angle && angle < angleC))
                                jointSet.insert(j);
                        }
                    }
                }else if (frame.frameType == FrameType::FRAMETYPE::VARIABLE || frame.frameType == FrameType::FRAMETYPE::CLVARIABLE){
                    QList<QPointF> pointList;
                    int indexB, indexC;
                    for (int i = 0; i < frame.pointList.count() - 1; i++){
                        if (CalcVector2D::between(frame.pointList.at(i), frame.pointList.at(i+1), bXyz)){
                            indexB = i;
                        }
                        if (CalcVector2D::between(frame.pointList.at(i), frame.pointList.at(i+1), cXyz)){
                            indexC = i;
                        }
                    }
                    if (frame.frameType == FrameType::FRAMETYPE::VARIABLE){
                        if (indexB == indexC){
                            pointList.append(bXyz);
                            pointList.append(cXyz);
                        }else if (indexB > indexC){
                            pointList.append(cXyz);
                            pointList.append(frame.pointList.mid(indexC + 1, indexB - indexC));
                            pointList.append(bXyz);
                        }else{
                            pointList.append(bXyz);
                            pointList.append(frame.pointList.mid(indexB + 1, indexC - indexB));
                            pointList.append(cXyz);
                        }
                    }else{
                        if (indexB == indexC){
                            QPointF start = frame.pointList.at(indexB);
                            double distanceB = CalcVector2D::length(start - bXyz);
                            double distanceC = CalcVector2D::length(start - cXyz);
                            if (distanceB <= distanceC){
                                pointList.append(bXyz);
                                pointList.append(cXyz);
                            }else{
                                pointList.append(cXyz);
                                pointList.append(frame.pointList.mid(indexC + 1));
                                pointList.append(frame.pointList.mid(0, indexB));
                                pointList.append(bXyz);
                            }
                        }else if (indexB > indexC){
                            pointList.append(cXyz);
                            pointList.append(frame.pointList.mid(indexC + 1));
                            pointList.append(frame.pointList.mid(0, indexB));
                            pointList.append(bXyz);
                        }else{
                            pointList.append(bXyz);
                            pointList.append(frame.pointList.mid(indexB + 1, indexC - indexB));
                            pointList.append(cXyz);
                        }
                    }

                    if (j->containsFrame(gB.UUID)){
                        jointSet.insert(j);
                    }
                    if (j->containsFrame(gC.UUID)){
                        jointSet.insert(j);
                    }
                    for (int i = 0; i < pointList.count() - 1; i++){
                        if (CalcVector2D::between(pointList.at(i), pointList.at(i+1), QPointF(j->xyz().x, j->xyz().y))){
                            jointSet.insert(j);
                            break;
                        }
                    }
                }
            }
        }
        joints.append(jointSet);
    }

    return joints;
}

QList<QSet<JointData*>> FloorAreaLineCondition::getJointList(const QString& txt, QString* errorMessage) const{
    const QString floor1 = txt.mid(20, 10).trimmed();
    const QString floor2 = txt.mid(30, 10).trimmed();
    QList<FLOORVALUES> floors = getFloors(floor1, floor2, errorMessage);
    if (errorMessage != QString())
        return QList<QSet<JointData*>>{};

    const QString gridA1 = txt.mid(40, 10).trimmed();
    const QString gridA2 = txt.mid(50, 10).trimmed();
    QList<FRAMEPOINTSVALUE> frames = getFrames(gridA1, gridA2, errorMessage);
    if (errorMessage != QString())
        return QList<QSet<JointData*>>{};

    const QString gridB = txt.mid(60, 10).trimmed();
    FRAMEPOINTSVALUE gB, gC;
    if (gridB != QString()){
        gB = getFrame(gridB, errorMessage);
        if (errorMessage != QString())
            return QList<QSet<JointData*>>{};
    }

    const QString gridC = txt.mid(70, 10).trimmed();
    if (gridC != QString()){
        gC = getFrame(gridC, errorMessage);
        if (errorMessage != QString())
            return QList<QSet<JointData*>>{};
    }

    if (gridA1 == QString{} && gridA2 == QString{} && (gridB != QString() || gridC != QString())){
        *errorMessage = u8"通芯の設定が不正です。";
        return QList<QSet<JointData*>>{};
    }

    if (floor1 == QString()
            && floor2 == QString()
            && gridA1 == QString()
            && gridA2 == QString()
            && gridB == QString()
            && gridC == QString()){
        QList<QSet<JointData*>> joints;
        Q_FOREACH(auto floor, UnifiedFixedData::getInstance()->getInputData()->getFloorList()){
            QSet<JointData*> jointSet;
            Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfFloor(floor.UUID)){
                if (j->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData())){
                    jointSet.insert(j);
                }
            }
            joints.append(jointSet);
        }
        return joints;
    }

    if (gridA1 == QString()
            && gridA2 == QString()
            && gridB == QString()
            && gridC == QString()){
        QList<QSet<JointData*>> joints;
        Q_FOREACH(auto floor, floors){
            QSet<JointData*> jointSet;
            Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfFloor(floor.UUID)){
                if (!j->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData())){
                    continue;
                }
                jointSet.insert(j);
            }
            joints.append(jointSet);
        }

        return joints;
    }

    QList<QUuid> uidFloors;
    if (floors.count() == 0){
        auto floorList = UnifiedFixedData::getInstance()->getInputData()->getFloorList();
        foreach(auto l, floorList){
            uidFloors.append(l.UUID);
        }
    }else
    {
        Q_FOREACH(auto fl, floors){
            uidFloors.append(fl.UUID);
        }
    }

    QList<QSet<JointData*>> joints;
    Q_FOREACH(auto uid, uidFloors){
        QSet<JointData*> jointSet;
        Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfFloor(uid)){
            if (!j->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()))
                continue;

            bool containsFrame = false;
            Q_FOREACH(auto frame, frames){
                if (j->containsFrame(frame.UUID)){
                    containsFrame = true;
                    break;
                }

                if (!containsFrame)
                    continue;

                if (gridB == QString() && gridC == QString()){
                    jointSet.insert(j);
                }else if (gridB == QString() || gridB == gridC){
                    if (j->containsFrame(gC.UUID))
                        jointSet.insert(j);
                }else if (gridC == QString()){
                    if (j->containsFrame(gB.UUID))
                        jointSet.insert(j);

                }else{
                    // 交点計算
                    QList<QPointF> crossB = CalcVector2D::crossPointList(frame.pointList, gB.pointList);
                    if (crossB.count() >= 2){
                        *errorMessage = gB.frameName + "has two cross points in" + frame.frameName;
                        return QList<QSet<JointData*>>{};
                    } else if (crossB.count() == 0){
                        *errorMessage = gB.frameName + "has no cross point in" + frame.frameName;
                        return QList<QSet<JointData*>>{};
                    }
                    QList<QPointF> crossC = CalcVector2D::crossPointList(frame.pointList, gC.pointList);
                    if (crossC.count() >= 2){
                        *errorMessage = gC.frameName + "has two cross points in" + frame.frameName;
                        return QList<QSet<JointData*>>{};
                    } else if (crossC.count() == 0){
                        *errorMessage = gC.frameName + "has no cross point in" + frame.frameName;
                        return QList<QSet<JointData*>>{};
                    }

                    QPointF bXyz = crossB.first();
                    QPointF cXyz = crossC.first();

                    if (frame.frameType == FrameType::FRAMETYPE::STRAIGHT
                            || frame.frameType == FrameType::FRAMETYPE::DIAMETER
                            || frame.frameType == FrameType::FRAMETYPE::RADIUS){

                        if (j->containsFrame(gB.UUID)){
                            jointSet.insert(j);
                        }
                        if (j->containsFrame(gC.UUID)){
                            jointSet.insert(j);
                        }
                        if (qAbs(bXyz.x() - cXyz.x()) > qAbs(bXyz.y() - cXyz.y())){
                            if ((bXyz.x() < j->xPos() && j->xPos() < cXyz.x()) || (cXyz.x() < j->xPos() && j->xPos() < bXyz.x()))
                                jointSet.insert(j);
                        }else{
                            if ((bXyz.y() < j->yPos() && j->yPos() < cXyz.y()) || (cXyz.y() < j->yPos() && j->yPos() < bXyz.y()))
                                jointSet.insert(j);
                        }
                    }else if (frame.frameType == FrameType::FRAMETYPE::CIRCLE || frame.frameType == FrameType::FRAMETYPE::CLCIRCLE){
                        FRAMEGROUPVALUE fgv;
                        QPointF basePoint;
                        Q_FOREACH(auto fg, UnifiedFixedData::getInstance()->getInputData()->getFrameGroupList()){
                            if (fg.GID == frame.GID){
                                fgv = fg;
                                basePoint = fg.basePoint;
                                break;
                            }
                        }
                        double angleB = bXyz.x() - basePoint.x() == 0 ? 0 : qTan((bXyz.y() - basePoint.y())/ bXyz.x() - basePoint.x());
                        double angleC = cXyz.x() - basePoint.x() == 0 ? 0 : qTan((cXyz.y() - basePoint.y())/ cXyz.x() - basePoint.x());

                        if (j->containsFrame(gB.UUID)){
                            jointSet.insert(j);
                        }
                        if (j->containsFrame(gC.UUID)){
                            jointSet.insert(j);
                        }
                        double angle = j->xyz().x - basePoint.x() == 0 ? 0 : qTan((j->xyz().y - basePoint.y())/ j->xyz().x - basePoint.x());
                        if (angleC > angleB){
                            if (angleB < angle && angle < angleC){
                                jointSet.insert(j);
                            }
                        }else{
                            if (frame.frameType == FrameType::FRAMETYPE::CIRCLE){
                                if (angleC < angle && angle < angleB)
                                    jointSet.insert(j);
                            }else{
                                if ((angleB < angle && angle < M_PI * 2.0) || (0.0 < angle && angle < angleC))
                                    jointSet.insert(j);
                            }
                        }
                    }else if (frame.frameType == FrameType::FRAMETYPE::VARIABLE || frame.frameType == FrameType::FRAMETYPE::CLVARIABLE){
                        QList<QPointF> pointList;
                        int indexB, indexC;
                        for (int i = 0; i < frame.pointList.count() - 1; i++){
                            if (CalcVector2D::between(frame.pointList.at(i), frame.pointList.at(i+1), bXyz)){
                                indexB = i;
                            }
                            if (CalcVector2D::between(frame.pointList.at(i), frame.pointList.at(i+1), cXyz)){
                                indexC = i;
                            }
                        }
                        if (frame.frameType == FrameType::FRAMETYPE::VARIABLE){
                            if (indexB == indexC){
                                pointList.append(bXyz);
                                pointList.append(cXyz);
                            }else if (indexB > indexC){
                                pointList.append(cXyz);
                                pointList.append(frame.pointList.mid(indexC + 1, indexB - indexC));
                                pointList.append(bXyz);
                            }else{
                                pointList.append(bXyz);
                                pointList.append(frame.pointList.mid(indexB + 1, indexC - indexB));
                                pointList.append(cXyz);
                            }
                        }else{
                            if (indexB == indexC){
                                QPointF start = frame.pointList.at(indexB);
                                double distanceB = CalcVector2D::length(start - bXyz);
                                double distanceC = CalcVector2D::length(start - cXyz);
                                if (distanceB <= distanceC){
                                    pointList.append(bXyz);
                                    pointList.append(cXyz);
                                }else{
                                    pointList.append(cXyz);
                                    pointList.append(frame.pointList.mid(indexC + 1));
                                    pointList.append(frame.pointList.mid(0, indexB));
                                    pointList.append(bXyz);
                                }
                            }else if (indexB > indexC){
                                pointList.append(cXyz);
                                pointList.append(frame.pointList.mid(indexC + 1));
                                pointList.append(frame.pointList.mid(0, indexB));
                                pointList.append(bXyz);
                            }else{
                                pointList.append(bXyz);
                                pointList.append(frame.pointList.mid(indexB + 1, indexC - indexB));
                                pointList.append(cXyz);
                            }
                        }

                        if (j->containsFrame(gB.UUID)){
                            jointSet.insert(j);
                        }
                        if (j->containsFrame(gC.UUID)){
                            jointSet.insert(j);
                        }
                        for (int i = 0; i < pointList.count() - 1; i++){
                            if (CalcVector2D::between(pointList.at(i), pointList.at(i+1), QPointF(j->xyz().x, j->xyz().y))){
                                jointSet.insert(j);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return joints;
}

QList<QSet<JointData*>> NodeAreaCondition::getJointList(const QString& txt, QString* errorMessage) const{
    bool isOk = true;
    int number = txt.mid(5, 5).toInt(&isOk);
    if (!isOk){
        *errorMessage = "cannot read number";
        return QList<QSet<JointData*>>{};
    }
    if (number < 3){
        *errorMessage = "number < 3";
        return QList<QSet<JointData*>>{};
    }

    const QString floor1 = txt.mid(20, 10).trimmed();
    const QString floor2 = txt.mid(30, 10).trimmed();
    QList<FLOORVALUES> floors = getFloors(floor1, floor2, errorMessage);
    if (errorMessage != QString())
        return QList<QSet<JointData*>>{};

    QList<QPointF> points;
    for (int i = 0; i < number; i++){
        QString gridA = txt.mid(40 + i * 20, 10).trimmed();
        if (gridA == QString()){
            *errorMessage = "unsigned grid error";
            return QList<QSet<JointData*>>{};
        }
        FRAMEPOINTSVALUE a = getFrame(gridA, errorMessage);
        if (errorMessage != QString())
            return QList<QSet<JointData*>>{};


        QString gridB = txt.mid(50 + i * 20, 10).trimmed();
        if (gridB == QString()){
            *errorMessage = "unsigned grid error";
            return QList<QSet<JointData*>>{};
        }
        FRAMEPOINTSVALUE b = getFrame(gridB, errorMessage);
        if (errorMessage != QString())
            return QList<QSet<JointData*>>{};

        QList<QPointF> point = CalcVector2D::crossPointList(a.pointList, b.pointList);
        if (point.count() > 1){
            *errorMessage = "two cross points";
            return QList<QSet<JointData*>>{};
        }
        points.append(point.first());
    }


    QList<FLOORVALUES> floorList;
    if (floor1 == QString() && floor2 == QString()){
        floorList = UnifiedFixedData::getInstance()->getInputData()->getFloorList();
    }else{
        floorList = floors;
    }

    QPolygonF polygon;
    Q_FOREACH(auto p, points){
        polygon.append(p);
    }

    QList<QSet<JointData*>> joints;
    Q_FOREACH(auto fl, floorList){
        QSet<JointData*> jointSet;
        Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfFloor(fl.UUID)){
            if (CalcVector2D::isInPolygon(polygon, QPointF(j->xyz().x, j->xyz().y))){
                jointSet.insert(j);
            }
        }
        joints.append(jointSet);
    }

    return joints;
}

QList<QSet<JointData*>> CloseAreaCondition::getJointList(const QString& txt, QString* errorMessage) const{
    const QString floor1 = txt.mid(20, 10).trimmed();
    const QString floor2 = txt.mid(30, 10).trimmed();
    QList<FLOORVALUES> floors = getFloors(floor1, floor2, errorMessage);
    if (errorMessage != QString())
        return QList<QSet<JointData*>>{};

    const QString grid = txt.mid(40, 10).trimmed();
    FRAMEPOINTSVALUE gr;
    if (grid != QString()){
        gr = getFrame(grid, errorMessage);
        if (errorMessage != QString())
            return QList<QSet<JointData*>>{};
    }

    if (gr.frameType != FrameType::FRAMETYPE::CLCIRCLE && gr.frameType != FrameType::FRAMETYPE::CLVARIABLE){
        *errorMessage = "frametype is not close";
        return QList<QSet<JointData*>>{};
    }

    QList<FLOORVALUES> floorList;
    if (floor1 == QString() && floor2 == QString()){
        floorList = UnifiedFixedData::getInstance()->getInputData()->getFloorList();
    }else{
        floorList = floors;
    }

    QPolygonF polygon;
    Q_FOREACH(auto p, gr.pointList){
        polygon.append(p);
    }

    QList<QSet<JointData*>> joints;
    Q_FOREACH(auto fl, floorList){
        QSet<JointData*> jointSet;
        Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfFloor(fl.UUID)){
            if (CalcVector2D::isInPolygon(polygon, QPointF(j->xyz().x, j->xyz().y))){
                jointSet.insert(j);
            }
        }
        joints.append(jointSet);
    }

    return joints;
}

QList<QSet<JointData*>> CoordinateAreaCondition::getJointList(const QString& txt, QString* errorMessage) const{
    bool isOk = true;
    const double XMin = txt.mid(20, 10).trimmed().toDouble(&isOk);
    if (!isOk){
        *errorMessage = "XMin format error";
        return QList<QSet<JointData*>>{};
    }

    const double YMin = txt.mid(30, 10).trimmed().toDouble(&isOk);
    if (!isOk){
        *errorMessage = "XMax format error";
        return QList<QSet<JointData*>>{};
    }

    const double ZMin = txt.mid(40, 10).trimmed().toDouble(&isOk);
    if (!isOk){
        *errorMessage = "YMin format error";
        return QList<QSet<JointData*>>{};
    }

    const double XMax = txt.mid(50, 10).trimmed().toDouble(&isOk);
    if (!isOk){
        *errorMessage = "YMax format error";
        return QList<QSet<JointData*>>{};
    }

    const double YMax = txt.mid(60, 10).trimmed().toDouble(&isOk);
    if (!isOk){
        *errorMessage = "ZMin format error";
        return QList<QSet<JointData*>>{};
    }

    const double ZMax = txt.mid(70, 10).trimmed().toDouble(&isOk);
    if (!isOk){
        *errorMessage = "ZMax format error";
        return QList<QSet<JointData*>>{};
    }

    if (XMin > XMax){
        *errorMessage = "XMinがXMaxより大きい";
        return QList<QSet<JointData*>>{};
    }

    if (YMin > YMax){
        *errorMessage = "YMinがYMaxより大きい";
        return QList<QSet<JointData*>>{};
    }

    if (ZMin > ZMax){
        *errorMessage = "ZMinがZMaxより大きい";
        return QList<QSet<JointData*>>{};
    }

    QSet<JointData*> jointSet;
    const double eps = 0.00001;
    Q_FOREACH(auto j, UnifiedFixedData::getInstance()->getInputData()->jointListOfAll()){
        if (XMin - eps < j->xPos() &&
            j->xPos() < XMax + eps &&
            YMin - eps < j->yPos() &&
            j->yPos() < YMax + eps &&
            ZMin - eps < j->zPos() &&
            j->zPos() < ZMax + eps)
            jointSet.insert(j);
    }

    return QList<QSet<JointData*>>{jointSet};
}

QList<MEMBERCATEGORY> AbstratAreaLineCondition::checkMemberCategory(const QString& str, QString* errorMessage) const{
    MEMBERCATEGORY select;
    if (str == QString()){
        return memberCategory;
    }else if(str == "MC"){
        select = MEMBERCATEGORY::COLUMN;
    }else if(str == "MG"){
        select = MEMBERCATEGORY::GIRDER;
    }else if(str == "MV"){
        select = MEMBERCATEGORY::BRACE;
    }else if(str == "WW"){
        select = MEMBERCATEGORY::WALL;
    }else if(str == "WS"){
        select = MEMBERCATEGORY::SLAB;
    }else if(str == "EI"){
        select = MEMBERCATEGORY::ISOLATOR;
    }else if(str == "DP"){
        select = MEMBERCATEGORY::DAMPER;
    }else{
        *errorMessage = "member category error";
        return QList<MEMBERCATEGORY>{};
    }

    if (!memberCategory.contains(select)){
        *errorMessage = "unmatch member catgegory";
        return QList<MEMBERCATEGORY>{};
    }else{
        return QList<MEMBERCATEGORY>{select};
    }
}
}

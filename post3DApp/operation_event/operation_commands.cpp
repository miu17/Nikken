#include "operation_commands.h"

#include <QtGui>
#include <QDebug>

#include "factory_unifieddata.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_data.h"

/* CreateJointCommand */
namespace post3dapp{
CreateJointCommand::CreateJointCommand(const GlobalPoint &gp, bool on,
                                       QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), orgPoint(gp), signalOn(on), isOutput(isout)
{
    stackedID = QUuid();
    setText("create Joint");
}

void CreateJointCommand::redo()
{
    recentJoint = FactoryUnifiedData::getInstance(isOutput)->createJoint(orgPoint, signalOn, stackedID);
    stackedID = recentJoint->getUuid();
}

void CreateJointCommand::undo()
{
    JointData *jd = FactoryUnifiedData::getInstance(isOutput)->jointIDToPointer(stackedID);
    FactoryUnifiedData::getInstance(isOutput)->removeJoints( QList<JointData *>() << jd, signalOn );
}

/* CreateJointsCommand */

CreateJointsCommand::CreateJointsCommand(const QList<GlobalPoint> &gplist, bool on,
                                         QUndoCommand *parent, const UnifiedDataType isout)
    : QUndoCommand(parent), orgPoints(gplist), signalOn(on), isOutput(isout)
{
    setText("create Joints");
}

void CreateJointsCommand::redo()
{
    recentJoints = FactoryUnifiedData::getInstance(isOutput)->createJoints(orgPoints, signalOn, stackedIDs);
    stackedIDs.clear();
    Q_FOREACH ( JointData *jd, recentJoints ) {
        stackedIDs.append( jd->getUuid() );
    }
}

void CreateJointsCommand::undo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedIDs);
    FactoryUnifiedData::getInstance(isOutput)->removeJoints( jdlist, signalOn );
}

/* RemoveJointsCommand */

RemoveJointsCommand::RemoveJointsCommand(const QList<JointData *> &jdlist, bool on,
                                         QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        stackedJoints.append( jd->getUuid() );
        stackedPoints.append( jd->toGlobalPoint() );
        Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
            QUuid m_id = md->getUuid();
            if ( !stackedMembers.contains( m_id ) ) {
                stackedMembers.append( m_id );
                stackedMemberValues.append( md->getMemberValues() );
                QList<QUuid> j_idlist;
                Q_FOREACH ( JointData *jj, md->getJointList() )
                    j_idlist.append( jj->getUuid() );
                stackedMemberJoints.append( j_idlist );
            }
        }
    }
    setText("remove Joints");
}

void RemoveJointsCommand::redo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->removeJoints(jdlist, signalOn);
}

void RemoveJointsCommand::undo()
{
    FactoryUnifiedData::getInstance(isOutput)->createJoints(stackedPoints, signalOn, stackedJoints);

    QList<MemberData *> created_mdlist;
    QList<MemberData *> changed_mdlist;

    for ( int i = 0; i < stackedMembers.count(); i++ ) {

        MemberData *md = FactoryUnifiedData::getInstance(isOutput)->memberIDToPointer( stackedMembers.at(i) );
        QList<JointData *> n_jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(
                                          stackedMemberJoints.at(i) );

        if ( md != nullptr ) {
            md->replaceJointList(n_jdlist);
            changed_mdlist.append(md);
        } else {
            MEMBERVALUES values = stackedMemberValues.at(i);
            md = FactoryUnifiedData::getInstance(isOutput)->createMember(n_jdlist, values.dataType, values.sectionName,
                                                          stackedMembers.at(i));
            md->copyValues(*FactoryUnifiedData::getInstance(isOutput), values);
            created_mdlist.append(md);
        }
    }

    QList<MemberData *> r_changedMembers;
    Q_FOREACH (MemberData *md, created_mdlist) {
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !r_changedMembers.contains(mm) ) r_changedMembers.append(mm);
                }
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !r_changedMembers.contains(mm) ) r_changedMembers.append(mm);
                }
            }
        }
    }
    Q_FOREACH (MemberData *md, changed_mdlist) {
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !r_changedMembers.contains(mm) ) r_changedMembers.append(mm);
                }
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !r_changedMembers.contains(mm) ) r_changedMembers.append(mm);
                }
            }
        }
    }
    Q_FOREACH ( MemberData *md, r_changedMembers ) {
        md->recalcLineLength();
        if ( !changed_mdlist.contains(md) && !created_mdlist.contains(md) ) changed_mdlist.append(md);
    }

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberAppended(created_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}

/* ChangeJointsCommand */

ChangeJointsCommand::ChangeJointsCommand(QList<JointData *> &jdlist,
                                         const QList<GlobalPoint> &gplist, bool on,
                                         QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), changedPoints(gplist), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        stackedJoints.append( jd->getUuid() );
        orgPoints.append( jd->toGlobalPoint() );
    }
    setText("change Joints");
}

void ChangeJointsCommand::redo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    isSuccess = FactoryUnifiedData::getInstance(isOutput)->changeJoints(jdlist, changedPoints, signalOn);
}

void ChangeJointsCommand::undo()
{
    if ( !isSuccess ) return;
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->changeJoints(jdlist, orgPoints, signalOn);
}

/* ChangeJointsAttachmentCommand */

ChangeJointsAttachmentCommand::ChangeJointsAttachmentCommand(QList<JointData *> &jdlist,
                                                             const QList<QUuid> &fidlist,
                                                             DATATYPE type, bool on,
                                                             QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), changedAttachments(fidlist), targetType(type), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        stackedJoints.append( jd->getUuid() );
        if ( targetType == DATATYPE::TPFLOOR ) {
            orgAttachments.append( jd->getAttachedFloorList() );
        } else {
            orgAttachments.append( jd->getAttachedFrameList() );
        }
    }
    setText("change Joints Attachment");
}

void ChangeJointsAttachmentCommand::redo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->changeJointsAttachment(jdlist, changedAttachments, targetType,
                                                       signalOn);
}

void ChangeJointsAttachmentCommand::undo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->changeJointsAttachment(jdlist, orgAttachments, targetType, signalOn);
}

/* AppendJointsAttachmentCommand */

AppendJointsAttachmentCommand::AppendJointsAttachmentCommand(QList<JointData *> &jdlist,
                                                             const QList<QUuid> &fidlist,
                                                             DATATYPE type, bool on,
                                                             QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), appendAttachments(fidlist), targetType(type), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        stackedJoints.append( jd->getUuid() );
        if ( targetType == DATATYPE::TPFLOOR ) {
            orgAttachments.append( jd->getAttachedFloorList() );
        } else {
            orgAttachments.append( jd->getAttachedFrameList() );
        }
    }
    setText("append Joints Attachment");
}

void AppendJointsAttachmentCommand::redo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->appendJointsAttachment(jdlist, appendAttachments, targetType, signalOn);
}

void AppendJointsAttachmentCommand::undo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->changeJointsAttachment(jdlist, orgAttachments, targetType, signalOn);
}

/* CompleteJointsAttachmentCommand */

CompleteJointsAttachmentCommand::CompleteJointsAttachmentCommand(const QList<JointData *> &jdlist,
                                                                 bool overwrite,
                                                                 QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), isOverwrite(overwrite), isOutput(isout)
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        stackedJoints.append( jd->getUuid() );
        orgFloorAttachments.append( jd->getAttachedFloorList() );
        orgFrameAttachments.append( jd->getAttachedFrameList() );
    }
    setText("complete Joints Attachment");
}

void CompleteJointsAttachmentCommand::redo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->completeJointsAttachment(jdlist, isOverwrite);
}

void CompleteJointsAttachmentCommand::undo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->changeJointsAttachment(jdlist, orgFloorAttachments, DATATYPE::TPFLOOR, false);
    FactoryUnifiedData::getInstance(isOutput)->changeJointsAttachment(jdlist, orgFrameAttachments, DATATYPE::TPFRAMEP, true);
}

/* CreateMemberCommand */

CreateMemberCommand::CreateMemberCommand(const QList<JointData *> &jdlist, DATATYPE dtype,
                                         const QString &sname, bool on,
                                         QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    stackedMember = QUuid();
    Q_FOREACH (JointData *jd, jdlist) stackedMemberJoints.append( jd->getUuid() );
    stackedMemberValues = MEMBERVALUES(dtype, sname);
    setText("create Member");
}

void CreateMemberCommand::redo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedMemberJoints);
    recentMember = FactoryUnifiedData::getInstance(isOutput)->createMember(jdlist, stackedMemberValues.dataType,
                                                            stackedMemberValues.sectionName, stackedMember);
    if ( recentMember == nullptr ) {
        stackedMember = QUuid();
        return;
    }
    if ( stackedMember.isNull() ) stackedMember = recentMember->getUuid();

    QList<MemberData *> changed_mdlist;
    if ( recentMember->dataType() == DATATYPE::TPCOLUMN ) {
        Q_FOREACH ( JointData *jd, recentMember->getJointList() ) {
            Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                if ( !mm->isGirderType() ) continue;
                if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
            }
        }
    } else if ( recentMember->isGirderType() ) {
        Q_FOREACH ( JointData *jd, recentMember->getInteriorJointList() ) {
            Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                if ( !mm->isGirderType() ) continue;
                if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
            }
        }
    }

    Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();

    if ( signalOn ) {
        FactoryUnifiedData::getInstance(isOutput)->signalMemberAppended( QList<MemberData *>() << recentMember );
        FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
        FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
    }
}

void CreateMemberCommand::undo()
{
    if ( stackedMember.isNull() ) return;
    MemberData *md = FactoryUnifiedData::getInstance(isOutput)->memberIDToPointer(stackedMember);
    FactoryUnifiedData::getInstance(isOutput)->removeMembers( QList<MemberData *>() << md, signalOn );
}

/* CreateMembersCommand */

CreateMembersCommand::CreateMembersCommand( const QList< QList<JointData *> > &jd_alllist,
                                            DATATYPE dtype, const QString &sname, bool on,
                                            QUndoCommand *parent,const UnifiedDataType isout )
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( QList<JointData *> jdlist, jd_alllist ) {
        QList<QUuid> idlist;
        Q_FOREACH ( JointData *jd, jdlist ) idlist.append( jd->getUuid() );
        stackedMemberJoints.append( idlist );
    }
    stackedMemberValues = MEMBERVALUES(dtype, sname);
    setText("create Members");
}

void CreateMembersCommand::redo()
{
    QList< QList<JointData *> > jd_alllist;
    Q_FOREACH ( QList<QUuid> idlist, stackedMemberJoints ) {
        QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(idlist);
        jd_alllist.append(jdlist);
    }
    QList<MemberData *> recentMembers = FactoryUnifiedData::getInstance(isOutput)->createMembers(jd_alllist,
                                                                                  stackedMemberValues.dataType,
                                                                                  stackedMemberValues.sectionName,
                                                                                  stackedMembers);

    QList<MemberData *> changed_mdlist;

    Q_FOREACH ( MemberData *md, recentMembers ) {
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();

    if ( signalOn ) {
        FactoryUnifiedData::getInstance(isOutput)->signalMemberAppended(recentMembers);
        FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
        FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
    }

    if ( stackedMembers.isEmpty() ) {
        for ( int i = recentMembers.count() - 1; i >= 0; i-- ) {
            MemberData *md = recentMembers.at(i);
            if ( md == NULL ) {
                stackedMemberJoints.removeAt(i);
            } else {
                stackedMembers.prepend( md->getUuid() );
            }
        }
    }
}

void CreateMembersCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);
    FactoryUnifiedData::getInstance(isOutput)->removeMembers( mdlist, signalOn );
}

/* CreateMembersMixedCommand */

CreateMembersMixedCommand::CreateMembersMixedCommand(const QList< QList<JointData *> > &jd_alllist,
                                                     const QList< MEMBERVALUES > &values, bool on,
                                                     QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    for ( int i = 0; i < jd_alllist.count(); i++ ) {
        QList<JointData *> jdlist = jd_alllist.at(i);
        MEMBERVALUES mv = ( i < values.count() ) ? values.at(i) : MEMBERVALUES() ;
        QList<QUuid> idlist;
        Q_FOREACH ( JointData *jd, jdlist ) idlist.append( jd->getUuid() );
        stackedMemberJoints.append( idlist );
        stackedMemberValues.append( mv );
    }

    setText("create Members Mixed");
}

void CreateMembersMixedCommand::redo()
{
    QList<MemberData *> created_mdlist;
    QList<MemberData *> r_changed_mdlist;

    for ( int i = 0; i < stackedMemberJoints.count(); i++ ) {
        QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedMemberJoints.at(
                                                                                       i));
        MEMBERVALUES mv = stackedMemberValues.at(i);
        QUuid id = ( i < stackedMembers.count() ) ? stackedMembers.at(i) : QUuid() ;
        MemberData *md = FactoryUnifiedData::getInstance(isOutput)->createMember(jdlist, mv.dataType, mv.sectionName, id);
        created_mdlist.append(md);
        if ( md == NULL ) continue;
        md->copyValues(*FactoryUnifiedData::getInstance(isOutput), mv);
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !r_changed_mdlist.contains(mm) ) r_changed_mdlist.append(mm);
                }
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !r_changed_mdlist.contains(mm) ) r_changed_mdlist.append(mm);
                }
            }
        }
    }

    if ( stackedMembers.isEmpty() ) {
        for ( int i = created_mdlist.count() - 1; i >= 0; i-- ) {
            MemberData *md = created_mdlist.at(i);
            if ( md == NULL ) {
                stackedMemberJoints.removeAt(i);
                stackedMemberValues.removeAt(i);
                created_mdlist.removeAt(i);
            } else {
                stackedMembers.prepend( md->getUuid() );
            }
        }
    }


    QList<MemberData *> changed_mdlist;
    if ( !r_changed_mdlist.isEmpty() ) {
        Q_FOREACH (MemberData *md, r_changed_mdlist) {
            md->recalcLineLength();
            if ( !created_mdlist.contains(md) ) changed_mdlist.append(md);
        }
    }

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberAppended(created_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}

void CreateMembersMixedCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);
    FactoryUnifiedData::getInstance(isOutput)->removeMembers( mdlist, signalOn );
}

/* CopyMembersValuesCommand */

CopyMembersValuesCommand::CopyMembersValuesCommand(const QList<MemberData *> &mdlist,
                                                   const QList< QList<JointData *> > &jd_alllist,
                                                   const QList<MEMBERVALUES> &values,
                                                   bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    for ( int i = 0; i < mdlist.count(); i++ ) {
        MemberData *md = mdlist.at(i);
        stackedMembers.append( md->getUuid() );
        QList<QUuid> s_idlist;
        Q_FOREACH ( JointData *jd, md->getJointList() ) s_idlist.append( jd->getUuid() );
        sourceMemberJoints.append( s_idlist );
        sourceMemberValues.append( md->getMemberValues() );

        QList<JointData *> jdlist = ( i < values.count() ) ? jd_alllist.at(i) : QList<JointData *>() ;
        QList<QUuid> d_idlist;
        Q_FOREACH ( JointData *jd, jdlist ) d_idlist.append( jd->getUuid() );
        destMemberJoints.append( d_idlist );
        MEMBERVALUES mv = ( i < values.count() ) ? values.at(i) : MEMBERVALUES() ;
        destMemberValues.append( mv );
    }

    setText("copy Member Values");
}

void CopyMembersValuesCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    QList<JointData *> changed_jdlist;
    QList<MemberData *> changed_mdlist;

    for ( int i = 0; i < mdlist.count(); i++ ) {
        MemberData *md = mdlist.at(i);
        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            if ( !changed_jdlist.contains(jd) ) changed_jdlist.append(jd);
            if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                Q_FOREACH ( JointData *jd, md->getJointList() ) {
                    Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                        if ( !mm->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            } else if ( md->isGirderType() ) {
                if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                    Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                        if ( !mm->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            }
        }

        QList<JointData *> dest_joints = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(destMemberJoints.at(
                                                                                            i));
        md->replaceJointList(dest_joints);
        md->copyValues(*FactoryUnifiedData::getInstance(isOutput), destMemberValues.at(i));
        Q_FOREACH ( JointData *jd, dest_joints ) {
            if ( !changed_jdlist.contains(jd) ) changed_jdlist.append(jd);
            if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                Q_FOREACH ( JointData *jd, md->getJointList() ) {
                    Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                        if ( !mm->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            } else if ( md->isGirderType() ) {
                if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                    Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                        if ( !mm->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            }
        }
    }

    Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalJointChanged(changed_jdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}

void CopyMembersValuesCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    QList<JointData *> changed_jdlist;
    QList<MemberData *> changed_mdlist;

    for ( int i = 0; i < mdlist.count(); i++ ) {

        MemberData *md = mdlist.at(i);
        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            if ( !changed_jdlist.contains(jd) ) changed_jdlist.append(jd);
            if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                Q_FOREACH ( JointData *jj, md->getJointList() ) {
                    Q_FOREACH ( MemberData *mm, jj->getComposeMembers() ) {
                        if ( !mm->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            } else if ( md->isGirderType() ) {
                if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                Q_FOREACH ( JointData *jj, md->getInteriorJointList() ) {
                    Q_FOREACH ( MemberData *mm, jj->getComposeMembers() ) {
                        if ( !mm->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            }
        }

        QList<JointData *> dest_joints = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(
                                             sourceMemberJoints.at(i));
        md->replaceJointList(dest_joints);
        md->copyValues(*FactoryUnifiedData::getInstance(isOutput), sourceMemberValues.at(i));
        Q_FOREACH ( JointData *jd, dest_joints ) {
            if ( !changed_jdlist.contains(jd) ) changed_jdlist.append(jd);
            if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                Q_FOREACH ( JointData *jj, md->getJointList() ) {
                    Q_FOREACH ( MemberData *mm, jj->getComposeMembers() ) {
                        if ( !mm->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            } else if ( md->isGirderType() ) {
                if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                Q_FOREACH ( JointData *jj, md->getInteriorJointList() ) {
                    Q_FOREACH ( MemberData *mm, jj->getComposeMembers() ) {
                        if ( !mm->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            }
        }
    }

    Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalJointChanged(changed_jdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}

/* RemoveMembersCommand */

RemoveMembersCommand::RemoveMembersCommand(QList<MemberData *> &mdlist, bool on,
                                           QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        QUuid m_id = md->getUuid();
        stackedMembers.append( m_id );
        stackedMemberValues.append( md->getMemberValues() );
        QList<QUuid> j_idlist;
        Q_FOREACH ( JointData *jj, md->getJointList() )
            j_idlist.append( jj->getUuid() );
        stackedMemberJoints.append(j_idlist);
    }
    setText("remove Members");
}

void RemoveMembersCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);
    FactoryUnifiedData::getInstance(isOutput)->removeMembers(mdlist, signalOn);
}

void RemoveMembersCommand::undo()
{
    QList<MemberData *> created_mdlist;
    QList<MemberData *> changed_mdlist;

    for ( int i = 0; i < stackedMembers.count(); i++ ) {

        MemberData *md = FactoryUnifiedData::getInstance(isOutput)->memberIDToPointer( stackedMembers.at(i) );

        QList<JointData *> n_jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(
                                          stackedMemberJoints.at(i) );

        MEMBERVALUES values = stackedMemberValues.at(i);
        md = FactoryUnifiedData::getInstance(isOutput)->createMember(n_jdlist, values.dataType, values.sectionName,
                                                      stackedMembers.at(i));
        md->copyValues(*FactoryUnifiedData::getInstance(isOutput), values);
        created_mdlist.append(md);
    }

    Q_FOREACH (MemberData *md, created_mdlist) {
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jj, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jj->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jj, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *mm, jj->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberAppended(created_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}

/* InteriorMembersCommand */

InteriorMembersCommand::InteriorMembersCommand(const QList<MemberData *> &mdlist,
                                               const QList< QList<JointData *> > &jd_alllist,
                                               bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    for ( int i = 0; i < mdlist.count(); i++ ) {
        stackedMembers.append( mdlist.at(i)->getUuid() );
        QList<QUuid> j_idlist;
        Q_FOREACH ( JointData *jj, jd_alllist.at(i) )
            j_idlist.append( jj->getUuid() );
        stackedInteriorJoints.append(j_idlist);
    }
    setText("interior Members");
}

void InteriorMembersCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);
    QList<int> countList;
    Q_FOREACH ( MemberData *md, mdlist ) countList.append( md->jointCount() );

    QList<MemberData *> changed_mdlist;
    QList<JointData *> changed_jdlist;

    for ( int i = 0 ; i < mdlist.count() ; i++ ) {

        MemberData *md = mdlist.at(i);
        QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(
                                        stackedInteriorJoints.at(i) );

        if ( FactoryUnifiedData::getInstance(isOutput)->interiorMember(md, jdlist) ) {

            if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);

            if ( md->dataType() == DATATYPE::TPCOLUMN || md->isGirderType() ) {
                Q_FOREACH ( JointData *jd, jdlist ) {
                    Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                        if ( !mm->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            }

            Q_FOREACH (JointData *jd, jdlist)
                if ( !changed_jdlist.contains(jd) ) changed_jdlist.append(jd);
        }
    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH (MemberData *md, changed_mdlist) md->recalcLineLength();
    }

    // 内分できなかった部材をスタックから消去
    for ( int i = mdlist.count() - 1; i >= 0; i-- ) {
        if ( countList.at(i) == mdlist.at(i)->jointCount() ) {
            stackedMembers.removeAt(i);
            stackedInteriorJoints.removeAt(i);
        }
    }

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalJointChanged(changed_jdlist);
}

void InteriorMembersCommand::undo()
{
    QList<MemberData *> changed_mdlist;
    QList<JointData *> changed_jdlist;

    for ( int i = 0; i < stackedMembers.count(); i++ ) {
        MemberData *md = FactoryUnifiedData::getInstance(isOutput)->memberIDToPointer( stackedMembers.at(i) );
        QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(
                                        stackedInteriorJoints.at(i) );
        FactoryUnifiedData::getInstance(isOutput)->removeJointsOfMember(jdlist, md);
        changed_mdlist.append(md);
        Q_FOREACH (JointData *jd, jdlist) {
            if ( !changed_jdlist.contains(jd) ) changed_jdlist.append(jd);
        }
    }

    Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalJointChanged(changed_jdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
}

/* DivideMembersCommand */

DivideMembersCommand::DivideMembersCommand(const QList<MemberData *> &mdlist,
                                           const QList< QList<JointData *> > &jd_alllist,
                                           bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    for ( int i = 0; i < mdlist.count(); i++ ) {
        stackedMembers.append( mdlist.at(i)->getUuid() );
        QList<QUuid> j_idlist;
        Q_FOREACH ( JointData *jj, jd_alllist.at(i) )
            j_idlist.append( jj->getUuid() );
        stackedInteriorJoints.append(j_idlist);
    }
    setText("divide Members");
}

void DivideMembersCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    bool isFirstOperation = ( dividedMembers.isEmpty() );
    QList<int> removeIndex;
    QList<MemberData *> created_mdlist;
//    QList<MemberData*> changed_mdlist;
    QList<MemberData *> removed_mdlist;

    for ( int i = 0; i < mdlist.count(); i++ ) {

        MemberData *md = mdlist.at(i);
        QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedInteriorJoints.at(
                                                                                       i));

        if ( md->isClosedMember() ) {
            int preCount = md->jointCount();
            FactoryUnifiedData::getInstance(isOutput)->interiorMember( md, jdlist );
            if ( md->jointCount() == preCount ) {
                removeIndex.prepend(i);
//            }else{
//                if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
            }
            if ( isFirstOperation ) dividedMembers.append( QList<QUuid>() << md->getUuid() );
            continue;
        }
        /*
                if( md->dataType()==TPCOLUMN || md->isGirderType() ){
                    Q_FOREACH( JointData* jd, md->getJointList() ){
                        if( jdlist.contains(jd) ){ // 分割点に取り付く梁材のフェイス変更
                            Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                                if( mm==md ) continue;
                                if( !mm->isGirderType() ) continue;
                                if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                            }
                        }else if( md->dataType()==TPCOLUMN ){ // 柱内分点に取り付く梁材のフェイス変更
                            Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                                if( mm==md ) continue;
                                if( !mm->isGirderType() ) continue;
                                if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                            }
                        }
                    }
                }
        */
        removed_mdlist.append(md);

        QList<QUuid> i_mdlist = ( isFirstOperation ) ?  QList<QUuid>() << stackedMembers.at(i)
                                :  dividedMembers.at(i);
        QList<MemberData *> c_mdlist = FactoryUnifiedData::getInstance(isOutput)->divideMember(md, jdlist, i_mdlist);
        created_mdlist.append(c_mdlist);
        if ( isFirstOperation ) {
            i_mdlist.clear();
            Q_FOREACH ( MemberData *mm, c_mdlist ) i_mdlist.append( mm->getUuid() );
            dividedMembers.append(i_mdlist);
        }

    }
    /*
        if( !changed_mdlist.isEmpty() ){
            Q_FOREACH( MemberData* md, changed_mdlist ) md->recalcLineLength();
        }
    */
    if ( signalOn ) {
        FactoryUnifiedData::getInstance(isOutput)->signalMemberAppended(created_mdlist);
//        FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
        FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
    }

    if ( !removed_mdlist.isEmpty() ) {
        FactoryUnifiedData::getInstance(isOutput)->removeMembers( removed_mdlist, signalOn );
    }

    if ( !isFirstOperation ) return;

    // 内分できなかった部材をスタックから消去
    for ( int i = 0; i < removeIndex.count(); i++ ) {
        int idx = removeIndex.at(i);
        stackedMembers.removeAt(idx);
        stackedInteriorJoints.removeAt(idx);
        dividedMembers.removeAt(idx);
    }
}

void DivideMembersCommand::undo()
{

    QMultiHash<int, MemberData *> removed_mdtable;
    QList<MemberData *> changed_mdlist;
    QList<JointData *> changed_jdlist;

    for ( int i = 0; i < stackedMembers.count(); i++ ) {

        if ( dividedMembers.at(i).count() == 1 ) {

            MemberData *md = FactoryUnifiedData::getInstance(isOutput)->memberIDToPointer( stackedMembers.at(i) );
            QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(
                                            stackedInteriorJoints.at(i) );
            FactoryUnifiedData::getInstance(isOutput)->removeJointsOfMember(jdlist, md);
            if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
            Q_FOREACH ( JointData *jd, jdlist ) {
                if ( !changed_jdlist.contains(jd) ) changed_jdlist.append(jd);
            }

        } else {

            QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers( dividedMembers.at(
                                                                                              i) );

            MemberData *remain_md = mdlist.first();
            QList<MemberData *> removed_mdlist = mdlist;
            removed_mdlist.removeFirst();
            QList<JointData *> removed_jdlist;

            Q_FOREACH ( MemberData *md, removed_mdlist ) {
                JointData *ijd = md->iJoint();
                removed_jdlist.append(ijd);
                if ( !changed_jdlist.contains(ijd) ) changed_jdlist.append(ijd);
                removed_mdtable.insert(static_cast<int>(remain_md->dataType()), md);
            }

            FactoryUnifiedData::getInstance(isOutput)->uniteMembers(mdlist);

            if ( remain_md->dataType() == DATATYPE::TPCOLUMN || remain_md->isGirderType() ) {

                Q_FOREACH ( JointData *jd, remain_md->getInteriorJointList() ) {
                    Q_FOREACH ( MemberData *md, jd->getComposeMembers() ) {
                        if ( !md->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                    }
                }

                if ( remain_md->dataType() == DATATYPE::TPCOLUMN ) {
                    Q_FOREACH ( MemberData *md, remain_md->getComposeMembers() ) {
                        if ( remain_md == md ) continue;
                        if ( !md->isGirderType() ) continue;
                        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                    }
                }
            }

            // 内分点消去
            FactoryUnifiedData::getInstance(isOutput)->removeJointsOfMember(removed_jdlist, remain_md);
            if ( !changed_mdlist.contains(remain_md) ) changed_mdlist.append(remain_md);

        }
    }

    Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();

    if ( !signalOn ) return;

    Q_FOREACH ( int type, removed_mdtable.uniqueKeys() ) {
        FactoryUnifiedData::getInstance(isOutput)->signalMemberRemoved( removed_mdtable.values(type), static_cast<DATATYPE>(type) );
    }

    FactoryUnifiedData::getInstance(isOutput)->signalJointChanged(changed_jdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}


/* DivideMembersByInteriorJointsCommand */

DivideMembersByInteriorJoinsCommand::DivideMembersByInteriorJoinsCommand(
    const QList<MemberData *> &mdlist,
    bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    for ( int i = 0; i < mdlist.count(); i++ ) {
        stackedMembers.append( mdlist.at(i)->getUuid() );
    }
    setText("divide Members By InteriorJoints");
}

void DivideMembersByInteriorJoinsCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    bool isFirstOperation = ( dividedMembers.isEmpty() );
    QList<int> removeIndex;
    QList<MemberData *> created_mdlist; // UnifiedData::removeMembers内でcreateされる
//    QList<MemberData*> changed_mdlist;  // UnifiedData::removeMembers内でchangedされる
    QList<MemberData *> removed_mdlist;

    for ( int i = 0; i < mdlist.count(); i++ ) {

        MemberData *md = mdlist.at(i);
        QList<JointData *> in_jdlist = md->getInteriorJointList();
        if ( in_jdlist.isEmpty() ) {
            removeIndex.prepend(i);
            continue;
        }

        /*
                if( md->dataType()==TPCOLUMN || md->isGirderType() ){
                    Q_FOREACH( JointData* jd, md->getJointList() ){
                        if( in_jdlist.contains(jd) ){ // 分割点に取り付く梁材のフェイス変更
                            Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                                if( mm==md ) continue;
                                if( !mm->isGirderType() ) continue;
                                if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                            }
                        }else if( md->dataType()==TPCOLUMN ){ // 柱内分点に取り付く梁材のフェイス変更
                            Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                                if( mm==md ) continue;
                                if( !mm->isGirderType() ) continue;
                                if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                            }
                        }
                    }
                }
        */

        removed_mdlist.append(md);

        QList<QUuid> i_mdlist = ( isFirstOperation ) ?  QList<QUuid>() << stackedMembers.at(i)
                                :  dividedMembers.at(i);
        QList<MemberData *> c_mdlist = FactoryUnifiedData::getInstance(isOutput)->divideMemberByInteriorJoints(md,
                                                                                                in_jdlist, i_mdlist);
        created_mdlist.append(c_mdlist);
        if ( isFirstOperation ) {
            i_mdlist.clear();
            Q_FOREACH ( MemberData *mm, c_mdlist ) i_mdlist.append( mm->getUuid() );
            dividedMembers.append(i_mdlist);
        }
    }

//    if( !changed_mdlist.isEmpty() ){
//        Q_FOREACH( MemberData* md, changed_mdlist ) md->recalcLineLength();
//    }

    if ( signalOn ) {
        FactoryUnifiedData::getInstance(isOutput)->signalMemberAppended(created_mdlist);
//        FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
        FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
    }

    if ( !removed_mdlist.isEmpty() ) {
        FactoryUnifiedData::getInstance(isOutput)->removeMembers( removed_mdlist, signalOn );
    }

    if ( !isFirstOperation ) return;

    // 内分できなかった部材をスタックから消去
    for ( int i = 0; i < removeIndex.count(); i++ ) {
        stackedMembers.removeAt( removeIndex.at(i) );
    }
}

void DivideMembersByInteriorJoinsCommand::undo()
{
    QMultiHash<int, MemberData *> removed_mdtable;
    QList<MemberData *> changed_mdlist;
    QList<JointData *> changed_jdlist;

    for ( int i = 0; i < stackedMembers.count(); i++ ) {

        QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers( dividedMembers.at(
                                                                                          i) );

        MemberData *remain_md = mdlist.first();
        QList<MemberData *> removed_mdlist = mdlist;
        removed_mdlist.removeFirst();

        Q_FOREACH ( MemberData *md, removed_mdlist ) {
            JointData *ijd = md->iJoint();
            if ( !changed_jdlist.contains(ijd) ) changed_jdlist.append(ijd);
            removed_mdtable.insert(static_cast<int>(remain_md->dataType()), md);
        }

        FactoryUnifiedData::getInstance(isOutput)->uniteMembers(mdlist);

        if ( remain_md->dataType() == DATATYPE::TPCOLUMN || remain_md->isGirderType() ) {

            Q_FOREACH ( JointData *jd, remain_md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *md, jd->getComposeMembers() ) {
                    if ( !md->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                }
            }

            if ( remain_md->dataType() == DATATYPE::TPCOLUMN ) {
                Q_FOREACH ( MemberData *md, remain_md->getComposeMembers() ) {
                    if ( remain_md == md ) continue;
                    if ( !md->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                }
            }
        }

        if ( !changed_mdlist.contains(remain_md) ) changed_mdlist.append(remain_md);

    }

    Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();

    if ( !signalOn ) return;

    Q_FOREACH ( int type, removed_mdtable.uniqueKeys() ) {
        FactoryUnifiedData::getInstance(isOutput)->signalMemberRemoved( removed_mdtable.values(type), static_cast<DATATYPE>(type) );
    }

    FactoryUnifiedData::getInstance(isOutput)->signalJointChanged(changed_jdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}


/* UniteMembersCommand */

UniteMembersCommand::UniteMembersCommand(const QList<MemberData *> &mdlist,
                                         bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        stackedMembers.append( md->getUuid() );
        if ( md == mdlist.first() ) continue;
        stackedConnectJoints.append( md->iJoint()->getUuid() );
    }
    setText("unite Members");
}

void UniteMembersCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    MemberData *remain_md = mdlist.first();
    QList<MemberData *> removed_mdlist = mdlist;
    removed_mdlist.removeFirst();

    QList<MemberData *> changed_mdlist;
    QList<JointData *> changed_jdlist;

    Q_FOREACH ( MemberData *md, removed_mdlist ) {
        JointData *ijd = md->iJoint();
        if ( !changed_jdlist.contains(ijd) ) changed_jdlist.append(ijd);
    }

    FactoryUnifiedData::getInstance(isOutput)->uniteMembers(mdlist);

    if ( remain_md->dataType() == DATATYPE::TPCOLUMN || remain_md->isGirderType() ) {

        Q_FOREACH ( JointData *jd, remain_md->getInteriorJointList() ) {
            Q_FOREACH ( MemberData *md, jd->getComposeMembers() ) {
                if ( !md->isGirderType() ) continue;
                if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
            }
        }

        if ( remain_md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( MemberData *md, remain_md->getComposeMembers() ) {
                if ( remain_md == md ) continue;
                if ( !md->isGirderType() ) continue;
                if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
            }
        }
    }

    if ( !changed_mdlist.contains(remain_md) ) changed_mdlist.append(remain_md);

    Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberRemoved( removed_mdlist, remain_md->dataType() );
    FactoryUnifiedData::getInstance(isOutput)->signalJointChanged(changed_jdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();

}

void UniteMembersCommand::undo()
{
    MemberData *remain_md = FactoryUnifiedData::getInstance(isOutput)->memberIDToPointer(stackedMembers.first());
    QList<JointData *> in_jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedConnectJoints);

    QList<MemberData *> created_mdlist;
//    QList<MemberData*> changed_mdlist;

    /*
        if( remain_md->dataType()==TPCOLUMN || remain_md->isGirderType() ){
            Q_FOREACH( JointData* jd, remain_md->getJointList() ){
                if( in_jdlist.contains(jd) ){ // 分割点に取り付く梁材のフェイス変更
                    Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                        if( mm==remain_md ) continue;
                        if( !mm->isGirderType() ) continue;
                        if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }else if( remain_md->dataType()==TPCOLUMN ){ // 柱内分点に取り付く梁材のフェイス変更
                    Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                        if( mm==remain_md ) continue;
                        if( !mm->isGirderType() ) continue;
                        if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            }
        }
    */
    QList<MemberData *> c_mdlist = FactoryUnifiedData::getInstance(isOutput)->divideMemberByInteriorJoints(remain_md,
                                                                                            in_jdlist, stackedMembers);
    created_mdlist.append(c_mdlist);

    if ( signalOn ) {
        FactoryUnifiedData::getInstance(isOutput)->signalMemberAppended(created_mdlist);
//        FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
        FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
    }

    FactoryUnifiedData::getInstance(isOutput)->removeMembers( QList<MemberData *>() << remain_md, signalOn );
    /*
        if( !changed_mdlist.isEmpty() ){
            Q_FOREACH( MemberData* md, changed_mdlist ) md->recalcLineLength();
        }
    */
}

/* ReplaceMemberJointCommand */
ReplaceMemberJointCommand::ReplaceMemberJointCommand(MemberData *md, JointData *j1, JointData *j2,
                                                     bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    stackedMember = md->getUuid();
    orgJoint = j1->getUuid();
    replacedJoint = j2->getUuid();
    setText("replace Member Joint");
}

void ReplaceMemberJointCommand::redo()
{
    MemberData *md = FactoryUnifiedData::getInstance(isOutput)->memberIDToPointer(stackedMember);
    JointData *j1 = FactoryUnifiedData::getInstance(isOutput)->jointIDToPointer(orgJoint);
    JointData *j2 = FactoryUnifiedData::getInstance(isOutput)->jointIDToPointer(replacedJoint);
    replaceMemberJoint(md, j1, j2);
}

void ReplaceMemberJointCommand::undo()
{
    MemberData *md = FactoryUnifiedData::getInstance(isOutput)->memberIDToPointer(stackedMember);
    JointData *j1 = FactoryUnifiedData::getInstance(isOutput)->jointIDToPointer(replacedJoint);
    JointData *j2 = FactoryUnifiedData::getInstance(isOutput)->jointIDToPointer(orgJoint);
    replaceMemberJoint(md, j1, j2);
}

void ReplaceMemberJointCommand::replaceMemberJoint( MemberData *md, JointData *j1, JointData *j2 )
{
    md->replaceJoint(j1, j2);
    if ( signalOn ) FactoryUnifiedData::getInstance(isOutput)->signalJointChanged( QList<JointData *>() << j1 );

    QList<MemberData *> changed_mdlist;
    if ( md->dataType() == DATATYPE::TPCOLUMN || md->isGirderType() ) {

        Q_FOREACH ( MemberData *mm, j1->getComposeMembers() ) {
            if ( !mm->isGirderType() ) continue;
            if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
        }

        Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
            Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                if ( !mm->isGirderType() ) continue;
                if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
            }
        }

        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( MemberData *mm, md->getComposeMembers() ) {
                if ( md == mm ) continue;
                if ( !mm->isGirderType() ) continue;
                if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
            }
        }
    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH ( MemberData *mm, changed_mdlist ) mm->recalcLineLength();
    }

    if ( !signalOn ) return;
    if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
}

/* ReverseMemberJointCommand */
ReverseMemberJointCommand::ReverseMemberJointCommand(const QList<MemberData *> &mdlist,
                                                     bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( MemberData *md, mdlist ) stackedMembers.append( md->getUuid() );
    setText("reverse Member Joint");
}

void ReverseMemberJointCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);
    reverseMemberJoint(mdlist);
}

void ReverseMemberJointCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);
    reverseMemberJoint(mdlist);
}

void ReverseMemberJointCommand::reverseMemberJoint( const QList<MemberData *> &mdlist )
{
    QList<MemberData *> changed_mdlist;
    Q_FOREACH ( MemberData *md, mdlist ) {
        md->reverseJointIndex();
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();
    }

    if ( !signalOn ) return;

    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
}

/* ChangeMemberSectionCommand */
ChangeMemberSectionCommand::ChangeMemberSectionCommand(const QList<MemberData *> &mdlist,
                                                       const QString &sname,
                                                       bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), changedName(sname), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        stackedMembers.append( md->getUuid() );
        orgNames.append( md->sectionName() );
    }
    setText("change Member Section");
}

void ChangeMemberSectionCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    QList<MemberData *> changed_mdlist;
    Q_FOREACH ( MemberData *md, mdlist ) {
        md->setSectionName(*FactoryUnifiedData::getInstance(isOutput), changedName);
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH (MemberData *md, changed_mdlist) md->recalcLineLength();
    }

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}

void ChangeMemberSectionCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    QList<MemberData *> changed_mdlist;
    for ( int i = 0; i < mdlist.count(); i++ ) {
        MemberData *md = mdlist.at(i);
        QString sname = orgNames.at(i);
        md->setSectionName(*FactoryUnifiedData::getInstance(isOutput), sname);
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }

    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH (MemberData *md, changed_mdlist) md->recalcLineLength();
    }

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
    FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}

/* ChangeCodeAngleCommand */
ChangeCodeAngleCommand::ChangeCodeAngleCommand(const QList<MemberData *> &mdlist, qreal angle,
                                               bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), changedAngle(angle), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        stackedMembers.append( md->getUuid() );
        orgAngles.append( md->codeAngle() );
    }
    setText("change Code Angle");
}

void ChangeCodeAngleCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    QList<MemberData *> changed_mdlist;
    Q_FOREACH ( MemberData *md, mdlist ) {
        md->setCodeAngle( changedAngle );
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH (JointData *jd, md->getJointList()) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();
    }

    if ( !signalOn ) return;

    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
}

void ChangeCodeAngleCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    QList<MemberData *> changed_mdlist;
    for ( int i = 0; i < mdlist.count(); i++ ) {
        MemberData *md = mdlist.at(i);
        qreal angle = orgAngles.at(i);
        md->setCodeAngle(angle);
        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();
    }

    if ( !signalOn ) return;

    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
}

/* ChangeShiftValueCommand */
ChangeShiftValueCommand::ChangeShiftValueCommand(const QList<MemberData *> &mdlist,
                                                 bool y_chg, qreal shy, bool z_chg, qreal shz,
                                                 bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), changedShiftY(shy), changedShiftZ(shz), isChangeY(y_chg), isChangeZ(z_chg), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        stackedMembers.append( md->getUuid() );
        orgShiftY.append( md->getShiftY() );
        orgShiftZ.append( md->getShiftZ() );
    }
    setText("change Shift Value");
}

void ChangeShiftValueCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( isChangeY ) md->setShiftY(changedShiftY);
        if ( isChangeZ ) md->setShiftZ(changedShiftZ);
    }

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(mdlist);
}

void ChangeShiftValueCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    for ( int i = 0; i < mdlist.count(); i++ ) {
        MemberData *md = mdlist.at(i);
        qreal shy = orgShiftY.at(i);
        qreal shz = orgShiftZ.at(i);

        if ( isChangeY ) md->setShiftY(shy);
        if ( isChangeZ ) md->setShiftZ(shz);
    }

    if ( !signalOn ) return;

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(mdlist);
}

/* ChangeFaceCommand */
ChangeFaceCommand::ChangeFaceCommand(const QList<MemberData *> &mdlist,
                                     bool ir_chg, int i_id, bool il_chg, qreal i_value,
                                     bool jr_chg, int j_id, bool jl_chg, qreal j_value,
                                     bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), changedIRule(i_id), changedILength(i_value), changedJRule(j_id), changedJLength(j_value),
      isChangeIRule(ir_chg), isChangeILength(il_chg), isChangeJRule(jr_chg), isChangeJLength(jl_chg),
      signalOn(on), isOutput(isout)
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        stackedMembers.append( md->getUuid() );
        orgIRules.append( (int)md->getIFaceRule() );
        orgILength.append( md->getIFace() );
        orgJRules.append( (int)md->getJFaceRule() );
        orgJLength.append( md->getJFace() );
    }
    setText("change Code Angle");
}

void ChangeFaceCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    QList<MemberData *> changed_mdlist;
    Q_FOREACH ( MemberData *md, mdlist ) {
        md->setFaceValues(isChangeIRule, changedIRule, isChangeILength, changedILength,
                          isChangeJRule, changedJRule, isChangeJLength, changedJLength);

        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH (JointData *jd, md->getJointList()) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH (MemberData *md, changed_mdlist) md->recalcLineLength();
    }

    if ( !signalOn ) return;

    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
}

void ChangeFaceCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    QList<MemberData *> changed_mdlist;
    for ( int i = 0; i < mdlist.count(); i++ ) {
        MemberData *md = mdlist.at(i);
        int i_id = orgIRules.at(i);
        qreal i_value = orgILength.at(i);
        int j_id = orgJRules.at(i);
        qreal j_value = orgJLength.at(i);

        md->setFaceValues(isChangeIRule, i_id, isChangeILength, i_value,
                          isChangeJRule, j_id, isChangeJLength, j_value);

        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH ( MemberData *md, changed_mdlist ) md->recalcLineLength();
    }

    if ( !signalOn ) return;

    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }

    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(changed_mdlist);
}


/* ChangeWallOpenCommand */
ChangeWallOpenCommand::ChangeWallOpenCommand(const QList<MemberData *> &mdlist, const QString &str,
                                             bool on, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), changedOpen(str), signalOn(on), isOutput(isout)
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( md->dataType() != DATATYPE::TPWALL && md->dataType() != DATATYPE::TPUWALL ) continue;
        stackedMembers.append( md->getUuid() );
        orgOpens.append( md->wallOpenName() );
    }
    setText("change Wall Open");
}

void ChangeWallOpenCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);

    Q_FOREACH (MemberData *md, mdlist) md->setWallOpen( changedOpen );
    if ( !signalOn ) return;
    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(mdlist);
}

void ChangeWallOpenCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);
    for ( int i = 0; i < mdlist.count(); i++ ) {
        MemberData *md = mdlist.at(i);
        md->setWallOpen( orgOpens.at(i) );
    }
    if ( !signalOn ) return;
    FactoryUnifiedData::getInstance(isOutput)->signalMemberChanged(mdlist);
}


/* SignalJointAppendedCommand */
SignalJointAppendedCommand::SignalJointAppendedCommand(const QList<JointData *> &jdlist,
                                                       QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), isOutput(isout)
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        stackedJoints.append( jd->getUuid() );
    }
    setText("signal Joint Appended");
}

void SignalJointAppendedCommand::redo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->signalJointAppended(jdlist);
}

void SignalJointAppendedCommand::undo()
{
    QList<JointData *> jdlist = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(stackedJoints);
    FactoryUnifiedData::getInstance(isOutput)->signalJointRemoved(jdlist);
}

/* SignalMemberAppendedCommand */
SignalMemberAppendedCommand::SignalMemberAppendedCommand(const QList<MemberData *> &mdlist,
                                                         QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), isOutput(isout)
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        stackedMembers.append( md->getUuid() );
    }
    myDataType = ( !mdlist.isEmpty() ) ? mdlist.first()->dataType() : DATATYPE::NODATATYPE;
    setText("signal Member Appended");
}

void SignalMemberAppendedCommand::redo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);
    FactoryUnifiedData::getInstance(isOutput)->signalMemberAppended(mdlist);
}

void SignalMemberAppendedCommand::undo()
{
    QList<MemberData *> mdlist = FactoryUnifiedData::getInstance(isOutput)->memberIDsToPointers(stackedMembers);
    FactoryUnifiedData::getInstance(isOutput)->signalMemberRemoved(mdlist, myDataType);
}

/* SignalElementCountChangedCommand */
SignalElementCountChangedCommand::SignalElementCountChangedCommand(bool ok, QUndoCommand *parent,const UnifiedDataType isout)
    : QUndoCommand(parent), isRedoOnly(ok), isOutput(isout)
{
    setText("signal Element Count Changed");
}

void SignalElementCountChangedCommand::redo()
{
    if ( isRedoOnly ) FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}

void SignalElementCountChangedCommand::undo()
{
    if ( !isRedoOnly ) FactoryUnifiedData::getInstance(isOutput)->signalElementCountChanged();
}
} // namespace post3dapp

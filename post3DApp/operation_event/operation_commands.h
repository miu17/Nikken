#ifndef OPERATIONCOMMANDS_H
#define OPERATIONCOMMANDS_H

#include <QUndoCommand>
#include "define_unifieddata.h"

namespace post3dapp{
class JointData;
class MemberData;

// 節点（１つ）生成
class CreateJointCommand : public QUndoCommand
{

public:

    CreateJointCommand(const GlobalPoint &gp, bool on = true,
                       QUndoCommand *parent = nullptr,UnifiedDataType isout = UnifiedDataType::Input);

    JointData *createdJoint() const
    {
        return recentJoint;
    }

    void undo() override;
    void redo() override;

private:

    const GlobalPoint orgPoint;
    JointData *recentJoint;
    QUuid stackedID;
    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 節点（複数同時）生成
class CreateJointsCommand : public QUndoCommand
{

public:

    CreateJointsCommand(const QList<GlobalPoint> &gplist, bool on = true,
                        QUndoCommand *parent = nullptr,const UnifiedDataType isout=UnifiedDataType::Input);

    QList<JointData *> createdJoints() const
    {
        return recentJoints;
    }

    void undo() override;
    void redo() override;

private:

    const QList<GlobalPoint> orgPoints;
    QList<JointData *> recentJoints;
    QList<QUuid> stackedIDs;
    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 節点（複数同時）削除
class RemoveJointsCommand : public QUndoCommand
{

public:

    RemoveJointsCommand(const QList<JointData *> &, bool on = true,
                        QUndoCommand *parent = nullptr , const UnifiedDataType isout= UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedJoints;
    QList<GlobalPoint> stackedPoints;

    QList<QUuid> stackedMembers;
    QList<QList<QUuid> > stackedMemberJoints;
    QList<MEMBERVALUES> stackedMemberValues;

    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 節点（複数同時）変更
class ChangeJointsCommand : public QUndoCommand
{

public:

    ChangeJointsCommand(QList<JointData *> &, const QList<GlobalPoint> &, bool on = true,
                        QUndoCommand *parent = nullptr,const UnifiedDataType isout = UnifiedDataType::Input);

    bool isSuccessfullyChanged() const
    {
        return isSuccess;
    }

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedJoints;
    QList<GlobalPoint> orgPoints;
    const QList<GlobalPoint> changedPoints;
    bool isSuccess;

    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 節点（複数同時）の属性変更
class ChangeJointsAttachmentCommand : public QUndoCommand
{

public:

    ChangeJointsAttachmentCommand(QList<JointData *> &, const QList<QUuid> &, DATATYPE, bool on = true,
                                  QUndoCommand *parent = nullptr,const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedJoints;
    QList<QList<QUuid> > orgAttachments;
    const QList<QUuid> changedAttachments;

    const DATATYPE targetType;
    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 節点（複数同時）の属性追加
class AppendJointsAttachmentCommand : public QUndoCommand
{

public:

    AppendJointsAttachmentCommand(QList<JointData *> &, const QList<QUuid> &, DATATYPE, bool on = true,
                                  QUndoCommand *parent = nullptr,const UnifiedDataType isout=UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedJoints;
    QList<QList<QUuid> > orgAttachments;
    const QList<QUuid> appendAttachments;

    const DATATYPE targetType;
    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 節点（複数同時）の属性補完
class CompleteJointsAttachmentCommand : public QUndoCommand
{

public:

    CompleteJointsAttachmentCommand(const QList<JointData *> &, bool overwrite = true,
                                    QUndoCommand *parent = nullptr,const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedJoints;
    QList<QList<QUuid> > orgFloorAttachments;
    QList<QList<QUuid> > orgFrameAttachments;

    const bool isOverwrite;
    const UnifiedDataType isOutput;

};






// 部材・荷重（１つ）生成
class CreateMemberCommand : public QUndoCommand
{

public:

    CreateMemberCommand(const QList<JointData *> &, DATATYPE, const QString &, bool on = true,
                        QUndoCommand *parent = nullptr,const UnifiedDataType isout = UnifiedDataType::Input);

    MemberData *createdMember() const
    {
        return recentMember;
    }

    void undo() override;
    void redo() override;

private:

    QUuid stackedMember;
    QList<QUuid> stackedMemberJoints;
    MEMBERVALUES stackedMemberValues;
    MemberData *recentMember;
    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重（複数）生成
class CreateMembersCommand : public QUndoCommand
{

public:

    CreateMembersCommand(const QList< QList<JointData *> > &, DATATYPE, const QString &, bool on = true,
                         QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<QList<QUuid> > stackedMemberJoints;
    MEMBERVALUES stackedMemberValues;
    const bool signalOn;
    const UnifiedDataType isOutput;
};

// 部材・荷重（複数）生成
class CreateMembersMixedCommand : public QUndoCommand
{

public:

    CreateMembersMixedCommand(const QList< QList<JointData *> > &, const QList<MEMBERVALUES> &,
                              bool on = true,
                              QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<QList<QUuid> > stackedMemberJoints;
    QList<MEMBERVALUES> stackedMemberValues;
    const bool signalOn;
    const UnifiedDataType isOutput;
};

// 部材・荷重（複数同時）削除
class RemoveMembersCommand : public QUndoCommand
{

public:

    RemoveMembersCommand(QList<MemberData *> &, bool on = true,
                         QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

//    QList<QUuid> stackedJoints;
//    QList<GlobalPoint> stackedPoints;

    QList<QUuid> stackedMembers;
    QList<QList<QUuid> > stackedMemberJoints;
    QList<MEMBERVALUES> stackedMemberValues;

    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重の諸元を入れ替え
class CopyMembersValuesCommand : public QUndoCommand
{

public:

    CopyMembersValuesCommand(const QList<MemberData *> &,
                             const QList< QList<JointData *> > &, const QList<MEMBERVALUES> &,
                             bool on = true, QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<QList<QUuid> > sourceMemberJoints;
    QList<MEMBERVALUES> sourceMemberValues;
    QList<QList<QUuid> > destMemberJoints;
    QList<MEMBERVALUES> destMemberValues;
    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重の内分
class InteriorMembersCommand : public QUndoCommand
{

public:

    InteriorMembersCommand(const QList<MemberData *> &, const QList< QList<JointData *> > &,
                           bool on = true,
                           QUndoCommand *parent = nullptr,const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<QList<QUuid> > stackedInteriorJoints;

    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重の分割
class DivideMembersCommand : public QUndoCommand
{

public:

    DivideMembersCommand(const QList<MemberData *> &, const QList< QList<JointData *> > &,
                         bool on = true,
                         QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<QList<QUuid> > stackedInteriorJoints;
    QList<QList<QUuid> > dividedMembers;

    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重の既存内分点による分割
class DivideMembersByInteriorJoinsCommand : public QUndoCommand
{

public:

    DivideMembersByInteriorJoinsCommand(const QList<MemberData *> &, bool on = true,
                                        QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<QList<QUuid> > dividedMembers;

    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重の一本化
class UniteMembersCommand : public QUndoCommand
{

public:

    UniteMembersCommand(const QList<MemberData *> &, bool on = true,
                        QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<QUuid> stackedConnectJoints;


    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重の従属節点の入れ替え（1点）
class ReplaceMemberJointCommand : public QUndoCommand
{

public:

    ReplaceMemberJointCommand(MemberData *, JointData *, JointData *, bool on = true,
                              QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QUuid stackedMember;
    QUuid orgJoint;
    QUuid replacedJoint;
    const bool signalOn;
    const UnifiedDataType isOutput;

    void replaceMemberJoint(MemberData *, JointData *, JointData *);

};

// 部材・荷重の従属節点のi->j入れ替え
class ReverseMemberJointCommand : public QUndoCommand
{

public:

    ReverseMemberJointCommand(const QList<MemberData *> &, bool on = true,
                              QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    const bool signalOn;
    const UnifiedDataType isOutput;

    void reverseMemberJoint(const QList<MemberData *> &);
};

// 部材・荷重の断面変更
class ChangeMemberSectionCommand : public QUndoCommand
{

public:

    ChangeMemberSectionCommand(const QList<MemberData *> &, const QString &, bool on = true,
                               QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QStringList orgNames;
    const QString changedName;

    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重のコードアングル変更
class ChangeCodeAngleCommand : public QUndoCommand
{

public:

    ChangeCodeAngleCommand(const QList<MemberData *> &, qreal, bool on = true,
                           QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<qreal> orgAngles;
    const qreal changedAngle;

    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重の寄り長さ変更
class ChangeShiftValueCommand : public QUndoCommand
{

public:

    ChangeShiftValueCommand(const QList<MemberData *> &, bool, qreal, bool, qreal, bool on = true,
                            QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<qreal> orgShiftY;
    QList<qreal> orgShiftZ;
    const qreal changedShiftY;
    const qreal changedShiftZ;
    const bool isChangeY;
    const bool isChangeZ;
    const bool signalOn;
    const UnifiedDataType isOutput;

};

// 部材・荷重のフェイス長さ変更
class ChangeFaceCommand : public QUndoCommand
{

public:

    ChangeFaceCommand(const QList<MemberData *> &, bool, int, bool, qreal,
                      bool, int, bool, qreal, bool on = true,
                      QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<int> orgIRules;
    QList<qreal> orgILength;
    QList<int> orgJRules;
    QList<qreal> orgJLength;

    const int changedIRule;
    const qreal changedILength;
    const int changedJRule;
    const qreal changedJLength;

    const bool isChangeIRule;
    const bool isChangeILength;
    const bool isChangeJRule;
    const bool isChangeJLength;

    const bool signalOn;
    const UnifiedDataType isOutput;

};


// 壁開口の追加・変更・削除
class ChangeWallOpenCommand : public QUndoCommand
{

public:

    ChangeWallOpenCommand(const QList<MemberData *> &, const QString &, bool on = true,
                          QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    QList<QString> orgOpens;
    const QString changedOpen;

    const bool signalOn;
    const UnifiedDataType isOutput;

};


// 節点の追加<->削除シグナル発行
class SignalJointAppendedCommand : public QUndoCommand
{

public:

    SignalJointAppendedCommand(const QList<JointData *> &, QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    const UnifiedDataType isOutput;
    QList<QUuid> stackedJoints;

};

// 部材・荷重の追加<->削除シグナル発行
class SignalMemberAppendedCommand : public QUndoCommand
{

public:

    SignalMemberAppendedCommand(const QList<MemberData *> &, QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    QList<QUuid> stackedMembers;
    DATATYPE myDataType;
    const UnifiedDataType isOutput;

};

//　節点・部材カウントの変更シグナル発行
class SignalElementCountChangedCommand : public QUndoCommand
{

public:

    SignalElementCountChangedCommand(bool ok, QUndoCommand *parent = nullptr, const UnifiedDataType isout = UnifiedDataType::Input);

    void undo() override;
    void redo() override;

private:

    const bool isRedoOnly;
    const UnifiedDataType isOutput;

};
} // namespace post3dapp
#endif

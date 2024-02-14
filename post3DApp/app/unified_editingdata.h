#ifndef UNIFIEDEDITINGDATA_H
#define UNIFIEDEDITINGDATA_H

#include <QFileInfo>
#include <QObject>

#include "define_settings.h"
#include "define_unifieddata.h"


class QUndoStack;
class QUndoCommand;
class QStyle;

namespace post3dapp{
class UnifiedData;
class JointData;
class MemberData;
class PersInterfaceScene;
class SpringData;
class UnifiedEditingData : public QObject
{
    Q_OBJECT
public:
    static UnifiedEditingData* getInstance(){
        if (myInstance == nullptr) myInstance = new UnifiedEditingData();
        return myInstance;
    }

    /* ファイル情報 */
    void setCurrentFile(const QString &);
    QString currentFilePath() const;  // ファイルの絶対パス
    QString currentFileBaseName() const;  // 拡張子を除くファイル名
    QString currentFileDir() const;  // ファイルの場所（絶対パス）

    /* スタイル */
    QStyle *getWindowXPStyle() const
    {
        return xpStyle;
    }
    QStyle *getTypicalColorStyle() const
    {
        return xpColorStyle;
    }

    /* 全体編集の制限 */
    void uploadLimitSelectTerm( LIMITSELECTTERM term );
    LIMITSELECTTERM currentLimitSelectTerm() const
    {
        return limitSelectTerm;
    }
    bool isSelectLimited() const
    {
        return limitSelectTerm.isSelectLimited;
    }
    bool isSelectLimitedFloor() const
    {
        return limitSelectTerm.isLimitFloor;
    }
    bool isSelectLimitedFrame() const
    {
        return limitSelectTerm.isLimitFrame;
    }
    bool isSelectLimitedDataType() const
    {
        return limitSelectTerm.isLimitDataType;
    }
    bool isSelectLimitedName() const
    {
        return limitSelectTerm.isLimitName;
    }

    /* プロパティウィジェット */
    void changeActiveWindowType(WINTYPE type);
    void changeActiveMdiWindowType(MDITYPE type);
    void changeActiveElements(const QList<JointData *> &, const QList<MemberData *> &);
    bool isPropertyEnable() const;
    bool isNoActiveElements() const;
    QList<JointData *> getActiveJoints() const;
    QList<MemberData *> getActiveMembers() const;

    /* メッセージ */
    void clearLogMessage();
    QString getCurrentLogMessage();

    /* UndoStack関連 */
    void pushUndoCommand(QUndoCommand *);
    void clearUndoStack();
    void beginUndoStackMacro(const QString &);
    void endUndoStackMacro();

    /* 伏図、軸組図オペレーションにおける節点のコピーバッファ */
    void setCopyBuffer( const QList<QPointF> &,
                        const QList<JointData *> &jdlist = QList<JointData *>(),
                        const QList<MemberData *> &mdlist = QList<MemberData *>() );
    bool existJointCopyBuffer() const;
    QList<QPointF> getPointsCopyBuffer() const;
    QList<JointData *> getJointsCopyBuffer(UnifiedData*) const;
    QList<MemberData *> getMembersCopyBuffer(UnifiedData*) const;

    // オペレーション

    ELEMENTGROUP elementGroup(ELEMENTTYPE) const;
    ELEMENTTYPE currentElement() const;
    ADDEDITMODE currentAddEditMode() const;
    SNAPSTATE currentSnapState() const;
    ADDMOVEACTION currentAddMoveAction() const;
    ADDMOVEDACTION currentAddMovedAction() const;
    DELETEACTION currentDeleteAction() const;
    JOINTBATCHACTION currentJointBatchAction() const;
    TARGETACTION currentTargetAction() const;
    int currentBeamDivValue() const;
    int currentBeamPalValue() const;
    int currentBeamVerValue() const;
    int currentBeamExtValue() const;
    int currentJointBatchPitch() const;
    QString currentSectionName() const;

    void uploadSettingStatus(SNAPSTATE);
    void uploadSettingStatus(ADDMOVEACTION);
    void uploadSettingStatus(ADDMOVEDACTION);
    void uploadSettingStatus(DELETEACTION);
    void uploadSettingStatus(JOINTBATCHACTION, int);
    void uploadSettingStatus(TARGETACTION);
    void uploadSettingStatus(ADDEDITMODE, int);
    void uploadElementName(ELEMENTTYPE, const QString & );



public slots:
    /* UndoStack関連 */
    void signalCanRedoChanged(bool);
    void signalCanUndoChanged(bool);
    void executeRedoCommand();
    void executeUndoCommand();

    void changeCurrentOperation(int);
    void changeCurrentElement(int);
    void changeCurrentMode(int);
    void sendLogMessage(const QString &);
    bool existCopyBuffer(UnifiedData*);  // 不整合なBufferは削除　×const
    void clearCopyBuffer();

    /* 3Dパース描画の唯一用意されるシーン */
    void setSingularPersScene( PersInterfaceScene *scene );
    PersInterfaceScene *getSingularPersScene() const {return singularPersScene;}

    /* OpenGL MultiWindowのための親となる QGLWidget */
    void setSingularGLWidget( QGLWidget *widget );
    QGLWidget *getSingularGLWidget() const {return singularGLWidget;}


signals:
    void selectTermChanged(const LIMITSELECTTERM&);
    void activeContentsChanged();
    void activeMdiWindowTypeChanged(MDITYPE);
    void logMessageChanged(const QString &);
    /* UndoStack関連 */
    void canRedoChanged(bool);
    void canUndoChanged(bool);

    void currentSnapStateChanged(SNAPSTATE);
    void currentAddMoveActionChanged(ADDMOVEACTION);
    void currentAddMovedActionChanged(ADDMOVEDACTION);
    void currentDeleteActionChanged(DELETEACTION);
    void currentTargetActionChanged(TARGETACTION);
    void currentStatusChanged(const OPERATIONSETTING&, const QHash<ELEMENTTYPE, QString>&);


private:
    UnifiedEditingData();
    UnifiedEditingData(const UnifiedEditingData &);
    static UnifiedEditingData* myInstance;
    QStyle *xpStyle;
    QStyle *xpColorStyle;
    QFileInfo currentFileInfo;

    /* 全体編集の制限 */
    LIMITSELECTTERM limitSelectTerm;

    /* プロパティウィジェット */
    WINTYPE activeWindowType;
    MDITYPE activeMdiWindowType;
    QList<JointData *> activeJoints;
    QList<MemberData *> activeMembers;

    QStringList logMessage;
    QUndoStack *undoStack;

    /* オペレーション */
    OPERATIONSETTING operationSetting;
    QHash<ELEMENTTYPE, QString> currentNameTable;

    /*節点のコピーバッファ*/
    QList<QPointF> pointsBuffer;
    QList<QUuid> jointsBuffer;
    QList<QUuid> membersBuffer;

    /* */
    PersInterfaceScene *singularPersScene;
    QGLWidget *singularGLWidget;


};
} // namespace post3dapp

#endif // UNIFIEDEDITINGDATA_H

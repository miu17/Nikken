#include "unified_editingdata.h"

#include <QDateTime>
#include <QProxyStyle>
#include <QUndoStack>

#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "typicalcolor_style.h"
#include "unified_calcload.h"
#include "unified_inputdata.h"

namespace post3dapp{
UnifiedEditingData* UnifiedEditingData::myInstance = nullptr;
UnifiedEditingData::UnifiedEditingData()
{
    limitSelectTerm = LIMITSELECTTERM();
    xpStyle = new QProxyStyle();
    xpColorStyle = new TypicalColorStyle();
    undoStack = new QUndoStack(this);

    connect(undoStack, &QUndoStack::canRedoChanged,
            this, &UnifiedEditingData::signalCanRedoChanged);
    connect(undoStack, &QUndoStack::canUndoChanged,
            this, &UnifiedEditingData::signalCanUndoChanged);
    connect(UnifiedInputData::getInInstance(), &UnifiedData::signalSendLogMessage,
            this, &UnifiedEditingData::sendLogMessage);
    connect(UnifiedInputData::getInInstance(), &UnifiedData::existCopyBuffer,
            this, &UnifiedEditingData::existCopyBuffer);
    connect(UnifiedInputData::getInInstance(), &UnifiedInputData::clearCopyBuffer,
            this, &UnifiedEditingData::clearCopyBuffer);

    operationSetting = OPERATIONSETTING();

    currentNameTable.insert(ELCOLUMN, QString("(dummy)"));
    currentNameTable.insert(ELGIRDER, QString("(dummy)"));
    currentNameTable.insert(ELBRACE, QString("(dummy)"));
    currentNameTable.insert(ELBEAM, QString("(dummy)"));
    currentNameTable.insert(ELSLAB, QString("(dummy)"));
    currentNameTable.insert(ELWALL, QString("(dummy)"));
    currentNameTable.insert(ELUWALL, QString("(dummy)"));
    currentNameTable.insert(ELDAMPER, QString("(dummy)"));
    currentNameTable.insert(ELISO, QString("(dummy)"));
    currentNameTable.insert(ELJOINTLOAD, QString("(dummy)"));
    currentNameTable.insert(ELMEMBERLOAD, QString("(dummy)"));
    currentNameTable.insert(ELSLABLOAD, QString("(dummy)"));


    emit currentStatusChanged(operationSetting, currentNameTable);

    singularPersScene = NULL;
    singularGLWidget = NULL;
}

void UnifiedEditingData::setCurrentFile(const QString &filename)
{
    currentFileInfo = ( filename.isEmpty() ) ? QFileInfo() : QFileInfo(filename) ;
}

QString UnifiedEditingData::currentFilePath() const
{
    return ( currentFileInfo.exists() ) ? currentFileInfo.absoluteFilePath() : QString() ;
}

QString UnifiedEditingData::currentFileBaseName() const
{
    return ( currentFileInfo.exists() ) ? currentFileInfo.baseName() : QString() ;
}

QString UnifiedEditingData::currentFileDir() const
{
    if ( !currentFileInfo.exists() ) return QString();
    return currentFileInfo.absolutePath();
}


/* 全体編集の制限 */
void UnifiedEditingData::uploadLimitSelectTerm( LIMITSELECTTERM term )
{
    limitSelectTerm = term;
    emit selectTermChanged(limitSelectTerm);
}

/* プロパティウィジェット */

void UnifiedEditingData::changeActiveWindowType(WINTYPE type)
{
    activeWindowType = type;
    emit activeContentsChanged();
}
void UnifiedEditingData::changeActiveMdiWindowType(MDITYPE type){
    activeMdiWindowType = type;
    emit activeMdiWindowTypeChanged(type);
}
void UnifiedEditingData::changeActiveElements(const QList<JointData *> &joints,
                                       const QList<MemberData *> &members)
{
    activeJoints = joints;
    activeMembers = members;
    emit activeContentsChanged();
}

bool UnifiedEditingData::isPropertyEnable() const
{
    return ( activeWindowType == WINTYPE::WINPLAN || activeWindowType == WINTYPE::WINELEVATION
             || activeWindowType == WINTYPE::WINPERS );
}

bool UnifiedEditingData::isNoActiveElements() const
{
    return ( activeJoints.count() + activeMembers.count() == 0 ) ;
}

QList<JointData *> UnifiedEditingData::getActiveJoints() const
{
    return activeJoints;
}

QList<MemberData *> UnifiedEditingData::getActiveMembers() const
{
    return activeMembers;
}

/* ログメッセーッジ */

void UnifiedEditingData::clearLogMessage()
{
    logMessage.clear();
    emit logMessageChanged( QString() );
}

QString UnifiedEditingData::getCurrentLogMessage()
{
    return logMessage.join("\n");
}

void UnifiedEditingData::sendLogMessage(const QString &str)
{
    QString line = QDateTime::currentDateTime().toString("<hh:mm:ss> ");
    line += str;
    logMessage.append( line );
    if ( logMessage.count() > 1000 ) logMessage.removeFirst();
    emit logMessageChanged( logMessage.join("\n") );
}

/* UndoStack関連 */

void UnifiedEditingData::pushUndoCommand(QUndoCommand *cmd)
{
    undoStack->push(cmd);
}

void UnifiedEditingData::clearUndoStack()
{
    undoStack->clear();
}

void UnifiedEditingData::beginUndoStackMacro(const QString &text)
{
    undoStack->beginMacro(text);
}

void UnifiedEditingData::endUndoStackMacro()
{
    undoStack->endMacro();
}

void UnifiedEditingData::executeRedoCommand()
{
    undoStack->redo();
}

void UnifiedEditingData::executeUndoCommand()
{
    undoStack->undo();
}

void UnifiedEditingData::signalCanRedoChanged(bool canRedo)
{
    emit canRedoChanged(canRedo);
}

void UnifiedEditingData::signalCanUndoChanged(bool canUndo)
{
    emit canUndoChanged(canUndo);
}

void UnifiedEditingData::changeCurrentOperation(int op)
{
    operationSetting.operationMode = static_cast<OPERATIONMODE>(op);
    emit currentStatusChanged(operationSetting, currentNameTable);
}

void UnifiedEditingData::changeCurrentElement(int el)
{
    operationSetting.elementType = static_cast<ELEMENTTYPE>(el + 1);
    emit currentStatusChanged(operationSetting, currentNameTable);
}

void UnifiedEditingData::changeCurrentMode(int md)
{
    operationSetting.addEditMode = static_cast<ADDEDITMODE>(md);
    emit currentStatusChanged(operationSetting, currentNameTable);
}

ELEMENTGROUP UnifiedEditingData::elementGroup(ELEMENTTYPE el_type) const
{
    if ( el_type == NOELEMENT ) {
        return ELGNONE;
    } else if ( el_type == ELJOINT || el_type == ELJOINTLOAD ) {
        return ELGPOINT;
    } else if ( el_type == ELCOLUMN || el_type == ELGIRDER || el_type == ELBRACE ||
                el_type == ELBEAM || el_type == ELDAMPER || el_type == ELISO || el_type == ELMEMBERLOAD ) {
        return ELGLINE;
    } else if ( el_type == ELSLAB || el_type == ELSLABLOAD ) {
        return ELGPLANE;
    } else if (el_type == ELWALL || el_type == ELUWALL ){
        return ELGWALL;
    } else if ( el_type == ALLELEMENT ) {
        return ELGALL;
    }
    return ELGNONE;
}

ELEMENTTYPE UnifiedEditingData::currentElement() const
{
    if ( operationSetting.operationMode == OPALLEDIT ) {
        return ALLELEMENT;
    }
    if ( operationSetting.operationMode == OPALLCHANGE ) {
        return NOELEMENT;
    }
    return operationSetting.elementType;
}

ADDEDITMODE UnifiedEditingData::currentAddEditMode() const
{
    if ( operationSetting.operationMode == OPALLEDIT ) {
        return EDITMODE;
    }
    if ( operationSetting.operationMode == OPALLCHANGE ) {
        return DISABLEMODE;
    }
    return operationSetting.addEditMode;
}

SNAPSTATE UnifiedEditingData::currentSnapState() const
{
    return operationSetting.snapState;
}

ADDMOVEACTION UnifiedEditingData::currentAddMoveAction() const
{
    return operationSetting.addMoveAction;
}

ADDMOVEDACTION UnifiedEditingData::currentAddMovedAction() const
{
    return operationSetting.addMovedAction;
}

DELETEACTION UnifiedEditingData::currentDeleteAction() const
{
    return operationSetting.deleteAction;
}

JOINTBATCHACTION UnifiedEditingData::currentJointBatchAction() const
{
    return operationSetting.jointBatchAction;
}

TARGETACTION UnifiedEditingData::currentTargetAction() const
{
    return operationSetting.targetAction;
}

int UnifiedEditingData::currentBeamDivValue() const
{
    return operationSetting.beamDivSpinValue;
}

int UnifiedEditingData::currentBeamPalValue() const
{
    return operationSetting.beamPalSpinValue;
}

int UnifiedEditingData::currentBeamVerValue() const
{
    return operationSetting.beamVerSpinValue;
}

int UnifiedEditingData::currentBeamExtValue() const
{
    return operationSetting.beamExtSpinValue;
}

int UnifiedEditingData::currentJointBatchPitch() const
{
    return operationSetting.jointBatchSpinValue;
}

QString UnifiedEditingData::currentSectionName() const
{
    if ( !currentNameTable.contains(operationSetting.elementType) ) return QString();
    QString str = currentNameTable.value(operationSetting.elementType);
    return ( str == "(dummy)" ) ? QString() : str ;
}

void UnifiedEditingData::uploadSettingStatus(SNAPSTATE state)
{
    operationSetting.snapState = state;
    emit currentSnapStateChanged(state);
}

void UnifiedEditingData::uploadSettingStatus(ADDMOVEACTION act)
{
    operationSetting.addMoveAction = act;
    emit currentAddMoveActionChanged(act);
}

void UnifiedEditingData::uploadSettingStatus(ADDMOVEDACTION act)
{
    operationSetting.addMovedAction = act;
    emit currentAddMovedActionChanged(act);
}

void UnifiedEditingData::uploadSettingStatus(DELETEACTION act)
{
    operationSetting.deleteAction = act;
    emit currentDeleteActionChanged(act);
}

void UnifiedEditingData::uploadSettingStatus(JOINTBATCHACTION act, int val)
{
    operationSetting.jointBatchAction = act;
    operationSetting.jointBatchSpinValue = val;
}

void UnifiedEditingData::uploadSettingStatus(TARGETACTION act)
{
    operationSetting.targetAction = act;
    emit currentTargetActionChanged(act);
}

void UnifiedEditingData::uploadSettingStatus(ADDEDITMODE mode, int val)
{
    if ( mode == DIVIDEMODE ) {
        operationSetting.beamDivSpinValue = val;
    } else if ( mode == PARALLELMODE ) {
        operationSetting.beamPalSpinValue = val;
    } else if ( mode == VERTICALMODE ) {
        operationSetting.beamVerSpinValue = val;
    } else if ( mode == EXTENDMODE ) {
        operationSetting.beamExtSpinValue = val;
    }
}

void UnifiedEditingData::uploadElementName(ELEMENTTYPE el, const QString &name)
{
    currentNameTable.insert(el, name);
    emit currentStatusChanged(operationSetting, currentNameTable);
}

/* 伏図、軸組図オペレーションにおける節点のコピーバッファ */

void UnifiedEditingData::setCopyBuffer( const QList<QPointF> &plist,
                                 const QList<JointData *> &jdlist,
                                 const QList<MemberData *> &mdlist )
{
    pointsBuffer = plist;
    jointsBuffer.clear();
    Q_FOREACH ( JointData *jd, jdlist ) jointsBuffer.append( jd->getUuid() );
    membersBuffer.clear();
    Q_FOREACH ( MemberData *md, mdlist ) membersBuffer.append( md->getUuid() );
}

bool UnifiedEditingData::existJointCopyBuffer() const
{
    return ( !pointsBuffer.isEmpty() );
}

bool UnifiedEditingData::existCopyBuffer(UnifiedData* unified)
{
    if ( jointsBuffer.isEmpty() ) {
        clearCopyBuffer();
        return false;
    }
    if ( jointsBuffer.count() != pointsBuffer.count() ) {
        clearCopyBuffer();
        return false;
    }
    bool isOk = true;
    Q_FOREACH ( QUuid id, jointsBuffer ) {
        if ( unified->jointIDToPointer(id) == nullptr ) {
            isOk = false;
            break;
        }
    }
    if ( !isOk ) {
        clearCopyBuffer();
        return false;
    }
    Q_FOREACH ( QUuid id, membersBuffer ) {
        if ( unified->memberIDToPointer(id) == nullptr ) {
            isOk = false;
            break;
        }
    }
    if ( !isOk ) {
        clearCopyBuffer();
        return false;
    }
    return true;
}

QList<QPointF> UnifiedEditingData::getPointsCopyBuffer() const
{
    return pointsBuffer;
}

QList<JointData *> UnifiedEditingData::getJointsCopyBuffer(UnifiedData* unified) const
{
    return unified->jointIDsToPointers( jointsBuffer );
}

QList<MemberData *> UnifiedEditingData::getMembersCopyBuffer(UnifiedData* unified) const
{
    return unified->memberIDsToPointers( membersBuffer );
}

void UnifiedEditingData::clearCopyBuffer()
{
    pointsBuffer.clear();
    jointsBuffer.clear();
    membersBuffer.clear();
}

void UnifiedEditingData::setSingularPersScene(PersInterfaceScene *scene)
{
    singularPersScene = scene;
}

void UnifiedEditingData::setSingularGLWidget(QGLWidget *widget)
{
    singularGLWidget = widget;
}



} // namespace post3dapp

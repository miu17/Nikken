#ifndef PERSINTERFACEWIDGET_H
#define PERSINTERFACEWIDGET_H

#include "define_glsetting.h"
#include "interface_widget.h"

class QComboBox;
class QToolButton;

namespace post3dapp{
enum ELEMENTTYPE;
enum class DATATYPE;
class JointData;
class MemberData;
class ManagePersActions;
class PersInterfaceSettingDialog;
class PersInterfaceGLViewer;

class PersInterfaceWidget : public InterfaceWidget
{
    Q_OBJECT

public:

    PersInterfaceWidget(QWidget *parent = 0);
    ~PersInterfaceWidget();

    void sendActiveStatusToScene();

    // glViewから移植
    bool changeJointsGlobalXYZ(bool, bool, bool, qreal, qreal, qreal);
    bool changJointsGlobalDelta(bool, bool, bool, qreal, qreal, qreal);

    void clearFloorAttachment();
    bool appendFloorAttachment(const QString &);
    void clearFrameAttachment();
    bool appendFrameAttachment(const QString &);

    PersInterfaceGLViewer* getView(){return myGLView;}
private slots:

    void slotSavePixmap();
    void slotShowControlDialog(bool);
    void slotChangeViewSettings();
    void slotControlButtonUnChecked();

    // glViewから移植
    void slotShowAutoAttachDialog();
    void slotShowChangeAngleDialog();
    void slotShowChangeFaceDialog();
    void slotShowChangeJointDialog();
    void slotChangeWallOpen();
    void slotChangeSectionDialog();
    void slotShowChangeShiftDialog();
    void slotFindItems();
    void slotDeleteItems();
    void slotDeleteIndependentJoints();
    void slotReverseMemberJoint();
    void slotDeleteWallOpen();

    void slotSetActionStatus(const QList<DATATYPE>&);
    void slotImportCurrentStatus();
    void slotSendSelectedItems();
    void slotSelectPoints(qglviewer::Camera *camera, const QRect &rect, PERSSELECTMODE mode);
    void slotAppendSelectedMemberID(GLuint id);
protected:

private:

    PersInterfaceGLViewer *myGLView;
    QComboBox *dirCombobox;
    PersInterfaceSettingDialog *controlDialog;
    QToolButton *controlButton;
    ManagePersActions *manageActions;

    void createSceneView();
    void createButtons();
    void warningInvalidValue();
    void showInformationMessage(const QString &);

    QString changeSelectedMembersSection(DATATYPE, const QString&);
    void removeSelectedJoint(JointData *);
    void appendSelectedJoint(JointData *);
    void appendSelectedMember(MemberData *);
    bool isJointOkToSelectLimited( JointData *jd ) const;
    bool isMemberOkToSelectLimited( MemberData *md ) const;
    ELEMENTTYPE dataTypeToElementType(DATATYPE dtype) const;

};

} // namespace post3dapp
#endif

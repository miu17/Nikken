#ifndef CHANGEPLANEJOINTDIALOG_H
#define CHANGEPLANEJOINTDIALOG_H

#include <QDialog>

class QButtonGroup;
class QLabel;
class QLineEdit;
namespace post3dapp{
class AbstractEventOperation;

struct PLANEJOINTVALUES {
    explicit PLANEJOINTVALUES(
        const QString &_gx = QString(),
        const QString &_gy = QString(),
        const QString &_gz = QString(),
        const QString &_gdx = QString(),
        const QString &_gdy = QString(),
        const QString &_gdz = QString(),
        const QString &_lx = QString(),
        const QString &_ly = QString(),
        const QString &_lz = QString(),
        const QString &_ldx = QString(),
        const QString &_ldy = QString(),
        const QString &_ldz = QString(),
        const QString &_fl = QString(),
        const QString &_fr = QString() )
        : gX(_gx), gY(_gy), gZ(_gz), gdX(_gdx), gdY(_gdy), gdZ(_gdz),
          lX(_lx), lY(_ly), lZ(_lz), ldX(_ldx), ldY(_ldy), ldZ(_ldz),
          floors(_fl), frames(_fr) { }

    QString gX;
    QString gY;
    QString gZ;
    QString gdX;
    QString gdY;
    QString gdZ;
    QString lX;
    QString lY;
    QString lZ;
    QString ldX;
    QString ldY;
    QString ldZ;
    QString floors;
    QString frames;
};

class ChangePlaneJointDialog : public QDialog
{
    Q_OBJECT

public:

    ChangePlaneJointDialog(AbstractEventOperation *, QWidget *parent = 0);

    void setValues(const PLANEJOINTVALUES &);

public slots:

    void slotChangeGlobalXYZ();
    void slotChangeGlobalDXYZ();
    void slotChangeLocalXYZ();
    void slotChangeLocalDXYZ();
    void slotChangeFloorFrame();

    void checkGlobalXYZModified();
    void checkGlobalDXYZModified();
    void checkLocalXYZModified();
    void checkLocalDXYZModified();
    void checkFloorFrameModified();

signals:

private:

    void createLayout();

    AbstractEventOperation *parentOperation;
    PLANEJOINTVALUES loadValues;

    QLineEdit *globalXEdit;
    QLineEdit *globalYEdit;
    QLineEdit *globalZEdit;
    QLineEdit *globalDXEdit;
    QLineEdit *globalDYEdit;
    QLineEdit *globalDZEdit;

    QButtonGroup *changeAttachButton;
    QLineEdit *localXEdit;
    QLineEdit *localYEdit;
    QLineEdit *localZEdit;
    QLineEdit *localDXEdit;
    QLineEdit *localDYEdit;
    QLineEdit *localDZEdit;
    QLineEdit *floorEdit;
    QLineEdit *frameEdit;

    QPushButton *globalXYZButton;
    QPushButton *globalDeltaButton;
    QPushButton *localXYZButton;
    QPushButton *localDeltaButton;
    QPushButton *floorFrameButton;

    void warningInvalidValue();
    void warningDisableChangeValue();
    void warningDisableChangeFloorFrame();

};
} // namespace post3dapp
#endif

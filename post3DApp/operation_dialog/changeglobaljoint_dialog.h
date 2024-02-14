#ifndef CHANGEGLOBALJOINTDIALOG_H
#define CHANGEGLOBALJOINTDIALOG_H

#include <QDialog>

class QButtonGroup;
class QLabel;
class QLineEdit;
namespace post3dapp{
class PersInterfaceWidget;

struct GLOBALJOINTVALUES {
    explicit GLOBALJOINTVALUES(
        const QString &_gx = QString("0.000"),
        const QString &_gy = QString("0.000"),
        const QString &_gz = QString("0.000"),
        const QString &_gdx = QString("0.000"),
        const QString &_gdy = QString("0.000"),
        const QString &_gdz = QString("0.000"),
        const QString &_fl = QString(),
        const QString &_fr = QString() )
        : gX(_gx), gY(_gy), gZ(_gz), gdX(_gdx), gdY(_gdy), gdZ(_gdz),
          floors(_fl), frames(_fr) { }

    QString gX;
    QString gY;
    QString gZ;
    QString gdX;
    QString gdY;
    QString gdZ;
    QString floors;
    QString frames;
};

class ChangeGlobalJointDialog : public QDialog
{
    Q_OBJECT

public:

    ChangeGlobalJointDialog(const QString &, const QString &, const QString &, PersInterfaceWidget *);

public slots:

    void slotChangeGlobalXYZ();
    void slotChangeGlobalDXYZ();

    void slotClearFloorAttachment();
    void slotAppendFloorAttachment();
    void slotClearFrameAttachment();
    void slotAppendFrameAttachment();

    void checkGlobalXYZModified();
    void checkGlobalDXYZModified();
    void checkAppendFloorModified();
    void checkAppendFrameModified();

signals:

private:

    void createLayout();

    PersInterfaceWidget *parentWidget;
    GLOBALJOINTVALUES loadValues;

    QLineEdit *globalXEdit;
    QLineEdit *globalYEdit;
    QLineEdit *globalZEdit;
    QLineEdit *globalDXEdit;
    QLineEdit *globalDYEdit;
    QLineEdit *globalDZEdit;

    QPushButton *clearFloorButton;
    QPushButton *appendFloorButton;
    QPushButton *clearFrameButton;
    QPushButton *appendFrameButton;
    QLineEdit *floorEdit;
    QLineEdit *frameEdit;

    QPushButton *globalXYZButton;
    QPushButton *globalDeltaButton;

    void warningInvalidValue();
    void warningDisableChangeValue();
    void warningDisableChangeFloorFrame();

};
} // namespace post3dapp
#endif

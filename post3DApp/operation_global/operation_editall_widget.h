#ifndef OPERATIONEDITALLWIDGET_H
#define OPERATIONEDITALLWIDGET_H

#include <QWidget>
#include <QDialog>
#include "define_unifieddata.h"

namespace post3dapp{
class ElidedLabel;
class LimitSelectDialog;
class OperationEditAllWidget : public QWidget
{
    Q_OBJECT

public:

    OperationEditAllWidget( QWidget *parent = nullptr );

protected:

private slots:

    void showLimitDialog();
    void slotEnableLimitParts(bool);

signals:

private:

    LimitSelectDialog *limitDialog;

    QCheckBox *limitCheckBox;
    QPushButton *limitButton;
    ElidedLabel *limitedLabel;
};


class LimitSelectDialog : public QDialog
{
    Q_OBJECT

public:

    LimitSelectDialog( QWidget *parent = nullptr );

    void setCurrentValues();
    inline LIMITSELECTTERM currentLimitSettings() const {return mySettings;}

protected:

private slots:

    void slotEnableFloorParts(bool);
    void slotEnableFrameParts(bool);
    void slotEnableElementParts(bool);

    void showSelectFloorDialog();
    void showSelectFrameDialog();
    void showSelectElementDialog();

    void accept() override;

signals:

private:

    LIMITSELECTTERM mySettings;

    QCheckBox *limitFloorCheckBox;
    QRadioButton *limitFloorAllButton;
    QRadioButton *limitFloorPartButton;
    QPushButton *limitFloorButton;
    ElidedLabel *limitFloorLabel;
    QStringList limitedFloors;

    QCheckBox *limitFrameCheckBox;
    QRadioButton *limitFrameAllButton;
    QRadioButton *limitFramePartButton;
    QPushButton *limitFrameButton;
    ElidedLabel *limitFrameLabel;
    QStringList limitedFrames;

    QCheckBox *limitDataTypeCheckBox;
    QPushButton *limitDataTypeButton;
    ElidedLabel *limitDataTypeLabel;
    QList<DATATYPE> limitedDataTypes;

    QCheckBox *limitNameCheckBox;
    QLineEdit *limitNameEdit;

    void createLayout();
};
} // namespace post3dapp


#endif

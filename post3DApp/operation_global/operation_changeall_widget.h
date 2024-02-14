#ifndef OPERATIONCHANGEALLWIDGET_H
#define OPERATIONCHANGEALLWIDGET_H

#include <QWidget>

class QButtonGroup;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
namespace post3dapp{
class ElidedLabel;
class ReferenceCombobox;
class OperationChangeAllWidget : public QWidget
{
    Q_OBJECT

public:

    OperationChangeAllWidget( QWidget *parent = nullptr );

private slots:

    void slotEnableFrameLimit(bool);
    void slotEnableElementLimit(bool);
    void slotEnableRangeLimit(bool);

    void showSelectFloorDialog();
    void showFrameRangeDialog();
    void showElementLimitDialog();

    void executeFloorCopy();
    void executeChangeName();
    void executeJointSort();

    void updateChangedNameCombobox();

protected:

    QWidget *floorCopyWidget;
    QWidget *changeNameWidget;
    QWidget *sortJointWidget;

    // 階コピー
    ReferenceCombobox *sourceCombobox;
    QStringList destinationFloors;
    ElidedLabel *destinationFloorLabel;

    QButtonGroup *baseButtonGroup;
    QButtonGroup *upperButtonGroup;
    QButtonGroup *jointButtonGroup;
    QButtonGroup *memberButtonGroup;

    QCheckBox *frameCheckBox;
    QPushButton *frameButton;
    QStringList rangeFrames;
    ElidedLabel *frameRangeLabel;

    QCheckBox *elementCheckBox;
    QPushButton *elementButton;
    QList<int> limitedElements;
    ElidedLabel *limitedElementLabel;

    QCheckBox *rangeCheckBox;
    QLabel *rangeMinXLabel;
    QLineEdit *minXLine;
    QLabel *rangeMinYLabel;
    QLineEdit *minYLine;
    QLabel *rangeMaxXLabel;
    QLineEdit *maxXLine;
    QLabel *rangeMaxYLabel;
    QLineEdit *maxYLine;

    //名前変更
    QComboBox *changedTypeCombobox;
    QComboBox *changedNameCombobox;
    QLineEdit *newNameLineEdit;

    //節点番号ソート
    QButtonGroup *firstButtonGroup;
    QComboBox *secondCombobox;
    QButtonGroup *secondButtonGroup;
    QComboBox *thirdCombobox;
    QButtonGroup *thirdButtonGroup;

    void createFloorCopyWidget();
    void createChangeNameWidget();
    void createSortJointWidget();
    void createStackedLayout();

};
} // namespace post3dapp
#endif

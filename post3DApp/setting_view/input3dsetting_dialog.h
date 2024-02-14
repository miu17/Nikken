#ifndef INPUT3DSETTINGDIALOG_H
#define INPUT3DSETTINGDIALOG_H

#include <QDialog>

class QButtonGroup;
namespace post3dapp{
enum ELEMENTSTATE;
enum ELEMENTTYPE;
enum class DATATYPE;
class ElementStateWidgetGroup;
class ElementWidgetGroup;
class ElidedLabel;
class FiveSizeComboBox;
class IndividualSettingTableWidget;
class Input3DViewSettings;
class ReferenceCombobox;
class TypicalColorComboBox;

class Input3DSettingDialog : public QDialog
{
    Q_OBJECT

public:

    Input3DSettingDialog( QWidget *parent = 0 );
    ~Input3DSettingDialog();

protected:

    void closeEvent( QCloseEvent * );
    void keyPressEvent ( QKeyEvent * ) { }

private slots:

    void initializeSettings();
    void uploadViewSettings();

    void showSelectFloorDialog();
    void showSelectFrameDialog();
    void updateSelectedFloorFrame(DATATYPE);

    void slotFileSave();
    void slotFileOpen();

signals:
    void windowClosed();

private:
    void createAllWidgets();
    void createIndividualWidgets();
    Input3DViewSettings makeSettings() const;
    void applySettings(const Input3DViewSettings&);
    void saveSettings(const QString &path) const;
    void readSettings(const QString &path);

    // パース関係

    QWidget *persAllWidget;
    IndividualSettingTableWidget *persIndividualWidget;

    QCheckBox *priorityCheckbox;
    QButtonGroup *limitedFloorGroup;
    QButtonGroup *limitedFrameGroup;

    TypicalColorComboBox *scaleColorCombobox;
    QCheckBox *frscaleCheck;
    ReferenceCombobox *frscaleFloorCombobox;
    QCheckBox *flscaleCheck;
    QLineEdit *flscaleXLine;
    QLineEdit *flscaleYLine;

    FiveSizeComboBox *jointSizeCombobox;
    TypicalColorComboBox *jointColorCombobox;
    FiveSizeComboBox *memberSizeCombobox;
    TypicalColorComboBox *memberColorCombobox;

    QHash<ELEMENTSTATE, ElementStateWidgetGroup *> elementStateWidgets;
    QHash<ELEMENTTYPE, ElementWidgetGroup *> elementWidgets;

    QStringList limitedFloors;
    QStringList limitedFrames;
    ElidedLabel *limitedFloorLabel;
    ElidedLabel *limitedFrameLabel;

};
} // namespace post3dapp
#endif


#ifndef INPUT2DSETTINGDIALOG_H
#define INPUT2DSETTINGDIALOG_H

#include <QDialog>

namespace post3dapp{
enum ELEMENTSTATE;
enum ELEMENTTYPE;

class ElementWidgetGroup;
class ElementStateWidgetGroup;
class DownWidgetGroup;
class FiveSizeComboBox;
class Input2DViewSettings;
class IndividualSettingTableWidget;
class TypicalColorComboBox;

class Input2DSettingDialog : public QDialog
{
    Q_OBJECT

public:

    Input2DSettingDialog( QWidget *parent = 0 );
    ~Input2DSettingDialog();

protected:

    void closeEvent(QCloseEvent *);
    void keyPressEvent ( QKeyEvent * ) { }

private slots:

    void initializeSettings();
    void uploadViewSettings();

    void slotFileSave();
    void slotFileOpen();

signals:
    void windowClosed();

private:
    void createAllWidgets();
    void createIndividualWidgets();
    Input2DViewSettings makeSettings() const;
    void applySettings(const Input2DViewSettings&);
    void saveSettings(const QString &path) const;
    void readSettings(const QString &path);
    // 伏図・軸組図関係

    QWidget *planAllWidget;
    IndividualSettingTableWidget *planIndividualWidget;

    QCheckBox *priorityCheckbox;
    QCheckBox *directionCheckbox;
    TypicalColorComboBox *frlineNormalCombobox;
    TypicalColorComboBox *frlineFocusCombobox;
    FiveSizeComboBox *jointSizeCombobox;
    FiveSizeComboBox *memberSizeCombobox;
    FiveSizeComboBox *loadSizeCombobox;

    QHash<ELEMENTTYPE, ElementWidgetGroup *> elementWidgets;
    QHash<ELEMENTSTATE, ElementStateWidgetGroup *> elementStateWidgets;
    ElementWidgetGroup* crossElementWidget;
    DownWidgetGroup* downWidgetGroup;



};
} // namespace post3dapp
#endif


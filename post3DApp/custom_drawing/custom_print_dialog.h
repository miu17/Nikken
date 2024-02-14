#ifndef CUSTOMPRINTDIALOG_H
#define CUSTOMPRINTDIALOG_H
#include <QDialog>
#include "custom_print_dialog_settingdialog.h"
#include "define_draw_figure.h"

class QComboBox;
class QRadioButton;
class QPrintPreviewWidget;
class QLabel;
class QPrinter;

namespace post3dapp{
class CustomDrawGrScene;


enum class Scale{
};
enum class Paper{
};

class CustomPrintDialog : public QDialog
{
    Q_OBJECT
public:
    CustomPrintDialog(CustomDrawGrScene*);

private slots:
    void previewClicked(QPrinter *newPrinter);
    void pageToggleChanged(int value);
    void settingButtonClicked();
    void scrolled();
    void setPrinter();

private:
    const CustomDrawGrScene* baseScene;
    CustomDrawGrScene* CopyScene() const;
    void createLayout();
    QWidget* createPreviewWidget();
    void drawPreview();

    QComboBox* paperSize;
    QSpinBox* tate;
    QSpinBox* yoko;
    QPushButton* settingButton;
    QPushButton* previewButton;
    QPushButton* printButton;

    QRadioButton* autoScaleRadioButton;
    QRadioButton* fixScaleRadioButton;
    QSpinBox* scale;

    QCheckBox* check_header;
    QCheckBox* check_footer;

    QPrinter* printer;
    QPrintPreviewWidget* myPrintPreview;

    QSpinBox* tate_now;
    QSpinBox* yoko_now;
    QLabel* tate_max;
    QLabel* yoko_max;
    QLabel* calcedScale;

    FIGURE_SIZE_POLICY policy;
    FIGURE_PROPERTY results;
    CustomPrintDialogSetting setting;
};

}
#endif // CUSTOMPRINTDIALOG_H

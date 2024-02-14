#ifndef CUSTOMPRINTDIALOGSETTINGDIALOG_H
#define CUSTOMPRINTDIALOGSETTINGDIALOG_H

#include <QDialog>

class QCheckBox;
class QSpinBox;

namespace post3dapp{
class FIGURE_SIZE_POLICY;
class FiveSizeComboBox;

class CustomPrintDialogSetting{
public:
    CustomPrintDialogSetting(){
        use_scaled_font = true;
        valueTextSize = 3;
    }
    bool use_scaled_font;
    int valueTextSize;
};

class CustomPrintDialogSettingDialog : public QDialog
{
    Q_OBJECT
public:
    CustomPrintDialogSettingDialog(const FIGURE_SIZE_POLICY&, const CustomPrintDialogSetting&, QWidget *parent = 0);
    void get_policy(FIGURE_SIZE_POLICY*, CustomPrintDialogSetting*);

private:
    void set_value(const FIGURE_SIZE_POLICY&, const CustomPrintDialogSetting&);

    QSpinBox* model_margin_left;
    QSpinBox* model_margin_right;
    QSpinBox* model_margin_top;
    QSpinBox* model_margin_bottom;
    QSpinBox* model_margin_overlap;

    QSpinBox* paper_margin_left;
    QSpinBox* paper_margin_right;
    QSpinBox* paper_margin_top;
    QSpinBox* paper_margin_bottom;

    QCheckBox* check_use_scaled_font;
    FiveSizeComboBox* valueFontSizeCombobox;
    QSpinBox* header_size;
    QSpinBox* marker_size;
    QCheckBox* check_waku;
};
}

#endif // CUSTOMPRINTDIALOGSETTINGWIDGET_H

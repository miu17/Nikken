#include "custom_print_dialog_settingdialog.h"

#include <QtWidgets>
#include <QDialogButtonBox>
#include "define_draw_figure.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{

CustomPrintDialogSettingDialog::CustomPrintDialogSettingDialog(const FIGURE_SIZE_POLICY& policy, const CustomPrintDialogSetting& setting, QWidget *parent)
    :QDialog(parent)
{
    model_margin_left = new QSpinBox;
    model_margin_left->setMinimum(0);
    model_margin_right = new QSpinBox;
    model_margin_right->setMinimum(0);
    model_margin_top = new QSpinBox;
    model_margin_top->setMinimum(0);
    model_margin_bottom = new QSpinBox;
    model_margin_bottom->setMinimum(0);
    model_margin_overlap = new QSpinBox;
    model_margin_overlap->setMinimum(0);

    QHBoxLayout* firstLine = new QHBoxLayout;
    firstLine->addWidget(new QLabel(u8"マージン左"));
    firstLine->addWidget(model_margin_left);
    firstLine->addWidget(new QLabel(u8"マージン右"));
    firstLine->addWidget(model_margin_right);
    firstLine->addWidget(new QLabel(u8"マージン上"));
    firstLine->addWidget(model_margin_top);
    firstLine->addWidget(new QLabel(u8"マージン下"));
    firstLine->addWidget(model_margin_bottom);
    firstLine->addWidget(new QLabel(u8"マージン重なり長"));
    firstLine->addWidget(model_margin_overlap);

    paper_margin_left = new QSpinBox;
    paper_margin_left->setMinimum(0);
    paper_margin_right = new QSpinBox;
    paper_margin_right->setMinimum(0);
    paper_margin_top = new QSpinBox;
    paper_margin_top->setMinimum(0);
    paper_margin_bottom = new QSpinBox;
    paper_margin_bottom->setMinimum(0);

    QHBoxLayout* secondLine = new QHBoxLayout;
    secondLine->addWidget(new QLabel(u8"余白左"));
    secondLine->addWidget(paper_margin_left);
    secondLine->addWidget(new QLabel(u8"余白右"));
    secondLine->addWidget(paper_margin_right);
    secondLine->addWidget(new QLabel(u8"余白上"));
    secondLine->addWidget(paper_margin_top);
    secondLine->addWidget(new QLabel(u8"余白下"));
    secondLine->addWidget(paper_margin_bottom);

    check_use_scaled_font = new QCheckBox(u8"数値の拡縮");
    valueFontSizeCombobox = new FiveSizeComboBox;
    header_size = new QSpinBox;
    header_size->setMinimum(4);
    header_size->setMaximum(20);
    marker_size = new QSpinBox;
    marker_size->setMinimum(4);
    marker_size->setMaximum(20);
    check_waku = new QCheckBox(u8"枠を表示する");

    QHBoxLayout* thirdLine = new QHBoxLayout;
    thirdLine->addWidget(check_use_scaled_font);
    thirdLine->addWidget(valueFontSizeCombobox);
    thirdLine->addWidget(new QLabel(u8"ヘッダーサイズ"));
    thirdLine->addWidget(header_size);
    thirdLine->addWidget(new QLabel(u8"切断マーカーサイズ"));
    thirdLine->addWidget(marker_size);
    thirdLine->addWidget(check_waku);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QHBoxLayout* lastLine = new QHBoxLayout;
    lastLine->addWidget(buttonBox);

    QVBoxLayout* vlay = new QVBoxLayout;
    vlay->addLayout(firstLine);
    vlay->addLayout(secondLine);
    vlay->addLayout(thirdLine);
    vlay->addLayout(lastLine);

    this->setLayout(vlay);

    set_value(policy, setting);

}

void CustomPrintDialogSettingDialog::set_value(const FIGURE_SIZE_POLICY& policy, const CustomPrintDialogSetting& setting)
{
    model_margin_left->setValue(policy.modelMargin_left);
    model_margin_right->setValue(policy.modelMargin_right);
    model_margin_top->setValue(policy.modelMargin_top);
    model_margin_bottom->setValue(policy.modelMargin_bottom);
    model_margin_overlap->setValue(policy.modelMargin_overlap);
    paper_margin_left->setValue(policy.paperMargin_left);
    paper_margin_right->setValue(policy.paperMargin_right);
    paper_margin_top->setValue(policy.paperMargin_top);
    paper_margin_bottom->setValue(policy.paperMargin_bottom);
    check_waku->setChecked(policy.drawWaku);
    header_size->setValue(policy.headerSize);
    marker_size->setValue(policy.markerSize);
    check_use_scaled_font->setChecked(setting.use_scaled_font);
    valueFontSizeCombobox->setSize(setting.valueTextSize);
}

void CustomPrintDialogSettingDialog::get_policy(FIGURE_SIZE_POLICY* policy, CustomPrintDialogSetting* setting){
    policy->modelMargin_left = model_margin_left->value();
    policy->modelMargin_right = model_margin_right->value();
    policy->modelMargin_top = model_margin_top->value();
    policy->modelMargin_bottom = model_margin_bottom->value();
    policy->modelMargin_overlap = model_margin_overlap->value();
    policy->paperMargin_left = paper_margin_left->value();
    policy->paperMargin_right = paper_margin_right->value();
    policy->paperMargin_top = paper_margin_top->value();
    policy->paperMargin_bottom = paper_margin_bottom->value();
    policy->drawWaku = check_waku->isChecked();
    policy->headerSize = header_size->value();
    policy->markerSize = marker_size->value();
    setting->use_scaled_font = check_use_scaled_font->isChecked();
    setting->valueTextSize = valueFontSizeCombobox->size();
}



}

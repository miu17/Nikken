#include "analysis2dsetting_totalwidget.h"

#include "define_output.h"
#include "define_analysis2dsettings.h"
#include "typicalcolor_combobox.h"
#include "vhline.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{

Analysis2DSettingTotalWidget::Analysis2DSettingTotalWidget(const QString& tabName, const QString& _englishTabName, QWidget* parent)
    : QWidget(parent), TabName(tabName), englishTabName(_englishTabName)
{
    int labelWidth = 150;
    int colorWidth = 180;
    // 階・通りのライン
    QLabel *scaleLineLabel = new QLabel(u8"階／通りスケール：");
    scaleLineLabel->setFixedWidth(labelWidth);
    scaleColorCombobox = new TypicalColorComboBox();
    scaleColorCombobox->setFixedWidth(colorWidth);
    QHBoxLayout *hlay_scale = new QHBoxLayout();
    hlay_scale->setContentsMargins(0, 0, 0, 0);
    hlay_scale->addWidget(scaleLineLabel);
    hlay_scale->addWidget(scaleColorCombobox);
    hlay_scale->addStretch();

    QHBoxLayout *hlay_gridfloor = new QHBoxLayout();
    QLabel *scaleStringLabel = new QLabel(u8"階／通り名：");
    scaleStringLabel->setFixedWidth(labelWidth);
    scaleSizeCombobox = new FiveSizeComboBox();
    hlay_gridfloor->setContentsMargins(0, 0, 0, 0);
    hlay_gridfloor->addWidget(scaleStringLabel);
    hlay_gridfloor->addWidget(scaleSizeCombobox);
    hlay_gridfloor->addSpacing(50);

    gridStringCheck = new QCheckBox(u8"通り表示");
    floorStringCheck = new QCheckBox(u8"階表示");
    hlay_gridfloor->addWidget(gridStringCheck);
    hlay_gridfloor->addWidget(floorStringCheck);
    hlay_gridfloor->addStretch();

    // 解析節点
    QLabel *jointLabel = new QLabel(u8"解析節点");
    jointLabel->setFixedWidth(labelWidth);
    jointSizeCombobox = new FiveSizeComboBox();
    jointColorCombobox = new TypicalColorComboBox();
    jointColorCombobox->setFixedWidth(colorWidth);
    QHBoxLayout *hlay_joint = new QHBoxLayout;
    hlay_joint->setContentsMargins(0, 0, 0, 0);
    hlay_joint->addWidget(jointLabel);
    hlay_joint->addWidget(jointColorCombobox);
    hlay_joint->addSpacing(30);
    hlay_joint->addWidget(jointSizeCombobox);
    hlay_joint->addStretch();

    // 解析部材（線）
    QLabel *lineLabel = new QLabel(u8"・解析部材（線材）");
    lineLabel->setFixedWidth(120);
    QLabel *lineRCColorLabel = new QLabel(u8"線部材(RC)：");
    lineRCColorLabel->setFixedWidth(labelWidth);
    lineRCColorCombobox = new TypicalColorComboBox();
    lineRCColorCombobox->setFixedWidth(colorWidth);
    QLabel *lineSRCColorLabel = new QLabel(u8"線部材(SRC)：");
    lineSRCColorLabel->setFixedWidth(labelWidth);
    lineSRCColorCombobox = new TypicalColorComboBox();
    lineSRCColorCombobox->setFixedWidth(colorWidth);
    QLabel *lineSColorLabel = new QLabel(u8"線部材(S)：");
    lineSColorLabel->setFixedWidth(labelWidth);
    lineSColorCombobox = new TypicalColorComboBox();
    lineSColorCombobox->setFixedWidth(colorWidth);
    QLabel *lineCFTColorLabel = new QLabel(u8"線部材(CFT)：");
    lineCFTColorLabel->setFixedWidth(labelWidth);
    lineCFTColorCombobox = new TypicalColorComboBox();
    lineCFTColorCombobox->setFixedWidth(colorWidth);
    QLabel *lineDamperColorLabel = new QLabel(u8"線部材(制振)：");
    lineDamperColorLabel->setFixedWidth(labelWidth);
    lineDamperColorCombobox = new TypicalColorComboBox();
    lineDamperColorCombobox->setFixedWidth(colorWidth);
    QLabel *lineIsoColorLabel = new QLabel(u8"線部材(免震)：");
    lineIsoColorLabel->setFixedWidth(labelWidth);
    lineIsoColorCombobox = new TypicalColorComboBox();
    lineIsoColorCombobox->setFixedWidth(colorWidth);

    QHBoxLayout *hlay_line = new QHBoxLayout;
    hlay_line->setContentsMargins(0, 0, 0, 0);
    hlay_line->addWidget(lineRCColorLabel);
    hlay_line->addWidget(lineRCColorCombobox);
    hlay_line->addStretch();

    QHBoxLayout *hlay_line2 = new QHBoxLayout;
    hlay_line2->setContentsMargins(0, 0, 0, 0);
    hlay_line2->addWidget(lineSRCColorLabel);
    hlay_line2->addWidget(lineSRCColorCombobox);
    hlay_line2->addStretch();

    QHBoxLayout *hlay_line3 = new QHBoxLayout;
    hlay_line3->setContentsMargins(0, 0, 0, 0);
    hlay_line3->addWidget(lineSColorLabel);
    hlay_line3->addWidget(lineSColorCombobox);
    hlay_line3->addStretch();

    QHBoxLayout *hlay_line4 = new QHBoxLayout;
    hlay_line4->setContentsMargins(0, 0, 0, 0);
    hlay_line4->addWidget(lineCFTColorLabel);
    hlay_line4->addWidget(lineCFTColorCombobox);
    hlay_line4->addStretch();

    QHBoxLayout *hlay_line5 = new QHBoxLayout;
    hlay_line5->setContentsMargins(0, 0, 0, 0);
    hlay_line5->addWidget(lineDamperColorLabel);
    hlay_line5->addWidget(lineDamperColorCombobox);
    hlay_line5->addStretch();

    QHBoxLayout *hlay_line6 = new QHBoxLayout;
    hlay_line6->setContentsMargins(0, 0, 0, 0);
    hlay_line6->addWidget(lineIsoColorLabel);
    hlay_line6->addWidget(lineIsoColorCombobox);
    hlay_line6->addStretch();

    // 解析部材（面）
    QLabel *planeLabel = new QLabel(u8"・解析部材（面材）");
    planeLabel->setFixedWidth(120);
    QLabel *planeRCColorLabel = new QLabel(u8"面部材(RC・SRC)：");
    planeRCColorLabel->setFixedWidth(labelWidth);
    planeRCColorCombobox = new TypicalColorComboBox();
    planeRCColorCombobox->setFixedWidth(colorWidth);
    QLabel *planeSColorLabel = new QLabel(u8"面部材(S)：");
    planeSColorLabel->setFixedWidth(labelWidth);
    planeSColorCombobox = new TypicalColorComboBox();
    planeSColorCombobox->setFixedWidth(colorWidth);
    QLabel *planeDamperColorLabel = new QLabel(u8"面部材(制振)：");
    planeDamperColorLabel->setFixedWidth(labelWidth);
    planeDamperColorCombobox = new TypicalColorComboBox();
    planeDamperColorCombobox->setFixedWidth(colorWidth);
    QHBoxLayout *hlay_plane = new QHBoxLayout;
    hlay_plane->setContentsMargins(0, 0, 0, 0);
    hlay_plane->addWidget(planeRCColorLabel);
    hlay_plane->addWidget(planeRCColorCombobox);
    hlay_plane->addStretch();

    QHBoxLayout *hlay_plane2 = new QHBoxLayout;
    hlay_plane2->setContentsMargins(0, 0, 0, 0);
    hlay_plane2->addWidget(planeSColorLabel);
    hlay_plane2->addWidget(planeSColorCombobox);
    hlay_plane2->addStretch();

    QHBoxLayout *hlay_plane3 = new QHBoxLayout;
    hlay_plane3->setContentsMargins(0, 0, 0, 0);
    hlay_plane3->addWidget(planeDamperColorLabel);
    hlay_plane3->addWidget(planeDamperColorCombobox);
    hlay_plane3->addStretch();

    // ばね・支点
    QLabel *supportLabel = new QLabel(u8"ばね／支点：");
    supportLabel->setFixedWidth(labelWidth);
    supportColorCombobox = new TypicalColorComboBox();
    supportColorCombobox->setFixedWidth(colorWidth);
    QHBoxLayout *hlay_support = new QHBoxLayout;
    hlay_support->setContentsMargins(0, 0, 0, 0);
    hlay_support->addWidget(supportLabel);
    hlay_support->addWidget(supportColorCombobox);
    hlay_support->addStretch();

    //値の共通文字サイズ・色
    QLabel *valueLabel = new QLabel(u8"数値：");
    valueLabel->setFixedWidth(labelWidth);
    valueColorCombobox = new TypicalColorComboBox();
    valueColorCombobox->setFixedWidth(colorWidth);
    valueSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_value = new QHBoxLayout;
    hlay_value->setContentsMargins(0, 0, 0, 0);
    hlay_value->addWidget(valueLabel);
    hlay_value->addWidget(valueColorCombobox);
    hlay_value->addSpacing(30);
    hlay_value->addWidget(valueSizeCombobox);
    hlay_value->addStretch();

    //　選択時の色
    QLabel *selectColorLabel = new QLabel(u8"選択部材：");
    selectColorLabel->setFixedWidth(labelWidth);
    selectColorCombobox = new TypicalColorComboBox();
    selectColorCombobox->setFixedWidth(colorWidth);
    QHBoxLayout *hlay_select = new QHBoxLayout;
    hlay_select->setContentsMargins(0, 0, 0, 0);
    hlay_select->addWidget(selectColorLabel);
    hlay_select->addWidget(selectColorCombobox);
    hlay_select->addStretch();

    //注記の共通文字サイズ・色
    QLabel *noteLabel = new QLabel(u8"付加データ：");
    noteLabel->setFixedWidth(labelWidth);
    noteColorCombobox = new TypicalColorComboBox();
    noteColorCombobox->setFixedWidth(colorWidth);
    noteSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_note = new QHBoxLayout;
    hlay_note->setContentsMargins(0, 0, 0, 0);
    hlay_note->addWidget(noteLabel);
    hlay_note->addWidget(noteColorCombobox);
    hlay_note->addSpacing(30);
    hlay_note->addWidget(noteSizeCombobox);
    hlay_note->addStretch();

    // 塑性記号
    QLabel *plasticLabel = new QLabel(u8"塑性記号：");
    plasticLabel->setFixedWidth(labelWidth);
    plasticSymbolColorCombobox = new TypicalColorComboBox();
    plasticSymbolColorCombobox->setFixedWidth(colorWidth);
    plasticSymbolSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_plastic = new QHBoxLayout;
    hlay_plastic->setContentsMargins(0, 0, 0, 0);
    hlay_plastic->addWidget(plasticLabel);
    hlay_plastic->addWidget(plasticSymbolColorCombobox);
    hlay_plastic->addSpacing(30);
    hlay_plastic->addWidget(plasticSymbolSizeCombobox);
    hlay_plastic->addStretch();

    // 応力図
    QLabel *stressLabel = new QLabel(u8"■応力図");
    QLabel *stressLineColorLabel = new QLabel(u8"応力線の色：");
    stressLineColorLabel->setFixedWidth(labelWidth);
    stressColorCombobox = new TypicalColorComboBox();
    stressColorCombobox->setFixedWidth(colorWidth);
    QLabel *stressLineScaleLabel = new QLabel(u8"スケール：");
    stressLineScaleLabel->setFixedWidth(labelWidth);
    stressScaleSpin = new QDoubleSpinBox();
    stressScaleSpin->setRange(0.01, 10000.0);
    QLabel *stressLineScale2Label = new QLabel(u8" kNm = 1.0m");

    QLabel *stressValueLabel = new QLabel(u8"数値：");
    stressValueLabel->setFixedWidth(labelWidth);
    NValueCheck = new QCheckBox(u8"N値の表示");
    MValueCheck = new QCheckBox(u8"M値の表示");
    QValueCheck = new QCheckBox(u8"Q値の表示");

    QLabel *stressColummLabel = new QLabel(u8"柱：");
    stressColummLabel->setFixedWidth(labelWidth);
    columnValueCheck = new QCheckBox(u8"値の表示");
    stressColumnColorCombobox = new TypicalColorComboBox();
    stressColumnColorCombobox->setFixedWidth(colorWidth);

    QLabel *stressGirderLabel = new QLabel(u8"大梁：");
    stressGirderLabel->setFixedWidth(labelWidth);
    girderValueCheck = new QCheckBox(u8"値の表示");
    stressGirderColorCombobox = new TypicalColorComboBox();
    stressGirderColorCombobox->setFixedWidth(colorWidth);

    QLabel *stressBraceLabel = new QLabel(u8"ブレース：");
    stressBraceLabel->setFixedWidth(labelWidth);
    braceValueCheck = new QCheckBox(u8"値の表示");
    stressBraceColorCombobox = new TypicalColorComboBox();
    stressBraceColorCombobox->setFixedWidth(colorWidth);

    QLabel *stressWallLabel = new QLabel(u8"壁：");
    stressWallLabel->setFixedWidth(labelWidth);
    wallValueCheck = new QCheckBox(u8"値の表示");
    stressWallColorCombobox = new TypicalColorComboBox();
    stressWallColorCombobox->setFixedWidth(colorWidth);

    QLabel *stressSlabLabel = new QLabel(u8"スラブ：");
    stressSlabLabel->setFixedWidth(labelWidth);
    slabValueCheck = new QCheckBox(u8"値の表示");
    stressSlabColorCombobox = new TypicalColorComboBox();
    stressSlabColorCombobox->setFixedWidth(colorWidth);

    QLabel *stressPanelLabel = new QLabel(u8"パネル：");
    stressPanelLabel->setFixedWidth(labelWidth);
    panelValueCheck = new QCheckBox(u8"値の表示");
    stressPanelColorCombobox = new TypicalColorComboBox();
    stressPanelColorCombobox->setFixedWidth(colorWidth);

    QLabel *stressSpringLabel = new QLabel(u8"ばね：");
    stressSpringLabel->setFixedWidth(labelWidth);
    springValueCheck = new QCheckBox(u8"値の表示");
    stressSpringColorCombobox = new TypicalColorComboBox();
    stressSpringColorCombobox->setFixedWidth(colorWidth);

    QLabel *stressDamperLabel = new QLabel(u8"制振：");
    stressDamperLabel->setFixedWidth(labelWidth);
    damperValueCheck = new QCheckBox(u8"値の表示");
    stressDamperColorCombobox = new TypicalColorComboBox();
    stressDamperColorCombobox->setFixedWidth(colorWidth);

    QLabel *stressIsoLabel = new QLabel(u8"免震：");
    stressIsoLabel->setFixedWidth(labelWidth);
    isoValueCheck = new QCheckBox(u8"値の表示");
    stressIsoColorCombobox = new TypicalColorComboBox();
    stressIsoColorCombobox->setFixedWidth(colorWidth);


    QHBoxLayout *hlay_stress = new QHBoxLayout;
    hlay_stress->addWidget(stressLineColorLabel);
    hlay_stress->addWidget(stressColorCombobox);
    hlay_stress->addStretch();

    QHBoxLayout *hlay_stress2 = new QHBoxLayout;
    hlay_stress2->addWidget(stressLineScaleLabel);
    hlay_stress2->addWidget(stressScaleSpin);
    hlay_stress2->addWidget(stressLineScale2Label);
    hlay_stress2->addStretch();

    QHBoxLayout *hlay_stress_value = new QHBoxLayout;
    hlay_stress_value->addWidget(stressValueLabel);
    hlay_stress_value->addWidget(NValueCheck);
    hlay_stress_value->addWidget(MValueCheck);
    hlay_stress_value->addWidget(QValueCheck);
    hlay_stress_value->addStretch();

    QHBoxLayout *hlay_stressColumn = new QHBoxLayout;
    hlay_stressColumn->addWidget(stressColummLabel);
    hlay_stressColumn->addWidget(columnValueCheck);
    hlay_stressColumn->addSpacing(30);
    hlay_stressColumn->addWidget(stressColumnColorCombobox);
    hlay_stressColumn->addStretch();

    QHBoxLayout *hlay_stressGirder = new QHBoxLayout;
    hlay_stressGirder->addWidget(stressGirderLabel);
    hlay_stressGirder->addWidget(girderValueCheck);
    hlay_stressGirder->addSpacing(30);
    hlay_stressGirder->addWidget(stressGirderColorCombobox);
    hlay_stressGirder->addStretch();

    QHBoxLayout *hlay_stressBrace = new QHBoxLayout;
    hlay_stressBrace->addWidget(stressBraceLabel);
    hlay_stressBrace->addWidget(braceValueCheck);
    hlay_stressBrace->addSpacing(30);
    hlay_stressBrace->addWidget(stressBraceColorCombobox);
    hlay_stressBrace->addStretch();

    QHBoxLayout *hlay_stressWall = new QHBoxLayout;
    hlay_stressWall->addWidget(stressWallLabel);
    hlay_stressWall->addWidget(wallValueCheck);
    hlay_stressWall->addSpacing(30);
    hlay_stressWall->addWidget(stressWallColorCombobox);
    hlay_stressWall->addStretch();

    QHBoxLayout *hlay_stressSlab = new QHBoxLayout;
    hlay_stressSlab->addWidget(stressSlabLabel);
    hlay_stressSlab->addWidget(slabValueCheck);
    hlay_stressSlab->addSpacing(30);
    hlay_stressSlab->addWidget(stressSlabColorCombobox);
    hlay_stressSlab->addStretch();

    QHBoxLayout *hlay_stressPanel = new QHBoxLayout;
    hlay_stressPanel->addWidget(stressPanelLabel);
    hlay_stressPanel->addWidget(panelValueCheck);
    hlay_stressPanel->addSpacing(30);
    hlay_stressPanel->addWidget(stressPanelColorCombobox);
    hlay_stressPanel->addStretch();

    QHBoxLayout *hlay_stressSpring = new QHBoxLayout;
    hlay_stressSpring->addWidget(stressSpringLabel);
    hlay_stressSpring->addWidget(springValueCheck);
    hlay_stressSpring->addSpacing(30);
    hlay_stressSpring->addWidget(stressSpringColorCombobox);
    hlay_stressSpring->addStretch();

    QHBoxLayout *hlay_stressDamper = new QHBoxLayout;
    hlay_stressDamper->addWidget(stressDamperLabel);
    hlay_stressDamper->addWidget(damperValueCheck);
    hlay_stressDamper->addSpacing(30);
    hlay_stressDamper->addWidget(stressDamperColorCombobox);
    hlay_stressDamper->addStretch();

    QHBoxLayout *hlay_stressIso = new QHBoxLayout;
    hlay_stressIso->addWidget(stressIsoLabel);
    hlay_stressIso->addWidget(isoValueCheck);
    hlay_stressIso->addSpacing(30);
    hlay_stressIso->addWidget(stressIsoColorCombobox);
    hlay_stressIso->addStretch();



    QVBoxLayout *vlay_stress = new QVBoxLayout;
    vlay_stress->addWidget(stressLabel);
    vlay_stress->addLayout(hlay_stress);
    vlay_stress->addLayout(hlay_stress2);
    vlay_stress->addLayout(hlay_stress_value);
    vlay_stress->addLayout(hlay_stressColumn);
    vlay_stress->addLayout(hlay_stressGirder);
    vlay_stress->addLayout(hlay_stressBrace);
    vlay_stress->addLayout(hlay_stressWall);
    vlay_stress->addLayout(hlay_stressSlab);
    vlay_stress->addLayout(hlay_stressPanel);
    vlay_stress->addLayout(hlay_stressSpring);
    vlay_stress->addLayout(hlay_stressDamper);
    vlay_stress->addLayout(hlay_stressIso);
    vlay_stress->addStretch();

    // その他(軸力図)
    QLabel *axialLabel = new QLabel(u8"■軸力図");
    QLabel *axialMinusLabel = new QLabel(u8"圧縮：");
    axialMinusLabel->setFixedWidth(labelWidth);
    axialMinusColorCombobox = new TypicalColorComboBox();
    axialMinusColorCombobox->setFixedWidth(colorWidth);
    QLabel *axialPlusLabel = new QLabel(u8"引張：");
    axialPlusLabel->setFixedWidth(labelWidth);
    axialPlusColorCombobox = new TypicalColorComboBox();
    axialPlusColorCombobox->setFixedWidth(colorWidth);
    QLabel *axialShapeScaleLabel = new QLabel(u8"スケール");
    axialShapeScaleLabel->setFixedWidth(labelWidth);
    axialScaleSpin = new QDoubleSpinBox();
    axialScaleSpin->setRange(1.0, 9999.99);
    QLabel *axialShapeScale2Label = new QLabel(u8" kN = 1.0m");

    QHBoxLayout *hlay_axial = new QHBoxLayout;
    hlay_axial->addWidget(axialMinusLabel);
    hlay_axial->addWidget(axialMinusColorCombobox);
    hlay_axial->addSpacing(30);
    hlay_axial->addWidget(axialPlusLabel);
    hlay_axial->addWidget(axialPlusColorCombobox);
    hlay_axial->addStretch();
    QHBoxLayout *hlay_axial2 = new QHBoxLayout;
    hlay_axial2->addWidget(axialShapeScaleLabel);
    hlay_axial2->addWidget(axialScaleSpin);
    hlay_axial2->addWidget(axialShapeScale2Label);
    hlay_axial2->addStretch();
    QVBoxLayout *vlay_axial = new QVBoxLayout;
    vlay_axial->addLayout(hlay_axial);
    vlay_axial->addLayout(hlay_axial2);

    // その他(反力図・反力マップ・組み合わせ反力マップ)
    QLabel *reactionLabel = new QLabel(u8"■反力マップ／組み合わせ反力マップ");
    QLabel *reactionMinusLabel = new QLabel(u8"負：");
    reactionMinusLabel->setFixedWidth(labelWidth);
    reactionMinusColorCombobox = new TypicalColorComboBox();
    reactionMinusColorCombobox->setFixedWidth(colorWidth);
    QLabel *reactionPlusLabel = new QLabel(u8"正：");
    reactionPlusLabel->setFixedWidth(labelWidth);
    reactionPlusColorCombobox = new TypicalColorComboBox();
    reactionPlusColorCombobox->setFixedWidth(colorWidth);
    QHBoxLayout *hlay_reaction = new QHBoxLayout;
    hlay_reaction->addWidget(reactionPlusLabel);
    hlay_reaction->addWidget(reactionPlusColorCombobox);
    hlay_reaction->addSpacing(30);
    hlay_reaction->addWidget(reactionMinusLabel);
    hlay_reaction->addWidget(reactionMinusColorCombobox);
    hlay_reaction->addStretch();

    // その他(変形図)
    QLabel *deformationLabel = new QLabel(u8"■変形図");
    QLabel *deformation_beforeLabel = new QLabel(u8"変形前：");
    deformation_beforeLabel->setFixedWidth(labelWidth);
    beforeColorCombobox = new TypicalColorComboBox();
    beforeColorCombobox->setFixedWidth(colorWidth);
    QLabel *deformation_afterLabel = new QLabel(u8"変形後：");
    deformation_afterLabel->setFixedWidth(labelWidth);
    afterColorCombobox = new TypicalColorComboBox();
    afterColorCombobox->setFixedWidth(colorWidth);
    QLabel *deformScaleLabel = new QLabel(u8"変形倍率：");
    deformScaleLabel->setFixedWidth(labelWidth);
    deformScaleSpin = new QSpinBox();
    deformScaleSpin->setRange(0, 10000);
    QLabel *deformScale2Label = new QLabel(u8" 倍");
    QHBoxLayout *hlay_deformation = new QHBoxLayout;
    hlay_deformation->addWidget(deformation_beforeLabel);
    hlay_deformation->addWidget(beforeColorCombobox);
    hlay_deformation->addSpacing(30);
    hlay_deformation->addWidget(deformation_afterLabel);
    hlay_deformation->addWidget(afterColorCombobox);
    hlay_deformation->addStretch();
    QHBoxLayout *hlay_deformation2 = new QHBoxLayout;
    hlay_deformation2->addWidget(deformScaleLabel);
    hlay_deformation2->addWidget(deformScaleSpin);
    hlay_deformation2->addWidget(deformScale2Label);
    hlay_deformation2->addStretch();
    QVBoxLayout *vlay_deformation = new QVBoxLayout;
    vlay_deformation->addLayout(hlay_deformation);
    vlay_deformation->addLayout(hlay_deformation2);

    // その他(検定比図)
    QLabel *checkLabel = new QLabel(u8"■検定比図");
    QLabel *check_NGLabel = new QLabel(u8"NG部材：");
    check_NGLabel->setFixedWidth(labelWidth);
    checkColorCombobox = new TypicalColorComboBox();
    checkColorCombobox->setFixedWidth(colorWidth);
    gradationColorCheck = new QCheckBox(u8"グラデーションを利用");
    QHBoxLayout *hlay_check = new QHBoxLayout;
    hlay_check->addWidget(check_NGLabel);
    hlay_check->addWidget(checkColorCombobox);
    hlay_check->addSpacing(30);
    hlay_check->addWidget(gradationColorCheck);
    hlay_check->addStretch();

    // その他(層せん断力中心・剛心伏図)
    QLabel *rigidityLable = new QLabel(u8"■層せん断力中心・剛心伏図");
    QLabel *shearForceLabel = new QLabel(u8"せん断力中心：");
    shearForceLabel->setFixedWidth(labelWidth);
    shearForceColorCombobox = new TypicalColorComboBox();
    shearForceColorCombobox->setFixedWidth(colorWidth);
    QLabel *rigidityLabel = new QLabel(u8"剛心:");
    rigidityLabel->setFixedWidth(labelWidth);
    rigidityColorCombobox = new TypicalColorComboBox();
    rigidityColorCombobox->setFixedWidth(colorWidth);
    QLabel *rigidityCircleSizeLabel = new QLabel(u8"表示サイズ:");
    rigidityCircleSizeLabel->setFixedWidth(labelWidth);
    rigidityCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_rigidity = new QHBoxLayout;
    hlay_rigidity->addWidget(shearForceLabel);
    hlay_rigidity->addWidget(shearForceColorCombobox);
    hlay_rigidity->addSpacing(30);
    hlay_rigidity->addWidget(rigidityLabel);
    hlay_rigidity->addWidget(rigidityColorCombobox);
    hlay_rigidity->addStretch();
    QHBoxLayout *hlay_rigidityCircle = new QHBoxLayout;
    hlay_rigidityCircle->addWidget(rigidityCircleSizeLabel);
    hlay_rigidityCircle->addWidget(rigidityCombobox);
    hlay_rigidityCircle->addStretch();

    QVBoxLayout *vlay_other = new QVBoxLayout;
    vlay_other->addWidget(axialLabel);
    vlay_other->addLayout(vlay_axial);
    vlay_other->addWidget(new HFLine(Qt::lightGray, this));
    vlay_other->addWidget(reactionLabel);
    vlay_other->addLayout(hlay_reaction);
    vlay_other->addWidget(new HFLine(Qt::lightGray, this));
    vlay_other->addWidget(deformationLabel);
    vlay_other->addLayout(vlay_deformation);
    vlay_other->addWidget(new HFLine(Qt::lightGray, this));
    vlay_other->addWidget(checkLabel);
    vlay_other->addLayout(hlay_check);
    vlay_other->addWidget(new HFLine(Qt::lightGray, this));
    vlay_other->addWidget(rigidityLable);
    vlay_other->addLayout(hlay_rigidity);
    vlay_other->addLayout(hlay_rigidityCircle);

    vlay_other->addStretch();

    QVBoxLayout *vlay_indivisual = new QVBoxLayout;
    vlay_indivisual->addLayout(vlay_stress, 1);
    vlay_indivisual->addWidget(new HFLine(Qt::lightGray, this));
    vlay_indivisual->addLayout(vlay_other, 1);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(hlay_scale);
    vlay->addLayout(hlay_gridfloor);
    vlay->addLayout(hlay_joint);
    vlay->addLayout(hlay_support);
    vlay->addSpacing(30);
    vlay->addLayout(hlay_line);
    vlay->addLayout(hlay_line2);
    vlay->addLayout(hlay_line3);
    vlay->addLayout(hlay_line4);
    vlay->addLayout(hlay_line5);
    vlay->addLayout(hlay_line6);
    vlay->addSpacing(30);
    vlay->addLayout(hlay_plane);
    vlay->addLayout(hlay_plane2);
    vlay->addLayout(hlay_plane3);
    vlay->addSpacing(30);
    vlay->addLayout(hlay_value);
    vlay->addLayout(hlay_select);
    vlay->addLayout(hlay_note);
    vlay->addLayout(hlay_plastic);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setContentsMargins(5, 10, 5, 10);
    hlay->setSpacing(3);
    hlay->addLayout(vlay);
    hlay->addWidget(new VFLine(Qt::lightGray, this));
    hlay->addLayout(vlay_indivisual);

    this->setLayout(hlay);
}

void Analysis2DSettingTotalWidget::initializeSettings(){
    applySettings(Analysis2DViewSettings::getDefault());
}

AnalysisCommon2DViewSetting *Analysis2DSettingTotalWidget::makeCommon() const
{
    return new AnalysisCommon2DViewSetting(
                scaleColorCombobox->color(),
                scaleSizeCombobox->currentIndex(),
                gridStringCheck->isChecked(),
                floorStringCheck->isChecked(),
                jointColorCombobox->color(),
                jointSizeCombobox->size(),
                lineRCColorCombobox->color(),
                lineSRCColorCombobox->color(),
                lineSColorCombobox->color(),
                lineCFTColorCombobox->color(),
                lineDamperColorCombobox->color(),
                lineIsoColorCombobox->color(),
                planeRCColorCombobox->color(),
                planeSColorCombobox->color(),
                planeDamperColorCombobox->color(),
                supportColorCombobox->color(),
                valueColorCombobox->color(),
                valueSizeCombobox->currentIndex(),
                selectColorCombobox->color(),
                noteColorCombobox->color(),
                noteSizeCombobox->currentIndex(),
                plasticSymbolColorCombobox->color(),
                plasticSymbolSizeCombobox->currentIndex()
                );
}

AnalysisStress2DViewSetting *Analysis2DSettingTotalWidget::makeStress() const
{
    return new AnalysisStress2DViewSetting(
                scaleColorCombobox->color(),
                scaleSizeCombobox->currentIndex(),
                gridStringCheck->isChecked(),
                floorStringCheck->isChecked(),
                jointColorCombobox->color(),
                jointSizeCombobox->size(),
                lineRCColorCombobox->color(),
                lineSRCColorCombobox->color(),
                lineSColorCombobox->color(),
                lineCFTColorCombobox->color(),
                lineDamperColorCombobox->color(),
                lineIsoColorCombobox->color(),
                planeRCColorCombobox->color(),
                planeSColorCombobox->color(),
                planeDamperColorCombobox->color(),
                supportColorCombobox->color(),
                valueColorCombobox->color(),
                valueSizeCombobox->currentIndex(),
                selectColorCombobox->color(),
                noteColorCombobox->color(),
                noteSizeCombobox->currentIndex(),
                plasticSymbolColorCombobox->color(),
                plasticSymbolSizeCombobox->currentIndex(),
                stressColorCombobox->color(),
                stressScaleSpin->value(),
                NValueCheck->isChecked(),
                MValueCheck->isChecked(),
                QValueCheck->isChecked(),
                columnValueCheck->isChecked(),
                stressColumnColorCombobox->color(),
                girderValueCheck->isChecked(),
                stressGirderColorCombobox->color(),
                braceValueCheck->isChecked(),
                stressBraceColorCombobox->color(),
                wallValueCheck->isChecked(),
                stressWallColorCombobox->color(),
                slabValueCheck->isChecked(),
                stressSlabColorCombobox->color(),
                panelValueCheck->isChecked(),
                stressPanelColorCombobox->color(),
                springValueCheck->isChecked(),
                stressSpringColorCombobox->color(),
                damperValueCheck->isChecked(),
                stressDamperColorCombobox->color(),
                isoValueCheck->isChecked(),
                stressIsoColorCombobox->color()
                );
}

AnalysisReactionForce2DViewSetting *Analysis2DSettingTotalWidget::makeReaction() const
{
    return new AnalysisReactionForce2DViewSetting(
                scaleColorCombobox->color(),
                scaleSizeCombobox->currentIndex(),
                gridStringCheck->isChecked(),
                floorStringCheck->isChecked(),
                jointColorCombobox->color(),
                jointSizeCombobox->size(),
                lineRCColorCombobox->color(),
                lineSRCColorCombobox->color(),
                lineSColorCombobox->color(),
                lineCFTColorCombobox->color(),
                lineDamperColorCombobox->color(),
                lineIsoColorCombobox->color(),
                planeRCColorCombobox->color(),
                planeSColorCombobox->color(),
                planeDamperColorCombobox->color(),
                supportColorCombobox->color(),
                valueColorCombobox->color(),
                valueSizeCombobox->currentIndex(),
                selectColorCombobox->color(),
                noteColorCombobox->color(),
                noteSizeCombobox->currentIndex(),
                plasticSymbolColorCombobox->color(),
                plasticSymbolSizeCombobox->currentIndex(),
                reactionMinusColorCombobox->color(),
                reactionPlusColorCombobox->color()
                );
}

AnalysisAxialForce2DViewSetting *Analysis2DSettingTotalWidget::makeAxial() const
{
    return new AnalysisAxialForce2DViewSetting(
                scaleColorCombobox->color(),
                scaleSizeCombobox->currentIndex(),
                gridStringCheck->isChecked(),
                floorStringCheck->isChecked(),
                jointColorCombobox->color(),
                jointSizeCombobox->size(),
                lineRCColorCombobox->color(),
                lineSRCColorCombobox->color(),
                lineSColorCombobox->color(),
                lineCFTColorCombobox->color(),
                lineDamperColorCombobox->color(),
                lineIsoColorCombobox->color(),
                planeRCColorCombobox->color(),
                planeSColorCombobox->color(),
                planeDamperColorCombobox->color(),
                supportColorCombobox->color(),
                valueColorCombobox->color(),
                valueSizeCombobox->currentIndex(),
                selectColorCombobox->color(),
                noteColorCombobox->color(),
                noteSizeCombobox->currentIndex(),
                plasticSymbolColorCombobox->color(),
                plasticSymbolSizeCombobox->currentIndex(),
                axialMinusColorCombobox->color(),
                axialPlusColorCombobox->color(),
                axialScaleSpin->value()
                );
}

AnalysisDeformation2DViewSetting *Analysis2DSettingTotalWidget::makeDeformation() const
{
    return new AnalysisDeformation2DViewSetting(
                scaleColorCombobox->color(),
                scaleSizeCombobox->currentIndex(),
                gridStringCheck->isChecked(),
                floorStringCheck->isChecked(),
                jointColorCombobox->color(),
                jointSizeCombobox->size(),
                lineRCColorCombobox->color(),
                lineSRCColorCombobox->color(),
                lineSColorCombobox->color(),
                lineCFTColorCombobox->color(),
                lineDamperColorCombobox->color(),
                lineIsoColorCombobox->color(),
                planeRCColorCombobox->color(),
                planeSColorCombobox->color(),
                planeDamperColorCombobox->color(),
                supportColorCombobox->color(),
                valueColorCombobox->color(),
                valueSizeCombobox->currentIndex(),
                selectColorCombobox->color(),
                noteColorCombobox->color(),
                noteSizeCombobox->currentIndex(),
                plasticSymbolColorCombobox->color(),
                plasticSymbolSizeCombobox->currentIndex(),
                beforeColorCombobox->color(),
                afterColorCombobox->color(),
                deformScaleSpin->value()
                );
}

AnalysisCheck2DViewSetting *Analysis2DSettingTotalWidget::makeCheck() const
{
    return new AnalysisCheck2DViewSetting(
                scaleColorCombobox->color(),
                scaleSizeCombobox->currentIndex(),
                gridStringCheck->isChecked(),
                floorStringCheck->isChecked(),
                jointColorCombobox->color(),
                jointSizeCombobox->size(),
                lineRCColorCombobox->color(),
                lineSRCColorCombobox->color(),
                lineSColorCombobox->color(),
                lineCFTColorCombobox->color(),
                lineDamperColorCombobox->color(),
                lineIsoColorCombobox->color(),
                planeRCColorCombobox->color(),
                planeSColorCombobox->color(),
                planeDamperColorCombobox->color(),
                supportColorCombobox->color(),
                valueColorCombobox->color(),
                valueSizeCombobox->currentIndex(),
                selectColorCombobox->color(),
                noteColorCombobox->color(),
                noteSizeCombobox->currentIndex(),
                plasticSymbolColorCombobox->color(),
                plasticSymbolSizeCombobox->currentIndex(),
                checkColorCombobox->color(),
                gradationColorCheck->isChecked()
                );
}

AnalysisCenterOfRigidity2DViewSetting *Analysis2DSettingTotalWidget::makeCenter() const
{
    return new AnalysisCenterOfRigidity2DViewSetting(
                scaleColorCombobox->color(),
                scaleSizeCombobox->currentIndex(),
                gridStringCheck->isChecked(),
                floorStringCheck->isChecked(),
                jointColorCombobox->color(),
                jointSizeCombobox->size(),
                lineRCColorCombobox->color(),
                lineSRCColorCombobox->color(),
                lineSColorCombobox->color(),
                lineCFTColorCombobox->color(),
                lineDamperColorCombobox->color(),
                lineIsoColorCombobox->color(),
                planeRCColorCombobox->color(),
                planeSColorCombobox->color(),
                planeDamperColorCombobox->color(),
                supportColorCombobox->color(),
                valueColorCombobox->color(),
                valueSizeCombobox->currentIndex(),
                selectColorCombobox->color(),
                noteColorCombobox->color(),
                noteSizeCombobox->currentIndex(),
                plasticSymbolColorCombobox->color(),
                plasticSymbolSizeCombobox->currentIndex(),
                shearForceColorCombobox->color(),
                rigidityColorCombobox->color(),
                rigidityCombobox->currentIndex()
                );
}

QHash<int, AnalysisBaseViewSettings *> Analysis2DSettingTotalWidget::getSettings() const
{
    QHash<int, AnalysisBaseViewSettings*> ret;
    ret.insert(static_cast<int>(OUTFIGTYPE::LENGTH), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::FACELENGTH), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::COORDINATE), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::RIGIDLENGTH), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::LATERALBUCKLEN), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::EULERBUCKLEN), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::JOINTMASS), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::OUTERFORCE), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::OUTERMOMENT), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::RIGIDITYPHI), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::CMQ), makeStress());
    ret.insert(static_cast<int>(OUTFIGTYPE::AXIALMAP), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::REACTMAP), makeReaction());
    ret.insert(static_cast<int>(OUTFIGTYPE::COMBIREACTMAP), makeReaction());
    ret.insert(static_cast<int>(OUTFIGTYPE::SHEARMAP), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::VERTICALLOADMAP), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::LAYERDEFORMMAP), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::LAYERSTIFFMAP), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::GCKCP), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::NSTRESS), makeStress());
    ret.insert(static_cast<int>(OUTFIGTYPE::FSTRESS), makeStress());
    ret.insert(static_cast<int>(OUTFIGTYPE::RSTRESS), makeStress());
    ret.insert(static_cast<int>(OUTFIGTYPE::AXIAL), makeAxial());
    ret.insert(static_cast<int>(OUTFIGTYPE::TORSION), makeStress());
    ret.insert(static_cast<int>(OUTFIGTYPE::DEFORM), makeDeformation());
    ret.insert(static_cast<int>(OUTFIGTYPE::DISP), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::CHECKRATIO), makeCheck());
    ret.insert(static_cast<int>(OUTFIGTYPE::ALLOWRATIOE), makeCheck());
    ret.insert(static_cast<int>(OUTFIGTYPE::ALLOWRATIOU), makeCheck());
    ret.insert(static_cast<int>(OUTFIGTYPE::PLASTICRATIO), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::RANK), makeStress());
    ret.insert(static_cast<int>(OUTFIGTYPE::ALLOW), makeStress());
    ret.insert(static_cast<int>(OUTFIGTYPE::STRENGTH), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::YIELDSTEP), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::YIELDLOADMAG), makeCommon());
    ret.insert(static_cast<int>(OUTFIGTYPE::CenterOfRigidity), makeCenter());
    return ret;
}

void Analysis2DSettingTotalWidget::applySettings(const Analysis2DViewSettings &settings)
{
    auto common = static_cast<AnalysisCommon2DViewSetting*>(settings.getSetting(OUTFIGTYPE::LENGTH));
    scaleColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getScaleColor()));
    scaleSizeCombobox->setCurrentIndex(common->getScaleSize());
    gridStringCheck->setChecked(common->getGridStringVisible());
    floorStringCheck->setChecked(common->getFloorStringVisible());
    jointColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getJointColor()));
    jointSizeCombobox->setCurrentIndex(common->getJointSize());
    lineRCColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getLineRcColor()));
    lineSRCColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getLineSrcColor()));
    lineSColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getLineSColor()));
    lineCFTColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getLineCftColor()));
    lineDamperColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getLineDamperColor()));
    lineIsoColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getLineIsoColor()));
    planeRCColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getPlaneRcColor()));
    planeSColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getPlaneSColor()));
    planeDamperColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getPlaneDamperColor()));
    supportColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getSupportColor()));
    valueColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getValueColor()));
    valueSizeCombobox->setCurrentIndex(common->getValueTextSize());
    selectColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getSelectedColor()));
    noteColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getNoteColor()));
    noteSizeCombobox->setCurrentIndex(common->getNoteTextSize());
    plasticSymbolColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(common->getPlasticSymbolColor()));
    plasticSymbolSizeCombobox->setCurrentIndex(common->getPlasticSymbolSize());

    auto stress = static_cast<AnalysisStress2DViewSetting*>(settings.getSetting(OUTFIGTYPE::CMQ));
    stressColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getStressColor()));
    stressScaleSpin->setValue(stress->getStressScale());
    NValueCheck->setChecked(stress->getValueNVisible());
    MValueCheck->setChecked(stress->getValueMVisible());
    QValueCheck->setChecked(stress->getValueQVisible());
    columnValueCheck->setChecked(stress->getColumnStressVisible());
    stressColumnColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getColumnStressValueColor()));
    girderValueCheck->setChecked(stress->getGirderStressVisible());
    stressGirderColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getGirderStressValueColor()));
    braceValueCheck->setChecked(stress->getBraceStressVisible());
    stressBraceColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getBraceStressValueColor()));
    wallValueCheck->setChecked(stress->getWallStressVisible());
    stressWallColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getWallStressValueColor()));
    slabValueCheck->setChecked(stress->getSlabStressVisible());
    stressSlabColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getSlabStressValueColor()));
    panelValueCheck->setChecked(stress->getPanelStressVisible());
    stressPanelColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getPanelStressValueColor()));
    springValueCheck->setChecked(stress->getSpringStressVisible());
    stressSpringColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getSpringStressValueColor()));
    damperValueCheck->setChecked(stress->geteDamperStressVisible());
    stressDamperColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getDamperStressValueColor()));
    isoValueCheck->setChecked(stress->getIsoStressVisible());
    stressIsoColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(stress->getIsoStressValueColor()));

    auto reaction = static_cast<AnalysisReactionForce2DViewSetting*>(settings.getSetting(OUTFIGTYPE::REACTMAP));
    reactionMinusColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(reaction->getMinusColor()));
    reactionPlusColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(reaction->getPlusColor()));

    auto axial = static_cast<AnalysisAxialForce2DViewSetting*>(settings.getSetting(OUTFIGTYPE::AXIAL));
    axialMinusColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(axial->getAxialMinusColor()));
    axialPlusColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(axial->getAxialPlusColor()));
    axialScaleSpin->setValue(axial->getAxialScale());

    auto deformation = static_cast<AnalysisDeformation2DViewSetting*>(settings.getSetting(OUTFIGTYPE::DEFORM));
    beforeColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(deformation->getBeforeColor()));
    afterColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(deformation->getAfterColor()));
    deformScaleSpin->setValue(deformation->getDeformScale());

    auto check = static_cast<AnalysisCheck2DViewSetting*>(settings.getSetting(OUTFIGTYPE::CHECKRATIO));
    checkColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(check->getNgColor()));
    gradationColorCheck->setChecked(check->getUseGradiationColor());

    auto center = static_cast<AnalysisCenterOfRigidity2DViewSetting*>(settings.getSetting(OUTFIGTYPE::CenterOfRigidity));
    shearForceColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(center->getShearForceColor()));
    rigidityColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(center->getRigidityColor()));
    rigidityCombobox->setCurrentIndex(center->getRigiditySize());
}

} // namespace post3dapp

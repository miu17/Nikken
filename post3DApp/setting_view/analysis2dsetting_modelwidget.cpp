#include "analysis2dsetting_modelwidget.h"

#include "define_output.h"
#include "define_analysis2dsettings.h"
#include "typicalcolor_combobox.h"
#include "vhline.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{

Analysis2DSettingModelWidget::Analysis2DSettingModelWidget(const QString& _tabName, const QString& _englishTabName, QWidget *parent)
    : BaseSettingWidget(parent), TabName(_tabName), englishTabName(_englishTabName)
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

    // 符号
    QLabel *markLabel = new QLabel(u8"符号の表示：");
    markLabel->setFixedWidth(labelWidth);
    columnMarkCheck = new QCheckBox(u8"柱符号");
    girderMarkCheck = new QCheckBox(u8"大梁符号");
    braceMarkCheck = new QCheckBox(u8"ブレース符号");
    wallMarkCheck = new QCheckBox(u8"壁符号");
    steelWallMarkCheck = new QCheckBox(u8"鋼板壁符号");
    foundationWallMarkCheck = new QCheckBox(u8"地下外壁符号");
    slabMarkCheck = new QCheckBox(u8"スラブ符号");
    damperMarkCheck = new QCheckBox(u8"制振部材符号");
    isoMarkCheck = new QCheckBox(u8"免震部材符号");

    QHBoxLayout *hlay_mark1 = new QHBoxLayout;
    hlay_mark1->setContentsMargins(0, 0, 0, 0);
    hlay_mark1->addWidget(markLabel);
    hlay_mark1->addWidget(columnMarkCheck);
    hlay_mark1->addWidget(girderMarkCheck);
    hlay_mark1->addWidget(braceMarkCheck);
    QHBoxLayout *hlay_mark2 = new QHBoxLayout;
    hlay_mark2->setContentsMargins(0, 0, 0, 0);
    hlay_mark2->addSpacing(labelWidth+5);
    hlay_mark2->addWidget(wallMarkCheck);
    hlay_mark2->addWidget(steelWallMarkCheck);
    hlay_mark2->addWidget(foundationWallMarkCheck);
    QHBoxLayout *hlay_mark3 = new QHBoxLayout;
    hlay_mark3->setContentsMargins(0, 0, 0, 0);
    hlay_mark3->addSpacing(labelWidth+5);
    hlay_mark3->addWidget(slabMarkCheck);
    hlay_mark3->addWidget(damperMarkCheck);
    hlay_mark3->addWidget(isoMarkCheck);

    //符号サイズ
    QLabel *markSizeLabel = new QLabel(u8"符号サイズ:");
    markSizeLabel->setFixedWidth(labelWidth);
    markSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_markSize = new QHBoxLayout;
    hlay_markSize->setContentsMargins(0, 0, 0, 0);
    hlay_markSize->addWidget(markSizeLabel);
    hlay_markSize->addWidget(markSizeCombobox);
    hlay_markSize->addStretch();

    //節点番号
    QLabel *jointNumberLabel = new QLabel(u8"節点番号:");
    jointNumberLabel->setFixedWidth(labelWidth);
    jointNumberCheck = new QCheckBox(u8"表示");
//    QLabel *jointNumberColorLabel = new QLabel(u8"カラー：");
    jointNumberColorCombobox = new TypicalColorComboBox();
    jointNumberColorCombobox->setFixedWidth(colorWidth);
//    QLabel *jointNumberSizeLabel = new QLabel(u8"サイズ：");
    jointNumberSizeCombobox = new FiveSizeComboBox();

    QHBoxLayout *hlay_jointNumber = new QHBoxLayout;
    hlay_jointNumber->setContentsMargins(0, 0, 0, 0);
    hlay_jointNumber->addWidget(jointNumberLabel);
    hlay_jointNumber->addWidget(jointNumberCheck);
//    hlay_jointNumber->addWidget(jointNumberColorLabel);
    hlay_jointNumber->addWidget(jointNumberColorCombobox);
//    hlay_jointNumber->addWidget(jointNumberSizeLabel);
    hlay_jointNumber->addWidget(jointNumberSizeCombobox);
    hlay_jointNumber->addStretch();

    //境界条件
    QLabel *boundaryConditionLabel = new QLabel(u8"境界条件:");
    boundaryConditionLabel->setFixedWidth(labelWidth);
    boundaryConditionCheck = new QCheckBox(u8"表示");
//    QLabel *boundaryConditionColorLabel = new QLabel(u8"カラー：");
    boundaryConditionColorCombobox = new TypicalColorComboBox();
    boundaryConditionColorCombobox->setFixedWidth(colorWidth);
//    QLabel *boundaryConditionSizeLabel = new QLabel(u8"サイズ：");
    boundaryConditionSizeCombobox = new FiveSizeComboBox();

    QHBoxLayout *hlay_boundaryCondition = new QHBoxLayout;
    hlay_boundaryCondition->setContentsMargins(0, 0, 0, 0);
    hlay_boundaryCondition->addWidget(boundaryConditionLabel);
    hlay_boundaryCondition->addWidget(boundaryConditionCheck);
//    hlay_boundaryCondition->addWidget(boundaryConditionColorLabel);
    hlay_boundaryCondition->addWidget(boundaryConditionColorCombobox);
//    hlay_boundaryCondition->addWidget(boundaryConditionSizeLabel);
    hlay_boundaryCondition->addWidget(boundaryConditionSizeCombobox);
    hlay_boundaryCondition->addStretch();

    // コードアングルの表示
    directionCheckbox = new QCheckBox(u8"線材の入力方向（i端→j端）を示す記号を表示.", this);
    QHBoxLayout *hlay_direction = new QHBoxLayout;
    hlay_direction->setContentsMargins(0, 0, 0, 0);
    hlay_direction->addWidget(directionCheckbox);
    hlay_direction->addStretch();

    // 雑壁・剛域・パネルゾーン
    QLabel *zwallColorLabel = new QLabel(u8"雑壁：");
    zwallColorLabel->setFixedWidth(labelWidth);
    zwallColorCombobox = new TypicalColorComboBox();
    zwallColorCombobox->setFixedWidth(colorWidth);
    QLabel *rigidRangeColorLabel = new QLabel(u8"剛域：");
    rigidRangeColorLabel->setFixedWidth(labelWidth);
    rigidRangeColorCombobox = new TypicalColorComboBox();
    rigidRangeColorCombobox->setFixedWidth(colorWidth);
    QLabel *panelZoneColorLabel = new QLabel(u8"パネルゾーン：");
    panelZoneColorLabel->setFixedWidth(labelWidth);
    panelZoneColorCombobox = new TypicalColorComboBox();
    panelZoneColorCombobox->setFixedWidth(colorWidth);
    QHBoxLayout *hlay_zwall = new QHBoxLayout;
//    hlay_zwall->setContentsMargins(0, 0, 0, 0);
    hlay_zwall->addWidget(zwallColorLabel);
    hlay_zwall->addWidget(zwallColorCombobox);
    hlay_zwall->addStretch();
    QHBoxLayout *hlay_zwall2 = new QHBoxLayout;
    hlay_zwall2->addWidget(rigidRangeColorLabel);
    hlay_zwall2->addWidget(rigidRangeColorCombobox);
    hlay_zwall2->addStretch();
    QHBoxLayout *hlay_zwall3 = new QHBoxLayout;
    hlay_zwall3->addWidget(panelZoneColorLabel);
    hlay_zwall3->addWidget(panelZoneColorCombobox);
    hlay_zwall3->addStretch();


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


    QVBoxLayout *vlay2 = new QVBoxLayout;
//    vlay2->addWidget(markLabel);
    vlay2->addLayout(hlay_mark1);
    vlay2->addLayout(hlay_mark2);
    vlay2->addLayout(hlay_mark3);
    vlay2->addLayout(hlay_markSize);
    vlay2->addSpacing(30);
//    vlay2->addWidget(jointNumberLabel);
    vlay2->addLayout(hlay_jointNumber);
//    vlay2->addWidget(boundaryConditionLabel);
    vlay2->addLayout(hlay_boundaryCondition);
    vlay2->addSpacing(30);
    vlay2->addLayout(hlay_zwall);
    vlay2->addLayout(hlay_zwall2);
    vlay2->addLayout(hlay_zwall3);
    vlay2->addSpacing(30);
    vlay2->addLayout(hlay_direction);

    vlay2->addStretch();

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setContentsMargins(5, 10, 5, 10);
    hlay->setSpacing(3);
    hlay->addLayout(vlay);
    hlay->addWidget(new VFLine(Qt::lightGray, this));
    hlay->addLayout(vlay2);

    this->setLayout(hlay);

    initializeSettings();
}

QHash<int, AnalysisBaseViewSettings *> Analysis2DSettingModelWidget::getSettings() const
{
    AnalysisModel2DViewSetting* setting = new AnalysisModel2DViewSetting(
                scaleColorCombobox->color(),
                scaleSizeCombobox->currentIndex(),
                gridStringCheck->isChecked(),
                floorStringCheck->isChecked(),
                jointColorCombobox->color(),
                jointSizeCombobox->currentIndex(),
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
                columnMarkCheck->isChecked(),
                girderMarkCheck->isChecked(),
                braceMarkCheck->isChecked(),
                wallMarkCheck->isChecked(),
                steelWallMarkCheck->isChecked(),
                foundationWallMarkCheck->isChecked(),
                slabMarkCheck->isChecked(),
                damperMarkCheck->isChecked(),
                isoMarkCheck->isChecked(),
                markSizeCombobox->currentIndex(),
                jointNumberCheck->isChecked(),
                jointNumberColorCombobox->name(),
                jointNumberSizeCombobox->currentIndex(),
                boundaryConditionCheck->isChecked(),
                boundaryConditionColorCombobox->color(),
                boundaryConditionSizeCombobox->currentIndex(),
                directionCheckbox->isChecked(),
                zwallColorCombobox->color(),
                rigidRangeColorCombobox->color(),
                panelZoneColorCombobox->color()
                );
    QHash<int, AnalysisBaseViewSettings*> ret;
    ret.insert(static_cast<int>(OUTFIGTYPE::MODEL), setting);
    return ret;
}

void Analysis2DSettingModelWidget::applySettings(const Analysis2DViewSettings &settings)
{
    auto model = static_cast<AnalysisModel2DViewSetting*>(settings.getSetting(OUTFIGTYPE::MODEL));
    scaleColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getScaleColor()));
    scaleSizeCombobox->setCurrentIndex(model->getScaleSize());
    gridStringCheck->setChecked(model->getGridStringVisible());
    floorStringCheck->setChecked(model->getFloorStringVisible());
    jointColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getJointColor()));
    jointSizeCombobox->setCurrentIndex(model->getJointSize());
    lineRCColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getLineRcColor()));
    lineSRCColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getLineSrcColor()));
    lineSColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getLineSColor()));
    lineCFTColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getLineCftColor()));
    lineDamperColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getLineDamperColor()));
    lineIsoColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getLineIsoColor()));
    planeRCColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getPlaneRcColor()));
    planeSColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getPlaneSColor()));
    planeDamperColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getPlaneDamperColor()));
    supportColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getSupportColor()));
    valueColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getValueColor()));
    valueSizeCombobox->setCurrentIndex(model->getValueTextSize());
    selectColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getSelectedColor()));
    noteColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getNoteColor()));
    noteSizeCombobox->setCurrentIndex(model->getNoteTextSize());
    columnMarkCheck->setChecked(model->getColumnMarkVisible());
    girderMarkCheck->setChecked(model->getGirderMarkVisible());
    braceMarkCheck->setChecked(model->getBraceMarkVisible());
    wallMarkCheck->setChecked(model->getWallMarkVisible());
    steelWallMarkCheck->setChecked(model->getSteelWallMarkVisible());
    foundationWallMarkCheck->setChecked(model->getFoundationWallMarkVisible());
    slabMarkCheck->setChecked(model->getSlabMarkVisible());
    damperMarkCheck->setChecked(model->getDamperMarkVisible());
    isoMarkCheck->setChecked(model->getIsoMarkVisible());
    markSizeCombobox->setCurrentIndex(model->getMarkSize());
    jointNumberCheck->setChecked(model->getJointNumberVisible());
    jointNumberColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getJointNumberColor()));
    jointNumberSizeCombobox->setCurrentIndex(model->getJointNumberSize());
    boundaryConditionCheck->setChecked(model->getBoundaryConditionVisible());
    boundaryConditionColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getBoundaryConditionColor()));
    boundaryConditionSizeCombobox->setCurrentIndex(model->getBoundaryConditionSize());
    directionCheckbox->setChecked(model->getDirectionVisible());
    zwallColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getZwallColor()));
    rigidRangeColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getRigidRangeColor()));
    panelZoneColorCombobox->setCurrentIndex(TypicalColorComboBox::colorToIndex(model->getPanelZoneColor()));
}

void Analysis2DSettingModelWidget::initializeSettings(){
    applySettings(Analysis2DViewSettings::getDefault());
}


} // namespace post3dapp

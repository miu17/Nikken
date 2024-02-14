#include "analysis2dsetting_graphwidget.h"

#include "define_output.h"
#include "define_analysis2dsettings.h"
#include "typicalcolor_combobox.h"
#include "vhline.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{

Analysis2DSettingGraphWidget::Analysis2DSettingGraphWidget(const QString& tabName, const QString& _englishTabName, QWidget* parent)
    : QWidget(parent), TabName(tabName), englishTabName(_englishTabName)
{
    //Qδ,Qθ図
    QLabel *QDGraphLabel = new QLabel(u8"■ Qδ図　Qθ図：");
    QVBoxLayout *vlay_qd = new QVBoxLayout;

    QLabel *qd_axisLabelSizeLabel = new QLabel(u8"軸ラベルの大きさ：");
    QLabel *qd_axisValueSizeLabel = new QLabel(u8"軸目盛り数値の大きさ：");
    QLabel *qd_titleLabelSizeLabel = new QLabel(u8"タイトルの大きさ：");
    QLabel *qd_plotPointSizeLabel = new QLabel(u8"プロット点の大きさ：");
    QLabel *qd_plotPointLabelSizeLabel = new QLabel(u8"プロット点ラベルの大きさ：");
    qd_axisValueSizeCombobox = new FiveSizeComboBox();
    qd_axisLabelSizeCombobox = new FiveSizeComboBox();
    qd_titleLabelSizeCombobox = new FiveSizeComboBox();
    qd_plotPointSizeCombobox = new FiveSizeComboBox();
    qd_plotPointLabelSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *qd_hlay1 = new QHBoxLayout;
    QHBoxLayout *qd_hlay2 = new QHBoxLayout;
    QHBoxLayout *qd_hlay3 = new QHBoxLayout;
    QHBoxLayout *qd_hlay4 = new QHBoxLayout;
    QHBoxLayout *qd_hlay5 = new QHBoxLayout;
    qd_hlay1->addWidget(qd_plotPointSizeLabel);
    qd_hlay1->addWidget(qd_plotPointSizeCombobox);
    qd_hlay2->addWidget(qd_plotPointLabelSizeLabel);
    qd_hlay2->addWidget(qd_plotPointLabelSizeCombobox);
    qd_hlay3->addWidget(qd_axisLabelSizeLabel);
    qd_hlay3->addWidget(qd_axisLabelSizeCombobox);
    qd_hlay4->addWidget(qd_axisValueSizeLabel);
    qd_hlay4->addWidget(qd_axisValueSizeCombobox);
    qd_hlay5->addWidget(qd_titleLabelSizeLabel);
    qd_hlay5->addWidget(qd_titleLabelSizeCombobox);

    qd_minmax_q=new AxisMinMaxWidget(u8"Q軸");
    qd_minmax_d=new AxisMinMaxWidget(u8"δ軸");
    qd_minmax_rad=new AxisMinMaxWidget(u8"θ軸");
    qd_style_ruler=new LineStyleWidget(u8"罫線の表示");
    vlay_qd->addWidget(QDGraphLabel);
    vlay_qd->addLayout(qd_hlay3);
    vlay_qd->addLayout(qd_hlay4);
    vlay_qd->addLayout(qd_hlay5);
    vlay_qd->addLayout(qd_hlay1);
    vlay_qd->addLayout(qd_hlay2);
    vlay_qd->addWidget(qd_minmax_q);
    vlay_qd->addWidget(qd_minmax_d);
    vlay_qd->addWidget(qd_minmax_rad);
    vlay_qd->addWidget(qd_style_ruler);

    //保有水平耐力図
    QLabel *CapacityGraphLabel = new QLabel(u8"■ 必要保有水平耐力図：");
    QVBoxLayout *vlay_capa = new QVBoxLayout;

    QLabel *capa_axisLabelSizeLabel = new QLabel(u8"軸ラベルの大きさ：");
    QLabel *capa_axisValueSizeLabel = new QLabel(u8"軸目盛り数値の大きさ：");
    QLabel *capa_titleLabelSizeLabel = new QLabel(u8"タイトルの大きさ：");
    capa_axisValueSizeCombobox = new FiveSizeComboBox();
    capa_axisLabelSizeCombobox = new FiveSizeComboBox();
    capa_titleLabelSizeCombobox = new FiveSizeComboBox();
    capa_minmax_q=new AxisMinMaxWidget(u8"Q軸");
    capa_style_u=new LineStyleWidget(u8"保有水平耐力の表示");
    capa_style_un=new LineStyleWidget(u8"必要保有水平耐力の表示");
    capa_style_ruler=new LineStyleWidget(u8"罫線の表示");
    QHBoxLayout *capa_hlay1 = new QHBoxLayout;
    QHBoxLayout *capa_hlay2 = new QHBoxLayout;
    QHBoxLayout *capa_hlay3 = new QHBoxLayout;
    capa_hlay1->addWidget(capa_axisLabelSizeLabel);
    capa_hlay1->addWidget(capa_axisValueSizeCombobox);
    capa_hlay2->addWidget(capa_axisValueSizeLabel);
    capa_hlay2->addWidget(capa_axisLabelSizeCombobox);
    capa_hlay3->addWidget(capa_titleLabelSizeLabel);
    capa_hlay3->addWidget(capa_titleLabelSizeCombobox);

    vlay_capa->addWidget(CapacityGraphLabel);
    vlay_capa->addLayout(capa_hlay1);
    vlay_capa->addLayout(capa_hlay2);
    vlay_capa->addLayout(capa_hlay3);
    vlay_capa->addWidget(capa_minmax_q);
    vlay_capa->addWidget(capa_style_u);
    vlay_capa->addWidget(capa_style_un);
    vlay_capa->addWidget(capa_style_ruler);

    //NMインタラクション図
    QLabel *nmintLabel = new QLabel(u8"■ NM-インタラクション図");
    QVBoxLayout *vlay_nm = new QVBoxLayout;

    QLabel *nm_axisLabelSizeLabel = new QLabel(u8"軸ラベルの大きさ：");
    QLabel *nm_axisValueSizeLabel = new QLabel(u8"軸目盛り数値の大きさ：");
    QLabel *nm_titleLabelSizeLabel = new QLabel(u8"タイトルの大きさ：");
    QLabel *nm_plotPointSizeLabel = new QLabel(u8"プロット点の大きさ：");
    QLabel *nm_plotPointLabelSizeLabel = new QLabel(u8"プロット点ラベルの大きさ：");
    nm_axisValueSizeCombobox = new FiveSizeComboBox();
    nm_axisLabelSizeCombobox = new FiveSizeComboBox();
    nm_titleLabelSizeCombobox = new FiveSizeComboBox();
    nm_plotPointSizeCombobox = new FiveSizeComboBox();
    nm_plotPointLabelSizeCombobox = new FiveSizeComboBox();

    nm_minmax_N=new AxisMinMaxWidget(u8"N軸");
    nm_minmax_My=new AxisMinMaxWidget(u8"My軸");
    nm_minmax_Mz=new AxisMinMaxWidget(u8"Mz軸");
    //スタイルを変更したら凡例も変える必要がある
    nm_style_l=new LineStyleWidget(u8"長期許容応力の表示");
    nm_style_s=new LineStyleWidget(u8"短期許容応力の表示");
    nm_style_u=new LineStyleWidget(u8"耐力の表示");
    nm_style_kotei=new LineStyleWidget(u8"固定成分の表示");
    nm_style_hendou=new LineStyleWidget(u8"変動成分の表示");
    nm_style_ruler=new LineStyleWidget(u8"罫線の表示");
//    nm_style_kotei->visible
//    nm_style_hendou->setVisible(true);
    QHBoxLayout *nm_hlay1 = new QHBoxLayout;
    nm_hlay1->addWidget(nm_axisLabelSizeLabel);
    nm_hlay1->addWidget(nm_axisLabelSizeCombobox);
    QHBoxLayout *nm_hlay2 = new QHBoxLayout;
    nm_hlay2->addWidget(nm_axisValueSizeLabel);
    nm_hlay2->addWidget(nm_axisValueSizeCombobox);
    QHBoxLayout *nm_hlay3 = new QHBoxLayout;
    nm_hlay3->addWidget(nm_titleLabelSizeLabel);
    nm_hlay3->addWidget(nm_titleLabelSizeCombobox);
    QHBoxLayout *nm_hlay4 = new QHBoxLayout;
    nm_hlay4->addWidget(nm_plotPointSizeLabel);
    nm_hlay4->addWidget(nm_plotPointSizeCombobox);
    QHBoxLayout *nm_hlay5 = new QHBoxLayout;
    nm_hlay5->addWidget(nm_plotPointLabelSizeLabel);
    nm_hlay5->addWidget(nm_plotPointLabelSizeCombobox);
    vlay_nm->addWidget(nmintLabel);
    vlay_nm->addLayout(nm_hlay1);
    vlay_nm->addLayout(nm_hlay2);
    vlay_nm->addLayout(nm_hlay3);
    vlay_nm->addLayout(nm_hlay4);
    vlay_nm->addLayout(nm_hlay5);
    vlay_nm->addWidget(nm_minmax_N);
    vlay_nm->addWidget(nm_minmax_My);
    vlay_nm->addWidget(nm_minmax_Mz);
    vlay_nm->addWidget(nm_style_l);
    vlay_nm->addWidget(nm_style_s);
    vlay_nm->addWidget(nm_style_u);
    vlay_nm->addWidget(nm_style_kotei);
    vlay_nm->addWidget(nm_style_hendou);
    vlay_nm->addWidget(nm_style_ruler);

    //組み立て
    QVBoxLayout *vlay_col1 = new QVBoxLayout;
    QVBoxLayout *vlay_col2 = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    vlay_col1->addLayout(vlay_qd);
    vlay_col1->addWidget(new HFLine(Qt::lightGray, this));
    vlay_col1->addLayout(vlay_capa);
    vlay_col2->addLayout(vlay_nm);
    hlay->addLayout(vlay_col1);
    hlay->addWidget(new VFLine(Qt::lightGray, this));
    hlay->addLayout(vlay_col2);
    this->setLayout(hlay);


    initializeSettings();
}

QHash<int, AnalysisBaseViewSettings *> Analysis2DSettingGraphWidget::getSettings() const
{
    QHash<int, AnalysisBaseViewSettings*> ret;
    ret.insert(static_cast<int>(OUTFIGTYPE::QDelta), makeQDelta());
    ret.insert(static_cast<int>(OUTFIGTYPE::QDeltaFrameFloors), makeQDelta());
    ret.insert(static_cast<int>(OUTFIGTYPE::QDeltaFloorFrames), makeQDelta());
    ret.insert(static_cast<int>(OUTFIGTYPE::QTheta), makeQDelta());
    ret.insert(static_cast<int>(OUTFIGTYPE::QThetaFrameFloors), makeQDelta());
    ret.insert(static_cast<int>(OUTFIGTYPE::QThetaFloorFrames), makeQDelta());
    ret.insert(static_cast<int>(OUTFIGTYPE::QuNeedf), makeHorizontalCapacity());
    ret.insert(static_cast<int>(OUTFIGTYPE::NMINT), makeNmint());
    return ret;
}

void Analysis2DSettingGraphWidget::applySettings(const Analysis2DViewSettings &settings)
{
    auto delta = static_cast<AnalysisGraphQDelta2DViewSetting*>(settings.getSetting(OUTFIGTYPE::QDelta));
    qd_axisLabelSizeCombobox->setCurrentIndex(delta->getAxisLabelSize());
    qd_axisValueSizeCombobox->setCurrentIndex(delta->getAxisScaleTextSize());
    qd_titleLabelSizeCombobox->setCurrentIndex(delta->getTitleTextSize());
    qd_plotPointSizeCombobox->setCurrentIndex(delta->getSymbolSize());
    qd_plotPointLabelSizeCombobox->setCurrentIndex(delta->getSymbolLabelSize());
    qd_minmax_q->setSetting(delta->getMinMaxAutoQ(),
                            delta->getMinMaxMinQ(),
                            delta->getMinMaxMaxQ());
    qd_minmax_d->setSetting(delta->getMinMaxAutoD(),
                            delta->getMinMaxMinD(),
                            delta->getMinMaxMaxD());
    qd_minmax_rad->setSetting(delta->getMinMaxAutoRad(),
                              delta->getMinMaxMinRad(),
                              delta->getMinMaxMaxRad());
    qd_style_ruler->setSetting(delta->getRuledLineVisible(),
                               delta->getRuledLineColor(),
                               delta->getRuledLineStyle());

    auto horizontal = static_cast<AnalysisGraphHorizontalCapacity2DViewSetting*>(settings.getSetting(OUTFIGTYPE::QuNeedf));
    capa_axisLabelSizeCombobox->setCurrentIndex(horizontal->getAxisLabelSize());
    capa_axisValueSizeCombobox->setCurrentIndex(horizontal->getAxisScaleTextSize());
    capa_titleLabelSizeCombobox->setCurrentIndex(horizontal->getTitleLabelSize());
    capa_minmax_q->setSetting(horizontal->getMinMaxAutoQ(),
                              horizontal->getMinMaxMinQ(),
                              horizontal->getMinMaxMaxQ());
    capa_style_u->setSetting(horizontal->getVisibleU(),
                             horizontal->getColorU(),
                             horizontal->getStyleU());
    capa_style_un->setSetting(horizontal->getVisibleUn(),
                              horizontal->getColorUn(),
                              horizontal->getStyleUn());
    capa_style_ruler->setSetting(horizontal->getVisibleRuler(),
                                 horizontal->getColorRuler(),
                                 horizontal->getStyleRuler());

    auto nmint = static_cast<AnalysisGraphNmint2DViewSetting*>(settings.getSetting(OUTFIGTYPE::NMINT));
    nm_axisLabelSizeCombobox->setCurrentIndex(nmint->getAxisLabelSize());
    nm_axisValueSizeCombobox->setCurrentIndex(nmint->getAxisScaleTextSize());
    nm_titleLabelSizeCombobox->setCurrentIndex(nmint->getTitleTextSize());
    nm_plotPointSizeCombobox->setCurrentIndex(nmint->getSymbolSize());
    nm_plotPointLabelSizeCombobox->setCurrentIndex(nmint->getSymbolLabelSize());
    nm_minmax_N->setSetting(nmint->getMinMaxAutoN(),
                            nmint->getMinNValue(),
                            nmint->getMaxNValue());
    nm_minmax_My->setSetting(nmint->getMinMaxAutoMy(),
                             nmint->getMinMyValue(),
                             nmint->getMaxMyValue());
    nm_minmax_Mz->setSetting(nmint->getMinMaxAutoMz(),
                             nmint->getMinMzValue(),
                             nmint->getMaxMzValue());
    nm_style_l->setSetting(nmint->getOutputLVisible(),
                           nmint->getOutputLColor(),
                           nmint->getOutputLStyle());
    nm_style_s->setSetting(nmint->getOutputSVisible(),
                           nmint->getOutputSColor(),
                           nmint->getOutputSStyle());
    nm_style_u->setSetting(nmint->getOutputUVisible(),
                           nmint->getOutputUColor(),
                           nmint->getOutputUStyle());
    nm_style_kotei->setSetting(nmint->getKoteiVisible(),
                               nmint->getKoteiColor(),
                               nmint->getKoteiStyle());
    nm_style_hendou->setSetting(nmint->getHendouVisible(),
                                nmint->getHendoColor(),
                                nmint->getHendouStyle());
    nm_style_ruler->setSetting(nmint->getRulerVisible(),
                               nmint->getRulerColor(),
                               nmint->getRulerStyle());
}

void Analysis2DSettingGraphWidget::initializeSettings(){
    applySettings(Analysis2DViewSettings::getDefault());
}

AnalysisGraphQDelta2DViewSetting *Analysis2DSettingGraphWidget::makeQDelta() const
{
    return new AnalysisGraphQDelta2DViewSetting(
                true,
                Qt::black,
                Qt::black,
                qd_axisLabelSizeCombobox->size(),
                qd_axisValueSizeCombobox->size(),
                qd_titleLabelSizeCombobox->size(),
                qd_plotPointSizeCombobox->size(),
                qd_plotPointLabelSizeCombobox->size(),
                qd_minmax_q->isAutoScale(),
                qd_minmax_q->minValue(),
                qd_minmax_q->maxValue(),
                qd_minmax_d->isAutoScale(),
                qd_minmax_d->minValue(),
                qd_minmax_d->maxValue(),
                qd_minmax_rad->isAutoScale(),
                qd_minmax_rad->minValue(),
                qd_minmax_rad->maxValue(),
                qd_style_ruler->getLineVisible(),
                qd_style_ruler->getLineColor(),
                qd_style_ruler->getLineStyle()
                );
}

AnalysisGraphHorizontalCapacity2DViewSetting *Analysis2DSettingGraphWidget::makeHorizontalCapacity() const
{
    return new AnalysisGraphHorizontalCapacity2DViewSetting(
                true,
                capa_axisLabelSizeCombobox->size(),
                capa_axisValueSizeCombobox->size(),
                capa_titleLabelSizeCombobox->size(),
                capa_minmax_q->isAutoScale(),
                capa_minmax_q->minValue(),
                capa_minmax_q->maxValue(),
                capa_style_u->getLineVisible(),
                capa_style_u->getLineColor(),
                capa_style_u->getLineStyle(),
                capa_style_un->getLineVisible(),
                capa_style_un->getLineColor(),
                capa_style_un->getLineStyle(),
                capa_style_ruler->getLineVisible(),
                capa_style_ruler->getLineColor(),
                capa_style_ruler->getLineStyle()
                );
}

AnalysisGraphNmint2DViewSetting *Analysis2DSettingGraphWidget::makeNmint() const
{
    return new AnalysisGraphNmint2DViewSetting(
                true,
                Qt::black,
                Qt::black,
                Qt::black,
                nm_axisLabelSizeCombobox->size(),
                nm_axisValueSizeCombobox->size(),
                nm_titleLabelSizeCombobox->size(),
                nm_plotPointSizeCombobox->size(),
                nm_plotPointLabelSizeCombobox->size(),
                nm_minmax_N->isAutoScale(),
                nm_minmax_N->minValue(),
                nm_minmax_N->maxValue(),
                nm_minmax_My->isAutoScale(),
                nm_minmax_My->minValue(),
                nm_minmax_My->maxValue(),
                nm_minmax_Mz->isAutoScale(),
                nm_minmax_Mz->minValue(),
                nm_minmax_Mz->maxValue(),
                nm_style_l->getLineVisible(),
                nm_style_l->getLineColor(),
                nm_style_l->getLineStyle(),
                nm_style_s->getLineVisible(),
                nm_style_s->getLineColor(),
                nm_style_s->getLineStyle(),
                nm_style_u->getLineVisible(),
                nm_style_u->getLineColor(),
                nm_style_u->getLineStyle(),
                nm_style_kotei->getLineVisible(),
                nm_style_kotei->getLineColor(),
                nm_style_kotei->getLineStyle(),
                nm_style_hendou->getLineVisible(),
                nm_style_hendou->getLineColor(),
                nm_style_hendou->getLineStyle(),
                nm_style_ruler->getLineVisible(),
                nm_style_ruler->getLineColor(),
                nm_style_ruler->getLineStyle()
                );
}

} // namespace post3dapp

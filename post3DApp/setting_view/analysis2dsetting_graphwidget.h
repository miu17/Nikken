#ifndef ANALYSIS2DSETTING_GRAPHWIDGET_H
#define ANALYSIS2DSETTING_GRAPHWIDGET_H

#include <QWidget>

namespace post3dapp{
enum class OUTFIGTYPE;
class Analysis2DViewSettings;
class AnalysisBaseViewSettings;
class AnalysisGraphHorizontalCapacity2DViewSetting;
class AnalysisGraphNmint2DViewSetting;
class AnalysisGraphQDelta2DViewSetting;
class AxisMinMaxWidget;
class FiveSizeComboBox;
class LineStyleWidget;


class Analysis2DSettingGraphWidget : public QWidget
{
    Q_OBJECT
public:
    Analysis2DSettingGraphWidget(const QString&, const QString&, QWidget *parent = 0);
    QHash<int, AnalysisBaseViewSettings*> getSettings() const;
    void applySettings(const Analysis2DViewSettings&);
    const QString TabName;
public slots:
    void initializeSettings();

private slots:

private:
    const QString englishTabName;
    AnalysisGraphQDelta2DViewSetting* makeQDelta() const;
    AnalysisGraphHorizontalCapacity2DViewSetting* makeHorizontalCapacity() const;
    AnalysisGraphNmint2DViewSetting* makeNmint() const;

    FiveSizeComboBox* qd_axisValueSizeCombobox;
    FiveSizeComboBox* qd_axisLabelSizeCombobox;
    FiveSizeComboBox* qd_titleLabelSizeCombobox;
    FiveSizeComboBox* qd_plotPointSizeCombobox;
    FiveSizeComboBox* qd_plotPointLabelSizeCombobox;
    AxisMinMaxWidget* qd_minmax_q;
    AxisMinMaxWidget* qd_minmax_d;
    AxisMinMaxWidget* qd_minmax_rad;
    LineStyleWidget* qd_style_ruler;

    FiveSizeComboBox* capa_axisValueSizeCombobox;
    FiveSizeComboBox* capa_axisLabelSizeCombobox;
    FiveSizeComboBox* capa_titleLabelSizeCombobox;
    AxisMinMaxWidget* capa_minmax_q;
    LineStyleWidget* capa_style_u;
    LineStyleWidget* capa_style_un;
    LineStyleWidget* capa_style_ruler;

    FiveSizeComboBox* nm_axisValueSizeCombobox;
    FiveSizeComboBox* nm_axisLabelSizeCombobox;
    FiveSizeComboBox* nm_titleLabelSizeCombobox;
    FiveSizeComboBox* nm_plotPointSizeCombobox;
    FiveSizeComboBox* nm_plotPointLabelSizeCombobox;
    AxisMinMaxWidget* nm_minmax_N;
    AxisMinMaxWidget* nm_minmax_My;
    AxisMinMaxWidget* nm_minmax_Mz;
    LineStyleWidget* nm_style_l;
    LineStyleWidget* nm_style_s;
    LineStyleWidget* nm_style_u;
    LineStyleWidget* nm_style_kotei;
    LineStyleWidget* nm_style_hendou;
    LineStyleWidget* nm_style_ruler;

};
} // namespace post3dapp

#endif // ANALYSIS2DSETTING_GRAPHWIDGET_H

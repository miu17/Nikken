#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "interface_widget.h"
#include "define_graph.h"
#include "define_analysis2dsettings.h"

namespace post3dapp{
class GraphView;

class GraphWidget : public InterfaceWidget
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);

private slots:
    void resetLayout(int);
    void slotClickedPrintButton();
    void updateGraph();
    void resizeEvent(QResizeEvent* e)override{InterfaceWidget::resizeEvent(e);updateGraph();}

private:
    void createButtons();
    void createLayout();
    QComboBox *outTypeComboBox;

    QComboBox *outRSTUComboBox;
    QComboBox *outQUDSComboBox;
    QComboBox *outFrameComboBox;
    QComboBox *outFrameGroupComboBox;
    QComboBox *outFloorComboBox;
    GraphView *graphView;

    QToolButton* fitButton;
    QToolButton* printButton;

    GRAPH_SUBTYPE_PROPERTY opt;
    GRAPH_PROPERTY prop;
    Analysis2DViewSettings setting;

};
} // namespace post3dapp

#endif // GRAPHWIDGET_H

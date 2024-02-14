#ifndef NMINTDIALOG_H
#define NMINTDIALOG_H

#include <QDialog>
#include "define_graph.h"

namespace post3dapp{
class GraphView;
class Graph3dView;
class Graph3dScene;

class  NmintDialog : public QDialog
{
    Q_OBJECT

public:
    NmintDialog( QWidget *parent= nullptr);
//    ~NmintDialog(){}
    void setMemberId(const QUuid& id){opt.memberId = id;}
//    void setCaseNames(QStringList strlist,QList<QList<STRESS>> stress){
//        opt.stressCombiNames_forPlotPoints = strlist;
//        opt.stressCombi_forPlotPoints = stress;
//    }
public slots:
    void updateGraph();
    void update3dGraph();//3dはリサイズタイミングで呼ぶとまずそう？初期化タイミングは？

private slots:
    void slotClickedPrintButton();
    void resizeEvent(QResizeEvent* e)override{
        QDialog::resizeEvent(e);
        updateGraph();
    }

private:
    void createButtons();
    void createLayout();
    QComboBox *outICJComboBox;
    QComboBox* outCaseNameComboBox;

    GraphView *graphView;
    Graph3dView* graph3dView;
    Graph3dScene* my3dGraphScene;

    QToolButton* fitButton;
    QToolButton* printButton;

    GRAPH_SUBTYPE_PROPERTY opt;
//    GRAPH_SIZE_OPTION size;
    GRAPH_PROPERTY prop;
    Analysis2DViewSettings setting;
    bool syncSetting;


};
} // namespace post3dapp
#endif // NMINTDIALOG_H

#ifndef GRAPHPRINTDIALOG_H
#define GRAPHPRINTDIALOG_H
#include <QDialog>
#include <QImage>
#include "define_draw_figure.h"

class QCheckBox;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QWidget;


namespace post3dapp{
struct GRAPH_SUBTYPE_PROPERTY;
class GraphScene;

class GraphPrintDialog : public QDialog
{
    Q_OBJECT
public:
    GraphPrintDialog(GraphScene* scene,QImage image=QImage(),QDialog* parent=nullptr);
public slots:
    void drawPreview();

    void previewClicked(QPrinter *newPrinter);
    void setPrinter();

private:
    const GraphScene* baseScene;
    const QImage myImage;
    GraphScene* CopySceneProperty();
    void createLayout();
    QWidget* createPreviewWidget();

    QComboBox* paperSize;
    QPushButton* previewButton;
    QPushButton* printButton;

    QCheckBox* check_header;
    QCheckBox* check_footer;

    //マージン、アライメント、アスペクト比、対数軸、最大値など
//    QComboBox* vAlign;
//    QComboBox* hAlign;
    QLineEdit* margin;
//    QLineEdit* padding;
    QSpinBox* nGraphPerRow;
//    QCheckBox* aspectFix;
//    QLineEdit* aspectRatio;


//    QCheckBox* scaleAuto;
//        QLineEdit* xMax;
//        QLineEdit* xMin;
//        QLineEdit* yMax;
//        QLineEdit* yMin;
//        QCheckBox* logScaleX;
//        QCheckBox* logScaleY;
    QCheckBox* drawLegend;
    QCheckBox* drawWaku;



    QPrinter* printer;
    QPrintPreviewWidget* myPrintPreview;

//    QList<GRAPH_SIZE_OPTION> size;
    QList<GRAPH_SUBTYPE_PROPERTY> type;
    GRAPH_PROPERTY prop;
    int graph_per_row;
};

}
#endif // GRAPHPRINTDIALOG_H

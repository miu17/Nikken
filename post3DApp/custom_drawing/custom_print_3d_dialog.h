#ifndef CUSTOMPRINT3DDIALOG_H
#define CUSTOMPRINT3DDIALOG_H

#include <QDialog>
#include "define_glsetting.h"

#include "customdraw_grscene.h"
#include "define_draw_figure.h"

namespace post3dapp{

class CustomPrint3dDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CustomPrint3dDialog(QGLViewer *, QWidget *parent = 0);


    static void drawOnePage(QGLViewer*, QPainter*, const FIGURE_SIZE_RESULT&, const FIGURE_PROPERTY&, const FIGURE_TYPE_PROPERTY&);

private slots:
    void previewClicked(QPrinter *newPrinter);
    void settingButtonClicked();
    void setPrinter();

private:
    QGLViewer* viewer;

    void createLayout();
    QWidget* createPreviewWidget();
    void drawPreview();

    QPrinter* printer;
    QPrintPreviewWidget* myPrintPreview;

    QComboBox* paperSize;
    QPushButton* settingButton;
    QPushButton* previewButton;
    QPushButton* printButton;

    QCheckBox* check_header;
    QCheckBox* check_footer;

};
} // namespace post3dapp

#endif // CUSTOMPRINT3DDIALOG_H

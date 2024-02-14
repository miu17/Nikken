#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets>

namespace post3dapp{


class TopMenuWidget;
class CentralWidget;
class MainWidget : public QWidget
{
    Q_OBJECT

public:

    MainWidget(QWidget *parent = 0);

protected:

    void closeEvent ( QCloseEvent *event );

public slots:

    void slotChangeTitle();
    void setModified();

private:

    TopMenuWidget *topWidget;
    CentralWidget *centralWidget;
    bool isModified;
    QString currentFileName;
};

} // namespace post3dapp

#endif

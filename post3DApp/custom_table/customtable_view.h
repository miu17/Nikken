#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QTableView>

class QMenu;

namespace post3dapp{
class CustomTableView : public QTableView
{
    Q_OBJECT

public:

    explicit CustomTableView( QWidget *parent = 0 );

    void setMenu(QMenu *mn)
    {
        menu = mn;
    }
    void setFirstColumn(int col)
    {
        firstColumn = col;
    }
    void setLastColumn(int col)
    {
        lastColumn = col;
    }

protected:

    virtual void keyPressEvent ( QKeyEvent *event ) override;
    virtual void contextMenuEvent ( QContextMenuEvent *event ) override;
    virtual void mousePressEvent ( QMouseEvent *event ) override;

public slots:

private slots:

signals:

    void delKeyClicked();
    void delKeyClicked(const QModelIndex &);
    void currentRowChanged(int);
    void selectionBehaviorChanged(bool);

protected:

    QMenu *menu;
    int firstColumn;
    int lastColumn;

};
} // namespace post3dapp
#endif


#ifndef SECTION_TABLEVIEW_H
#define SECTION_TABLEVIEW_H

#include "customtable_view.h"
namespace post3dapp{
class SectionDataType;

class SectionTableView : public CustomTableView
{
    Q_OBJECT

public:

    explicit SectionTableView( QWidget *parent = 0 );
    void setMiddleViewFirstColumns(const QList<int> &fcols)
    {
        middleViewFirstColumns = fcols;
    }
    void setMiddleViewLastColumns(const QList<int> &lcols)
    {
        middleViewLastColumns = lcols;
    }
    int getFirstColumn() const
    {
        return firstColumn;
    }
    int getLastColumn() const
    {
        return lastColumn;
    }
    QList<int> getMiddleViewFirstColumns() const
    {
        return middleViewFirstColumns;
    }
    QList<int> getMiddleViewLastColumns() const
    {
        return middleViewLastColumns;
    }
    void setTableViewBorderColumns(const SectionDataType &, const QList<bool> &);

protected:
    QList<int> middleViewFirstColumns;
    QList<int> middleViewLastColumns;
    void keyPressEvent ( QKeyEvent *event ) override;


public slots:

private slots:

signals:

    void homeKeyClicked();
    void endKeyClicked();
    void requestChangeViewNext();
    void requestChangeViewBack();

protected:

};
} // namespace post3dapp
#endif // SECTION_TABLEVIEW_H

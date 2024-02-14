#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QWidget>
#include "define_customtable.h"

class QLabel;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QUndoStack;
class QMenu;
class QAction;
class QHBoxLayout;
class QItemSelectionModel;

namespace post3dapp{
class CustomTableModel;
class CustomTableView;
class MySortFilterProxyModel;

class CustomTableWidget : public QWidget
{
    Q_OBJECT

public:

    CustomTableWidget( const CustomTableDataType &type, QWidget *parent = nullptr );

    virtual bool checkTextFile ( QTextStream &) const;
    virtual bool readTextFile ( QTextStream &, bool);
    virtual void writeTextFile (QTextStream &);

    virtual bool checkBinaryFile ( QDataStream &) const;
    virtual void readBinaryFile ( QDataStream &);
    virtual void writeBinaryFile ( QDataStream &);
    virtual void reflectUnifiedData(){};

    virtual void clearData();

    virtual void modelUpload() {}
    virtual void modelEdit() {}
    virtual void modelRewind() {}

    virtual void replaceMatchingName( const QString &, const QString & );
    CustomTableModel* getTableModel() const;

public slots:

    virtual void slotChangeTableRowCount(int);
    virtual void slotChangeSpinBoxValue(int);
    virtual void slotChangeSpinBoxMinimum(int);

    virtual void slotRowActionEnable(bool);

    virtual void slotInsertLine();
    virtual void slotDeleteLine();
    virtual void slotMoveUp();
    virtual void slotMoveDown();
    virtual void slotEditItem(const QModelIndex &, const QVariant &);

    virtual void slotFilterOn();
    virtual void slotFilterReset();

    virtual void copy();
    virtual void cut();
    virtual void paste();
    virtual void del();

signals:

protected:

    CustomTableDataType myType;

    QMenu *myMenu;
    QUndoStack *undoStack;
    QHBoxLayout *topLayout;

    QList<QAction *> myActionList;
    QAction *copyAction;
    QAction *cutAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *insertAction;
    QAction *upAction;
    QAction *downAction;
    QAction *undoAction;
    QAction *redoAction;

    QLabel *mySpinBoxLabel;
    QSpinBox *mySpinBox;
    QComboBox *filter_combo;
    QLineEdit *filter_ledit;

    CustomTableModel *myTableModel;
    MySortFilterProxyModel *myProxyModel;
    QItemSelectionModel *mySelectionModel;

    CustomTableView *myTableView;

    virtual void createActions();
    virtual void createButtons();
    virtual void createModels() = 0 ;
    virtual void createViews() = 0 ;
    virtual void createWidgets() = 0 ;

    bool filterWidgetsVisible;
    QWidget *filterWidgets;
    bool undoOnOff;

    void setFilterWidgetsVisible(bool flg)
    {
        filterWidgets->setVisible(flg);
    }
    void setSpinBoxVisible(bool onoff);
    void setUndoOnOff(bool flg)
    {
        undoOnOff = flg;
    }

    void warningSelectContinuousLine();
    void warningSelectOneLine();
};
} // namespace post3dapp
#endif

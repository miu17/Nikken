#ifndef INPUTJOINTTABLEWIDGET_H
#define INPUTJOINTTABLEWIDGET_H

#include "customtable_widget.h"
#include "define_textinput.h"

#include "combobox_delegate.h"

class InputJointTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    InputJointTableWidget( const CustomTableDataType &type, QWidget* parent=0 );

    virtual bool readTextFile ( QTextStream& infile){
        if( !myTableModel->readTextData(infile) ) return false;
        undoStack->clear();
        return true;
    }

    virtual void writeTextFile ( QTextStream& outfile ){
        QList<int> list;
        list.append( myType.getTaggedColumn("UUID") );
        myTableModel->writeTextData(outfile,list);
    }

    int getJointCount(){ return myTableModel->rowCount(); }
    void importJointData( const QList<JointData*> & );

    QString checkJointData() const;
    void uploadJoints();

protected:

public slots:

    void slotDeleteLine();

protected:

    virtual void createModels();
    virtual void createViews();
    virtual void createWidgets();

    bool warningLineDelete();

};

#endif

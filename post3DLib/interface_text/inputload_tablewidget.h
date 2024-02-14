#ifndef INPUTLOADTABLEWIDGET_H
#define INPUTLOADTABLEWIDGET_H

#include "customtable_widget.h"
#include "define_textinput.h"

#include "combobox_delegate.h"

class InputLoadTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    InputLoadTableWidget( const CustomTableDataType &type, QWidget* parent=0 );

    virtual bool readTextFile ( QTextStream& infile){
        if( ! myTableModel->readTextData(infile) ) return false;
        undoStack->clear();
        return true;
    }

    virtual void writeTextFile ( QTextStream& outfile ){
        QList<int> list;
        list.append( myType.getTaggedColumn("UUID") );
        myTableModel->writeTextData(outfile,list);
    }

    void importMemberData( const QList<MemberData*> & );

    QString checkLoadData(int) const;
    void uploadLoads();

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

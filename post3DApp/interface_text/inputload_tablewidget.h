#ifndef INPUTLOADTABLEWIDGET_H
#define INPUTLOADTABLEWIDGET_H

#include "customtable_widget.h"

class QTextStream;
namespace post3dapp{
class CustomTableDataType;
class MemberData;

class InputLoadTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    InputLoadTableWidget( const CustomTableDataType &type, QWidget *parent = 0 );

    bool readTextFile ( QTextStream &infile, bool flg) override;

    void writeTextFile ( QTextStream &outfile) override;

    void importMemberData( const QList<MemberData *> & );

    QString checkLoadData(int) const;
    void uploadLoads();

protected:

public slots:

    void slotDeleteLine() override;

protected:

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

    bool warningLineDelete();

};
} // namespace post3dapp
#endif

#ifndef INPUTMEMBERTABLEWIDGET_H
#define INPUTMEMBERTABLEWIDGET_H

#include "customtable_widget.h"

class QTextStream;

namespace post3dapp{
class CustomTableDataType;
class MemberData;

class InputMemberTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    InputMemberTableWidget( const CustomTableDataType &type, QWidget *parent = 0 );

    bool readTextFile ( QTextStream &infile, bool flg) override;

    void writeTextFile ( QTextStream &outfile ) override;

    void importMemberData( const QList<MemberData *> & );

    QString checkMemberData(int) const;
    void uploadMembers();

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

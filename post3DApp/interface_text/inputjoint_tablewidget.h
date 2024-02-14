#ifndef INPUTJOINTTABLEWIDGET_H
#define INPUTJOINTTABLEWIDGET_H

#include "customtable_widget.h"

class QTextStream;
namespace post3dapp{
class CustomTableDataType;
class JointData;

class InputJointTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    InputJointTableWidget( const CustomTableDataType &type, QWidget *parent = 0 );

    bool readTextFile ( QTextStream &infile, bool flg) override;

    void writeTextFile ( QTextStream &outfile) override;

    int getJointCount();
    void importJointData( const QList<JointData *> & );

    QString checkJointData() const;
    void uploadJoints();

protected:

public slots:

    void slotDeleteLine() override;

protected:

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};
} // namespace post3dapp
#endif

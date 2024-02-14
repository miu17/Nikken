#ifndef FRAMESETTINGXYDIALOG_H
#define FRAMESETTINGXYDIALOG_H

#include <QDialog>
#include "customtable_widget.h"
namespace post3dapp{
class CustomTableDataType;

class FrameSettingXYWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    FrameSettingXYWidget( const CustomTableDataType &type, QWidget *parent = 0 );

    void setXYList(const QString &xylist);
    QString xyString() const;

protected:

public slots:

signals:

protected:

    void createModels() override;
    void createViews() override;
    void createWidgets() override;
};


class FrameSettingXYDialog : public QDialog
{
    Q_OBJECT

public:

    FrameSettingXYDialog( const QString &title, const QString &str, QWidget *parent = 0 );

    QString xyString() const;

private:

    FrameSettingXYWidget *myWidget;

};
} // namespace post3dapp

#endif

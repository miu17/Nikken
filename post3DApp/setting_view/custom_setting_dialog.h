#ifndef CUSTOMSETTINGDIALOG_H
#define CUSTOMSETTINGDIALOG_H

#include <QDialog>
#include "define_settings.h"
#include "define_output.h"

// in / out /calc
// display /default / custom
// 2d / 3d
// type
// single multi

//save load update set get

namespace  post3dapp {
class BaseSettingWidget;

enum DIALOGMODE{DIALOG_SINGLEOUTPUT=0,DIALOG_MULTIOUTPUT,DIALOG_DISPLAYOUTPUT,
               };//単ページ、全項目、ディスプレイ向け
//enum SETTINGDATATYPE{DATATYPE_DISPLAY=0,DATATYPE_DEFAULT,DATATYPE_CUSTOM};

struct SETTINGDIALOGSETTINGS
{
    MODELVIEWTYPE modelViewType;
//    SETTINGDATATYPE settingDataType;
    DIALOGMODE dialogMode;
    bool editable;
    bool updateUnifiedSettings;
    OUTFIGTYPE type;

};
/*
class CustomSettingDialog : public QDialog
{
    Q_OBJECT
public:
    CustomSettingDialog(const SETTINGDIALOGSETTINGS&,QWidget *parent=nullptr);
    ~CustomSettingDialog();

private slots:
    void uploadViewSettings();

    void slotFileSave();
    void slotFileOpen();

private:
    QWidget *settingWidget;
//    OUTPUT2DVIEWSETTINGS createOutput2dViewSetting() const;

    void saveSettings(const QString&) const;
    void readSettings(const QString&);


    void setDialogSettings();
    void setData(const BASEVIEWSETTINGS& );
    BASEVIEWSETTINGS* getData();

//    void read();//file -> myData
//    void save();//myData -> file

    void reload();//unified -> myData
    void apply();//myData -> unified

    void readWidgets();//widget -> myData
    void setWidgets();//myData -> widget

private:
//MODELVIEWTYPE modelViewType;
//SETTINGDATATYPE settingDataType;
//OUTFIGTYPE type;
//DIALOGMODE dialogMode;
//bool editable;

    QList<BASEVIEWSETTINGS*> myData;
    BaseSettingWidget* myWidget;
    SETTINGDIALOGSETTINGS myDialogSetting;
};
*/
}

#endif // CUSTOMSETTINGDIALOG_H

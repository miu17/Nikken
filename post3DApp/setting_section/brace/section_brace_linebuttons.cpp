#include "section_brace_linebuttons.h"
namespace post3dapp{
SectionBraceLineButtons::SectionBraceLineButtons( const SectionDataType &type, const QList<bool> &filter, QWidget *parent )
    :SectionLineButtons(type, filter, parent)
{

}


void SectionBraceLineButtons::setStandardLineEdit(){
    QList<int> dilist = mySectionType.getDirectInputColumns();
    int maxdcol = 0;
    bool isDirectInput = false;
    Q_FOREACH(int dcol, dilist){
        if ( maxdcol < dcol ) maxdcol = dcol;
    }
    Q_FOREACH (int col, dilist) {
        const double EPS = 0.001;
        if (myValues.at(col).toDouble() > EPS){
            isDirectInput = true;
            break;
        }
    }
    if (isDirectInput) {
        for (int i = 1; i < myLelist.count(); i++){
            if (dilist.contains(i)){
                myLelist.at(i)->setEnabled(true);
            }
            else if (maxdcol < i){
                myLelist.at(i)->setEnabled(false);
            }
        }
    }else{
        bool hasSection = false;
        for (int i = maxdcol + 1; i < myLelist.count(); i++){
            if (myValues.at(i).isValid() && myValues.at(i).toString() != QString{}){
                hasSection = true;
                break;
            }
        }
        if (hasSection)
        {
            for (int i = 1; i < myLelist.count(); i++){
                if (dilist.contains(i)){
                    myLelist.at(i)->setEnabled(false);
                }
                else if (maxdcol < i){
                    //myLelist.at(i)->setEnabled(true);
                    SectionLineButtons::setStandardLineEdit();
                }
            }
        }else{
            for (int i = 1; i < myLelist.count(); i++){
                if (dilist.contains(i)){
                    myLelist.at(i)->setEnabled(true);
                }
                else if (maxdcol < i){
                    myLelist.at(i)->setEnabled(true);
                }
            }
        }
    }
}
} // namespace post3dapp

#include "section_base_linebuttons.h"
namespace post3dapp{
SectionBaseLineButtons::SectionBaseLineButtons(const SectionDataType &type, const QList<bool> &filter, QWidget *parent)
    : SectionLineButtons(type, filter, parent)
{

}

void SectionBaseLineButtons::setStandardLineEdit()
{
    QList<int> sclist = mySectionType.getShapeTypeColumns();
    QStringList standardSectionFiles = SectionDataType::standardSectionFileList(StandardSectionTable::listType::BASE);
    QString st1, st2;
    Q_FOREACH (int col, sclist) {
        st1 = qobject_cast<QComboBox *>(myLelist.at(col))->currentText();
        st2 = qobject_cast<QComboBox *>(myLelist.at(col + 1))->currentText();
        bool flag = ( standardSectionFiles.contains(st1) &&
                      SectionDataType::standardSectionNameList(st1).contains(st2));

        for (int i = col + 2; i < myLelist.count(); ++i){
            myLblist.at(i)->setEnabled(!flag);
            myLelist.at(i)->setEnabled(!flag);
        }
    }
}
} // namespace post3dapp

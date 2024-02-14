#ifndef TEXTDATAUNIT_H
#define TEXTDATAUNIT_H

#include <QList>
#include <QString>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
enum class TEXTTYPE{TYPE_NONE, TYPE_INT, TYPE_DOUBLE, TYPE_TEXT, TYPE_BLANK};

class POST3D_CLASS_DLLSPEC TextDataUnit
{
public:
    TextDataUnit(
            TEXTTYPE _type,
            int _start,
            int _end,
            bool _isNecessary = false,
            const QString& _statusBar = QString(),
            const QString& _defaultValue = QString(),
            const QList<QString>& _optionValue = QList<QString>())
        :type(_type), startCol(_start), endCol(_end), isNecessary(_isNecessary), statusBar(_statusBar), defaultValue(_defaultValue), optionValue(_optionValue){}
    const TEXTTYPE type;
    const int startCol;
    const int endCol;
    const bool isNecessary;
    const QString statusBar;
    const QString defaultValue; // 入力しなくてもデフォルトの数値が入る場合に、デフォルトを記入。デフォルトがない場合に必須項目となるので、ヒントを出したくない場合は" "とする。
    const QList<QString> optionValue;

    bool match(QString& word) const{
        bool isOk = true;
        switch(type){
        case TEXTTYPE::TYPE_NONE:
            return false;
        case TEXTTYPE::TYPE_INT:
            if (!isNecessary && word.trimmed() == QString())
                return true;

            if (isNecessary && word.trimmed() == QString()){
                if (defaultValue != QString()){return true;}
                else{return false;}
            }

            if (optionValue.count() > 0){
                if (optionValue.contains(word))
                    return true;
                else
                    return false;
            }
            word.toInt(&isOk);
            if (!isOk){
                return false;
            }
            return true;
        case TEXTTYPE::TYPE_DOUBLE:
            if (!isNecessary && word.trimmed() == QString())
                return true;

            if (isNecessary && word.trimmed() == QString()){
                if (defaultValue != QString()){return true;}
                else{return false;}
            }

            if (optionValue.count() > 0){
                auto value = word.toDouble(&isOk);
                if(!isOk){
                    return false;
                }
                foreach(auto opt, optionValue){
                    if (value == opt.toDouble(&isOk))
                        return true;
                }
                return false;
            }
            else{
                word.toDouble(&isOk);
                if (!isOk){
                    return false;
                }
                return true;
            }
        case TEXTTYPE::TYPE_TEXT:
            if (!isNecessary && word.trimmed() == QString())
                return true;

            if (isNecessary && defaultValue == QString() && word.trimmed() == QString())
                return false;

            if (optionValue.count() > 0){
                if (optionValue.contains(word))
                    return true;
                else
                    return false;
            }

            return true;
        case TEXTTYPE::TYPE_BLANK:
            if (word.trimmed()!=QString()){
                return false;
            }
            return true;
        }

        return false;
    }
};
}

#endif // TEXTDATAUNIT_H

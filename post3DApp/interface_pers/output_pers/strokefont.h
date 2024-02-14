#ifndef STROKEFONT_H
#define STROKEFONT_H

#include <QString>
#include <QPainterPath>
#include <QHash>

namespace post3dapp{
class strokeFont
{
public:
    //シングルトン
    static strokeFont* getInstance(){
        if(stroke_font==nullptr){
            stroke_font=new strokeFont;
            return stroke_font;
        }else{
            return stroke_font;
        }
    }

    //QStringを、個々の文字Pathを結合したQPainterPathに変換
    QPainterPath getStringPath(QString str);
private:
    static strokeFont* stroke_font;

    //ファイル読み込みは初回の呼び出し時
    strokeFont(){readFile();}

    //shpファイルから、QHash<QString,QPainterPath>のテーブルを作成
    void readFile();
    QHash<QString,QPainterPath> strokeFontPath;
};
}

#endif // STROKEFONT_H

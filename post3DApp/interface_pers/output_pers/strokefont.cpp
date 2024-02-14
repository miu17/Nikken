#include "strokefont.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace post3dapp{
strokeFont* strokeFont::stroke_font=nullptr;
//QStringをQPainterPathに変換
QPainterPath strokeFont::getStringPath(QString str)

{
    QPainterPath mojiretu;
    qreal xbase=0.0;
    qreal ybase=0.0;
//    QVector<QPainterPath> vec;
    for(int i=0;i<str.length();i++){
        QString s = str.mid(i,1);
        if(s==u8" "){
            xbase += 15.0+1.0;
        }else if(s==u8"　"){
            xbase += 30.0+1.0;
        }else if(strokeFontPath.contains(s)){
            QPainterPath moji=strokeFontPath.value(s);
//            qDebug()<<moji;
            xbase += moji.boundingRect().width()/2+1;
            moji.translate(xbase,ybase);
            mojiretu.addPath(moji);
            xbase += moji.boundingRect().width()/2+1;
        }
    }
    return mojiretu;
}

//shpファイルから、QHash<QString,QPainterPath>のテーブルを作成
void strokeFont::readFile()
{
    QString s;// = QString::fromUtf8("\u4FF0");
    //fileopen

    QFile file(":/app/text/kst32b.shp");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    //
    QPainterPath path;
    QPointF pt(0,0);
    bool PENDOWN = false;
    QString line;
    QString moji;
    line=in.readLine(0);
    line=in.readLine(0);
    line=in.readLine(0);
    line=in.readLine(0);
    line=in.readLine(0);
    line=in.readLine(0);
    line=in.readLine(0);
    line=in.readLine(0);
    line=in.readLine(0);
    line=in.readLine(0);
    do{
        line=in.readLine(0);
        if(line.isEmpty())continue;

        if(line.left(1)==u8"*"){
            uint32_t num=line.midRef(2,4).toUInt(nullptr,16);//16進数で読む
            s=QString(QChar(num));//numにあたるUnicode文字をQStringにする
            path.clear();
            PENDOWN=false;
//            qDebug()<<s<<num;
            continue;
        }
        if(line==u8"2"){
            PENDOWN=false;
            continue;
        }
        if(line==u8"1"){
            PENDOWN=true;
            continue;
        }
        if(line==u8"0"){ //moji end
            PENDOWN=false;
            pt=QPointF(0,0);
            strokeFontPath.insert(s,path);
            continue;
        }
        if(line.left(1)==u8"8"){//move or line
            QStringList linelist = line.split(u8",");
            int x=linelist.at(1).midRef(1).toInt();
            int y=linelist.at(2).chopped(1).toInt();

            if(PENDOWN){
                pt=pt+QPointF(x,-y);
                path.lineTo(pt);
            }else{
                pt=pt+QPointF(x,-y);
                path.moveTo(pt);
            }
            continue;
        }
        if(line.left(1)==u8"9"){//poly line
            QStringList linelist = line.split(u8",");
            int x;
            int y;

            for(int i=1;i<linelist.count();i++){
                if(i%2==1){
                  x = linelist.at(i).toInt();
                }else{
                    y = linelist.at(i).toInt();
                    pt=pt+QPointF(x,-y);
                    if(PENDOWN){
                        path.lineTo(pt);
                    }else{
                        path.moveTo(pt);
                    }
                }
            }
            continue;
        }
    }while(!in.atEnd());

    file.close();
}
}
//C2S_COMMAND_END = 0,
//C2S_COMMAND_PENDOWN = 1,
//C2S_COMMAND_PENUP = 2,
//C2S_COMMAND_SINGLEVECTOR = 8,
//C2S_COMMAND_MULTIVECTOR = 9,
//C2S_COMMAND_VARTICAL = 14,

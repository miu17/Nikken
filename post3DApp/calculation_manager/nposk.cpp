#include "nposk.h"

#include <QFile>
#include <QFileInfo>
#include <QTextCodec>

#include "define_draw_figure.h"
#include "draw_figure.h"
#include "elevationUI_grscene.h"
#include "fixed_data.h"
#include "planUI_grscene.h"
#include "section01.h"
#include "section02.h"
#include "section03.h"
#include "section04.h"
#include "section05.h"
#include "section06.h"
#include "section07.h"
#include "section08.h"
#include "section09.h"
#include "section10.h"
#include "section11.h"
#include "unified_inputdata.h"
#include "utility.h"

namespace post3dapp{

QMap<QString, QStringList> Nposk::outGroupPlan;
QMap<QString, QStringList> Nposk::outGroupElevation;
QMap<QString, QList<STRESS>> Nposk::stressMap;
QMap<QString, FIGURE_PROPERTY> Nposk::planOption;
QMap<QString, PAPERSETTING> Nposk::planPaperSetting;
QMap<QString, FIGURE_PROPERTY> Nposk::elevationOption;
QMap<QString, PAPERSETTING> Nposk::elevationPaperSetting;

Nposk::~Nposk(){
    delete section01;
    delete section02;
    delete section03;
    delete section04;
    delete section05;
    delete section06;
    delete section07;
    delete section08;
    delete section09;
    delete section10;
    delete section11;
    delete contents;
    delete contents2;
    delete end;
    delete organize;
}


bool Nposk::parseAll(
        CalculationMakerLib::CalculationMaker *maker,
        const QStringList& posk){
    QStringList list;
    Q_FOREACH(QString line, posk){
        if (line.startsWith("*END")){
            parseList(maker, list);
            list.clear();
            parseList(maker, QStringList{line});
            break;
        }
        else if (line.startsWith("*")){
            if (list != QStringList()){
                parseList(maker, list);
            }
            list.clear();
            list.append(line);
        }
        else if (line.startsWith("!")){
        }
        else{
            list.append(line);
        }
    }

    if (!check(maker->getSetting()))
        return false;
    return context(maker, maker->getSetting());
}


QString Nposk::parseList(CalculationMakerLib::CalculationMaker *maker, QStringList list){
    QString command = list.first().trimmed();
    if (command == "*CONTENTS"){
        contents = new WriteContents(list);
    }
    else if (command == "*CONTENTS2"){
        contents2 = new WriteContents2(list);
    }
    else if (command == "*COVER"){
        cover = new WriteCover(list);
    }
    else if (command == "*END"){
        end = new WriteEnd(list);
    }
    else if (command == "*ORGANIZE"){
        organize = new WriteOrganize(list);
    }
    else if (Section01::Contain(command))
    {
        if (section01 == nullptr)
            section01 = new Section01();
        section01->set(command, list);
    }
    else if (Section02::Contain(command)){
        if (section02 == nullptr)
            section02 = new Section02();
        section02->set(command, list);
    }
    else if (Section03::Contain(command)){
        if (section03 == nullptr)
            section03 = new Section03();
        section03->set(command, list);
    }
    else if (Section04::Contain(command)){
        if (section04 == nullptr)
            section04 = new Section04();
        section04->set(command, list);
    }
    else if (Section05::Contain(command)){
        if (section05 == nullptr)
            section05 = new Section05();
        section05->set(command, list);
    }
    else if (Section06::Contain(command)){
        if (section06 == nullptr)
            section06 = new Section06();
        section06->set(command, list);
    }
    else if (Section07::Contain(command)){
        if (section07 == nullptr)
            section07 = new Section07();
        section07->set(command, list);
    }
    else if (Section08::Contain(command)){
        if (section08 == nullptr)
            section08 = new Section08();
        section08->set(command, list);
    }
    else if (Section09::Contain(command)){
        if (section09 == nullptr)
            section09 = new Section09();
        section09->set(command, list);
    }
    else if (Section10::Contain(command)){
        if (section10 == nullptr)
            section10 = new Section10();
        section10->set(command, list);
    }
    else if (Section11::Contain(command)){
        if (section11 == nullptr)
            section11 = new Section11();
        section11->set(command, list);
    }
    else{
        qDebug() << list.first().trimmed();
    }

    return QString();
}

bool Nposk::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){

    resorce.clear();
    resorce << common();
    if (cover)
        resorce << cover->context(maker, setting);
    if (organize)
        resorce << organize->context(maker, setting);
    if (section01)
        resorce << section01->context(maker, setting);
    if (section02)
        resorce << section02->context(maker, setting);
    if (section03)
        resorce << section03->context(maker, setting);
    if (section04)
        resorce << section04->context(maker, setting);
    if (section05)
        resorce << section05->context(maker, setting);
    if (section06)
        resorce << section06->context(maker, setting);
    if (section07)
        resorce << section07->context(maker, setting);
    if (section08)
        resorce << section08->context(maker, setting);
    if (section09)
        resorce << section09->context(maker, setting);
    if (section10)
        resorce << section10->context(maker, setting);
    if (section11)
        resorce << section11->context(maker, setting);
    if (contents)
        resorce << contents->context(maker, setting);
    if (contents2)
        resorce << contents2->context(maker, setting);

    return true;
}

QString Nposk::common(){
    QString common;
    common += u8"\\common{\n";
    common += u8"pagesize:A4  # ページサイズ。 \n";
    common += u8"pageorientation:PORTRAIT  # ページの縦横。\n";
    common += u8"margin:20.0,20.0,15.0,20.0  # Left,Top,Right,Bottomの余白(mm)。 \n";
    common += u8"linespacing:1.5  # 行間(mm)。 \n";
    common += u8"tablerowheight:5.0  # 表描画の際の1行高さ(mm)。 \n";
    common += u8"normalpen:0.2  # normalpenのペン幅(mm)。\n";
    common += u8"heavypen:0.5  # heavypenのペン幅(mm)。 \n";
    common += u8"headerformat:10.0,\"Minion Pro\",8.0　# ヘッダーの位置(mm),フォント,フォントサイズ(pt)。ヘッダー位置は用紙の上からの高さ。\n";
    common += u8"headerL:\"NEW BUILDING3D  TPRG-0040  BCJ-基評-CP0035-01  \\structuretype{}\" # 左寄せ文字列 \n";
    common += u8"headerR:\"\\date{MM/dd/yy hh:mm}\" # 右寄せ文字列\n";
    common += u8"footerformat:14.0,\"Minion Pro\",8.0 # フッターの位置(mm),フォント,フォントサイズ(pt)。フッター位置は用紙の下からの高さ。 \n";
    common += u8"footerL:\"\\projectname{}\" # 左寄せ文字列 \n";
    common += u8"footerR:\"\\pg{}\" # 右寄せ文字列\n";
    common += u8"normal:\"Century Gothic\",10.0,10.0,0.0,LEFT # 標準文字のフォント,フォントサイズ(pt),左インデント(mm),右インデント(mm),文字寄せ。\n";
    common += u8"title:\"Minion Pro\",15.0,0.0,0.0,CENTER # タイトルのフォント,フォントサイズ(pt),左インデント(mm),右インデント(mm),文字寄せ。\n";
    common += u8"subtitle:\"Minion Pro\",13.0,0.0,0.0,CENTER # サブタイトルのフォント,フォントサイズ(pt),左インデント(mm),右インデント(mm),文字寄せ。\n";
    common += u8"contents:\"Minion Pro\",10.5 # 目次のフォント,フォントサイズ(pt)。 \n";
    common += u8"chapter:\"Minion Pro\",12.5,0.0,LEFT,ON,ON # 章のフォント,フォントサイズ(pt),左インデント(mm),文字寄せ,PDFのしおり化,目次化。\n";
    common += u8"section:\"Minion Pro\",12.0,3.0,LEFT,ON,ON # 節のフォント,フォントサイズ(pt),左インデント(mm),文字寄せ,PDFのしおり化,目次化。\n";
    common += u8"subsection:\"Minion Pro\",11.0,5.0,LEFT,ON,ON # 小節のフォント,フォントサイズ(pt),左インデント(mm),文字寄せ,PDFのしおり化,目次化。\n";
    common += u8"} \n";
    return common;
}

bool Nposk::check(const OutputPaperSetting& setting){

    // p3dファイルの有無

    // pstn, npstnファイルの有無

    // ルートのチェック

    // scale、紙設定
    // 仮のGRSceneを作って算出
    Q_FOREACH(auto floor, UnifiedFixedData::getInstance()->getInputData()->getFloorList()){
        PlanInterfaceGrScene* plan = new PlanInterfaceGrScene(LIMITSELECTTERM(), UnifiedSettings::getInstance()->getInput2DViewSettings(), false);
        plan->setStatus(floor.UUID);
        auto scaleAndDivide = DrawFigure::calcScaleAndDivision(plan, setting.figure_size_policy);
        planPaperSetting.insert(floor.floorName, PAPERSETTING(PAPER(scaleAndDivide.first.unitPaper), scaleAndDivide.first.divide));
        planOption.insert(floor.floorName, scaleAndDivide.first);
    }

    auto frameGroups = UnifiedFixedData::getInstance()->getInputData()->getFrameGroupList();
    auto framePoints = UnifiedFixedData::getInstance()->getInputData()->getFramePointList();
    Q_FOREACH(auto frameGroup, frameGroups){
        ElevationInterfaceGrScene* elevation = new ElevationInterfaceGrScene(LIMITSELECTTERM(), UnifiedSettings::getInstance()->getInput2DViewSettings(), false);
        Q_FOREACH(auto fp, framePoints){
            if (fp.GID == frameGroup.GID){
                elevation->setStatus(fp.UUID);
                break;
            }
        }
        auto scaleAndDivide = DrawFigure::calcScaleAndDivision(elevation, setting.figure_size_policy);
        Q_FOREACH(auto fp, framePoints){
            if (fp.GID != frameGroup.GID)
                continue;
            elevationPaperSetting.insert(fp.frameName, PAPERSETTING(PAPER(scaleAndDivide.first.unitPaper), scaleAndDivide.first.divide));
            elevationOption.insert(fp.frameName, scaleAndDivide.first);
        }
    }

    return true;
}

} // namespace post3dapp

#include "abstracttext.h"
#include "fixed_data.h"
#include "fortranFunc.h"
#include "unified_output.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextCwArea::write(const FixedData& fixedData) const{
    constexpr int jwcase = 8;
    constexpr int nrofg = 4;
    constexpr int nlfg = 2;
    constexpr int ndac = 12;
    auto model = fixedData.getPstnData()->getCmdlData();

    /*
    int* iroute = new int[model.iroute.count()];
    double* route = new double[model.nroute];
    int* numfl = new int[model.numfl.count()];
    char* namefl = new char[model.namefl.count()];
    int* ifl = new int[model.ifl.count()];
    double* dfl = new double[model.ndfl * model.mxnfl * model.numfg];
    int* lsfl = new int[model.nxy * model.mxnfl * model.numfg];
    int* icfl = new int[model.mxnfl * model.numfg];
    double* hfl = new double[model.mxnfl * model.numfg];
    double* flmass = new double[jwcase * model.mxnfl * model.numfg];
    double* rofg = new double[nrofg * model.numfg];
    int* lfg = new int[nlfg * model.numfg];
    double* dfg = new double[model.numfg];
    char* nameac = new char[model.numacd];
    double* dac = new double[ndac * model.numacd];
    */

    /*
    PR701(&model.iunit, iroute, route,
          numfl, namefl, ifl, dfl, lsfl, icfl, hfl,
          flmass, rofg, lfg, dfg, &model.mxnfl, &model.numfg,
          nameac, dac, &model.numac);
          */
    QString second;
    second.append(u8"XXX ZZ- ZZ  壁と柱の水平断面積 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/06/02 15:35:10 ] XXXXXXXXXX").append("\n");
    second.append(u8" 検討方向 X  [XXXX.X ]        計算ル－ト [ XXX ]     Z = X.XXX  T1= X.XXX (S)").append("\n");
    second.append(u8"                                                    ＲＣ造  ： A25 = (2.5Aw + 0.7 Ac+ 0.7Aw')/βZWAi , A18 = (1.8Aw + 1.8 Ac)/βZWAi").append("\n");
    second.append(u8"                                                    ＳＲＣ造： A25 = (2.5Aw + 1.0 Ac+ 0.7Aw')/βZWAi , A18 = (2.0Aw + 2.0 Ac)/βZWAi").append("\n");
    second.append(u8"+--------+------+----------+------+----------+----------+----------+----------+----------+-------+---------+---------+----------------+").append("\n");
    second.append(u8"|  階名  | 構造 |   CONC.  |  β  |    Aw    |    Ac    |    Aw'   | floor Wi | total W  |  Ai   |   A25   |   A18   |     判 定      |").append("\n");
    second.append(u8"|        | 形式 |   材料   |      |  (m**2)  |  (m**2)  |  (m**2)  |   (kN)   |   (kN)   |       |         |         |                |").append("\n");
    second.append(u8"+--------+------+----------+------+----------+----------+----------+----------+----------+-------+---------+---------+----------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX |      |          |      |          |          |          |XXXXXXXX. |          |       |         |         |                |").append("\n");
        second.append(u8"|        |  XXX | XXXXXXXX | X.XX | XXXXX.XX | XXXXX.XX | XXXXX.XX |          |XXXXXXXX. | X.XXX | XXX.XXX | XXX.XXX | XXXXXXXXXXXXXX |").append("\n");
        second.append(u8"+--------+------+----------+------+----------+----------+----------+----------+----------+-------+---------+---------+----------------+").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp

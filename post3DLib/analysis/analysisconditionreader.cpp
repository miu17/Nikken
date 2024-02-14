#include "analysisconditionreader.h"

#include <QRegularExpression>
#include <QTextCodec>

#include "arealinecondition.h"
#include "generallinecondition.h"
#include "linecondition.h"
#include "textdataunit.h"
#include "unified_data.h"

namespace post3dapp {

QStringList AnalysisConditionReader::defaultGeneralAnalysisConditionLines;
QMap<int, QSharedPointer<GeneralLineCondition>> AnalysisConditionReader::defaultGeneralAnalysisConditionMap;

QSharedPointer<LineCondition> AnalysisConditionReader::endCondition =
        QSharedPointer<LineCondition>(new LineCondition("END",
                          ANALYSISSTEP::STEP_NONE,
                          LINESTATUSTYPE::END,
                          QList<TextDataUnit>{TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3}}));


QList<QSharedPointer<LineCondition>> AnalysisConditionReader::convConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("DFG",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DFG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 4, 10},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 11, 11, true, "oobari", "1", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 12, 12, true, "", "1", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 13, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 35, false, "U1(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 36, 40, false, "U2(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 45, false, "U3(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 46, 50, false, "D1(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 55, false, "D2(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 56, 60, false, "D3(m)"}
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 18, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 19, 24, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 25, 30, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_DFG})),
        QSharedPointer<LineCondition>(new LineCondition("DFC",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DFC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 4, 10},
                         TextDataUnit{TEXTTYPE::TYPE_INT, 11, 11, true, "", "1", QList<QString>{"0", "1"}},
                         TextDataUnit{TEXTTYPE::TYPE_INT, 12, 12, true, "", "1", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 13, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 35, false, "Y1(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 36, 40, false, "Y2(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 41, 45},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 46, 50, false, "Z1(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 55, false, "Z2(m)"}
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 18, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 19, 24, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 25, 30, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_DFC})),
        QSharedPointer<LineCondition>(new LineCondition("RO",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_RO,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 8},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 10, true, "", "", QList<QString>{"10", "20", "21", "22", "23", "30"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 11},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 12, 13, true, "", "", QList<QString>{"10", "20", "21", "22", "23", "30"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 14, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60}})),
        QSharedPointer<LineCondition>(new LineCondition("FG",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_RO_FG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "(auto)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "(auto)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true, "", "1.0"}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_RO})),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 11, true, "", "", QList<QString>{"S  ", "RC ", "SRC"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 12, 14, true, "", "", QList<QString>{"S  ", "RC ", "SRC"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT,15, 15, false, "", "", QList<QString>{" ", "*"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT,16, 21},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 22, 27},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 28, 33},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 34, 39},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 40, 45}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_RO, LINESTATUSTYPE::CONV_RO_FG})),
        QSharedPointer<LineCondition>(new LineCondition("D4",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_D4,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 19},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 20, 20, false, "", "", QList<QString>{" ", "*"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 27, 32},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 38},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 44},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 45, 50},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 56},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 57, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false, "Xc(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false, "Yc(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 85, false, "Xmin(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 86, 90, false, "Xmax(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 95, false, "Ymin(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 96, 100, false, "Ymax(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, false, "Zmin(m)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, false, "Zmax(m)"},
                          }))
        } +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_D4, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new D3LineCondition("D3",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_D3,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 22, 22, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 28, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 29, 34, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 35, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 41, 41, true, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 42, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 51, 52, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 53, 58, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 59, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 65, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 71, 71, false, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 72, 80, false}},
                      QList<LINESTATUSTYPE>(),
                      1)),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 22},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 28, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 29, 34, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 35, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 41, 41, true, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 42, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 51, 52, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 53, 58, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 59, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 65, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 71, 71, false, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 72, 80, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_D3})),
        QSharedPointer<LineCondition>(new D3LineCondition("D3T",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_D3T,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 22, 22},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 28, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 29, 34, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 35, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 41, 41, true, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 42, 50, true, "", ""}}))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_D3T, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("BC",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "X", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "Y", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "Z", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 18, 18, true, u8"θX", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, u8"θY", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, true, u8"θZ", "", QList<QString>{"0", "1"}}}))
        } +

        Create(ANALYSISSTEP::STEP_CONV,LINESTATUSTYPE::CONV_BC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("WM",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_WM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 13},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 14, 15, true, "", "", QList<QString>{"WI", "WH", "SI", "SH"}}}))
        } +
        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_WM, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("DE",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DE,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 22, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 23, true, "", "", QList<QString>{"R", "S", "T"}}})),
        QSharedPointer<LineCondition>(new LineCondition("SC",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 22, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 24, false, "", "", QList<QString>{" 0", " 1", " 2", "10", "11", "12", "20", "21", "22", "32"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 25, 26},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 27, 27, false, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 28, 28, false, "", "", QList<QString>{"0", "1", "2"}}})),
        QSharedPointer<LineCondition>(new LineCondition(" R",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 3},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 4, 4, false, "", "", QList<QString>{"B"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 5},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 6, 6, false, "", "", QList<QString>{"T"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 7, 10},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 11, 11, false, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 12, 12, false, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 13, 14, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 17, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 18, 22, false, "", "0.07"},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 23, false, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 24, 25, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 26, 28, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 29, 33, false, "", "0.07"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 34, 35},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 36, 36, false, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 37, 37, false, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 38, 39, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 40, 42, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 43, 47, false, "", "0.07"},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 48, 48, false, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 49, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 53, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 54, 58, false, "", "0.07"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 59, 60},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 61, 62, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 63, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 65, 67, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 68, 72, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 73, 75},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 76, 80, false, "", "0.08"}},
                          QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SC})),
        QSharedPointer<LineCondition>(new LineCondition("SG",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 22, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 24, false, "", "", QList<QString>{" 0", " 1", " 2", "10", "11", "12", "20", "21", "22", "32"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 25, 26},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 27, 27, false, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 28, 28, false, "", "", QList<QString>{"0", "1", "2"}}})),
        QSharedPointer<LineCondition>(new LineCondition(" R",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SG_R,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 3},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 4, 4, false, "", "", QList<QString>{"I"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 5, 5, false, "", "", QList<QString>{"C"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 6, 6, false, "", "", QList<QString>{"J"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 7, 10},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 11, 12, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 13, 15, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 16, 20, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 22, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 25, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 26, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 35, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 36, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 44, 45, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 46, 47, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 48, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 55, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 56, 60, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SG, LINESTATUSTYPE::CONV_SG_R2})),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SG_R2,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 10},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 11, 12, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 13, 15, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 16, 20, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 22, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 25, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 26, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 35, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 36, 40, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SG_R})),
        QSharedPointer<LineCondition>(new LineCondition(" S",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 3},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 4, 4, false, "", "", QList<QString>{"I"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 5, 5, false, "", "", QList<QString>{"C"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 6, 6, false, "", "", QList<QString>{"J"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 7, 73},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 74, 81, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 82, 82, false, "", "", QList<QString>{"0", "1"}}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SG, LINESTATUSTYPE::CONV_SG_R2})),
        QSharedPointer<LineCondition>(new LineCondition("SN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 22, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 24, false, "", "", QList<QString>{" 0", " 1", " 2", "10", "11", "12", "20", "21", "22", "32"}}})),
        QSharedPointer<LineCondition>(new LineCondition(" QY",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SN_QY,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 4, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true, "", "(=31-40)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true, "", "(=41-50)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true, "", "(=51-60)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 100, true, "", "(=31-40)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 110, true, "", "(=41-50)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=51-60)"}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN})),
        QSharedPointer<LineCondition>(new LineCondition(" QZ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SN_QZ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 4, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 21, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true, "", "(=31-40)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true, "", "(=41-50)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true, "", "(=51-60)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 100, true, "", "(=31-40)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 110, true, "", "(=41-50)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=51-60)"}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_QY})),
        QSharedPointer<LineCondition>(new LineCondition(" C",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SN_C,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 3},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 4, 4, false, "", "", QList<QString>{"I"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 5, 5, false, "", "", QList<QString>{"J"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 6, 6, false, "", "", QList<QString>{"C"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 7, 7},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 8, 15, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_QZ, LINESTATUSTYPE::CONV_SN_C, LINESTATUSTYPE::CONV_SN_CX, LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX, LINESTATUSTYPE::CONV_SN_R})),
        QSharedPointer<LineCondition>(new LineCondition("  CC",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_C, LINESTATUSTYPE::CONV_SN_CX})),
        QSharedPointer<LineCondition>(new LineCondition("  CO",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_C, LINESTATUSTYPE::CONV_SN_CX})),
        QSharedPointer<LineCondition>(new LineCondition("  CR",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_C, LINESTATUSTYPE::CONV_SN_CX})),
        QSharedPointer<LineCondition>(new LineCondition("  CE",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_C, LINESTATUSTYPE::CONV_SN_CX})),
        QSharedPointer<LineCondition>(new LineCondition("  CX",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SN_CX,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_C, LINESTATUSTYPE::CONV_SN_CX})),
        QSharedPointer<LineCondition>(new LineCondition("   ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_CX})),
        QSharedPointer<LineCondition>(new LineCondition(" S",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SN_S,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 3},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 4, 4, false, "", "", QList<QString>{"I"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 5, 5, false, "", "", QList<QString>{"J"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 6, 6, false, "", "", QList<QString>{"C"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 7, 7},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 8, 15, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_QZ, LINESTATUSTYPE::CONV_SN_C, LINESTATUSTYPE::CONV_SN_CX, LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX, LINESTATUSTYPE::CONV_SN_R})),
        QSharedPointer<LineCondition>(new LineCondition("  SC",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("  SO",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("  SR",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("  SE",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("  SH",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("  SI",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("  SB",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("  SG",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("  ST",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("  SX",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SN_SX,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition("   ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_SX})),
        QSharedPointer<LineCondition>(new LineCondition(" R",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SN_R,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 3},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 4, 4, false, "", "", QList<QString>{"I"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 5, 5, false, "", "", QList<QString>{"J"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 6, 6, false, "", "", QList<QString>{"C"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 7, 7},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 8, 15, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_QZ, LINESTATUSTYPE::CONV_SN_C, LINESTATUSTYPE::CONV_SN_CX, LINESTATUSTYPE::CONV_SN_S, LINESTATUSTYPE::CONV_SN_SX, LINESTATUSTYPE::CONV_SN_R})),
        QSharedPointer<LineCondition>(new LineCondition("  RN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_R}, 4)),
        QSharedPointer<LineCondition>(new LineCondition("  RN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_R})),
        QSharedPointer<LineCondition>(new LineCondition("  RM",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_R}, 4)),
        QSharedPointer<LineCondition>(new LineCondition("  RM",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_R})),
        QSharedPointer<LineCondition>(new LineCondition("  RR",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_R}, 4)),
        QSharedPointer<LineCondition>(new LineCondition("  RR",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_R})),
        QSharedPointer<LineCondition>(new LineCondition("  RX",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_R}, 4)),
        QSharedPointer<LineCondition>(new LineCondition("  RX",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_SN_R})),
        QSharedPointer<LineCondition>(new LineCondition("AP",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_AP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 10, true, "", "", QList<QString>{" 1", " 2", " 8", " 9"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}})),
        QSharedPointer<LineCondition>(new LineCondition("SP",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false}})),
        QSharedPointer<LineCondition>(new LineCondition("NPC",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_NPC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3}}))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_NPC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("NPJ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_NPJ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3}}))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_NPJ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("NPS",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_NPS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 4, 20},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 22, 22, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 23, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 24, 24, true, "", "", QList<QString>{"0", "1"}}}))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_NPS, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("BL PP",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_PP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, true},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("P ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_PP_P,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 8},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 27, 27},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 28, 28, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 29, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 35, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 36, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 45, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 46, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 55, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 56, 60, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_PP}))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_BL_PP_P, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("MZ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_MZ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 17, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_MZ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("MR",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_MR,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 73, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 96, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 97, 104, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 105, 112, false},
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_MR, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("ME",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_ME,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 18, 18, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, true, "", "", QList<QString>{"0", "1"}}
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_ME, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("MH",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_MH,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 25, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 26, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 35, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 36, 43, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 44, 44, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 45, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 55, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 56, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 65, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 66, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 75, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 76, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 81, 88, true, "", "SD295"},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 89, 89, false, "", "", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 90, 90, false, "", "", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 91, 91, false, "", "", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 92, 92, false, "", "", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 93, 93, false, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 94, 94, false, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 95},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 96, 100, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 115, false},
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_MH, QList<MEMBERCATEGORY>{MEMBERCATEGORY::GIRDER}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("EM",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_EM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CC",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_CC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 33, 48},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 73, 80, true},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CG",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_CG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 73, 80, true},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CV",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_CV,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 33, 48},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 73, 80, true},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("AM",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_AM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 33, 48},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 73, 80, true},
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_AM, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("EW",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_EW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CW",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_CW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CZ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_CZ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CS",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_CS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("AW",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_AW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_AW, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("AZ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_AZ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_AZ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::ZWALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("AS",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_AS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_AS, QList<MEMBERCATEGORY>{MEMBERCATEGORY::SLAB}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("ML",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_ML,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 19},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 20, 20, true, "", "", QList<QString>{"Y", "Z"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "(def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "(def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "(def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true, "", "(def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true, "", "(def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true, "", "(def)"}
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_ML, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("MC",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_MC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_MC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("AB",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_AB,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 10, true, "", "", QList<QString>{" 1", " 2", " 3", " 8", " 9"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("BL BJ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BJ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_AB, LINESTATUSTYPE::CONV_BL_BJ_BJ}
                      ))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_MC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("BJ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BJ_BJ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 11},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 12, 12, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 13, 13, false, "", "", QList<QString>{"X", "Y", "Z"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 14, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 21, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BJ}
                      ))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_BL_BJ_BJ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("BZ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BZ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 9},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 10, 10, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_AB, LINESTATUSTYPE::CONV_BL_BJ_BJ}
                      )),
        QSharedPointer<LineCondition>(new LineCondition("BL BN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BZ, LINESTATUSTYPE::CONV_BL_BN_BN2}
                      )),
        QSharedPointer<LineCondition>(new LineCondition("BN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BN_BN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 21, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 41, 60},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 61, 66, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 67, 70},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 75, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 76, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 85, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BN}
                      )),
        QSharedPointer<LineCondition>(new TwoPointLineCondition(" ",
                              ANALYSISSTEP::STEP_CONV,
                              LINESTATUSTYPE::CONV_BL_BN_BN2,
                              QList<TextDataUnit>{
                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 8},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 20, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 27, 32, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 38, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 44, true},
                              },
                              QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BN_BN, LINESTATUSTYPE::CONV_BL_BN_BN2}
                              )),
        QSharedPointer<LineCondition>(new LineCondition("BL BQ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BQ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BZ, LINESTATUSTYPE::CONV_BL_BN_BN2, LINESTATUSTYPE::CONV_BL_BQ_BQ2}
                      )),
        QSharedPointer<LineCondition>(new LineCondition("BQ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BQ_BQ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2, true,"", "BQ"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 21, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 45, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 46, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 55, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 56, 60},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 61, 66, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BQ}
                      )),
        QSharedPointer<LineCondition>(new TwoPointLineCondition(" ",
                              ANALYSISSTEP::STEP_CONV,
                              LINESTATUSTYPE::CONV_BL_BQ_BQ2,
                              QList<TextDataUnit>{
                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 8},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 20, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 27, 32, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 38, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 44, true},
                              },
                              QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BQ_BQ}
                              )),
        QSharedPointer<LineCondition>(new LineCondition("BL BY",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BY,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BZ, LINESTATUSTYPE::CONV_BL_BN_BN2, LINESTATUSTYPE::CONV_BL_BQ_BQ2, LINESTATUSTYPE::CONV_BL_BY_BY2}
                      )),
        QSharedPointer<LineCondition>(new LineCondition("BY",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BY_BY,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 26, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 27, 27},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 28, 28, true, "", "", QList<QString>{"2", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 29, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 61, 66, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BY}
                      )),
        QSharedPointer<LineCondition>(new TwoPointLineCondition(" ",
                              ANALYSISSTEP::STEP_CONV,
                              LINESTATUSTYPE::CONV_BL_BY_BY2,
                              QList<TextDataUnit>{
                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 8},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 20, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 27, 32, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 38, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 44, true},
                              },
                              QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BY_BY}
                              )),
        QSharedPointer<LineCondition>(new LineCondition("BL BS",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BZ, LINESTATUSTYPE::CONV_BL_BN_BN2, LINESTATUSTYPE::CONV_BL_BQ_BQ2, LINESTATUSTYPE::CONV_BL_BY_BY2, LINESTATUSTYPE::CONV_BL_BS_BS2}
                      )),
        QSharedPointer<LineCondition>(new LineCondition("BS",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BS_BS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 21, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 61, 66, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BS}
                      )),
        QSharedPointer<LineCondition>(new TwoPointLineCondition(" ",
                              ANALYSISSTEP::STEP_CONV,
                              LINESTATUSTYPE::CONV_BL_BS_BS2,
                              QList<TextDataUnit>{
                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 8},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 20, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 27, 32, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 38, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 44, true},
                              },
                              QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BS_BS}
                              )),
        QSharedPointer<LineCondition>(new LineCondition("BL BT",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BT,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 5},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BZ, LINESTATUSTYPE::CONV_BL_BN_BN2, LINESTATUSTYPE::CONV_BL_BQ_BQ2, LINESTATUSTYPE::CONV_BL_BY_BY2, LINESTATUSTYPE::CONV_BL_BS_BS2, LINESTATUSTYPE::CONV_BL_BT_BT2}
                      )),
        QSharedPointer<LineCondition>(new LineCondition("BT",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_BL_BT_BT,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 21, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 51, 56, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BT}
                      )),
        QSharedPointer<LineCondition>(new TwoPointLineCondition(" ",
                              ANALYSISSTEP::STEP_CONV,
                              LINESTATUSTYPE::CONV_BL_BT_BT2,
                              QList<TextDataUnit>{
                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 8},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 20, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 27, 32, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 38, true},
                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 44, true},
                              },
                              QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_BL_BT_BT}
                              )),
        QSharedPointer<LineCondition>(new LineCondition("MT",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_MT,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 20, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 26, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 27, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 33, 38, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 39, 40},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true, "", "1.0"},
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_MT, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("PN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_PN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 8},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 9, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 10, 10, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 12, true, "", "", QList<QString>{"RC"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 13, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "(=0.2Dj)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 71, 80},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 81, 88, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 89, 90},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 95, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 96, 100, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 115, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 116, 120, false}},
                      QList<LINESTATUSTYPE>(),4)),
        QSharedPointer<LineCondition>(new LineCondition("PN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_PN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 8},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 9, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 10, 10, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 11, true, "", "", QList<QString>{"H"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 12, 19},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 61, 90},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 95, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 96, 100, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 115, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 116, 120, false}},
                      QList<LINESTATUSTYPE>(), 4)),
        QSharedPointer<LineCondition>(new LineCondition("PN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_PN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 8},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 9, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 10, 10, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 11, true, "", "", QList<QString>{"S"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 12, 19},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 20, 20, true, "", "", QList<QString>{"H", "I", "B"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 61, 90},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 95, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 96, 100, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 115, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 116, 120, false}},
                      QList<LINESTATUSTYPE>(), 6)),
        QSharedPointer<LineCondition>(new LineCondition("PN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_PN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 8},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 9, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 10, 10, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 11, true, "", "", QList<QString>{"S"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 12, 19},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 20, 20, true, "", "", QList<QString>{"P"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 41, 90},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 95, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 96, 100, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 115, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 116, 120, false}},
                      QList<LINESTATUSTYPE>(), 6)),
        QSharedPointer<LineCondition>(new LineCondition("PN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::DUMMY,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 8},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 20, true, "", "", QList<QString>{"0", "1"}},  // intentional error
                          TextDataUnit{TEXTTYPE::TYPE_INT, 10, 10, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 12, true, "", ""},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 13, 19},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 20, 20, true, "", ""},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 41, 90},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 95, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 96, 100, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 115, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 116, 120, false}})),
        } +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_PN, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("WN",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_WN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true, "", "(=def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "(=def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "(=def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "(=def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true, "", "(=def)"}
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_WN, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("DK",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DK,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true}
                      })),
        QSharedPointer<LineCondition>(new DkLineCondition(" ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 8},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 27, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 38, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_DK})),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{" 0", "00", "0 "}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 31, 90},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false},
                      }, QList<LINESTATUSTYPE>{}, 6)),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{"21"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 51, 80},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false},
                      }, QList<LINESTATUSTYPE>{}, 6)),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{"22"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false},
                      }, QList<LINESTATUSTYPE>{}, 6)),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{"23"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 71, 80},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false},
                      }, QList<LINESTATUSTYPE>{}, 6)),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{"24"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 65, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 66, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 75, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 76, 80},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 85, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 86, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false},
                      }, QList<LINESTATUSTYPE>{}, 6)),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{"25"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 71, 80},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false},
                      }, QList<LINESTATUSTYPE>{}, 6)),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{"26"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 61, 80},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false},
                      }, QList<LINESTATUSTYPE>{}, 6)),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{"27"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 41, 80},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false},
                      }, QList<LINESTATUSTYPE>{}, 6)),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_DL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{"29"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 61, 80},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false},
                      }, QList<LINESTATUSTYPE>{}, 6)),
        QSharedPointer<LineCondition>(new LineCondition("DL",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::DUMMY,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 15},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true, "", "", QList<QString>{"XX"}}, // intentinal error
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 91, 94, true, "", "SHLK"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 95, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 106, 110, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 111, 120, true, "", "(=infinity)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 121, 125, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 126, 129, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 130, 130, false, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 131, 140, false}})),
        QSharedPointer<LineCondition>(new LineCondition("SD                   ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SD,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_DL})),
        QSharedPointer<LineCondition>(new LineCondition("SD ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SD,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 27, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 44, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 45, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 56, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 57, 62, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 63, 68, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 69, 74, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_DL})),
        QSharedPointer<LineCondition>(new LineCondition("SD",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_SD,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 11, 16, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 22, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 28, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CONV_DL})),
        QSharedPointer<LineCondition>(new LineCondition("CJ",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_CJ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_CJ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("PD",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_PD,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 21, true, "", "", QList<QString>{"H", "V"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 22, 22, true, "", "", QList<QString>{"0","1"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 28, false}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("LC",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV_LC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 26, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 27, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 36, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 37, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 56, false},
                      }))} +

        Create(ANALYSISSTEP::STEP_CONV, LINESTATUSTYPE::CONV_LC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE});

QList<QSharedPointer<LineCondition>> AnalysisConditionReader::premConditions = QList<QSharedPointer<LineCondition>>{};

QList<QSharedPointer<LineCondition>> AnalysisConditionReader::nminConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("CR",
                      ANALYSISSTEP::STEP_NMIN,
                      LINESTATUSTYPE::NMIN_CR,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 11, 11, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 12, 12, true, "", "", QList<QString>{"0", "1"}},
                      })),
        };

QList<QSharedPointer<LineCondition>> AnalysisConditionReader::presConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("FM",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_FM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 16, true, "", "", QList<QString>{"SEMASS"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 17, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "1.0"},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("SEISMIC",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_AI,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 7},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 8, 8},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 16},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 18, true, "", "", QList<QString>{"AI"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 19, 19},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, false, "", "", QList<QString>{"0", "1", "2"}}
                      },
                      QList<LINESTATUSTYPE>(),
                      4)),
        QSharedPointer<LineCondition>(new LineCondition("FG",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_AI_FG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 16, 20, false, "", "0.1"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "", QList<QString>{"0.4", "0.6", "0.8"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_AI})),
        QSharedPointer<LineCondition>(new LineCondition("SEISMIC",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_CI,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 7},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 8, 8},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 16},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 18, true, "", "", QList<QString>{"CI"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 19, 19},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, true, "", "", QList<QString>{"0", "1", "2"}}
                      },
                      QList<LINESTATUSTYPE>(),
                      4)),
        QSharedPointer<LineCondition>(new LineCondition("FG",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_CI_FG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 16, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 43, true, "", "", QList<QString>{"0.4", "0.6", "0.8"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 44, 50},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_CI})),
        QSharedPointer<LineCondition>(new LineCondition("FT",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_CI_FT,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true, "", "0.0"}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_CI_FG})),
        QSharedPointer<LineCondition>(new LineCondition("FU",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_CI_FU,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_CI_FT, LINESTATUSTYPE::PRES_SEISMIC_CI_FU})),
        QSharedPointer<LineCondition>(new LineCondition("FD",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_CI_FD,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_CI_FT, LINESTATUSTYPE::PRES_SEISMIC_CI_FU, LINESTATUSTYPE::PRES_SEISMIC_CI_FD})),
        QSharedPointer<LineCondition>(new LineCondition("SEISMIC",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_KI,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 7},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 8, 8},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 16},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 18, true, "", "", QList<QString>{"KI"}}
                      },
                      QList<LINESTATUSTYPE>(),
                      4)),
        QSharedPointer<LineCondition>(new LineCondition("FG",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_KI_FG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_KI})),
        // TODO:How to assign Rigid Floor
        QSharedPointer<LineCondition>(new LineCondition("FK",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_KI_FK,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_KI_FG}))} +

        Create(ANALYSISSTEP::STEP_PRES, LINESTATUSTYPE::PRES_SEISMIC_KI_FK, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("SEISMIC",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_FI,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 7},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 8, 8},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 16},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 18, true, "", "", QList<QString>{"FI"}}
                      },
                      QList<LINESTATUSTYPE>(),
                      4)),
        QSharedPointer<LineCondition>(new LineCondition("FG",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_FI_FG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_FI})),
        // TODO:check
        QSharedPointer<LineCondition>(new LineCondition("FF",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_FI_FF,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_FI_FG}))} +

        Create(ANALYSISSTEP::STEP_PRES, LINESTATUSTYPE::PRES_SEISMIC_FI_FF, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("SEISMIC",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_QN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 7},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 8, 8},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 16},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 18, true, "", "", QList<QString>{"QN"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 19, 19},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, true, "", "", QList<QString>{"0", "1", "2"}}
                      },
                      QList<LINESTATUSTYPE>(),
                      4)),
        QSharedPointer<LineCondition>(new LineCondition("FG",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_QN_FG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 16, 20, true, "", "0.1"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 43, true, "", "", QList<QString>{"0.4", "0.6", "0.8"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 44, 50},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true, "", "0.0"},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_QN})),
        QSharedPointer<LineCondition>(new LineCondition("FN",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SEISMIC_QN_FN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 13, false, "", "", QList<QString>{"S  ", "RC ", "SRC"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 14, 15},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 17, true, "", "", QList<QString>{"FA", "FB", "FC", "FD"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{"WA", "WB", "WC", "WD"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 23, 25},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 26, 26, true, "", "", QList<QString>{"A", "B", "C"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 27, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_SEISMIC_QN_FG})),
        QSharedPointer<LineCondition>(new LineCondition("SEISMIC",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::DUMMY,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 7},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 8, 8},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 16},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 18, true, "", "", QList<QString>{"XX"}}, // intentional error
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 19, 19},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, true, "", "", QList<QString>{"0", "1", "2"}}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("C ",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_C,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 80, false}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("FS",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_FS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "1.0"}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("SE",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_SE,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "1.0"}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("PJ",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_PJ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_PRES, LINESTATUSTYPE::PRES_PJ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("DJ",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_DJ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}
                      }))} +

        Create(ANALYSISSTEP::STEP_PRES, LINESTATUSTYPE::PRES_DJ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("LM",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 15, true, "", "", QList<QString>{"G", "L"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 17, true, "", "", QList<QString>{"X ", "Y ", "Z ", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}},
                      QList<LINESTATUSTYPE>(),5)),
        QSharedPointer<LineCondition>(new LineCondition("LM",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 15, true, "", "", QList<QString>{"G", "L"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 17, true, "", "", QList<QString>{"X ", "Y ", "Z ", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"2"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true}},
                      QList<LINESTATUSTYPE>(),5)),
        QSharedPointer<LineCondition>(new LineCondition("LM",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 15, true, "", "", QList<QString>{"G", "L"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 17, true, "", "", QList<QString>{"X ", "Y ", "Z ", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"3"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 31, 40},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true}},
                      QList<LINESTATUSTYPE>(),5)),
        QSharedPointer<LineCondition>(new LineCondition("LM",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 15, true, "", "", QList<QString>{"G", "L"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 17, true, "", "", QList<QString>{"X ", "Y ", "Z ", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"4"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true}},
                      QList<LINESTATUSTYPE>(),5)),
        QSharedPointer<LineCondition>(new LineCondition("LM",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 15, true, "", "", QList<QString>{"G", "L"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 17, true, "", "", QList<QString>{"X ", "Y ", "Z ", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"5"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 31, 40},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true}},
                      QList<LINESTATUSTYPE>(),5)),
        QSharedPointer<LineCondition>(new LineCondition("LM",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 15, true, "", "", QList<QString>{"G", "L"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 17, true, "", "", QList<QString>{"X ", "Y ", "Z ", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"6"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true}},
                      QList<LINESTATUSTYPE>(),5)),
        QSharedPointer<LineCondition>(new LineCondition("LM",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 15, true, "", "", QList<QString>{"G", "L"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 17, true, "", "", QList<QString>{"X ", "Y ", "Z ", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"7"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}},
                      QList<LINESTATUSTYPE>(),5)),
        QSharedPointer<LineCondition>(new LineCondition("LM",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 15, true, "", "", QList<QString>{"G", "L"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 17, true, "", "", QList<QString>{"X ", "Y ", "Z ", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"0"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_PRES, LINESTATUSTYPE::PRES_LM, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("LW",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_PRES, LINESTATUSTYPE::PRES_LW, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("LS",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_LS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_PRES, LINESTATUSTYPE::PRES_LS, QList<MEMBERCATEGORY>{MEMBERCATEGORY::SLAB}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("PF",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_PF,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("DF",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_DF,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("RF",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_RF,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_RF2,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 15, 20, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_RF}))} +

        Create(ANALYSISSTEP::STEP_PRES, LINESTATUSTYPE::PRES_RF2, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("PD",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_PD,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES_PD2,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 15, 20, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 21, 74},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 75, 80, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PRES_PD}))} +

        Create(ANALYSISSTEP::STEP_PRES, LINESTATUSTYPE::PRES_PD2, QList<MEMBERCATEGORY>{MEMBERCATEGORY::ISOLATOR});

QList<QSharedPointer<LineCondition>> AnalysisConditionReader::preaConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("CR ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_CR,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 3},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 4, 4, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 5, 5, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 22, 22, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 23, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 24, 24, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 25, 25, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 26, 26, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 27, 27, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 28, 28, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 29, 29, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 30, 30, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 31, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 32, 32, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 33, 33, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 34, 34, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 35, 35, true, "", "0", QList<QString>{"0", "1"}},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CRM",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_CRM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 4, 20},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "0", QList<QString>{"0", "1", "2", "3", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 22, 22, true, "", "0", QList<QString>{"0", "1", "2", "3", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 23, true, "", "0", QList<QString>{"0", "1", "2", "3", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 24, 24, true, "", "0", QList<QString>{"0", "1", "2", "3", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 25, 25, true, "", "0", QList<QString>{"0", "1", "2", "3", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 26, 26, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 27, 27, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 28, 28, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 29, 29, true, "", "0", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 30, 30, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 31, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 32, 32, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 33, 33, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 34, 34, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 35, 35, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 36, 36, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 37, 37, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 38, 38, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 39, 39, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 40, 40, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 41, 41, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 42, 42, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 43, 43, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 44, 44, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 45, 45, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 46, 46, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 47, 47, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 48, 48, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 49, 49, true, "", "0", QList<QString>{"0", "1"}}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CRW",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_CRW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 4, 20},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 22, 22, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 23, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 24, 24, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 25, 25, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 26, 26, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 27, 27, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 28, 28, true, "", "0", QList<QString>{"0"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 29, 29, true, "", "0", QList<QString>{"0", "1"}}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CRV",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_CRV,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 4, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "500.0"}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("IS ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_IS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_IS})),
        QSharedPointer<LineCondition>(new LineCondition("ISC",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_ISC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 4, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_ISC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("QS",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_QS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_QS})),
        QSharedPointer<LineCondition>(new LineCondition("FM",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_FM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "0", QList<QString>{"0", "1", "2", "3", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 22, 22, true, "", "0", QList<QString>{"0", "1", "2", "3", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 23, true, "", "0", QList<QString>{"0", "1", "2", "3", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 24, 24, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 25, 25, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 26, 26, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 27, 27, true, "", "0", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 28, 28, true, "", "0", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 29, 29, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 30, 30, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 31, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 32, 32, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 33, 33, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 34, 34, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 35, 35, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 36, 36, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 37, 37, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 38, 38, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 39, 39, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 40, 40, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 41, 41, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 42, 42, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 43, 43, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 44, 44, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 45, 45, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 46, 46, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 47, 47, true, "", "0", QList<QString>{"0", "1"}},
                      }))
        } +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_FM, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("FW",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_FW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 18},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 22, 22, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 23, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 24, 24, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 25, 25, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 26, 26, true, "", "0", QList<QString>{"0", "1", "2"}}
                      }))
        } +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_FW, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("EM",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CC",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EM_CC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 33, 48},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 73, 80, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EM, LINESTATUSTYPE::PREA_EM_CG, LINESTATUSTYPE::PREA_EM_CV, LINESTATUSTYPE::PREA_EM_AM})),
        QSharedPointer<LineCondition>(new LineCondition("CG",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EM_CG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 73, 80, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EM, LINESTATUSTYPE::PREA_EM_CC, LINESTATUSTYPE::PREA_EM_CV, LINESTATUSTYPE::PREA_EM_AM})),
        QSharedPointer<LineCondition>(new LineCondition("CV",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EM_CV,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 33, 48},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 73, 80, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EM, LINESTATUSTYPE::PREA_EM_CC, LINESTATUSTYPE::PREA_EM_CG, LINESTATUSTYPE::PREA_EM_AM})),
        QSharedPointer<LineCondition>(new LineCondition("AM",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EM_AM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 33, 48},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 73, 80, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EM, LINESTATUSTYPE::PREA_EM_CC, LINESTATUSTYPE::PREA_EM_CG, LINESTATUSTYPE::PREA_EM_CV, LINESTATUSTYPE::PREA_EM_AM}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_EM_AM, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("EW",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CW",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EW_CW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EW, LINESTATUSTYPE::PREA_EW_CZ, LINESTATUSTYPE::PREA_EW_CS, LINESTATUSTYPE::PREA_EW_AW, LINESTATUSTYPE::PREA_EW_AZ, LINESTATUSTYPE::PREA_EW_AS})),
        QSharedPointer<LineCondition>(new LineCondition("CZ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EW_CZ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EW, LINESTATUSTYPE::PREA_EW_CW, LINESTATUSTYPE::PREA_EW_CS, LINESTATUSTYPE::PREA_EW_AW, LINESTATUSTYPE::PREA_EW_AZ, LINESTATUSTYPE::PREA_EW_AS})),
        QSharedPointer<LineCondition>(new LineCondition("CS",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EW_CS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EW, LINESTATUSTYPE::PREA_EW_CW, LINESTATUSTYPE::PREA_EW_CZ, LINESTATUSTYPE::PREA_EW_AW, LINESTATUSTYPE::PREA_EW_AZ, LINESTATUSTYPE::PREA_EW_AS})),
        QSharedPointer<LineCondition>(new LineCondition("AW",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EW_AW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EW, LINESTATUSTYPE::PREA_EW_CW, LINESTATUSTYPE::PREA_EW_CZ, LINESTATUSTYPE::PREA_EW_CS, LINESTATUSTYPE::PREA_EW_AW, LINESTATUSTYPE::PREA_EW_AZ, LINESTATUSTYPE::PREA_EW_AS}))
        } +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_EW_AW, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("AZ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EW_AZ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EW, LINESTATUSTYPE::PREA_EW_CW, LINESTATUSTYPE::PREA_EW_CZ, LINESTATUSTYPE::PREA_EW_CS, LINESTATUSTYPE::PREA_EW_AW, LINESTATUSTYPE::PREA_EW_AZ, LINESTATUSTYPE::PREA_EW_AS}))
        } +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_EW_AZ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::ZWALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("AS",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_EW_AS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_EW, LINESTATUSTYPE::PREA_EW_CW, LINESTATUSTYPE::PREA_EW_CZ, LINESTATUSTYPE::PREA_EW_CS, LINESTATUSTYPE::PREA_EW_AW, LINESTATUSTYPE::PREA_EW_AZ, LINESTATUSTYPE::PREA_EW_AS}))
        } +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_EW_AS, QList<MEMBERCATEGORY>{MEMBERCATEGORY::SLAB}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("PM1",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM1,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("MS",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM1_MS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 16, true, "", "", QList<QString>{"IJ", "I ", "J "}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 18, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 22, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 25, 26, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 27, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 41, 100},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 110, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM1})),
        QSharedPointer<LineCondition>(new LineCondition("S ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM1_MS_S,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 1},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM1_MS, LINESTATUSTYPE::PREA_PM1_MS_C, LINESTATUSTYPE::PREA_PM1_MS_RC}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PM1_MS_S, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("C ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM1_MS_C,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 1},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM1_MS, LINESTATUSTYPE::PREA_PM1_MS_S, LINESTATUSTYPE::PREA_PM1_MS_RC}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PM1_MS_C, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("RC",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM1_MS_RC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM1_MS, LINESTATUSTYPE::PREA_PM1_MS_S, LINESTATUSTYPE::PREA_PM1_MS_C}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PM1_MS_RC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("HG",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM1_HG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 16, true, "", "", QList<QString>{"IJ", "I ", "J "}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 18, true, "", "", QList<QString>{" 0", " 1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 19, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 100, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 110, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM1, LINESTATUSTYPE::PREA_PM1_MS_S, LINESTATUSTYPE::PREA_PM1_MS_C, LINESTATUSTYPE::PREA_PM1_MS_RC}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PM1_HG, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("AX",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM1_AX,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 16, true, "", "", QList<QString>{"IJ", "I ", "J "}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 18, true, "", "", QList<QString>{" 0", " 1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 19, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 100, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 110, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM1, LINESTATUSTYPE::PREA_PM1_MS_S, LINESTATUSTYPE::PREA_PM1_MS_C, LINESTATUSTYPE::PREA_PM1_MS_RC, LINESTATUSTYPE::PREA_PM1_HG}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PM1_AX, QList<MEMBERCATEGORY>{MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("ZS",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM1_ZS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 16, true, "", "", QList<QString>{"IJ", "I ", "J "}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 22, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 25, 29},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 30, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("Z-",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM1_Z_,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 16},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 17, 22, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 24, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 25, 29},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 30, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM1_ZS, LINESTATUSTYPE::PREA_PM1_Z_}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PM1_ZS, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER}) +
        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PM1_Z_, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("PM2",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM2,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CC",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM2_CC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"2", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"1", "2", "8", "9"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 88, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 89, 96, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 97, 104, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM2})),
        QSharedPointer<LineCondition>(new LineCondition("CG",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM2_CG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"2", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"1", "2", "8", "9"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 88, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 89, 96, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 97, 104, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM2})),
        QSharedPointer<LineCondition>(new LineCondition("CV",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM2_CV,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"2", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"1", "2", "8", "9"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 88, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 89, 96, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 97, 104, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM2})),
        QSharedPointer<LineCondition>(new LineCondition("AM",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PM2_AM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"2", "4"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"1", "2", "8", "9"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 65, 72, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 88, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 89, 96, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 97, 104, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PM2, LINESTATUSTYPE::PREA_PM2_AM}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PM2_AM, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("PW1",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PW1,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("MS",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PW1_MS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "0"},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "1"},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 18, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 19, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PW1})),
        QSharedPointer<LineCondition>(new LineCondition("S ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PW1_MS_S,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 1},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PW1_MS, LINESTATUSTYPE::PREA_PW1_MS_C, LINESTATUSTYPE::PREA_PW1_MS_RC}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PW1_MS_S, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("C ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PW1_MS_C,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 1},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PW1_MS, LINESTATUSTYPE::PREA_PW1_MS_S, LINESTATUSTYPE::PREA_PW1_MS_RC}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PM1_MS_C, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("RC",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PW1_MS_RC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PW1_MS, LINESTATUSTYPE::PREA_PW1_MS_S, LINESTATUSTYPE::PREA_PW1_MS_C}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PW1_MS_RC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("HG",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PW1_HG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "1", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 18, true, "", "", QList<QString>{" 0", " 1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 19, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 90, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 100, true},
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PW1, LINESTATUSTYPE::PREA_PW1_MS_S, LINESTATUSTYPE::PREA_PW1_MS_C, LINESTATUSTYPE::PREA_PW1_MS_RC}))} +
        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PW1_HG, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("PW2",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PW2,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 3}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CW",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PW2_CW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, false}

                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PW2})),
        QSharedPointer<LineCondition>(new LineCondition("AW",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_PW2_AW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 17, 24, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 25, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 33, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 48, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 49, 56, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 57, 64, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_PW2, LINESTATUSTYPE::PREA_PW2_CW, LINESTATUSTYPE::PREA_PW2_AW}))} +
        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_PW2_AW, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("BK",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_BK,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, false},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("BC",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_BC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "X", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "Y", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "Z", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 18, 18, true, "ThetaX", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "ThetaY", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, true, "ThetaZ", "", QList<QString>{"0", "1"}}}))} +

        Create(ANALYSISSTEP::STEP_PREA, LINESTATUSTYPE::PREA_BC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new D3LineCondition("D3",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_D3,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 22, 22, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 28, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 29, 34, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 35, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 41, 41, true, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 42, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 51, 52, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 53, 58, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 59, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 65, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 71, 71, false, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 72, 80, false}})),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 22},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 28, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 29, 34, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 35, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 41, 41, true, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 42, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 51, 52, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 53, 58, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 59, 64, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 65, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 71, 71, false, "", "", QList<QString>{"1", "2", "3", "4", "5", "6"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 72, 80, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::PREA_D3})),
        QSharedPointer<LineCondition>(new LineCondition("E4",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA_E4,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8}
                      }))
        };


QList<QSharedPointer<LineCondition>> AnalysisConditionReader::solsConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("ST",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_ST,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 8, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, false, "", "", QList<QString>{"0", "1", "2", "3", "4", "5"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 16, 17},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 18, 18, true, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, false, "", "", QList<QString>{" ", "0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 20, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "5.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "0.5"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "0.1"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 51, 78},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 79, 80, false, "", "", QList<QString>{"  ", "QU", "DS"}},
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 80, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::SOLS_ST})),
        QSharedPointer<LineCondition>(new LineCondition("F ",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_F,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 1},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 11, true, "", "  1"},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 12, 14, true, "", "  1"},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 17, true, "", "  1"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 18, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "1.0e-7.0"}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CP",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_CP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_MA,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true, "", "   1"},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"MA"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{" U", " V", " A", " B"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 24, true, "", "", QList<QString>{" X", " Y", " Z", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 25, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_EN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true, "", "   1"},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"EN"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{" I", " K", " D", " S", " C", " B", "DP"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 23, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_FL,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true, "", "   1"},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"FL"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{" S", "FS", "BS", "DS", " D", " O"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 24, true, "", "", QList<QString>{" X", " Y"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 25, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        /* 未対応の記載あり
        LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      SOLS_SA_FR,
                      QList<ConditionDataUnit>{
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 3, 6, true},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"FR"}},
                          ConditionDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 11, 14, true},
                          ConditionDataUnit{TEXTTYPE::TYPE_BLANK, 15, 16},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 17, 20, true},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{" S", " D", " O"},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 23, 24, true, "", "", QList<QString>{" X", " Y", " U", " V"},
                          ConditionDataUnit{TEXTTYPE::TYPE_BLANK, 25, 30},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          ConditionDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2),
        */
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_J,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"J "}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{" P", " I"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 24, true, "", "", QList<QString>{" X", " Y", " Z", "RX", "RY", "RZ"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 25, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_M,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"M "}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{"MY", "MZ", "QY", "QZ", " N", " T", "DA", "SY", "SZ", "JY", "DT", "DF", "MB", "MS"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 23, 24, false, "", "", QList<QString>{" I", " J"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 25, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_DP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"DP"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{" N", "DA", "VA", "DD", "VD", "EN"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 23, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_W,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"W "}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{"MI", "MJ", " Q", " N", "DS"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 23, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_BJBNBQ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"BJ", "BN", "BQ"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{" F", " D"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 23, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_BY,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true, "", "", QList<QString>{"BY"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{"FY", "FZ", "DY", "DZ", "EN"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 23, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SA_EI,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true,"", "EI"},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{"FX", "FY", "FV", "DX", "DY", "DV", "EN"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 23, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}},
                          QList<LINESTATUSTYPE>{},2)),
        QSharedPointer<LineCondition>(new LineCondition("SA",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::DUMMY,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 7, 8, true,"", "EI"},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 14, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 15, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 22, true, "", "", QList<QString>{"FX", "FY", "FV", "DX", "DY", "DV", "EN"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 23, 30},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 38, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 39, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 40, 40},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 60, false}})),
        QSharedPointer<LineCondition>(new LineCondition("SB",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SB,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 6, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 7, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 18, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 19, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 28, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 29, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 48, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 49, 50},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 68, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 69, 70},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}})),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 28, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 29, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 48, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 49, 50},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 68, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 69, 70},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::SOLS_SB})),
        QSharedPointer<LineCondition>(new LineCondition("FM",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_FM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 4},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 5, 10, true, "", "     2"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 27, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                      })),
        /* FMの2行目の仕様がよくわからない
        LineCondition("  ",
                      ANALYSISSTEP::STEP_SOLS,
                      SKIP,
                      QList<ConditionDataUnit>{
                          ConditionDataUnit{TEXTTYPE::TYPE_BLANK, 1, 20},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 21, 28, true},
                          ConditionDataUnit{TEXTTYPE::TYPE_BLANK, 29, 30},
                          ConditionDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 41, 48, false},
                          ConditionDataUnit{TEXTTYPE::TYPE_BLANK, 49, 50},
                          ConditionDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          ConditionDataUnit{TEXTTYPE::TYPE_TEXT, 61, 68, false},
                          ConditionDataUnit{TEXTTYPE::TYPE_BLANK, 69, 70},
                          ConditionDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}},
                      QList<LINESTATUSTYPE>{SOLS_FM}),
        */
        QSharedPointer<LineCondition>(new LineCondition("SP",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_SP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 3, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 4, 4, true, "", "", QList<QString>{"1", "2", "3", "4", "5"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 5, 7},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 8, 10, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 11, 11},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 12, 15, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 16, 25, true, "", "(=def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 26, 30, true, "", "0.05"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true, "", "0.02"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true, "", "1.0"},
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::SOLS_SP})),
        QSharedPointer<LineCondition>(new LineCondition("DS",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_DS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 21, 30},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "0.02"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "20.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true, "", "5.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true, "", "30.0"}
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" S",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_S,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true, "", "1.8"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "300.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "0.02"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "30.0"}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::SOLS_DS})),
        QSharedPointer<LineCondition>(new LineCondition(" D",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_D,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true, "", "2.2"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "1100.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "0.01"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "3000.0"}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::SOLS_S})),
        QSharedPointer<LineCondition>(new LineCondition(" E",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS_E,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true, "", "2.8"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "3000.0"}},
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::SOLS_D})),
        };
QList<QSharedPointer<LineCondition>> AnalysisConditionReader::stecConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("GO",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC_GO,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 31, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 32, 32, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 33, 33, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 34, 34, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 35, 35, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 36, 36, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 37, 37, true, "", "0", QList<QString>{"0", "1", "2"}}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::STEC})),
        QSharedPointer<LineCondition>(new LineCondition("TH",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC_TH,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, true}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::STEC_GO, LINESTATUSTYPE::STEC_TH})),
        QSharedPointer<LineCondition>(new LineCondition("IS",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC_IS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::STEC_TH})),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::STEC_IS})),
        QSharedPointer<LineCondition>(new LineCondition("BH",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC_BH,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("EC",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC_EC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_STEC, LINESTATUSTYPE::STEC_EC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::WALL, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE, MEMBERCATEGORY::SLAB}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("QG",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC_QG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_STEC, LINESTATUSTYPE::STEC_QG, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("RG",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC_QG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 27, 32, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 33, 38, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 39, 44, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 45, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 56, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 57, 62, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 63, 68, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 69, 74, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 75, 80, false}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("CG",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC_CG,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true}
                      })),

        };
QList<QSharedPointer<LineCondition>> AnalysisConditionReader::chckConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("CH",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK_CH,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 3, true, "", "", QList<QString>{"L", "S", "U"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 4, 4, true, "", "0", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 5, 5, true, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 31, true, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 32, 40, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 51, 51, false, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 52, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 71, 71, false, "", "0", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 72, 80, false}
                      })),
        QSharedPointer<LineCondition>(new LineCondition(" ",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::SKIP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 1, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 31, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 32, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 51, 51, false, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 52, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 71, 71, false, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 72, 80, false}
                      },
                      QList<LINESTATUSTYPE>{LINESTATUSTYPE::CHCK_CH})),
        QSharedPointer<LineCondition>(new LineCondition("MC",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK_MC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_CHCK, LINESTATUSTYPE::CHCK_MC, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("MS",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK_MS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 16, 16, true, "", "", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 17, 17, true, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 18, 18, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 19, 19, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 20, 20, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 22, 22, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 23, 23, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 24, 60},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 65, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 66, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 75, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 76, 80, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 85, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 86, 90, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 95, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 96, 100, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, false},
                      }))} +

        Create(ANALYSISSTEP::STEP_CHCK, LINESTATUSTYPE::CHCK_MS, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("WS",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK_WS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 16, 20},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 30, false}
                      }))} +

        Create(ANALYSISSTEP::STEP_CHCK, LINESTATUSTYPE::CHCK_WS, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("MF",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK_MF,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 21, 21, true, "", "I", QList<QString>{"I", "C", "J"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 22, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 31, true, "", "C", QList<QString>{"I", "C", "J"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 32, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 41, 41, true, "", "J", QList<QString>{"I", "C", "J"}},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 42, 50, false}
                      }))} +

        Create(ANALYSISSTEP::STEP_CHCK, LINESTATUSTYPE::CHCK_MF, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN,MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("MQ",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK_MF,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_CHCK, LINESTATUSTYPE::CHCK_MQ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("MD",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK_MD,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, true, "", "1.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, true, "", "(=def)"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 81, 85, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 86, 90, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 91, 95, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 96, 100, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 101, 105, false},
                      }))} +

        Create(ANALYSISSTEP::STEP_CHCK, LINESTATUSTYPE::CHCK_MD, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("WD",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK_WD,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 14},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 15, 15, true, "", "", QList<QString>{"0", "1", "2", "3"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 16, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, false, "", "(=def)"}
                      }))} +

        Create(ANALYSISSTEP::STEP_CHCK, LINESTATUSTYPE::CHCK_WD, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL});

QList<QSharedPointer<LineCondition>> AnalysisConditionReader::qudsConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("CASE",
                      ANALYSISSTEP::STEP_QUDS,
                      LINESTATUSTYPE::QUDS_CASE,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 4},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 5, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 41, 50},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 61, 69},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 70, 70, true, "", "", QList<QString>{"1", "2"}},
                      })),

        };


QList<QSharedPointer<LineCondition>> AnalysisConditionReader::qdcvConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("GO",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_GO,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 27, 29},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 30, 30, true, "", "", QList<QString>{"1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 31, 31, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 32, 32, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 33, 33, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 34, 34, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 35, 35, true, "", "", QList<QString>{"0", "1", "2"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 36, 36, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 37, 40},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 45, true, "", "0.35"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 46, 50, true, "", "45.0"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 55, true, "", "2.0"},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 56, 70},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 71, 71, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 72, 72, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 73, 73, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 74, 74, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 75, 75, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 76, 76, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 77, 77, true, "", "", QList<QString>{"0", "1"}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 78, 78},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 79, 80, true, "", "", QList<QString>{"QU", "DS"}},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("TH",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_TH,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("QN",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_QN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("DS",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_DS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 9, 10},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, false}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("SH",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_SH,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 51, 60, false},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 71, 80, false}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("SQ",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_SQ,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 10, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 40, false},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 45, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 46, 50, true}
                      }))} +

        Create(ANALYSISSTEP::STEP_QDCV, LINESTATUSTYPE::QDCV_SQ, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE, MEMBERCATEGORY::WALL, MEMBERCATEGORY::SLAB}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("QM ",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_QM2,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 25, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 26, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 31, 40},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 45, true},
                      })),
        QSharedPointer<LineCondition>(new LineCondition("QM",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_QM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 5, true, "", "", QList<QString>{"S  ", "RC ", "SRC", "SC "}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 25, true, "", "1.10"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 26, 30, true, "", "1.20"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 35, true, "", "1.10"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 36, 40, true, "", "1.25"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 45, true, "", "0.90"},
                      }))
        } +

        Create(ANALYSISSTEP::STEP_QDCV, LINESTATUSTYPE::QDCV_QM2, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("QW ",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_QW2,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 25, true, "", ""}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("QW",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_QW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 5, true, "", "", QList<QString>{"S  ", "RC ", "SRC", "SC "}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 25, true, "", "1.25"}
                      }))
        } +

        Create(ANALYSISSTEP::STEP_QDCV, LINESTATUSTYPE::QDCV_QW2, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("QP ",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_QP2,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 25, true, "", ""},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 26, 30, true, "", ""}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("QP",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_QP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 3, 5, true, "", "", QList<QString>{"S  ", "RC ", "SRC", "SC "}},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 6, 20},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 25, true, "", "1.1"},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 26, 30, true, "", ""}
                      }))
        } +

        Create(ANALYSISSTEP::STEP_QDCV, LINESTATUSTYPE::QDCV_QP2, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("RM",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_RM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 18},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 19, 19, true, "", "", QList<QString>{"A", "B", "C", "D"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 20, 20, true, "", "", QList<QString>{"A", "B", "C", "D"}}
                      }))} +

        Create(ANALYSISSTEP::STEP_QDCV, LINESTATUSTYPE::QDCV_RM, QList<MEMBERCATEGORY>{MEMBERCATEGORY::COLUMN, MEMBERCATEGORY::GIRDER, MEMBERCATEGORY::BRACE}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("RW",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_RW,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 18},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 19, 19, true, "", "", QList<QString>{"A", "B", "C", "D"}}
                      }))} +

        Create(ANALYSISSTEP::STEP_QDCV, LINESTATUSTYPE::QDCV_RW, QList<MEMBERCATEGORY>{MEMBERCATEGORY::WALL}) +

        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("RP",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV_RP,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_BLANK, 3, 18},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 19, 19, false, "", "", QList<QString>{"*"}},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 20, 20, false, "", "", QList<QString>{"*"}}
                      }))} +

        Create(ANALYSISSTEP::STEP_QDCV, LINESTATUSTYPE::QDCV_RP, QList<MEMBERCATEGORY>{MEMBERCATEGORY::JOINT});

QList<QSharedPointer<LineCondition>> AnalysisConditionReader::modeConditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("MD",
                      ANALYSISSTEP::STEP_MODE,
                      LINESTATUSTYPE::MODE_MD,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 3, 8, true},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 9, 9, true, "", ""},
                          TextDataUnit{TEXTTYPE::TYPE_INT, 10, 10, true, "", ""},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 31, 40, true},
                          TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, true},
                      })),
        // Where is Mass?
        QSharedPointer<LineCondition>(new MaLineCondition("MA",
                        ANALYSISSTEP::STEP_MODE,
                        LINESTATUSTYPE::MODE_MA,
                        QList<TextDataUnit>{
                            TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 2},
                            TextDataUnit{TEXTTYPE::TYPE_INT, 3, 8, false},
                            TextDataUnit{TEXTTYPE::TYPE_TEXT, 9, 15, false},
                            TextDataUnit{TEXTTYPE::TYPE_TEXT, 16, 21, false},
                            TextDataUnit{TEXTTYPE::TYPE_TEXT, 22, 27, false}
                        })),
        }
 ;

QList<QSharedPointer<LineCondition>> AnalysisConditionReader::conditions =
        QList<QSharedPointer<LineCondition>>{
        QSharedPointer<LineCondition>(new LineCondition("===== CONV",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== CONV",
                      ANALYSISSTEP::STEP_CONV,
                      LINESTATUSTYPE::CONV,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== PREM",
                      ANALYSISSTEP::STEP_PREM,
                      LINESTATUSTYPE::PREM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== PREM",
                      ANALYSISSTEP::STEP_PREM,
                      LINESTATUSTYPE::PREM,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== NMIN",
                      ANALYSISSTEP::STEP_NMIN,
                      LINESTATUSTYPE::NMIN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== NMIN",
                      ANALYSISSTEP::STEP_NMIN,
                      LINESTATUSTYPE::NMIN,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== PRES",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== PRES",
                      ANALYSISSTEP::STEP_PRES,
                      LINESTATUSTYPE::PRES,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== PREA",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10},
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 40, false}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== PREA",
                      ANALYSISSTEP::STEP_PREA,
                      LINESTATUSTYPE::PREA,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== SOLS",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== SOLS",
                      ANALYSISSTEP::STEP_SOLS,
                      LINESTATUSTYPE::SOLS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== STEC",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== STEC",
                      ANALYSISSTEP::STEP_STEC,
                      LINESTATUSTYPE::STEC,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== CHCK",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== CHCK",
                      ANALYSISSTEP::STEP_CHCK,
                      LINESTATUSTYPE::CHCK,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== QUDS",
                      ANALYSISSTEP::STEP_QUDS,
                      LINESTATUSTYPE::QUDS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== QUDS",
                      ANALYSISSTEP::STEP_QUDS,
                      LINESTATUSTYPE::QUDS,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== QDCV",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== QDCV",
                      ANALYSISSTEP::STEP_QDCV,
                      LINESTATUSTYPE::QDCV,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("===== MODE",
                      ANALYSISSTEP::STEP_MODE,
                      LINESTATUSTYPE::MODE,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10}
                      })),
        QSharedPointer<LineCondition>(new LineCondition("/===== MODE",
                      ANALYSISSTEP::STEP_MODE,
                      LINESTATUSTYPE::MODE,
                      QList<TextDataUnit>{
                          TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 11}
                      }))
        };
QSharedPointer<LineCondition> AnalysisConditionReader::CheckLineCondition(const QString& text, ANALYSISSTEP step, LINESTATUSTYPE status, bool* isOk){
    QSharedPointer<LineCondition> condition = MatchLineCondition(text, step, status, isOk);
    if (!*isOk)
        return nullptr;

    if (condition == nullptr)
        return nullptr;

    if (condition->hasFormatError(text)){
        *isOk = false;
        return nullptr;
    }
    return condition;
}

QSharedPointer<LineCondition> AnalysisConditionReader::MatchLineCondition(const QString &text, ANALYSISSTEP step, LINESTATUSTYPE status, bool* isOk){
    *isOk = false;
    if (text.startsWith("*")){
        *isOk = true;
        return nullptr;
    }
    if (text.startsWith("END")){
        *isOk = true;
        return endCondition;
    }
    if (text.startsWith("FINAL END")){
        *isOk = true;
        return QSharedPointer<LineCondition>(new LineCondition("FINAL END",
                             ANALYSISSTEP::STEP_NONE,
                             LINESTATUSTYPE::FINALEND,
                             QList<TextDataUnit>{TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 9}}));
    }

    QList<QSharedPointer<LineCondition>> lines;
    switch(step){
    case ANALYSISSTEP::STEP_NONE:
        lines = conditions;
        break;
    case ANALYSISSTEP::STEP_CONV:
        lines = convConditions;
        break;
    case ANALYSISSTEP::STEP_PREM:
        lines = premConditions;
        break;
    case ANALYSISSTEP::STEP_NMIN:
        lines = nminConditions;
        break;
    case ANALYSISSTEP::STEP_PRES:
        lines = presConditions;
        break;
    case ANALYSISSTEP::STEP_PREA:
        lines = preaConditions;
        break;
    case ANALYSISSTEP::STEP_SOLS:
        lines = solsConditions;
        break;
    case ANALYSISSTEP::STEP_STEC:
        lines = stecConditions;
        break;
    case ANALYSISSTEP::STEP_CHCK:
        lines = chckConditions;
        break;
    case ANALYSISSTEP::STEP_QUDS:
        lines = qudsConditions;
        break;
    case ANALYSISSTEP::STEP_QDCV:
        lines = qdcvConditions;
        break;
    case ANALYSISSTEP::STEP_MODE:
        lines = modeConditions;
        break;
    }

    Q_FOREACH(auto condition, lines){
        if (condition->MatchKeyword(step, status, text)){
            *isOk = true;
            return condition;
        }
    }
    return nullptr;
}


QString AnalysisConditionReader::ConvertAnalysisCondition(const QString& txt, QTextStream& out){
    ANALYSISSTEP step = ANALYSISSTEP::STEP_NONE;
    LINESTATUSTYPE status = LINESTATUSTYPE::NONE;
    bool isOk = true;
    QString erase; // D3Tの除外用
    QSet<QString> joints;
    QSet<QString> members;
    QString errorMessage;
    foreach(auto str, txt.split("\n")){
        QSharedPointer<LineCondition> condition = CheckLineCondition(str, step, status, &isOk);
        if (isOk){
            if (condition == nullptr)
                continue;

            step = condition->getStep();
            if (condition->getStatus() != LINESTATUSTYPE::SKIP){
                status = condition->getStatus();
            }


            QString line = condition->convert(str, &joints, &members, &errorMessage);
            if (errorMessage != QString()){
                return errorMessage;
            }

            if (line != QString()){
                // 条件指定が終わった次の行で範囲指定を書き込む
                QSharedPointer<D3TLineCondition> d3TLine = qSharedPointerDynamicCast<D3TLineCondition>(condition);
                if (d3TLine != nullptr){
                    // d3TはjointSetに差し引くための節点を使用している。
                    erase = joints.values().first();
                    out << line << "\n";
                    continue;
                }

                auto maLine = qSharedPointerDynamicCast<MaLineCondition>(condition);
                if (maLine != nullptr){
                    continue;
                }

                if (joints.count() > 0){
                    if (erase != QString{}){
                        joints.remove(erase);
                        erase = QString{};
                    }
                    out << "JOINT     " << QString::number(joints.count()).leftJustified(10);
                    Q_FOREACH(auto joint, joints){
                        out << joint.rightJustified(6);
                    }
                    out << "\n";
                    joints.clear();
                }

                if (members.count() > 0){
                    out << "MEMBER    " << QString::number(members.count()).leftJustified(10);
                    Q_FOREACH(auto member, members){
                        out <<member.rightJustified(6);
                    }
                    out << "\n";
                    members.clear();
                }

                out << line << "\n";
            }

            if (status == LINESTATUSTYPE::FINALEND)
                return QString{};
        }else{
            return "error";
        }
    }
    return QString{};
}



QMap<int, QString> AnalysisConditionReader::ReadGeneralAnalysisCondition(const QStringList& list){
    QMap<int, QString> geneMap;
    Q_FOREACH(auto str, list){
        QRegularExpression reg("^      \\d\\d\\d\\d");
        QRegularExpressionMatch match = reg.match(str);
        if (match.hasMatch()){
            int number = str.mid(6, 4).toInt();
            geneMap.insert(number, str);
        }
    }
    return geneMap;
}

QStringList AnalysisConditionReader::DefaultGeneralAnalysisConditionLines()
{
    if (!defaultGeneralAnalysisConditionLines.empty())
        return defaultGeneralAnalysisConditionLines;

    QFile file(":/post3DLib/include_files/GeneralLineConditionList.csv");
    if (!file.open(QIODevice::ReadOnly))
        throw;
    QTextStream infile(&file);
    QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
    infile.setCodec( codec );
    infile.readLine(); // HEADER
    QStringList lines;
    while(!infile.atEnd()){
        lines.append(infile.readLine());
    }
    defaultGeneralAnalysisConditionLines = lines;
    return defaultGeneralAnalysisConditionLines;
}

QString AnalysisConditionReader::DefaultGeneralAnalysisConditionText()
{
    QString txt;
    foreach(auto l, DefaultGeneralAnalysisConditionMap().values()){
        txt.append(l->write());
    }
    return txt;
}

QMap<int, QSharedPointer<GeneralLineCondition>> AnalysisConditionReader::DefaultGeneralAnalysisConditionMap()
{
    if (!defaultGeneralAnalysisConditionMap.empty())
        return defaultGeneralAnalysisConditionMap;

    auto lines = DefaultGeneralAnalysisConditionLines();
    QMap<int, QSharedPointer<GeneralLineCondition>> map;
    foreach(auto l, lines){
        if (l.startsWith("*"))
            continue;
        QStringList strlist = l.split(",");
        int num = strlist.at(0).toInt();
        map.insert(num, GeneralLineCondition::Create(l));
    }
    defaultGeneralAnalysisConditionMap = map;
    return defaultGeneralAnalysisConditionMap;
}

bool AnalysisConditionReader::hasFormatError(const QString& plainText){
    if (plainText == QString())
        return false;

    ANALYSISSTEP step = ANALYSISSTEP::STEP_NONE;
    LINESTATUSTYPE status = LINESTATUSTYPE::NONE;
    bool isOk = true;

    foreach(auto str, plainText.split("\n")){
        QSharedPointer<LineCondition> condition = CheckLineCondition(str, step, status, &isOk);
        if (!isOk)
            return true;

        if (condition == nullptr)
            return false;

        step = condition->getStep();
        if (condition->getStatus() != LINESTATUSTYPE::SKIP)
            status = condition->getStatus();
        if (status == LINESTATUSTYPE::FINALEND)
            return false;
    }
    return false;
}

QList<QSharedPointer<LineCondition>> AnalysisConditionReader::Create(ANALYSISSTEP step, LINESTATUSTYPE type, const QList<MEMBERCATEGORY> &mlist){
    QList<QSharedPointer<LineCondition>> selectLineConditions;
    selectLineConditions.append(QSharedPointer<LineCondition>(new BulkAreaLineCondition("+", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new BulkAreaLineCondition("-", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new GridAreaLineCondition("+", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new GridAreaLineCondition("-", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new FloorAreaLineCondition("+", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new FloorAreaLineCondition("-", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new NodeAreaCondition("+", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new NodeAreaCondition("-", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new CloseAreaCondition("+", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new CloseAreaCondition("-", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new CoordinateAreaCondition("+", step, QList<LINESTATUSTYPE>{type}, mlist)));
    selectLineConditions.append(QSharedPointer<LineCondition>(new CoordinateAreaCondition("-", step, QList<LINESTATUSTYPE>{type}, mlist)));
    return selectLineConditions;
}

} // namespace post3dapp

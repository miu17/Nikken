#ifndef LINECONDITION_H
#define LINECONDITION_H

#include <QFile>
#include <QList>
#include <QSet>
#include <QString>
#include <QTextStream>
#include <QUuid>
#include "abstractlinedata.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{

enum class ANALYSISSTEP{STEP_NONE, STEP_GENE, STEP_CONV, STEP_PREM, STEP_NMIN, STEP_PRES, STEP_PREA, STEP_SOLS,
                 STEP_STEC, STEP_CHCK, STEP_QUDS, STEP_QDCV, STEP_MODE};

enum class LINESTATUSTYPE{NONE, SKIP, END, DUMMY, FINALEND,
                    CONV, CONV_DFG, CONV_DFC, CONV_RO, CONV_RO_FG, CONV_D4, CONV_D3, CONV_D3T,CONV_BC, CONV_WM, CONV_DE, CONV_SC, CONV_SG, CONV_SG_R, CONV_SG_R2, CONV_SN, CONV_SN_QY, CONV_SN_QZ,
                    CONV_SN_C, CONV_SN_CX, CONV_SN_S, CONV_SN_SX, CONV_SN_R, CONV_AP, CONV_SP, CONV_NPC, CONV_NPJ, CONV_NPS, CONV_BL_PP, CONV_BL_PP_P, CONV_MZ, CONV_MR, CONV_ME, CONV_MH,
                    CONV_EM, CONV_CC, CONV_CG, CONV_CV, CONV_AM, CONV_EW, CONV_CW, CONV_CZ, CONV_CS, CONV_AW, CONV_AZ, CONV_AS, CONV_ML, CONV_MC,
                    CONV_AB, CONV_BL_BJ, CONV_BL_BJ_BJ, CONV_BZ, CONV_BL_BN, CONV_BL_BN_BN, CONV_BL_BN_BN2, CONV_BL_BQ, CONV_BL_BQ_BQ, CONV_BL_BQ_BQ2, CONV_BL_BY, CONV_BL_BY_BY, CONV_BL_BY_BY2,
                    CONV_BL_BS, CONV_BL_BS_BS, CONV_BL_BS_BS2, CONV_BL_BT, CONV_BL_BT_BT, CONV_BL_BT_BT2, CONV_MT, CONV_PN, CONV_WN, CONV_DK, CONV_DL,CONV_SD,CONV_CJ, CONV_PD, CONV_LC,
                    PREM,
                    NMIN, NMIN_CR,
                    PRES, PRES_FM, PRES_SEISMIC_AI,PRES_SEISMIC_AI_FG, PRES_SEISMIC_CI,PRES_SEISMIC_CI_FG, PRES_SEISMIC_CI_FT, PRES_SEISMIC_CI_FU, PRES_SEISMIC_CI_FD,
                    PRES_SEISMIC_KI, PRES_SEISMIC_KI_FG, PRES_SEISMIC_KI_FK, PRES_SEISMIC_FI, PRES_SEISMIC_FI_FG, PRES_SEISMIC_FI_FF,
                    PRES_SEISMIC_QN, PRES_SEISMIC_QN_FG, PRES_SEISMIC_QN_FN,
                    PRES_C,PRES_FS,PRES_SE,PRES_PJ, PRES_DJ, PRES_LM, PRES_LW, PRES_LS, PRES_PF, PRES_DF, PRES_RF, PRES_RF2, PRES_PD, PRES_PD2,
                    PREA, PREA_CR, PREA_CRM, PREA_CRW, PREA_CRV, PREA_IS, PREA_ISC, PREA_QS, PREA_FM, PREA_FW,
                    PREA_EM, PREA_EM_CC, PREA_EM_CG, PREA_EM_CV, PREA_EM_AM,PREA_EW, PREA_EW_CW,PREA_EW_CZ, PREA_EW_CS, PREA_EW_AW, PREA_EW_AZ,PREA_EW_AS,
                    PREA_PM1, PREA_PM1_MS, PREA_PM1_MS_S, PREA_PM1_MS_C, PREA_PM1_MS_RC, PREA_PM1_HG, PREA_PM1_AX,
                    PREA_PM1_ZS, PREA_PM1_Z_, PREA_PM2, PREA_PM2_CC, PREA_PM2_CG, PREA_PM2_CV, PREA_PM2_AM,
                    PREA_PW1, PREA_PW1_MS, PREA_PW1_MS_S, PREA_PW1_MS_C, PREA_PW1_MS_RC, PREA_PW1_HG,
                    PREA_PW2, PREA_PW2_CW, PREA_PW2_AW, PREA_BK, PREA_BC, PREA_D3, PREA_E4,
                    SOLS, SOLS_ST, SOLS_F, SOLS_CP, SOLS_SA_MA, SOLS_SA_EN, SOLS_SA_FL, SOLS_SA_FR,
                    SOLS_SA_J, SOLS_SA_M, SOLS_SA_DP, SOLS_SA_W, SOLS_SA_BJBNBQ, SOLS_SA_BY, SOLS_SA_EI,SOLS_SB, SOLS_FM, SOLS_SP,
                    SOLS_DS,SOLS_S, SOLS_D, SOLS_E,
                    STEC,STEC_GO, STEC_TH, STEC_IS, STEC_BH, STEC_EC, STEC_QG, STEC_CG,
                    CHCK,CHCK_CH, CHCK_MC, CHCK_MS, CHCK_WS, CHCK_MF, CHCK_MQ, CHCK_MD, CHCK_WD,
                    QUDS, QUDS_CASE,
                    QDCV, QDCV_GO, QDCV_TH, QDCV_QN, QDCV_DS, QDCV_SH, QDCV_SQ, QDCV_QM, QDCV_QM2,
                    QDCV_QW, QDCV_QW2, QDCV_QP, QDCV_QP2, QDCV_RM, QDCV_RW, QDCV_RP,
                    MODE, MODE_MD, MODE_MA
                   };
enum class MEMBERCATEGORY{JOINT, COLUMN, GIRDER, BRACE, WALL, ZWALL, SLAB, ISOLATOR, DAMPER, ALL};



class POST3D_CLASS_DLLSPEC LineCondition : public AbstractLineData
{
public:
    explicit LineCondition(
            const QString& _key = QString(),
            ANALYSISSTEP _myStep = ANALYSISSTEP::STEP_NONE,
            LINESTATUSTYPE _myType = LINESTATUSTYPE::NONE,
            const QList<TextDataUnit>& _units = QList<TextDataUnit>(),
            const QList<LINESTATUSTYPE>& _types = QList<LINESTATUSTYPE>(),
            int _secondKeyWordIndex = -1)
        :AbstractLineData(_key, _units), myStep(_myStep), myType(_myType), premiseTypes(_types), secondKeywordIndex(_secondKeyWordIndex){};
    virtual ~LineCondition(){};
    bool MatchKeyword(ANALYSISSTEP, LINESTATUSTYPE, const QString&);
    ANALYSISSTEP getStep() const{return myStep;}
    LINESTATUSTYPE getStatus() const{return myType;}
    virtual QString convert(const QString& txt, QSet<QString>* , QSet<QString>*, QString*) const{return txt;}

protected:
    const ANALYSISSTEP myStep;
    const LINESTATUSTYPE myType;
    QList<LINESTATUSTYPE> premiseTypes;
    int secondKeywordIndex; // SEISMICのようにもう一つkeywordが必要な場合に指定する。ConditionDataUnitの番号。
};

class TwoPointLineCondition : public LineCondition
{
public:
    TwoPointLineCondition(
            const QString& _key = QString(),
            ANALYSISSTEP _myStep = ANALYSISSTEP::STEP_NONE,
            LINESTATUSTYPE _myType = LINESTATUSTYPE::NONE,
            const QList<TextDataUnit>& _units = QList<TextDataUnit>(),
            const QList<LINESTATUSTYPE>& _types = QList<LINESTATUSTYPE>())
        :LineCondition(_key, _myStep, _myType, _units, _types){};

    QString convert(const QString&, QSet<QString>* , QSet<QString>*, QString*) const override;
};

class DkLineCondition : public LineCondition{
public:
    DkLineCondition(
            const QString& _key = QString(),
            ANALYSISSTEP _myStep = ANALYSISSTEP::STEP_NONE,
            LINESTATUSTYPE _myType = LINESTATUSTYPE::NONE,
            const QList<TextDataUnit>& _units = QList<TextDataUnit>(),
            const QList<LINESTATUSTYPE>& _types = QList<LINESTATUSTYPE>())
        :LineCondition(_key, _myStep, _myType, _units, _types){};

    QString convert(const QString&, QSet<QString>* , QSet<QString>*, QString*) const override;
};

class D3LineCondition : public LineCondition{
public:
    D3LineCondition(
            const QString& _key = QString(),
            ANALYSISSTEP _myStep = ANALYSISSTEP::STEP_NONE,
            LINESTATUSTYPE _myType = LINESTATUSTYPE::NONE,
            const QList<TextDataUnit>& _units = QList<TextDataUnit>(),
            const QList<LINESTATUSTYPE>& _types = QList<LINESTATUSTYPE>(),
            int _secondKeyWordIndex = -1)
        :LineCondition(_key, _myStep, _myType, _units, _types, _secondKeyWordIndex){};

    QString convert(const QString&, QSet<QString>* , QSet<QString>*, QString*) const override;
};

class D3LineCondition2 : public LineCondition{
public:
    D3LineCondition2(
            const QString& _key = QString(),
            ANALYSISSTEP _myStep = ANALYSISSTEP::STEP_NONE,
            LINESTATUSTYPE _myType = LINESTATUSTYPE::NONE,
            const QList<TextDataUnit>& _units = QList<TextDataUnit>(),
            const QList<LINESTATUSTYPE>& _types = QList<LINESTATUSTYPE>(),
            int _secondKeyWordIndex = -1);;

    QString convert(const QString&, QSet<QString>* , QSet<QString>*, QString*) const override;
};

class D3TLineCondition : public LineCondition{
public:
    D3TLineCondition(
            const QString& _key = QString(),
            ANALYSISSTEP _myStep = ANALYSISSTEP::STEP_NONE,
            LINESTATUSTYPE _myType = LINESTATUSTYPE::NONE,
            const QList<TextDataUnit>& _units = QList<TextDataUnit>(),
            const QList<LINESTATUSTYPE>& _types = QList<LINESTATUSTYPE>(),
            int _secondKeyWordIndex = -1)
        :LineCondition(_key, _myStep, _myType, _units, _types, _secondKeyWordIndex){};

    QString convert(const QString&, QSet<QString>* , QSet<QString>*, QString*) const override;
};

class MaLineCondition : public LineCondition{
public:
    MaLineCondition(
            const QString& _key = QString(),
            ANALYSISSTEP _myStep = ANALYSISSTEP::STEP_NONE,
            LINESTATUSTYPE _myType = LINESTATUSTYPE::NONE,
            const QList<TextDataUnit>& _units = QList<TextDataUnit>(),
            const QList<LINESTATUSTYPE>& _types = QList<LINESTATUSTYPE>())
        :LineCondition(_key, _myStep, _myType, _units, _types){};
    QString convert(const QString&, QSet<QString>* , QSet<QString>*, QString*) const override;
};



} // namespace post3dapp



#endif // LINECONDITION_H

#ifndef DEFINESETTINGS_H
#define DEFINESETTINGS_H

#include <QBrush>
#include <QDebug>
#include <QHash>
#include <QUuid>
#include <QObject>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{

enum class STRUCTTYPE;

/* 操作モード　・・個別操作、全体編集、一括操作 */
enum OPERATIONMODE {OPELEMENT = 0, OPALLEDIT, OPALLCHANGE};

/* 要素タイプ　・・無し、形状節点、柱、大梁、ブレース、小梁、壁、スラブ、地下外壁、制振要素、免震要素、床荷重、部材荷重、節点荷重、全対象 */
enum ELEMENTTYPE {NOELEMENT = 0, ELJOINT, ELCOLUMN, ELGIRDER, ELBRACE,
                                       ELBEAM, ELSLAB, ELWALL, ELUWALL, ELDAMPER, ELISO,
                                       ELJOINTLOAD, ELMEMBERLOAD, ELSLABLOAD, ALLELEMENT, ELSPRING
                                      };

/* 要素グループ　・・無し、点タイプ、線タイプ、面タイプ、全タイプ */
enum ELEMENTGROUP {ELGNONE = 0, ELGPOINT, ELGLINE, ELGPLANE, ELGALL, ELGWALL};

/* 要素状態 ・・通常、ダミー、エラー、選択、入力補助 */
enum ELEMENTSTATE {ELNORMAL = 0, ELDUMMY, ELERROR, ELSELECT, ELINPUT};

/* 要素面内外 ・・面内、直交、下階 */
enum class ELEMENTDIR {ELINPLANE = 0, ELCROSS, ELDOWN};

/* 要素の階・通り所属 ・・指定なし、全所属、部分所属  */
enum ELEMENTLIMITED {ELUNLIMITED = 0, ELCONTAIN, ELTOUCH};

/* 表示モデルタイプ ・・形状伏軸、形状３Ｄ、解析伏軸、解析３Ｄ、グラフ */
enum MODELVIEWTYPE {INPUT2DMODEL = 0, INPUT3DMODEL, CALC2DMODEL, CALC3DMODEL, OUTPUT2DMODEL, OUTPUT3DMODEL,
                   INPUT2DMODELCOPY, INPUT3DMODELCOPY, OUTPUTGRAPH};

/* 追加・編集　・・端点指定入力、１点ヒット入力、一括入力、編集、等分割入力、平行入力、垂直入力、延長入力 */
enum ADDEDITMODE {DISABLEMODE = 0, POINTHITMODE, ONEHITMODE, BATCHMODE, SELECTHITMODE, EDITMODE,
                                       DIVIDEMODE, PARALLELMODE, VERTICALMODE, EXTENDMODE
                                      };

/* 追加（移動）節点が他部材上　・・他部材の内分点、他部材の構成点、単独節点 */
enum ADDMOVEACTION {AMINTER = 0, AMCOMPOSE, AMSELF};

/* 既存節点が追加（移動）部材上　・・追加（移動）部材の内分点、単独節点 */
enum ADDMOVEDACTION {AMDINTER = 0, AMDSELF};

/* 部材削除時の節点　・・節点削除、節点を残す */
enum DELETEACTION {DELETEJOINT = 0, REMAINJOINT};

/* 節点の一括入力　・・通りと通りの交点、グリッド点、通りの定義点 */
enum JOINTBATCHACTION {SPCROSS = 0, SPDEFINE, SPGRID};

/* 領域認識　・・大梁・柱で囲まれる領域、小梁・大梁・柱で囲まれる領域 */
enum TARGETACTION {TARGETCG = 0, TARGETCGB};

/* マウス入力時の候補点 */
struct POST3D_CLASS_DLLSPEC SNAPSTATE {
    explicit SNAPSTATE(
        bool check1 = false,
        int value1 = 1000,
        bool check2 = true,
        bool check3 = true,
        bool check4 = false,
        bool check5 = false,
        int value2 = 2,
        bool check6 = false,
        int value3 = 1500,
        int value4 = 1500,
        int _offset = 0
            )
        : gridIsChecked(check1), gridPitch(value1), crossIsChecked(check2),
          defineIsChecked(check3), divideIsChecked(check4),
          divideEquallyIsChecked(check5), divideParts(value2),
          divideFromIJIsChecked(check6), divideFromI(value3), divideFromJ(value4),crossOffset(_offset) { }

    bool gridIsChecked;
    int gridPitch;
    bool crossIsChecked;
    bool defineIsChecked;
    bool divideIsChecked;
    bool divideEquallyIsChecked;
    int divideParts;
    bool divideFromIJIsChecked;
    int divideFromI;
    int divideFromJ;
    int crossOffset;
};

/* オペレーション設定 */

struct POST3D_CLASS_DLLSPEC OPERATIONSETTING {
    explicit OPERATIONSETTING(
        OPERATIONMODE _mode = OPELEMENT,
        ELEMENTTYPE _etype = ELJOINT,
        ADDEDITMODE _addedit = POINTHITMODE,
        SNAPSTATE _snap = SNAPSTATE(),
        ADDMOVEACTION _ac1 = AMINTER,
        ADDMOVEDACTION _ac2 = AMDINTER,
        DELETEACTION _acd = DELETEJOINT,
        JOINTBATCHACTION _acj = SPCROSS,
        TARGETACTION _act = TARGETCG,
        int _vbd = 2,
        int _vbp = 3000,
        int _vbv = 3000,
        int _vbe = 3000,
        int _vjs = 1000 )
        : operationMode(_mode), elementType(_etype), addEditMode(_addedit),
          snapState(_snap), addMoveAction(_ac1), addMovedAction(_ac2),
          deleteAction(_acd), jointBatchAction(_acj), targetAction(_act),
          beamDivSpinValue(_vbd), beamPalSpinValue(_vbp), beamVerSpinValue(_vbv),
          beamExtSpinValue(_vbe), jointBatchSpinValue(_vjs) { }

    OPERATIONMODE operationMode;
    ELEMENTTYPE elementType;
    ADDEDITMODE addEditMode;

    SNAPSTATE snapState;
    ADDMOVEACTION addMoveAction;
    ADDMOVEDACTION addMovedAction;
    DELETEACTION deleteAction;
    JOINTBATCHACTION jointBatchAction;
    TARGETACTION targetAction;

    int beamDivSpinValue;
    int beamPalSpinValue;
    int beamVerSpinValue;
    int beamExtSpinValue;
    int jointBatchSpinValue;
};

/* 要素別の表示設定 */
struct POST3D_CLASS_DLLSPEC ELEMENTVIEW {
    explicit ELEMENTVIEW(
        bool _visible = true,
        bool _namevisible = true,
        const QColor &_color = Qt::black,
        const QBrush &_brush = Qt::NoBrush )
        : isVisible(_visible), isNameVisible(_namevisible),
          lineColor(_color), paintBrush(_brush) { }
    explicit ELEMENTVIEW(
        bool _visible,
        bool _nameVisible,
        const QColor &_lineColor,
        const QColor &_brushColor,
        int alpha)
        : isVisible(_visible),
          isNameVisible(_nameVisible),
          lineColor(_lineColor),
          paintBrush(QColor{_brushColor.red(), _brushColor.green(), _brushColor.blue(), alpha}){}
    bool isVisible;
    bool isNameVisible;
    QColor lineColor;
    QBrush paintBrush;
};

/* ELEMENTSTATE に応じた設定 */
struct POST3D_CLASS_DLLSPEC COLORBRUSHSETTING {
    explicit COLORBRUSHSETTING(
        const QColor &_color = Qt::black,
        const QBrush &_brush = Qt::NoBrush )
        : lineColor(_color), paintBrush(_brush) { }
    explicit COLORBRUSHSETTING(const QColor &_lineColor, const QColor &_brushColor, int _alpha)
        :lineColor(_lineColor), paintBrush(QColor{_brushColor.red(), _brushColor.green(), _brushColor.blue(), _alpha}){}
    QColor lineColor;
    QBrush paintBrush;
};

/* ELDIR::DOWN に応じた設定 */
struct POST3D_CLASS_DLLSPEC DOWNSETTING {
    explicit DOWNSETTING(
        bool _isNameVisible = true,
        const QColor &_color = Qt::black,
        const QBrush &_brush = Qt::NoBrush)
        : isNameVisible(_isNameVisible), lineColor(_color), paintBrush(_brush) { }
    explicit DOWNSETTING(
        bool _isNameVisible,
        const QColor &_lineColor,
        const QColor &_brushColor,
        int alpha)
        : isNameVisible(_isNameVisible),
          lineColor(_lineColor),
          paintBrush(QColor{_brushColor.red(), _brushColor.green(), _brushColor.blue(), alpha}) { }
    bool isNameVisible;
    QColor lineColor;
    QBrush paintBrush;
};

} // namespace post3dapp

#endif

#ifndef DEFINE_LINESHAPE_H
#define DEFINE_LINESHAPE_H

#include <QPainterPath>

namespace post3dapp{

enum class RENDERSHAPE { SHAPE_BOX = 0, SHAPE_H, SHAPE_PIPE, SHAPE_I,
                   SHAPE_T1, SHAPE_T2, SHAPE_T3, SHAPE_T4, SHAPE_C1, SHAPE_C2, SHAPE_C3, SHAPE_C4, SHAPE_L , SHAPE_CROSS,SHAPE_CROSSH
                 };       // 描画用断面タイプ（ボックス、Ｈ型、円形、Ｔ型、チャンネル、アングル）

struct VLINESHAPE {  // 部材断面（直交部材描画用）
    explicit VLINESHAPE(
        const QString &_sname = QString(),
        RENDERSHAPE _type = RENDERSHAPE::SHAPE_BOX,
        qreal _sizeB = 0.0,
        qreal _sizeH = 0.0,
        qreal _angle = 0.0,
        qreal _shifty = 0.0,
        qreal _shiftz = 0.0,
        const QString &_bname = QString(),
        qreal _bsize1 = 0.0,
        qreal _bsize2 = 0.0 ,
            qreal _sizeBT = 0.0,
            qreal _sizeHT = 0.0
            )
        : sectionName(_sname), renderType(_type),
          sizeB(_sizeB), sizeH(_sizeH),
          angle(_angle), shiftY(_shifty), shiftZ(_shiftz),
          baseName(_bname), baseSizeB(_bsize1), baseSizeD(_bsize2),sizeB_T(_sizeBT),sizeH_T(_sizeHT) { }

    QString sectionName;            // 断面符号
    RENDERSHAPE renderType;         // 断面タイプ
    qreal sizeB;                    //　幅(mm)
    qreal sizeH;                    //　高さ(mm)
    qreal sizeB_T;                    //　幅(mm)
    qreal sizeH_T;                    //　高さ(mm)
    qreal angle;                    // コードアングル (rad)
    qreal shiftY;                   // 部材の寄り　y方向(mm)
    qreal shiftZ;                   // 部材の寄り　z方向(mm)

    QString baseName;               // 柱脚名
    qreal baseSizeB;                // 柱脚　幅(mm)
    qreal baseSizeD;                // 柱脚 高さ(mm)

    qreal getGirderElevationOffset(bool) const;
    void drawIsoLineShape(QPainterPath&) const;
    void drawDamperLineShape(QPainterPath&) const;
    void drawColumnPlanLineShape(QPainterPath&, bool) const;
    void drawColumnElevationLineShape(QPainterPath&, bool, qreal, qreal) const;
    void drawGirderPlanLineShape(QPainterPath&, bool, qreal, qreal, qreal) const;
    void drawGirderElevationLineShape(QPainterPath&, bool, qreal, qreal) const;

    void drawColumnPlanBoundingShape(QPainterPath&, bool) const;
    void drawColumnElevationBoundingShape(QPainterPath&, bool, qreal, qreal) const;
    void drawGirderPlanBoundingShape(QPainterPath&, bool, qreal, qreal, qreal) const;
    void drawGirderElevationBoundingShape(QPainterPath&, bool, qreal, qreal) const;

    void drawColumnPlanBaseShape(QPainterPath&, bool) const;
    void drawColumnElevationBaseShape(QPainterPath&, bool, qreal, qreal) const;
private:
    void drawLineShape(QPainterPath&, qreal, qreal, qreal) const;
    void drawBaseShape(QPainterPath&, qreal, qreal, qreal) const;
    void addRectangleShape(QPainterPath&, qreal, qreal, qreal) const;
    void addIShape(QPainterPath&, qreal, qreal, qreal, qreal rotate = 0.0) const;
    void addHShape(QPainterPath&, qreal, qreal, qreal) const;
    void addPipeShape(QPainterPath&, qreal, qreal) const;
    void addTShape(QPainterPath&, qreal, qreal, qreal, qreal rotate = 0.0) const;
    void addCShape(QPainterPath&, qreal, qreal, qreal, qreal rotate = 0.0) const;
    void addLShape(QPainterPath&, qreal, qreal, qreal, qreal rotate = 0.0) const;
    void addCrossPlateShape(QPainterPath&, qreal, qreal, qreal) const;
    void addCrossHShape(QPainterPath&, qreal, qreal, qreal)const;

    void addT1Shape(QPainterPath&, qreal, qreal, qreal) const;
    void addT2Shape(QPainterPath&, qreal, qreal, qreal) const;
    void addT3Shape(QPainterPath&, qreal, qreal, qreal) const;
    void addT4Shape(QPainterPath&, qreal, qreal, qreal) const;
    void addC1Shape(QPainterPath&, qreal, qreal, qreal) const;
    void addC2Shape(QPainterPath&, qreal, qreal, qreal) const;
    void addC3Shape(QPainterPath&, qreal, qreal, qreal) const;
    void addC4Shape(QPainterPath&, qreal, qreal, qreal) const;
};

struct HLINESHAPE {
    explicit HLINESHAPE(
        const QString &_sname = QString(),
        RENDERSHAPE _type = RENDERSHAPE::SHAPE_BOX,
        qreal _size1 = 0.0,
        qreal _size2 = 0.0,
        qreal _size3 = 0.0,
        qreal _size4 = 0.0,
        qreal _size5 = 0.0,
        qreal _size6 = 0.0,
        qreal _angle = 0.0,
        qreal _shifty = 0.0,
        qreal _shiftz = 0.0,
        const QString &_bname = QString(),
        qreal _bsize1 = 0.0,
        qreal _bsize2 = 0.0,
        qreal _bsize3 = 0.0 ,
            qreal _size11 = 0.0,
            qreal _size12 = 0.0,
            qreal _size13 = 0.0,
            qreal _size14 = 0.0,
            qreal _size15 = 0.0,
            qreal _size16 = 0.0
            )
        : sectionName(_sname), renderType(_type),
          leftSizeB(_size1), leftSizeH(_size2),
          centerSizeB(_size3), centerSizeH(_size4),
          rightSizeB(_size5), rightSizeH(_size6),
          angle(_angle), shiftY(_shifty), shiftZ(_shiftz),
          baseName(_bname), baseSizeB(_bsize1), baseSizeD(_bsize2), baseSizeT(_bsize3),
          leftSizeB_T(_size11), leftSizeH_T(_size12),
          centerSizeB_T(_size13), centerSizeH_T(_size14),
          rightSizeB_T(_size15), rightSizeH_T(_size16)
    { }

    QString sectionName;        // 断面符号
    RENDERSHAPE renderType;     // 断面タイプ
    qreal leftSizeB;            //　左端　幅(mm)
    qreal leftSizeH;            // 左端　高さ(mm)
    qreal centerSizeB;          // 中央 幅(mm)
    qreal centerSizeH;          // 中央 高さ(mm)
    qreal rightSizeB;           // 右端 幅(mm)
    qreal rightSizeH;           // 右端 高さ(mm)
    qreal leftSizeB_T;            //　左端　幅(mm)
    qreal leftSizeH_T;            // 左端　高さ(mm)
    qreal centerSizeB_T;          // 中央 幅(mm)
    qreal centerSizeH_T;          // 中央 高さ(mm)
    qreal rightSizeB_T;           // 右端 幅(mm)
    qreal rightSizeH_T;           // 右端 高さ(mm)

    qreal angle;                // コードアングル (rad)
    qreal shiftY;               // 部材の寄り　y方向(mm)
    qreal shiftZ;               // 部材の寄り　z方向(mm)

    QString baseName;           // 柱脚名
    qreal baseSizeB;            // 柱脚　幅(mm)
    qreal baseSizeD;            // 柱脚 高さ(mm)
    qreal baseSizeT;            // 柱脚 厚み(mm)
};
} // namespace post3dapp

#endif // DEFINE_LINESHAPE_H

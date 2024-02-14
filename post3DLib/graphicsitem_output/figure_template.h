#ifndef FIGURE_TEMPLATE_H
#define FIGURE_TEMPLATE_H

#include "define_draw_figure.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif


namespace post3dapp{
enum class ATTENTION_STATUS;
class JointData;
class MemberData;
class OutputGraphicsItem;
class OutputGrScene;
class SpringData;

class POST3D_CLASS_DLLSPEC FigureTemplate : public QObject
{
    Q_OBJECT
public:
    bool useScaledFont();
    OUTFIGTYPE getOutFigType() const {return option.type;}
    FIGURE_TYPE_PROPERTY getFigureTypeProperty() const{return option;}
    QUuid getId()const{return uuid;}
    void addItems(OutputGrScene*);

    virtual void createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp) = 0;
    virtual void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>) = 0;
    virtual void createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection ) = 0;
    virtual void createSpringItem(OutputGrScene *scene, SpringData* sd,QList<QPointF> points );

    virtual QColor valueTextColor(DATATYPE,ATTENTION_STATUS) const = 0;
    virtual QColor figureColor(DATATYPE, STRUCTTYPE material, ATTENTION_STATUS) const = 0;
    virtual QColor selectColor(DATATYPE) const = 0;
    virtual bool isLineDirVisible() const = 0;
    virtual int nameTextSize(DATATYPE) const = 0;
    virtual int valueTextSize(DATATYPE) const = 0;
    virtual bool nameVisible(DATATYPE) const = 0;
    virtual bool figureVisible(DATATYPE) const = 0;
    virtual bool valueVisible(DATATYPE) const = 0;

    virtual QColor scaleColor() const = 0;
    virtual QColor supportColor() const = 0;
    virtual int jointSize() const = 0;
    virtual QColor noteColor() const = 0;
    virtual int noteSize() const = 0;
    virtual QColor plasticSymbolColor() const = 0;
    virtual int plasticSymbolSize() const = 0;
    virtual int scaleTextSize() const = 0;
    virtual bool scaleFloorTextVisible() const = 0;
    virtual bool scaleGridTextVisible() const = 0;

    QString getMemberInfo(const QUuid &id) const;
    QString getJointInfo(const QUuid &id) const;
    QString getSpringInfo(const QUuid &id) const;

protected:
    FigureTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    virtual AnalysisBaseViewSettings* getViewSettings() const = 0;

    virtual void preCreateItems(OutputGrScene*){};
    virtual void createJointItems(OutputGrScene*) = 0;
    virtual void createMemberItems(OutputGrScene*) = 0;
    virtual void createSpringItems(OutputGrScene*);
    virtual void createPointItems(OutputGrScene*){};
    virtual void createTextItems(OutputGrScene*){};
    virtual void postCreateItems(OutputGrScene*){};


    QPointF jointToPoint(JointData *, qreal nx = 0.0) const;
    OutputGraphicsItem *createOffJointItem(const JointData&, const QPointF &);

    const Analysis2DViewSettings setting;
    const FIGURE_TYPE_PROPERTY option;
    const QUuid uuid; // floor or frame id
};


class POST3D_CLASS_DLLSPEC FigureCommonTemplate : public FigureTemplate
{
    Q_OBJECT

public:
    FigureCommonTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);

    virtual QColor valueTextColor(DATATYPE,ATTENTION_STATUS) const override;
    virtual QColor figureColor(DATATYPE, STRUCTTYPE material, ATTENTION_STATUS) const override;
    QColor selectColor(DATATYPE) const final;
    bool isLineDirVisible() const final;
    int nameTextSize(DATATYPE) const final;
    int valueTextSize(DATATYPE) const final;
    bool nameVisible(DATATYPE) const final;
    virtual bool figureVisible(DATATYPE) const override;
    bool valueVisible(DATATYPE) const final;

    QColor scaleColor() const final;
//    virtual QColor springColor();
    QColor supportColor() const final;
    int jointSize() const final;
    QColor noteColor() const final;
    int noteSize() const final;
    QColor plasticSymbolColor() const final;
    int plasticSymbolSize() const final;
    int scaleTextSize() const final;
    bool scaleFloorTextVisible() const final;
    bool scaleGridTextVisible() const final;

    //memo : viewsettingwidgetの更新時に、Sceneのslotが受けてこの関数を呼び出す。Sceneのインスタンス作成時にWidgetにコネクト

protected:
    virtual void createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp) override;
    virtual void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>) override;
    virtual void createPlaneItem(OutputGrScene*, MemberData *, QList<QPointF>,bool) override;

    virtual void createLineSectionItem(OutputGrScene *scene, MemberData* md,const QPointF &point );

    virtual ATTENTION_STATUS getAttentionStatus(qreal) const;
    virtual AnalysisCommon2DViewSetting* getViewSettings() const override{return static_cast<AnalysisCommon2DViewSetting*>(setting.getSetting(option.type));}

//    VEC3D planeXVector() const{return myPlaneXVector;}
//    VEC3D planeVector() const{return myPlaneVector;}


//    void createSpringItems(OutputGrScene*) final;

};
} // namespace post3dapp
#endif // FIGURE_TEMPLATE_H

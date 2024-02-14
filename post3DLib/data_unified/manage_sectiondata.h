#ifndef MANAGESECTIONDATA_H
#define MANAGESECTIONDATA_H

#include "define_section_struct.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class FloorOrderData;
class MultiSectionData;
class UnifiedData;
class N3DUuidTable;

class POST3D_CLASS_DLLSPEC ManageSectionData : public QObject
{
    Q_OBJECT

public:

    ManageSectionData(UnifiedDataType isout, QObject *parent = nullptr);
    ManageSectionData(const ManageSectionData&, UnifiedDataType isout, QObject *parent = nullptr);

    int dataCount(DATATYPE type) const;
    QStringList getNameList(DATATYPE type) const;
    bool isExistedName(const QString &, DATATYPE) const;

    bool checkColumnData(QWidget *, const QList<SectionValues> &);
    bool checkGirderData(QWidget *, const QList<SectionValues> &);
    bool checkBraceData(QWidget *, const QList<SectionValues> &);
    bool checkBeamData(QWidget *, const QList<SectionValues> &);
    bool checkSlabData(QWidget *, const QList<SectionValues> &);
    bool checkWallData(QWidget *, const QList<SectionValues> &);
    bool checkUWallData(QWidget *, const QList<SectionValues> &);
    bool checkOpenData(QWidget *, const QList<SectionValues> &);
    bool checkBaseData(QWidget *, const QList<SectionValues> &);
    bool checkDamperData(QWidget *, const QList<SectionValues> &);
    bool checkIsoData(QWidget *, const QList<SectionValues> &);

    void registerColumnData();
    void registerGirderData();
    void registerBraceData();
    void registerBeamData();
    void registerSlabData();
    void registerWallData();
    void registerUWallData();
    void registerBaseData();
    void registerOpenData();
    void registerDamperData();
    void registerIsoData();

    void registerTempData(DATATYPE, const QList<SectionValues> &);
    void replaceSectionName(DATATYPE, const QString &, const QString &);

    void clearData();
    void clearTempData()
    {
        cash_SecList.clear();
    }

    LINESHAPE_IN sectionNameToLineShape(const UnifiedData&, DATATYPE, const QString &, qreal hh = 0.0) const;
    LINEPROPERTY sectionNameToLineProperty(const UnifiedData&, DATATYPE, const QString &, qreal hh = 0.0) const;
    PLANEPROPERTY sectionNameToPlaneProperty(const UnifiedData&, DATATYPE, const QString &, qreal hh = 0.0) const;

    SectionValues sectionNameToValue(const UnifiedData&, DATATYPE, const QString &, qreal hh = 0.0) const;
    SECTIONWEIGHT sectionNameToUnitWeight(const UnifiedData&, DATATYPE, const QString &, qreal hh = 0.0) const;
    STRUCTTYPE sectionNameToStructureType(const UnifiedData&, DATATYPE, const QString &, qreal hh = 0.0) const;

    BASEVALUES getBaseValues(const QString &) const;
    QList<WallOpenParameter> getWallOpenValues(const QString &) const;

    qreal getSlabLoadDir(const QString &) const;
    bool isStructualSection(const UnifiedData&, DATATYPE, const QString &, qreal hh = 0.0) const;
    QList<SectionValues> getSectionValues(DATATYPE)const;

    QString writePsvData(QSharedPointer<N3DUuidTable>) const;
    QString writeNsvData(QSharedPointer<N3DUuidTable>) const;

public slots:

signals:

    void sectionDeleted(DATATYPE, const QStringList &);
    void sectionChanged(DATATYPE, const QStringList &);

private:

    /* 部材符号が複数回定義される（階範囲指定がある）部材　・・・　柱、梁 */

    QHash<QString, FloorOrderData *> columnTable;
    QHash<QString, FloorOrderData *> girderTable;
    QHash<QString, FloorOrderData *> wallTable;

    QHash<QString, SectionValues> braceTable;
    QHash<QString, SectionValues> beamTable;
    QHash<QString, SectionValues> slabTable;
    QHash<QString, SectionValues> uwallTable;
    QHash<QString, SectionValues> baseTable;
    QHash<QString, SectionValues> damperTable;
    QHash<QString, SectionValues> isoTable;

    QHash<QString, MultiSectionData *> openTable;

    /* 定義順を保持　*/

    QStringList columnNameList;
    QStringList girderNameList;
    QStringList braceNameList;
    QStringList beamNameList;
    QStringList slabNameList;
    QStringList wallNameList;
    QStringList uwallNameList;
    QStringList openNameList;
    QStringList baseNameList;
    QStringList damperNameList;
    QStringList isoNameList;

    /* キャッシュ */

    QList<SectionValues> cash_SecList;

    bool compareSectionData( const SectionValues &, const SectionValues &, DATATYPE ) const;
    bool compareLineCommonParameter( const LineCommonParameter &, const LineCommonParameter & ) const;
    bool compareLinePartsParameter( const LinePartsParameter &, const LinePartsParameter & ) const;
    bool comparePlaneParameter( const PlaneSectionParameter &, const PlaneSectionParameter & ) const;
    bool compareBaseParameter( const BaseSectionParameter &, const BaseSectionParameter & ) const;
    bool compareWallOpenParameter( const WallOpenParameter &, const WallOpenParameter & ) const;
    bool compareDamperParameter( const DamperParameter &, const DamperParameter & ) const;
    bool compareIsoParameter( const IsolatorParameter &, const IsolatorParameter & ) const;
    bool compareSpringParameter( const SpringParameter &, const SpringParameter & ) const;

    LINESHAPE_IN judgeRenderShape( DATATYPE, const SectionValues & ) const;
    LINEPROPERTY judgeLineValue( const SectionValues & ) const;

    SECTIONWEIGHT judgeLineWeight( const UnifiedData&, const SectionValues & ) const; // kN/m
    SECTIONWEIGHT judgePlaneWeight( const UnifiedData&, const SectionValues & ) const;  // kN/m^2
    qreal calcArea_S(const LinePartsParameterSRC &) const;
    qreal calcArea_RC(ConcreteType::Type, qreal, qreal) const;

    RENDERSHAPE sectionTypeToSteelRenderShape( SteelType::Type ) const;
    RENDERSHAPE sectionTypeToConcRenderShape( ConcreteType::Type ) const;

    void errorUndefinedName(QWidget *parent, const QStringList &names);
    void errorOverlapNames(QWidget *parent, const QStringList &names);

    const UnifiedDataType isOutput;
    QString writeData(QSharedPointer<N3DUuidTable>, bool) const;
    QString writeColumnData(QSharedPointer<N3DUuidTable>) const;
    QString writeGirderData(QSharedPointer<N3DUuidTable>) const;
    QString writeBraceData(QSharedPointer<N3DUuidTable>) const;
    QString writeBeamData(QSharedPointer<N3DUuidTable>) const;
    QString writeSlabData(QSharedPointer<N3DUuidTable>, bool) const;
    QString writeWallData(QSharedPointer<N3DUuidTable>, bool) const;
    QString writeBasePlateData(QSharedPointer<N3DUuidTable>) const;
    QString writeWallOpenData(QSharedPointer<N3DUuidTable>, bool) const;
    QString writeDamperData(QSharedPointer<N3DUuidTable>) const;
    QString writeIsolatorData(QSharedPointer<N3DUuidTable>) const;
};


/* 階別リスト */
class POST3D_CLASS_DLLSPEC FloorOrderData
{

public:

    explicit FloorOrderData( UnifiedDataType isout,const QString &name = QString() )
        : isOutput(isout), mySectionName(name) {}

    void appendValue(const SectionValues &value)
    {
        mySectionList.append(value);
    }
    QString sectionName() const
    {
        return mySectionName;
    }
    int listCount() const
    {
        return mySectionList.count();
    }
    QList<SectionValues> getSectionValues() const
    {
        return mySectionList;
    }

    SectionValues getValueOfLimitedFloors( const QUuid &f1, const QUuid &f2 ) const;
    SectionValues getSectionValueOfHeight(const UnifiedData&, qreal) const;
    void updateFloorID();

    void changeSectionName( const QString &str );
    void replaceBaseName( const QString &, const QString & );

private:

    const UnifiedDataType isOutput;
    QString mySectionName;
    QList<SectionValues> mySectionList;

};

/* 複合リスト */
class POST3D_CLASS_DLLSPEC MultiSectionData
{

public:

    explicit MultiSectionData( const QString &name = QString() )
        : mySectionName(name) { }

    void appendValue(const SectionValues &value)
    {
        mySectionList.append(value);
    }
    QString sectionName() const
    {
        return mySectionName;
    }
    int listCount() const
    {
        return mySectionList.count();
    }
    QList<SectionValues> getSetionValues() const
    {
        return mySectionList;
    }

    void changeSectionName( const QString &str );

private:

    QString mySectionName;
    QList<SectionValues> mySectionList;

};
} // namespace post3dapp
#endif

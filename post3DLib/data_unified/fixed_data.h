#ifndef FIXEDDATA_H
#define FIXEDDATA_H

#include <QObject>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class UnifiedAnalysisData;
class UnifiedCalcLoad;
class UnifiedInputData;
class UnifiedOutput;
class N3DUuidTable;

class POST3D_CLASS_DLLSPEC FixedData : public QObject
{
    Q_OBJECT
public:
    static FixedData* CreateDummy();
    static FixedData* Create();
    UnifiedInputData* getInputData() const {return inputData;}
    UnifiedAnalysisData* getAnalysisData() const{return analysisData;}
    UnifiedOutput* getPstnData() const{return pstnData;}
    UnifiedCalcLoad* getCalcLoadData() const{return calcLoadData;}
    QString getTime() const{return time;}
    QSharedPointer<N3DUuidTable> getUuidTable() const{return table;}

    // GUIを経由するか経由しないかでOutputTextをかき分ける
    virtual QStringList getGirderRcList() const;
    virtual QStringList getGirderSList() const;
    virtual QStringList getGirderSrcList() const;
    virtual QStringList getColumnRcList() const;
    virtual QStringList getColumnSList() const;
    virtual QStringList getColumnSrcList() const;
    virtual QStringList getColumnCftList() const;
    virtual QStringList getBeamRcList() const;
    virtual QStringList getBeamSList() const;
    virtual QStringList getBeamSrcList() const;
    virtual QStringList getBraceList() const;
    virtual QStringList getSlabList() const;
    virtual QStringList getWallList() const;
    virtual QStringList getFoundationWallList() const;
    virtual QStringList getSteelWallList() const;
    virtual QStringList getOpenList() const;
    virtual QStringList getBaseSList() const;
    virtual QStringList getBaseSrcList() const;
    virtual QStringList getDamperList() const;
    virtual QStringList getIsoList() const;
    virtual QStringList getJointLoadList() const;
    virtual QStringList getMemberLoadList() const;
    virtual QStringList getSlabLoadList() const;

    /* UUidTable */
    QString getAnalysisJointName(const QUuid&) const;
    QString getAnalysisMemberName(const QUuid&, int) const;


protected:
    FixedData(bool, const QString&);
    const QString time;
    UnifiedInputData* inputData;
    UnifiedAnalysisData* analysisData;
    UnifiedOutput* pstnData;
    UnifiedCalcLoad* calcLoadData;
    QSharedPointer<N3DUuidTable> table;

};

class POST3D_CLASS_DLLSPEC UnifiedFixedData : public QObject
{
    Q_OBJECT
public:
    static FixedData* getInstance(){
        if (myInstance == nullptr) myInstance = FixedData::CreateDummy();
        return myInstance;
    }

    static void setInstance(FixedData* data){
        myInstance = data;
    }

private:
    static FixedData* myInstance;
};
} // namespace post3dapp

#endif // FIXEDDATA_H

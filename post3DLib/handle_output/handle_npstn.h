#ifndef HANDLE_NPSTN_H
#define HANDLE_NPSTN_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHash>
#include <QProcess>
#include <QProgressDialog>
#include <QWidget>

#include "define_npstn.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
enum class B3D_DATATYPE;
struct OpeningSet;
struct FRAMEGROUPVALUE;
struct FRAMEPOINTSVALUE;

enum  OUTPUT_FILE_TYPE{OFILE_MODEL=0,OFILE_RST,OFILE_LOAD,OFILE_CHCK,OFILE_PREA,OFILE_QUDS,OFILE_RSTQD,OFILE_MODE, OFILE_GENE, OFILE_STEC,OFILE_NMIN};
enum class READING_STATUS{STATUS_ERROR=-1,STATUS_NOTEXIST,STATUS_PARTLYSUCCESS,STATUS_SUCCESS};

class POST3D_CLASS_DLLSPEC handleNPstn : public QObject
{
    Q_OBJECT

public:
    static handleNPstn* CreateWithDialog(int *errorCode);
    static handleNPstn* CreateWithoutDialog(const QString&, int *errorCode);
    //描画に不要な配列は読み飛ばしてるので必要に応じて定義
    QString getFileName() const
    {
        return QFileInfo(filename).absoluteFilePath();
    }

    QStringList getStressCase() const
    {
        return stresscase;
    }

    // enum classをQHashのkeyに利用する場合、operator==とqHashを定義する必要がある
    //フラグは整理したい。ModeやChck、RstQDファイルがないときもある。モデルは必須、など。これはUnifiedOutputに保存すべきステータス？
    QHash<OUTPUT_FILE_TYPE,READING_STATUS> pstnReadStatus;
    bool existModeFile;

private:
    static constexpr int head = 4;
    static constexpr int foot = 4;
    static constexpr int new_line = head + foot;
    static constexpr int integer = 4;
    static constexpr int double_precision = 8;
    static constexpr int single = 4;

    handleNPstn(QObject *parent = nullptr): QObject(parent)
    {
        ver1_17 = QDateTime(QDate(2018, 1, 10)); //building3D ver 1.17のリリース日
    }

    int read(const QString&);
    READING_STATUS readBLD3D_CMDL(DataNBLD3D_CMDL&) const;
    READING_STATUS readRST(QHash<QString, DataNBLD3D_RST>& rst, QStringList& stresscase, const DataNBLD3D_CMDL&) const;
    QPair<READING_STATUS, DataNBLD3D_RST> readOneRst(const QString& rstname, const DataNBLD3D_CMDL& model) const; // rstの読み込みに一番時間がかかるので、その中の繰返しを並列化するため
    READING_STATUS readQUDS(DataNBLD3D_QUDS& quds) const;
    READING_STATUS readRSTQD(QHash<QString, DataNBLD3D_RSTQD>& rstqd, QStringList& qdlist, const DataNBLD3D_CMDL&) const;
    READING_STATUS readBLD3D_LOAD(DataNBLD3D_LOAD& load, const DataNBLD3D_CMDL &model, const QHash<QString, DataNBLD3D_PREA>& prea) const;
    READING_STATUS readBLD3D_CHCK(DataNBLD3D_CHCK& chck, const DataNBLD3D_CMDL &model) const;
    READING_STATUS readBLD3D_MODE(DataNBLD3D_MODE& mode) const;
    READING_STATUS readBLD3D_PREA(QHash<QString, DataNBLD3D_PREA>& prea, QStringList& modelcase, const DataNBLD3D_CMDL &model) const;
    READING_STATUS readBLD3D_GENE(DataNBLD3D_GENE& gene) const;
    READING_STATUS readBLD3D_STEC(DataNBLD3D_STEC& stec, const DataNBLD3D_CMDL& model) const;
    READING_STATUS readBLD3D_NMIN(DataNBLD3D_NMIN& nmin, const DataNBLD3D_CMDL& model) const;

    void UnzipPstn(const QString&, const QString&) const;



    DataNBLD3D_CMDL model;
    DataNBLD3D_LOAD load;
    QHash<QString, DataNBLD3D_RST> rst;
    QHash<QString, DataNBLD3D_RSTQD> rstqd;
    QHash<QString, DataNBLD3D_PREA> prea;
    DataNBLD3D_MODE mode;
    DataNBLD3D_CHCK chck;
    DataNBLD3D_QUDS quds;
    DataNBLD3D_GENE gene;
    DataNBLD3D_STEC stec;
    DataNBLD3D_NMIN nmin;
    QList<OpeningSet *> openingList;
    void sleep(unsigned long) const;
    //現状未使用。バージョンで処理を分ける場合など想定
    QDateTime ver1_17;
    QDateTime timestamp;

    bool toDecompressPstn;
    bool readAllSteps;
    QDir filepath() const; //pstnのあるフォルダまでのパス
    QDir tmpdirpath() const; //filepath/tmpdirnameまでのフルパス
    QString filename; //パスから拡張子まで含めたpstnの名前
    QString apppath; //実行ファイルの場所（7za.exeを入れておく）
    QString tmpdirname() const; //pstnの名前（パス、拡張子除く）
    QStringList stresscase;
    QStringList qdlist;
    QStringList modelcase;


    QHash<int, QUuid> floorNoToUuid;
    QHash<int, QUuid> frameGroupNoToUuid;
    QHash<int, QUuid> frameNoToUuid;
    QHash<int, QUuid> jointNoToUuid;
    QHash<int, QUuid> memberNoToUuid;
    QHash<int, QUuid> wallNoToUuid;
//    QHash<int, QUuid> zwallNoToUuid;
    QHash<int, QUuid> slabNoToUuid;
    QHash<QUuid,QPair<int,B3D_DATATYPE>> planeTable;
    QHash<int, QUuid> damperNoToUuid;
    QMultiHash<int, QUuid> springNoToUuid;
    QHash<QUuid,QPair<int,B3D_DATATYPE>> springTable;

    void makeUnifiedData();

    void registerFloors();
    void registerSectionValues();
    void registerOpenings();
    void registerFrames(QList<FRAMEGROUPVALUE>&,QList<FRAMEPOINTSVALUE>&);
    void registerJoints();
    void fixFrameMinMaxValues(QList<FRAMEGROUPVALUE>&, QList<FRAMEPOINTSVALUE>&);
    void registerLineMembers();
    void registerPlaneMembers();
    void registerSprings();

    void registerCmdl();
    void registerMode();
    void registerChck();
    void registerQuDs();
    void registerUuidIndexTable();
    void registerJointOutput(QStringList&);
    void registerMemberOutput(QStringList&);

    //data read
    QVector<qint32> read1DimArrayInt(QDataStream &,int) const;
    QVector<QVector<qint32>> read2DimArrayInt(QDataStream &,int,int) const ;
    QVector<QVector<QVector<qint32>>> read3DimArrayInt(QDataStream &,int,int,int) const;
    QVector<QVector<QVector<QVector<qint32>>>> read4DimArrayInt(QDataStream &,int,int,int,int) const;
    QVector<QVector<QVector<QVector<QVector<qint32>>>>> read5DimArrayInt(QDataStream &,int,int,int,int,int) const;

    QVector<double> read1DimArrayFloat(QDataStream &,int) const;
    QVector<QVector<double>> read2DimArrayFloat(QDataStream &,int,int) const;
    QVector<QVector<QVector<double>>> read3DimArrayFloat(QDataStream &,int,int,int) const;
    QVector<QVector<QVector<QVector<double>>>> read4DimArrayFloat(QDataStream &,int,int,int,int) const;
    QVector<QVector<QVector<QVector<QVector<double>>>>> read5DimArrayFloat(QDataStream &,int,int,int,int,int) const;
    QVector<QVector<QVector<QVector<QVector<QVector<double>>>>>> read6DimArrayFloat(QDataStream &,int,int,int,int,int,int) const;
    QVector<QVector<QVector<QVector<QVector<QVector<QVector<double>>>>>>> read7DimArrayFloat(QDataStream &,int,int,int,int,int,int,int) const;


    QVector<double> read1DimArrayDouble(QDataStream &,int) const;
    QVector<QVector<double>> read2DimArrayDouble(QDataStream &,int,int) const;
    QVector<QVector<QVector<double>>> read3DimArrayDouble(QDataStream &,int,int,int) const;

    QString readString(QDataStream &,int charlen) const;
    QStringList readStringList(QDataStream & ,int charlen,int) const;
    QVector<QStringList> read2DimStringList(QDataStream &,int charlen,int,int) const;
    int fileOpenWithDialog();


};
} // namespace post3dapp
#endif // HANDLE_NPSTN_H

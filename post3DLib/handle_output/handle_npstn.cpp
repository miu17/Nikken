
#include "handle_npstn.h"

#include <QtCore>
#include <QAbstractButton>
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QVector3D>
#include <QFuture>
#include <QFutureSynchronizer>
#include <QtConcurrent>


#include "calc_vector2d.h"
#include "calc_vector3d.h"
#include "define_section_struct.h"
#include "fixed_data.h"
#include "fortranFunc.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "manage_springdata.h"
#include "pstn_optional_data.h"
#include "unified_analysisdata.h"
#include "unified_output.h"


//################################# indexは反転 ##############################################
namespace post3dapp{

handleNPstn *handleNPstn::CreateWithDialog(int *errorCode)
{
    handleNPstn* npstn = new handleNPstn();
    *errorCode = npstn->fileOpenWithDialog();
    return npstn;
}

handleNPstn *handleNPstn::CreateWithoutDialog(const QString &filename, int *errorCode)
{
    // 7za.exeが.exeと同じフォルダにある必要がある。
    handleNPstn* npstn = new handleNPstn();
    *errorCode = npstn->read(filename);
    return npstn;
}


void handleNPstn::sleep(unsigned long msec) const
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}

QDir handleNPstn::filepath() const
{
    return QDir(QFileInfo(filename).absolutePath());
}

QDir handleNPstn::tmpdirpath() const
{
    return QDir(filepath().absolutePath() + QDir::separator() + tmpdirname());
}

QString handleNPstn::tmpdirname() const
{
    return QFileInfo(filename).baseName() + "_7za";
}


int handleNPstn::read(const QString &_filename){
    filename = _filename;
    apppath = QCoreApplication::applicationDirPath();
    pstnReadStatus.clear();
    timestamp = QFileInfo(filename).lastModified();
    toDecompressPstn = true;
    readAllSteps = true;


    /*
    int error;
    error = readBLD3D_CMDL();
    if (error < 0)
        return error;
    error = readRST();
    if (error < 0)
        return error;
    error = readQUDS();
    if (error < 0)
        return error;
    error = readRSTQD();
    if (error < 0)
        return error;
    error = readBLD3D_LOAD();
    if (error < 0)
        return error;
    error = readBLD3D_PREA();
    if (error < 0)
        return error;
    error = readBLD3D_CHCK();
    if (error < 0)
        return error;
    error = readBLD3D_STEC(model.mxnfl, model.numfg, model.mxngr, model.numgg, model.numj);
    if (error < 0)
        return error;
    error = readBLD3D_GENE();
    if (error < 0)
        return error;
    //    error = readBLD3D_NMIN();
    //    if (error < 0)
    //        return error;

    readBLD3D_MODE();
    */



    // 他でmodel, preaを使用するので先読み
    auto status = readBLD3D_CMDL(model);
    if (status != READING_STATUS::STATUS_SUCCESS)
        return -1;
    else
        pstnReadStatus.insert(OFILE_MODEL, READING_STATUS::STATUS_SUCCESS);
    status = readBLD3D_PREA(prea, modelcase, model);
    if (status != READING_STATUS::STATUS_SUCCESS)
        return -1;
    else
        pstnReadStatus.insert(OFILE_PREA, READING_STATUS::STATUS_SUCCESS);

    QFutureSynchronizer<READING_STATUS> synchronizer;
    synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readRST, std::ref(rst), std::ref(stresscase), model));
    synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readQUDS, std::ref(quds)));
    synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readRSTQD, std::ref(rstqd), std::ref(qdlist), model));
    synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readBLD3D_LOAD, std::ref(load), model, prea));
    synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readBLD3D_CHCK, std::ref(chck), model));
    synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readBLD3D_MODE, std::ref(mode)));
    synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readBLD3D_GENE, std::ref(gene)));
    synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readBLD3D_STEC, std::ref(stec), model));
    synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readBLD3D_NMIN, std::ref(nmin), model));
    synchronizer.waitForFinished();
    auto list = synchronizer.futures();

    pstnReadStatus.insert(OFILE_RST,list.at(0));
    pstnReadStatus.insert(OFILE_QUDS,list.at(1));
    pstnReadStatus.insert(OFILE_RSTQD,list.at(2));
    pstnReadStatus.insert(OFILE_LOAD,list.at(3));
    pstnReadStatus.insert(OFILE_CHCK,list.at(4));
    pstnReadStatus.insert(OFILE_MODE,list.at(5));
    pstnReadStatus.insert(OFILE_GENE,list.at(6));
    pstnReadStatus.insert(OFILE_STEC,list.at(7));
    pstnReadStatus.insert(OFILE_NMIN,list.at(8));

    for(auto key : pstnReadStatus.keys()){
        if (pstnReadStatus.value(key) == READING_STATUS::STATUS_ERROR){
            qDebug() << key;
            return -1;
        }
    }

    qDebug() << "makeUnified";
    makeUnifiedData();
    return 0;
}

int handleNPstn::fileOpenWithDialog()
{

    //    qDebug()<<"fortran test :: VADD in handleNPstn";
    //    double arr1[] = {1.0, 1.0, 13.2, -21.5};
    //    double arr2[] = {1.5, 5.0, -3.2, 1.5};
    //    int num = 4;


    //    VADD(arr1, arr2,&num);
    //    qDebug()<<"fortran test :: VADD = "<< arr1[0]<< arr1[1]<< arr1[2]<< arr1[3];

    //extract pstn
    apppath = QCoreApplication::applicationDirPath();
    QString tmpfilename = QFileDialog::getOpenFileName( nullptr, QString(), QString(), "File(*npstn*)" );

    if ( tmpfilename.isEmpty() ) return -1;

    //reset status
    pstnReadStatus.clear();

    //progress dialog
    QProgressDialog progress(u8"ファイル読み込み中", u8"キャンセル", 0, 100);
    QPushButton *cancelbutton = new QPushButton;
    cancelbutton->setVisible(false);
    progress.setAutoClose(true);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    progress.setLabelText(u8"ファイル読み込み中");
    progress.setValue(0);

    if ( QFileInfo(tmpfilename).suffix() == "gz") { //gzipの場合
        QProcess *decompgz = new QProcess;
        filename = QFileInfo(tmpfilename).absolutePath() + "/" + QFileInfo(tmpfilename).completeBaseName();
        QFile pstnfile(filename);

        if (pstnfile.exists()) { //pstnがある場合は、ダイアログで解凍するか決める
            QMessageBox askDecompressGz(QMessageBox::Question,
                                        u8".gzファイルの解凍",
                                        QFileInfo(pstnfile).fileName() + u8"は存在します。.gzファイルを解凍しますか？",
                                        QMessageBox::Yes | QMessageBox::No, nullptr);
            askDecompressGz.setButtonText(QMessageBox::Yes, u8"はい");
            askDecompressGz.setButtonText(QMessageBox::No, u8"いいえ");
            int toDecompressGz = askDecompressGz.exec();
            if (toDecompressGz == QMessageBox::Yes) {
                progress.setLabelText(u8".gzファイルの解凍中");
                progress.setValue(10);
                //x：解凍　-y：強制的にYes(ファイルの上書きなど)　-o：解凍先フォルダを指定　（-ir：取り出すファイルを指定,'!'isWildCard）　解凍するファイル名
                //                QString command = "\""+apppath+"/7za.exe\" x -y \"-o" + QFileInfo(tmpfilename).absolutePath() + "/\" " +"\""+tmpfilename+"\"";
                //                decompgz->execute(command,QStringList());
                QStringList arguments;
                QString program="\""+apppath+"/7za.exe\"";
                arguments<<"x";//解凍
                arguments<<"-y";//強制的にYes
                arguments<<"\"-o" + QFileInfo(tmpfilename).absolutePath() + "/\"";//解凍先フォルダ
                arguments<<"\""+tmpfilename+"\"";//解凍するファイル
                decompgz->execute(program,arguments);
            } else if (toDecompressGz == QMessageBox::No) {}
        }
        else if (!pstnfile.exists()) { //pstnが無い場合はgzファイルを解凍
            //x：解凍　-y：強制的にYes(ファイルの上書きなど)　-o：解凍先フォルダを指定　（-ir：取り出すファイルを指定,'!'isWildCard）　解凍するファイル名
            progress.setLabelText(u8".gzファイルの解凍中");
            progress.setValue(10);
            //            QString command = "\""+apppath+"/7za.exe\" x -y \"-o" + QFileInfo(tmpfilename).absolutePath() + "/\" " +"\""+tmpfilename+"\"";
            //            decompgz->execute(command);
            QStringList arguments;
            QString program="\""+apppath+"/7za.exe\"";
            arguments<<"x";//解凍
            arguments<<"-y";//強制的にYes
            arguments<<"\"-o" + QFileInfo(tmpfilename).absolutePath() + "/\"";//解凍先フォルダ
            arguments<<"\""+tmpfilename+"\"";//解凍するファイル
            decompgz->execute(program,arguments);

            for (int time = 0; time < 500; time++) {
                sleep(1000);
                if (pstnfile.open(QIODevice::ReadOnly))break;
                if (time == 499) {
                    progress.close();
                    return -1;
                }
            }
        }
    } else if ( QFileInfo(tmpfilename).suffix() == "npstn") { //pstnの場合
        filename = tmpfilename;
    } else {
        progress.close();
        return -1;
    }

    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    timestamp = QFileInfo(filename).lastModified();

    if (tmpdirpath().exists()) {
        QMessageBox askDecompressPstn(QMessageBox::Question,
                                      u8".npstnファイルの解凍",
                                      tmpdirname() + u8"フォルダーは存在します。.npstnファイルを解凍して上書きしますか？",
                                      QMessageBox::Yes | QMessageBox::No, nullptr);
        askDecompressPstn.setButtonText(QMessageBox::Yes, u8"はい");
        askDecompressPstn.setButtonText(QMessageBox::No, u8"いいえ");
        int askResult = askDecompressPstn.exec();
        if (askResult == QMessageBox::Yes) {
            toDecompressPstn = true;
        } else if (askResult == QMessageBox::No) {
            toDecompressPstn = false;
        }
        //        toDecompressPstn=false;//ディレクトリがあったらpstnの解凍を勝手にスキップ
    } else if (!tmpdirpath().exists()) {
        toDecompressPstn = true;
        bool mkdirerr = filepath().mkdir(tmpdirname());
        if (mkdirerr == false) return -1;
    }
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    //read pstn
    //正常、異常、ファイル無しなどあるのでpstnReadStatusを見ること。こちらにフラグ統合する？

    //memo:modelはOK
    progress.setLabelText(u8"MODEL読み込み中");
    progress.setValue(40);

    pstnReadStatus.insert(OFILE_MODEL, readBLD3D_CMDL(model));
    if (pstnReadStatus.value(OFILE_MODEL) != READING_STATUS::STATUS_SUCCESS) {
        QMessageBox msg;
        msg.setText(u8"エラー ： MODEL");
        msg.exec();
        return -1;
    }
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    //memo:StressはNG。構造体のチェック→部材中間荷重の変更の対応など
    progress.setLabelText(u8"STRESS読み込み中");
    progress.setValue(50);

    QMessageBox askReadAllSteps(QMessageBox::Question,
                                u8"ステップ読み込み中",
                                tmpdirname() + u8"全ステップを読み込みますか？",
                                QMessageBox::Yes | QMessageBox::No, nullptr);
    askReadAllSteps.setButtonText(QMessageBox::Yes, u8"はい");
    askReadAllSteps.setButtonText(QMessageBox::No, u8"いいえ");
    int askResult = askReadAllSteps.exec();
    if (askResult == QMessageBox::Yes) {
        readAllSteps = true;
    } else if (askResult == QMessageBox::No) {
        readAllSteps = false;
    }

    pstnReadStatus.insert(OFILE_RST, readRST(rst, stresscase, model));
    qDebug() << "readRST all done";
    if (pstnReadStatus.value(OFILE_RST) != READING_STATUS::STATUS_SUCCESS) {
        QMessageBox msg;
        msg.setText(u8"エラー ： STRESS");
        msg.exec();
        return -1;
    }
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    //quds
    progress.setLabelText(u8"QUDS読み込み中");
    progress.setValue(52);
    pstnReadStatus.insert(OFILE_QUDS, readQUDS(quds));
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    //rstqd、rstqdがない場合はスキップしたい
    progress.setLabelText(u8"RSTQD読み込み中");
    progress.setValue(55);
    pstnReadStatus.insert(OFILE_RSTQD, readRSTQD(rstqd, qdlist, model));
    if (pstnReadStatus.value(OFILE_RSTQD) == READING_STATUS::STATUS_ERROR) {
        QMessageBox msg;
        msg.setText(u8"エラー ： RSTQD");
        msg.exec();
        return -1;
    }
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    // prea
    progress.setLabelText(u8"PREA読み込み中");
    progress.setValue(60);
    pstnReadStatus.insert(OFILE_PREA, readBLD3D_PREA(prea, modelcase, model));
    if (pstnReadStatus.value(OFILE_PREA) != READING_STATUS::STATUS_SUCCESS) {
        QMessageBox msg;
        msg.setText(u8"エラー ： PREA");
        msg.exec();
        return -1;
    }
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    //load
    progress.setLabelText(u8"LOAD読み込み中");
    progress.setValue(65);
    pstnReadStatus.insert(OFILE_LOAD, readBLD3D_LOAD(load, model, prea));
    qDebug() << "readLoad done";
    if (pstnReadStatus.value(OFILE_LOAD) != READING_STATUS::STATUS_SUCCESS) {
        QMessageBox msg;
        msg.setText(u8"エラー ： LOAD");
        msg.exec();
        return -1;
    }
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    // chck
    progress.setLabelText(u8"CHCK読み込み中");
    progress.setValue(70);
    pstnReadStatus.insert(OFILE_CHCK, readBLD3D_CHCK(chck, model));
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }
    qDebug() << "chck done";
    progress.setLabelText(u8"MODE読み込み中");
    progress.setValue(80);
    pstnReadStatus.insert(OFILE_MODE, readBLD3D_MODE(mode));
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    qDebug() << "gene done";
    progress.setLabelText(u8"GENE読み込み中");
    progress.setValue(85);
    pstnReadStatus.insert(OFILE_GENE, readBLD3D_GENE(gene));
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }

    qDebug() << "stec done";
    progress.setLabelText(u8"STEC読み込み中");
    progress.setValue(85);
    pstnReadStatus.insert(OFILE_STEC, readBLD3D_STEC(stec, model));
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }
    //    qDebug() << "nmin done";
    //    progress.setLabelText(u8"NMIN読み込み中");
    //    progress.setValue(85);
    //    readBLD3D_NMIN();
    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }

    progress.setLabelText(u8"データ登録中");
    progress.setValue(95);
    makeUnifiedData();
    qDebug() << "register uudata";
    if (progress.wasCanceled()) {
        progress.close();
        return -1;
    }


    qDebug() << "progress not canceled";
    progress.setValue(100);
    progress.close();
    qDebug() << "progress close";

    return 0;

}

void handleNPstn::UnzipPstn(const QString& file, const QString& comment) const{
        QString command = "\""+apppath+"/7za.exe\" x -y \"-o" + tmpdirpath().absolutePath() + "/\" " + "\"-ir!" + file + "\" "+"\""+filename+"\"";
    QProcess *decompmdl = new QProcess;
        decompmdl->execute(command);
//    QStringList arguments;
//    QString program="\""+apppath+"/7za.exe\"";
//    arguments<<"x";//解凍
//    arguments<<"-y";//強制的にYes
//    arguments<<"\"-o" + tmpdirpath.absolutePath() + "/\"";//解凍先フォルダ
//    arguments<<"\"-ir!" + file + "\"";//解凍するファイル
//    arguments<<"\""+filename+"\"";//解凍するファイル
//    decompmdl->execute(program,arguments);

    delete decompmdl;
    qDebug() << comment;
}

READING_STATUS handleNPstn::readBLD3D_GENE(DataNBLD3D_GENE &gene) const
{
    const QString Gene = "BLD3D_GENE";
    if (toDecompressPstn) {
        UnzipPstn(Gene, u8"GENEの解凍");
    }

    QFile f_gene(filepath().absolutePath() + "/" + tmpdirname() + "/" + Gene);
    bool successFileOpen = false;
    for (int time = 0; time < 120; time++){
        successFileOpen = f_gene.open(QIODevice::ReadOnly);
        if (successFileOpen)
            break;
        sleep(1000);
    }
    if (!successFileOpen){
        return READING_STATUS::STATUS_NOTEXIST;
    }

    QDataStream ingene(&f_gene);
    ingene.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian
    ingene.skipRawData(head + 80 + new_line); // ICARD*80
    ingene >> gene.kflags >> gene.vflags;

    f_gene.close();
    return READING_STATUS::STATUS_SUCCESS;
}

READING_STATUS handleNPstn::readBLD3D_NMIN(DataNBLD3D_NMIN &nmin, const DataNBLD3D_CMDL &model) const
{
    const QString Nmin = "BLD3D_NMIN";
    if (toDecompressPstn) {
        UnzipPstn(Nmin, u8"NMINの解凍");
    }

    QFile f_nmin(filepath().absolutePath() + "/" + tmpdirname() + "/" + Nmin);
    bool successFileOpen = false;
    for (int time = 0; time < 120; time++){
        successFileOpen = f_nmin.open(QIODevice::ReadOnly);
        if (successFileOpen)
            break;
        sleep(1000);
    }
    if (!successFileOpen){
        return READING_STATUS::STATUS_NOTEXIST;
    }

    QDataStream innmin(&f_nmin);
    innmin.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian
    innmin.skipRawData(head + 80 + new_line); // ICARD*80
    innmin >> nmin.nrstn >> nmin.nrstm;

    innmin.skipRawData(new_line);
    nmin.rnss1 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmss1 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.rnss2 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmss2 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.rnss3 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmss3 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.rnrc1 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmrc1 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.rnrc2 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmrc2 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.rnrc3 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmrc3 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.rnsec1 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmsec1 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.rnsec2 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmsec2 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.rnsec3 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmsec3 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.rnsec5 = read3DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstn);
    innmin.skipRawData(new_line);
    nmin.rmsec5 = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,model.nxy);

    innmin.skipRawData(new_line);
    nmin.ptddnm = read5DimArrayFloat(innmin, model.numla, model.nicj, nmin.nrstm,nmin.nrstn,nmin.nptdd);

    f_nmin.close();
    return READING_STATUS::STATUS_SUCCESS;

}

READING_STATUS handleNPstn::readBLD3D_STEC(DataNBLD3D_STEC &stec, const DataNBLD3D_CMDL &model) const
{
    const QString Stec = "BLD3D_STEC";
    stec.clear();
    if (toDecompressPstn) {
        UnzipPstn(Stec, u8"STECの解凍");
    }

    QFile fStec(filepath().absolutePath() + "/" + tmpdirname() + "/" + Stec);
    bool successFileOpen = false;
    for (int time = 0; time < 10; time++){
        successFileOpen = fStec.open(QIODevice::ReadOnly);
        if (successFileOpen)
            break;
        sleep(500);
    }
    if (!successFileOpen){
        return READING_STATUS::STATUS_NOTEXIST;
    }

    QDataStream instec(&fStec);
    instec.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian
    instec.skipRawData(head + 80); // ICARD*80

    instec.skipRawData(new_line);
    instec >> stec.numec >> stec.numecd >> stec.mxcase >> stec.nset;

    instec.skipRawData(new_line);
    stec.nameec = readStringList(instec, 6, stec.numecd);
    stec.intec = read2DimArrayInt(instec, stec.numecd, stec.nintec);

    instec.skipRawData(new_line);
    stec.bbhfl = read3DimArrayFloat(instec, model.numfg, model.mxnfl, stec.nxy);

    instec.skipRawData(new_line);
    stec.outs1 = read4DimArrayFloat(instec, stec.nset, model.numfg, model.mxnfl, stec.nouts1);

    instec.skipRawData(new_line);
    stec.outs2= read5DimArrayFloat(instec, stec.nset, stec.mxcase, model.numfg, model.mxnfl, stec.nouts2);

    instec.skipRawData(new_line);
    stec.oute1 = read3DimArrayFloat(instec, stec.nset, stec.numecd, stec.noute1);

    instec.skipRawData(new_line);
    stec.oute2 = read4DimArrayFloat(instec, stec.nset, stec.mxcase, stec.numecd, stec.noute2);

    instec.skipRawData(new_line);
    stec.valgr = read7DimArrayFloat(instec, stec.nset, stec.mxcase, model.numgg, model.mxngr, model.numfg, model.mxnfl, stec.nvalgr);

    instec.skipRawData(new_line);
    stec.icgc = read4DimArrayInt(instec, stec.nset, stec.mxcase, model.numj, stec.nicgc);

    instec.skipRawData(new_line);
    stec.dcgc = read4DimArrayFloat(instec, stec.nset, stec.mxcase, model.numj, stec.ndcgc);

    instec.skipRawData(new_line);
    stec.nmfefs = read2DimStringList(instec, 10, stec.nset, stec.mxcase);
    stec.crstdr = read2DimStringList(instec, 10, stec.nset, stec.mxcase);
    stec.mcase = read1DimArrayInt(instec, stec.nset);
    stec.sedir = read2DimArrayDouble(instec, stec.nset, model.numfg);
    stec.nmrste = read2DimStringList(instec, 10, stec.nset, stec.nxy);

    instec.skipRawData(new_line);
    stec.cedir = read2DimArrayFloat(instec, stec.nset, stec.mxcase);

    fStec.close();
    return READING_STATUS::STATUS_SUCCESS;
}

READING_STATUS handleNPstn::readBLD3D_CMDL(DataNBLD3D_CMDL &model) const
{
    const QString Cmdl = "BLD3D_CMDL";
    model.clear();
    if (toDecompressPstn) {
        UnzipPstn(Cmdl, u8"CMDLの解凍");
    }

    QFile cmdl(filepath().absolutePath() + "/" + tmpdirname() + "/" + Cmdl);
    bool successFileOpen = false;
    for (int time = 0; time < 120; time++){
        successFileOpen = cmdl.open(QIODevice::ReadOnly);
        if (successFileOpen)
            break;
        sleep(1000);
    }
    if (!successFileOpen){
        return READING_STATUS::STATUS_NOTEXIST;
    }

    QDataStream incmdl(&cmdl);
    incmdl.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian
    incmdl.skipRawData(head);
    model.icard = readString(incmdl,80); // ICARD*80

    incmdl.skipRawData(new_line);
    incmdl >>
            model.msize >> model.nband >> model.mxnfl >> model.numfg >> model.mxngr >> model.numgg >>
            model.numj >>
            model.mxjfl >> model.mxjgr >> model.mxjm >> model.numrld >> model.numrl >> model.maxsl >>
            model.mjslvd >>
            model.mjslv >> model.mxid4 >> model.numd4d >> model.numd4 >> model.numasd >> model.numas >>
            model.numacd >>
            model.numac >> model.numard >> model.numar >> model.numrrd >> model.numrr >> model.numlsd >>
            model.numls >>
            model.mxjs >> model.mxss >> model.mxjc >> model.mxcc >> model.mxjr >> model.mxrr >> model.nummd >>
            model.numm >>
            model.numbl >> model.numswd >> model.numsw >> model.mxso >> model.numsod >> model.numso >>
            model.mxww >>
            model.numwwd >> model.numww >> model.mxwz >> model.numwzd >> model.numwz >> model.mxmw >>
            model.nummwd >>
            model.nummw >> model.mxmz >> model.nummzd >> model.nummz >> model.numzzd >> model.numzz >>
            model.numwsd >>
            model.numws >> model.numapd >> model.numap >> model.numspd >> model.numsp >> model.numpd >>
            model.nump >>
            model.numabd >> model.numab >> model.numbjd >> model.numbj >> model.numbzd >> model.numbz >>
            model.numbnd >>
            model.numbn >> model.numbqd >> model.numbq >> model.lbymax >> model.numbyd >> model.numby >>
            model.numbsd >>
            model.numbs >> model.numbtd >> model.numbt >> model.numbwd >> model.numbw >> model.nummtd >>
            model.nummt >>
            model.numbd >> model.numb >> model.numcol >> model.mxgrj >> model.mxgrm >> model.mxgrw >>
            model.iexmcb >>
            model.nwk >> model.numd3 >> model.numlbd >> model.numlb >> model.mxdlb1 >> model.mxdlb2 >> model.mxdlb3 >> model.mxdlb4 >>
            model.numlrd >> model.numlr >> model.mxbeam >>model.numlmd >> model.numlm >>
            model.mxlhmm >> model.mxmems >> model.numemsd >> model.numems >>model.numild >> model.numil >>
            model.numeid >> model.numei >> model.numdld >> model.numdl >>
            model.numsdpd >> model.numsdp >> model.numla;

    //値を取り出す際の仮の変数・コンテナ
    incmdl.skipRawData(new_line);
    model.title = readString(incmdl,80);
    constexpr int XYZ = 2;
    model.iroute = read1DimArrayInt(incmdl, XYZ);
    incmdl >> model.iunit;
    incmdl.skipRawData(17); //CHARACTER DTIME*17

    incmdl.skipRawData(new_line);
    model.numfl = read1DimArrayInt(incmdl,model.numfg);
    model.kfl = read2DimArrayInt(incmdl,model.numfg,model.mxnfl);
    model.namefl = read2DimStringList(incmdl,6,model.numfg,model.mxnfl);
    model.ifl = read2DimArrayInt(incmdl,model.numfg,model.mxnfl);
    incmdl.skipRawData(double_precision * model.mxnfl * model.numfg); //HFL[MXNFL][NUMFG]
    model.ahfl = read2DimArrayDouble(incmdl,model.numfg,model.mxnfl);
    incmdl.skipRawData(integer * model.nxy * model.mxnfl * model.numfg); //LSFL[NXY][MXNFL][NUMFG]
    model.lfg = read2DimArrayInt(incmdl, model.numfg, model.nlfg);
    model.dfg = read1DimArrayDouble(incmdl,model.numfg);
    incmdl.skipRawData(double_precision * model.ndfl * model.mxnfl * model.numfg); //DFL[NDFL][MXNFL][NUMFG]
    incmdl.skipRawData(integer * model.mxnfl * model.numfg); //ICFL[MXNFL][NUMFG]
    incmdl.skipRawData(double_precision * model.nrofg * model.numfg); //ROFG[NROFG][NUMFG]
    model.mdlfl = read2DimArrayInt(incmdl, model.numfg, model.mxnfl);
    model.jdspfl = read3DimArrayInt(incmdl,model.numfg, model.mxnfl, 2);

    incmdl.skipRawData(new_line);
    model.numgr = read1DimArrayInt(incmdl,model.numgg);
    model.namegr = read2DimStringList(incmdl,6, model.numgg, model.mxngr);
    model.igg = read1DimArrayInt(incmdl,model.numgg);
    model.dgg = read2DimArrayDouble(incmdl, model.numgg, model.ndgg);
    model.dgr = read2DimArrayDouble(incmdl, model.numgg, model.mxngr);
    model.lgr = read2DimArrayInt(incmdl, model.numgg, model.mxngr);
    incmdl.skipRawData(integer * model.numgg * model.mxngr * model.numfg * model.mxnfl); //JSTGR[numgg][mxngr][numfg][mxnfl]
    incmdl.skipRawData(double_precision * model.numgg * model.mxngr); //ASTGR[numgg][mxngr]
    incmdl.skipRawData(foot);//行終わり

    int writtenByte;
    incmdl >> writtenByte;//FORTRANのwriteで付加される4バイト＝そのwrite文で書き込まれるバイト数
    model.jntnm = read1DimArrayInt(incmdl, model.numj);
    model.namej = readStringList(incmdl, 35, model.numj);
    model.indj = read2DimArrayInt(incmdl, model.numj, model.mxjdof);
    //incmdl.skipRawData(integer * (model.msize + 1)); //MAXA[msize+1] or MAXAE[msize+1]
    //incmdl.skipRawData(integer * (model.msize)); //IVPRM[msize]
    //バイトのずれが生じる場合があり、原因不明。この行終わりまでスキップ
    incmdl.skipRawData(writtenByte - integer*model.numj - 35*model.numj - integer*model.numj*model.mxjdof);

    incmdl.skipRawData(new_line);
    model.xyz = read2DimArrayFloat(incmdl, model.numj, model.ndim);

    incmdl.skipRawData(new_line);
    model.njfl = read1DimArrayInt(incmdl, model.numj);
    model.jntfl = read3DimArrayInt(incmdl, model.numj, model.mxjfl, model.njgk);

    incmdl.skipRawData(new_line);
    model.njgr = read1DimArrayInt(incmdl, model.numj);
    model.jntgr = read3DimArrayInt(incmdl, model.numj, model.mxjgr, model.njgk);

    incmdl.skipRawData(new_line);
    model.njmem = read1DimArrayInt(incmdl, model.numj);//NJMEM[numj]
    model.jmem = read2DimArrayInt(incmdl, model.numj, model.mxjm);//JMEM[numj][mxjm]
    incmdl.skipRawData(new_line);
    model.jntpp = read1DimArrayInt(incmdl, model.numj);//JNTPP[numj]
    model.jntrl = read1DimArrayInt(incmdl, model.numj);//JNTRL[numj]
    model.jrgdf = read1DimArrayInt(incmdl, model.numj);//JRGDF[numj]

    incmdl.skipRawData(new_line);
    model.drl = read2DimArrayFloat(incmdl, model.numrld,model.ndrl);//DRL[numrld][ndrl]

    incmdl.skipRawData(new_line);
    model.jnti = read1DimArrayInt(incmdl, model.mjslvd);
    model.jnto = read2DimArrayInt(incmdl, model.mjslvd, model.maxsl);
    model.jntom = read2DimArrayInt(incmdl, model.mjslvd, model.maxsl);
    model.numsl = read1DimArrayInt(incmdl, model.mjslvd);

    incmdl.skipRawData(new_line);
    model.cosl = read2DimArrayFloat(incmdl, model.mjslvd, model.maxsl);

    incmdl.skipRawData(new_line);
    model.ncid4 = read1DimArrayInt(incmdl, model.numd4d);
    model.id4 = read2DimArrayInt(incmdl, model.numd4d,model.mxid4);

    model.named4=readStringList(incmdl, 6, model.numd4d);//incmdl.skipRawData(6 * model.numd4d); //NAMED4[numd4d]*6

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numd4d * model.ndd4); //DD4[numd4d][ndd4]

    incmdl.skipRawData(new_line);
    model.nameas = readStringList(incmdl, 8, model.numasd);//NAMEAS[numasd]*8
    model.nameac = readStringList(incmdl, 8, model.numacd);//NAMEAC[numacd]*8
    model.namear = readStringList(incmdl, 8, model.numard);//NAMEAR[numard]*8
    model.namerr = readStringList(incmdl, 3, model.numrrd);//NAMERR[numrrd]*3
    incmdl.skipRawData(integer * model.numrrd); //IRR[numrrd]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(4 * model.numasd * model.ndas); //DAS[numasd][ndas]!

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numacd * model.ndac); //DAC[numacd][ndac]!

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numard * model.ndar); //DAR[numard][ndar]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numrrd * model.ndrr); //DRR[numrrd][ndrr]

    incmdl.skipRawData(new_line);
    model.namels = readStringList(incmdl, 16, model.numlsd);
    model.ils1 = read2DimArrayInt(incmdl, model.numlsd, model.nils1);
    model.ils2 = read3DimArrayInt(incmdl, model.numlsd, model.nicj, model.nils2);

    incmdl.skipRawData(new_line);
    model.dls1 = read3DimArrayFloat(incmdl, model.numlsd, model.nicj, model.ndls1);

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numlsd * model.nicj * model.ndls2); //DLS2[numlsd][nicj][ndls2]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numlsd * model.nicj * model.ndls3); //DLS3[numlsd][nicj][ndls3]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxss); //ISTY[numlsd][nicj][mxss]
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxss); //ISTYZ[numlsd][nicj][mxss]
    incmdl.skipRawData(integer * model.numla * model.nicj); //NNS[numlsd][nicj]
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxss); //NJS[numlsd][nicj][mxss]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxss * model.mxjs * model.nyz); //XYS0[numla][nicj][mxss][mxjs][nyz]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxss); //es[numla][nicj][mxss]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxss * model.nlsu * model.nftbc); //sgms[numlsd][nicj][mxss][nlsu][nftbc]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxcc); //icty[numlsd][nicj][mxcc]
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxcc); //ictyz[numlsd][nicj][mxcc]
    incmdl.skipRawData(integer * model.numla * model.nicj); //nnc[numlsd][nicj]
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxcc); //njc[numlsd][nicj][mxcc]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxcc * model.mxjc * model.nyz); //xyc0[numlsd][nicj][mxcc][mxjc][nyz]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxcc); //ec[numlsd][nicj][mxcc]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxcc * model.nlsu); //sgmc[numlsd][nicj][mxcc][nlsu]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxrr); //irty[numlsd][nicj][mxrr]
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxrr); //irtyz[numlsd][nicj][mxrr]
    incmdl.skipRawData(integer * model.numla * model.nicj); //nnr[numlsd][nicj]
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxrr); //njr[numlsd][nicj][mxrr]
    incmdl.skipRawData(integer * model.numla * model.nicj * model.mxrr); //iconr[numlsd][nicj][mxrr]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxrr * model.mxjr * model.nyz); //xyr0[numlsd][nicj][mxrr][mxjr][nyz]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxrr); //er[numlsd][nicj][mxrr]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxrr * model.nlsu); //sgmr[numlsd][nicj][mxrr][nlsu]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxrr * model.mxjr); //ar[numlsd][nicj][mxrr][mxjr]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxrr * model.mxjr); //ap[numlsd][nicj][mxrr][mxjr]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxrr * model.mxjr); //psepr[numlsd][nicj][mxrr][mxjr]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numla * model.nicj * model.mxrr * model.mxjr); //psaxr[numlsd][nicj][mxrr][mxjr]

    incmdl.skipRawData(new_line);
    model.imm1 = read2DimArrayInt(incmdl, model.nummd, model.nimm1);
    model.indm = read2DimArrayInt(incmdl, model.nummd, model.nij);
    model.ifpmm = read2DimArrayInt(incmdl, model.nummd, model.mxpin);

    incmdl.skipRawData(new_line);
    model.dmm1 = read2DimArrayFloat(incmdl, model.nummd, model.ndmm1);

    incmdl.skipRawData(new_line);
    model.evmem = read3DimArrayFloat(incmdl, model.nummd, model.nxy, model.ndim);

    incmdl.skipRawData(new_line);
    qDebug()<<"namebl";
    model.namebl = readStringList(incmdl, 6, model.numbl);
    model.mbl = read2DimArrayInt(incmdl, model.numbl, model.nibl);

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numbl * model.ndbl); //dbl[numbl][ndbl]

    incmdl.skipRawData(new_line);
    qDebug()<<"namesw";
    model.namesw = readStringList(incmdl, 8, model.numswd);
    model.isw = read2DimArrayInt(incmdl, model.numswd, model.nisw);

    incmdl.skipRawData(new_line);
    model.dsw = read2DimArrayFloat(incmdl, model.numswd, model.ndsw);

    //開口
    incmdl.skipRawData(new_line);
    qDebug()<<"nameso";
    model.nameso = readStringList(incmdl, 8, model.numsod);
    model.iso = read3DimArrayInt(incmdl, model.numsod, model.mxso, model.niso);
    model.nso = read1DimArrayInt(incmdl, model.numsod);

    incmdl.skipRawData(new_line);
    model.dso = read3DimArrayFloat(incmdl, model.numsod, model.mxso, model.ndso);

    incmdl.skipRawData(new_line);
    model.iww1 = read2DimArrayInt(incmdl, model.numwwd, model.niww1);
    model.iww2 = read2DimArrayInt(incmdl, model.numwwd, model.mxww);
    model.iww3 = read2DimArrayInt(incmdl, model.numwwd, model.mxww);

    incmdl.skipRawData(new_line);
    model.dww1 = read2DimArrayFloat(incmdl, model.numwwd, model.ndww1);

    incmdl.skipRawData(new_line);
    model.evmemw = read3DimArrayFloat(incmdl, model.numwwd, model.nxz, model.ndim);

    incmdl.skipRawData(new_line);
    model.iwz1 = read2DimArrayInt(incmdl, model.numwzd, model.niwz1);
    model.iwz2 = read2DimArrayInt(incmdl, model.numwzd, model.mxwz);
    incmdl.skipRawData(integer * model.numwzd * model.mxwz); //iwz3[numwzd][mxwz]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numwzd * model.ndwz1); //dwz1[numwzd][ndwz1]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(integer * model.nummwd); //nmw[nummwd]
    incmdl.skipRawData(integer * model.nummwd * model.mxmw); //imw[nummwd][mxmw]

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.nummwd * model.mxmw); //dmw[nummwd][mxmw]

    incmdl.skipRawData(new_line);
    model.nmz = read1DimArrayInt(incmdl, model.nummzd);//nmz[nummzd]
    model.imz = read2DimArrayInt(incmdl, model.nummzd, model.mxmz);//imz[nummzd][mxmz]

    incmdl.skipRawData(new_line);
    model.dmz = read2DimArrayFloat(incmdl, model.nummzd, model.mxmz);//dmz[nummzd][mxmz]

    incmdl.skipRawData(new_line);
    model.izz = read2DimArrayInt(incmdl, model.numzzd, model.nizz);//izz[numzzd][nizz]

    incmdl.skipRawData(new_line);
    model.dzz = read3DimArrayFloat(incmdl, model.numzzd, model.nzdmax, model.ndzz);//dzz[numzzd][nzdmax][ndzz]

    incmdl.skipRawData(new_line);
    model.iws1 = read2DimArrayInt(incmdl, model.numwsd, model.niws1);
    incmdl.skipRawData(integer * model.numwsd); //itypws[numwsd]

    incmdl.skipRawData(new_line);
    model.debws = read2DimArrayFloat(incmdl, model.numwsd, model.ndebws);

    incmdl.skipRawData(new_line);
    model.evws = read3DimArrayFloat(incmdl, model.numwsd, model.nxz, model.ndim);

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(integer * model.numapd * model.niap); //IAP[numapd][niap]
    model.isp = read2DimArrayInt(incmdl, model.numspd, model.nisp);
    model.ipp = read2DimArrayInt(incmdl, model.numpd, model.nipp);

    incmdl.skipRawData(new_line);
    incmdl.skipRawData(single * model.numapd * model.ndap); //DAP[numapd][ndap]

    incmdl.skipRawData(new_line);
    model.dsp = read2DimArrayFloat(incmdl, model.numspd, model.ndsp);

    incmdl.skipRawData(new_line);
    model.evpp = read3DimArrayFloat(incmdl,model.numpd, model.nxy, model.ndim);//EVPP[numpd][nxy][ndim]

    incmdl.skipRawData(new_line);
    model.iab = read2DimArrayInt(incmdl, model.numabd, model.niab);//IAB[numabd][niab]

    incmdl.skipRawData(new_line);
    model.ibj = read2DimArrayInt(incmdl, model.numbjd, model.nibj);
    model.ibn = read2DimArrayInt(incmdl, model.numbnd, model.nibn);
    model.ibq = read2DimArrayInt(incmdl, model.numbqd, model.nibq);
    model.iby = read2DimArrayInt(incmdl, model.numbyd, model.niby);
    model.ibs = read2DimArrayInt(incmdl, model.numbsd, model.nibs);
    model.ibt = read2DimArrayInt(incmdl, model.numbtd, model.nibt);
    incmdl.skipRawData(integer * model.numbwd * model.nibw); //IBW
    model.imt = read2DimArrayInt(incmdl, model.nummtd, model.nimt);
    incmdl.skipRawData(integer * model.numbzd * model.nibz); //IBZ
    model.cbz = readStringList(incmdl, 10, model.numbzd);

    incmdl.skipRawData(new_line);
    model.dab = read2DimArrayFloat(incmdl, model.numabd, model.ndab);

    incmdl.skipRawData(new_line);
    model.dbj = read2DimArrayFloat(incmdl, model.numbjd, model.ndbj);

    //DBZ
    incmdl.skipRawData(new_line);
    model.dbz = read2DimArrayFloat(incmdl, model.numbzd, model.ndbz);
    //DBN
    incmdl.skipRawData(new_line);
    model.dbn = read2DimArrayFloat(incmdl, model.numbnd, model.ndbn);
    //DBQ
    incmdl.skipRawData(new_line);
    model.dbq = read2DimArrayFloat(incmdl, model.numbqd, model.ndbq);
    //DBY
    incmdl.skipRawData(new_line);
    model.dby = read2DimArrayFloat(incmdl, model.numbyd, model.ndby);
    //DBS
    incmdl.skipRawData(new_line);
    model.dbs = read2DimArrayFloat(incmdl, model.numbsd, model.ndbs);
    //DBT
    incmdl.skipRawData(new_line);
    model.dbt = read2DimArrayFloat(incmdl, model.numbtd, model.ndbt);
    //DBW
    incmdl.skipRawData(new_line);
    model.dbw = read2DimArrayFloat(incmdl, model.numbwd, model.ndbw);
    //NSPBY JBYCON
    incmdl.skipRawData(new_line);
    model.nspby = read1DimArrayInt(incmdl, model.numbyd);
    model.jbycon = read2DimArrayInt(incmdl, model.numbyd,model.lbymax);
    //DBYCON
    incmdl.skipRawData(new_line);
    model.dbycon = read3DimArrayFloat(incmdl, model.numbyd, model.lbymax,model.nrbyc);

    //JBBCON
    incmdl.skipRawData(new_line);
    model.jbbcon = read1DimArrayInt(incmdl, model.numbd);
    //DBBCON
    incmdl.skipRawData(new_line);
    model.dbbcon = read2DimArrayFloat(incmdl, model.numbd,model.nrbbc);
    //DMME
    incmdl.skipRawData(new_line);
    model.dmme = read2DimArrayFloat(incmdl, model.nummd,model.ndmme);
    //DZZE
    incmdl.skipRawData(new_line);
    model.dzze = read2DimArrayFloat(incmdl, model.numzzd,model.ndzze);

    //NAMEDL
    incmdl.skipRawData(new_line);
    model.namedl = readStringList(incmdl,8,model.numdld);
    //IDL
    model.idl = read2DimArrayInt(incmdl, model.numdld,model.nisdp);
    //DDL
    incmdl.skipRawData(new_line);
    model.ddl = read2DimArrayFloat(incmdl, model.numdld,model.ndsdp);
    //ISDP
    incmdl.skipRawData(new_line);
    model.isdp = read2DimArrayInt(incmdl, model.numsdpd,model.nisdp);
    //DSDP
    incmdl.skipRawData(new_line);
    model.dsdp = read2DimArrayFloat(incmdl, model.numsdpd,model.ndsdp);
    //PNLJ1
    incmdl.skipRawData(new_line);
    model.pnlj1 = read2DimArrayFloat(incmdl, model.numj,model.npnlj1);
    //IMZTYP
    incmdl.skipRawData(new_line);
    model.imztyp = read2DimArrayInt(incmdl, model.nummzd,model.mxmz);

    //IFLAG
    //ISWOR
    //IEI
    //DEI
    //NAMEEI
    //ITYPBN
    //ITYPBY
    //SECJNT
    cmdl.close();
    return READING_STATUS::STATUS_SUCCESS;
}

READING_STATUS handleNPstn::readQUDS(DataNBLD3D_QUDS &quds) const
{
    quds.clear();
    if (toDecompressPstn) {
        UnzipPstn("BLD3D_QUDS", u8"QUDSの解凍");
    }
    //filepath
    QFile fquds(filepath().absolutePath() + "/" + tmpdirname() + "/BLD3D_QUDS");
    bool successFileOpen = fquds.open(QIODevice::ReadOnly);
    if (successFileOpen == false) {
        for (int time = 0; time < 120; time++) {
            sleep(1000);
            if (successFileOpen == fquds.open(QIODevice::ReadOnly))break;
        }
        return READING_STATUS::STATUS_NOTEXIST;
    }
    QDataStream inqdcv(&fquds);
    inqdcv.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    inqdcv.skipRawData(head + 80 + new_line); // ICARD*80
    inqdcv >> quds.numdq >> quds.numdqd;

    //値を取り出す際の仮の変数・コンテナ
    inqdcv.skipRawData(new_line);
    quds.cnamedq = readStringList(inqdcv, 10, quds.numdqd);

    inqdcv.skipRawData(new_line);
    quds.angldq = read1DimArrayDouble(inqdcv, quds.numdqd);

    inqdcv.skipRawData(new_line);
    quds.casedq = read2DimStringList(inqdcv, 10, quds.numdqd, 2);

    inqdcv.skipRawData(new_line);
    quds.casefes = readStringList(inqdcv, 10, quds.numdqd);

    inqdcv.skipRawData(new_line);
    quds.commdq = readStringList(inqdcv, 10, quds.numdqd);

    inqdcv.skipRawData(new_line);
    quds.kdir = read1DimArrayInt(inqdcv, quds.numdqd);

    fquds.close();
    return READING_STATUS::STATUS_SUCCESS;
}

READING_STATUS handleNPstn::readRSTQD(QHash<QString, DataNBLD3D_RSTQD> &rstqd, QStringList& qdlist, const DataNBLD3D_CMDL &model) const
{
    rstqd.clear();
    qdlist.clear();
    if(!toDecompressPstn) {
        qDebug()<<"not decompress";
        QDir Tmpdirpath(tmpdirpath());
        Tmpdirpath.setNameFilters(QStringList() << "RST*QD");
        Tmpdirpath.setFilter(QDir::Files | QDir::NoSymLinks);
        qdlist = Tmpdirpath.entryList();
        if (qdlist.isEmpty()){
            return READING_STATUS::STATUS_NOTEXIST;
        }
    }
    //filepath
    else {
        QString getlistcommand = "\""+apppath+"/7za.exe\" l -y  -ir!RST*QD -xr!*.pun "+"\""+filename+"\"";
        QProcess getlist;
        getlist.start(getlistcommand,QStringList());
        getlist.waitForFinished();
        QString output(getlist.readAllStandardOutput());
        QStringList outputlist = output.split("\n");
        for (int i = 0; i < outputlist.count(); i++) {
            QString line = outputlist.at(i);
            if (line.left(2) != "20")continue;
            if (line.left(2) == "20")qdlist << line.remove(0,53).trimmed(); //20XXから始まる行、54文字目から呼んで末尾はtrim。2100年問題
        }
        if(qdlist.count()!=0)qdlist.pop_back();
        if (qdlist.isEmpty()){
            return READING_STATUS::STATUS_NOTEXIST;
        }
    }

    for (int qd = 0; qd < qdlist.count(); qd++) {
        QString rstqdname = qdlist.at(qd);
        if (this->toDecompressPstn) {
            UnzipPstn(rstqdname, rstqdname + u8"の解凍");
        }
        QFile rstqdfile(this->filepath().absolutePath() + "/" + this->tmpdirname() + "/" + rstqdname);
        bool successFileOpen = rstqdfile.open(QIODevice::ReadOnly);
        if (successFileOpen == false){
            return READING_STATUS::STATUS_NOTEXIST;
        }

        DataNBLD3D_RSTQD rqd;
        QDataStream inrstqd(&rstqdfile);
        inrstqd.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
        inrstqd.skipRawData(head + 80 + new_line); // ICARD*80
        inrstqd >> rqd.nstp ;
        rqd.iastep = read1DimArrayInt(inrstqd,rqd.nstp);
        inrstqd >> rqd.numis >> rqd.numqs >> rqd.numisd
                >> rqd.numqsd >> rqd.ksolv >> rqd.idir;

        inrstqd.skipRawData(new_line);
        rqd.castep = read1DimArrayFloat(inrstqd , rqd.nstp);

        inrstqd.skipRawData(new_line);
        rqd.qst = read4DimArrayFloat(inrstqd, rqd.nstp, model.numfg, model.mxnfl, rqd.nxyr);

        inrstqd.skipRawData(new_line);
        rqd.ust = read4DimArrayFloat(inrstqd, rqd.nstp, model.numfg, model.mxnfl, rqd.nxyr);

        inrstqd.skipRawData(new_line);
        rqd.ualst = read4DimArrayFloat(inrstqd, rqd.nstp, model.numfg, model.mxnfl, rqd.nualst);

        inrstqd.skipRawData(new_line);
        rqd.qstgr = read6DimArrayFloat(inrstqd, rqd.nstp, model.numgg, model.mxngr, model.numfg, model.mxnfl, rqd.nxyr);

        inrstqd.skipRawData(new_line);
        rqd.ustgr = read6DimArrayFloat(inrstqd, rqd.nstp, model.numgg, model.mxngr, model.numfg, model.mxnfl, rqd.nxyr);

        inrstqd.skipRawData(new_line);
        rqd.ualgr = read6DimArrayFloat(inrstqd, rqd.nstp, model.numgg, model.mxngr, model.numfg, model.mxnfl, rqd.nualgr);

        inrstqd.skipRawData(new_line);
        rqd.iflhng = read3DimArrayInt(inrstqd, model.numfg, model.mxnfl, rqd.nhng);
        rqd.igrhng = read5DimArrayInt(inrstqd, model.numgg, model.mxngr, model.numfg, model.mxnfl, rqd.nhng);

        inrstqd.skipRawData(new_line);
        rqd.dufmm = read2DimArrayFloat(inrstqd,model.nummd,rqd.ndufmm);

        inrstqd.skipRawData(new_line);
        rqd.ultmm = read2DimArrayFloat(inrstqd,model.nummd,rqd.nultmm);

        inrstqd.skipRawData(new_line);
        rqd.istpmm = read2DimArrayInt(inrstqd,model.nummd,rqd.nstpmm);

        inrstqd.skipRawData(new_line);
        rqd.cstpmm = read2DimArrayFloat(inrstqd,model.nummd,rqd.nstpmm);

        inrstqd.skipRawData(new_line);
        rqd.ultww = read2DimArrayFloat(inrstqd,model.numwwd,rqd.nultww);

        inrstqd.skipRawData(new_line);
        rqd.istpww = read2DimArrayInt(inrstqd,model.numwwd,rqd.nstpww);

        inrstqd.skipRawData(new_line);
        rqd.cstpww = read2DimArrayFloat(inrstqd,model.numwwd,rqd.nstpww);

        inrstqd.skipRawData(new_line);
        rqd.istppp = read3DimArrayInt(inrstqd,model.numpd,rqd.npdim,rqd.nstppp);

        inrstqd.skipRawData(new_line);
        rqd.cstppp = read3DimArrayFloat(inrstqd,model.numpd,rqd.npdim,rqd.nstppp);

        inrstqd.skipRawData(new_line);

        inrstqd.skipRawData(new_line);
        rqd.coefis = read1DimArrayDouble(inrstqd,rqd.numisd);
        qDebug()<<"nameis";
        rqd.nameis = readStringList(inrstqd,10,rqd.numisd);
        rqd.nkcis = read1DimArrayInt(inrstqd,rqd.numisd);
        rqd.ikcis = read2DimArrayInt(inrstqd,rqd.numisd,rqd.mxkcis);
        rqd.ckcis = read2DimArrayDouble(inrstqd,rqd.numisd,rqd.mxkcis);

        inrstqd.skipRawData(new_line);
        rqd.coefqs = read1DimArrayDouble(inrstqd,rqd.numqsd);
        qDebug()<<"nameqs";
        rqd.nameqs = readStringList(inrstqd,10,rqd.numqsd);
        rqd.nkcqs = read1DimArrayInt(inrstqd,rqd.numqsd);
        rqd.ikcqs = read2DimArrayInt(inrstqd,rqd.numqsd,rqd.mxkcqs);
        rqd.ckcqs = read2DimArrayDouble(inrstqd,rqd.numqsd,rqd.mxkcqs);

        inrstqd.skipRawData(new_line);
        qDebug()<<"nameam";
        rqd.nameam = readString(inrstqd,10);
        qDebug()<<"nmrsu";
        rqd.nmrsu = readString(inrstqd,10);

        inrstqd.skipRawData(new_line);
        rqd.sedir = read1DimArrayDouble(inrstqd,model.numfg);

        inrstqd.skipRawData(new_line);
        rqd.rfgqun = read3DimArrayFloat(inrstqd,model.numfg,model.mxnfl,rqd.nfgqun);

        inrstqd.skipRawData(new_line);
        rqd.mfgqun = read3DimArrayInt(inrstqd,model.numfg,model.mxnfl,rqd.nfrw);

        inrstqd.skipRawData(new_line);
        rqd.mamem = read2DimArrayInt(inrstqd,model.nummd,rqd.nmamem);
        //       qDebug()<<rqd.mamem;

        inrstqd.skipRawData(new_line);
        rqd.rdmem = read3DimArrayFloat(inrstqd,model.nummd,model.nij,rqd.nrdmem);

        inrstqd.skipRawData(new_line);
        rqd.ramem = read2DimArrayFloat(inrstqd,model.nummd,rqd.nramem);

        inrstqd.skipRawData(new_line);
        rqd.maplt = read2DimArrayInt(inrstqd,model.numwwd,rqd.nmaplt);

        inrstqd.skipRawData(new_line);
        rqd.rdplt = read2DimArrayFloat(inrstqd,model.numwwd,rqd.nrdplt);

        inrstqd.skipRawData(new_line);
        rqd.kajnt = read4DimArrayInt(inrstqd,model.numgg,model.mxngr,model.mxgrj,rqd.nkajnt);

        inrstqd.skipRawData(new_line);
        rqd.sajnt = read4DimArrayFloat(inrstqd,model.numgg,model.mxngr,model.mxgrj,rqd.nsajnt);

        inrstqd.skipRawData(new_line);
        rqd.kamem = read4DimArrayInt(inrstqd,model.numgg,model.mxngr,model.mxgrm,rqd.nkamem);

        inrstqd.skipRawData(new_line);
        rqd.samem = read4DimArrayFloat(inrstqd,model.numgg,model.mxngr,model.mxgrm,rqd.nsamem);

        inrstqd.skipRawData(new_line);
        inrstqd >> rqd.iend >> rqd.lqustep;

        inrstqd.skipRawData(new_line);
        rqd.icpnl = read3DimArrayInt(inrstqd,model.numj,model.nyz,rqd.nicpnl);

        inrstqd.skipRawData(new_line);
        rqd.dcpnl = read3DimArrayFloat(inrstqd,model.numj,model.nyz,rqd.ndcpnl);

        inrstqd.skipRawData(new_line);
        rqd.dsfl = read3DimArrayFloat(inrstqd,model.numfg,model.mxnfl,rqd.ndsfl);

        inrstqd.skipRawData(new_line);
        rqd.cqumm = read2DimArrayFloat(inrstqd,model.nummd,rqd.ncqumm);

        inrstqd.skipRawData(new_line);
        rqd.cquww = read2DimArrayFloat(inrstqd,model.numwwd,rqd.ncquww);

        inrstqd.skipRawData(new_line);
        rqd.cqupp = read3DimArrayFloat(inrstqd,model.numj,model.nyz,rqd.ncqupp);

        inrstqd.skipRawData(new_line);
        rqd.cqust = read2DimArrayFloat(inrstqd,model.numfg,model.mxnfl);

        inrstqd.skipRawData(new_line);
        rqd.lqumm = read2DimArrayInt(inrstqd,model.nummd,rqd.nlqumm);
        rqd.lquww = read2DimArrayInt(inrstqd,model.numwwd,rqd.nlquww);
        rqd.lqupp = read3DimArrayInt(inrstqd,model.numj,model.nyz,rqd.nlqupp);
        rqd.lqust = read2DimArrayInt(inrstqd,model.numfg,model.mxnfl);

        inrstqd.skipRawData(new_line);
        rqd.cgpjnt = read2DimArrayFloat(inrstqd,model.numj,rqd.ncgpjnt);

        inrstqd.skipRawData(new_line);
        rqd.cgpcmp = read3DimArrayFloat(inrstqd,model.numfg,model.mxnfl,rqd.ncgpcmp);

        inrstqd.skipRawData(new_line);
        rqd.rstmml = read2DimArrayFloat(inrstqd,model.nummd,rqd.nrstmm);

        inrstqd.skipRawData(new_line);
        rqd.jsscb = read2DimArrayInt(inrstqd,model.numlbd,rqd.njsscb);

        inrstqd.skipRawData(new_line);
        rqd.esscb = read2DimArrayFloat(inrstqd,model.numlbd,rqd.nesscb);

        inrstqd.skipRawData(new_line);
        rqd.esrcb = read2DimArrayFloat(inrstqd,model.numlrd,rqd.nesrcb);

        inrstqd.skipRawData(new_line);
        rqd.ibar1 = read2DimArrayInt(inrstqd,model.nummd,rqd.nibar1);
        rqd.ibar2 = read2DimArrayInt(inrstqd,model.numj,rqd.nibar2);
        rqd.ibar3 = read2DimArrayInt(inrstqd,model.numj,rqd.nibar3);

        inrstqd.skipRawData(new_line);
        rqd.dbar1 = read2DimArrayFloat(inrstqd,model.nummd,rqd.ndbar1);

        inrstqd.skipRawData(new_line);
        rqd.dbar2 = read2DimArrayFloat(inrstqd,model.numj,rqd.ndbar2);

        inrstqd.skipRawData(new_line);
        rqd.dbar3 = read2DimArrayFloat(inrstqd,model.numj,rqd.ndbar3);

        rstqd.insert(rstqdname,rqd);

    }
    return READING_STATUS::STATUS_SUCCESS;
}

READING_STATUS handleNPstn::readRST(QHash<QString, DataNBLD3D_RST> &rst, QStringList &stresscase, const DataNBLD3D_CMDL &model) const
{
    rst.clear();
    stresscase.clear();
    QDir Tmpdirpath(tmpdirpath());
    if (!toDecompressPstn) {
        Tmpdirpath.setNameFilters(QStringList() << "RST*");
        Tmpdirpath.setFilter(QDir::Files | QDir::NoSymLinks);
        QStringList tmp;
        tmp = Tmpdirpath.entryList();
        foreach(QString str,tmp){
            if(str.right(2)!="QD"){
                stresscase.append(str);
            }
        }
        if (stresscase.isEmpty()){
            return READING_STATUS::STATUS_NOTEXIST;
        }
    }

    //filepath
    else {
        //        QString getlistcommand = "\"" + apppath + "/7za.exe\" l -y  -ir!RST* -xr!*.pun -xr!*QD " + filename;
        QString getlistcommand = "\""+apppath+"/7za.exe\" l -y  -ir!RST* -xr!*.pun -xr!*QD "+"\""+filename+"\"";
        QProcess getlist;
        getlist.start(getlistcommand,QStringList());
        getlist.waitForFinished();
        QString output(getlist.readAllStandardOutput());
        QStringList outputlist = output.split("\n");
        for (int i = 0; i < outputlist.count(); i++) {
            QString line = outputlist.at(i);
            if (line.left(2) != "20")continue;
            if (line.left(2) == "20")stresscase << line.remove(0,
                                                               53).trimmed(); //20XXから始まる行、54文字目から呼んで末尾はtrim。2100年問題
        }
        stresscase.pop_back();
        if (stresscase.isEmpty()){
            return READING_STATUS::STATUS_NOTEXIST;
        }
    }

    QFutureSynchronizer<QPair<READING_STATUS, DataNBLD3D_RST>> synchronizer;
    for (auto rstname : stresscase){
        synchronizer.addFuture(QtConcurrent::run(this, &handleNPstn::readOneRst, rstname, model));
    }
    synchronizer.waitForFinished();
    auto list = synchronizer.futures();
    for (int i = 0; i < stresscase.count(); i++){
        QPair<READING_STATUS, DataNBLD3D_RST> pair = list.at(i);
        if (pair.first != READING_STATUS::STATUS_SUCCESS)
            return pair.first;
        rst.insert(stresscase.at(i), pair.second);
    }
    //読み込みここまで

    Tmpdirpath.setNameFilters(QStringList() << "RST*");
    Tmpdirpath.setFilter(QDir::Files | QDir::NoSymLinks);
    stresscase.clear();
    stresscase = Tmpdirpath.entryList();
    if (stresscase.isEmpty()){
        return READING_STATUS::STATUS_NOTEXIST;
    }
    return READING_STATUS::STATUS_SUCCESS;
}

QPair<READING_STATUS, DataNBLD3D_RST>  handleNPstn::readOneRst(const QString& rstname, const DataNBLD3D_CMDL &model) const
{
    if (this->toDecompressPstn) {
        UnzipPstn(rstname, rstname + u8"の解凍");
    }
    QFile rstfile(this->filepath().absolutePath() + "/" + this->tmpdirname() + "/" + rstname);
    bool successFileOpen = rstfile.open(QIODevice::ReadOnly);
    if (successFileOpen == false){
        return QPair<READING_STATUS, DataNBLD3D_RST>{READING_STATUS::STATUS_NOTEXIST, DataNBLD3D_RST{}};
    }

    DataNBLD3D_RST nbld3d_rst;
    QDataStream inrst(&rstfile);
    inrst.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    inrst.skipRawData(head + 80); //ICARD*80

    inrst.skipRawData(new_line);
    inrst.skipRawData(integer); // -1
    nbld3d_rst.nameam = readString(inrst, 10);
    inrst.skipRawData(integer * 9); // NCAM, NUMJ, NUMM, NUMWW, NUMB, MSIZE, NBAND, ICHKW, LCASE(1)
    inrst.skipRawData(foot);

    int step = 0;
    while(true){
        DataNBLD3D_RSTStep rstcase;
        inrst.skipRawData(head);
        qDebug() << "iastep";
        inrst >> rstcase.iastep >> rstcase.iend;
        if (rstcase.iastep <= 0){
            inrst.skipRawData(foot);
            break;
        }
        qDebug() << "ujnt";
        inrst.skipRawData(new_line);
        rstcase.ujnt = read2DimArrayFloat(inrst,model.numj,model.mxjdof); // 変位

        inrst.skipRawData(new_line);
        rstcase.fintj = read2DimArrayFloat(inrst,model.numj,model.mxjdof);

        inrst.skipRawData(new_line);
        rstcase.react = read2DimArrayFloat(inrst,model.numj,model.mxjdof); // 反力

        inrst.skipRawData(new_line);
        rstcase.rstmm = read2DimArrayFloat(inrst,model.nummd,nbld3d_rst.nrst);// 部材応力

        inrst.skipRawData(new_line);
        rstcase.phaiyz = read3DimArrayFloat(inrst,model.nummd,model.nij,nbld3d_rst.nphai);

        inrst.skipRawData(new_line);
        rstcase.ipspmm = read2DimArrayInt(inrst,model.nummd,nbld3d_rst.nipspm);
        rstcase.ductmm = read2DimArrayDouble(inrst,model.nummd,nbld3d_rst.nductm);
        rstcase.kpspmm = read2DimArrayInt(inrst,model.nummd,nbld3d_rst.nipspm);

        inrst.skipRawData(new_line);
        rstcase.rstww = read3DimArrayFloat(inrst,model.numwwd,nbld3d_rst.nelbww,nbld3d_rst.nrstww);

        inrst.skipRawData(new_line);
        rstcase.ipspww = read2DimArrayInt(inrst,model.numwwd,nbld3d_rst.nipspw);

        inrst.skipRawData(new_line);
        rstcase.fintws = read2DimArrayFloat(inrst,model.numwsd,nbld3d_rst.nadfws);

        inrst.skipRawData(new_line);
        rstcase.rstws = read3DimArrayFloat(inrst,model.numwsd,nbld3d_rst.ngaus,nbld3d_rst.nrstws);

        inrst.skipRawData(new_line);
        rstcase.rstpp = read2DimArrayFloat(inrst,model.numpd,nbld3d_rst.npdim);

        inrst.skipRawData(new_line);
        rstcase.ipsppp = read2DimArrayInt(inrst,model.numpd,nbld3d_rst.npdim);

        inrst.skipRawData(new_line);
        rstcase.rstbb = read1DimArrayFloat(inrst,model.numbd);

        inrst.skipRawData(new_line);
        rstcase.ipspbb = read1DimArrayInt(inrst,model.numbd);

        inrst.skipRawData(new_line);
        rstcase.rstby = read2DimArrayFloat(inrst,model.numbyd,nbld3d_rst.nrstby);

        inrst.skipRawData(new_line);
        rstcase.ipspby = read1DimArrayInt(inrst,model.numbyd);

        inrst.skipRawData(new_line);
        rstcase.rstbs = read2DimArrayFloat(inrst,model.numbsd,nbld3d_rst.nrstbs);

        inrst.skipRawData(new_line);
        inrst >> rstcase.kcasol;

        inrst.skipRawData(new_line);
        rstcase.icase = read1DimArrayInt(inrst,rstcase.kcasol);
        rstcase.ccase = read1DimArrayDouble(inrst,rstcase.kcasol);

        inrst.skipRawData(new_line);
        rstcase.dsdpv = read2DimArrayFloat(inrst,model.numsdpd,nbld3d_rst.ndsdpv);

        inrst.skipRawData(new_line);
        rstcase.squmm = read3DimArrayFloat(inrst,model.nummd,model.nicj,model.nyz);

        inrst.skipRawData(new_line);
        rstcase.squww = read1DimArrayFloat(inrst,model.numwwd);
        inrst.skipRawData(foot);
        step++;
        nbld3d_rst.stressSteps.append(rstcase);
    }
    rstfile.close();

    return QPair<READING_STATUS, DataNBLD3D_RST>{READING_STATUS::STATUS_SUCCESS, nbld3d_rst};
}

READING_STATUS handleNPstn::readBLD3D_LOAD(DataNBLD3D_LOAD &load, const DataNBLD3D_CMDL &model, const QHash<QString, DataNBLD3D_PREA>& prea) const
{
    QString model1 = "MODEL1";
    load.clear();
    if (toDecompressPstn) {
        UnzipPstn("BLD3D_LOAD", u8"LOADの解凍");
    }

    //filepath
    QFile fload(filepath().absolutePath() + "/" + tmpdirname() + "/BLD3D_LOAD");
    bool successFileOpen = fload.open(QIODevice::ReadOnly);
    if (successFileOpen == false){
        return READING_STATUS::STATUS_NOTEXIST;
    }
    QDataStream inload(&fload);
    inload.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian
    inload.skipRawData(head + 80 + new_line); // ICARD*80
    inload >>
            load.numc >> load.lxlemm >> load.numcl >> load.numcld >> load.numse >>
            load.numsed >> load.mxwifl >> load.numwi >> load.numwid >> load.mxlhmm;

    //    qDebug()<<"numc="<<load.numc;
    //    exit(0);
    inload.skipRawData(new_line);
    load.indle = read1DimArrayInt(inload,load.numc);
    load.namel = readStringList(inload,6,load.numc);
    load.titc = readStringList(inload,60,load.numc);
    load.indse = read1DimArrayInt(inload,load.numc);
    load.indwi = read1DimArrayInt(inload,load.numc);

    inload.skipRawData(new_line);
    load.dirse = read1DimArrayFloat(inload,load.numc);

    inload.skipRawData(new_line);
    load.frcfl = read4DimArrayFloat(inload,load.numc,model.numfg,model.mxnfl,load.nfrcfl);

    inload.skipRawData(new_line);
    load.ibc = read2DimArrayInt(inload,load.numc,model.msize);
    load.kincj = read3DimArrayInt(inload,load.numc,model.numj,model.mxjdof);

    inload.skipRawData(new_line);
    load.fincj = read3DimArrayFloat(inload,load.numc,model.numj,model.mxjdof);

    inload.skipRawData(new_line);
    load.nlemm = read1DimArrayInt(inload,load.numcld);
    load.ilemm = read3DimArrayInt(inload,load.numcld,load.lxlemm,load.nilemm);

    inload.skipRawData(new_line);
    load.cmqmm = read3DimArrayFloat(inload,load.numcld,model.nummd,load.ncmqmm);

    inload.skipRawData(new_line);
    load.dlemm = read3DimArrayFloat(inload,load.numcld,load.lxlemm,load.ndlemm);

    inload.skipRawData(new_line);
    load.clemm = read3DimArrayFloat(inload,load.numcld,load.lxlemm,load.ncmqmm);

    inload.skipRawData(new_line);
    load.cmqmmf = read3DimArrayFloat(inload,load.numcld,model.nummd,load.ncmqmm);

    inload.skipRawData(new_line);
    load.cmqww = read3DimArrayFloat(inload,load.numcld,model.numwwd,prea.value(model1).neww);

    inload.skipRawData(new_line);
    load.dleww = read3DimArrayFloat(inload,load.numcld,model.numwwd,load.ndleww);

    inload.skipRawData(new_line);
    load.cmqws = read3DimArrayFloat(inload,load.numcld,model.numwsd,load.ncmqws);

    inload.skipRawData(new_line);
    load.dlews = read3DimArrayFloat(inload,load.numcld,model.numwsd,load.ndlews);

    inload.skipRawData(new_line);
    load.semass = read2DimArrayFloat(inload,model.numj,load.jwcase);

    inload.skipRawData(new_line);
    load.flmass = read3DimArrayFloat(inload,model.numfg,model.mxnfl,load.jwcase);

    inload.skipRawData(new_line);
    load.isefg = read3DimArrayInt(inload,load.numsed,model.numfg,load.nisefg);
    load.isefl = read4DimArrayInt(inload,load.numsed,model.numfg,model.mxnfl,load.nisefl);
    load.namese = readStringList(inload,6,load.numsed);
    load.itypse = read1DimArrayInt(inload,load.numsed);

    inload.skipRawData(new_line);
    load.dsefg = read3DimArrayFloat(inload,load.numsed,model.numfg,load.ndsefg);

    inload.skipRawData(new_line);
    load.dsefl = read4DimArrayFloat(inload,load.numsed,model.numfg,model.mxnfl,load.ndsefl);

    inload.skipRawData(new_line);
    load.iwifg = read3DimArrayInt(inload,load.numwid,model.numfg,load.niwifg);
    load.nwifl = read3DimArrayInt(inload,load.numwid,model.numfg,model.mxnfl);
    load.iwifl = read5DimArrayInt(inload,load.numwid,model.numfg,model.mxnfl,load.mxwifl,load.niwifl);
    load.namewi = readStringList(inload,6,load.numwid);
    load.itypwi = read1DimArrayInt(inload,load.numwid);

    inload.skipRawData(new_line);
    load.dwifg = read3DimArrayFloat(inload,load.numwid,model.numfg,load.ndwifg);

    inload.skipRawData(new_line);
    load.dwifl = read5DimArrayFloat(inload,load.numwid,model.numfg,model.mxnfl,load.mxwifl,load.ndwifl);

    inload.skipRawData(new_line);
    load.nlhmm = read1DimArrayInt(inload,load.numcld);

    inload.skipRawData(new_line);
    load.ilhmm = read3DimArrayInt(inload,load.numcld,load.mxlhmm,load.nilhmm);

    inload.skipRawData(new_line);
    load.dlhmm = read3DimArrayFloat(inload,load.numcld,model.mxlhmm,load.ndlhmm);

    inload.skipRawData(new_line);
    load.lndle = read3DimArrayInt(inload,load.numcld,model.nummd,load.nlndle);
    load.lndlh = read3DimArrayInt(inload,load.numcld,model.nummd,load.nlndlh);

    fload.close();
    return READING_STATUS::STATUS_SUCCESS;
}

READING_STATUS handleNPstn::readBLD3D_PREA(QHash<QString, DataNBLD3D_PREA> &prea, QStringList &modelcase, const DataNBLD3D_CMDL &model) const
{
    prea.clear();
    modelcase.clear();
    if (!toDecompressPstn) {
        QDir Tmpdirpath(tmpdirpath());
        Tmpdirpath.setNameFilters(QStringList() << "MODEL*");
        Tmpdirpath.setFilter(QDir::Files | QDir::NoSymLinks);
        modelcase = Tmpdirpath.entryList();
        if (modelcase.isEmpty()){
            return READING_STATUS::STATUS_NOTEXIST;
        }
    }

    //filepath
    else {
        //        QString getlistcommand = "\"" + apppath + "/7za.exe\" l -y  -ir!RST* -xr!*.pun -xr!*QD " + filename;
        QString getlistcommand = "\""+apppath+"/7za.exe\" l -y  -ir!MODEL* -xr!*.pun -xr!*QD "+"\""+filename+"\"";
        QProcess getlist;
        getlist.start(getlistcommand,QStringList());
        getlist.waitForFinished();
        QString output(getlist.readAllStandardOutput());
        QStringList outputlist = output.split("\n");
        for (int i = 0; i < outputlist.count(); i++) {
            QString line = outputlist.at(i);
            if (line.left(2) != "20")continue;
            if (line.left(2) == "20")modelcase << line.remove(0,53).trimmed(); //20XXから始まる行、54文字目から呼んで末尾はtrim。2100年問題
        }
        modelcase.pop_back();
        if (modelcase.isEmpty()){
            return READING_STATUS::STATUS_NOTEXIST;
        }
    }

    for (int mdcase = 0; mdcase < modelcase.count(); mdcase++) {
        QString modelname = modelcase.at(mdcase);
        if (this->toDecompressPstn) {
            UnzipPstn(modelname, modelname + u8"の解凍");
        }
        QFile fprea(this->filepath().absolutePath() + "/" + this->tmpdirname() + "/" + modelname);
        bool successFileOpen = fprea.open(QIODevice::ReadOnly);
        if (successFileOpen == false){
            return READING_STATUS::STATUS_NOTEXIST;
        }
        DataNBLD3D_PREA m_prea;


        QDataStream inprea(&fprea);
        inprea.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
        //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian

        qDebug()<<"preread";
        inprea.skipRawData(head + 80 + new_line); // ICARD*80
        inprea >> m_prea.numwp >> m_prea.numwpd;
        inprea.skipRawData(integer * 6); //nmtyp[6]
        inprea >> m_prea.numez >> m_prea.numezd >>
                m_prea.numms >> m_prea.nummsd >> m_prea.numbr >> m_prea.numbrd >> m_prea.numcb >>
                m_prea.lmsmax >> m_prea.lmrmax >> m_prea.lmsmxw >>
                m_prea.mxzs >> m_prea.numzs >> m_prea.numzsd >>
                m_prea.numis >>m_prea.numisd>>m_prea.numqs>>m_prea.numqsd>>m_prea.numhg>>m_prea.numhgd>>m_prea.numg1>>m_prea.numg1d;

        inprea>>m_prea.numhg3>>m_prea.numhg3d>>m_prea.numhg2>>m_prea.numhg2d>>
                m_prea.ifbmax>>m_prea.numfs>>m_prea.numfsd>>m_prea.numfb>>m_prea.numfbd>>
                m_prea.nrstn>>m_prea.nrstm>>m_prea.numnmu>>m_prea.numnmud>>m_prea.numnmp>>m_prea.numnmpd>>
                m_prea.njdel>>m_prea.nd4del>>
                m_prea.nrstnw>>m_prea.numhgw>>m_prea.numhgwd>>m_prea.numnmw>>m_prea.numnmwd>>
                m_prea.maxsla>>m_prea.mjslvad>>m_prea.mjslva>>m_prea.msize>>m_prea.nband>>m_prea.nwk;

        if(m_prea.numwpd==0)m_prea.numwpd=1;
        if(m_prea.numezd==0)m_prea.numezd=1;
        if(m_prea.nummsd==0)m_prea.nummsd=1;
        if(m_prea.numbrd==0)m_prea.numbrd=1;
        if(m_prea.numzsd==0)m_prea.numzsd=1;
        if(m_prea.numaxcd==0)m_prea.numaxcd=1;
        if(m_prea.numhgd==0)m_prea.numhgd=1;
        if(m_prea.numhsd==0)m_prea.numhsd=1;

        inprea.skipRawData(new_line);
        m_prea.mtyp = read2DimArrayInt(inprea,model.nummd,m_prea.lmtyp);

        inprea.skipRawData(new_line);
        m_prea.debmm = read2DimArrayFloat(inprea,model.nummd,m_prea.ndeb);

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.nzsiz*m_prea.numezd); //zstmm[numezd][nzsiz]

        inprea.skipRawData(new_line);
        inprea.skipRawData(integer * model.nij*m_prea.nummsd); //nspms[nummsd][nij]
        inprea.skipRawData(integer * m_prea.nimsc*m_prea.lmsmax*2*m_prea.nummsd); //jmscon[nummsd][2][lmsmax][nimsc]

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.nrmsc*m_prea.lmsmax*2*m_prea.nummsd); //dmscon[nummsd][2][lmsmax][nrmsc]

        inprea.skipRawData(new_line);
        inprea.skipRawData(integer * m_prea.nummsd); //nspmr[nummsd]
        inprea.skipRawData(integer * m_prea.nimrc*m_prea.lmrmax*m_prea.nummsd); //jmrcon[nummsd][lmrmax][nimrc]

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.nrmrc*m_prea.lmrmax*m_prea.nummsd); //dmrcon[nrmrc][lmrmax][nummsd]

        inprea.skipRawData(new_line);
        inprea.skipRawData(integer * m_prea.nibrc*m_prea.numbrd); //jbrcon[nibrc][numbrd]

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.nrbrc*m_prea.numbrd); //dbrcon[nrbrc][numbrd]

        inprea.skipRawData(new_line);
        inprea.skipRawData(integer * m_prea.lmtypw*model.numwwd); //mtypww[lmtypw][numwwd]
        inprea.skipRawData(integer * m_prea.nrect*model.numwwd); //indww[nrect][numwwd]

        inprea.skipRawData(new_line);
        m_prea.debww = read2DimArrayFloat(inprea,model.numwwd,m_prea.ndebww);

        inprea.skipRawData(new_line);
        inprea.skipRawData(integer * model.nij*m_prea.numwpd); //nspmsw[nij][numwpd]
        inprea.skipRawData(integer * m_prea.nimscw*m_prea.lmsmxw*model.nij*m_prea.numwpd); //jmscow[nimscw][lmsmxw][ij][numwpd]

        inprea.skipRawData(new_line);
        constexpr int NIJ = 2;
        m_prea.dmscow = read4DimArrayFloat(inprea,m_prea.numwpd,NIJ,m_prea.lmsmxw,m_prea.nrmscw);

        inprea.skipRawData(new_line);
        inprea.skipRawData(integer * m_prea.nimrcw*m_prea.numwpd); //jmrcow

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.nrmrcw*m_prea.numwpd); //dmrcow

        inprea.skipRawData(new_line);
        m_prea.indws = read2DimArrayInt(inprea,model.numwsd,m_prea.nrect);

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.ndebws*model.numwsd); //debws

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.ndws1*model.numwsd); //dws1

        inprea.skipRawData(new_line);
        inprea.skipRawData(integer * model.numpd); //jindpp
        m_prea.jppcon = read2DimArrayInt(inprea,model.numpd,m_prea.npdim);

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.ndebpp*m_prea.npdim*model.numpd); //debpp

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.nrppc*m_prea.npdim*model.numpd); //dppcon

        inprea.skipRawData(new_line);
        inprea.skipRawData(integer * model.numbd); //jbbcon

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.nrbbc*model.numbd); //dbbcon

        inprea.skipRawData(new_line);
        m_prea.nspby = read1DimArrayInt(inprea,model.numbyd);
        inprea.skipRawData(integer * model.lbymax*model.numbyd); //jbycon

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.nrbyc*model.lbymax*model.numbyd); //dbycon

        inprea.skipRawData(new_line);
        m_prea.rstmmi = read2DimArrayFloat(inprea,model.nummd,m_prea.nrstmm);

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.ncmqmm*model.nummd); //cmqmmi

        inprea.skipRawData(new_line);
        m_prea.dnms = read3DimArrayFloat(inprea,model.nummd,model.nij,m_prea.msdd);//dnms

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.nrstww*m_prea.neww*model.numwwd); //rstwwi

        inprea.skipRawData(new_line);
        inprea.skipRawData(single * m_prea.neww*model.numwwd); //cmqwwi

        inprea.skipRawData(new_line);
        m_prea.imm2 = read2DimArrayInt(inprea, model.nummd,m_prea.nimm2);
        inprea.skipRawData(integer * m_prea.nimm3*2*model.nummd); //imm3

        inprea.skipRawData(new_line);
        m_prea.dmm2 = read2DimArrayFloat(inprea, model.nummd,m_prea.ndmm2);

        //dmm3
        inprea.skipRawData(new_line);
        m_prea.dmm3 = read3DimArrayFloat(inprea, model.nummd,model.nicj,m_prea.ndmm3);
        //dmm5
        inprea.skipRawData(new_line);
        m_prea.dmm5 = read4DimArrayFloat(inprea, model.nummd,model.nicj,model.nyz,m_prea.ndmm5);
        //iww4
        inprea.skipRawData(new_line);
        m_prea.iww4 = read2DimArrayInt(inprea, model.numwwd,m_prea.niww4);
        //dww2
        inprea.skipRawData(new_line);
        m_prea.dww2 = read2DimArrayFloat(inprea, model.numwwd,m_prea.ndww2);
        //dww3
        inprea.skipRawData(new_line);
        m_prea.dww3 = read2DimArrayFloat(inprea, model.numwwd,m_prea.ndww3);
        //nzs, izs
        inprea.skipRawData(new_line);
        m_prea.nzs = read1DimArrayInt(inprea, m_prea.numzsd);
        m_prea.izs = read3DimArrayInt(inprea, m_prea.numzsd,m_prea.mxzs,m_prea.nizs);
        //dzs
        inprea.skipRawData(new_line);
        m_prea.dzs = read3DimArrayFloat(inprea, m_prea.numzsd,m_prea.mxzs,m_prea.ndzs);
        //dwz2
        inprea.skipRawData(new_line);
        m_prea.dwz2 = read2DimArrayFloat(inprea, model.numwzd,m_prea.ndwz2);
        //icrf
        inprea.skipRawData(new_line);
        m_prea.icrf = read1DimArrayInt(inprea, m_prea.nicrf);

        //coefis, nameis, nkcis, ikcis, ckcis
        inprea.skipRawData(new_line);
        m_prea.coefis = read1DimArrayDouble(inprea, m_prea.numisd);
        m_prea.nameis = readStringList(inprea, 10,m_prea.numisd);
        m_prea.nkcis = read1DimArrayInt(inprea, m_prea.numisd);
        m_prea.ikcis = read2DimArrayInt(inprea, m_prea.numisd,m_prea.mxkcis);
        m_prea.ckcis = read2DimArrayDouble(inprea, m_prea.numisd,m_prea.mxkcis);

        //coefqs,nameqs,nkcqs,ikcqs,ckcqs
        inprea.skipRawData(new_line);
        m_prea.coefqs = read1DimArrayDouble(inprea, m_prea.numqsd);
        m_prea.nameqs = readStringList(inprea, 10,m_prea.numqsd);
        m_prea.nkcqs = read1DimArrayInt(inprea, m_prea.numqsd);
        m_prea.ikcqs = read2DimArrayInt(inprea, m_prea.numqsd,m_prea.mxkcqs);
        m_prea.ckcqs = read2DimArrayDouble(inprea, m_prea.numqsd,m_prea.mxkcqs);
        //rstppi,rstbbi,rstbyi
        inprea.skipRawData(new_line);
        m_prea.rstppi = read3DimArrayDouble(inprea, model.numpd,m_prea.npdim,m_prea.nrstpp);
        m_prea.rstbbi = read2DimArrayDouble(inprea, model.numbd,m_prea.nrstbb);
        m_prea.rstbyi = read2DimArrayDouble(inprea, model.numbyd,m_prea.nrstby);
        inprea.skipRawData(new_line);
        inprea.skipRawData(new_line);
        //
        //iban
        inprea.skipRawData(new_line);
        m_prea.iban = read3DimArrayInt(inprea, m_prea.numhgd,model.nij,m_prea.niban);
        //rban
        inprea.skipRawData(new_line);
        m_prea.rban = read3DimArrayFloat(inprea, m_prea.numhgd,model.nij,m_prea.nrban);
        //dmm6
        inprea.skipRawData(new_line);
        m_prea.dmm6 = read2DimArrayFloat(inprea,model.nummd,m_prea.ndmm6);
        //imm4
        inprea.skipRawData(new_line);
        m_prea.imm4 = read2DimArrayInt(inprea,model.nummd,m_prea.nimm4);
        //dmm21
        //dww31
        //iban3
        //rban3
        //jban
        //tban
        //jban3
        //tban3
        //jban2
        //tban2
        //ifib1
        //ifib3
        //rfib3
        //jban4
        //tban4
        //ibanw
        //rbanw
        //sbanw
        //jbanw
        //tbanw
        //rnmw
        //rstn
        //rstm
        //ptdd
        //jg1con
        //dg1con
        //jg1con
        //dg1cos

        prea.insert(modelname,m_prea);
    }
    return READING_STATUS::STATUS_SUCCESS;
}

READING_STATUS handleNPstn::readBLD3D_CHCK(DataNBLD3D_CHCK &chck, const DataNBLD3D_CMDL &model) const
{
    chck.clear();
    if (toDecompressPstn) {
        UnzipPstn("BLD3D_CHCK", u8"CHCKの解凍");
    }
    //filepath
    QFile fchck(filepath().absolutePath() + "/" + tmpdirname() + "/BLD3D_CHCK");
    bool successFileOpen = fchck.open(QIODevice::ReadOnly);
    if (successFileOpen == false){
        return READING_STATUS::STATUS_NOTEXIST;
    }
    QDataStream inchck(&fchck);
    inchck.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BigEndian

    inchck.skipRawData(head + 80 + new_line); // ICARD*80
    inchck >> chck.numch >> chck.mxrsch;

    inchck.skipRawData(new_line);
    chck.namech = readStringList(inchck,10,chck.numch);
    chck.nrsch = read1DimArrayInt(inchck,chck.numch);
    chck.irsch = read2DimArrayInt(inchck,chck.numch,chck.mxrsch);
    chck.corsch = read2DimArrayDouble(inchck,chck.numch,chck.mxrsch);
    chck.nmrsch = read2DimStringList(inchck,10,chck.numch,chck.mxrsch);

    inchck.skipRawData(new_line);
    chck.rstmmc = read4DimArrayFloat(inchck,chck.numch,model.nummd,chck.nchp,chck.nrstmc);

    inchck.skipRawData(new_line);
    chck.rstmma = read4DimArrayFloat(inchck,chck.numch,model.nummd,chck.nchp,chck.nrstma);

    //壁のせん断余裕率Qd/Qa
    inchck.skipRawData(new_line);
    chck.rstwwc = read3DimArrayFloat(inchck,chck.numch,model.numwwd,chck.nrstwc);

    inchck.skipRawData(new_line);
    chck.rstwwa = read3DimArrayFloat(inchck,chck.numch,model.numwwd,chck.nrstwa);

    inchck.skipRawData(new_line);
    chck.ich = read2DimArrayInt(inchck,chck.numch,chck.nich);

    inchck.skipRawData(new_line);
    chck.cupw50 = read2DimArrayFloat(inchck,chck.numch,model.nummd);

    inchck.skipRawData(new_line);
    chck.immch = read3DimArrayInt(inchck,chck.numch,model.nummd,chck.nimmch);

    inchck.skipRawData(new_line);
    chck.ippch = read4DimArrayInt(inchck,chck.numch,model.numj,model.nyz,chck.nippch);

    inchck.skipRawData(new_line);
    chck.dppch = read4DimArrayFloat(inchck,chck.numch,model.numj,model.nyz,chck.ndppch);

    inchck.skipRawData(new_line);
    chck.iopch = read3DimArrayInt(inchck,chck.numch,model.numwwd,chck.niopch);

    inchck.skipRawData(new_line);
    chck.dopch = read3DimArrayFloat(inchck,chck.numch,model.numwwd,chck.ndopch);

    inchck.skipRawData(new_line);
    chck.tnsec = read4DimArrayFloat(inchck,chck.numch,model.nummd,model.nicj,chck.nrstn);

    inchck.skipRawData(new_line);
    chck.tmsec = read6DimArrayFloat(inchck,chck.numch,model.nummd,model.nicj,chck.nrstm,chck.nrstn,model.nxy);

    inchck.skipRawData(new_line);
    chck.icoff = read3DimArrayInt(inchck,chck.numch,model.nummd,chck.nicoff);

    inchck.skipRawData(new_line);
    chck.dcoff = read3DimArrayFloat(inchck,chck.numch,model.nummd,chck.ndcoff);

    inchck.skipRawData(new_line);
    chck.isscb = read3DimArrayInt(inchck,chck.numch,model.numlbd,chck.nisscb);

    inchck.skipRawData(new_line);
    chck.dsscb = read3DimArrayFloat(inchck,chck.numch,model.numlbd,chck.ndsscb);

    inchck.skipRawData(new_line);
    chck.dsrcb = read3DimArrayFloat(inchck,chck.numch,model.numlbd,chck.ndsrcb);

    qDebug() << "read CHCK";

    fchck.close();
    return READING_STATUS::STATUS_SUCCESS;
}

READING_STATUS handleNPstn::readBLD3D_MODE(DataNBLD3D_MODE &mode) const
{
    mode.clear();
    if (toDecompressPstn) {
        UnzipPstn("BLD3D_MODE", u8"MODEの解凍");
    }

    QFile fmode(filepath().absolutePath() + "/" + tmpdirname() + "/BLD3D_MODE");
    bool successFileOpen = fmode.open(QIODevice::ReadOnly);
    if (successFileOpen == false) {
        return READING_STATUS::STATUS_NOTEXIST;
    };

    QTextStream in(&fmode);
    QString line;

    line = in.readLine();
    mode.nummode = line.leftRef(10).toInt();
    in.readLine();//空読み

    line = in.readLine();
    mode.totalMass << line.midRef(0, 15).toDouble();
    mode.totalMass << line.midRef(15, 15).toDouble();
    mode.totalMass << line.midRef(30, 15).toDouble();
    mode.totalMass << line.midRef(45, 15).toDouble();
    mode.totalMass << line.midRef(60, 15).toDouble();
    mode.totalMass << line.midRef(75, 15).toDouble();

    for (int i = 0; i < mode.nummode; i++) {
        line = in.readLine();
        mode.numj = line.midRef(10, 10).toInt();
        mode.naturalPeriod << line.midRef(20, 15).toDouble();
        QVector<qreal> beta;
        beta << line.midRef(50, 15).toDouble() << line.midRef(65, 15).toDouble() << line.midRef(80, 15).toDouble()
             << line.midRef(95, 15).toDouble() << line.midRef(110, 15).toDouble() << line.midRef(125, 15).toDouble();
        mode.beta << beta;

        QVector<XYZ> xyzlist;
        for (int j = 0; j < mode.numj; j++) {
            line = in.readLine();
            xyzlist << XYZ(line.midRef(0, 15).toDouble(), line.midRef(15, 15).toDouble(), line.midRef(30,15).toDouble());
        }
        //QVector<QList<XYZ> > mode; //mode[nummode][jno]
        mode.mode << xyzlist;

    }
    return READING_STATUS::STATUS_SUCCESS;
}
void handleNPstn::makeUnifiedData()
{
    UnifiedFixedData::getInstance()->getPstnData()->setReadStatus(pstnReadStatus);
    floorNoToUuid.clear();frameGroupNoToUuid.clear();frameNoToUuid.clear();
    jointNoToUuid.clear();memberNoToUuid.clear();wallNoToUuid.clear();
    slabNoToUuid.clear();damperNoToUuid.clear();
    springNoToUuid.clear();springTable.clear();planeTable.clear();

    if (pstnReadStatus.value(OFILE_MODEL)!=READING_STATUS::STATUS_SUCCESS)return;
    qDebug()<<"register model";
    UnifiedFixedData::getInstance()->getPstnData()->setCmdlData(model);
    UnifiedFixedData::getInstance()->getPstnData()->setCurrentFile(filename);
    registerFloors();
    registerSectionValues();
    //開口は定義が固まった後で改めて直します
    //registerOpenings();

    //通りグループ
    QList<FRAMEGROUPVALUE> frgrouplist;
    QList<FRAMEPOINTSVALUE> frlist;
    //    QHash<int, qreal> minimumcoordinate;

    registerFrames(frgrouplist,frlist);
    registerJoints();
    fixFrameMinMaxValues(frgrouplist,frlist);//節点座標の最大最小を参照して、通りの延長長さなどを修正している
    registerLineMembers();
    registerPlaneMembers();
    registerSprings();


    qDebug()<<"register output";
    QStringList loadcase;
    loadcase << "" << this->getStressCase();
    UnifiedFixedData::getInstance()->getPstnData()->setStressCaseList(loadcase);

    //Uuidと部材番号(npstnとp3dそれぞれ)の対応を登録
    qDebug()<<"register uuid table";
    //    if (pstnReadStatus.value(OFILE_RSTQD)==READING_STATUS::STATUS_PARTLYSUCCESS
    //            || pstnReadStatus.value(OFILE_RSTQD)==READING_STATUS::STATUS_SUCCESS){
    registerUuidIndexTable();
    //    }


    //モデル（PREA）の登録
    if(pstnReadStatus.value(OFILE_PREA)==READING_STATUS::STATUS_SUCCESS||pstnReadStatus.value(OFILE_PREA)==READING_STATUS::STATUS_PARTLYSUCCESS){
        UnifiedFixedData::getInstance()->getPstnData()->setPreaData(prea);
    }

    //荷重の登録
    if (pstnReadStatus.value(OFILE_LOAD)==READING_STATUS::STATUS_SUCCESS){
        UnifiedFixedData::getInstance()->getPstnData()->setLoadData(load);
    }


    //応力・変位の読み込み
    if((pstnReadStatus.value(OFILE_RST)==READING_STATUS::STATUS_SUCCESS ||pstnReadStatus.value(OFILE_RST)==READING_STATUS::STATUS_PARTLYSUCCESS)
            && pstnReadStatus.value(OFILE_LOAD)==READING_STATUS::STATUS_SUCCESS){
        //jointOutput
        qDebug()<<"register joint output";
        registerJointOutput(loadcase);
        //MemberOutput
        qDebug()<<"register member output";
        registerMemberOutput(loadcase);
        UnifiedFixedData::getInstance()->getPstnData()->setNbldRstData(rst);
    }

    //QUDS
    if(pstnReadStatus.value(OFILE_QUDS)==READING_STATUS::STATUS_SUCCESS){
        qDebug()<<"register quds data";
        registerQuDs();
        UnifiedFixedData::getInstance()->getPstnData()->setNbldQuDsData(quds);
    }


    //RSTQD
    if(pstnReadStatus.value(OFILE_RSTQD)==READING_STATUS::STATUS_PARTLYSUCCESS
            ||pstnReadStatus.value(OFILE_RSTQD)==READING_STATUS::STATUS_SUCCESS){
        qDebug()<<"register rstqd data";
        UnifiedFixedData::getInstance()->getPstnData()->setNbldRstQDData(rstqd);
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU1QD").nameam;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU1QD").nmrsu;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU1QD").nameis;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU1QD").nameqs;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU1QD").castep;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU1QD").coefis;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU1QD").iend;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU1QD").lqustep;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU2QD").nameam;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU2QD").nmrsu;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU2QD").nameis;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU2QD").nameqs;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU2QD").castep;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU2QD").coefis;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU2QD").iend;
        qDebug()<<UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value("RSTU2QD").lqustep;
    }

    //mode
    if (pstnReadStatus.value(OFILE_MODE)==READING_STATUS::STATUS_SUCCESS) {
        qDebug()<<"register mode";
        registerMode();
    }

    //CHCK
    if (pstnReadStatus.value(OFILE_CHCK)==READING_STATUS::STATUS_SUCCESS){
        qDebug()<<"register chck";
        registerChck();
        UnifiedFixedData::getInstance()->getPstnData()->setNbldChckData(chck);
    }

    //GENE
    if (pstnReadStatus.value(OFILE_GENE)==READING_STATUS::STATUS_SUCCESS){
        qDebug()<<"register gene";
        UnifiedFixedData::getInstance()->getPstnData()->setNbldGeneData(gene);
    }

    //STEC
    if (pstnReadStatus.value(OFILE_STEC)==READING_STATUS::STATUS_SUCCESS){
        qDebug()<<"register stec";
        UnifiedFixedData::getInstance()->getPstnData()->setNbldStecData(stec);
    }

    //    //NMIN
    //    if (pstnReadStatus.value(OFILE_NMIN)==READING_STATUS::STATUS_SUCCESS){
    //        qDebug()<<"register nmin";
    //        UnifiedFixedData::getInstance()->getPstnData()->setNbldNminData(nmin);
    //    }

}
void handleNPstn::registerQuDs(){
    QHash<QString,QUDSDATA> qudslist;
    for(int i=0;i<quds.numdqd;i++){
        QUDSDATA qd;
        qd.fes = quds.casefes.at(i);
        qd.angle = quds.angldq.at(i);
        qd.comment = quds.commdq.at(i);
        qd.direction = quds.kdir.at(i);
        qd.quFileName = quds.casedq.at(i).at(0);
        qd.dsFileName = quds.casedq.at(i).at(1);
        QString str = quds.cnamedq.at(i);
        //        qDebug()<<str<<qd.angle<<qd.fes<<qd.comment<<qd.direction<<qd.quFileName<<qd.dsFileName;
        qudslist.insert(str,qd);
    }
    UnifiedFixedData::getInstance()->getPstnData()->setQuDsData(qudslist);
}


void handleNPstn::registerLineMembers(){
    qDebug()<<"registerLineMembers init()";
    //部材
    for (int numm = 0; numm < this->model.numm; numm++) {
        //JOINT
        QList<JointData *> jdlist;
        //JointData i
        int jno_i = this->model.indm[numm][0] - 1;
        QUuid ijid = jointNoToUuid[jno_i];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(ijid));

        //JointData j
        int jno_j = this->model.indm[numm][1] - 1;
        QUuid jjid = jointNoToUuid[jno_j];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jjid));

        //SECTION
        int ils = this->model.imm1[numm][0] - 1;
        if(ils<0)continue;
        QString secname = this->model.namels[ils].trimmed();
        QStringList columnnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPCOLUMN);
        QStringList girdernamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPGIRDER);
        QStringList bracenamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPBRACE);
        QStringList wallnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPWALL);

        SectionValues sv;
        qreal hh=jdlist.first()->zPos();//i端の高さ[m]
        if (columnnamelist.contains(secname)){
            sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPCOLUMN,secname,hh);
        }else if (girdernamelist.contains(secname)){
            sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPGIRDER, secname,hh);
        }else if (bracenamelist.contains(secname)){
            sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPBRACE,secname,hh);
        }else if (wallnamelist.contains(secname)){
            sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPWALL,secname,hh);
        }else{
            sv = SectionValues();
        }
        qDebug()<<"define sectionvalues";

        DATATYPE dtyp;
        if (sv.memberType == DATATYPE::TPCOLUMN)dtyp = DATATYPE::TPCOLUMN;
        else if (sv.memberType == DATATYPE::TPGIRDER)dtyp = DATATYPE::TPGIRDER;
        else if (sv.memberType == DATATYPE::TPBRACE)dtyp = DATATYPE::TPBRACE;
        else if (sv.memberType == DATATYPE::TPWALL)dtyp = DATATYPE::TPWALL;
        else dtyp = DATATYPE::TPCOLUMN;


        QUuid uid = QUuid::createUuid();
        UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jdlist, dtyp, secname, uid );
        UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid)->setFaceValues(
                    true, 1, true,this->model.dmm1[numm][6] * 1000,
                true, 1, true, this->model.dmm1[numm][7] * 1000);
        qDebug()<<"create member";
        foreach(QString model,modelcase){
            PREADATA* pa = new PREADATA;
            pa->phaiIy=prea.value(model).dmm2[numm][1];
            pa->EIy=prea.value(model).dmm2[numm][14];
            pa->phaiIz=prea.value(model).dmm2[numm][2];
            pa->EIz=prea.value(model).dmm2[numm][15];
            UnifiedFixedData::getInstance()->getPstnData()->setPrea(model,uid,pa);
        }
        qDebug()<<"set prea";

        UnifiedFixedData::getInstance()->getPstnData()->setModelCaseList(modelcase);
        qDebug()<<"set modelcaselist";

        PREMDATA* pm = new PREMDATA;
        pm->Lky=model.dmm1[numm][20];
        pm->Lkz=model.dmm1[numm][21];
        UnifiedFixedData::getInstance()->getPstnData()->setPrem(uid,pm);
        qDebug()<<"set prem";

        memberNoToUuid[numm] = uid;
        //code angle
        qreal betaangle;//pos4参照 rad.
        QVector3D vecX, vecY, vecZ, unitZ, dvecY, dvecZ;
        qreal cosbeta, sinbeta;
        vecX.setX(this->model.evmem[numm][0][0]);
        vecX.setY(this->model.evmem[numm][0][1]);
        vecX.setZ(this->model.evmem[numm][0][2]);
        vecY.setX(this->model.evmem[numm][1][0]);
        vecY.setY(this->model.evmem[numm][1][1]);
        vecY.setZ(this->model.evmem[numm][1][2]);
        vecZ = QVector3D::crossProduct(vecX, vecY);
        unitZ.setX(0.0);
        unitZ.setY(0.0), unitZ.setZ(1.0);
        const qreal eps = 0.001; //鉛直かどうかの閾値
        if (vecX.x()*vecX.x() + vecX.y()*vecX.y() < eps) {
            betaangle = atan2(vecY.y(), vecY.x());
        } else {
            dvecY = QVector3D::crossProduct(unitZ, vecX);
            dvecY.normalize();
            dvecZ = QVector3D::crossProduct(vecX, dvecY);
            cosbeta = QVector3D::dotProduct(dvecY, vecY);
            sinbeta = QVector3D::dotProduct(dvecZ, vecY);
            betaangle = atan2(sinbeta, cosbeta);
        }
        qDebug()<<"before set codeangle";

        MemberData* md=UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);
        md->setCodeAngle(betaangle);
        md->recalcLocalVector();

    }
    //材端ばね
    for(int nummt=0;nummt<model.nummt;nummt++){
        int numm = model.imt[nummt][0]-1;
        QUuid uid = memberNoToUuid[numm];
        MemberData* md=UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);

        QString iyBaneName = QString::number(model.imt[nummt][5]);
        QString izBaneName = QString::number(model.imt[nummt][6]);
        QString jyBaneName = QString::number(model.imt[nummt][7]);
        QString jzBaneName = QString::number(model.imt[nummt][8]);
        if(iyBaneName==u8"0")iyBaneName=QString();
        if(izBaneName==u8"0")izBaneName=QString();
        if(jyBaneName==u8"0")jyBaneName=QString();
        if(jzBaneName==u8"0")jzBaneName=QString();
        qDebug()<<"before set bane name";
        md->setFaceBaneName(iyBaneName,izBaneName,jyBaneName,jzBaneName);
        qDebug()<<"done set bane name";
    }

    //ダンパー
    for (int numsdp = 0; numsdp < this->model.numsdp; numsdp++) {
        //JOINT
        QList<JointData *> jdlist;
        //JointData i
        int jno_i = this->model.isdp[numsdp][0] - 1;
        QUuid ijid = jointNoToUuid[jno_i];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(ijid));

        //JointData j
        int jno_j = this->model.isdp[numsdp][1] - 1;
        QUuid jjid = jointNoToUuid[jno_j];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jjid));

        if(model.isdp[numsdp][4]==3){//link
            //JointData k
            int jno_k = this->model.isdp[numsdp][3] - 1;
            QUuid kjid = jointNoToUuid[jno_k];
            jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(kjid));
        }

        //SECTION
        int il = this->model.isdp[numsdp][5] - 1;
        if(il<0)continue;
        QString secname = this->model.namedl[il].trimmed();
        QStringList dampernamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPDAMPER);

        SectionValues sv;
        qreal hh=jdlist.first()->zPos();//i端の高さ[m]
        if (dampernamelist.contains(secname)){
            sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPDAMPER,secname,hh);
        }else{
            sv = SectionValues();
        }
        qDebug()<<"define sectionvalues";

        DATATYPE dtyp = DATATYPE::TPDAMPER;


        QUuid uid = QUuid::createUuid();
        UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jdlist, dtyp, secname, uid );


        damperNoToUuid[numsdp] = uid;
        //code angle
        qreal betaangle;//pos4参照 rad.
        QVector3D vecX, vecY, vecZ, unitZ, dvecY, dvecZ;
        qreal cosbeta, sinbeta;


        XYZ vec = jdlist.at(1)->xyz()-jdlist.at(0)->xyz();
        vecX.setX(vec.x);
        vecX.setY(vec.y);
        vecX.setZ(vec.z);
        vecY.setX(model.ddl[il][16]);
        vecY.setY(model.ddl[il][17]);
        vecY.setZ(model.ddl[il][18]);
        vecZ = QVector3D::crossProduct(vecX, vecY);
        unitZ.setX(0.0);
        unitZ.setY(0.0), unitZ.setZ(1.0);
        const qreal eps = 0.001; //鉛直かどうかの閾値
        if (vecX.x()*vecX.x() + vecX.y()*vecX.y() < eps) {
            betaangle = atan2(vecY.y(), vecY.x());
        } else {
            dvecY = QVector3D::crossProduct(unitZ, vecX);
            dvecY.normalize();
            dvecZ = QVector3D::crossProduct(vecX, dvecY);
            cosbeta = QVector3D::dotProduct(dvecY, vecY);
            sinbeta = QVector3D::dotProduct(dvecZ, vecY);
            betaangle = atan2(sinbeta, cosbeta);
        }
        qDebug()<<"before set codeangle";


        MemberData* md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);
        md->setCodeAngle(betaangle);
        md->recalcLocalVector();
    }


}
void handleNPstn::registerPlaneMembers(){
    //耐震壁
    for (int numww = 0; numww < this->model.numww; numww++) {
        //中間節点含む節点
        //        int nwjnt = this->model.iww1[numww][3]; //周囲節点数
        //        QList<JointData *> jlist;
        //        for (int jnt = 0; jnt < nwjnt; jnt++) {
        //            int jno = this->model.iww2[numww][jnt] - 1;
        //            jlist.append(UnifiedAnalysisData::getOutInstance()->jointIDToPointer(jointNoToUuid[jno]));
        //        }

        //周囲4点のみ
        int nwjnt = 4;
        QList<JointData*> jlist;
        for (int jnt = 0; jnt < nwjnt; jnt++) {
            int jno = this->model.iww1[numww][5+jnt] - 1;
            jlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jointNoToUuid[jno]));
        }


        QString secname = this->model.namesw[this->model.iww1[numww][1] - 1].trimmed();

        QStringList wallnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPWALL);
        int direction = this->model.iww1[numww][4]; //1:WI,2:WH,3:SI,4:SH

        SectionValues sv;
        QUuid uid = QUuid::createUuid();
            if (wallnamelist.contains(secname)){
                sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPWALL,secname);
                UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jlist, DATATYPE::TPWALL, secname, uid );
            }else{
                continue;
            }
        MemberData *data = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);


        data->setSectionName(*UnifiedFixedData::getInstance()->getAnalysisData(), sv.sectionName);
        data->setPlaneModel(PLANEMODEL::WALL_MODEL);
        int op = this->model.iww1[numww][2] - 1; //開口番号
        if (op >= 0)data->setWallOpen(this->model.nameso[op]);

        //sectionNameToPlanePropertyなどでは断面の方からデータを引いているのでそちらにも登録必要だが、雑壁か耐震壁かは部材に紐づくべき？
        //                sv.plane_param.planeType=PlaneType::WALL;
        //        int direction = this->model.iww1[numww][4]; //1:WI,2:WH,3:SI,4:SH
        switch (direction) {
        case 1:
            data->setPlaneSubModel(PLANESUBMODEL::WI_MODEL);
            break;
        case 2:
            data->setPlaneSubModel(PLANESUBMODEL::WH_MODEL);
            break;
        case 3:
            data->setPlaneSubModel(PLANESUBMODEL::SI_MODEL);
            break;
        case 4:
            data->setPlaneSubModel(PLANESUBMODEL::SH_MODEL);
            break;
        default:
            data->setPlaneSubModel(PLANESUBMODEL::NONSTRUCT_SUBMODEL);
            break;
        }
        wallNoToUuid[numww] = uid;
        planeTable.insert(uid,QPair<int,B3D_DATATYPE>(numww,B3D_DATATYPE::B3D_TPWALL));
        foreach(QString model,modelcase){
            PREADATA* pa = new PREADATA;
            pa->lenBeamB=prea.value(model).debww[numww][0];
            pa->lenBeamU=prea.value(model).debww[numww][1];
            pa->lenColumn=prea.value(model).debww[numww][2];
            pa->phaiB=prea.value(model).debww[numww][12];
            pa->phaiU=prea.value(model).debww[numww][13];
            UnifiedFixedData::getInstance()->getPstnData()->setPrea(model,uid,pa);
        }

        VEC3D vecX, vecY, vecZ;
        vecX = VEC3D(this->model.evmemw[numww][0][0],this->model.evmemw[numww][0][1],this->model.evmemw[numww][0][2]);
        vecY = VEC3D(this->model.evmemw[numww][1][0],this->model.evmemw[numww][1][1],this->model.evmemw[numww][1][2]);
        vecZ = CalcVector3D::crossNormalized(vecX, vecY);
        UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid)->setDirectionVector(vecX,vecY);

    }
    //雑壁
    for (int numwz = 0; numwz < this->model.numwz; numwz++) {
        int nwjnt = this->model.iwz1[numwz][3]; //周囲節点数
        QList<JointData *> jlist;
        for (int jnt = 0; jnt < nwjnt; jnt++) {
            int jno = this->model.iwz2[numwz][jnt] - 1;
            jlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jointNoToUuid[jno]));
        }
        QString secname = this->model.namesw[this->model.iwz1[numwz][1] - 1].trimmed();
        QStringList wallnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPWALL);
        SectionValues sv;
        if (wallnamelist.contains(secname))sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPWALL,
                                                                                                                     secname);
        else continue;
        //
        QUuid uid = QUuid::createUuid();
        UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jlist, DATATYPE::TPWALL, secname, uid );
        MemberData *data = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);
        //
        data->setSectionName(*UnifiedFixedData::getInstance()->getAnalysisData(), sv.sectionName);
        int op = this->model.iwz1[numwz][2] - 1; //開口番号
        if (op >= 0)data->setWallOpen(this->model.nameso[op]);
        data->setPlaneModel(PLANEMODEL::ZWALL_MODEL);
        //                sv.plane_param.planeType=PlaneType::ZWALL;

        wallNoToUuid[numwz+model.numww] = uid;
        planeTable.insert(uid,QPair<int,B3D_DATATYPE>(numwz,B3D_DATATYPE::B3D_TPZWALL));


    }
    qDebug() << "bracecount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPBRACE);
    qDebug() << "wallcount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPWALL);

    //平面応力要素
    //平板シェル要素
    for (int numws = 0; numws < this->model.numwsd; numws++) {
        if(this->model.namesw.count()==0)break;
        if(this->model.iws1[numws][1]==0)break;
        //qDebug()<<"IWS1 type,numj,name="<<this->model.iws1[numws][2]<<this->model.iws1[numws][3]<<this->model.namesw[this->model.iws1[numws][1] - 1].trimmed();
        QList<JointData *> jlist;
        for(int i=0;i<this->model.iws1[numws][3];i++){
            jlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jointNoToUuid[this->model.iws1[numws][4+i] -1]));
        }
        QString secname = this->model.namesw[this->model.iws1[numws][1] - 1].trimmed();

        if(this->model.iws1[numws][2] == 1 || this->model.iws1[numws][2] == 11)//スラブ
        {
            QStringList slabnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPSLAB);
            SectionValues sv;
            if (slabnamelist.contains(secname)){
                sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPSLAB,secname);
            }else{
                continue;
            }
            //
            QUuid uid = QUuid::createUuid();
            UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jlist, DATATYPE::TPSLAB, secname, uid );
            MemberData *data = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);

            data->setSectionName(*UnifiedFixedData::getInstance()->getAnalysisData(), sv.sectionName);
            data->setPlaneModel(PLANEMODEL::SLAB_MODEL);
            slabNoToUuid[numws] = uid;
            planeTable.insert(uid,QPair<int,B3D_DATATYPE>(numws,B3D_DATATYPE::B3D_TPPLANE_WS));
            if(this->model.iws1[numws][2] == 1){
                data->setPlaneSubModel(PLANESUBMODEL::MEMBRANE_MODEL);
            }else if(this->model.iws1[numws][2] == 11){

                data->setPlaneSubModel(PLANESUBMODEL::PLATE_MODEL);
            }
        }
        if(this->model.iws1[numws][2] == 2 || this->model.iws1[numws][2] == 12)//壁
        {
            QStringList namelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPWALL);
            SectionValues sv;
            if (namelist.contains(secname)){
                sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPWALL,secname);
            }else{
                continue;
            }
            //
            QUuid uid = QUuid::createUuid();
            UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jlist, DATATYPE::TPWALL, secname, uid );
            MemberData *data = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);

            data->setSectionName(*UnifiedFixedData::getInstance()->getAnalysisData(), sv.sectionName);
            data->setPlaneModel(PLANEMODEL::WALL_MODEL);
            wallNoToUuid[numws+model.numww+model.numwz] = uid;
            planeTable.insert(uid,QPair<int,B3D_DATATYPE>(numws,B3D_DATATYPE::B3D_TPPLANE_WS));
            if(this->model.iws1[numws][2] == 2){
                data->setPlaneSubModel(PLANESUBMODEL::MEMBRANE_MODEL);
            }else if(this->model.iws1[numws][2] == 12){
                data->setPlaneSubModel(PLANESUBMODEL::PLATE_MODEL);
            }
        }


    }
    qDebug() << "slabcount(plus WS)=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPSLAB);
    qDebug() << "wallcount(plus WS)=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPWALL);

}
void handleNPstn::registerSprings(){
//    bool isISOMss = false;
    //
    QList<QList<JointData*>> bn_banelist;
    QList<QUuid> bn_baneidlist;
    //軸ばね
    for (int numbn = 0; numbn < this->model.numbn; numbn++) {

        QString springname = QString();
        springname = u8"BN"+QString::number(numbn+1).rightJustified(3,'0');

        int springNo = this->model.ibn[numbn][3];//NUMB
        //        int springCharacteristic =   model.jbbcon[springNo-1];
        QString yBaneName = QString::number(springNo);

        B3D_DATATYPE type = B3D_DATATYPE::B3D_TPSPRING_N;
//        DATATYPE datatype = DATATYPE::TPSPRING;

        QList<JointData *> jdlist;
        int jno;
        //JointData i
        jno = this->model.ibn[numbn][0] - 1;
        QUuid ijid = jointNoToUuid[jno];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(ijid));

        //JointData j
        jno = this->model.ibn[numbn][1] - 1;
        QUuid jjid = jointNoToUuid[jno];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jjid));

        QUuid uid = QUuid::createUuid();

        VEC3D vecx = VEC3D(model.dbn[numbn][0],model.dbn[numbn][1],model.dbn[numbn][2]);
        VEC3D vecy = VEC3D();
        UnifiedFixedData::getInstance()->getAnalysisData()->createSpring(jdlist, type, springname,uid,
                                                                         vecx,vecy,yBaneName,QString(),false);
        bn_banelist.append(jdlist);
        bn_baneidlist.append(uid);

        springNoToUuid.insert(springNo, uid);
        springTable.insert(uid,QPair<int,B3D_DATATYPE>(numbn,type));
    }
    //せん断ばね
    for (int numbq = 0; numbq < this->model.numbq; numbq++) {

        QString springname = QString();
        springname = u8"BQ"+QString::number(numbq+1).rightJustified(3,'0');

        int springNo = this->model.ibq[numbq][3];
        //        int springCharacteristic =   model.jbbcon[springNo-1];
        QString yBaneName = QString::number(springNo);

        B3D_DATATYPE type = B3D_DATATYPE::B3D_TPSPRING_Q;

        QList<JointData *> jdlist;
        int jno;
        //JointData i
        jno = this->model.ibq[numbq][0] - 1;
        QUuid ijid = jointNoToUuid[jno];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(ijid));

        //JointData j
        jno = this->model.ibq[numbq][1] - 1;
        QUuid jjid = jointNoToUuid[jno];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jjid));

        QUuid uid = QUuid::createUuid();

        VEC3D vecx = VEC3D(model.dbq[numbq][0],model.dbq[numbq][1],model.dbq[numbq][2]);
        VEC3D vecy = VEC3D(model.dbq[numbq][3],model.dbq[numbq][4],model.dbq[numbq][5]);
        UnifiedFixedData::getInstance()->getAnalysisData()->createSpring(jdlist, type, springname,uid,
                                                                         vecx,vecy,yBaneName,QString(),false);

        springNoToUuid.insert(springNo, uid);
        springTable.insert(uid,QPair<int,B3D_DATATYPE>(numbq,type));
    }
    qDebug()<<"register MSS in";
    //MSSばね
    for (int numby = 0; numby < this->model.numby; numby++) {

        QString springname = QString();
        springname = u8"BY"+QString::number(numby+1).rightJustified(3,'0');

        //MSSは2方向ある。これだけAB番号なので、怪しい？表記が間違っている？
        //        int s1 = this->model.iby[numby][2];//NUMAB
        //        int springCharacteristicY =   model.iab[s1-1][1];
        //        int s2 = this->model.iby[numby][3];//NUMAB
        //        int springCharacteristicZ =   model.iab[s2-1][1];
        int springNoY = this->model.iby[numby][2];
        //        int springCharacteristicY =   model.jbbcon[springNoY-1];
        int springNoZ = this->model.iby[numby][3];
        //        int springCharacteristicZ =   model.jbbcon[springNoZ-1];
        QString yBaneName = QString::number(springNoY);
        QString zBaneName = QString::number(springNoZ);


        B3D_DATATYPE type = B3D_DATATYPE::B3D_TPSPRING_MSS;

        QList<JointData *> jdlist;
        int jno;
        //JointData i
        jno = this->model.iby[numby][0] - 1;
        QUuid ijid = jointNoToUuid[jno];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(ijid));

        //JointData j
        jno = this->model.iby[numby][1] - 1;
        QUuid jjid = jointNoToUuid[jno];
        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jjid));

        QUuid uid = QUuid::createUuid();
        VEC3D vecx = VEC3D(model.dby[numby][0],model.dby[numby][1],model.dby[numby][2]);
        VEC3D vecy = VEC3D(model.dby[numby][3],model.dby[numby][4],model.dby[numby][5]);
        bool isISO = false;
        for(int i=0;i<bn_banelist.count();i++){
            if(bn_banelist.at(i).first()==jdlist.first() &&bn_banelist.at(i).last()==jdlist.last()){
                isISO=true;
                SpringData* sd=UnifiedFixedData::getInstance()->getAnalysisData()->springIDToPointer(bn_baneidlist.at(i));
                sd->setISOFlag(true);
                break;
            }
        }
        UnifiedFixedData::getInstance()->getAnalysisData()->createSpring(jdlist, type, springname,uid,
                                                                         vecx,vecy,yBaneName,zBaneName,isISO );
        springNoToUuid.insert(springNoY, uid);
        springNoToUuid.insert(springNoZ, uid);
        springTable.insert(uid,QPair<int,B3D_DATATYPE>(numby,type));
    }
    qDebug()<<"register MSS out";

    //曲げせん断ばね、回転ばね、T型剛域ばねが残り

}

void handleNPstn::fixFrameMinMaxValues(QList<FRAMEGROUPVALUE> &frgrouplist,QList<FRAMEPOINTSVALUE> &frlist){
    //全節点を当該framegroupに投影したときの座標を拾って最大最小を決める
    //計算量大のためもっと単純化できるか

    //    //真ん中を求める
    //    qreal xMin=99999.0;
    //    qreal xMax=-99999.0;
    //    qreal yMin=99999.0;
    //    qreal yMax=-99999.0;
    //    Q_FOREACH(JointData* jd,UnifiedFixedData::getInstance()->getAnalysisData()->jointListOfAll()){
    //        if(jd->xPos() < xMin)xMin = jd->xPos();
    //        if(jd->xPos() > xMax)xMax = jd->xPos();
    //        if(jd->yPos() < yMin)yMin = jd->yPos();
    //        if(jd->yPos() > yMax)yMax = jd->yPos();
    //    }
    //    QPointF G(0.5*(xMin+xMax),0.5*(yMin+yMax));

    for (int nfg = 0; nfg < frgrouplist.count(); nfg++) {
        FRAMEGROUPVALUE fgv = frgrouplist[nfg];
        qreal minus = 0.0;
        qreal plus = 0.0;
        foreach (FRAMEPOINTSVALUE frpv, frlist) {
            if (fgv.frameType == FrameType::FRAMETYPE::STRAIGHT && !frpv.straight.isFirst)continue;
            if (frpv.GID == fgv.GID) {
                QList<XYZ> globalxyzs;
                QList<JointData*> joints=UnifiedFixedData::getInstance()->getAnalysisData()->jointListOfFrame(frpv.UUID);
                for (int i = 0; i < joints.count(); i++) {
                    globalxyzs << joints.at(i)->xyz();
                }
                QList<XYZ> framexyzs = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameXYZ(frpv.UUID, globalxyzs);
                foreach (XYZ framexyz, framexyzs) {
                    if (framexyz.x < minus)minus = framexyz.x;
                    if (framexyz.x > plus)plus = framexyz.x;
                }
            }
        }
        frgrouplist[nfg].minusLength = -minus + 1.0;
        frgrouplist[nfg].plusLength = plus + 1.0;
    }

    QList<FRAMEPOINTSVALUE> fixedfrlist;
    qreal span = 0.0;
    //    bool normalOrder = true;
    for (int frnum = 0; frnum < frlist.count(); frnum++) {
        FRAMEPOINTSVALUE fr = frlist.at(frnum);
        FRAMEGROUPVALUE fgitem;
        FrameType::FRAMETYPE itype = fr.frameType;
        for (int i = 0; i < frgrouplist.count(); i++) {
            if (frgrouplist.at(i).GID == fr.GID) {
                fgitem = frgrouplist.at(i);
                break;
            }
        }
        QPointF npoint;
        QList<QPointF> plist;
        STRAIGHTFRAMEVALUE st_value;
        if (itype == FrameType::FRAMETYPE::STRAIGHT) {
            if (fr.straight.isFirst)span = 0.0;
            qreal m_len = fgitem.minusLength;
            qreal p_len = fgitem.plusLength;
            qreal theta = fgitem.startAngle / 180.0 * M_PI;
            QPointF po = fgitem.basePoint;
            QPointF spoint;
            QPointF p1, p2, p3, p4;
            //建物図心と通り芯の距離で、右左どっちに通りを描画するか決める
            //            QPointF p00 = G - QPointF(-m_len * qCos(theta) , -m_len * qSin(theta) ) + po;
            //            QPointF p01 = G - QPointF( p_len * qCos(theta) ,  p_len * qSin(theta) ) + po;
            //            bool migiuegawa = p00.manhattanLength()<=p01.manhattanLength() ;
            bool horizontal = qAbs(qCos(fgitem.startAngle/180.0*M_PI))>qAbs(qSin(fgitem.startAngle/180.0*M_PI));
            //            bool drawP2Side = true;
            //            if ( horizontal && migiuegawa){drawP2Side = true;normalOrder=true;}
            //            if ( !horizontal && !migiuegawa){drawP2Side = false;normalOrder=true;}
            //            if ( horizontal && !migiuegawa){drawP2Side = false;normalOrder=false;}
            //            if ( !horizontal && migiuegawa){drawP2Side = true;normalOrder=false;}
            if ( horizontal ) {
                p1 = QPointF(-m_len * qCos(theta) - span * qSin(theta),
                             -m_len * qSin(theta) + span * qCos(theta)) + po;
                p2 = QPointF( p_len * qCos(theta) - span * qSin(theta),
                              p_len * qSin(theta) + span * qCos(theta)) + po;
                npoint = CalcVector2D::extendPoint(p1, p2, 5.0);
                if (fr.straight.isLast) {
                    p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
                    p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                    spoint = CalcVector2D::extendPoint(p3, p4, 1.5);
                } else {
                    spoint = CalcVector2D::extendPoint(p1, p2, 0.5);
                }

            } else {
                p1 = QPointF(-m_len * qCos(theta) + span * qSin(theta),
                             -m_len * qSin(theta) - span * qCos(theta)) + po;
                p2 = QPointF( p_len * qCos(theta) + span * qSin(theta),
                              p_len * qSin(theta) - span * qCos(theta)) + po;
                npoint = CalcVector2D::extendPoint(p2, p1, 5.0);
                if (fr.straight.isLast) {
                    p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
                    p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                    spoint = CalcVector2D::extendPoint(p4, p3, 1.5);
                } else {
                    spoint = CalcVector2D::extendPoint(p2, p1, 0.5);
                }

            }
            plist.append(p1);
            plist.append(p2);
            st_value = STRAIGHTFRAMEVALUE(fr.straight.span, fr.straight.angle, spoint,
                                          fr.straight.isFirst, fr.straight.isLast);
            span += fr.straight.span;

        } else if (itype == FrameType::FRAMETYPE::CIRCLE) {
            plist = fr.pointList;
            npoint = fr.namePoint;

        } else if (itype == FrameType::FRAMETYPE::RADIUS) {

            QPointF po = fgitem.basePoint;
            qreal theta = atan2(fr.pointList.last().y() - fr.pointList.first().y(),
                                fr.pointList.last().x() - fr.pointList.first().x());
            qreal p_len = fgitem.plusLength;

            plist.append(po);
            QPointF p1 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
            plist.append(p1);

            npoint = CalcVector2D::extendPoint(po, p1, 3.0);

        } else if (itype == FrameType::FRAMETYPE::DIAMETER) {
        } else if (itype == FrameType::FRAMETYPE::VARIABLE) {
        } else if (itype == FrameType::FRAMETYPE::CLCIRCLE) {
        } else if (itype == FrameType::FRAMETYPE::CLVARIABLE) {
        }

        FRAMEPOINTSVALUE fmp = FRAMEPOINTSVALUE(fr.GID, plist, fr.frameName, npoint, itype, st_value,
                                                fr.UUID);
        //        if(normalOrder){
        fixedfrlist.append(fmp);
        //        }else{
        //            fixedfrlist.push_front(fmp);
        //        }
    }
    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(frgrouplist, fixedfrlist);
    qDebug()<<"grid register end";

}
void handleNPstn::registerJoints(){
    //Joint
    //節点


    jointNoToUuid.clear();
    QList<FLOORVALUES> floorlist = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList();
    QList<JointData *> jdlist;
    QList<QList<QUuid> > flooridlist;
    QList<QList<QUuid> > frameidlistlist;
    for (int jno = 0; jno < this->model.numj; jno++) {
        GlobalPoint gp(this->model.xyz[jno][0], this->model.xyz[jno][1], this->model.xyz[jno][2]);
        JointData *njd = UnifiedFixedData::getInstance()->getAnalysisData()->createAnalysisJoint(gp, this->model.jntnm[jno], true, QUuid::createUuid());
        njd->setBoundary(this->model.indj[jno]);
        QList<JOINTSPRING *> springs;
        for (int nbj = 0; nbj < this->model.ibj.count(); nbj++) {
            if (this->model.ibj.at(nbj).at(0) != jno + 1)continue;
            JOINTSPRING *js = new JOINTSPRING;
            js->uid = QUuid::createUuid();
            js->springNo = this->model.ibj.at(nbj).at(3);
            js->springVec = VEC3D(model.dbj.at(nbj).at(0), model.dbj.at(nbj).at(1), model.dbj.at(nbj).at(2));
            if (this->model.ibj.at(nbj).at(1) == 0) {
                js->isRotation = false;
            } else {
                js->isRotation = true;
            }
            springs.append(js);
            springNoToUuid.insert(js->springNo, js->uid);
        }
        njd->setJointSpring(springs);
        jointNoToUuid.insert(jno, njd->getUuid());
        jdlist.append(njd);
        //int floorgroup=this->model.jntfl[jno][0][0]-1;
        int floornumber = this->model.jntfl[jno][0][1] - 1;
        QUuid flid = floorNoToUuid[floornumber];
        QUuid frid;

        //古いコードそのままのため表現冗長。つまりはflidとfridのリストを作っているだけ
        QList<QUuid> frameidlist;
        foreach (FLOORVALUES fv, floorlist) {
            if (fv.UUID == flid) {
                QList<QUuid> dummylist;
                dummylist << fv.UUID;
                flooridlist << dummylist;
                QList<FRAMEPOINTSVALUE> framelist;
                framelist = UnifiedFixedData::getInstance()->getAnalysisData()->getFrameValuesOfFloor(fv.UUID);
                for (int gr = 0; gr < this->model.njgr[jno]; gr++) {
                    int gridgroup = this->model.jntgr[jno][gr][0] - 1;
                    int gridnumber = this->model.jntgr[jno][gr][1] - 1;
                    if (gridgroup != -1) {
                        frid = frameNoToUuid[gridgroup * 10000 + gridnumber];
                    }
                    foreach (FRAMEPOINTSVALUE fpv, framelist) {
                        if (fpv.UUID == frid)frameidlist << fpv.UUID;
                    }
                }
                break;
            }
        }
        frameidlistlist << frameidlist;

    }
    UnifiedFixedData::getInstance()->getAnalysisData()->changeJointsAttachment(jdlist, flooridlist, DATATYPE::TPFLOOR, true);
    UnifiedFixedData::getInstance()->getAnalysisData()->changeJointsAttachment(jdlist, frameidlistlist, DATATYPE::TPFRAMEP, true);
    qDebug() << "jointcount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPJOINT);

}
void handleNPstn::registerFrames(QList<FRAMEGROUPVALUE> &frgrouplist, QList<FRAMEPOINTSVALUE> &frlist){
    QHash<int,qreal> minimumcoordinate;
    QHash<int,qreal> radius_start;
    QHash<int,qreal> radius_max;
    int radius_count=0;

    //先に円周通り以外、後に円周通り
    for (int gridgroup = 0; gridgroup < model.numgg-1; gridgroup++) {
        //GC通りはスキップする？必要になったら復活
        FRAMEGROUPVALUE fgvalue;
        fgvalue.GID = QUuid::createUuid();
        frameGroupNoToUuid[gridgroup] = fgvalue.GID;
        //        fgvalue.groupName = QString::number(gridgroup);
        fgvalue.groupName = this->model.namegr[gridgroup][0].at(0);
        if (model.igg[gridgroup] == 0){
            fgvalue.frameType = FrameType::FRAMETYPE::STRAIGHT;
        }else if (model.igg[gridgroup] == 1){
            continue;
            //            fgvalue.frameType = FRAMETYPE::CIRCLE;
        }else if (model.igg[gridgroup] == 2){
            fgvalue.frameType = FrameType::FRAMETYPE::RADIUS;
            radius_count++;
        }else {
            fgvalue.frameType = FrameType::FRAMETYPE::NOFRAMETYPE;
        }
        fgvalue.count = model.numgr[gridgroup];
        fgvalue.basePoint = QPointF(model.dgg[gridgroup][0], model.dgg[gridgroup][1]);
        fgvalue.startAngle = model.dgg[gridgroup][2]*180.0/M_PI;
        fgvalue.endAngle = model.dgg[gridgroup][3]*180.0/M_PI;
        fgvalue.division = 36;
        fgvalue.startFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().first().UUID;
        fgvalue.endFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().last().UUID;
        frgrouplist << fgvalue;
        //通り

        for (int gridnumber = 0; gridnumber < model.numgr[gridgroup]; gridnumber++) {
            qreal coordinate = 0.0;
            for (int gn = 0; gn < gridnumber; gn++) {
                coordinate = coordinate + model.dgr[gridgroup][gn];
            }
            if (!minimumcoordinate.contains(gridgroup))minimumcoordinate[gridgroup] = coordinate;
            else if (minimumcoordinate[gridgroup] > coordinate)minimumcoordinate[gridgroup] = coordinate;
        }
        for (int gridnumber = 0; gridnumber < model.numgr[gridgroup]; gridnumber++) {
            qreal coordinate = 0.0;
            for (int gn = 0; gn < gridnumber; gn++) {
                coordinate = coordinate + model.dgr[gridgroup][gn];
            }
            qreal nextSpan = model.dgr[gridgroup][gridnumber];
            //            if(nextSpan==0.0)coordinate+=0.000001;
            if (nextSpan == 0.0)coordinate -= 0.000001;

            QList<QPointF> plist;
            qreal span = 0.0;
            QPointF npoint;
            STRAIGHTFRAMEVALUE st_value;
            if (fgvalue.frameType == FrameType::FRAMETYPE::STRAIGHT) {
                QPointF po = fgvalue.basePoint;
                qreal theta = fgvalue.startAngle /180.0*M_PI;
                span = coordinate - minimumcoordinate[gridgroup];
                qreal p_len =
                        fgvalue.plusLength; //通りのプラスマイナスをどこまで伸ばすかは節点の情報がないと決まらない
                qreal m_len = fgvalue.minusLength;
                QPointF p1, p2, p3, p4;
                QPointF spoint;
                qreal m_span = nextSpan;
                qreal s_angle;
                //if(true){
                if ( fgvalue.startAngle < 45.0 ) {
                    p1 = QPointF(-m_len * qCos(theta) - span * qSin(theta),
                                 -m_len * qSin(theta) + span * qCos(theta)) + po;
                    p2 = QPointF( p_len * qCos(theta) - span * qSin(theta),
                                  p_len * qSin(theta) + span * qCos(theta)) + po;
                    npoint = CalcVector2D::extendPoint(p1, p2, 1.0);
                    s_angle = fgvalue.startAngle + 90.;
                    if (gridnumber == fgvalue.count - 1) {
                        p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
                        p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                        spoint = CalcVector2D::extendPoint(p3, p4, 1.5);
                    } else {
                        spoint = CalcVector2D::extendPoint(p1, p2, 0.5);
                    }
                } else {
                    p1 = QPointF(-m_len * qCos(theta) + span * qSin(theta),
                                 -m_len * qSin(theta) - span * qCos(theta)) + po;
                    p2 = QPointF( p_len * qCos(theta) + span * qSin(theta),
                                  p_len * qSin(theta) - span * qCos(theta)) + po;
                    npoint = CalcVector2D::extendPoint(p2, p1, 1.0);
                    s_angle = fgvalue.startAngle- 90.;
                    if (gridnumber == fgvalue.count - 1) {
                        p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
                        p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                        spoint = CalcVector2D::extendPoint(p4, p3, 1.5);
                    } else {
                        spoint = CalcVector2D::extendPoint(p2, p1, 0.5);
                    }
                }
                plist.append(p1);
                plist.append(p2);

                if (gridnumber == fgvalue.count - 1) m_span = span;
                st_value = STRAIGHTFRAMEVALUE(m_span, s_angle, spoint,
                                              (gridnumber == 0), (gridnumber == fgvalue.count - 1));

            } else if (fgvalue.frameType == FrameType::FRAMETYPE::CIRCLE) {



            } else if (fgvalue.frameType == FrameType::FRAMETYPE::RADIUS) {

                QPointF po = fgvalue.basePoint;
                qreal angle = fgvalue.startAngle + (coordinate - minimumcoordinate[gridgroup])*180.0/M_PI ;
                qreal theta = angle /180.0*M_PI;
                qreal p_len = fgvalue.plusLength;
                radius_start.insert(radius_count,fgvalue.startAngle);
                radius_max.insert(radius_count,angle);


                plist.append(po);
                QPointF p1 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                plist.append(p1);

                npoint = CalcVector2D::extendPoint(po, p1, 3.0);

            } else {
                qDebug()<<"UNKNOWN FRAME TYPE";
                QPointF po = fgvalue.basePoint;
                QPointF p1 = QPointF(1.0, 0) + po;
                plist.append(po);
                plist.append(p1);
                npoint = CalcVector2D::extendPoint(po, p1, 3.0);
            }
            //            else if(fgvalue.frameType==DIAMETER){
            //            }else if(fgvalue.frameType==VARIABLE){
            //            }else if(fgvalue.frameType==CLCIRCLE){
            //            }else if(fgvalue.frameType==CLVARIABLE){
            //            }else{

            //            }//定義してない通りタイプが来た時、pointlistが空になる
            QString name = this->model.namegr[gridgroup][gridnumber].trimmed(); //GRは複数の通りに所属
            QString uid = QUuid::createUuid().toString() ;
            //            QList<int> grid_frame;grid_frame<<gridgroup;grid_frame<<gridnumber;
            frameNoToUuid[gridgroup * 10000 + gridnumber] = uid;


            FRAMEPOINTSVALUE fmp = FRAMEPOINTSVALUE(fgvalue.GID, plist, name, npoint, fgvalue.frameType,
                                                    st_value, uid);
            frlist.append(fmp);
            //            gcount++;
        }



    }

    int circle_count=0;
    //改めて円周通り（先に半径通りのデータが欲しいので）
    for (int gridgroup = 0; gridgroup < model.numgg-1; gridgroup++) {
        //GC通りはスキップする？必要になったら復活
        FRAMEGROUPVALUE fgvalue;
        fgvalue.GID = QUuid::createUuid();
        frameGroupNoToUuid[gridgroup] = fgvalue.GID;
        fgvalue.groupName = this->model.namegr[gridgroup][0].at(0);
        if (model.igg[gridgroup] == 1){
            fgvalue.frameType = FrameType::FRAMETYPE::CIRCLE;
            circle_count++;
        }else {
            continue;
        }
        fgvalue.count = model.numgr[gridgroup];
        fgvalue.basePoint = QPointF(model.dgg[gridgroup][0], model.dgg[gridgroup][1]);
        fgvalue.startAngle = model.dgg[gridgroup][2]*180.0/M_PI;
        fgvalue.endAngle = model.dgg[gridgroup][3]*180.0/M_PI;
        //START ENDが特に指定されていない場合・・・対になる半径通りを参照して、±5度する。半径と円周は交互にあると仮定して、
        qreal eps = 0.00000001;
        if(qAbs(fgvalue.startAngle-fgvalue.endAngle) < eps){
            fgvalue.startAngle = radius_start.value(circle_count)-5.0;
            fgvalue.endAngle = radius_max.value(circle_count)+5.0;
        }

        fgvalue.division = ((fgvalue.endAngle - fgvalue.startAngle)+eps)/5 + 1;
        fgvalue.startFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().first().UUID;
        fgvalue.endFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().last().UUID;
        frgrouplist << fgvalue;
        //通り

        for (int gridnumber = 0; gridnumber < model.numgr[gridgroup]; gridnumber++) {
            qreal coordinate = 0.0;
            for (int gn = 0; gn < gridnumber; gn++) {
                coordinate = coordinate + model.dgr[gridgroup][gn];
            }
            if (!minimumcoordinate.contains(gridgroup))minimumcoordinate[gridgroup] = coordinate;
            else if (minimumcoordinate[gridgroup] > coordinate)minimumcoordinate[gridgroup] = coordinate;
        }
        for (int gridnumber = 0; gridnumber < model.numgr[gridgroup]; gridnumber++) {
            qreal coordinate = 0.0;
            for (int gn = 0; gn < gridnumber; gn++) {
                coordinate = coordinate + model.dgr[gridgroup][gn];
            }
            qreal nextSpan = model.dgr[gridgroup][gridnumber];
            //            if(nextSpan==0.0)coordinate+=0.000001;
            if (nextSpan == 0.0)coordinate -= 0.000001;

            QList<QPointF> plist;
            qreal span = 0.0;
            QPointF npoint;
            STRAIGHTFRAMEVALUE st_value;
            if (fgvalue.frameType == FrameType::FRAMETYPE::STRAIGHT) {


            } else if (fgvalue.frameType == FrameType::FRAMETYPE::CIRCLE) {

                QPointF po = fgvalue.basePoint;
                qreal s_angle = fgvalue.startAngle;
                qreal e_angle = fgvalue.endAngle;
                qDebug()<<"CIRCLE_ANGLE_S_E"<<s_angle<<e_angle;
                span = coordinate - minimumcoordinate[gridgroup];
                int div = fgvalue.division;

                qreal s_theta = s_angle /180.0*M_PI;
                qreal e_theta = e_angle /180.0*M_PI;
                qreal d_theta = (e_theta - s_theta) / qreal(div);
                for ( int j = 0; j < div + 1; j++ ) {
                    qreal j_theta = s_theta + d_theta * qreal(j);
                    QPointF p1 = QPointF(po.x() + span * qCos(j_theta), po.y() + span * qSin(j_theta));
                    plist.append(p1);
                }

                if (plist.count() > 1)
                    npoint = CalcVector2D::extendPoint(plist.at(1), plist.at(0), 3.0);

            } else if (fgvalue.frameType == FrameType::FRAMETYPE::RADIUS) {


            } else {
            }

            QString name = this->model.namegr[gridgroup][gridnumber].trimmed(); //GRは複数の通りに所属
            QString uid = QUuid::createUuid().toString() ;
            frameNoToUuid[gridgroup * 10000 + gridnumber] = uid;


            FRAMEPOINTSVALUE fmp = FRAMEPOINTSVALUE(fgvalue.GID, plist, name, npoint, fgvalue.frameType,
                                                    st_value, uid);
            frlist.append(fmp);
        }



    }
    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(frgrouplist, frlist);
    qDebug() << "framepointcount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPFRAMEP);

}

void handleNPstn::registerOpenings(){
    //開口はBLD3DとunifiedDataで考え方が異なるので保留
    //    QList<SectionValues> wallopsectionvalues;
    //    for(int numso=0;numso<this->model.numso;numso++){

    //        for(int nso=0;nso<this->model.nso[numso];nso++){
    //            SectionValues sectionvalue;
    //            sectionvalue.sectionName=this->model.nameso[numso].trimmed();
    //            sectionvalue.wopen_param.openType=WallOpenType::NORMAL;
    //            sectionvalue.wopen_param.isXL=true;
    //            sectionvalue.wopen_param.isYB=true;
    //            if(this->model.iso[numso][nso][0]==0){
    //                sectionvalue.wopen_param.openType=WallOpenType::SLIT;
    //            }else if(this->model.iso[numso][nso][0]==1){
    //                sectionvalue.wopen_param.isXL=true;
    //            }else if(this->model.iso[numso][nso][0]==2){
    //                sectionvalue.wopen_param.isXL=false;
    //            }
    //            if(this->model.iso[numso][nso][1]==0){
    //                sectionvalue.wopen_param.openType=WallOpenType::SLIT;
    //            }else if(this->model.iso[numso][nso][1]==1){
    //                sectionvalue.wopen_param.isYB=true;
    //            }else{sectionvalue.wopen_param.isYB=false;}
    //            sectionvalue.wopen_param.ox=this->model.dso[numso][nso][0];//x
    //            sectionvalue.wopen_param.oy=this->model.dso[numso][nso][1];//y
    //            sectionvalue.wopen_param.width=this->model.dso[numso][nso][2];//l0
    //            sectionvalue.wopen_param.height=this->model.dso[numso][nso][3];//h0
    //            wallopsectionvalues.append(sectionvalue);
    //        }
    //    }
    //    UnifiedAnalysisData::getOutInstance()->registerTempData(TPWALLOPEN,wallopsectionvalues);
    qDebug() << "opening";
    //openingはあとでかんがえます
    for (int numso = 0; numso < model.numso; numso++) {
        QString label = model.nameso[numso].trimmed();
        QList<Opening *> opening;
        for (int nso = 0; nso < model.nso[numso]; nso++) {
            Opening::LEFT_OR_RIGHT lr = Opening::LEFT_OR_RIGHT::CENTER;
            if (model.iso[numso][nso][0] == 0) {
                lr = Opening::LEFT_OR_RIGHT::CENTER;
            } else if (model.iso[numso][nso][0] == 1) {
                lr = Opening::LEFT_OR_RIGHT::LEFT;
            } else if (model.iso[numso][nso][0] == 2) {
                lr = Opening::LEFT_OR_RIGHT::RIGHT;
            }
            Opening::TOP_OR_BOTTOM tb = Opening::TOP_OR_BOTTOM::MIDDLE;
            if (model.iso[numso][nso][1] == 0) {
                tb = Opening::TOP_OR_BOTTOM::MIDDLE;
            } else if (model.iso[numso][nso][1] == 1) {
                tb = Opening::TOP_OR_BOTTOM::BOTTOM;
            } else {
                tb = Opening::TOP_OR_BOTTOM::TOP;
            }
            qreal xpos = model.dso[numso][nso][0]; //x
            qreal ypos = model.dso[numso][nso][1]; //y
            qreal width = model.dso[numso][nso][2]; //l0
            qreal height = model.dso[numso][nso][3]; //h0
            if (width == 0)lr = Opening::LEFT_OR_RIGHT::WFULL;
            if (height == 0)tb = Opening::TOP_OR_BOTTOM::HFULL;
            Opening *op = new Opening(lr, tb, xpos, ypos, width, height);
            opening.append(op);
        }
        OpeningSet *openingset = new OpeningSet(label, opening);
        openingList.append(openingset);
    }
    qDebug() << "opening";
}
void handleNPstn::registerFloors(){
    //階
    QList<FLOORVALUES> fllist;
    for (int floorgroup = 0; floorgroup < this->model.numfg; floorgroup++) {
        int basefloorno = this->model.lfg[floorgroup][0] - 1;
        qreal basefloorlevel = this->model.dfg[floorgroup];
        for (int floornumber = 0; floornumber < this->model.numfl[floorgroup]; floornumber++) {
            FLOORVALUES flvalue;
            flvalue.floorName =
                    this->model.namefl[floorgroup][floornumber].trimmed(); //MXJFL is NJFLの最大値。各節点はNJFL個の階に所属している。ただしpost3dでは一個なのでデータが落ちる
            flvalue.analysisName = this->model.namefl[floorgroup][floornumber].trimmed();
            flvalue.floorHeight = 0.0;
            flvalue.UUID = QUuid::createUuid();
            if (this->model.ifl[floorgroup][floornumber] == 2)flvalue.floorType = FLOORTYPE::FL_OVERGROUND;
            else if (this->model.ifl[floorgroup][floornumber] == 1)flvalue.floorType = FLOORTYPE::FL_UNDERGROUND;
            else if (this->model.ifl[floorgroup][floornumber] == 0)flvalue.floorType = FLOORTYPE::FL_CHILD;
            else if (this->model.ifl[floorgroup][floornumber] == 3)flvalue.floorType = FLOORTYPE::FL_PENTHOUSE;
            for (int fn = 0; fn < floornumber; fn++) {
                flvalue.floorHeight = flvalue.floorHeight + this->model.ahfl[floorgroup][fn];
            }
            if (floornumber == basefloorno)basefloorlevel -= flvalue.floorHeight;

            fllist.append(flvalue);
            floorNoToUuid[floornumber] = flvalue.UUID;
        }
        for (int i = 0; i < fllist.count(); i++) {
            fllist[i].floorHeight += basefloorlevel;
        }
    }
    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(fllist);
    qDebug() << "floorcount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPFLOOR);

}
void handleNPstn::registerSectionValues(){
    //startFloorIDとendFloorIDをセットして、ハイフン区切りの断面名をばらす。Memberの断面名も変更

    //section
    QList<SectionValues> allsectionvalues;
    QList<SectionValues> columnsectionvalues;
    QList<SectionValues> bracesectionvalues;
    QList<SectionValues> girdersectionvalues;
    //    QHash<QString,SectionValues*> sectionNameTable;
    SectionValues sectionvalue;

    if (pstnReadStatus.value(OFILE_MODEL)!=READING_STATUS::STATUS_SUCCESS)return;
    for (int ils = 0; ils < this->model.numlsd; ils++) {
        sectionvalue.sectionName = this->model.namels[ils].trimmed();

        switch (this->model.ils1[ils][0]) {
        case 1:
            sectionvalue.memberType = DATATYPE::TPCOLUMN;
            break;
        case 2:
            sectionvalue.memberType = DATATYPE::TPGIRDER;
            break;
        case 3:
            sectionvalue.memberType = DATATYPE::TPBRACE;
            break;
        default:
            sectionvalue.memberType = DATATYPE::TPCOLUMN;
        }

        int type = this->model.ils2[ils][0][0]; //=1::S,=2::RC,=3::SRC,=4::CFT
        switch (type) {
        case 1:
            sectionvalue.sectionType = STRUCTTYPE::STP_S;
            break;
        case 2:
            sectionvalue.sectionType = STRUCTTYPE::STP_RC;
            break;
        case 3:
            sectionvalue.sectionType = STRUCTTYPE::STP_SRC;
            break;
        case 4:
            sectionvalue.sectionType = STRUCTTYPE::STP_CFT;
            break;
        }
        qDebug()<<"SectionName:"<<sectionvalue.sectionName<<(int)sectionvalue.memberType<<this->model.ils1[ils][0]<<type;
        SteelType::Type stltyp = SteelType::Type::UNDEFINED;
        //        SteelType::Type stltyp_T = SteelType::Type::UNDEFINED;
        qreal sizeH = 0.0;
        qreal sizeB = 0.0;
        qreal sizeH2 = 0.0;
        qreal sizeB2 = 0.0;
        qreal sizeDy = 0.0;
        qreal sizeDz = 0.0;
        qreal size1_T1 = 0.0;
        qreal size1_T2 = 0.0;
        qreal size2_T1 = 0.0;
        qreal size2_T2 = 0.0;
//TODO:ICJでちゃんと読む
        int ictype = this->model.ils2[ils][0][3]; //コンクリート形状
        int istype = this->model.ils2[ils][0][4]; //鉄骨形状
        if (sectionvalue.sectionType == STRUCTTYPE::STP_S || sectionvalue.sectionType == STRUCTTYPE::STP_CFT
                ||sectionvalue.sectionType == STRUCTTYPE::STP_SRC) { //S or CFT or SRC
            if (istype == 1 || istype == 2) {
                stltyp = SteelType::Type::ST_H;   //direction=DIRECTION_Y;柱のH型
            } else if (istype == 3 || istype == 4) {
                stltyp = SteelType::Type::ST_H;   //梁のH型、柱のI型。2,4はウェブが斜めに取りつくパターン
            }//direction=DIRECTION_Z;}
            else if (istype == 5 || istype == 30 || istype == 31 || istype == 32) {
                stltyp = SteelType::Type::ST_BX;   //30,31,32は冷間
            }
            else if (istype == 6) {
                stltyp = SteelType::Type::ST_P;
            }else if (istype == 7) {//トの逆
                stltyp = SteelType::Type::ST_T1;
            } else if ( istype == 8) {//ト
                stltyp = SteelType::Type::ST_T2;
            } else if (istype == 9 ) {//T
                stltyp = SteelType::Type::ST_T4;
            } else if ( istype == 10) {//凸
                stltyp = SteelType::Type::ST_T3;
            }else if (istype == 11) {
                stltyp = SteelType::Type::ST_C1;   //Π
            }else if ( istype == 12) {
                stltyp = SteelType::Type::ST_C2;   //うけばこ
            }else if (istype == 13) {
                stltyp = SteelType::Type::ST_C4;   //C
            }else if (istype == 14) {
                stltyp = SteelType::Type::ST_C3;   //]
            }else if (istype == 15) {
                stltyp = SteelType::Type::ST_R;   //R
            } else if (istype == 16) {
                stltyp = SteelType::Type::ST_BR;   //BR
            }//dshape=DRAWSHAPE_CIRCLE;direction=DIRECTION_Y;}
            else if (istype == 17 || istype == 45) {
                stltyp = SteelType::Type::ST_CROSSH;
            }//dshape=DRAWSHAPE_CROSSH;direction=DIRECTION_Z;}
            else if (istype == 46) {
                stltyp = SteelType::Type::ST_CROSS;
            }//dshape=DRAWSHAPE_CROSS;direction=DIRECTION_Y;}
            sectionvalue.i_param.steelType = stltyp;
            sectionvalue.c_param.steelType = stltyp;
            sectionvalue.j_param.steelType = stltyp;

            //post3d:柱も梁もsizeHに格納し、柱のHは横向き、梁のHは縦向きとなるようにとなるように
            // inputでは：部材種別ごとに描画関数に渡す角度を90度変えて対応
            //b3d:柱はｙ方向、梁はz方向に格納。
            //BXの板厚の対応確認。
            //壁の厚さ入っていない
            if (sectionvalue.memberType == DATATYPE::TPCOLUMN) {
                sizeH = 1000.0*this->model.dls1[ils][0][24];
                sizeB = 1000.0*this->model.dls1[ils][0][25];
                size1_T1 = 1000.0*this->model.dls1[ils][0][26];
                size1_T2 = 1000.0*this->model.dls1[ils][0][27];
                sizeH2 = 1000.0*this->model.dls1[ils][0][33];
                sizeB2 = 1000.0*this->model.dls1[ils][0][34];
                size2_T1 = 1000.0*this->model.dls1[ils][0][35];
                size2_T2 = 1000.0*this->model.dls1[ils][0][36];
            }else{
                sizeH = 1000.0*this->model.dls1[ils][0][33];
                sizeB = 1000.0*this->model.dls1[ils][0][34];
                size1_T1 = 1000.0*this->model.dls1[ils][0][35];
                size1_T2 = 1000.0*this->model.dls1[ils][0][36];
                sizeH2 = 1000.0*this->model.dls1[ils][0][24];
                sizeB2 = 1000.0*this->model.dls1[ils][0][25];
                size2_T1 = 1000.0*this->model.dls1[ils][0][26];
                size2_T2 = 1000.0*this->model.dls1[ils][0][27];
            }

        }
        if(sectionvalue.sectionType == STRUCTTYPE::STP_RC ||sectionvalue.sectionType == STRUCTTYPE::STP_SRC ){ //RC or SRC
            sectionvalue.i_param.concType = ConcreteType::Type::RECTANGLE;
            sectionvalue.c_param.concType = ConcreteType::Type::RECTANGLE;
            sectionvalue.j_param.concType = ConcreteType::Type::RECTANGLE;
            if (ictype == 20) {
                sectionvalue.i_param.concType = ConcreteType::Type::CIRCLE;
                sectionvalue.c_param.concType = ConcreteType::Type::CIRCLE;
                sectionvalue.j_param.concType = ConcreteType::Type::CIRCLE;
            }

            sizeDz = 1000.0*this->model.dls1[ils][0][1]; //D
            sizeDy = 1000.0*this->model.dls1[ils][0][0]; //b
        }
        sectionvalue.i_param.sizeH = sizeH;
        sectionvalue.i_param.sizeB = sizeB;
        sectionvalue.i_param.H_T = sizeH2;
        sectionvalue.i_param.B_T = sizeB2;
        sectionvalue.i_param.t1 = size1_T1;
        sectionvalue.i_param.t2 = size1_T2;
        sectionvalue.i_param.t1_T = size2_T1;
        sectionvalue.i_param.t2_T = size2_T2;

        sectionvalue.c_param.sizeH = sizeH;
        sectionvalue.c_param.sizeB = sizeB;
        sectionvalue.c_param.H_T = sizeH2;
        sectionvalue.c_param.B_T = sizeB2;
        sectionvalue.c_param.t1 = size1_T1;
        sectionvalue.c_param.t2 = size1_T2;
        sectionvalue.c_param.t1_T = size2_T1;
        sectionvalue.c_param.t2_T = size2_T2;

        sectionvalue.j_param.sizeH = sizeH;
        sectionvalue.j_param.sizeB = sizeB;
        sectionvalue.j_param.H_T = sizeH2;
        sectionvalue.j_param.B_T = sizeB2;
        sectionvalue.j_param.t1 = size1_T1;
        sectionvalue.j_param.t2 = size1_T2;
        sectionvalue.j_param.t1_T = size2_T1;
        sectionvalue.j_param.t2_T = size2_T2;

        sectionvalue.i_param.Dy = sizeDy;
        sectionvalue.c_param.Dy = sizeDy;
        sectionvalue.j_param.Dy = sizeDy;
        sectionvalue.i_param.Dz = sizeDz;
        sectionvalue.c_param.Dz = sizeDz;
        sectionvalue.j_param.Dz = sizeDz;

        sectionvalue.startFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().first().UUID;
        sectionvalue.endFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().last().UUID;
        allsectionvalues << sectionvalue;
        UnifiedFixedData::getInstance()->getPstnData()->insertLineSectionNameB3dIndexTable(sectionvalue.sectionName,ils);
        //        UnifiedFixedData::getInstance()->getPstnData()->insertLineSectionNameB3dIndexTable(this->model.namels[ils].trimmed(),ils);
    }
    foreach (SectionValues secitem, allsectionvalues) {
        if (secitem.memberType == DATATYPE::TPCOLUMN)columnsectionvalues << secitem;
        if (secitem.memberType == DATATYPE::TPGIRDER)girdersectionvalues << secitem;
        if (secitem.memberType == DATATYPE::TPBRACE)bracesectionvalues << secitem;
    }
    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPCOLUMN, columnsectionvalues);
    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPGIRDER, girdersectionvalues);
    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPBRACE, bracesectionvalues);

    //wall,slab
    //TODO:スラブ
    QList<SectionValues> wallsectionvalues;
    QList<SectionValues> slabsectionvalues;
    for (int iisw = 0; iisw < this->model.numsw; iisw++) {
        SectionValues sectionvalue;
        sectionvalue.sectionName = this->model.namesw[iisw].trimmed();
        if(this->model.isw[iisw][0]==2){
            //s
            int material = this->model.isw[iisw][1];
            QString name=this->model.nameas.at(material-1);
            sectionvalue.plane_param.steelMaterial =name;// 1000.0*this->model.dsw[iisw][0]; //壁厚t
            sectionvalue.plane_param.steelThickness = 1000.0*this->model.dsw[iisw][0]; //壁厚t
            sectionvalue.sectionType = STRUCTTYPE::STP_S;
            sectionvalue.memberType = DATATYPE::TPWALL;
            wallsectionvalues.append(sectionvalue);
        }else{
            //rc
            int material = this->model.isw[iisw][1];
            QString name=this->model.nameac.at(material-1);
            sectionvalue.plane_param.concMaterial =name;// 1000.0*this->model.dsw[iisw][0]; //壁厚t
            sectionvalue.plane_param.concThickness = 1000.0*this->model.dsw[iisw][0]; //壁厚t
            sectionvalue.sectionType = STRUCTTYPE::STP_RC;
            sectionvalue.memberType = DATATYPE::TPWALL;
            wallsectionvalues.append(sectionvalue);
            sectionvalue.memberType = DATATYPE::TPSLAB;
            slabsectionvalues.append(sectionvalue);
        }


        UnifiedFixedData::getInstance()->getPstnData()->insertPlaneSectionNameB3dIndexTable(sectionvalue.sectionName,iisw);
    }

    QList<SectionValues> dampersectionvalues;
    for (int iidp = 0; iidp < this->model.numdl; iidp++) {
        SectionValues sectionvalue;
        sectionvalue.sectionName = this->model.namedl[iidp].trimmed();
        sectionvalue.memberType = DATATYPE::TPDAMPER;
        dampersectionvalues.append(sectionvalue);

        UnifiedFixedData::getInstance()->getPstnData()->insertDamperSectionNameB3dIndexTable(sectionvalue.sectionName,iidp);
    }

    QList<SectionValues> springsectionvalues;
    for (int ii = 0; ii < this->model.numb; ii++) {
        SectionValues sectionvalue;
        sectionvalue.sectionName = QString::number(ii+1);
        sectionvalue.memberType = DATATYPE::TPSPRING;
        switch(model.jbbcon.at(ii)){
        case 1:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::LINEAR;
        case 2:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::BILINEAR;
        case 3:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::DEGRADING;
        case 4:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::SLIP;
        case 5:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::ORIGIN_ORIENTED;
        case 6:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::REVERSIBLE_BILINEAR;
        case 7:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::TRILINEAR;
        case 8:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::ORIGIN_ORIENTED;
        case 9:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::REVERSIBLE_BILINEAR;
        default:
            sectionvalue.vspring_param.hysteresis=SpringType::Type::LINEAR;
        }
        springsectionvalues.append(sectionvalue);

        UnifiedFixedData::getInstance()->getPstnData()->insertSpringSectionNameB3dIndexTable(sectionvalue.sectionName,ii);
    }



    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPWALL, wallsectionvalues);
    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPSLAB, slabsectionvalues);
    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPDAMPER, dampersectionvalues);
    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPSPRING, springsectionvalues);


}
void handleNPstn::registerUuidIndexTable(){
    Q_FOREACH(QUuid uid,floorNoToUuid){
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,floorNoToUuid.key(uid),DATATYPE::TPFLOOR);
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,floorNoToUuid.key(uid),B3D_DATATYPE::B3D_TPFLOOR);
    }
    Q_FOREACH(QUuid uid,frameGroupNoToUuid){
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,frameGroupNoToUuid.key(uid),DATATYPE::TPFRAMEG);
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,frameGroupNoToUuid.key(uid),B3D_DATATYPE::B3D_TPFRAMEG);
    }
    Q_FOREACH(QUuid uid,frameNoToUuid){
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,frameNoToUuid.key(uid),DATATYPE::TPFRAMEP);
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,frameNoToUuid.key(uid),B3D_DATATYPE::B3D_TPFRAME);
    }
    Q_FOREACH(QUuid uid,jointNoToUuid){
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,jointNoToUuid.key(uid),DATATYPE::TPJOINT);
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,jointNoToUuid.key(uid),B3D_DATATYPE::B3D_TPJOINT);
    }
    int columnCount = 0;
    int girderCount = 0;
    int braceCount = 0;
    Q_FOREACH(QUuid uid,memberNoToUuid){
        int numm = memberNoToUuid.key(uid);
        int msyu = model.imm1[numm][1];//１：柱，２：大梁，３：ブレース，４：杭，０：その他
        switch(msyu){
        case  1:
            UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,columnCount,DATATYPE::TPCOLUMN);
            UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,memberNoToUuid.key(uid),B3D_DATATYPE::B3D_TPLINEMEMBER);
            columnCount++;
            break;
        case  2:
            UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,girderCount,DATATYPE::TPGIRDER);
            UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,memberNoToUuid.key(uid),B3D_DATATYPE::B3D_TPLINEMEMBER);
            girderCount++;
            break;
        case  3:
            UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,braceCount,DATATYPE::TPBRACE);
            UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,memberNoToUuid.key(uid),B3D_DATATYPE::B3D_TPLINEMEMBER);
            braceCount++;
            break;
        default:
            UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,0,DATATYPE::NODATATYPE);
            UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,memberNoToUuid.key(uid),B3D_DATATYPE::B3D_NODATATYPE);
            braceCount++;
        }
    }
    Q_FOREACH(QUuid uid,wallNoToUuid){
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,wallNoToUuid.key(uid),DATATYPE::TPWALL);
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,planeTable.value(uid).first,planeTable.value(uid).second);
    }
    Q_FOREACH(QUuid uid,slabNoToUuid){
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,slabNoToUuid.key(uid),DATATYPE::TPSLAB);
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,planeTable.value(uid).first,planeTable.value(uid).second);
    }
    qDebug()<<"springNoToUuid register before";
    Q_FOREACH(QUuid uid,springNoToUuid){
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,springNoToUuid.key(uid),DATATYPE::TPSPRING);
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,springTable.value(uid).first,springTable.value(uid).second);
    }
    Q_FOREACH(QUuid uid,damperNoToUuid){
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidIndexTable(uid,damperNoToUuid.key(uid),DATATYPE::TPDAMPER);
        UnifiedFixedData::getInstance()->getPstnData()->insertUuidB3dIndexTable(uid,damperNoToUuid.key(uid),B3D_DATATYPE::B3D_TPDAMPER);
    }
    qDebug()<<"springNoToUuid register done";

}
void handleNPstn::registerJointOutput(QStringList &loadcase){
    foreach (QString load, loadcase) {
        if (!load.isEmpty())UnifiedFixedData::getInstance()->getPstnData()->setMaxStep(load, this->rst[load].stressSteps.count());
        if (load.isEmpty())continue;

        for (int jno = 0; jno < this->model.numj; jno++) {
            QUuid uid = jointNoToUuid[jno];
            QList<XYZ> xyzlist;//list of step
            QList<XYZ> RxRyRzlist;//list of step
            QList<QList<qreal> > reactlist;//list of step

            for (int step = 0; step < this->rst[load].stressSteps.count(); step++) {
                DataNBLD3D_RSTStep rst_of_step = this->rst[load].stressSteps[step];
                //変位
                qreal x = rst_of_step.ujnt[jno][0];
                qreal y = rst_of_step.ujnt[jno][1];
                qreal z = rst_of_step.ujnt[jno][2];
                xyzlist << XYZ(x, y, z);
                //回転角
                qreal Rx = rst_of_step.ujnt[jno][3];
                qreal Ry = rst_of_step.ujnt[jno][4];
                qreal Rz = rst_of_step.ujnt[jno][5];
                RxRyRzlist << XYZ(Rx, Ry, Rz);
                //反力
                QList<qreal> react;
                react << rst_of_step.react[jno][0]
                        << rst_of_step.react[jno][1]
                        << rst_of_step.react[jno][2]
                        << rst_of_step.react[jno][3]
                        << rst_of_step.react[jno][4]
                        << rst_of_step.react[jno][5];
                reactlist << react;
            }
            UnifiedFixedData::getInstance()->getPstnData()->setJointDisp(uid, load, xyzlist);
            UnifiedFixedData::getInstance()->getPstnData()->setJointRotate(uid, load, RxRyRzlist);
            UnifiedFixedData::getInstance()->getPstnData()->setReaction(uid, load, reactlist); //reactlist[load][step][dof]
        }
    }

}
void handleNPstn::registerMemberOutput(QStringList &loadcase){
    foreach (QString load, loadcase) {

        for (int numm = 0; numm < this->model.numm; numm++) {
            QUuid uid = memberNoToUuid[numm];
            QList<MemberForce> facesectionalforcelist;//list of step
            QList<MemberForce> nodesectionalforcelist;
            QList<QList<MRESULT::YIELD_STATE> > yieldstatelist;

            for (int step = 0; step < this->rst[load].stressSteps.count(); step++) {
                DataNBLD3D_RSTStep rst_of_step = this->rst[load].stressSteps[step];
                MemberForce facesectionalforce;
                MemberForce nodesectionalforce;
                facesectionalforce.iMy = rst_of_step.rstmm[numm][0];
                facesectionalforce.iMz = rst_of_step.rstmm[numm][1];
                facesectionalforce.jMy = rst_of_step.rstmm[numm][2];
                facesectionalforce.jMz = rst_of_step.rstmm[numm][3];
                facesectionalforce.iQy = rst_of_step.rstmm[numm][4];
                facesectionalforce.iQz = rst_of_step.rstmm[numm][5];
                facesectionalforce.jQy = rst_of_step.rstmm[numm][4];
                facesectionalforce.jQz = rst_of_step.rstmm[numm][5];
                facesectionalforce.N = rst_of_step.rstmm[numm][6];
                facesectionalforce.iMx = rst_of_step.rstmm[numm][7];
                facesectionalforce.jMx = rst_of_step.rstmm[numm][7];
                nodesectionalforce = facesectionalforce;
                nodesectionalforce.iMy -= nodesectionalforce.iQz *
                        this->model.dmm1[numm][6]; //i端のフェイス長さ＊せん断力
                nodesectionalforce.iMz += nodesectionalforce.iQy *
                        this->model.dmm1[numm][6]; //i端のフェイス長さ＊せん断力
                nodesectionalforce.jMy += nodesectionalforce.jQz *
                        this->model.dmm1[numm][7]; //j端のフェイス長さ＊せん断力
                nodesectionalforce.jMz -= nodesectionalforce.jQy *
                        this->model.dmm1[numm][7]; //j端のフェイス長さ＊せん断力
                facesectionalforce.cMy = (facesectionalforce.iMy + facesectionalforce.jMy) / 2.0 ;
                facesectionalforce.cMz = (facesectionalforce.iMz + facesectionalforce.jMz) / 2.0 ;
                nodesectionalforce.cMy = (nodesectionalforce.iMy + nodesectionalforce.jMy) / 2.0 ;
                nodesectionalforce.cMz = (nodesectionalforce.iMz + nodesectionalforce.jMz) / 2.0 ;


                //CMQ:旧BLD3D向け
                //            qreal in = 0.0;
                //            qreal jn = 0.0;
                //            qreal iqy = 0.0;
                //            qreal iqz = 0.0;
                //            qreal jqy = 0.0;
                //            qreal jqz = 0.0;
                //            qreal imx = 0.0;
                //            qreal jmx = 0.0;
                //            qreal imy = 0.0;
                //            qreal imz = 0.0;
                //            qreal jmy = 0.0;
                //            qreal jmz = 0.0;
                //            qreal cmy = 0.0;
                //            qreal cmz = 0.0;
                ////            DLHMM(2,IL,ICL)
                //            for (int cl = 0; cl < rst_of_step.kcasol; cl++) {
                //                int icase = rst_of_step.icase[cl]; //cl番目の荷重ケースの番号
                //                qreal ccase = rst_of_step.ccase[cl]; //係数
                //                //                qreal ccase=1.0;
                //                int memberloadcase = this->load.indle[icase - 1]; //部材荷重の番号
                //                if (memberloadcase == 0)continue;
                //                in = in - ccase * this->load.cmqmm[memberloadcase - 1][numm][0];
                //                iqy = iqy - ccase * this->load.cmqmm[memberloadcase - 1][numm][1];
                //                iqz = iqz - ccase * this->load.cmqmm[memberloadcase - 1][numm][2];
                //                imx = imx - ccase * this->load.cmqmm[memberloadcase - 1][numm][3];
                //                imy = imy - ccase * this->load.cmqmm[memberloadcase - 1][numm][4];
                //                imz = imz - ccase * this->load.cmqmm[memberloadcase - 1][numm][5];
                //                jn = jn + ccase * this->load.cmqmm[memberloadcase - 1][numm][6];
                //                jqy = jqy + ccase * this->load.cmqmm[memberloadcase - 1][numm][7];
                //                jqz = jqz + ccase * this->load.cmqmm[memberloadcase - 1][numm][8];
                //                jmx = jmx + ccase * this->load.cmqmm[memberloadcase - 1][numm][9];
                //                jmy = jmy + ccase * this->load.cmqmm[memberloadcase - 1][numm][10];
                //                jmz = jmz + ccase * this->load.cmqmm[memberloadcase - 1][numm][11];
                //                cmy = cmy + ccase * this->load.cmqmm[memberloadcase - 1][numm][12];
                //                cmz = cmz + ccase * this->load.cmqmm[memberloadcase - 1][numm][13];
                //            }

                //            facesectionalforce.N += (in + jn) * 0.5;
                //            facesectionalforce.iQy += iqy;
                //            facesectionalforce.iQz += iqz;
                //            facesectionalforce.jQy += jqy;
                //            facesectionalforce.jQz += jqz;
                //            facesectionalforce.iMx += imx;
                //            facesectionalforce.jMx += jmx;

                //            facesectionalforce.iMy += imy;
                //            facesectionalforce.iMz += imz;
                //            facesectionalforce.jMy += jmy;
                //            facesectionalforce.jMz += jmz;

                //            nodesectionalforce.N += (in + jn) * 0.5;
                //            nodesectionalforce.iQy += iqy;
                //            nodesectionalforce.iQz += iqz;
                //            nodesectionalforce.jQy += jqy;
                //            nodesectionalforce.jQz += jqz;
                //            nodesectionalforce.iMx += imx;
                //            nodesectionalforce.jMx += jmx;

                //            nodesectionalforce.iMy += imy;
                //            nodesectionalforce.iMz += imz;
                //            nodesectionalforce.jMy += jmy;
                //            nodesectionalforce.jMz += jmz;
                //            facesectionalforce.cMy = (facesectionalforce.iMy + facesectionalforce.jMy) / 2.0 + cmy;
                //            facesectionalforce.cMz = (facesectionalforce.iMz + facesectionalforce.jMz) / 2.0 + cmz;
                //            nodesectionalforce.cMy = (nodesectionalforce.iMy + nodesectionalforce.jMy) / 2.0 + cmy;
                //            nodesectionalforce.cMz = (nodesectionalforce.iMz + nodesectionalforce.jMz) / 2.0 + cmz;

                facesectionalforcelist << facesectionalforce;
                nodesectionalforcelist << nodesectionalforce;

                QList<MRESULT::YIELD_STATE> yieldstate;
                int istate = rst_of_step.ipspmm[numm][0]; //i
                int jstate = rst_of_step.ipspmm[numm][1]; //j
                int cstate = rst_of_step.ipspmm[numm][2]; //c

                if (UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid)->dataType() == DATATYPE::TPBRACE ) {

                    if (istate == 0) {
                        yieldstate << MRESULT::YIELD_STATE::NOT;
                    } else if (istate > 0) {
                        yieldstate << MRESULT::YIELD_STATE::YIELD;
                    } else if (istate < 0) {
                        yieldstate << MRESULT::YIELD_STATE::HINGE;
                    }
                    if (jstate == 0) {
                        yieldstate << MRESULT::YIELD_STATE::NOT;
                    } else if (jstate > 0) {
                        yieldstate << MRESULT::YIELD_STATE::YIELD;
                    } else if (jstate < 0) {
                        yieldstate << MRESULT::YIELD_STATE::HINGE;
                    }
                    if (cstate == 0) {
                        yieldstate << MRESULT::YIELD_STATE::NOT;
                    } else if (cstate > 0) {
                        yieldstate << MRESULT::YIELD_STATE::AXIAL_YIELD;
                    } else if (cstate < 0) {
                        yieldstate << MRESULT::YIELD_STATE::AXIAL_YIELD;
                    }

                } else {
                    if (istate == 0) {
                        yieldstate << MRESULT::YIELD_STATE::NOT;
                    } else if (istate > 0) {
                        yieldstate << MRESULT::YIELD_STATE::YIELD;
                    } else if (istate < 0) {
                        yieldstate << MRESULT::YIELD_STATE::HINGE;
                    }
                    if (jstate == 0) {
                        yieldstate << MRESULT::YIELD_STATE::NOT;
                    } else if (jstate > 0) {
                        yieldstate << MRESULT::YIELD_STATE::YIELD;
                    } else if (jstate < 0) {
                        yieldstate << MRESULT::YIELD_STATE::HINGE;
                    }
                    if (cstate == 0) {
                        yieldstate << MRESULT::YIELD_STATE::NOT;
                    } else if (cstate > 0) {
                        yieldstate << MRESULT::YIELD_STATE::SHEAR_CRACK;
                    } else if (cstate < 0) {
                        yieldstate << MRESULT::YIELD_STATE::SHEAR_YIELD;
                    }
                }
                yieldstatelist << yieldstate;

            }
            UnifiedFixedData::getInstance()->getPstnData()->setFaceMemberForce(uid, load, facesectionalforcelist);
            UnifiedFixedData::getInstance()->getPstnData()->setNodeMemberForce(uid, load, nodesectionalforcelist);
            UnifiedFixedData::getInstance()->getPstnData()->setYieldState(uid, load, yieldstatelist);
        }
        qDebug() << "mload";

        for (int numb = 0; numb < this->model.numb; numb++) {
            qDebug()<<u8"numb"<<numb;

            QUuid uid = springNoToUuid.value(numb);
            qDebug()<<u8"numb,uid"<<numb<<uid;
            QVector<qreal> springValueList;
            for (int step = 0; step < this->rst[load].stressSteps.count(); step++) {
                qreal value;
                value = this->rst[load].stressSteps[step].rstbb[numb];

                springValueList << value;

            }
            UnifiedFixedData::getInstance()->getPstnData()->setSpringValue(uid, load, springValueList);
        }

        for (int numww = 0; numww < this->model.numww; numww++) {
            QUuid uid = wallNoToUuid[numww];
            QList<MemberForce> sectionalforcelist;
            QList<QList<MRESULT::YIELD_STATE> > yieldstatelist;

            for (int step = 0; step < this->rst[load].stressSteps.count(); step++) {
                DataNBLD3D_RSTStep rst_of_step = this->rst[load].stressSteps[step];

                MemberForce sectionalforce;
                sectionalforce.iMz = rst_of_step.rstww[numww][2][0]; //2=壁柱
                sectionalforce.jMz = rst_of_step.rstww[numww][2][1];
                sectionalforce.iQy = rst_of_step.rstww[numww][2][2];
                sectionalforce.jQy = rst_of_step.rstww[numww][2][2];
                sectionalforce.N = rst_of_step.rstww[numww][2][3];
                sectionalforcelist << sectionalforce;

                //降伏状態
                QList<MRESULT::YIELD_STATE> yieldstate;
                int istate = rst_of_step.ipspww[numww][0]; //i
                //                int jstate=rst_of_step.ipspww[numww][1];//j
                int cstate = rst_of_step.ipspww[numww][2]; //c
                if (istate == 0) {
                    yieldstate << MRESULT::YIELD_STATE::NOT;
                } else if (istate > 0) {
                    yieldstate << MRESULT::YIELD_STATE::YIELD;
                } else if (istate < 0) {
                    yieldstate << MRESULT::YIELD_STATE::HINGE;
                }
                if (true) {
                    yieldstate << MRESULT::YIELD_STATE::NOT;
                }
                if (cstate == 0) {
                    yieldstate << MRESULT::YIELD_STATE::NOT;
                } else if (cstate > 0) {
                    yieldstate << MRESULT::YIELD_STATE::SHEAR_CRACK;
                } else if (cstate < 0) {
                    yieldstate << MRESULT::YIELD_STATE::SHEAR_YIELD;
                }

                yieldstatelist << yieldstate;

            }
            UnifiedFixedData::getInstance()->getPstnData()->setFaceMemberForce(uid, load, sectionalforcelist);
            UnifiedFixedData::getInstance()->getPstnData()->setNodeMemberForce(uid, load, sectionalforcelist);
            UnifiedFixedData::getInstance()->getPstnData()->setYieldState(uid, load, yieldstatelist);

        }//step loop
    }//loadcase

}
void handleNPstn::registerMode(){
    QHash<QUuid, QVector<XYZ>> modehash;
    for (int ii = 0; ii < mode.mode.count(); ii++) {
        modehash[jointNoToUuid[ii + 1]] = mode.mode.at(ii);
    }
    UnifiedFixedData::getInstance()->getPstnData()->setModeData(modehash, mode.beta, mode.naturalPeriod, mode.totalMass);

}
void handleNPstn::registerChck(){
    //chck
    QHash<QUuid, QVector<QVector<float> > > verificationS;
    QHash<QUuid, QVector<QVector<float> > > verificationS2;
    QHash<QUuid, QVector<QVector<float> > > verificationB;
    QHash<QUuid, QVector<QVector<float> > > allowanceB;

    qDebug()<<"set name ch"<<chck.namech;
    UnifiedFixedData::getInstance()->getPstnData()->setNameCh(chck.namech);
    //10,11,21,22
    qDebug()<<"register line member chck";
    for (int numm = 0; numm < model.numm; numm++) {
        QUuid uid = memberNoToUuid[numm];
        QVector<QVector<float> > allowanceB_numch_icj;
        QVector<QVector<float> > verificationB_numch_icj;
        QVector<QVector<float> > verificationS_numch_icj;
        QVector<QVector<float> > verificationS2_numch_icj;
        for (int numch = 0; numch < chck.numch; numch++) {
            QVector<float> verificationB_icj;//検定比
            QVector<float> allowanceB_icj;//曲げ余裕率
            QVector<float> verificationS_icj;//せん断余裕率
            QVector<float> verificationS2_icj;//せん断余裕率SofSRC,付着ofRC
            verificationB_icj << chck.rstmma.at(numch).at(numm).at(0).at(10)<< chck.rstmma.at(numch).at(numm).at(1).at(10) << chck.rstmma.at(numch).at(numm).at(2).at(10);
            allowanceB_icj << chck.rstmma.at(numch).at(numm).at(0).at(11)<< chck.rstmma.at(numch).at(numm).at(1).at(11) << chck.rstmma.at(numch).at(numm).at(2).at(11);
            verificationS_icj << chck.rstmma.at(numch).at(numm).at(0).at(21)<< chck.rstmma.at(numch).at(numm).at(1).at(21) << chck.rstmma.at(numch).at(numm).at(2).at(21);
            verificationS2_icj << chck.rstmma.at(numch).at(numm).at(0).at(22)<< chck.rstmma.at(numch).at(numm).at(1).at(22) << chck.rstmma.at(numch).at(numm).at(2).at(22);
            allowanceB_numch_icj << allowanceB_icj;
            verificationB_numch_icj << verificationB_icj;
            verificationS_numch_icj << verificationS_icj;
            verificationS2_numch_icj << verificationS2_icj;
        }
        verificationB[uid] = verificationB_numch_icj;
        verificationS[uid] = verificationS_numch_icj;
        verificationS2[uid] = verificationS2_numch_icj;
        allowanceB[uid] = allowanceB_numch_icj;
    }
    //14
    qDebug()<<"register plane member chck";
    for (int numww = 0; numww < model.numww; numww++) {
        QUuid uid = wallNoToUuid[numww];
        QVector<QVector<float> > allowanceB_numch_icj;
        QVector<QVector<float> > verificationB_numch_icj;
        QVector<QVector<float> > verificationS_numch_icj;
        QVector<QVector<float> > verificationS2_numch_icj;
        for (int numch = 0; numch < chck.numch; numch++) {

            QVector<float> allowanceB_icj;
            QVector<float> verificationB_icj;
            QVector<float> verificationS_icj;
            QVector<float> verificationS2_icj;
            allowanceB_icj<<chck.rstwwa.at(numch).at(numww).at(14)<<chck.rstwwa.at(numch).at(numww).at(14)<<chck.rstwwa.at(numch).at(numww).at(14);
            verificationB_icj<<chck.rstwwa.at(numch).at(numww).at(14)<<chck.rstwwa.at(numch).at(numww).at(14)<<chck.rstwwa.at(numch).at(numww).at(14);
            verificationS_icj<<chck.rstwwa.at(numch).at(numww).at(14)<<chck.rstwwa.at(numch).at(numww).at(14)<<chck.rstwwa.at(numch).at(numww).at(14);
            verificationS2_icj<<chck.rstwwa.at(numch).at(numww).at(14)<<chck.rstwwa.at(numch).at(numww).at(14)<<chck.rstwwa.at(numch).at(numww).at(14);
            allowanceB_numch_icj << allowanceB_icj;
            verificationB_numch_icj << verificationB_icj;
            verificationS_numch_icj << verificationS_icj;
            verificationS2_numch_icj << verificationS2_icj;
        }
        verificationB[uid] = verificationB_numch_icj;
        verificationS[uid] = verificationS_numch_icj;
        verificationS2[uid] = verificationS2_numch_icj;
        allowanceB[uid] = allowanceB_numch_icj;
    }
    qDebug()<<"set to unified output";
    UnifiedFixedData::getInstance()->getPstnData()->setVerificationRatioBending(verificationB);
    UnifiedFixedData::getInstance()->getPstnData()->setVerificationRatioShear(verificationS);
    UnifiedFixedData::getInstance()->getPstnData()->setVerificationRatioShear2(verificationS2);
    UnifiedFixedData::getInstance()->getPstnData()->setAllowanceRateBending(allowanceB);

}

QVector<double> handleNPstn::read1DimArrayDouble(QDataStream &data, int ii) const{
    data.setFloatingPointPrecision(QDataStream::DoublePrecision);
    QVector<double> retlist;
    double tmpfl;

    for (int i = 0; i < ii; i++) {
        data >> tmpfl;
        retlist << tmpfl;
    }
    data.setFloatingPointPrecision(QDataStream::SinglePrecision);
    return retlist;
}

QVector<QVector<double>> handleNPstn::read2DimArrayDouble(QDataStream &data,int ii,int jj) const{
    QVector<QVector<double>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read1DimArrayDouble(data, jj);
    }
    return retlist;
}
QVector<QVector<QVector<double>>> handleNPstn::read3DimArrayDouble(QDataStream &data,int ii,int jj,int kk) const{
    QVector<QVector<QVector<double>>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read2DimArrayDouble(data, jj,kk);
    }
    return retlist;
}

QVector<double> handleNPstn::read1DimArrayFloat(QDataStream &data, int ii) const{
    QVector<double> retlist;
    float tmpfl;

    for (int i = 0; i < ii; i++) {
        data >> tmpfl;
        retlist << tmpfl;
    }
    return retlist;
}

QVector<QVector<double>> handleNPstn::read2DimArrayFloat(QDataStream &data,int ii,int jj) const{
    QVector<QVector<double>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read1DimArrayFloat(data, jj);
    }
    return retlist;
}

QVector<QVector<QVector<double>>> handleNPstn::read3DimArrayFloat(QDataStream &data,int ii,int jj,int kk) const{
    QVector<QVector<QVector<double>>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read2DimArrayFloat(data, jj, kk);
    }
    return retlist;
}

QVector<QVector<QVector<QVector<double>>>> handleNPstn::read4DimArrayFloat(QDataStream &data,int ii,int jj,int kk,int ll) const{
    QVector<QVector<QVector<QVector<double>>>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read3DimArrayFloat(data, jj, kk, ll);
    }
    return retlist;

}
QVector<QVector<QVector<QVector<QVector<double>>>>> handleNPstn::read5DimArrayFloat(QDataStream &data,int ii,int jj,int kk,int ll,int mm) const{
    QVector<QVector<QVector<QVector<QVector<double>>>>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read4DimArrayFloat(data, jj, kk, ll, mm);
    }
    return retlist;

}

QVector<QVector<QVector<QVector<QVector<QVector<double>>>>>> handleNPstn::read6DimArrayFloat(QDataStream &data,int ii,int jj,int kk,int ll,int mm,int nn) const{
    QVector<QVector<QVector<QVector<QVector<QVector<double>>>>>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read5DimArrayFloat(data, jj, kk, ll, mm, nn);
    }
    return retlist;
}

QVector<QVector<QVector<QVector<QVector<QVector<QVector<double>>>>>>> handleNPstn::read7DimArrayFloat(QDataStream &data,int ii,int jj,int kk,int ll,int mm,int nn, int oo) const{
    QVector<QVector<QVector<QVector<QVector<QVector<QVector<double>>>>>>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read6DimArrayFloat(data, jj, kk, ll, mm, nn, oo);
    }
    return retlist;
}



QVector<qint32> handleNPstn::read1DimArrayInt(QDataStream &data, int ii) const{
    QVector<qint32> retlist;
    qint32 tmpi;

    for (int i = 0; i < ii; i++) {
        data >> tmpi;
        retlist << tmpi;
    }
    return retlist;
}

QVector<QVector<qint32>> handleNPstn::read2DimArrayInt(QDataStream &data, int ii, int jj) const{
    QVector<QVector<qint32>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read1DimArrayInt(data, jj);
    }
    return retlist;
}

QVector<QVector<QVector<qint32>>> handleNPstn::read3DimArrayInt(QDataStream &data,int ii,int jj,int kk) const{
    QVector<QVector<QVector<qint32>>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read2DimArrayInt(data, jj, kk);
    }
    return retlist;
}

QVector<QVector<QVector<QVector<qint32>>>> handleNPstn::read4DimArrayInt(QDataStream &data,int ii,int jj,int kk,int ll) const{
    QVector<QVector<QVector<QVector<qint32>>>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read3DimArrayInt(data, jj, kk, ll);
    }
    return retlist;

}
QVector<QVector<QVector<QVector<QVector<qint32>>>>> handleNPstn::read5DimArrayInt(QDataStream &data,int ii,int jj,int kk,int ll,int mm) const{
    QVector<QVector<QVector<QVector<QVector<qint32>>>>> retlist;
    for (int i = 0; i < ii; i++) {
        retlist << read4DimArrayInt(data, jj, kk, ll, mm);
    }
    return retlist;

}
QString handleNPstn::readString(QDataStream &data,int charlen) const{
    //    char ch[charlen+1];//VLA
    if(charlen >= 300)qDebug()<<"String size > maxlength";
    char ch[300];
    ch[charlen] = '\0';

    data.readRawData(ch, charlen);
    qDebug()<<QString(ch);
    return QString(ch).trimmed();
}
QStringList handleNPstn::readStringList(QDataStream &data ,int charlen,int arraysize) const{
    //    char ch[charlen+1];
    if(charlen >= 300)qDebug()<<"String size > maxlength";
    char ch[300];
    ch[charlen] = '\0';

    QStringList retlist;

    for (int i = 0; i < arraysize; i++) {
        data.readRawData(ch, charlen);
        retlist << QString(ch).trimmed();
        qDebug()<<QString(ch);
        memset(ch, 0, static_cast<size_t>(charlen));
    }
    return retlist;

}
QVector<QStringList> handleNPstn::read2DimStringList(QDataStream &data ,int charlen,int arraysize1,int arraysize2) const{
    //    char ch[charlen+1];
    if(charlen >= 300)qDebug()<<"String size > maxlength";
    char ch[300];
    ch[charlen] = '\0';
    QStringList tmpstrlst;
    QVector<QStringList> retlist;

    for (int i = 0; i < arraysize1; i++) {
        for (int j = 0; j < arraysize2; j++) {
            data.readRawData(ch, charlen);
            tmpstrlst << QString(ch).trimmed();
            qDebug()<<QString(ch);
            //            qDebug()<<"namefl"<<tmpstrlst.last();
            memset(ch, 0, static_cast<size_t>(charlen));
        }
        retlist << tmpstrlst;
        tmpstrlst.clear();
    }
    return retlist;

}
} // namespace post3dapp

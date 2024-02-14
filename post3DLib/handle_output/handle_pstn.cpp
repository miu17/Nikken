#include "handle_pstn.h"

// #include <QtCore>
// #include <QAbstractButton>
// #include <QCoreApplication>
// #include <QDebug>
// #include <QMessageBox>
// #include <QPushButton>
// #include <QTimer>
// #include <QVector3D>
// #include "calc_vector2d.h"
// #include "calc_vector3d.h"
// #include "define_section_struct.h"
// #include "fixed_data.h"
// #include "manage_jointdata.h"
// #include "manage_memberdata.h"
// #include "unified_analysisdata.h"
// #include "unified_output.h"

namespace post3dapp
{

    // void handlePstn::sleep(unsigned long msec)
    //{
    //     QEventLoop loop;
    //     QTimer::singleShot(msec, &loop, SLOT(quit()));
    //     loop.exec();
    // }

    // int handlePstn::slotFileOpen()
    //{
    //     //extract pstn
    //     QProcess *decompgz = new QProcess;
    //     apppath = QCoreApplication::applicationDirPath();
    //     QString tmpfilename = QFileDialog::getOpenFileName( 0, QString(), QString(), "File(*.pstn*)" );

    //     if ( tmpfilename.isEmpty() ) return -1;

    //    //progress dialog
    //    QProgressDialog progress(u8"ファイル読み込み中", u8"キャンセル", 0, 100);
    //    QPushButton *cancelbutton = new QPushButton;
    //    cancelbutton->setVisible(false);
    //    progress.setAutoClose(true);
    //    progress.setWindowModality(Qt::WindowModal);
    //    progress.show();
    //    progress.setLabelText(u8"ファイル読み込み中");
    //    progress.setValue(0);

    //    if ( QFileInfo(tmpfilename).suffix() == "gz") { //gzipの場合
    //        filename = QFileInfo(tmpfilename).absolutePath() + "/" + QFileInfo(tmpfilename).completeBaseName();
    //        QFile pstnfile(filename);

    //        if (pstnfile.exists()) { //pstnがある場合は、ダイアログで解凍するか決める
    //            QMessageBox askDecompressGz(QMessageBox::Question,
    //                                        u8".gzファイルを解凍",
    //                                        QFileInfo(pstnfile).fileName() + u8"は存在します。.gzファイルを解凍しますか？",
    //                                        QMessageBox::Yes | QMessageBox::No, 0);
    //            askDecompressGz.setButtonText(QMessageBox::Yes,u8"はい");
    //            askDecompressGz.setButtonText(QMessageBox::No, u8"いいえ");
    //            int toDecompressGz = askDecompressGz.exec();
    //            if (toDecompressGz == QMessageBox::Yes) {
    //                progress.setLabelText(u8".gzファイルを解凍中");
    //                progress.setValue(10);
    //                //x：解凍　-y：強制的にYes(ファイルの上書きなど)　-o：解凍先フォルダを指定　（-ir：取り出すファイルを指定,'!'isWildCard）　解凍するファイル名
    ////                QString command = "\"" + apppath + "/7za.exe\" x -y -o" + QFileInfo(tmpfilename).absolutePath() +
    ////                                  "/ " + tmpfilename;
    //                QString command = "\""+apppath+"/7za.exe\" x -y \"-o" + QFileInfo(tmpfilename).absolutePath() + "/\" " +"\""+tmpfilename+"\"";
    //                decompgz->execute(command);
    //            } else if (toDecompressGz == QMessageBox::No) {}
    //        }
    ////        if(false){}//※pstnがあったら勝手にスキップ
    //        else if (!pstnfile.exists()) { //pstnが無い場合はgzファイルを解凍
    //            //x：解凍　-y：強制的にYes(ファイルの上書きなど)　-o：解凍先フォルダを指定　（-ir：取り出すファイルを指定,'!'isWildCard）　解凍するファイル名
    //            progress.setLabelText(u8".gzファイルを解凍中");
    //            progress.setValue(10);
    ////            QString command = "\"" + apppath + "/7za.exe\" x -y -o" + QFileInfo(tmpfilename).absolutePath() +
    ////                              "/ " + tmpfilename;
    //            QString command = "\""+apppath+"/7za.exe\" x -y \"-o" + QFileInfo(tmpfilename).absolutePath() + "/\" " +"\""+tmpfilename+"\"";
    //            decompgz->execute(command);
    //            for (int time = 0; time < 500; time++) {
    //                sleep(1000);
    //                if (pstnfile.open(QIODevice::ReadOnly))break;
    //                if (time == 499) {
    //                    progress.close();
    //                    return -1;
    //                }
    //            }
    //        }
    //    } else if ( QFileInfo(tmpfilename).suffix() == "pstn") { //pstnの場合
    //        filename = tmpfilename;
    //    } else {
    //        progress.close();
    //        return -1;
    //    }
    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }

    //    timestamp = QFileInfo(filename).lastModified();
    //    filepath = QDir(QFileInfo(filename).absolutePath());
    //    tmpdirname = QFileInfo(filename).baseName();
    //    //filename・・・パスから拡張子まで含めたpstnの名前
    //    //filepath・・・QDir型の、pstnのあるフォルダまでのパス
    //    //tmpdirname・・・QString型の、pstnの名前（パス、拡張子除く）
    //    //tmpdirpath・・・・QDir型の、filepath/tmpdirnameまでのフルパス
    //    //apppath・・・実行ファイルの場所（7za.exeを入れておく）
    //    tmpdirpath = QDir(filepath.absolutePath() + "/" + tmpdirname);

    //    if (tmpdirpath.exists()) {
    //        QMessageBox askDecompressPstn(QMessageBox::Question,
    //                                      u8".pstnファイルを解凍",
    //                                      tmpdirname + u8"フォルダーは存在します。.pstnファイルを解凍して上書きしますか？",
    //                                      QMessageBox::Yes | QMessageBox::No, 0);
    //        askDecompressPstn.setButtonText(QMessageBox::Yes, u8"はい");
    //        askDecompressPstn.setButtonText(QMessageBox::No, u8"いいえ");
    //        int askResult = askDecompressPstn.exec();
    //        if (askResult == QMessageBox::Yes) {
    //            toDecompressPstn = true;
    //        } else if (askResult == QMessageBox::No) {
    //            toDecompressPstn = false;
    //        }
    ////        toDecompressPstn=false;//ディレクトリがあったらpstnの解凍を勝手にスキップ
    //    } else if (!tmpdirpath.exists()) {
    //        toDecompressPstn = true;
    //        bool mkdirerr = filepath.mkdir(tmpdirname);
    //        if (mkdirerr == false) return -1;
    //    }
    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }

    //    //read pstn
    //    int error;
    //    QMessageBox msg(0);

    //    progress.setLabelText(tr(u8"MODEL読み込み中"));
    //    progress.setValue(40);
    //    error = readBLD3D_CMDL();
    //    msg.setText(u8"エラー ： MODEL");
    //    if (error != 0) {
    //        msg.exec();
    //        return -1;
    //    }
    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }

    //    progress.setLabelText(u8"STRESS読み込み中");
    //    progress.setValue(50);

    //    QMessageBox askReadAllSteps(QMessageBox::Question,
    //                                u8"ステップ読み込み中",
    //                                tmpdirname + u8"全ステップを読み込みますか？",
    //                                QMessageBox::Yes | QMessageBox::No, 0);
    //    askReadAllSteps.setButtonText(QMessageBox::Yes, u8"はい");
    //    askReadAllSteps.setButtonText(QMessageBox::No, u8"いいえ");
    //    int askResult = askReadAllSteps.exec();
    //    if (askResult == QMessageBox::Yes) {
    //        readAllSteps = true;
    //    } else if (askResult == QMessageBox::No) {
    //        readAllSteps = false;
    //    }
    ////    readAllSteps=false;

    //    error = readRST();
    //    qDebug() << "readRST all done";
    //    msg.setText(u8"エラー ： STRESS");
    //    if (error != 0) {
    //        msg.exec();
    //        return -1;
    //    }
    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }

    //    progress.setLabelText(u8"LOAD読み込み中");
    //    progress.setValue(60);
    //    error = readBLD3D_LOAD();
    //    msg.setText(u8"エラー ： LOAD");
    //    if (error != 0) {
    //        msg.exec();
    //        return -1;
    //    }
    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }

    //    qDebug() << "readLoad done";
    //    progress.setLabelText(u8"PREA読み込み中");
    //    progress.setValue(65);
    //    error = readBLD3D_PREA();
    //    msg.setText(u8"エラー ： PREA");
    //    if (error != 0) {
    //        msg.exec();
    //        return -1;
    //    }
    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }

    //    progress.setLabelText(u8"CHCK読み込み中");
    //    progress.setValue(70);
    //    error = readBLD3D_CHCK();

    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }
    //    qDebug() << "chck done";
    //    progress.setLabelText(u8"MODE読み込み中");
    //    progress.setValue(80);
    //    readBLD3D_MODE();
    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }

    //    progress.setLabelText(u8"データ登録中");
    //    progress.setValue(90);
    //    makeUnifiedData();
    //    qDebug() << "made uudata";
    //    if (progress.wasCanceled()) {
    //        progress.close();
    //        return -1;
    //    }
    //    qDebug() << "progress not canceled";
    //    progress.setValue(100);
    //    progress.close();
    //    qDebug() << "progress close";

    //    return 0;

    //}

    // int handlePstn::readBLD3D_CMDL()
    //{
    //     successToReadModelFile = false;
    //     model.clear();
    //     //filepath
    //     QFile cmdl(filepath.absolutePath() + "/" + tmpdirname + "/BLD3D_CMDL");
    ////    QString command = "\"" + apppath + "/7za.exe\" x -y -o" + tmpdirpath.absolutePath() +
    ////                      "/ -ir!BLD3D_CMDL " + filename;
    //    QString command = "\""+apppath+"/7za.exe\" x -y \"-o" + tmpdirpath.absolutePath() + "/\" " +"\"-ir!BLD3D_CMDL\" "+"\""+filename+"\"";
    //    if (toDecompressPstn) {
    //        QProcess *decompmdl = new QProcess;
    //        decompmdl->execute(command);
    //        qDebug() << u8"CMDLの解凍";
    //    }
    //    bool successFileOpen = cmdl.open(QIODevice::ReadOnly);
    //    if (successFileOpen == false) {
    //        for (int time = 0; time < 500; time++) {
    //            sleep(1000);
    //            if (successFileOpen == cmdl.open(QIODevice::ReadOnly))break;
    //        }
    //        return -1;
    //    }
    //    QDataStream incmdl(&cmdl);
    //    incmdl.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian
    //    cmdl.seek(92);
    //    incmdl >>
    //           model.msize >> model.nbande >> model.mxnfl >> model.numfg >> model.mxngr >> model.numgg >>
    //           model.numj >>
    //           model.mxjfl >> model.mxjgr >> model.mxjm >> model.numrld >> model.numrl >> model.maxsl >>
    //           model.mjslvd >>
    //           model.mjslv >> model.mxid4 >> model.numd4d >> model.numd4 >> model.numasd >> model.numas >>
    //           model.numacd >>
    //           model.numac >> model.numard >> model.numar >> model.numrrd >> model.numrr >> model.numlsd >>
    //           model.numls >>
    //           model.mxjs >> model.mxss >> model.mxjc >> model.mxcc >> model.mxjr >> model.mxrr >> model.nummd >>
    //           model.numm >>
    //           model.numbl >> model.numswd >> model.numsw >> model.mxso >> model.numsod >> model.numso >>
    //           model.mxww >>
    //           model.numwwd >> model.numww >> model.mxwz >> model.numwzd >> model.numwz >> model.mxmw >>
    //           model.nummwd >>
    //           model.nummw >> model.mxmz >> model.nummzd >> model.nummz >> model.numzzd >> model.numzz >>
    //           model.numwsd >>
    //           model.numws >> model.numapd >> model.numap >> model.numspd >> model.numsp >> model.numpd >>
    //           model.nump >>
    //           model.numabd >> model.numab >> model.numbjd >> model.numbj >> model.numbzd >> model.numbz >>
    //           model.numbnd >>
    //           model.numbn >> model.numbqd >> model.numbq >> model.lbymax >> model.numbyd >> model.numby >>
    //           model.numbsd >>
    //           model.numbs >> model.numbtd >> model.numbt >> model.numbwd >> model.numbw >> model.nummtd >>
    //           model.nummt >>
    //           model.numbd >> model.numb >> model.numcol >> model.mxgrj >> model.mxgrm >> model.mxgrw >>
    //           model.iexmcb >>
    //           model.nwk >> model.nwke >> model.numsdp >> model.mxmems >> model.numemsd >> model.numems;
    //    cmdl.seek(cmdl.pos() + 12); //numeid,numei,jsize読み飛ばし

    //    //値を取り出す際の仮の変数・コンテナ
    //    qint32 tmpi;
    //    float tmpfl;
    //    double tmpdbl;
    //    QVector<qint32> tmplst;
    //    QVector<QVector<qint32> > tmplstlst;
    //    QVector<float> tmplstfl;
    //    QVector<QVector<float> > tmplstlstfl;
    //    QVector<double> tmplstdbl;
    //    QStringList tmpstrlst;
    //    char ch80[81];
    //    ch80[80] = '\0';
    //    char ch35[36];
    //    ch35[35] = '\0';
    //    char ch16[17];
    //    ch16[16] = '\0';
    //    char ch10[11];
    //    ch10[10] = '\0';
    //    char ch8[9];
    //    ch8[8] = '\0';
    //    char ch6[7];
    //    ch6[6] = '\0';

    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    incmdl.readRawData(ch80, 80);
    //    model.title = QString(ch80);
    ////    qDebug()<<model.title;
    //    memset(ch80, 0, 80);
    //    cmdl.seek(cmdl.pos() + 29); //IROUTE[NXY],IUNIT,DTIME*17 skip
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //numfl[numfg]
    //    for (int i = 0; i < model.numfg; i++) {
    //        incmdl >> tmpi;
    //        model.numfl << tmpi;
    //    }
    //    //kfl[numfg][mxnfl]
    //    for (int i = 0; i < model.numfg; i++) {
    //        for (int j = 0; j < model.mxnfl; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.kfl << tmplst;
    //        tmplst.clear();
    //    }
    //    //namefl[numfg][mxnfl]
    //    for (int i = 0; i < model.numfg; i++) {
    //        for (int j = 0; j < model.mxnfl; j++) {
    //            incmdl.readRawData(ch6, 6);
    //            tmpstrlst << QString(ch6);
    ////            qDebug()<<"namefl"<<tmpstrlst.last();
    //            memset(ch6, 0, 6);
    //        }
    //        model.namefl << tmpstrlst;
    //        tmpstrlst.clear();
    //    }
    //    //IFL[MXNFL][NUMFG]
    //    for (int i = 0; i < model.numfg; i++) {
    //        for (int j = 0; j < model.mxnfl; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.ifl << tmplst;
    //        tmplst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8 * model.mxnfl * model.numfg); //HFL[MXNFL][NUMFG]
    //    //AHFL[NUMFG][MXNFL]
    //    incmdl.setFloatingPointPrecision(QDataStream::DoublePrecision);
    //    for (int i = 0; i < model.numfg; i++) {
    //        for (int j = 0; j < model.mxnfl; j++) {
    //            incmdl >> tmpdbl;
    //            tmplstdbl << tmpdbl;
    //        }
    //        model.ahfl << tmplstdbl;
    //        tmplstdbl.clear();
    //    }
    //    incmdl.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //    cmdl.seek(cmdl.pos() + 4 * model.nxy * model.mxnfl * model.numfg); //LSFL[NXY][MXNFL][NUMFG]
    //    //LFG[NLFG][NUMFG]
    //    for (int i = 0; i < model.nlfg; i++) {
    //        for (int j = 0; j < model.numfg; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.lfg << tmplst;
    //        tmplst.clear();
    //    }
    //    incmdl.setFloatingPointPrecision(QDataStream::DoublePrecision);
    //    for (int i = 0; i < model.numfg; i++) {
    //        incmdl >> tmpdbl;    //dfg[numfg]
    //        model.dfg << tmpdbl;
    //    }
    //    incmdl.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //    cmdl.seek(cmdl.pos() + 8 * model.ndfl * model.mxnfl * model.numfg); //DFL[NDFL][MXNFL][NUMFG]
    //    cmdl.seek(cmdl.pos() + 4 * model.mxnfl * model.numfg); //ICFL[MXNFL][NUMFG]
    //    cmdl.seek(cmdl.pos() + 8 * model.nrofg * model.numfg); //ROFG[NROFG][NUMFG]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    for (int i = 0; i < model.numgg; i++) {
    //        incmdl >> tmpi;    //numgr[numgg]
    //        model.numgr << tmpi;
    //    }
    //    //namegr[numgg][mxngr]
    //    for (int i = 0; i < model.numgg; i++) {
    //        for (int j = 0; j < model.mxngr; j++) {
    //            incmdl.readRawData(ch6, 6);
    //            tmpstrlst << QString(ch6);
    ////            qDebug()<<"namegr"<<tmpstrlst.last();
    //            memset(ch6, 0, 6);
    //        }
    //        model.namegr << tmpstrlst;
    //        tmpstrlst.clear();
    //    }
    //    //IGG[numgg]
    //    for (int i = 0; i < model.numgg; i++) {
    //        incmdl >> tmpi;
    //        model.igg << tmpi;
    //    }
    //    //DGG[numgg][ndgg]
    //    incmdl.setFloatingPointPrecision(QDataStream::DoublePrecision);
    //    for (int i = 0; i < model.numgg; i++) {
    //        for (int j = 0; j < model.ndgg; j++) {
    //            incmdl >> tmpdbl;
    //            tmplstdbl << tmpdbl;
    //        }
    //        model.dgg << tmplstdbl;
    //        tmplstdbl.clear();
    //    }
    //    incmdl.setFloatingPointPrecision(QDataStream::SinglePrecision);

    //    //DGR[numgg][mxngr]
    //    incmdl.setFloatingPointPrecision(QDataStream::DoublePrecision);
    //    for (int i = 0; i < model.numgg; i++) {
    //        for (int j = 0; j < model.mxngr; j++) {
    //            incmdl >> tmpdbl;
    //            tmplstdbl << tmpdbl;
    //        }
    //        model.dgr << tmplstdbl;
    //        tmplstdbl.clear();
    //    }
    //    incmdl.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //    //LGR[numgg][mxngr]
    //    for (int i = 0; i < model.numgg; i++) {
    //        for (int j = 0; j < model.mxngr; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.lgr << tmplst;
    //        tmplst.clear();
    //    }

    //    cmdl.seek(cmdl.pos() + 4 * model.numgg * model.mxngr * model.numfg *
    //              model.mxnfl); //JSTGR[numgg][mxngr][numfg][mxnfl]

    //    cmdl.seek(cmdl.pos() + 8 * model.numgg * model.mxngr); //ASTGR[numgg][mxngr]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    for (int i = 0; i < model.numj; i++) {
    //        incmdl >> tmpi;    //JNTNM[numj]
    //        model.jntnm << tmpi;
    //    }
    //    //NAMEJ[numj]*35
    //    for (int i = 0; i < model.numj; i++) {
    //        incmdl.readRawData(ch35, 35);
    //        model.namej << QString(ch35);
    //        memset(ch35, 0, 35);
    //    }
    //    //INDJ[numj][mxjdof]<-OK
    //    for (int i = 0; i < model.numj; i++) {
    //        for (int j = 0; j < model.mxjdof; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.indj << tmplst;
    //        tmplst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 4 * (model.msize + 1)); //MAXA[msize+1]
    //    cmdl.seek(cmdl.pos() + 4 * (model.msize + 1)); //MAXAE[msize+1]
    //    cmdl.seek(cmdl.pos() + 4 * model.msize); //IVPRM[msize]
    //    cmdl.seek(cmdl.pos() + 4 * model.msize); //LPERM[msize]<-OK
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //XYZ[numj][ndim]!
    //    for (int i = 0; i < model.numj; i++) {
    //        for (int j = 0; j < model.ndim; j++) {
    //            incmdl >> tmpfl;
    //            tmplstfl << tmpfl;
    //        }
    //        model.xyz << tmplstfl;
    //        tmplstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //NJFL[numj]!
    //    for (int i = 0; i < model.numj; i++) {
    //        incmdl >> tmpi;
    //        model.njfl << tmpi;
    //    }
    //    //JNTFL[numj][mxjfl][njgk]!
    //    for (int i = 0; i < model.numj; i++) {
    //        for (int j = 0; j < model.mxjfl; j++) {
    //            for (int k = 0; k < model.njgk; k++) {
    //                incmdl >> tmpi;
    //                tmplst << tmpi;
    //            }
    //            tmplstlst << tmplst;
    //            tmplst.clear();
    //        }
    //        model.jntfl << tmplstlst;
    //        tmplstlst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head]
    //    //NJGR[numj]!
    //    for (int i = 0; i < model.numj; i++) {
    //        incmdl >> tmpi;
    //        model.njgr << tmpi;
    //    }
    //    //JNTGR[numj][mxjgr][njgk]!
    //    for (int i = 0; i < model.numj; i++) {
    //        for (int j = 0; j < model.mxjgr; j++) {
    //            for (int k = 0; k < model.njgk; k++) {
    //                incmdl >> tmpi;
    //                tmplst << tmpi;
    //            }
    //            tmplstlst << tmplst;
    //            tmplst.clear();
    //        }
    //        model.jntgr << tmplstlst;
    //        tmplstlst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numj); //NJMEM[numj]
    //    cmdl.seek(cmdl.pos() + 4 * model.numj * model.mxjm); //JMEM[numj][mxjm]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numj); //JNTPP[numj]
    //    cmdl.seek(cmdl.pos() + 4 * model.numj); //JNTRL[numj]
    //    cmdl.seek(cmdl.pos() + 4 * model.numj); //JRGDF[numj]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numrld * model.ndrl); //DRL[numrld][ndrl]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //JNTI[mjslvd]!
    //    for (int i = 0; i < model.mjslvd; i++) {
    //        incmdl >> tmpi;
    //        model.jnti << tmpi;
    //    }
    //    //JNTO[mjslvd][maxsl]!
    //    for (int i = 0; i < model.mjslvd; i++) {
    //        for (int j = 0; j < model.maxsl; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.jnto << tmplst;
    //        tmplst.clear();
    //    }
    //    //JNTOM[mjslvd][maxsl]!
    //    for (int i = 0; i < model.mjslvd; i++) {
    //        for (int j = 0; j < model.maxsl; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.jntom << tmplst;
    //        tmplst.clear();
    //    }
    ////    qDebug()<<"jntom"<<model.jntom[0][0];

    //    //NUMSL[mjslvd]!
    //    for (int i = 0; i < model.mjslvd; i++) {
    //        incmdl >> tmpi;
    //        model.numsl << tmpi;
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head

    //    //COSL[mjslvd][maxsl]!
    //    for (int i = 0; i < model.mjslvd; i++) {
    //        for (int j = 0; j < model.maxsl; j++) {
    //            incmdl >> tmpfl;
    //            tmplstfl << tmpfl;
    //        }
    //        model.cosl << tmplstfl;
    //        tmplstfl.clear();
    //    }
    ////    qDebug()<<"cosl"<<model.cosl[0][0];

    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numd4d); //NCID4[numd4d]
    //    cmdl.seek(cmdl.pos() + 4 * model.numd4d * model.mxid4); //ID4[numd4d][mxid4]
    //    cmdl.seek(cmdl.pos() + 6 * model.numd4d); //NAMED4[numd4d]*6
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numd4d * model.ndd4); //DD4[numd4d][ndd4]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 8 * model.numasd); //NAMEAS[numasd]*8
    //    cmdl.seek(cmdl.pos() + 8 * model.numacd); //NAMEAC[numacd]*8
    //    cmdl.seek(cmdl.pos() + 8 * model.numard); //NAMEAR[numard]*8
    //    cmdl.seek(cmdl.pos() + 3 * model.numrrd); //NAMERR[numrrd]*3
    //    cmdl.seek(cmdl.pos() + 4 * model.numrrd); //IRR[numrrd]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //DAS[numasd][ndas]!
    //    cmdl.seek(cmdl.pos() + 4 * model.numasd * model.ndas);
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //DAC[numacd][ndac]!
    //    cmdl.seek(cmdl.pos() + 4 * model.numacd * model.ndac);
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numard * model.ndar); //DAR[numard][ndar]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numrrd * model.ndrr); //DRR[numrrd][ndrr]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //NAMELS[numlsd]*16!
    //    for (int i = 0; i < model.numlsd; i++) {
    //        incmdl.readRawData(ch16, 16);
    //        model.namels << QString(ch16);
    ////        qDebug()<<QString(ch16); ->OK
    //        memset(ch16, 0, 16);
    //    }
    //    //ILS1[numlsd][nils1]!
    //    for (int i = 0; i < model.numlsd; i++) {
    //        for (int j = 0; j < model.nils1; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.ils1 << tmplst;
    //        tmplst.clear();
    //    }
    //    //ILS2[numlsd][nicj][nils2]!
    //    for (int i = 0; i < model.numlsd; i++) {
    //        for (int j = 0; j < model.nicj; j++) {
    //            for (int k = 0; k < model.nils2; k++) {
    //                incmdl >> tmpi;
    //                tmplst << tmpi;
    //            }
    //            tmplstlst << tmplst;
    //            tmplst.clear();
    //        }
    //        model.ils2 << tmplstlst;
    //        tmplstlst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //DLS1[numlsd][nicj][ndls1]!
    //    for (int i = 0; i < model.numlsd; i++) {
    //        for (int j = 0; j < model.nicj; j++) {
    //            for (int k = 0; k < model.ndls1; k++) {
    //                incmdl >> tmpfl;
    //                tmplstfl << tmpfl;
    //            }
    //            tmplstlstfl << tmplstfl;
    //            tmplstfl.clear();
    //        }
    //        model.dls1 << tmplstlstfl;
    //        tmplstlstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.ndls2); //DLS2[numlsd][nicj][ndls2]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.ndls3); //DLS3[numlsd][nicj][ndls3]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxss); //ISTY[numlsd][nicj][mxss]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxss); //ISTYZ[numlsd][nicj][mxss]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj); //NNS[numlsd][nicj]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxss); //NJS[numlsd][nicj][mxss]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxss * model.mxjs *
    //              model.nyz); //XYS0[numlsd][nicj][mxss][mxjs][nyz]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxss); //es[numlsd][nicj][mxss]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxss * model.nlsu *
    //              model.nftbc); //sgms[numlsd][nicj][mxss][nlsu][nftbc]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxcc); //icty[numlsd][nicj][mxcc]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxcc); //ictyz[numlsd][nicj][mxcc]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj); //nnc[numlsd][nicj]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxcc); //njc[numlsd][nicj][mxcc]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxcc * model.mxjc *
    //              model.nyz); //xyc0[numlsd][nicj][mxcc][mxjc][nyz]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxcc); //ec[numlsd][nicj][mxcc]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxcc *
    //              model.nlsu); //sgmc[numlsd][nicj][mxcc][nlsu]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr); //irty[numlsd][nicj][mxrr]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr); //irtyz[numlsd][nicj][mxrr]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj); //nnr[numlsd][nicj]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr); //njr[numlsd][nicj][mxrr]
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr); //iconr[numlsd][nicj][mxrr]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr * model.mxjr *
    //              model.nyz); //xyr0[numlsd][nicj][mxrr][mxjr][nyz]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr); //er[numlsd][nicj][mxrr]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr *
    //              model.nlsu); //sgmr[numlsd][nicj][mxrr][nlsu]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr *
    //              model.mxjr); //ar[numlsd][nicj][mxrr][mxjr]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr *
    //              model.mxjr); //ap[numlsd][nicj][mxrr][mxjr]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr *
    //              model.mxjr); //psepr[numlsd][nicj][mxrr][mxjr]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numlsd * model.nicj * model.mxrr *
    //              model.mxjr); //psaxr[numlsd][nicj][mxrr][mxjr]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //imm1[nummd][nimm1]!
    //    for (int i = 0; i < model.nummd; i++) {
    //        for (int j = 0; j < model.nimm1; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.imm1 << tmplst;
    //        tmplst.clear();
    //    }
    //    //indm[nummd][nij]!
    //    for (int i = 0; i < model.nummd; i++) {
    //        for (int j = 0; j < model.nij; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.indm << tmplst;
    //        tmplst.clear();
    //    }
    //    //ifpmm[nummd][mxpin]!
    //    for (int i = 0; i < model.nummd; i++) {
    //        for (int j = 0; j < model.mxpin; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.ifpmm << tmplst;
    //        tmplst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 4 * model.nummd * model.nindmes); //indmes[nummd][nindmes]
    //    cmdl.seek(cmdl.pos() + 4 * model.numemsd * model.mxmems); //mems[numemsd][mxmems]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //dmm1[nummd][ndmm1]!
    //    for (int i = 0; i < model.nummd; i++) {
    //        for (int j = 0; j < model.ndmm1; j++) {
    //            incmdl >> tmpfl;
    //            tmplstfl << tmpfl;
    //        }
    //        model.dmm1 << tmplstfl;
    //        tmplstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //evmem[nummd][nxy][ndim]!
    //    for (int i = 0; i < model.nummd; i++) {
    //        for (int j = 0; j < model.nxy; j++) {
    //            for (int k = 0; k < model.ndim; k++) {
    //                incmdl >> tmpfl;
    //                tmplstfl << tmpfl;
    //            }
    //            tmplstlstfl << tmplstfl;
    //            tmplstfl.clear();
    //        }
    //        model.evmem << tmplstlstfl;
    //        tmplstlstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //namebl[numbl]*6!
    //    for (int i = 0; i < model.numbl; i++) {
    //        incmdl.readRawData(ch6, 6);
    //        model.namebl << QString(ch6);
    ////        qDebug()<<"bl"<<QString(ch6);
    //    }
    //    //mbl[numbl][nibl]!
    //    for (int i = 0; i < model.numbl; i++) {
    //        for (int j = 0; j < model.nibl; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.mbl << tmplst;
    //        tmplst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numbl * model.ndbl); //dbl[numbl][ndbl]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //namesw[numswd]*8!<-OK
    //    for (int i = 0; i < model.numswd; i++) {
    //        incmdl.readRawData(ch8, 8);
    //        model.namesw << QString(ch8);
    ////        qDebug()<<"sw"<<QString(ch8);
    //        memset(ch8, 0, 8);
    //    }
    //    //isw[numswd][nisw]!
    //    for (int i = 0; i < model.numswd; i++) {
    //        for (int j = 0; j < model.nisw; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.isw << tmplst;
    //        tmplst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //dsw[numswd][ndsw]!
    //    for (int i = 0; i < model.numswd; i++) {
    //        for (int j = 0; j < model.ndsw; j++) {
    //            incmdl >> tmpfl;
    //            tmplstfl << tmpfl;
    //        }
    //        model.dsw << tmplstfl;
    //        tmplstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //開口
    //    //nameso[numsod]*8
    //    for (int i = 0; i < model.numsod; i++) {
    //        incmdl.readRawData(ch8, 8);
    //        model.nameso << QString(ch8);
    ////        qDebug()<<"so"<<QString(ch8);
    //        memset(ch8, 0, 8);
    //    }
    //    //iso[numsod][mxso][niso]
    //    for (int i = 0; i < model.numsod; i++) {
    //        for (int j = 0; j < model.mxso; j++) {
    //            for (int k = 0; k < model.niso; k++) {
    //                incmdl >> tmpi;
    //                tmplst << tmpi;
    //            }
    //            tmplstlst << tmplst;
    //            tmplst.clear();
    //        }
    //        model.iso << tmplstlst;
    //        tmplstlst.clear();
    //    }
    //    //nso[numsod]
    //    for (int i = 0; i < model.numsod; i++) {
    //        incmdl >> tmpi;
    //        model.nso << tmpi;
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //dso[numsod][mxso][ndso]
    //    for (int i = 0; i < model.numsod; i++) {
    //        for (int j = 0; j < model.mxso; j++) {
    //            for (int k = 0; k < model.ndso; k++) {
    //                incmdl >> tmpfl;
    //                tmplstfl << tmpfl;
    //            }
    //            tmplstlstfl << tmplstfl;
    //            tmplstfl.clear();
    //        }
    //        model.dso << tmplstlstfl;
    //        tmplstlstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //iww1[numwwd][niww1]!
    //    for (int i = 0; i < model.numwwd; i++) {
    //        for (int j = 0; j < model.niww1; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.iww1 << tmplst;
    //        tmplst.clear();
    //    }
    //    //iww2[numwwd][mxww]!
    //    for (int i = 0; i < model.numwwd; i++) {
    //        for (int j = 0; j < model.mxww; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.iww2 << tmplst;
    //        tmplst.clear();
    //    }
    //    //cmdl.seek(cmdl.pos()+4*model.numwwd*model.mxww);
    //    //iww3[numwwd][mxww]
    //    for (int i = 0; i < model.numwwd; i++) {
    //        for (int j = 0; j < model.mxww; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.iww3 << tmplst;
    //        tmplst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //dww1[numwwd][ndww1]!
    //    for (int i = 0; i < model.numwwd; i++) {
    //        for (int j = 0; j < model.ndww1; j++) {
    //            incmdl >> tmpfl;
    //            tmplstfl << tmpfl;
    //        }
    //        model.dww1 << tmplstfl;
    //        tmplstfl.clear();
    //    }
    ////    qDebug()<<model.dww1[0][0];
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //evmemw[numwwd][nxz][ndim]!
    //    for (int i = 0; i < model.numwwd; i++) {
    //        for (int j = 0; j < model.nxz; j++) {
    //            for (int k = 0; k < model.ndim; k++) {
    //                incmdl >> tmpfl;
    //                tmplstfl << tmpfl;
    //            }
    //            tmplstlstfl << tmplstfl;
    //            tmplstfl.clear();
    //        }
    //        model.evmemw << tmplstlstfl;
    //        tmplstlstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //iwz1[numwzd][niwz1]
    //    for (int i = 0; i < model.numwzd; i++) {
    //        for (int j = 0; j < model.niwz1; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.iwz1 << tmplst;
    //        tmplst.clear();
    //    }
    //    //iwz2[numwzd][mxwz]
    //    for (int i = 0; i < model.numwzd; i++) {
    //        for (int j = 0; j < model.mxwz; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.iwz2 << tmplst;
    //        tmplst.clear();
    //    }
    ////    qDebug()<<model.iwz2[0][0];
    //    cmdl.seek(cmdl.pos() + 4 * model.numwzd * model.mxwz); //iwz3[numwzd][mxwz]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numwzd * model.ndwz1); //dwz1[numwzd][ndwz1]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.nummwd); //nmw[nummwd]
    //    cmdl.seek(cmdl.pos() + 4 * model.nummwd * model.mxmw); //imw[nummwd][mxmw]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.nummwd * model.mxmw); //dmw[nummwd][mxmw]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.nummzd); //nmz
    //    cmdl.seek(cmdl.pos() + 4 * model.nummzd * model.mxmz); //imz
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.nummzd * model.mxmz); //dmz[nummzd][mxmz]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numzzd * model.nizz); //izz[numzzd][nizz]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    cmdl.seek(cmdl.pos() + 4 * model.numzzd * model.nzdmax * model.ndzz); //dzz[numzzd][nzdmax][ndzz]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //iws1[numwsd][niws1]!
    //    for (int i = 0; i < model.numwsd; i++) {
    //        for (int j = 0; j < model.niws1; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.iws1 << tmplst;
    //        tmplst.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 4 * model.numwsd); //itypws[numwsd]
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //debws[numwsd][ndebws]!
    //    for (int i = 0; i < model.numwsd; i++) {
    //        for (int j = 0; j < model.ndebws; j++) {
    //            incmdl >> tmpfl;
    //            tmplstfl << tmpfl;
    //        }
    //        model.debws << tmplstfl;
    //        tmplstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //evws[numwsd][nxz][ndim]!
    //    for (int i = 0; i < model.numwsd; i++) {
    //        for (int j = 0; j < model.nxz; j++) {
    //            for (int k = 0; k < model.ndim; k++) {
    //                incmdl >> tmpfl;
    //                tmplstfl << tmpfl;
    //            }
    //            tmplstlstfl << tmplstfl;
    //            tmplstfl.clear();
    //        }
    //        model.evws << tmplstlstfl;
    //        tmplstlstfl.clear();
    //    }

    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //IAP[numapd][niap] ISP[numspd][nisp] IPP[numpd][nipp]
    //    cmdl.seek(cmdl.pos() + 4 * model.numapd * model.niap);
    //    cmdl.seek(cmdl.pos() + 4 * model.numspd * model.nisp);
    //    cmdl.seek(cmdl.pos() + 4 * model.numpd * model.nipp);
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //DAP[numapd][ndap]
    //    cmdl.seek(cmdl.pos() + 4 * model.numapd * model.ndap);
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //DSP[numspd][ndsp]
    //    cmdl.seek(cmdl.pos() + 4 * model.numspd * model.ndsp);
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //EVPP[numpd][nxy][ndim]
    //    cmdl.seek(cmdl.pos() + 4 * model.numpd * model.nxy * model.ndim);
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //IAB[numabd][niab]
    //    cmdl.seek(cmdl.pos() + 4 * model.numabd * model.niab);
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //IBJ[numbjd][nibj] IBN[numbnd][nibn] IBQ[numbqd][nibq] IBY[numbyd][niby] IBS[numbsd][nibs] IBT[numbtd][nibt]
    //    for (int i = 0; i < model.numbjd; i++) {
    //        for (int j = 0; j < model.nibj; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.ibj << tmplst;
    //        tmplst.clear();
    //    }
    //    for (int i = 0; i < model.numbnd; i++) {
    //        for (int j = 0; j < model.nibn; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.ibn << tmplst;
    //        tmplst.clear();
    //    }
    //    for (int i = 0; i < model.numbqd; i++) {
    //        for (int j = 0; j < model.nibq; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.ibq << tmplst;
    //        tmplst.clear();
    //    }
    //    for (int i = 0; i < model.numbyd; i++) {
    //        for (int j = 0; j < model.niby; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.iby << tmplst;
    //        tmplst.clear();
    //    }
    //    for (int i = 0; i < model.numbsd; i++) {
    //        for (int j = 0; j < model.nibs; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.ibs << tmplst;
    //        tmplst.clear();
    //    }
    //    for (int i = 0; i < model.numbtd; i++) {
    //        for (int j = 0; j < model.nibt; j++) {
    //            incmdl >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        model.ibt << tmplst;
    //        tmplst.clear();
    //    }
    //    //IBW IMT IBZ CBZ CMT
    //    cmdl.seek(cmdl.pos() + 4 * model.numbwd * model.nibw);
    //    cmdl.seek(cmdl.pos() + 4 * model.nummtd * model.nimt);
    //    cmdl.seek(cmdl.pos() + 4 * model.numbzd * model.nibz);
    //    for (int i = 0; i < model.numbzd; i++) {
    //        incmdl.readRawData(ch10, 10);
    //        model.cbz << QString(ch10);
    //        memset(ch10, 0, 10);
    //    }
    //    cmdl.seek(cmdl.pos() + 16 * model.nummtd); //CMT
    //    cmdl.seek(cmdl.pos() + 8); //foot head

    //    //DAB
    //    for (int i = 0; i < model.numabd; i++) {
    //        for (int j = 0; j < model.ndab; j++) {
    //            incmdl >> tmpfl;
    //            tmplstfl << tmpfl;
    //        }
    //        model.dab << tmplstfl;
    //        tmplstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //DBJ
    //    for (int i = 0; i < model.numbjd; i++) {
    //        for (int j = 0; j < model.ndbj; j++) {
    //            incmdl >> tmpfl;
    //            tmplstfl << tmpfl;
    //        }
    //        model.dbj << tmplstfl;
    //        tmplstfl.clear();
    //    }
    //    cmdl.seek(cmdl.pos() + 8); //foot head
    //    //DBZ
    //    //DBN
    //    //DBQ
    //    //DBY
    //    //DBS
    //    //DBT
    //    //DBW
    //    //NSPBY JBYCON
    //    //DBYCON
    //    //DMME
    //    //DZZE
    //    //IF(ISDP DSDP)
    //    //PNLJ1
    //    //IMZTYP
    //    //IFLAG
    //    //ISWOR
    //    //IEI
    //    //DEI
    //    //NAMEEI
    //    //ITYPBN
    //    //ITYPBY
    //    //SECJNT
    //    cmdl.close();
    //    successToReadModelFile = true;

    //    return 0;

    //}

    // int handlePstn::readRST()
    //{
    //     successToReadSomeRstFile = false;
    //     rst.clear();
    //     stresscase.clear();
    //     if (!toDecompressPstn) {
    //         tmpdirpath.setNameFilters(QStringList() << "RST*");
    //         tmpdirpath.setFilter(QDir::Files | QDir::NoSymLinks);
    //         stresscase = tmpdirpath.entryList();
    //         if (stresscase.isEmpty())return -1;
    //     }

    //    //filepath
    //    else {
    ////        QString getlistcommand = "\"" + apppath + "/7za.exe\" l -y  -ir!RST* -xr!*.pun -xr!*QD " + filename;
    //        QString getlistcommand = "\""+apppath+"/7za.exe\" l -y  -ir!RST* -xr!*.pun -xr!*QD "+"\""+filename+"\"";
    //        QProcess getlist;
    //        getlist.start(getlistcommand);
    //        getlist.waitForFinished();
    //        QString output(getlist.readAllStandardOutput());
    //        QStringList outputlist = output.split("\n");
    //        for (int i = 0; i < outputlist.count(); i++) {
    //            QString line = outputlist.at(i);
    //            if (line.left(2) != "20")continue;
    //            if (line.left(2) == "20")stresscase << line.remove(0,
    //                                                                   53).trimmed(); //20XXから始まる行、54文字目から呼んで末尾はtrim。2100年問題
    //        }
    //        stresscase.pop_back();
    //        if (stresscase.isEmpty())return -1;
    //    }

    //    for (int stcase = 0; stcase < stresscase.count(); stcase++) {
    //        QString rstname = stresscase.at(stcase);
    //        QString command = "\""+this->apppath+"/7za.exe\" x -y \"-o" + this->tmpdirpath.absolutePath() + "/\" " +"\"-ir!"+rstname+"\" \""+this->filename+"\"";
    //        if (this->toDecompressPstn) {
    //            QProcess decomprst;
    //            decomprst.start(command);
    //            decomprst.waitForFinished(300000);
    //        }
    //        QFile rstfile(this->filepath.absolutePath() + "/" + this->tmpdirname + "/" + rstname);
    //        bool successFileOpen = rstfile.open(QIODevice::ReadOnly);
    //        if (successFileOpen == false)return -1;

    //        QDataStream inrst(&rstfile);
    //        inrst.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //        DataBLD3D_RST stress;
    //        QList<DataBLD3D_RST> stresscase;

    //        qint32 tmpi;
    //        float tmpfl;
    //        QVector<qint32> tmplst;
    //        QVector<float> tmplstfl;
    //        QVector<double> tmplstdbl;
    //        QVector<QVector<float> > tmplstlstfl;
    //        int stepdatasize = 8 //iastep,iend除く
    //                           + 4 * this->model.numj * this->model.mxjdof + 8
    //                           + 4 * this->model.numj * this->model.mxjdof + 8
    //                           + 4 * this->model.numj * this->model.mxjdof + 8
    //                           + 4 * this->model.nummd * stress.nrst + 8
    //                           + 4 * this->model.nummd * this->model.nij * stress.nphai + 8
    //                           + 4 * this->model.nummd * stress.nipspm
    //                           + 8 * this->model.nummd * stress.nductm
    //                           + 4 * this->model.nummd * stress.nipspm + 8
    //                           + 4 * this->model.numwwd * stress.nelbww * stress.nrstww + 8
    //                           + 4 * this->model.numwwd * stress.nipspw + 8
    //                           + 4 * this->model.numwsd * stress.nadfws + 8
    //                           + 4 * this->model.numwsd * stress.ngaus * stress.nrstws + 8
    //                           + 4 * this->model.numpd * stress.npdim + 8
    //                           + 4 * this->model.numpd * stress.npdim + 8
    //                           + 4 * this->model.numbd + 8
    //                           + 4 * this->model.numbd + 8
    //                           + 4 * this->model.numbyd * stress.nrstby + 8
    //                           + 4 * this->model.numbyd + 8
    //                           + 4 * this->model.numbsd * stress.nrstbs + 8
    //                           ;//kcasolの手前まで
    //        rstfile.seek(150);//この間何のデータがあるか要確認

    //        int step = 0;

    //        while (true) { //最終ステップまでループ
    //            if (!this->readAllSteps) {
    //                stress.clear();
    //                inrst >> stress.iastep >> stress.iend;

    //                rstfile.seek(rstfile.pos() + stepdatasize);
    //                step++;

    //                //kcasol
    //                inrst >> stress.kcasol;
    //                                rstfile.seek(rstfile.pos() + 8); //foot head
    //                //icase[kcasol]
    //                for (int i = 0; i < stress.kcasol; i++) {
    //                    inrst >> tmpi;
    //                    stress.icase << tmpi;
    //                }
    //                //ccase[kcasol]
    //                inrst.setFloatingPointPrecision(QDataStream::DoublePrecision);
    //                for (int i = 0; i < stress.kcasol; i++) {
    //                    inrst >> tmpfl;
    //                    stress.ccase << tmpfl;
    //                }
    //                inrst.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //                rstfile.seek(rstfile.pos() + 4); //foot

    //                //--static damper--
    //                //dsdpv[numsdp][ndsdpv]  if numsdp g.t. 0
    //                if (this->model.numsdp > 0) {
    //                    rstfile.seek(rstfile.pos() + 4); //head
    //                    for (int i = 0; i < this->model.numsdp; i++) {
    //                        for (int j = 0; j < stress.ndsdpv; j++) {
    //                            inrst >> tmpfl;
    //                            tmplstfl << tmpfl;
    //                        }
    //                        stress.dsdpv << tmplstfl;
    //                        tmplstfl.clear();
    //                    }
    //                    rstfile.seek(rstfile.pos() + 4); //foot
    //                }
    //                if (rstfile.seek(rstfile.pos() + 4) == false) {
    //                    //1step巻き戻す
    //                    rstfile.seek(rstfile.pos() - 4 - 4 - stepdatasize
    //                                 - 4 - 8 - 4 * stress.kcasol - 8 * stress.kcasol - 4
    //                                 - 4);
    //                    if (this->model.numsdp > 0) {
    //                        rstfile.seek(rstfile.pos() - 4 - 4 * this->model.numsdp * stress.ndsdpv - 4);
    //                    }
    //                } else {
    //                    qint32 nextstep, nextend;
    //                    inrst >> nextstep >> nextend; //qDebug()<<"next="<<nextstep<<nextend;

    //                    if (nextstep <= stress.iastep || nextend != 0) {
    //                        //1step+nextstep,nextend巻き戻す
    //                        rstfile.seek(rstfile.pos() - 4 - 4 - stepdatasize
    //                                     - 4 - 8 - 4 * stress.kcasol - 8 * stress.kcasol - 4
    //                                     - 4
    //                                     - 4 - 4);
    //                        if (this->model.numsdp > 0) {
    //                            rstfile.seek(rstfile.pos() - 4 - 4 * this->model.numsdp * stress.ndsdpv - 4);
    //                        }
    //                    } else {
    //                        rstfile.seek(rstfile.pos() - 8);    //nextstep,nextend巻き戻す
    //                        continue;
    //                    }
    //                }

    //            }
    //            //1step分seekして次のステップがあったら読み飛ばしてkcasolから先を読んで次のステップへ。無かったら巻き戻して読み込み
    //            stress.clear();

    //            inrst >> stress.iastep >> stress.iend;
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //ujnt[numj][mxjdof] 変位
    //            for (int i = 0; i < this->model.numj; i++) {
    //                for (int j = 0; j < this->model.mxjdof; j++) {
    //                    inrst >> tmpfl;
    //                    tmplstfl << tmpfl;
    //                }
    //                stress.ujnt << tmplstfl;
    //                tmplstfl.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            rstfile.seek(rstfile.pos() + 4 * this->model.numj * this->model.mxjdof); //fintj[numj][mxjdof]
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //react[numj][mxjdof]　反力
    //            for (int i = 0; i < this->model.numj; i++) {
    //                for (int j = 0; j < this->model.mxjdof; j++) {
    //                    inrst >> tmpfl;
    //                    tmplstfl << tmpfl;
    //                }
    //                stress.react << tmplstfl;
    //                tmplstfl.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //rstmm[nummd][nrst]　部材応力
    //            for (int i = 0; i < this->model.nummd; i++) {
    //                for (int j = 0; j < stress.nrst; j++) {
    //                    inrst >> tmpfl;
    //                    tmplstfl << tmpfl;
    //                }
    //                stress.rstmm << tmplstfl;
    //                tmplstfl.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            rstfile.seek(rstfile.pos() + 4 * this->model.nummd * this->model.nij *
    //                         stress.nphai); //phaiyz[nummd][nij][nphai]
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //ipspmm[nummd][nipspm] 降伏
    //            for (int i = 0; i < this->model.nummd; i++) {
    //                for (int j = 0; j < stress.nipspm; j++) {
    //                    inrst >> tmpi;
    //                    tmplst << tmpi;
    //                }
    //                stress.ipspmm << tmplst;
    //                tmplst.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8 * this->model.nummd * stress.nductm); //ductmm[nummd][nductm]
    //            rstfile.seek(rstfile.pos() + 4 * this->model.nummd * stress.nipspm); //kpspmm[nummd][nipspm]
    //            rstfile.seek(rstfile.pos() + 8); //foot head

    //            //--wall--
    //            //rstww[numwwd][nelbww][nrstww] 壁部材応力
    //            for (int i = 0; i < this->model.numwwd; i++) {
    //                for (int j = 0; j < stress.nelbww; j++) {
    //                    for (int k = 0; k < stress.nrstww; k++) {
    //                        inrst >> tmpfl;
    //                        tmplstfl << tmpfl;
    //                    }
    //                    tmplstlstfl << tmplstfl;
    //                    tmplstfl.clear();
    //                }
    //                stress.rstww << tmplstlstfl;
    //                tmplstlstfl.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //ipspww[numwwd][nipspw]　壁降伏
    //            for (int i = 0; i < this->model.numwwd; i++) {
    //                for (int j = 0; j < stress.nipspw; j++) {
    //                    inrst >> tmpi;
    //                    tmplst << tmpi;
    //                }
    //                stress.ipspww << tmplst;
    //                tmplst.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head

    //            //--slab--
    //            //fintws[numwsd][nadfws]
    //            for (int i = 0; i < this->model.numwsd; i++) {
    //                for (int j = 0; j < stress.nadfws; j++) {
    //                    inrst >> tmpfl;
    //                    tmplstfl << tmpfl;
    //                }
    //                stress.fintws << tmplstfl;
    //                tmplstfl.clear();
    //            }

    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //rstws[numwsd][ngaus][nrstws]
    //            for (int i = 0; i < this->model.numwsd; i++) {
    //                for (int j = 0; j < stress.ngaus; j++) {
    //                    for (int k = 0; k < stress.nrstws; k++) {
    //                        inrst >> tmpfl;
    //                        tmplstfl << tmpfl;
    //                    }
    //                    tmplstlstfl << tmplstfl;
    //                    tmplstfl.clear();
    //                }
    //                stress.rstws << tmplstlstfl;
    //                tmplstlstfl.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //--panel--
    //            //rstpp[numpd][npdim]
    //            for (int i = 0; i < this->model.numpd; i++) {
    //                for (int j = 0; j < stress.npdim; j++) {
    //                    inrst >> tmpfl;
    //                    tmplstfl << tmpfl;
    //                }
    //                stress.rstpp << tmplstfl;
    //                tmplstfl.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //ipsppp[numpd][npdim]
    //            for (int i = 0; i < this->model.numpd; i++) {
    //                for (int j = 0; j < stress.npdim; j++) {
    //                    inrst >> tmpi;
    //                    tmplst << tmpi;
    //                }
    //                stress.ipsppp << tmplst;
    //                tmplst.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head

    //            //--spring--
    //            //rstbb[numbd]
    //            for (int i = 0; i < this->model.numbd; i++) {
    //                inrst >> tmpfl;
    //                stress.rstbb << tmpfl;
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //ipspbb[numbd]
    //            for (int i = 0; i < this->model.numbd; i++) {
    //                inrst >> tmpi;
    //                stress.ipspbb << tmpi;
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //rstby[numbyd][nrstby]
    //            for (int i = 0; i < this->model.numbyd; i++) {
    //                for (int j = 0; j < stress.nrstby; j++) {
    //                    inrst >> tmpfl;
    //                    tmplstfl << tmpfl;
    //                }
    //                stress.rstby << tmplstfl;
    //                tmplstfl.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //ipspby[numbyd]
    //            for (int i = 0; i < this->model.numbyd; i++) {
    //                inrst >> tmpi;
    //                stress.ipspby << tmpi;
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //rstbs[numbsd][nrstbs]
    //            for (int i = 0; i < this->model.numbsd; i++) {
    //                for (int j = 0; j < stress.nrstbs; j++) {
    //                    inrst >> tmpfl;
    //                    tmplstfl << tmpfl;
    //                }
    //                stress.rstbs << tmplstfl;
    //                tmplstfl.clear();
    //            }
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //--load--

    //            //kcasol
    //            inrst >> stress.kcasol;
    //            rstfile.seek(rstfile.pos() + 8); //foot head
    //            //icase[kcasol]
    //            for (int i = 0; i < stress.kcasol; i++) {
    //                inrst >> tmpi;
    //                stress.icase << tmpi;
    //            }
    //            //ccase[kcasol]
    //            inrst.setFloatingPointPrecision(QDataStream::DoublePrecision);
    //            for (int i = 0; i < stress.kcasol; i++) {
    //                inrst >> tmpfl;
    //                stress.ccase << tmpfl;
    //            }
    //            inrst.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //            rstfile.seek(rstfile.pos() + 4); //foot

    //            //--static damper--
    //            //dsdpv[numsdp][ndsdpv]  if numsdp g.t. 0
    //            if (this->model.numsdp > 0) {
    //                rstfile.seek(rstfile.pos() + 4); //head
    //                for (int i = 0; i < this->model.numsdp; i++) {
    //                    for (int j = 0; j < stress.ndsdpv; j++) {
    //                        inrst >> tmpfl;
    //                        tmplstfl << tmpfl;
    //                    }
    //                    stress.dsdpv << tmplstfl;
    //                    tmplstfl.clear();
    //                }
    //                rstfile.seek(rstfile.pos() + 4); //foot
    //            }
    //            stresscase << stress;

    //            if (rstfile.seek(rstfile.pos() + 4) == false) {
    //                break;   //head file終端だったらbreak;
    //            }
    //            qint32 nextstep, nextend;
    //            inrst >> nextstep >> nextend; //qDebug()<<"next="<<nextstep<<nextend;
    //            if (nextstep <= stress.iastep || nextend != 0)break;
    //            //全ステップ数、スキップの値が不明なので次のデータ読みに行って不正な値でなければ次のステップを読んでいるが、バグを生じる可能性あり
    //            rstfile.seek(rstfile.pos() - 8); //nextstep,nextend分戻す
    //        }
    //        this->rst.insert(rstname, stresscase);
    //        //       qDebug()<<rstname<<this->rst[rstname].count();
    //        rstfile.close();
    //        qDebug() << "readOneRST done";
    //        this->successToReadSomeRstFile = true;
    //    }

    //    //読み込みここまで

    //    tmpdirpath.setNameFilters(QStringList() << "RST*");
    //    tmpdirpath.setFilter(QDir::Files | QDir::NoSymLinks);
    //    stresscase.clear();
    //    stresscase = tmpdirpath.entryList();
    //    if (stresscase.isEmpty())return -1;
    //    return 0;

    //}
    // int handlePstn::readBLD3D_LOAD()
    //{
    //    successToReadLoadFile = false;
    //    load.clear();
    //    //filepath
    //    QFile fload(filepath.absolutePath() + "/" + tmpdirname + "/BLD3D_LOAD");
    //    QString command = "\""+apppath+"/7za.exe\" x -y \"-o" + tmpdirpath.absolutePath() + "/\" " +"\"-ir!BLD3D_LOAD\" \""+filename+"\"";
    //    if (toDecompressPstn) {
    //        QProcess *decompload = new QProcess;
    //        decompload->execute(command);
    //        qDebug() << u8"LOADの解凍";
    //    }
    //    bool successFileOpen = fload.open(QIODevice::ReadOnly);
    //    if (successFileOpen == false)return -1;
    //    QDataStream inload(&fload);
    //    inload.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian
    //    fload.seek(92);//???
    //    inload >>
    //           load.numc >> load.lxlemm >> load.numcl >> load.numcld >> load.numse >>
    //           load.numsed >> load.mxwifl >> load.numwi >> load.numwid;
    //    //値を取り出す際の仮の変数・コンテナ
    //    qint32 tmpi;
    //    float tmpfl;
    //    QVector<qint32> tmplst;
    //    QVector<QVector<qint32> > tmplstlst;
    //    QVector<float> tmplstfl;
    //    QVector<QVector<float> > tmplstlstfl;
    //    QVector<QVector<QVector<float> > > tmplstlstlstfl;
    //    char ch60[61];
    //    ch60[60] = '\0';
    //    char ch6[7];
    //    ch6[6] = '\0';

    //    fload.seek(fload.pos() + 8); //foot head
    //    //indle[numc]
    //    for (int i = 0; i < load.numc; i++) {
    //        inload >> tmpi;
    //        load.indle << tmpi;
    //    }
    //    //namel[numc]*6
    //    for (int i = 0; i < load.numc; i++) {
    //        inload.readRawData(ch6, 6);
    //        load.namel << QString(ch6);
    ////        qDebug()<<QString(ch6);
    //        memset(ch6, 0, 6);
    //    }
    //    //titc[numc]*60
    //    for (int i = 0; i < load.numc; i++) {
    //        inload.readRawData(ch60, 60);
    //        load.titc << QString(ch60);
    ////        qDebug()<<QString(ch60);
    //        memset(ch60, 0, 60);
    //    }
    //    //indse[numc]
    //    for (int i = 0; i < load.numc; i++) {
    //        inload >> tmpi;
    //        load.indse << tmpi;
    //    }
    //    //indwi[numc]
    //    for (int i = 0; i < load.numc; i++) {
    //        inload >> tmpi;
    //        load.indwi << tmpi;
    //    }
    //    fload.seek(fload.pos() + 8); //foot head
    //    //dirse[numc]
    //    for (int i = 0; i < load.numc; i++) {
    //        inload >> tmpfl;
    //        load.dirse << tmpfl;
    //    }
    //    fload.seek(fload.pos() + 8); //foot head
    //    //frcfl[numc][numfg][mxnfl][nfrcfl]
    //    for (int i = 0; i < load.numc; i++) {
    //        for (int j = 0; j < model.numfg; j++) {
    //            for (int k = 0; k < model.mxnfl; k++) {
    //                for (int l = 0; l < load.nfrcfl; l++) {
    //                    inload >> tmpfl;
    //                    tmplstfl << tmpfl;
    //                }
    //                tmplstlstfl << tmplstfl;
    //                tmplstfl.clear();
    //            }
    //            tmplstlstlstfl << tmplstlstfl;
    //            tmplstlstfl.clear();
    //        }
    //        load.frcfl << tmplstlstlstfl;
    //        tmplstlstlstfl.clear();
    //    }
    //    fload.seek(fload.pos() + 8); //foot head
    //    //ibc[numc][msize]
    //    for (int i = 0; i < load.numc; i++) {
    //        for (int j = 0; j < model.msize; j++) {
    //            inload >> tmpi;
    //            tmplst << tmpi;
    //        }
    //        load.ibc << tmplst;
    //        tmplst.clear();
    //    }
    //    fload.seek(fload.pos() + 8); //foot head
    //    //fincj[numc][numj][mxjdof]
    //    for (int i = 0; i < load.numc; i++) {
    //        for (int j = 0; j < model.numj; j++) {
    //            for (int k = 0; k < model.mxjdof; k++) {
    //                inload >> tmpfl;
    //                tmplstfl << tmpfl;
    //            }
    //            tmplstlstfl << tmplstfl;
    //            tmplstfl.clear();
    //        }
    //        load.fincj << tmplstlstfl;
    //        tmplstlstfl.clear();
    //    }
    //    fload.seek(fload.pos() + 8); //foot head
    //    //nlemm[numcld]
    //    for (int i = 0; i < load.numcld; i++) {
    //        inload >> tmpi;
    //        load.nlemm << tmpi;
    //    }
    //    //ilemm[numcld][mxlemm][nilemm]
    //    for (int i = 0; i < load.numcld; i++) {
    //        for (int j = 0; j < load.mxlemm; j++) {
    //            for (int k = 0; k < load.nilemm; k++) {
    //                inload >> tmpi;
    //                tmplst << tmpi;
    //            }
    //            tmplstlst << tmplst;
    //            tmplst.clear();
    //        }
    //        load.ilemm << tmplstlst;
    //        tmplstlst.clear();
    //    }
    //    fload.seek(fload.pos() + 8); //foot head
    //    //cmqmm[numcld][nummd][ncmqmm]
    //    for (int i = 0; i < load.numcld; i++) {
    //        qDebug() << "numcld=" << load.numcld;
    //        for (int j = 0; j < model.nummd; j++) {
    //            for (int k = 0; k < load.ncmqmm; k++) {
    //                inload >> tmpfl;
    //                tmplstfl << tmpfl;
    //            }
    //            tmplstlstfl << tmplstfl;
    //            tmplstfl.clear();
    //        }
    //        load.cmqmm << tmplstlstfl;
    //        tmplstlstfl.clear();
    //    }

    //    fload.seek(fload.pos() + 8); //foot head
    //    fload.close();
    //    successToReadLoadFile = true;

    //    return 0;

    //}

    ////model違いを全部読み込むか？MODEL*でフィルタかけてハッシュで？
    // int handlePstn::readBLD3D_PREA(){
    //     successToReadPreaFile = false;
    //     prea.clear();
    //     modelcase.clear();
    //     if (!toDecompressPstn) {
    //         tmpdirpath.setNameFilters(QStringList() << "MODEL*");
    //         tmpdirpath.setFilter(QDir::Files | QDir::NoSymLinks);
    //         modelcase = tmpdirpath.entryList();
    //         if (modelcase.isEmpty())return -1;
    //     }

    //    //filepath
    //    else {
    ////        QString getlistcommand = "\"" + apppath + "/7za.exe\" l -y  -ir!RST* -xr!*.pun -xr!*QD " + filename;
    //        QString getlistcommand = "\""+apppath+"/7za.exe\" l -y  -ir!MODEL* -xr!*.pun -xr!*QD "+"\""+filename+"\"";
    //        QProcess getlist;
    //        getlist.start(getlistcommand);
    //        getlist.waitForFinished();
    //        QString output(getlist.readAllStandardOutput());
    //        QStringList outputlist = output.split("\n");
    //        for (int i = 0; i < outputlist.count(); i++) {
    //            QString line = outputlist.at(i);
    //            if (line.left(2) != "20")continue;
    //            if (line.left(2) == "20")modelcase << line.remove(0,53).trimmed(); //20XXから始まる行、54文字目から呼んで末尾はtrim。2100年問題
    //        }
    //        modelcase.pop_back();
    //        if (modelcase.isEmpty())return -1;
    //    }

    //    for (int mdcase = 0; mdcase < modelcase.count(); mdcase++) {
    //        QString modelname = modelcase.at(mdcase);
    //        QString command = "\""+this->apppath+"/7za.exe\" x -y \"-o" + this->tmpdirpath.absolutePath() + "/\" " +"\"-ir!"+modelname+"\" \""+this->filename+"\"";
    //        if (this->toDecompressPstn) {
    //            QProcess decomprst;
    //            decomprst.start(command);
    //            decomprst.waitForFinished(300000);
    //        }
    //        QFile fprea(this->filepath.absolutePath() + "/" + this->tmpdirname + "/" + modelname);
    //        bool successFileOpen = fprea.open(QIODevice::ReadOnly);
    //        if (successFileOpen == false)return -1;
    //        DataBLD3D_PREA prea;

    //    QDataStream inprea(&fprea);
    //    inprea.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian

    //    qDebug()<<"preread";
    //    fprea.seek(92);//???
    //    inprea >>
    //            prea.numwp >> prea.numwpd;
    //    fprea.seek(fprea.pos()+24);//nmtyp[6]
    //    inprea >> prea.numez >> prea.numezd >>
    //            prea.numms >> prea.nummsd >> prea.numbr >> prea.numbrd >> prea.numcb >>
    //            prea.lmsmax >> prea.lmrmax >> prea.lmsmxw >> prea.mxzs >> prea.numzs >>
    //            prea.numzsd >> prea.numis >> prea.numqs >> prea.numaxc >> prea.numaxcd >>
    //            prea.numhg >> prea.numhgd >> prea.lhlmax >> prea.lhsmax >> prea.lhrmax >>
    //            prea.numhs >> prea.numhsd;
    //    if(prea.numwpd==0)prea.numwpd=1;
    //    if(prea.numezd==0)prea.numezd=1;
    //    if(prea.nummsd==0)prea.nummsd=1;
    //    if(prea.numbrd==0)prea.numbrd=1;
    //    if(prea.numzsd==0)prea.numzsd=1;
    //    if(prea.numaxcd==0)prea.numaxcd=1;
    //    if(prea.numhgd==0)prea.numhgd=1;
    //    if(prea.numhsd==0)prea.numhsd=1;
    //    //値を取り出す際の仮の変数・コンテナ
    //    qint32 tmpi;
    //    float tmpfl;
    //    QVector<qint32> tmplst;
    //    QVector<QVector<qint32> > tmplstlst;
    //    QVector<float> tmplstfl;
    //    QVector<QVector<float> > tmplstlstfl;
    //    QVector<QVector<QVector<float> > > tmplstlstlstfl;
    //    char ch60[61];
    //    ch60[60]='\0';
    //    char ch6[7];
    //    ch6[6]='\0';

    //    fprea.seek(fprea.pos()+8);//foot head
    //    //mtyp[nummd][lmtyp] int
    //    for(int i=0;i<model.nummd;i++){
    //        for(int j=0;j<prea.lmtyp;j++){inprea >> tmpi; tmplst << tmpi;}
    //        prea.mtyp << tmplst;
    //        tmplst.clear();
    //    }
    ////    qDebug()<<"mtyp"<<prea.mtyp[0][0];
    //    fprea.seek(fprea.pos() + 8); //foot head

    //    //debmm[nummd][ndeb] float
    //    for(int i=0;i<model.nummd;i++){
    //        for(int j=0;j<prea.ndeb;j++){inprea >> tmpfl; tmplstfl << tmpfl;}
    //        prea.debmm << tmplstfl;
    //        tmplstfl.clear();
    //    }
    ////    qDebug()<<"debmm"<<prea.debmm[0][0];
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //zstmm[nzsiz][numezd]
    //    fprea.seek(fprea.pos() + 4 * prea.nzsiz*prea.numezd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //nspms[nij][nummsd]
    //    fprea.seek(fprea.pos() + 4 * model.nij*prea.nummsd);
    //    //jmscon[nimsc][lmsmax][2][nummsd]
    //    fprea.seek(fprea.pos() + 4 * prea.nimsc*prea.lmsmax*2*prea.nummsd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //dmscon[nrmsc][lmsmax][2][nummsd]
    //    fprea.seek(fprea.pos() + 4 * prea.nrmsc*prea.lmsmax*2*prea.nummsd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //nspmr[nummsd]
    //    fprea.seek(fprea.pos() + 4 * prea.nummsd);
    //    //jmrcon[nimrc][lmrmax][nummsd]
    //    fprea.seek(fprea.pos() + 4 * prea.nimrc*prea.lmrmax*prea.nummsd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //dmrcon[nrmrc][lmrmax][nummsd]
    //    fprea.seek(fprea.pos() + 4 * prea.nrmrc*prea.lmrmax*prea.nummsd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //jbrcon[nibrc][numbrd]
    //    fprea.seek(fprea.pos() + 4 * prea.nibrc*prea.numbrd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //dbrcon[nrbrc][numbrd]
    //    fprea.seek(fprea.pos() + 4 * prea.nrbrc*prea.numbrd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //mtypww[lmtypw][numwwd]
    //    fprea.seek(fprea.pos() + 4 * prea.lmtypw*model.numwwd);
    //    //indww[nrect][numwwd]
    //    fprea.seek(fprea.pos() + 4 * prea.nrect*model.numwwd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //debww[ndebww][numwwd]
    //    for (int i = 0; i < model.numwwd; i++) {
    //        for (int j = 0; j < prea.ndebww; j++) {
    //            inprea >> tmpfl;
    //            tmplstfl << tmpfl;
    //            //qDebug()<<tmpfl;
    //        }
    //        prea.debww << tmplstfl;
    //        tmplstfl.clear();
    //    }//ここまでOK

    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //nspmsw[nij][numwpd]
    //    fprea.seek(fprea.pos() + 4 * model.nij*prea.numwpd);
    //    //jmscow[nimscw][lmsmxw][ij][numwpd]
    //    fprea.seek(fprea.pos() + 4 * prea.nimscw*prea.lmsmxw*model.nij*prea.numwpd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //dmscow[nrmscw][lmsmxw][ij][numwpd]
    ////    qDebug()<<prea.nrmscw<<prea.lmsmxw<<prea.numwpd;
    //    for(int k=0;k<prea.numwpd;k++){
    //        for(int m=0;m<2;m++){
    //            for (int i = 0; i < prea.lmsmxw; i++) {
    //                for (int j = 0; j < prea.nrmscw; j++) {
    //                    inprea >> tmpfl;
    //                    tmplstfl << tmpfl;
    ////                    qDebug()<<tmpfl;
    //                }
    ////                prea.debww << tmplstfl;
    //                tmplstfl.clear();
    //            }
    //        }
    //    }

    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //jmrcow
    //    fprea.seek(fprea.pos() + 4 * prea.nimrcw*prea.numwpd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //dmrcow
    //    fprea.seek(fprea.pos() + 4 * prea.nrmrcw*prea.numwpd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //indws
    //    for (int i = 0; i < model.numwsd; i++) {
    //        for (int j = 0; j < prea.nrect; j++) {
    //            inprea >> tmpi;
    //            tmplst<< tmpi;
    ////            qDebug()<<tmpi;
    //        }
    //        tmplst.clear();
    //    }

    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //debws
    //    fprea.seek(fprea.pos() + 4 * prea.ndebws*model.numwsd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //dws1
    //    fprea.seek(fprea.pos() + 4 * prea.ndws1*model.numwsd);
    //    fprea.seek(fprea.pos() + 8); //foot head
    //    //jindpp
    //    fprea.seek(fprea.pos() + 4 * model.numpd);
    //    //jppcon
    //    for (int i = 0; i < model.numpd; i++) {
    //        for (int j = 0; j < prea.npdim; j++) {
    //            inprea >> tmpi;
    //            tmplst<< tmpi;
    ////            qDebug()<<tmpi;
    //        }
    //        tmplst.clear();
    //    }//だめ

    //     // 通りグループ
    //     QList<FRAMEGROUPVALUE> frgrouplist;
    //     QList<FRAMEPOINTSVALUE> frlist;
    //     QHash<int, qreal> minimumcoordinate;
    //     for (int gridgroup = 0; gridgroup < this->model.numgg; gridgroup++)
    //     {
    //         FRAMEGROUPVALUE fgvalue;
    //         fgvalue.GID = QUuid::createUuid();
    //         frameGroupNoToUuid[gridgroup] = fgvalue.GID;
    //         fgvalue.groupName = QString::number(gridgroup);
    //         if (this->model.igg[gridgroup] == 0)
    //             fgvalue.frameType = FrameType::FRAMETYPE::STRAIGHT;
    //         else if (this->model.igg[gridgroup] == 1)
    //             fgvalue.frameType = FrameType::FRAMETYPE::CIRCLE;
    //         else if (this->model.igg[gridgroup] == 2)
    //             fgvalue.frameType = FrameType::FRAMETYPE::RADIUS;
    //         else
    //             fgvalue.frameType = FrameType::FRAMETYPE::NOFRAMETYPE;
    //         fgvalue.count = this->model.numgr[gridgroup];
    //         fgvalue.basePoint = QPointF(this->model.dgg[gridgroup][0], this->model.dgg[gridgroup][1]);
    //         fgvalue.startAngle = this->model.dgg[gridgroup][2] * 180. / M_PI;
    //         fgvalue.endAngle = this->model.dgg[gridgroup][3] * 180. / M_PI;
    //         fgvalue.division = 36;
    //         fgvalue.startFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().first().UUID;
    //         fgvalue.endFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().last().UUID;
    //         frgrouplist << fgvalue;
    //         // 通り
    //         for (int gridnumber = 0; gridnumber < this->model.numgr[gridgroup]; gridnumber++)
    //         {
    //             qreal coordinate = 0.0;
    //             for (int gn = 0; gn < gridnumber; gn++)
    //             {
    //                 coordinate = coordinate + this->model.dgr[gridgroup][gn];
    //             }
    //             if (!minimumcoordinate.contains(gridgroup))
    //                 minimumcoordinate[gridgroup] = coordinate;
    //             else if (minimumcoordinate[gridgroup] > coordinate)
    //                 minimumcoordinate[gridgroup] = coordinate;
    //         }
    //         for (int gridnumber = 0; gridnumber < this->model.numgr[gridgroup]; gridnumber++)
    //         {
    //             qreal coordinate = 0.0;
    //             for (int gn = 0; gn < gridnumber; gn++)
    //             {
    //                 coordinate = coordinate + this->model.dgr[gridgroup][gn];
    //             }
    //             qreal nextSpan = this->model.dgr[gridgroup][gridnumber];
    //             //            if(nextSpan==0.0)coordinate+=0.000001;
    //             if (nextSpan == 0.0)
    //                 coordinate -= 0.000001;

    //             QList<QPointF> plist;
    //             qreal span = 0.0;
    //             QPointF npoint;
    //             STRAIGHTFRAMEVALUE st_value;
    //             if (fgvalue.frameType == FrameType::FRAMETYPE::STRAIGHT)
    //             {
    //                 QPointF po = fgvalue.basePoint;
    //                 qreal theta = fgvalue.startAngle / 180.0 * M_PI;
    //                 span = coordinate - minimumcoordinate[gridgroup];
    //                 qreal p_len =
    //                     fgvalue.plusLength; // 通りのプラスマイナスをどこまで伸ばすかは節点の情報がないと決まらない
    //                 qreal m_len = fgvalue.minusLength;
    //                 QPointF p1, p2, p3, p4;
    //                 QPointF spoint;
    //                 qreal m_span = nextSpan;
    //                 qreal s_angle;

    //                 if (fgvalue.startAngle < 45.0)
    //                 {
    //                     p1 = QPointF(-m_len * qCos(theta) - span * qSin(theta),
    //                                  -m_len * qSin(theta) + span * qCos(theta)) +
    //                          po;
    //                     p2 = QPointF(p_len * qCos(theta) - span * qSin(theta),
    //                                  p_len * qSin(theta) + span * qCos(theta)) +
    //                          po;
    //                     npoint = CalcVector2D::extendPoint(p1, p2, 1.0);
    //                     s_angle = fgvalue.startAngle + 90.;
    //                     if (gridnumber == fgvalue.count - 1)
    //                     {
    //                         p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
    //                         p4 = QPointF(p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                         spoint = CalcVector2D::extendPoint(p3, p4, 1.5);
    //                     }
    //                     else
    //                     {
    //                         spoint = CalcVector2D::extendPoint(p1, p2, 0.5);
    //                     }
    //                 }
    //                 else
    //                 {
    //                     p1 = QPointF(-m_len * qCos(theta) + span * qSin(theta),
    //                                  -m_len * qSin(theta) - span * qCos(theta)) +
    //                          po;
    //                     p2 = QPointF(p_len * qCos(theta) + span * qSin(theta),
    //                                  p_len * qSin(theta) - span * qCos(theta)) +
    //                          po;
    //                     npoint = CalcVector2D::extendPoint(p2, p1, 1.0);
    //                     s_angle = fgvalue.startAngle - 90.;
    //                     if (gridnumber == fgvalue.count - 1)
    //                     {
    //                         p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
    //                         p4 = QPointF(p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                         spoint = CalcVector2D::extendPoint(p4, p3, 1.5);
    //                     }
    //                     else
    //                     {
    //                         spoint = CalcVector2D::extendPoint(p2, p1, 0.5);
    //                     }
    //                 }
    //                 plist.append(p1);
    //                 plist.append(p2);

    //                 if (gridnumber == fgvalue.count - 1)
    //                     m_span = span;
    //                 st_value = STRAIGHTFRAMEVALUE(m_span, s_angle, spoint,
    //                                               (gridnumber == 0), (gridnumber == fgvalue.count - 1));
    //             }
    //             else if (fgvalue.frameType == FrameType::FRAMETYPE::CIRCLE)
    //             {

    //                 QPointF po = fgvalue.basePoint;
    //                 qreal s_angle = fgvalue.startAngle;
    //                 qreal e_angle = fgvalue.endAngle;
    //                 span = coordinate - minimumcoordinate[gridgroup];
    //                 int div = fgvalue.division;

    //                 if ((s_angle < e_angle) && (e_angle - s_angle < 360.0) && div > 0)
    //                 {
    //                     qreal s_theta = s_angle / 180.0 * M_PI;
    //                     qreal e_theta = e_angle / 180.0 * M_PI;
    //                     qreal d_theta = (e_theta - s_theta) / qreal(div);
    //                     for (int j = 0; j < div + 1; j++)
    //                     {
    //                         qreal j_theta = s_theta + d_theta * qreal(j);
    //                         QPointF p1 = QPointF(po.x() + span * qCos(j_theta), po.y() + span * qSin(j_theta));
    //                         plist.append(p1);
    //                     }
    //                 }

    //                 if (plist.count() > 1)
    //                     npoint = CalcVector2D::extendPoint(plist.at(1), plist.at(0), 3.0);
    //             }
    //             else if (fgvalue.frameType == FrameType::FRAMETYPE::RADIUS)
    //             {

    //                 QPointF po = fgvalue.basePoint;
    //                 qreal angle = fgvalue.startAngle + (coordinate - minimumcoordinate[gridgroup]) * 180.0 / M_PI;
    //                 qreal theta = angle / 180.0 * M_PI;
    //                 qreal p_len = fgvalue.plusLength;

    //                 plist.append(po);
    //                 QPointF p1 = QPointF(p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                 plist.append(p1);

    //                 npoint = CalcVector2D::extendPoint(po, p1, 3.0);
    //             }
    //             else
    //             {
    //                 QPointF po = fgvalue.basePoint;
    //                 QPointF p1 = QPointF(1.0, 0) + po;
    //                 plist.append(po);
    //                 plist.append(p1);
    //                 npoint = CalcVector2D::extendPoint(po, p1, 3.0);
    //             }
    //             //            else if(fgvalue.frameType==DIAMETER){
    //             //            }else if(fgvalue.frameType==VARIABLE){
    //             //            }else if(fgvalue.frameType==CLCIRCLE){
    //             //            }else if(fgvalue.frameType==CLVARIABLE){
    //             //            }else{

    //             //            }//定義してない通りタイプが来た時、pointlistが空になる
    //             QString name = this->model.namegr[gridgroup][gridnumber].trimmed(); // GRは複数の通りに所属
    //             QString uid = QUuid::createUuid().toString();
    //             //            QList<int> grid_frame;grid_frame<<gridgroup;grid_frame<<gridnumber;
    //             frameNoToUuid[gridgroup * 10000 + gridnumber] = uid;
    //             == == == =
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //debpp
    //             //    fprea.seek(fprea.pos() + 4 * prea.ndebpp*prea.npdim*model.numpd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //dppcon
    //             //    fprea.seek(fprea.pos() + 4 * prea.nrppc*prea.npdim*model.numpd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //jbbcon
    //             //    fprea.seek(fprea.pos() + 4 * model.numbd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //dbbcon
    //             //    fprea.seek(fprea.pos() + 4 * prea.nrbbc*model.numbd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //nspby
    //             //    for (int j = 0; j < model.numbyd; j++) {
    //             //        inprea >> tmpi;
    //             ////        qDebug()<<tmpi;
    //             //    }
    //             //    //jbycon
    //             //    fprea.seek(fprea.pos() + 4 * model.lbymax*model.numbyd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //dbycon
    //             //    fprea.seek(fprea.pos() + 4 * prea.nrbyc*model.lbymax*model.numbyd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //rstmmi
    //             //    for (int i = 0; i < model.nummd; i++) {
    //             //        for (int j = 0; j < prea.nrstmm; j++) {
    //             //            inprea >> tmpfl;
    //             //            tmplstfl << tmpfl;
    //             //           // qDebug()<<tmpfl;
    //             //        }
    //             //        prea.rstmmi << tmplstfl;
    //             //        tmplstfl.clear();
    //             //    }

    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //cmqmmi
    //             //    fprea.seek(fprea.pos() + 4 * prea.ncmqmm*model.nummd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //dnms
    //             //    fprea.seek(fprea.pos() + 4 * prea.msdd*model.nij*model.nummd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //rstwwi
    //             //    fprea.seek(fprea.pos() + 4 * prea.nrstww*prea.neww*model.numwwd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //cmqwwi
    //             //    fprea.seek(fprea.pos() + 4 * prea.neww*model.numwwd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //imm2
    //             //    for (int i = 0; i < model.nummd; i++) {
    //             //        for (int j = 0; j < prea.nimm2; j++) {
    //             //            inprea >> tmpi;
    //             //            tmplst << tmpi;
    //             ////            qDebug()<<tmpi;
    //             //        }
    //             //        prea.imm2 << tmplst;
    //             //        tmplst.clear();
    //             //    }
    //             //    //imm3
    //             //    fprea.seek(fprea.pos() + 4 * prea.nimm3*2*model.nummd);
    //             //    fprea.seek(fprea.pos() + 8); //foot head
    //             //    //dmm2
    //             //    for (int i = 0; i < model.nummd; i++) {
    //             //        for (int j = 0; j < prea.ndmm2; j++) {
    //             //            inprea >> tmpfl;
    //             //            tmplstfl << tmpfl;
    //             //         }
    //             //        prea.dmm2 << tmplstfl;
    //             //        tmplstfl.clear();
    //             //    }

    //             //    //dmm3
    //             //    //dmm5
    // >>>>>>> OutputDiagrams:post3DLib/handle_pstn/handle_pstn.cpp

    //    handlePstn::prea.insert(modelname,prea);
    //    }
    //    successToReadPreaFile = true;
    //            return 0;
    //}

    // int handlePstn::readBLD3D_CHCK()
    //{
    //     successToReadChckFile = false;
    //     chck.clear();
    //     //filepath
    //     QFile fchck(filepath.absolutePath() + "/" + tmpdirname + "/BLD3D_CHCK");
    ////    QString command = "\"" + apppath + "/7za.exe\" x -y -o" + tmpdirpath.absolutePath() +
    ////                      "/ -ir!BLD3D_CHCK " + filename;
    //    QString command = "\""+apppath+"/7za.exe\" x -y \"-o" + tmpdirpath.absolutePath() + "/\" " +"\"-ir!BLD3D_CHCK\" \""+filename+"\"";
    //    if (toDecompressPstn) {
    //        QProcess *decompchck = new QProcess;
    //        decompchck->execute(command);
    //        qDebug() << u8"CHCKの解凍";
    //    }
    //    bool successFileOpen = fchck.open(QIODevice::ReadOnly);
    //    if (successFileOpen == false)return -1;
    //    QDataStream inchck(&fchck);
    //    inchck.setFloatingPointPrecision(QDataStream::SinglePrecision);//実数を単精度で読み込み
    //    //incmdl.setByteOrder(QDataStream::LittleEndian);//バイトオーダ変更 default:BinEndian
    //    fchck.seek(92);//???
    //    //値を取り出す際の仮の変数・コンテナ
    //    float tmpfl1, tmpfl2, tmpfl3, tmpfl4;
    //    QVector<float> tmplstfl1, tmplstfl2, tmplstfl3, tmplstfl4;
    //    QVector<QVector<float> > tmplstlstfl1, tmplstlstfl2, tmplstlstfl3, tmplstlstfl4;
    //    char ch10[11];
    //    ch10[10] = '\0';
    //    //定数
    //    qint32 nrstmc = 18;
    //    qint32 nrstma = 33;
    //    qint32 nrstwc = 2;
    //    qint32 nrstwa = 17;

    //    inchck >> chck.numch;
    //    fchck.seek(fchck.pos() + 8); //foot head
    //    //namech[numch]*10
    //    for (int i = 0; i < chck.numch; i++) {
    //        inchck.readRawData(ch10, 10);
    //        chck.namech << QString(ch10);
    //        qDebug() << QString(ch10);
    //        memset(ch10, 0, 10);
    //    }
    //    fchck.seek(fchck.pos() + 8); //foot head
    //    //rstmmc[numch][nummd][nicj][nrstmc]
    //    fchck.seek(fchck.pos() + 4 * chck.numch * model.nummd * model.nicj * nrstmc);
    //    fchck.seek(fchck.pos() + 8); //foot head

    //    //rstmma[numch][nummd][nicj][nrstma]
    //    for (int i = 0; i < chck.numch; i++) {
    //        for (int j = 0; j < model.nummd; j++) {
    //            for (int k = 0; k < model.nicj; k++) {
    //                for (int l = 0; l < nrstma; l++) {
    //                    if (l == 10) {
    //                        inchck >> tmpfl1;
    //                    } else if (l == 11) {
    //                        inchck >> tmpfl2;
    //                    } else if (l == 21) {
    //                        inchck >> tmpfl3;
    //                    } else if (l == 22) {
    //                        inchck >> tmpfl4;
    //                    } else {
    //                        fchck.seek(fchck.pos() + 4);
    //                    }
    //                }//qDebug()<<i<<j<<k<<l<<tmpfl;}
    //                tmplstfl1 << tmpfl1;
    //                tmplstfl2 << tmpfl2;
    //                tmplstfl3 << tmpfl3;
    //                tmplstfl4 << tmpfl4;
    //            }
    //            tmplstlstfl1 << tmplstfl1;
    //            tmplstlstfl2 << tmplstfl2;
    //            tmplstlstfl3 << tmplstfl3;
    //            tmplstlstfl4 << tmplstfl4;
    //            tmplstfl1.clear();
    //            tmplstfl2.clear();
    //            tmplstfl3.clear();
    //            tmplstfl4.clear();
    //        }
    //        if (model.numm != 0) {
    //            chck.line_verificationRatioBending << tmplstlstfl1;//検定比
    //            chck.line_allowanceRateBending << tmplstlstfl2;//曲げ余裕率
    //            chck.line_verificationRatioShear << tmplstlstfl3;//せん断余裕率
    //            chck.line_verificationRatioShear2 << tmplstlstfl4;//せん断余裕率SofSRC,付着ofRC
    //        }
    //        tmplstlstfl1.clear();
    //        tmplstlstfl2.clear();
    //        tmplstlstfl3.clear();
    //        tmplstlstfl4.clear();
    //    }

    ////壁のせん断余裕率Qd/Qa
    //    fchck.seek(fchck.pos() + 8); //foot head
    //    //rstwwc[numch][numwwd][nrstwc]
    //    fchck.seek(fchck.pos() + 4 * chck.numch * model.numwwd * nrstwc);
    //    fchck.seek(fchck.pos() + 8); //foot head

    //    //rstwwa[numch][numwwd][nrstwa]
    //    for (int i = 0; i < chck.numch; i++) {
    //        for (int j = 0; j < model.numwwd; j++) {
    //            for (int k = 0; k < nrstwa; k++) {
    //                if (k == 14) {
    //                    inchck >> tmpfl1;
    //                } else {
    //                    fchck.seek(fchck.pos() + 4);
    //                }
    //         qDebug() << "nfg=" << nfg;
    //         qDebug() << "frgrouplist.count=" << frgrouplist.count();
    //         qDebug() << "frlist.count=" << frlist.count();
    //         FRAMEGROUPVALUE fgv = frgrouplist[nfg];
    // //                qDebug()<<"get fgv";
    //         qreal minus = 0.0;
    //         qreal plus = 0.0;
    //         foreach (FRAMEPOINTSVALUE frpv, frlist) {
    //             qDebug()<<frpv.frameName;
    //             if (fgv.frameType == FrameType::FRAMETYPE::STRAIGHT && !frpv.straight.isFirst)continue;
    //             if (frpv.GID == fgv.GID) {
    //                 QList<XYZ> globalxyzs;
    //                 QList<JointData*> joints=UnifiedFixedData::getInstance()->getAnalysisData()->jointListOfFrame(frpv.UUID);
    //                 for (int i = 0; i < joints.count(); i++) {
    // //                            qDebug()<<"get jdlist"<<i;
    //                     globalxyzs << joints.at(i)->xyz();
    //                     qDebug()<<i;
    //                 }
    // //                        qDebug()<<"globalxyzcount="<<globalxyzs.count();
    // //                        qDebug()<<frpv.UUID;
    //                 QList<XYZ> framexyzs = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameXYZ(frpv.UUID, globalxyzs);
    // //                        qDebug()<<"framexyzcount="<<framexyzs.count();
    //                 qDebug()<<"aaa";
    //                 foreach (XYZ framexyz, framexyzs) {
    //                     if (framexyz.x < minus)minus = framexyz.x;
    //                     if (framexyz.x > plus)plus = framexyz.x;
    //                 }
    //             }
    //         }
    //         minus = qMax(1.0, qAbs(minus - 1.0));
    // //                qDebug()<<"frgrouplist call"<<nfg;
    //         frgrouplist[nfg].minusLength = minus;
    //         frgrouplist[nfg].plusLength = plus - minus;
    // //                qDebug()<<"plus,minus="<<plus<<minus;
    //     }
    //             qDebug()<<"end of loop1";

    //     QList<FRAMEPOINTSVALUE> fixedfrlist;
    //     qreal span = 0.0;
    //     for (int frnum = 0; frnum < frlist.count(); frnum++) {
    //         FRAMEPOINTSVALUE fr = frlist.at(frnum);
    //         FRAMEGROUPVALUE fgitem;
    //         FrameType::FRAMETYPE itype = fr.frameType;
    //         for (int i = 0; i < frgrouplist.count(); i++) {
    //             if (frgrouplist.at(i).GID == fr.GID) {
    //                 fgitem = frgrouplist.at(i);
    //                 break;
    //             }
    //         }
    //         QPointF npoint;
    //         QList<QPointF> plist;
    //         STRAIGHTFRAMEVALUE st_value;
    //         if (itype == FrameType::FRAMETYPE::STRAIGHT) {
    //             if (fr.straight.isFirst)span = 0.0;
    //             qreal m_len = fgitem.minusLength;
    //             qreal p_len = fgitem.plusLength;
    //             qreal theta = fgitem.startAngle / 180.0 * M_PI;
    //             QPointF po = fgitem.basePoint;
    //             QPointF spoint;
    //             QPointF p1, p2, p3, p4;
    //             if ( fgitem.startAngle < 45.0 ) {
    //                 p1 = QPointF(-m_len * qCos(theta) - span * qSin(theta),
    //                              -m_len * qSin(theta) + span * qCos(theta)) + po;
    //                 p2 = QPointF( p_len * qCos(theta) - span * qSin(theta),
    //                               p_len * qSin(theta) + span * qCos(theta)) + po;
    //                 npoint = CalcVector2D::extendPoint(p1, p2, 5.0);
    //                 if (fr.straight.isLast) {
    //                     p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
    //                     p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                     spoint = CalcVector2D::extendPoint(p3, p4, 1.5);
    //                 } else {
    //                     spoint = CalcVector2D::extendPoint(p1, p2, 0.5);
    //                 }
    //             } else {
    //                 p1 = QPointF(-m_len * qCos(theta) + span * qSin(theta),
    //                              -m_len * qSin(theta) - span * qCos(theta)) + po;
    //                 p2 = QPointF( p_len * qCos(theta) + span * qSin(theta),
    //                               p_len * qSin(theta) - span * qCos(theta)) + po;
    //                 npoint = CalcVector2D::extendPoint(p2, p1, 5.0);
    //                 if (fr.straight.isLast) {
    //                     p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
    //                     p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                     spoint = CalcVector2D::extendPoint(p4, p3, 1.5);
    //                 } else {
    //                     spoint = CalcVector2D::extendPoint(p2, p1, 0.5);
    //                 }
    //             }
    //             plist.append(p1);
    //             plist.append(p2);
    //             st_value = STRAIGHTFRAMEVALUE(fr.straight.span, fr.straight.angle, spoint,
    //                                           fr.straight.isFirst, fr.straight.isLast);
    //             span += fr.straight.span;

    //         } else if (itype == FrameType::FRAMETYPE::CIRCLE) {

    //             plist = fr.pointList;
    //             npoint = fr.namePoint;

    //         } else if (itype == FrameType::FRAMETYPE::RADIUS) {

    //             QPointF po = fgitem.basePoint;
    //             qreal theta = atan2(fr.pointList.last().y() - fr.pointList.first().y(),
    //                                 fr.pointList.last().x() - fr.pointList.first().x());
    //             qreal p_len = fgitem.plusLength;

    //             plist.append(po);
    //             QPointF p1 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
    //             plist.append(p1);

    //             npoint = CalcVector2D::extendPoint(po, p1, 3.0);

    //         } else if (itype == FrameType::FRAMETYPE::DIAMETER) {
    //         } else if (itype == FrameType::FRAMETYPE::VARIABLE) {
    //         } else if (itype == FrameType::FRAMETYPE::CLCIRCLE) {
    //         } else if (itype == FrameType::FRAMETYPE::CLVARIABLE) {
    //         }

    //         FRAMEPOINTSVALUE fmp = FRAMEPOINTSVALUE(fr.GID, plist, fr.frameName, npoint, itype, st_value,
    //                                                 fr.UUID);
    //         fixedfrlist.append(fmp);
    //     }
    //            qDebug()<<"end of loop2";
    //            }
    //            tmplstfl1 << tmpfl1 << tmpfl1 << tmpfl1; //icjに同じ値入れる
    //            tmplstlstfl1 << tmplstfl1;
    //            tmplstfl1.clear();
    //        }
    //        if (model.numwwd != 0) {
    //            chck.plane_allowanceRateBending << tmplstlstfl1;//曲げ余裕率
    //            chck.plane_verificationRatioShear << tmplstlstfl1;//せん断余裕率（SRCの場合RC）
    //            chck.plane_verificationRatioShear2 << tmplstlstfl1;//せん断余裕率（SRCのS、RCの付着）
    //            chck.plane_verificationRatioBending << tmplstlstfl1;//検定比
    //        }
    //        tmplstlstfl1.clear();
    //    }

    //    qDebug() << "read CHCK";

    //    fchck.close();
    //    successToReadChckFile = true;
    //    return 0;

    //}

    // int handlePstn::readBLD3D_MODE()
    //{
    //     mode.clear();
    //     existModeFile = true;

    //    QFile fmode(filepath.absolutePath() + "/" + tmpdirname + "/BLD3D_MODE");
    ////    QString command = "\"" + apppath + "/7za.exe\" x -y -o" + tmpdirpath.absolutePath() +
    ////                      "/ -ir!BLD3D_MODE " + filename;
    //    QString command = "\""+apppath+"/7za.exe\" x -y \"-o" + tmpdirpath.absolutePath() + "/\" " +"\"-ir!BLD3D_MODE\" \""+filename+"\"";
    //    if (toDecompressPstn) {
    //        QProcess *decompchck = new QProcess;
    //        decompchck->execute(command);
    //        qDebug() << u8"MODEの解凍";
    //    }
    //    bool successFileOpen = fmode.open(QIODevice::ReadOnly);
    //    if (successFileOpen == false) {
    //        existModeFile = false;
    //        return -1;
    //    };

    //    QTextStream in(&fmode);
    //    QString line;

    //    line = in.readLine();
    //    mode.nummode = line.left(10).toInt();
    //    in.readLine();//空読み

    //    line = in.readLine();
    //    mode.totalMass << line.mid(0, 15).toDouble();
    //    mode.totalMass << line.mid(15, 15).toDouble();
    //    mode.totalMass << line.mid(30, 15).toDouble();
    //    mode.totalMass << line.mid(45, 15).toDouble();
    //    mode.totalMass << line.mid(60, 15).toDouble();
    //    mode.totalMass << line.mid(75, 15).toDouble();

    //    for (int i = 0; i < mode.nummode; i++) {
    //        line = in.readLine();
    //        mode.numj = line.mid(10, 10).toInt();
    //        mode.naturalPeriod << line.mid(20, 15).toDouble();
    //        QVector<qreal> beta;
    //        beta << line.mid(50, 15).toDouble() << line.mid(65, 15).toDouble() << line.mid(80, 15).toDouble()
    //             << line.mid(95, 15).toDouble() << line.mid(110, 15).toDouble() << line.mid(125, 15).toDouble();
    //        mode.beta << beta;

    //        QVector<XYZ> xyzlist;
    //        for (int j = 0; j < mode.numj; j++) {
    //            line = in.readLine();
    //            xyzlist << XYZ(line.mid(0, 15).toDouble(), line.mid(15, 15).toDouble(), line.mid(30,
    //                                                                                             15).toDouble());
    //        }
    //        //QVector<QVector<XYZ> > mode; //mode[nummode][jno]
    //        mode.mode << xyzlist;

    //    }
    //    return 0;
    //}
    // void handlePstn::makeUnifiedData()
    //{
    //    QHash<int, QUuid> floorNoToUuid;
    //    QHash<int, QUuid> frameGroupNoToUuid;
    //    QHash<int, QUuid> frameNoToUuid;
    //    QHash<int, QUuid> jointNoToUuid;
    //    QHash<int, QUuid> memberNoToUuid;
    //    QHash<int, QUuid> wallNoToUuid;
    //    QHash<int, QUuid> zwallNoToUuid;
    //    QHash<int, QUuid> springNoToUuid;

    //    if (!successToReadModelFile)return;
    //    //階
    //    QList<FLOORVALUES> fllist;
    //    for (int floorgroup = 0; floorgroup < this->model.numfg; floorgroup++) {
    //        int basefloorno = this->model.lfg[floorgroup][0] - 1;
    //        qreal basefloorlevel = this->model.dfg[floorgroup];
    //        for (int floornumber = 0; floornumber < this->model.numfl[floorgroup]; floornumber++) {
    //            FLOORVALUES flvalue;
    //            flvalue.floorName =
    //                this->model.namefl[floorgroup][floornumber].trimmed(); //MXJFL is NJFLの最大値。各節点はNJFL個の階に所属している。ただしpost3dでは一個なのでデータが落ちる
    //            flvalue.analysisName = this->model.namefl[floorgroup][floornumber].trimmed();
    //            flvalue.floorHeight = 0.0;
    //            flvalue.UUID = QUuid::createUuid();
    //            if (this->model.ifl[floorgroup][floornumber] == 2)flvalue.floorType = FLOORTYPE::FL_OVERGROUND;
    //            else if (this->model.ifl[floorgroup][floornumber] == 1)flvalue.floorType = FLOORTYPE::FL_UNDERGROUND;
    //            else if (this->model.ifl[floorgroup][floornumber] == 0)flvalue.floorType = FLOORTYPE::FL_CHILD;
    //            else if (this->model.ifl[floorgroup][floornumber] == 3)flvalue.floorType = FLOORTYPE::FL_PENTHOUSE;
    //            for (int fn = 0; fn < floornumber; fn++) {
    //                flvalue.floorHeight = flvalue.floorHeight + this->model.ahfl[floorgroup][fn];
    //            }
    //            if (floornumber == basefloorno)basefloorlevel -= flvalue.floorHeight;

    //            fllist.append(flvalue);
    //            floorNoToUuid[floornumber] = flvalue.UUID;
    //        }
    //        for (int i = 0; i < fllist.count(); i++) {
    //            fllist[i].floorHeight += basefloorlevel;
    //        }
    //    }
    //    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(fllist);
    //    qDebug() << "floorcount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPFLOOR);

    //    //section
    //    QList<SectionValues> allsectionvalues;
    //    QList<SectionValues> columnsectionvalues;
    //    QList<SectionValues> bracesectionvalues;
    //    QList<SectionValues> girdersectionvalues;
    ////    QHash<QString,SectionValues*> sectionNameTable;
    //    SectionValues sectionvalue;

    //    if (!this->successToReadModelFile)return;
    //    for (int ils = 0; ils < this->model.numlsd; ils++) {
    //        sectionvalue.sectionName = this->model.namels[ils].trimmed();
    //        switch (this->model.ils1[ils][0]) {
    //        case 1:
    //            sectionvalue.memberType = DATATYPE::TPCOLUMN;
    //            break;
    //        case 2:
    //            sectionvalue.memberType = DATATYPE::TPGIRDER;
    //            break;
    //        case 3:
    //            sectionvalue.memberType = DATATYPE::TPBRACE;
    //            break;
    //        default:
    //            sectionvalue.memberType = DATATYPE::TPCOLUMN;
    //        }

    //        int type = this->model.ils2[ils][0][0]; //=1::S,=2::RC,=3::SRC,=4::CFT
    //        switch (type) {
    //        case 1:
    //            sectionvalue.sectionType = STRUCTTYPE::STP_S;
    //            break;
    //        case 2:
    //            sectionvalue.sectionType = STRUCTTYPE::STP_RC;
    //            break;
    //        case 3:
    //            sectionvalue.sectionType = STRUCTTYPE::STP_SRC;
    //            break;
    //        case 4:
    //            sectionvalue.sectionType = STRUCTTYPE::STP_CFT;
    //            break;
    //        }
    //        SteelType::Type stltyp = SteelType::Type::UNDEFINED;
    //        SteelType::Type stltyp_T = SteelType::Type::UNDEFINED;
    //        qreal sizeH = 0.0;
    //        qreal sizeB = 0.0;
    //        qreal sizeH2 = 0.0;
    //        qreal sizeB2 = 0.0;
    //        int ictype = this->model.ils2[ils][0][3]; //コンクリート形状
    //        int istype = this->model.ils2[ils][0][4]; //鉄骨形状
    //        if (sectionvalue.sectionType == STRUCTTYPE::STP_S || sectionvalue.sectionType == STRUCTTYPE::STP_CFT) { //S or CFT
    //            if (istype == 1 || istype == 2) {
    //                stltyp = SteelType::Type::ST_H;   //direction=DIRECTION_Y;柱のH型
    //            } else if (istype == 3 || istype == 4) {
    //                stltyp_T =
    //                    SteelType::Type::ST_H;   //梁のH型、柱のI型。2,4はウェブが斜めに取りつくパターン
    //            }//direction=DIRECTION_Z;}
    //            else if (istype == 5 || istype == 30 || istype == 31 || istype == 32) {
    //                stltyp = SteelType::Type::ST_BX;   //30,31,32は冷間
    //            }//direction=DIRECTION_Y;}
    //            else if (istype == 6) {
    //                stltyp = SteelType::Type::ST_P;
    //            } else if (istype == 9 || istype == 10) {
    //                stltyp_T = SteelType::Type::ST_T1;   //9がWT1,10がWT2
    //            }//direction=DIRECTION_Z;}
    //            else if (istype == 7 || istype == 8) {
    //                stltyp = SteelType::Type::ST_T2;   //7がWT1,8がWT2
    //            }//direction=DIRECTION_Y;}
    //            else if (istype == 13 || istype == 14) {
    //                stltyp_T = SteelType::Type::ST_C1;   //13がC1,14がC2
    //            }//direction=DIRECTION_Z;}
    //            else if (istype == 11 || istype == 12) {
    //                stltyp = SteelType::Type::ST_C1;   //13がC1,14がC2
    //            }//direction=DIRECTION_Y;}
    //            else if (istype == 15) {
    //                stltyp = SteelType::Type::ST_R;   //R
    //            } else if (istype == 16) {
    //                stltyp = SteelType::Type::ST_BR;   //BR
    //            }//dshape=DRAWSHAPE_CIRCLE;direction=DIRECTION_Y;}
    //            else if (istype == 17 || istype == 45) {
    //                stltyp = SteelType::Type::ST_CROSSH;
    //                stltyp_T = SteelType::Type::ST_CROSSH;
    //            }//dshape=DRAWSHAPE_CROSSH;direction=DIRECTION_Z;}
    //            else if (istype == 46) {
    //                stltyp = SteelType::Type::ST_CROSS;
    //                stltyp_T = SteelType::Type::ST_CROSS;
    //            }//dshape=DRAWSHAPE_CROSS;direction=DIRECTION_Y;}
    //            sectionvalue.i_param.steelType = stltyp;
    //            sectionvalue.c_param.steelType = stltyp;
    //            sectionvalue.j_param.steelType = stltyp;
    //            sectionvalue.i_param.steelType_T = stltyp_T;
    //            sectionvalue.c_param.steelType_T = stltyp_T;
    //            sectionvalue.j_param.steelType_T = stltyp_T;

    //            if (stltyp == SteelType::Type::ST_BX && sectionvalue.memberType == DATATYPE::TPCOLUMN) {
    //                sizeH = this->model.dls1[ils][0][25]; //D
    //                sizeB = this->model.dls1[ils][0][24]; //b
    //            } else if (stltyp == SteelType::Type::ST_CROSS) { //direction Y
    //                sizeH = this->model.dls1[ils][0][25]; //sizeB
    //                sizeB = this->model.dls1[ils][0][24]; //sizeA
    //            } else if (stltyp == SteelType::Type::ST_BX || stltyp == SteelType::Type::ST_R) {
    //                sizeH = this->model.dls1[ils][0][25];
    //                sizeB = this->model.dls1[ils][0][24];
    //            } else if (stltyp == SteelType::Type::ST_BR || stltyp == SteelType::Type::ST_P) {
    //                sizeH = this->model.dls1[ils][0][24];
    //                sizeB = this->model.dls1[ils][0][24];
    //            } else if (stltyp == SteelType::Type::ST_CROSSH) {
    //                sizeH = this->model.dls1[ils][0][33];
    //                sizeB = this->model.dls1[ils][0][34];
    //                sizeH2 = this->model.dls1[ils][0][24];
    //                sizeB2 = this->model.dls1[ils][0][25];
    //            } else if (istype == 3 || istype == 4 || istype == 9 || istype == 10 || istype == 13
    //                       || istype == 14) { //dirZ
    //                sizeH = this->model.dls1[ils][0][33];
    //                sizeB = this->model.dls1[ils][0][34];
    //            } else { //direction Y
    //                sizeH = this->model.dls1[ils][0][24]; //sizeA
    //                sizeB = this->model.dls1[ils][0][25]; //sizeB
    //            }
    //        } else { //RC or SRC
    //            if (ictype == 20) {
    //                sectionvalue.i_param.concType = ConcreteType::Type::CIRCLE;
    //                sectionvalue.c_param.concType = ConcreteType::Type::CIRCLE;
    //                sectionvalue.j_param.concType = ConcreteType::Type::CIRCLE;
    //            }
    //            sectionvalue.i_param.concType = ConcreteType::Type::RECTANGLE;
    //            sectionvalue.c_param.concType = ConcreteType::Type::RECTANGLE;
    //            sectionvalue.j_param.concType = ConcreteType::Type::RECTANGLE;
    //            sizeH = this->model.dls1[ils][0][1]; //D
    //            sizeB = this->model.dls1[ils][0][0]; //b
    //        }
    //        sectionvalue.i_param.sizeH = sizeH;
    //        sectionvalue.i_param.sizeB = sizeB;
    //        sectionvalue.i_param.H_T = sizeH2;
    //        sectionvalue.i_param.B_T = sizeB2;
    //        sectionvalue.c_param.sizeH = sizeH;
    //        sectionvalue.c_param.sizeB = sizeB;
    //        sectionvalue.c_param.H_T = sizeH2;
    //        sectionvalue.c_param.B_T = sizeB2;
    //        sectionvalue.j_param.sizeH = sizeH;
    //        sectionvalue.j_param.sizeB = sizeB;
    //        sectionvalue.j_param.H_T = sizeH2;
    //        sectionvalue.j_param.B_T = sizeB2;
    //        sectionvalue.startFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().first().UUID;
    //        sectionvalue.endFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().last().UUID;
    //        allsectionvalues << sectionvalue;
    ////        sectionNameTable[sectionvalue.sectionName]=&sectionvalue;
    //        //            QColor linecolor=Qt::black;
    //        //            QColor paintcolor=Qt::lightGray;
    //        //            SECTIONDIM* section=new SECTIONDIM(secname,sectype,dshape,itype,direction,sizeH,sizeB,linecolor,paintcolor,sizeH2,sizeB2);
    //        //            currentSectionList.append(section);
    //    }
    //    foreach (SectionValues secitem, allsectionvalues) {
    //        if (secitem.memberType == DATATYPE::TPCOLUMN)columnsectionvalues << secitem;
    //        if (secitem.memberType == DATATYPE::TPGIRDER)columnsectionvalues << secitem;
    //        if (secitem.memberType == DATATYPE::TPBRACE)columnsectionvalues << secitem;
    //    }
    //    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPCOLUMN, columnsectionvalues);
    //    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPGIRDER, girdersectionvalues);
    //    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPBRACE, bracesectionvalues);

    //    //wall,slab
    //    QList<SectionValues> wallsectionvalues;
    //    for (int iisw = 0; iisw < this->model.numsw; iisw++) {
    //        SectionValues sectionvalue;
    //        sectionvalue.sectionName = this->model.namesw[iisw].trimmed();
    //        sectionvalue.plane_param.concThickness = this->model.dsw[iisw][0]; //壁厚t
    //        wallsectionvalues.append(sectionvalue);
    ////        sectionNameTable[sectionvalue.sectionName]=&sectionvalue;
    //    }
    //    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(DATATYPE::TPWALL, wallsectionvalues);

    //    //開口は3DVisualの構造体にも納めてそちらを使う。unifiedDataのほう、妥当か見直し
    ////    QList<SectionValues> wallopsectionvalues;
    ////    for(int numso=0;numso<this->model.numso;numso++){

    ////        for(int nso=0;nso<this->model.nso[numso];nso++){
    ////            SectionValues sectionvalue;
    ////            sectionvalue.sectionName=this->model.nameso[numso].trimmed();
    ////            sectionvalue.wopen_param.openType=WallOpenType::NORMAL;
    ////            sectionvalue.wopen_param.isXL=true;
    ////            sectionvalue.wopen_param.isYB=true;
    ////            if(this->model.iso[numso][nso][0]==0){
    ////                sectionvalue.wopen_param.openType=WallOpenType::SLIT;
    ////            }else if(this->model.iso[numso][nso][0]==1){
    ////                sectionvalue.wopen_param.isXL=true;
    ////            }else if(this->model.iso[numso][nso][0]==2){
    ////                sectionvalue.wopen_param.isXL=false;
    ////            }
    ////            if(this->model.iso[numso][nso][1]==0){
    ////                sectionvalue.wopen_param.openType=WallOpenType::SLIT;
    ////            }else if(this->model.iso[numso][nso][1]==1){
    ////                sectionvalue.wopen_param.isYB=true;
    ////            }else{sectionvalue.wopen_param.isYB=false;}
    ////            sectionvalue.wopen_param.ox=this->model.dso[numso][nso][0];//x
    ////            sectionvalue.wopen_param.oy=this->model.dso[numso][nso][1];//y
    ////            sectionvalue.wopen_param.width=this->model.dso[numso][nso][2];//l0
    ////            sectionvalue.wopen_param.height=this->model.dso[numso][nso][3];//h0
    ////            wallopsectionvalues.append(sectionvalue);
    ////        }
    ////    }
    ////    UnifiedAnalysisData::getOutInstance()->registerTempData(TPWALLOPEN,wallopsectionvalues);
    //    qDebug() << "opening";
    //    //openingはあとでかんがえます
    //    for (int numso = 0; numso < model.numso; numso++) {
    //        QString label = model.nameso[numso].trimmed();
    //        QList<Opening *> opening;
    //        for (int nso = 0; nso < model.nso[numso]; nso++) {
    //            Opening::LEFT_OR_RIGHT lr = Opening::LEFT_OR_RIGHT::CENTER;
    //            if (model.iso[numso][nso][0] == 0) {
    //                lr = Opening::LEFT_OR_RIGHT::CENTER;
    //            } else if (model.iso[numso][nso][0] == 1) {
    //                lr = Opening::LEFT_OR_RIGHT::LEFT;
    //            } else if (model.iso[numso][nso][0] == 2) {
    //                lr = Opening::LEFT_OR_RIGHT::RIGHT;
    //            }
    //            Opening::TOP_OR_BOTTOM tb = Opening::TOP_OR_BOTTOM::MIDDLE;
    //            if (model.iso[numso][nso][1] == 0) {
    //                tb = Opening::TOP_OR_BOTTOM::MIDDLE;
    //            } else if (model.iso[numso][nso][1] == 1) {
    //                tb = Opening::TOP_OR_BOTTOM::BOTTOM;
    //            } else {
    //                tb = Opening::TOP_OR_BOTTOM::TOP;
    //            }
    //            qreal xpos = model.dso[numso][nso][0]; //x
    //            qreal ypos = model.dso[numso][nso][1]; //y
    //            qreal width = model.dso[numso][nso][2]; //l0
    //            qreal height = model.dso[numso][nso][3]; //h0
    //            if (width == 0)lr = Opening::LEFT_OR_RIGHT::WFULL;
    //            if (height == 0)tb = Opening::TOP_OR_BOTTOM::HFULL;
    //            Opening *op = new Opening(lr, tb, xpos, ypos, width, height);
    //            opening.append(op);
    //        }
    //        OpeningSet *openingset = new OpeningSet(label, opening);
    //        openingList.append(openingset);
    //    }
    //    qDebug() << "opening";

    //    //通りグループ
    //    QList<FRAMEGROUPVALUE> frgrouplist;
    //    QList<FRAMEPOINTSVALUE> frlist;
    //    QHash<int, qreal> minimumcoordinate;
    //    for (int gridgroup = 0; gridgroup < this->model.numgg; gridgroup++) {
    //        FRAMEGROUPVALUE fgvalue;
    //        fgvalue.GID = QUuid::createUuid();
    //        frameGroupNoToUuid[gridgroup] = fgvalue.GID;
    //        fgvalue.groupName = QString::number(gridgroup);
    //        if (this->model.igg[gridgroup] == 0)fgvalue.frameType = FRAMETYPE::STRAIGHT;
    //        else if (this->model.igg[gridgroup] == 1)fgvalue.frameType = FRAMETYPE::CIRCLE;
    //        else if (this->model.igg[gridgroup] == 2)fgvalue.frameType = FRAMETYPE::RADIUS;
    //        else fgvalue.frameType = FRAMETYPE::NOFRAMETYPE;
    //        fgvalue.count = this->model.numgr[gridgroup];
    //        fgvalue.basePoint = QPointF(this->model.dgg[gridgroup][0], this->model.dgg[gridgroup][1]);
    //        fgvalue.startAngle = this->model.dgg[gridgroup][2] * 180. / M_PI;
    //        fgvalue.endAngle = this->model.dgg[gridgroup][3] * 180. / M_PI;
    //        fgvalue.division = 36;
    //        fgvalue.startFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().first().UUID;
    //        fgvalue.endFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().last().UUID;
    //        frgrouplist << fgvalue;
    //        //通り
    //        for (int gridnumber = 0; gridnumber < this->model.numgr[gridgroup]; gridnumber++) {
    //            qreal coordinate = 0.0;
    //            for (int gn = 0; gn < gridnumber; gn++) {
    //                coordinate = coordinate + this->model.dgr[gridgroup][gn];
    //            }
    //            if (!minimumcoordinate.contains(gridgroup))minimumcoordinate[gridgroup] = coordinate;
    //            else if (minimumcoordinate[gridgroup] > coordinate)minimumcoordinate[gridgroup] = coordinate;
    //        }
    //        for (int gridnumber = 0; gridnumber < this->model.numgr[gridgroup]; gridnumber++) {
    //            qreal coordinate = 0.0;
    //            for (int gn = 0; gn < gridnumber; gn++) {
    //                coordinate = coordinate + this->model.dgr[gridgroup][gn];
    //            }
    //            qreal nextSpan = this->model.dgr[gridgroup][gridnumber];
    ////            if(nextSpan==0.0)coordinate+=0.000001;
    //            if (nextSpan == 0.0)coordinate -= 0.000001;

    //            QList<QPointF> plist;
    //            qreal span = 0.0;
    //            QPointF npoint;
    //            STRAIGHTFRAMEVALUE st_value;
    //            if (fgvalue.frameType == FRAMETYPE::STRAIGHT) {
    //                QPointF po = fgvalue.basePoint;
    //                qreal theta = fgvalue.startAngle / 180.0 * M_PI;
    //                span = coordinate - minimumcoordinate[gridgroup];
    //                qreal p_len =
    //                    fgvalue.plusLength; //通りのプラスマイナスをどこまで伸ばすかは節点の情報がないと決まらない
    //                qreal m_len = fgvalue.minusLength;
    //                QPointF p1, p2, p3, p4;
    //                QPointF spoint;
    //                qreal m_span = nextSpan;
    //                qreal s_angle;

    //                if ( fgvalue.startAngle < 45.0 ) {
    //                    p1 = QPointF(-m_len * qCos(theta) - span * qSin(theta),
    //                                 -m_len * qSin(theta) + span * qCos(theta)) + po;
    //                    p2 = QPointF( p_len * qCos(theta) - span * qSin(theta),
    //                                  p_len * qSin(theta) + span * qCos(theta)) + po;
    //                    npoint = CalcVector2D::extendPoint(p1, p2, 1.0);
    //                    s_angle = fgvalue.startAngle + 90.;
    //                    if (gridnumber == fgvalue.count - 1) {
    //                        p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
    //                        p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                        spoint = CalcVector2D::extendPoint(p3, p4, 1.5);
    //                    } else {
    //                        spoint = CalcVector2D::extendPoint(p1, p2, 0.5);
    //                    }
    //                } else {
    //                    p1 = QPointF(-m_len * qCos(theta) + span * qSin(theta),
    //                                 -m_len * qSin(theta) - span * qCos(theta)) + po;
    //                    p2 = QPointF( p_len * qCos(theta) + span * qSin(theta),
    //                                  p_len * qSin(theta) - span * qCos(theta)) + po;
    //                    npoint = CalcVector2D::extendPoint(p2, p1, 1.0);
    //                    s_angle = fgvalue.startAngle - 90.;
    //                    if (gridnumber == fgvalue.count - 1) {
    //                        p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
    //                        p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                        spoint = CalcVector2D::extendPoint(p4, p3, 1.5);
    //                    } else {
    //                        spoint = CalcVector2D::extendPoint(p2, p1, 0.5);
    //                    }
    //                }
    //                plist.append(p1);
    //                plist.append(p2);

    //                if (gridnumber == fgvalue.count - 1) m_span = span;
    //                st_value = STRAIGHTFRAMEVALUE(m_span, s_angle, spoint,
    //                                              (gridnumber == 0), (gridnumber == fgvalue.count - 1));

    //            } else if (fgvalue.frameType == FRAMETYPE::CIRCLE) {

    //                QPointF po = fgvalue.basePoint;
    //                qreal s_angle = fgvalue.startAngle;
    //                qreal e_angle = fgvalue.endAngle;
    //                span = coordinate - minimumcoordinate[gridgroup];
    //                int div = fgvalue.division;

    //                if ( (s_angle < e_angle) && (e_angle - s_angle < 360.0) && div > 0 ) {
    //                    qreal s_theta = s_angle / 180.0 * M_PI;
    //                    qreal e_theta = e_angle / 180.0 * M_PI;
    //                    qreal d_theta = (e_theta - s_theta) / qreal(div);
    //                    for ( int j = 0; j < div + 1; j++ ) {
    //                        qreal j_theta = s_theta + d_theta * qreal(j);
    //                        QPointF p1 = QPointF(po.x() + span * qCos(j_theta), po.y() + span * qSin(j_theta));
    //                        plist.append(p1);
    //                    }
    //                }

    //                if (plist.count() > 1)
    //                    npoint = CalcVector2D::extendPoint(plist.at(1), plist.at(0), 3.0);

    //            } else if (fgvalue.frameType == FRAMETYPE::RADIUS) {

    //                QPointF po = fgvalue.basePoint;
    //                qreal angle = fgvalue.startAngle + (coordinate - minimumcoordinate[gridgroup]) * 180.0 / M_PI;
    //                qreal theta = angle / 180.0 * M_PI;
    //                qreal p_len = fgvalue.plusLength;

    //                plist.append(po);
    //                QPointF p1 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                plist.append(p1);

    //                npoint = CalcVector2D::extendPoint(po, p1, 3.0);

    //            } else {
    //                QPointF po = fgvalue.basePoint;
    //                QPointF p1 = QPointF(1.0, 0) + po;
    //                plist.append(po);
    //                plist.append(p1);
    //                npoint = CalcVector2D::extendPoint(po, p1, 3.0);
    //            }
    ////            else if(fgvalue.frameType==DIAMETER){
    ////            }else if(fgvalue.frameType==VARIABLE){
    ////            }else if(fgvalue.frameType==CLCIRCLE){
    ////            }else if(fgvalue.frameType==CLVARIABLE){
    ////            }else{

    ////            }//定義してない通りタイプが来た時、pointlistが空になる
    //            QString name = this->model.namegr[gridgroup][gridnumber].trimmed(); //GRは複数の通りに所属
    //            QString uid = QUuid::createUuid().toString() ;
    ////            QList<int> grid_frame;grid_frame<<gridgroup;grid_frame<<gridnumber;
    //            frameNoToUuid[gridgroup * 10000 + gridnumber] = uid;

    //            FRAMEPOINTSVALUE fmp = FRAMEPOINTSVALUE(fgvalue.GID, plist, name, npoint, fgvalue.frameType,
    //                                                    st_value, uid);
    //            frlist.append(fmp);
    ////            gcount++;
    //        }

    //    }
    //    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(frgrouplist, frlist);
    //    qDebug() << "framepointcount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPFRAMEP);

    //    //Joint
    //    //節点

    //    jointNoToUuid.clear();
    //    QList<FLOORVALUES> floorlist = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList();
    //    QList<JointData *> jdlist;
    //    QList<QList<QUuid> > flooridlist;
    //    QList<QList<QUuid> > frameidlistlist;
    //    for (int jno = 0; jno < this->model.numj; jno++) {
    //        GlobalPoint gp(this->model.xyz[jno][0], this->model.xyz[jno][1], this->model.xyz[jno][2]);
    //        JointData *njd = UnifiedFixedData::getInstance()->getAnalysisData()->createAnalysisJoint(gp, this->model.jntnm[jno], true, QUuid::createUuid());
    //        njd->setBoundary(this->model.indj[jno]);
    //        QList<JOINTSPRING *> springs;
    //        for (int nbj = 0; nbj < this->model.ibj.count(); nbj++) {
    ////                qDebug()<<"bane joint"<<this->model.ibj.at(nbj).at(0);
    //            if (this->model.ibj.at(nbj).at(0) != jno + 1)continue;
    //            JOINTSPRING *js = new JOINTSPRING;
    //            js->uid = QUuid::createUuid();
    //            js->springNo = this->model.ibj.at(nbj).at(3);
    //            js->springVec = VEC3D(model.dbj.at(nbj).at(0), model.dbj.at(nbj).at(1), model.dbj.at(nbj).at(2));
    //            if (this->model.ibj.at(nbj).at(1) == 0) {
    //                js->isRotation = false;
    //            } else {
    //                js->isRotation = true;
    //            }
    //            springs.append(js);
    //            springNoToUuid.insert(js->springNo, js->uid);
    //            qDebug() << "somespring defined";
    //        }
    //        njd->setJointSpring(springs);
    //        jointNoToUuid.insert(jno, njd->getUuid());
    //        jdlist.append(njd);
    //        //int floorgroup=this->model.jntfl[jno][0][0]-1;
    //        int floornumber = this->model.jntfl[jno][0][1] - 1;
    //        QUuid flid = floorNoToUuid[floornumber];
    //        QUuid frid;

    //        //古いコードそのままのため表現冗長。つまりはflidとfridのリストを作っているだけ
    //        QList<QUuid> frameidlist;
    //        foreach (FLOORVALUES fv, floorlist) {
    //            if (fv.UUID == flid) {
    //                QList<QUuid> dummylist;
    //                dummylist << fv.UUID;
    //                flooridlist << dummylist;
    //                QList<FRAMEPOINTSVALUE> framelist;
    //                framelist = UnifiedFixedData::getInstance()->getAnalysisData()->getFrameValuesOfFloor(fv.UUID);
    //                for (int gr = 0; gr < this->model.njgr[jno]; gr++) {
    //                    int gridgroup = this->model.jntgr[jno][gr][0] - 1;
    //                    int gridnumber = this->model.jntgr[jno][gr][1] - 1;
    //                    if (gridgroup != -1) {
    //                        frid = frameNoToUuid[gridgroup * 10000 + gridnumber];
    //                    }
    //                    foreach (FRAMEPOINTSVALUE fpv, framelist) {
    //                        if (fpv.UUID == frid)frameidlist << fpv.UUID;
    //                    }
    //                }
    //                break;
    //            }
    //        }
    //        frameidlistlist << frameidlist;

    //    }
    //    UnifiedFixedData::getInstance()->getAnalysisData()->changeJointsAttachment(jdlist, flooridlist, DATATYPE::TPFLOOR, true);
    //    UnifiedFixedData::getInstance()->getAnalysisData()->changeJointsAttachment(jdlist, frameidlistlist, DATATYPE::TPFRAMEP, true);
    //    qDebug() << "jointcount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPJOINT);

    //    //全節点を当該framegroupに投影したときの座標を拾って最大最小を決める
    //    //計算量大のためもっと単純化できるか
    //    for (int nfg = 0; nfg < frgrouplist.count(); nfg++) {
    ////                if(nfg==2){
    ////                    qDebug()<<"nfg too large"<<frgrouplist.count();
    //////                    break;
    ////                }
    //        qDebug() << "nfg=" << nfg;
    //        qDebug() << "frgrouplist.count=" << frgrouplist.count();
    //        qDebug() << "frlist.count=" << frlist.count();
    //        FRAMEGROUPVALUE fgv = frgrouplist[nfg];
    ////                qDebug()<<"get fgv";
    //        qreal minus = 0.0;
    //        qreal plus = 0.0;
    //        foreach (FRAMEPOINTSVALUE frpv, frlist) {
    //            qDebug()<<frpv.frameName;
    //            if (fgv.frameType == FRAMETYPE::STRAIGHT && !frpv.straight.isFirst)continue;
    //            if (frpv.GID == fgv.GID) {
    //                QList<XYZ> globalxyzs;
    //                QList<JointData*> joints=UnifiedFixedData::getInstance()->getAnalysisData()->jointListOfFrame(frpv.UUID);
    //                for (int i = 0; i < joints.count(); i++) {
    ////                            qDebug()<<"get jdlist"<<i;
    //                    globalxyzs << joints.at(i)->xyz();
    //                    qDebug()<<i;
    //                }
    ////                        qDebug()<<"globalxyzcount="<<globalxyzs.count();
    ////                        qDebug()<<frpv.UUID;
    //                QList<XYZ> framexyzs = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameXYZ(frpv.UUID, globalxyzs);
    ////                        qDebug()<<"framexyzcount="<<framexyzs.count();
    //                qDebug()<<"aaa";
    //                foreach (XYZ framexyz, framexyzs) {
    //                    if (framexyz.x < minus)minus = framexyz.x;
    //                    if (framexyz.x > plus)plus = framexyz.x;
    //                }
    //            }
    //        }
    //        minus = qMax(1.0, qAbs(minus - 1.0));
    ////                qDebug()<<"frgrouplist call"<<nfg;
    //        frgrouplist[nfg].minusLength = minus;
    //        frgrouplist[nfg].plusLength = plus - minus;
    ////                qDebug()<<"plus,minus="<<plus<<minus;
    //    }
    //            qDebug()<<"end of loop1";

    //    QList<FRAMEPOINTSVALUE> fixedfrlist;
    //    qreal span = 0.0;
    //    for (int frnum = 0; frnum < frlist.count(); frnum++) {
    //        FRAMEPOINTSVALUE fr = frlist.at(frnum);
    //        FRAMEGROUPVALUE fgitem;
    //        FRAMETYPE itype = fr.frameType;
    //        for (int i = 0; i < frgrouplist.count(); i++) {
    //            if (frgrouplist.at(i).GID == fr.GID) {
    //                fgitem = frgrouplist.at(i);
    //                break;
    //            }
    //        }
    //        QPointF npoint;
    //        QList<QPointF> plist;
    //        STRAIGHTFRAMEVALUE st_value;
    //        if (itype == FRAMETYPE::STRAIGHT) {
    //            if (fr.straight.isFirst)span = 0.0;
    //            qreal m_len = fgitem.minusLength;
    //            qreal p_len = fgitem.plusLength;
    //            qreal theta = fgitem.startAngle / 180.0 * M_PI;
    //            QPointF po = fgitem.basePoint;
    //            QPointF spoint;
    //            QPointF p1, p2, p3, p4;
    //            if ( fgitem.startAngle < 45.0 ) {
    //                p1 = QPointF(-m_len * qCos(theta) - span * qSin(theta),
    //                             -m_len * qSin(theta) + span * qCos(theta)) + po;
    //                p2 = QPointF( p_len * qCos(theta) - span * qSin(theta),
    //                              p_len * qSin(theta) + span * qCos(theta)) + po;
    //                npoint = CalcVector2D::extendPoint(p1, p2, 5.0);
    //                if (fr.straight.isLast) {
    //                    p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
    //                    p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                    spoint = CalcVector2D::extendPoint(p3, p4, 1.5);
    //                } else {
    //                    spoint = CalcVector2D::extendPoint(p1, p2, 0.5);
    //                }
    //            } else {
    //                p1 = QPointF(-m_len * qCos(theta) + span * qSin(theta),
    //                             -m_len * qSin(theta) - span * qCos(theta)) + po;
    //                p2 = QPointF( p_len * qCos(theta) + span * qSin(theta),
    //                              p_len * qSin(theta) - span * qCos(theta)) + po;
    //                npoint = CalcVector2D::extendPoint(p2, p1, 5.0);
    //                if (fr.straight.isLast) {
    //                    p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
    //                    p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
    //                    spoint = CalcVector2D::extendPoint(p4, p3, 1.5);
    //                } else {
    //                    spoint = CalcVector2D::extendPoint(p2, p1, 0.5);
    //                }
    //            }
    //            plist.append(p1);
    //            plist.append(p2);
    //            st_value = STRAIGHTFRAMEVALUE(fr.straight.span, fr.straight.angle, spoint,
    //                                          fr.straight.isFirst, fr.straight.isLast);
    //            span += fr.straight.span;

    //        } else if (itype == FRAMETYPE::CIRCLE) {

    //            plist = fr.pointList;
    //            npoint = fr.namePoint;

    //        } else if (itype == FRAMETYPE::RADIUS) {

    //            QPointF po = fgitem.basePoint;
    //            qreal theta = atan2(fr.pointList.last().y() - fr.pointList.first().y(),
    //                                fr.pointList.last().x() - fr.pointList.first().x());
    //            qreal p_len = fgitem.plusLength;

    //            plist.append(po);
    //            QPointF p1 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
    //            plist.append(p1);

    //            npoint = CalcVector2D::extendPoint(po, p1, 3.0);

    //        } else if (itype == FRAMETYPE::DIAMETER) {
    //        } else if (itype == FRAMETYPE::VARIABLE) {
    //        } else if (itype == FRAMETYPE::CLCIRCLE) {
    //        } else if (itype == FRAMETYPE::CLVARIABLE) {
    //        }

    //        FRAMEPOINTSVALUE fmp = FRAMEPOINTSVALUE(fr.GID, plist, fr.frameName, npoint, itype, st_value,
    //                                                fr.UUID);
    //        fixedfrlist.append(fmp);
    //    }
    ////            qDebug()<<"end of loop2";

    //    UnifiedFixedData::getInstance()->getAnalysisData()->registerTempData(frgrouplist, fixedfrlist);

    //    //部材
    //    for (int numm = 0; numm < this->model.numm; numm++) {
    //        int ils = this->model.imm1[numm][0] - 1;
    //        QString secname = this->model.namels[ils].trimmed();

    ////            SectionValues* sv=sectionNameTable[secname];
    //        QStringList columnnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPCOLUMN);
    //        QStringList girdernamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPGIRDER);
    //        QStringList bracenamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPBRACE);
    //        QStringList wallnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPWALL);

    //        SectionValues sv;
    //        if (columnnamelist.contains(secname))sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPCOLUMN,
    //                                                                                                   secname);
    //        else if (girdernamelist.contains(secname))sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(
    //                                                               DATATYPE::TPGIRDER, secname);
    //        else if (bracenamelist.contains(secname))sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPBRACE,
    //                                                                                                       secname);
    //        else if (wallnamelist.contains(secname))sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPWALL,
    //                                                                                                      secname);
    //        else sv = SectionValues();

    //        DATATYPE dtyp;
    //        if (sv.memberType == DATATYPE::TPCOLUMN)dtyp = DATATYPE::TPCOLUMN;
    //        else if (sv.memberType == DATATYPE::TPGIRDER)dtyp = DATATYPE::TPGIRDER;
    //        else if (sv.memberType == DATATYPE::TPBRACE)dtyp = DATATYPE::TPBRACE;
    //        else if (sv.memberType == DATATYPE::TPWALL)dtyp = DATATYPE::TPWALL;
    //        else dtyp = DATATYPE::TPCOLUMN;
    ////        qDebug() << "dtype=" << dtyp << secname << sv.sectionName;

    //        QList<JointData *> jdlist;
    //        int jno;
    //        //JointData i
    //        jno = this->model.indm[numm][0] - 1;
    //        QUuid ijid = jointNoToUuid[jno];
    //        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(ijid));

    //        //JointData j
    //        jno = this->model.indm[numm][1] - 1;
    //        QUuid jjid = jointNoToUuid[jno];
    //        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jjid));

    //        QUuid uid = QUuid::createUuid();
    //        UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jdlist, dtyp, secname, uid );
    //        UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid)->setFaceValues(true, 1, true,
    //                                                                          this->model.dmm1[numm][6] * 1000, true, 1, true, this->model.dmm1[numm][7] * 1000);
    ////        qDebug()<<modelcase;
    //        foreach(QString model,modelcase){
    //            PREADATA* pa = new PREADATA;
    //            pa->phaiIy=prea.value(model).dmm2[numm][1];
    //            pa->EIy=prea.value(model).dmm2[numm][14];
    //            pa->phaiIz=prea.value(model).dmm2[numm][2];
    //            pa->EIz=prea.value(model).dmm2[numm][15];
    //            UnifiedFixedData::getInstance()->getPstnData()->setPrea(model,uid,pa);
    //        }
    //        UnifiedFixedData::getInstance()->getPstnData()->setModelCaseList(modelcase);

    //        PREMDATA* pm = new PREMDATA;
    //        pm->Lky=model.dmm1[numm][20];
    //        pm->Lkz=model.dmm1[numm][21];
    //        UnifiedFixedData::getInstance()->getPstnData()->setPrem(uid,pm);

    //        memberNoToUuid[numm] = uid;
    //        //code angle
    //        qreal betaangle;//pos4参照 rad.
    //        QVector3D vecX, vecY, vecZ, unitZ, dvecY, dvecZ;
    //        qreal cosbeta, sinbeta;
    //        vecX.setX(this->model.evmem[numm][0][0]);
    //        vecX.setY(this->model.evmem[numm][0][1]);
    //        vecX.setZ(this->model.evmem[numm][0][2]);
    //        vecY.setX(this->model.evmem[numm][1][0]);
    //        vecY.setY(this->model.evmem[numm][1][1]);
    //        vecY.setZ(this->model.evmem[numm][1][2]);
    //        vecZ = QVector3D::crossProduct(vecX, vecY);
    //        unitZ.setX(0.0);
    //        unitZ.setY(0.0), unitZ.setZ(1.0);
    //        const qreal eps = 0.001; //鉛直かどうかの閾値
    //        if (vecX.x()*vecX.x() + vecX.y()*vecX.y() < eps) {
    //            betaangle = atan2(vecY.y(), vecY.x());
    //        } else {
    //            dvecY = QVector3D::crossProduct(unitZ, vecX);
    //            dvecY.normalize();
    //            dvecZ = QVector3D::crossProduct(vecX, dvecY);
    //            cosbeta = QVector3D::dotProduct(dvecY, vecY);
    //            sinbeta = QVector3D::dotProduct(dvecZ, vecY);
    //            betaangle = atan2(sinbeta, cosbeta);
    //        }

    //        UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid)->setCodeAngle(betaangle);
    //    }
    //    qDebug() << "columncount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPCOLUMN);
    //    qDebug() << "girdercount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPGIRDER);
    //    qDebug() << "member";

    //    //耐震壁
    //    for (int numww = 0; numww < this->model.numww; numww++) {
    //        qDebug() << "wallnum=" << numww;
    //        //中間節点含む節点
    ////        int nwjnt = this->model.iww1[numww][3]; //周囲節点数
    ////        QList<JointData *> jlist;
    ////        for (int jnt = 0; jnt < nwjnt; jnt++) {
    ////            int jno = this->model.iww2[numww][jnt] - 1;
    ////            jlist.append(UnifiedAnalysisData::getOutInstance()->jointIDToPointer(jointNoToUuid[jno]));
    ////        }

    //        //周囲4点のみ
    //        int nwjnt = 4;
    //        QList<JointData*> jlist;
    //        for (int jnt = 0; jnt < nwjnt; jnt++) {
    //            int jno = this->model.iww1[numww][5+jnt] - 1;
    //            jlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jointNoToUuid[jno]));
    //        }

    //        QString secname = this->model.namesw[this->model.iww1[numww][1] - 1].trimmed();

    //        QStringList wallnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPWALL);
    //        SectionValues sv;
    //        if (wallnamelist.contains(secname))sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPWALL,
    //                                                                                                 secname);
    //        else continue;

    //        QUuid uid = QUuid::createUuid();
    //        UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jlist, DATATYPE::TPWALL, secname, uid );
    //        MemberData *data = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);

    //        qDebug() << "createmember" << secname;

    //        data->setSectionName(*UnifiedFixedData::getInstance()->getAnalysisData(), sv.sectionName);
    //        int op = this->model.iww1[numww][2] - 1; //開口番号
    //        if (op >= 0)data->setWallOpen(this->model.nameso[op]);

    //        data->setPlaneModel(PLANEMODEL::WALL_MODEL);
    //        //sectionNameToPlanePropertyなどでは断面の方からデータを引いているのでそちらにも登録必要だが、雑壁か耐震壁かは部材に紐づくべき？
    ////                sv.plane_param.planeType=PlaneType::WALL;
    //        int direction = this->model.iww1[numww][4]; //1:WI,2:WH,3:SI,4:SH
    //        switch (direction) {
    //        case 1:
    //            data->setPlaneSubModel(PLANESUBMODEL::WI_MODEL);
    ////                    sv.plane_param.planeType2=PlaneType2::WI;
    //            break;
    //        case 2:
    //            data->setPlaneSubModel(PLANESUBMODEL::WH_MODEL);
    ////                    sv.plane_param.planeType2=PlaneType2::WH;
    //            break;
    //        case 3:
    //            data->setPlaneSubModel(PLANESUBMODEL::SI_MODEL);
    ////                    sv.plane_param.planeType2=PlaneType2::SI;
    //            break;
    //        case 4:
    //            data->setPlaneSubModel(PLANESUBMODEL::SH_MODEL);
    ////                    sv.plane_param.planeType2=PlaneType2::SH;
    //            break;
    //        default:
    //            data->setPlaneSubModel(PLANESUBMODEL::NONSTRUCT_SUBMODEL);
    ////                    sv.plane_param.planeType2=PlaneType2::WI;
    //            break;
    //        }
    //        wallNoToUuid[numww] = uid;
    //        foreach(QString model,modelcase){
    //            PREADATA* pa = new PREADATA;
    //            pa->lenBeamB=prea.value(model).debww[numww][0];
    //            pa->lenBeamU=prea.value(model).debww[numww][1];
    //            pa->lenColumn=prea.value(model).debww[numww][2];
    //            pa->phaiB=prea.value(model).debww[numww][12];
    //            pa->phaiU=prea.value(model).debww[numww][13];
    //            qDebug()<<"wall pa"<<pa->lenBeamB<<pa->lenBeamU<<pa->lenColumn<<pa->phaiB<<pa->phaiU;
    //            UnifiedFixedData::getInstance()->getPstnData()->setPrea(model,uid,pa);
    //        }

    //                VEC3D vecX, vecY, vecZ;
    //                vecX = VEC3D(this->model.evmemw[numww][0][0],this->model.evmemw[numww][0][1],this->model.evmemw[numww][0][2]);
    //                vecY = VEC3D(this->model.evmemw[numww][1][0],this->model.evmemw[numww][1][1],this->model.evmemw[numww][1][2]);
    //                vecZ = CalcVector3D::crossNormalized(vecX, vecY);
    //                UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid)->setDirectionVector(vecX,vecY);
    ////        //code angle
    ////        qreal betaangle;//pos4参照 rad.
    ////        QVector3D vecX, vecY, vecZ, unitZ, dvecY, dvecZ;
    ////        qreal cosbeta, sinbeta;
    ////        vecX.setX(this->model.evmemw[numww][0][0]);
    ////        vecX.setY(this->model.evmemw[numww][0][1]);
    ////        vecX.setZ(this->model.evmemw[numww][0][2]);
    ////        vecY.setX(this->model.evmemw[numww][1][0]);
    ////        vecY.setY(this->model.evmemw[numww][1][1]);
    ////        vecY.setZ(this->model.evmemw[numww][1][2]);
    ////        vecZ = QVector3D::crossProduct(vecX, vecY);
    ////        unitZ.setX(0.0);
    ////        unitZ.setY(0.0), unitZ.setZ(1.0);
    ////        const qreal eps = 0.001; //鉛直かどうかの閾値
    ////        if (vecX.x()*vecX.x() + vecX.y()*vecX.y() < eps) {
    ////            betaangle = atan2(vecY.y(), vecY.x());
    ////        } else {
    ////            dvecY = QVector3D::crossProduct(unitZ, vecX);
    ////            dvecY.normalize();
    ////            dvecZ = QVector3D::crossProduct(vecX, dvecY);
    ////            cosbeta = QVector3D::dotProduct(dvecY, vecY);
    ////            sinbeta = QVector3D::dotProduct(dvecZ, vecY);
    ////            betaangle = atan2(sinbeta, cosbeta);
    ////        }

    ////        UnifiedAnalysisData::getOutInstance()->memberIDToPointer(uid)->setCodeAngle(betaangle);

    //    }
    //    //雑壁
    //    for (int numwz = 0; numwz < this->model.numwz; numwz++) {
    //        int nwjnt = this->model.iwz1[numwz][3]; //周囲節点数
    //        QList<JointData *> jlist;
    //        for (int jnt = 0; jnt < nwjnt; jnt++) {
    //            int jno = this->model.iwz2[numwz][jnt] - 1;
    //            jlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jointNoToUuid[jno]));
    //        }
    //        QString secname = this->model.namesw[this->model.iwz1[numwz][1] - 1].trimmed();
    //        QStringList wallnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPWALL);
    //        SectionValues sv;
    //        if (wallnamelist.contains(secname))sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPWALL,
    //                                                                                                 secname);
    //        else continue;
    ////
    //        QUuid uid = QUuid::createUuid();
    //        UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jlist, DATATYPE::TPWALL, secname, uid );
    //        MemberData *data = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);
    ////
    //        data->setSectionName(*UnifiedFixedData::getInstance()->getAnalysisData(), sv.sectionName);
    //        int op = this->model.iwz1[numwz][2] - 1; //開口番号
    //        if (op >= 0)data->setWallOpen(this->model.nameso[op]);
    //        data->setPlaneModel(PLANEMODEL::ZWALL_MODEL);
    ////                sv.plane_param.planeType=PlaneType::ZWALL;

    //        zwallNoToUuid[numwz] = uid;

    //    }
    //    qDebug() << "bracecount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPBRACE);
    //    qDebug() << "wallcount=" << UnifiedFixedData::getInstance()->getAnalysisData()->getDataCount(DATATYPE::TPWALL);

    //    //スラブ(平面応力要素未実装のため一旦非表示)
    //    for (int numws = 0; numws < this->model.numws; numws++) {
    //        QList<JointData *> jlist;
    //        jlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jointNoToUuid[this->model.iws1[numws][4] -
    //                                                                                                           1]));
    //        jlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jointNoToUuid[this->model.iws1[numws][5] -
    //                                                                                                           1]));
    //        jlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jointNoToUuid[this->model.iws1[numws][6] -
    //                                                                                                           1]));
    //        jlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jointNoToUuid[this->model.iws1[numws][7] -
    //                                                                                                           1]));
    //        QString secname = this->model.namesw[this->model.iws1[numws][1] - 1].trimmed();

    //        QStringList slabnamelist = UnifiedFixedData::getInstance()->getAnalysisData()->getNameList(DATATYPE::TPSLAB);
    //        SectionValues sv;
    //        if (slabnamelist.contains(secname))sv = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(DATATYPE::TPSLAB,
    //                                                                                                 secname);
    //        else continue;
    ////
    //        QUuid uid = QUuid::createUuid();
    //        UnifiedFixedData::getInstance()->getAnalysisData()->createMember(jlist, DATATYPE::TPSLAB, secname, uid );

    //    }
    //    qDebug() << "plane/N-spring" << this->model.numbn;

    //    //軸ばね
    //    for (int numbn = 0; numbn < this->model.numbn; numbn++) {

    //        QString springname = QString();

    //        int springCharacteristic = 0;
    //        int springNo = this->model.ibn[numbn][3];

    //        B3D_DATATYPE type = B3D_DATATYPE::B3D_TPSPRING_N;

    //        QList<JointData *> jdlist;
    //        int jno;
    //        //JointData i
    //        jno = this->model.ibn[numbn][0] - 1;
    //        QUuid ijid = jointNoToUuid[jno];
    //        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(ijid));

    //        //JointData j
    //        jno = this->model.ibn[numbn][1] - 1;
    //        QUuid jjid = jointNoToUuid[jno];
    //        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jjid));

    //        QUuid uid = QUuid::createUuid();
    //        UnifiedFixedData::getInstance()->getAnalysisData()->createSpring(jdlist, type, springname, springCharacteristic, springNo,
    //                                                 uid );
    //        springNoToUuid.insert(springNo, uid);
    //    }
    //    //せん断ばね
    //    for (int numbq = 0; numbq < this->model.numbq; numbq++) {

    //        QString springname = QString();

    //        int springCharacteristic = 0;
    //        int springNo = this->model.ibq[numbq][3];

    //        B3D_DATATYPE type = B3D_DATATYPE::B3D_TPSPRING_Q;

    //        QList<JointData *> jdlist;
    //        int jno;
    //        //JointData i
    //        jno = this->model.ibq[numbq][0] - 1;
    //        QUuid ijid = jointNoToUuid[jno];
    //        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(ijid));

    //        //JointData j
    //        jno = this->model.ibq[numbq][1] - 1;
    //        QUuid jjid = jointNoToUuid[jno];
    //        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jjid));

    //        QUuid uid = QUuid::createUuid();
    //        UnifiedFixedData::getInstance()->getAnalysisData()->createSpring(jdlist, type, springname, springCharacteristic, springNo,
    //                                                 uid );
    //        springNoToUuid.insert(springNo, uid);
    //    }
    //    //MSSばね
    //    for (int numby = 0; numby < this->model.numby; numby++) {

    //        QString springname = QString();

    //        int springCharacteristic = 0;
    //        int springNo = this->model.iby[numby][3];

    //        B3D_DATATYPE type = B3D_DATATYPE::B3D_TPSPRING_MSS;

    //        QList<JointData *> jdlist;
    //        int jno;
    //        //JointData i
    //        jno = this->model.iby[numby][0] - 1;
    //        QUuid ijid = jointNoToUuid[jno];
    //        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(ijid));

    //        //JointData j
    //        jno = this->model.iby[numby][1] - 1;
    //        QUuid jjid = jointNoToUuid[jno];
    //        jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(jjid));

    //        QUuid uid = QUuid::createUuid();
    //        UnifiedFixedData::getInstance()->getAnalysisData()->createSpring(jdlist, type, springname, springCharacteristic, springNo,
    //                                                 uid );
    //        springNoToUuid.insert(springNo, uid);
    //    }

    //    QStringList loadcase;
    //    loadcase << "" << this->getStressCase();
    //    UnifiedFixedData::getInstance()->getPstnData()->setStressCaseList(loadcase);

    //    if (!this->successToReadSomeRstFile || !this->successToReadLoadFile)return;

    //    //応力・変位の読み込み
    //    foreach (QString load, loadcase) {
    //        if (!load.isEmpty())UnifiedFixedData::getInstance()->getPstnData()->setMaxStep(load, this->rst[load].count());
    //        qDebug() << load << this->rst[load].count();
    //        if (load.isEmpty())continue;

    //        for (int jno = 0; jno < this->model.numj; jno++) {
    //            QUuid uid = jointNoToUuid[jno];
    //            QList<XYZ> xyzlist;//list of step
    //            QList<QList<qreal> > reactlist;//list of step

    //            for (int step = 0; step < this->rst[load].count(); step++) {
    ////                    qDebug()<<"steps="<<rst[load].count();
    //                DataBLD3D_RST rst_of_step = this->rst[load][step];
    //                //変位
    //                qreal x = rst_of_step.ujnt[jno][0];
    //                qreal y = rst_of_step.ujnt[jno][1];
    //                qreal z = rst_of_step.ujnt[jno][2];
    //                xyzlist << XYZ(x, y, z);
    //                //反力
    //                QList<qreal> react;
    //                react << rst_of_step.react[jno][0]
    //                      << rst_of_step.react[jno][1]
    //                      << rst_of_step.react[jno][2]
    //                      << rst_of_step.react[jno][3]
    //                      << rst_of_step.react[jno][4]
    //                      << rst_of_step.react[jno][5];
    //                reactlist << react;
    //            }
    ////                qDebug()<<"setjointdisp?";
    //            UnifiedFixedData::getInstance()->getPstnData()->setJointDisp(uid, load, xyzlist);
    ////                qDebug()<<"setjointdisp!";
    //            UnifiedFixedData::getInstance()->getPstnData()->setReaction(uid, load, reactlist); //reactlist[load][step][dof]
    ////                qDebug()<<"setreaction";
    //        }
    //        qDebug() << "jload";

    //        for (int numm = 0; numm < this->model.numm; numm++) {
    //            QUuid uid = memberNoToUuid[numm];
    //            QList<MemberForce> facesectionalforcelist;//list of step
    //            QList<MemberForce> nodesectionalforcelist;
    //            QList<QList<MRESULT::YIELD_STATE> > yieldstatelist;

    //            for (int step = 0; step < this->rst[load].count(); step++) {
    //                DataBLD3D_RST rst_of_step = this->rst[load][step];
    //                MemberForce facesectionalforce;
    //                MemberForce nodesectionalforce;
    //                facesectionalforce.iMy = rst_of_step.rstmm[numm][0];
    //                facesectionalforce.iMz = rst_of_step.rstmm[numm][1];
    //                facesectionalforce.jMy = rst_of_step.rstmm[numm][2];
    //                facesectionalforce.jMz = rst_of_step.rstmm[numm][3];
    //                facesectionalforce.iQy = rst_of_step.rstmm[numm][4];
    //                facesectionalforce.iQz = rst_of_step.rstmm[numm][5];
    //                facesectionalforce.jQy = rst_of_step.rstmm[numm][4];
    //                facesectionalforce.jQz = rst_of_step.rstmm[numm][5];
    //                facesectionalforce.N = rst_of_step.rstmm[numm][6];
    //                facesectionalforce.iMx = rst_of_step.rstmm[numm][7];
    //                facesectionalforce.jMx = rst_of_step.rstmm[numm][7];
    //                nodesectionalforce = facesectionalforce;
    //                nodesectionalforce.iMy -= nodesectionalforce.iQz *
    //                                          this->model.dmm1[numm][6]; //i端のフェイス長さ＊せん断力
    //                nodesectionalforce.iMz += nodesectionalforce.iQy *
    //                                          this->model.dmm1[numm][6]; //i端のフェイス長さ＊せん断力
    //                nodesectionalforce.jMy += nodesectionalforce.jQz *
    //                                          this->model.dmm1[numm][7]; //j端のフェイス長さ＊せん断力
    //                nodesectionalforce.jMz -= nodesectionalforce.jQy *
    //                                          this->model.dmm1[numm][7]; //j端のフェイス長さ＊せん断力
    //                qreal in = 0.0;
    //                qreal jn = 0.0;
    //                qreal iqy = 0.0;
    //                qreal iqz = 0.0;
    //                qreal jqy = 0.0;
    //                qreal jqz = 0.0;
    //                qreal imx = 0.0;
    //                qreal jmx = 0.0;
    //                qreal imy = 0.0;
    //                qreal imz = 0.0;
    //                qreal jmy = 0.0;
    //                qreal jmz = 0.0;
    //                qreal cmy = 0.0;
    //                qreal cmz = 0.0;
    ////                qDebug() << "kcasol"; //足してる荷重の数
    //                for (int cl = 0; cl < rst_of_step.kcasol; cl++) {
    //                    int icase = rst_of_step.icase[cl]; //cl番目の荷重ケースの番号
    //                    qreal ccase = rst_of_step.ccase[cl]; //係数
    //                    //                qreal ccase=1.0;
    //                    int memberloadcase = this->load.indle[icase - 1]; //部材荷重の番号
    //                    if (memberloadcase == 0)continue;
    //                    in = in - ccase * this->load.cmqmm[memberloadcase - 1][numm][0];
    //                    iqy = iqy - ccase * this->load.cmqmm[memberloadcase - 1][numm][1];
    //                    iqz = iqz - ccase * this->load.cmqmm[memberloadcase - 1][numm][2];
    //                    imx = imx - ccase * this->load.cmqmm[memberloadcase - 1][numm][3];
    //                    imy = imy - ccase * this->load.cmqmm[memberloadcase - 1][numm][4];
    //                    imz = imz - ccase * this->load.cmqmm[memberloadcase - 1][numm][5];
    //                    jn = jn + ccase * this->load.cmqmm[memberloadcase - 1][numm][6];
    //                    jqy = jqy + ccase * this->load.cmqmm[memberloadcase - 1][numm][7];
    //                    jqz = jqz + ccase * this->load.cmqmm[memberloadcase - 1][numm][8];
    //                    jmx = jmx + ccase * this->load.cmqmm[memberloadcase - 1][numm][9];
    //                    jmy = jmy + ccase * this->load.cmqmm[memberloadcase - 1][numm][10];
    //                    jmz = jmz + ccase * this->load.cmqmm[memberloadcase - 1][numm][11];
    //                    cmy = cmy + ccase * this->load.cmqmm[memberloadcase - 1][numm][12];
    //                    cmz = cmz + ccase * this->load.cmqmm[memberloadcase - 1][numm][13];
    //                }
    //                facesectionalforce.N += (in + jn) * 0.5;
    //                facesectionalforce.iQy += iqy;
    //                facesectionalforce.iQz += iqz;
    //                facesectionalforce.jQy += jqy;
    //                facesectionalforce.jQz += jqz;
    //                facesectionalforce.iMx += imx;
    //                facesectionalforce.jMx += jmx;

    //                facesectionalforce.iMy += imy;
    //                facesectionalforce.iMz += imz;
    //                facesectionalforce.jMy += jmy;
    //                facesectionalforce.jMz += jmz;

    //                nodesectionalforce.N += (in + jn) * 0.5;
    //                nodesectionalforce.iQy += iqy;
    //                nodesectionalforce.iQz += iqz;
    //                nodesectionalforce.jQy += jqy;
    //                nodesectionalforce.jQz += jqz;
    //                nodesectionalforce.iMx += imx;
    //                nodesectionalforce.jMx += jmx;

    //                nodesectionalforce.iMy += imy;
    //                nodesectionalforce.iMz += imz;
    //                nodesectionalforce.jMy += jmy;
    //                nodesectionalforce.jMz += jmz;
    //                facesectionalforce.cMy = (facesectionalforce.iMy + facesectionalforce.jMy) / 2.0 + cmy;
    //                facesectionalforce.cMz = (facesectionalforce.iMz + facesectionalforce.jMz) / 2.0 + cmz;
    //                nodesectionalforce.cMy = (nodesectionalforce.iMy + nodesectionalforce.jMy) / 2.0 + cmy;
    //                nodesectionalforce.cMz = (nodesectionalforce.iMz + nodesectionalforce.jMz) / 2.0 + cmz;

    //                facesectionalforcelist << facesectionalforce;
    //                nodesectionalforcelist << nodesectionalforce;

    //                QList<MRESULT::YIELD_STATE> yieldstate;
    //                int istate = rst_of_step.ipspmm[numm][0]; //i
    //                int jstate = rst_of_step.ipspmm[numm][1]; //j
    //                int cstate = rst_of_step.ipspmm[numm][2]; //c
    //                if (UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid)->dataType() == DATATYPE::TPBRACE ) {

    //                    if (istate == 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::NOT;
    //                    } else if (istate > 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::YIELD;
    //                    } else if (istate < 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::HINGE;
    //                    }
    //                    if (jstate == 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::NOT;
    //                    } else if (jstate > 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::YIELD;
    //                    } else if (jstate < 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::HINGE;
    //                    }
    //                    if (cstate == 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::NOT;
    //                    } else if (cstate > 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::AXIAL_YIELD;
    //                    } else if (cstate < 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::AXIAL_YIELD;
    //                    }

    //                } else {
    //                    if (istate == 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::NOT;
    //                    } else if (istate > 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::YIELD;
    //                    } else if (istate < 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::HINGE;
    //                    }
    //                    if (jstate == 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::NOT;
    //                    } else if (jstate > 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::YIELD;
    //                    } else if (jstate < 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::HINGE;
    //                    }
    //                    if (cstate == 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::NOT;
    //                    } else if (cstate > 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::SHEAR_CRACK;
    //                    } else if (cstate < 0) {
    //                        yieldstate << MRESULT::YIELD_STATE::SHEAR_YIELD;
    //                    }
    //                }
    //                yieldstatelist << yieldstate;

    //            }
    //            UnifiedFixedData::getInstance()->getPstnData()->setFaceMemberForce(uid, load, facesectionalforcelist);
    //            UnifiedFixedData::getInstance()->getPstnData()->setNodeMemberForce(uid, load, nodesectionalforcelist);
    //            UnifiedFixedData::getInstance()->getPstnData()->setYieldState(uid, load, yieldstatelist);
    //        }
    //        qDebug() << "mload";

    //        for (int numb = 0; numb < this->model.numb; numb++) {

    //            QUuid uid = springNoToUuid[numb];
    //            QVector<qreal> springValueList;
    //            for (int step = 0; step < this->rst[load].count(); step++) {
    //                qreal value;
    //                value = this->rst[load][step].rstbb[numb];

    //                springValueList << value;

    //            }
    //            UnifiedFixedData::getInstance()->getPstnData()->setSpringValue(uid, load, springValueList);
    //        }

    //        for (int numww = 0; numww < this->model.numww; numww++) {
    //            QUuid uid = wallNoToUuid[numww];
    //            QList<MemberForce> sectionalforcelist;
    //            QList<QList<MRESULT::YIELD_STATE> > yieldstatelist;

    //            for (int step = 0; step < this->rst[load].count(); step++) {
    //                DataBLD3D_RST rst_of_step = this->rst[load][step];

    //                MemberForce sectionalforce;
    //                sectionalforce.iMz = rst_of_step.rstww[numww][2][0]; //2=壁柱
    //                sectionalforce.jMz = rst_of_step.rstww[numww][2][1];
    //                sectionalforce.iQy = rst_of_step.rstww[numww][2][2];
    //                sectionalforce.jQy = rst_of_step.rstww[numww][2][2];
    //                sectionalforce.N = rst_of_step.rstww[numww][2][3];
    //                sectionalforcelist << sectionalforce;

    //                //降伏状態
    //                QList<MRESULT::YIELD_STATE> yieldstate;
    //                int istate = rst_of_step.ipspww[numww][0]; //i
    //                //                int jstate=rst_of_step.ipspww[numww][1];//j
    //                int cstate = rst_of_step.ipspww[numww][2]; //c
    //                if (istate == 0) {
    //                    yieldstate << MRESULT::YIELD_STATE::NOT;
    //                } else if (istate > 0) {
    //                    yieldstate << MRESULT::YIELD_STATE::YIELD;
    //                } else if (istate < 0) {
    //                    yieldstate << MRESULT::YIELD_STATE::HINGE;
    //                }
    //                if (true) {
    //                    yieldstate << MRESULT::YIELD_STATE::NOT;
    //                }
    //                if (cstate == 0) {
    //                    yieldstate << MRESULT::YIELD_STATE::NOT;
    //                } else if (cstate > 0) {
    //                    yieldstate << MRESULT::YIELD_STATE::SHEAR_CRACK;
    //                } else if (cstate < 0) {
    //                    yieldstate << MRESULT::YIELD_STATE::SHEAR_YIELD;
    //                }

    //                qDebug() << "load" << load << "numww" << numww;
    //                Q_FOREACH(auto state, yieldstate){
    //                    qDebug() << static_cast<int>(state);
    //                }
    //                yieldstatelist << yieldstate;

    //            }
    //            UnifiedFixedData::getInstance()->getPstnData()->setFaceMemberForce(uid, load, sectionalforcelist);
    //            UnifiedFixedData::getInstance()->getPstnData()->setNodeMemberForce(uid, load, sectionalforcelist);
    //            UnifiedFixedData::getInstance()->getPstnData()->setYieldState(uid, load, yieldstatelist);

    //        }//step loop
    //    }//loadcase
    //    qDebug() << "load done";

    //    qDebug() <<
    //             "return で止まるのでデストラクタでdeleteできてない/しすぎの恐れ";

    //    return;//////////////////////////応力まで

    //    //mode
    //    if (existModeFile) {
    //        QHash<QUuid, QVector<XYZ>> modehash;
    //        for (int ii = 0; ii < mode.mode.count(); ii++) {
    //            modehash[jointNoToUuid[ii + 1]] = mode.mode.at(ii);
    //        }
    //        UnifiedFixedData::getInstance()->getPstnData()->setModeData(modehash, mode.beta, mode.naturalPeriod, mode.totalMass);
    //    }

    ////        QHash<QUuid,QVector<XYZ>> modetable;
    ////        for(int jno=0;jno<this->model.numj;jno++){
    ////            QUuid uid=jointNoToUuid[jno];
    ////            QVector<XYZ> xyzlist_nmode;
    ////            for(int nmode=0;nmode<this->mode.nummode;nmode++){
    ////                XYZ xyz=this->mode.mode.at(nmode).at(jno);//XYZ*[nummode][numj]
    ////                xyzlist_nmode<<xyz;
    ////            }
    ////            modetable[uid]=xyzlist_nmode;
    ////        }
    ////        UnifiedOutput::getInstance()->setModeData(modetable,this->mode.beta,this->mode.naturalPeriod,this->mode.totalMass);
    //    qDebug() << "mode done";

    //    if (successToReadChckFile) {

    //        //chck
    //        QHash<QUuid, QVector<QVector<float> > > verificationS;
    //        QHash<QUuid, QVector<QVector<float> > > verificationS2;
    //        QHash<QUuid, QVector<QVector<float> > > verificationB;
    //        QHash<QUuid, QVector<QVector<float> > > allowanceB;

    //        UnifiedFixedData::getInstance()->getPstnData()->setNameCh(this->chck.namech);
    //        for (int numm = 0; numm < this->model.numm; numm++) {
    //            QUuid uid = memberNoToUuid[numm];
    //            QVector<QVector<float> > allowanceB_numch_icj;
    //            QVector<QVector<float> > verificationB_numch_icj;
    //            QVector<QVector<float> > verificationS_numch_icj;
    //            QVector<QVector<float> > verificationS2_numch_icj;
    //            for (int numch = 0; numch < this->chck.numch; numch++) {
    //                QVector<float> allowanceB_icj = this->chck.line_allowanceRateBending.at(numch).at(numm);
    //                QVector<float> verificationB_icj = this->chck.line_verificationRatioBending.at(numch).at(numm);
    //                QVector<float> verificationS_icj = this->chck.line_verificationRatioShear.at(numch).at(numm);
    //                QVector<float> verificationS2_icj = this->chck.line_verificationRatioShear2.at(numch).at(numm);
    //                allowanceB_numch_icj << allowanceB_icj;
    //                verificationB_numch_icj << verificationB_icj;
    //                verificationS_numch_icj << verificationS_icj;
    //                verificationS2_numch_icj << verificationS2_icj;
    //            }
    //            verificationB[uid] = verificationB_numch_icj;
    //            verificationS[uid] = verificationS_numch_icj;
    //            verificationS2[uid] = verificationS2_numch_icj;
    //            allowanceB[uid] = allowanceB_numch_icj;
    //        }
    //        for (int numww = 0; numww < this->model.numww; numww++) {
    //            QUuid uid = wallNoToUuid[numww];
    //            QVector<QVector<float> > allowanceB_numch_icj;
    //            QVector<QVector<float> > verificationB_numch_icj;
    //            QVector<QVector<float> > verificationS_numch_icj;
    //            QVector<QVector<float> > verificationS2_numch_icj;
    //            for (int numch = 0; numch < this->chck.numch; numch++) {
    //                QVector<float> allowanceB_icj = this->chck.plane_allowanceRateBending.at(numch).at(numww);
    //                QVector<float> verificationB_icj = this->chck.plane_verificationRatioBending.at(numch).at(numww);
    //                QVector<float> verificationS_icj = this->chck.plane_verificationRatioShear.at(numch).at(numww);
    //                QVector<float> verificationS2_icj = this->chck.plane_verificationRatioShear2.at(numch).at(numww);
    //                allowanceB_numch_icj << allowanceB_icj;
    //                verificationB_numch_icj << verificationB_icj;
    //                verificationS_numch_icj << verificationS_icj;
    //                verificationS2_numch_icj << verificationS2_icj;
    //            }
    //            verificationB[uid] = verificationB_numch_icj;
    //            verificationS[uid] = verificationS_numch_icj;
    //            verificationS2[uid] = verificationS2_numch_icj;
    //            allowanceB[uid] = allowanceB_numch_icj;
    //        }
    //        UnifiedFixedData::getInstance()->getPstnData()->setVerificationRatioBending(verificationB);
    //        UnifiedFixedData::getInstance()->getPstnData()->setVerificationRatioShear(verificationS);
    //        UnifiedFixedData::getInstance()->getPstnData()->setVerificationRatioShear2(verificationS2);
    //        UnifiedFixedData::getInstance()->getPstnData()->setAllowanceRateBending(allowanceB);
    //    }
    //}
} // namespace post3dapp

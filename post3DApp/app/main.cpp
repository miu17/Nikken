#include <QApplication>
#include <QProgressDialog>
#include <QSplashScreen>
#include <QTextCodec>
#include <QTimer>
#include <QTranslator>
//Miutest
#include <QSettings>
#include <QTemporaryFile>

#include "main_widget.h"
#include "unified_data.h"
#include "unified_output.h"



int main(int argc, char *argv[])
{
    qSetMessagePattern("%{file}:%{line}:%{function} => %{message}");
    QApplication app(argc, argv);
    QTranslator myappTranslator;
    myappTranslator.load("post3D_ja", ":/translate");
    app.installTranslator(&myappTranslator);

    QTranslator mydllTranslator;
    mydllTranslator.load("post3DLib_ja", ":/post3DLib/translate");
    app.installTranslator(&mydllTranslator);

    QSplashScreen splash(QPixmap(":/icons/splash.png")
                         .scaled(QSize(1000, 700), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    splash.show();
    splash.showMessage(u8"メイン画面のセットアップ中 ...", Qt::AlignRight | Qt::AlignTop, Qt::white);
    app.processEvents();

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication::setStyle("plastique");

    QFont::insertSubstitution(QObject::tr("Microsoft JhengHei"),
                              QObject::tr("ＭＳ ゴシック"));
    QFont::insertSubstitution(QObject::tr("Meiryo UI"),
                              QObject::tr("ＭＳ ゴシック"));

    post3dapp::MainWidget main;
    splash.finish(&main);
    main.showMaximized();

    return app.exec();
}

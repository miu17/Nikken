#include <QtGui>
#include <QComboBox>
#include <QLabel>
#include <QMdiArea>
#include <QToolButton>
#include <QVBoxLayout>

#include "central_widget.h"
#include "mdisubwindow.h"
#include "message_dialog.h"
#include "persUI_scene.h"
#include "property_dialog.h"
#include "unified_editingdata.h"
#include "unified_settings.h"
#include "utility.h"
#include "vhline.h"


namespace post3dapp{

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    myMDIArea = new QMdiArea(this);
    myMDIArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    myMDIArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(myMDIArea, &QMdiArea::subWindowActivated,
            this, &CentralWidget::sendActiveStatusToSubWindow);

    createWindowMakeBar();
    createSettingDialogs();

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->addWidget(windowMakeBar);
    vlay1->addWidget(myMDIArea);
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->setSpacing(0);
    this->setLayout(vlay1);
}

QPoint CentralWidget::positionAtRightTop() const
{
    QPoint pp = myMDIArea->rect().topRight();
    return myMDIArea->mapToGlobal(pp);
}

void CentralWidget::createWindowMakeBar()
{
    windowMakeBar = new QWidget(this);
    windowMakeBar->setFixedHeight(33);

    inoutComboBox = new QComboBox(this);
    inoutComboBox->addItems(QStringList{u8"形状入力", u8"形状(解析時)", u8"荷重拾い結果", u8"解析結果"});
    inoutComboBox->setFixedWidth(120);
    inoutComboBox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    typeComboBox = new QComboBox(this);
    slotChangeTypeComboBox();
    typeComboBox->setFixedWidth(120);
    typeComboBox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );

    connect( inoutComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
             this, &CentralWidget::slotChangeTypeComboBox);

    /*
        解析結果の種類、モデル、解析ケース等の選択はSubWindow内で
        QComboBox *output_combo=new QComboBox;
        QStringList output_str;
        output_str << tr("（解析結果）") << tr("応力図") << tr("変形図") << tr("許容応力図") << tr("4F");
        output_combo->addItems(output_str);
        output_combo->setFixedWidth(130);
        output_combo->setStyle(xp_style);
    */
    QToolButton *newwin_button = new QToolButton(this);
    newwin_button->setAutoRaise(true);
    newwin_button->setIcon(QIcon(QPixmap(":/icons/win_new.png")));
    newwin_button->setToolTip(u8"新しいウィンドウを開く");
    newwin_button->setIconSize(QSize(30, 30));
    newwin_button->setFixedSize(30, 30);

    QToolButton *tile_button = new QToolButton(this);
    tile_button->setAutoRaise(true);
    tile_button->setIcon(QIcon(QPixmap(":/icons/win_tile.png")));
    tile_button->setToolTip(u8"ウィンドウを横に並べて整列");
    tile_button->setIconSize(QSize(30, 30));
    tile_button->setFixedSize(30, 30);

    QToolButton *cascade_button = new QToolButton(this);
    cascade_button->setAutoRaise(true);
    cascade_button->setIcon(QIcon(QPixmap(":/icons/win_cascade.png")));
    cascade_button->setToolTip(u8"ウィンドウを前後に並べて整列");
    cascade_button->setIconSize(QSize(30, 30));
    cascade_button->setFixedSize(30, 30);

    QLabel *undoRedoLabel = new QLabel(u8"Undo/Redo：", this);

    undoButton = new QToolButton(this);
    undoButton->setAutoRaise(true);
    undoButton->setIcon(QIcon(QPixmap(":/icons/undo.png")));
    undoButton->setToolTip(u8"取り消し（Undo）");
    undoButton->setIconSize(QSize(30, 30));
    undoButton->setFixedSize(30, 30);
    undoButton->setEnabled(false);

    connect(undoButton, &QAbstractButton::clicked,
            UnifiedEditingData::getInstance(), &UnifiedEditingData::executeUndoCommand);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::canUndoChanged,
            this, &CentralWidget::slotUndoButtonEnable);

    redoButton = new QToolButton(this);
    redoButton->setAutoRaise(true);
    redoButton->setIcon(QIcon(QPixmap(":/icons/redo.png")));
    redoButton->setToolTip(u8"やり直し（Redo）");
    redoButton->setIconSize(QSize(30, 30));
    redoButton->setFixedSize(30, 30);
    redoButton->setEnabled(false);

    connect(redoButton, &QAbstractButton::clicked,
            UnifiedEditingData::getInstance(), &UnifiedEditingData::executeRedoCommand);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::canRedoChanged,
            this, &CentralWidget::slotRedoButtonEnable);
    /*
        viewSettingButton=new QToolButton(this);
        viewSettingButton->setAutoRaise(true);
        viewSettingButton->setIcon(QIcon(QPixmap(":/icons/color_settings.png")));
        viewSettingButton->setToolTip(tr("Display setting window"));
        viewSettingButton->setIconSize(QSize(30,30));
        viewSettingButton->setFixedSize(30,30);

        viewSettingButton->setCheckable(true);
        viewSettingButton->setChecked(false);
    */
    messageButton = new QToolButton(this);
    messageButton->setAutoRaise(true);
    messageButton->setIcon(QIcon(QPixmap(":/icons/message.png")));
    messageButton->setToolTip(u8"操作履歴ウィンドウ");
    messageButton->setIconSize(QSize(30, 30));
    messageButton->setFixedSize(30, 30);

    messageButton->setCheckable(true);
    messageButton->setChecked(false);

    propertyButton = new QToolButton(this);
    propertyButton->setAutoRaise(true);
    propertyButton->setIcon(QIcon(QPixmap(":/icons/property.png")));
    propertyButton->setToolTip(u8"プロパティウィンドウ");
    propertyButton->setIconSize(QSize(30, 30));
    propertyButton->setFixedSize(30, 30);

    propertyButton->setCheckable(true);
    propertyButton->setChecked(true);

    connect( newwin_button, &QAbstractButton::clicked, this, &CentralWidget::slotAddSubWindow );
    connect( tile_button, &QAbstractButton::clicked, myMDIArea, &QMdiArea::tileSubWindows );
    connect( cascade_button, &QAbstractButton::clicked, myMDIArea, &QMdiArea::cascadeSubWindows );

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setContentsMargins(2, 2, 2, 2);
    hlay->setSpacing(3);
    hlay->addWidget(inoutComboBox);
    hlay->addWidget(typeComboBox);
    hlay->addSpacing(5);
    hlay->addWidget(newwin_button);
    hlay->addWidget(new VFLine(Qt::lightGray, this));
    hlay->addWidget(tile_button);
    hlay->addWidget(cascade_button);
    hlay->addStretch();
    hlay->addWidget(undoRedoLabel);
    hlay->addWidget(undoButton);
    hlay->addWidget(redoButton);
    hlay->addSpacing(3);
    hlay->addWidget(new VFLine(Qt::lightGray, this));
//    hlay->addSpacing(3);
//    hlay->addWidget(viewSettingButton);
    hlay->addSpacing(3);
    hlay->addWidget(messageButton);
    hlay->addSpacing(3);
    hlay->addWidget(propertyButton);

    windowMakeBar->setLayout(hlay);
}

void CentralWidget::createSettingDialogs()
{
//    viewSettingDialog=new ViewSettingDialog(this);
//    viewSettingDialog->hide();
    messageDialog = new MessageDialog(this);
    messageDialog->hide();
    propertyDialog = new PropertyDialog(this);
    propertyDialog->hide();

//    connect(viewSettingButton,SIGNAL(toggled(bool)),this,SLOT(slotShowViewSettingDialog(bool)));
//    connect(viewSettingDialog,SIGNAL(windowClosed()),this,SLOT(slotViewSettingButtonUnChecked()));

    connect(messageButton, &QAbstractButton::toggled, this, &CentralWidget::slotShowMessageDialog);
    connect(messageDialog, &MessageDialog::windowClosed, this, &CentralWidget::slotMessageButtonUnChecked);

    connect(propertyButton, &QAbstractButton::toggled, this, &CentralWidget::slotShowPropertyDialog);
    connect(propertyDialog, &PropertyDialog::windowClosed, this, &CentralWidget::slotPropertyButtonUnChecked);

    QMetaObject::invokeMethod(this, "slotShowPropertyDialog",
                              Qt::QueuedConnection, Q_ARG(bool, true));
}

void CentralWidget::slotAddSubWindow()
{
    if ( !checkSubWindowType() ) return;

    SUBWINSTATUS subtype = currentSubWindowType();

    //cl,outのパースは同じ処理必要？
    if ( subtype.WinType == WINTYPE::WINPERS && subtype.MDIType == MDITYPE::MDIINPUT ) {
        if ( !existPersWindow() ) {
            qDebug() << "new Scene";
            auto etype = UnifiedEditingData::getInstance()->currentElement();
            ADDEDITMODE mode = UnifiedEditingData::getInstance()->currentAddEditMode();
            PersInterfaceScene *scene = new PersInterfaceScene(etype, mode, UnifiedSettings::getInstance()->getInput3DViewSettings(), this);
            UnifiedEditingData::getInstance()->setSingularPersScene(scene);
        }
    }

    MdiSubWindow *sw = new MdiSubWindow(subtype);
    connect( sw, &QObject::destroyed, this, &CentralWidget::slotWindowDestroy );
    myMDIArea->addSubWindow(sw);
    sw->show();
}

void CentralWidget::slotWindowDestroy()
{
    if ( !existPersWindow() ) {
        PersInterfaceScene *scene = UnifiedEditingData::getInstance()->getSingularPersScene();
        if ( scene != nullptr ) {
            qDebug() << "del scene";
            delete scene;
            scene = nullptr;
            UnifiedEditingData::getInstance()->setSingularPersScene(scene);
            qDebug() << "Scene=NULL";
        }
    }
}

void CentralWidget::slotChangeTypeComboBox()
{
    int c_idx = typeComboBox->currentIndex();
    typeComboBox->clear();
    QStringList figureTypes{u8"（表示種類）",
                            u8"伏図",
                            u8"軸組図",
                            u8"３Ｄパース",
                            u8"グラフ",
                            u8"テキスト表示"};
    if (inoutComboBox->currentIndex() == static_cast<int>(MDITYPE::MDIOUTPUT)){
        typeComboBox->addItems(figureTypes);
        typeComboBox->setCurrentIndex(c_idx);
    }else{
        typeComboBox->addItems(figureTypes.mid(0, 4));
        if (c_idx < 0 || typeComboBox->count() - 1 < c_idx){
            typeComboBox->setCurrentIndex(0);
        }else{
            typeComboBox->setCurrentIndex(c_idx);
        }
    }
}

bool CentralWidget::existPersWindow() const
{
    Q_FOREACH ( QMdiSubWindow *w, myMDIArea->subWindowList() ) {
        MdiSubWindow *sw = qobject_cast<MdiSubWindow *>(w);
        SUBWINSTATUS st = sw->subWindowStatus();
        if ( st.WinType == WINTYPE::WINPERS && st.MDIType == MDITYPE::MDIINPUT ) return true;
    }
    return false;
}

bool CentralWidget::checkSubWindowType()
{
    int id1 = typeComboBox->currentIndex();
    if ( id1 == 0 ) {
        Utility::showWarningMessage(this, u8"表示種類を指定してください。");
        return false;
    }
    return true;
}

SUBWINSTATUS CentralWidget::currentSubWindowType() const
{
    MDITYPE mdtype = static_cast<MDITYPE>(inoutComboBox->currentIndex());
    WINTYPE wintype = static_cast<WINTYPE>(typeComboBox->currentIndex());
    return  SUBWINSTATUS(mdtype, wintype);
}

void CentralWidget::sendActiveStatusToSubWindow(QMdiSubWindow *sub)
{
    if ( sub == nullptr ) {
        UnifiedEditingData::getInstance()->changeActiveWindowType(WINTYPE::NOWINTYPE);
        return;
    }
    qobject_cast<MdiSubWindow *>(sub)->sendActiveStatusToChildWidget();
}

void CentralWidget::slotShowPropertyDialog(bool onoff)
{
    if (!onoff) {
        propertyDialog->hide();
        return;
    }

    QPoint gp = positionAtRightTop();
    propertyDialog->move(gp.x() - propertyDialog->width() - 20, gp.y() + 10);
    propertyDialog->show();
}

void CentralWidget::slotPropertyButtonUnChecked()
{
    propertyButton->setChecked(false);
}
/*
void CentralWidget::slotShowViewSettingDialog(bool onoff)
{
    if(!onoff){
        viewSettingDialog->hide();
        return;
    }
    viewSettingDialog->show();
}

void CentralWidget::slotViewSettingButtonUnChecked()
{
    viewSettingButton->setChecked(false);
}
*/
void CentralWidget::slotShowMessageDialog(bool onoff)
{
    if (!onoff) {
        messageDialog->hide();
        return;
    }
    messageDialog->show();
}

void CentralWidget::slotMessageButtonUnChecked()
{
    messageButton->setChecked(false);
}

void CentralWidget::slotUndoButtonEnable(bool on)
{
    undoButton->setEnabled(on);
}

void CentralWidget::slotRedoButtonEnable(bool on)
{
    redoButton->setEnabled(on);
}

} // namespace post3dapp

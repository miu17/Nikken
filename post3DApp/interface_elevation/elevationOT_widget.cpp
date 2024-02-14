#include "elevationOT_widget.h"

#include <QtGui>
#include <QLabel>

#include "custom_print_dialog.h"
#include "elevationOT_grscene.h"
#include "elevationOT_grview.h"
#include "fixed_data.h"
#include "output_control.h"
#include "output_grscene.h"
#include "reference_combobox.h"
#include "unified_analysisdata.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "unified_output.h"
#include "unified_project.h"
#include "unified_settings.h"


namespace post3dapp
{
    ElevationOutputWidget::ElevationOutputWidget(QWidget *parent)
        : CustomDrawWidget(parent)
    {
        qDebug()<<"ElevationOutputWidget::in";
        createSceneView();
        createButtons();
        createLayout();

        connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::dataUpdated,
                this, &ElevationOutputWidget::updateElevationView);
        connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::dataCleared,
                this, &ElevationOutputWidget::initializePlanView);

        //    connect( UnifiedSettings::getInstance(), &UnifiedSettings::out2D_SettingChanged,
        //             this, &ElevationOutputWidget::resetSceneView );

        connect(UnifiedProject::getInstance(), &UnifiedProject::stressCombi_SettingChanged,
                this, &ElevationOutputWidget::resetSceneView);
        qDebug()<<"ElevationOutputWidget::out";
    }

CustomDrawGrView *ElevationOutputWidget::getView() const
{
    qDebug()<<"ElevationOutputWidget::getView in";
    return elevationGrView;
    qDebug()<<"ElevationOutputWidget::getView out";
}


    void ElevationOutputWidget::createSceneView()
    {
        qDebug()<<"ElevationOutputWidget::createSceneView in";
        elevationGrScene = new ElevationOutputGrScene(UnifiedSettings::getInstance()->getOutput2DViewSettings(), this);
        connect(elevationGrScene, &OutputGrScene::resetSceneView, this, &ElevationOutputWidget::resetSceneView);
        elevationGrView = new ElevationOutputGrView(elevationGrScene, this);
        elevationGrView->setMinimumHeight(300);
        elevationGrView->setMinimumWidth(200);
        connect(elevationGrView, &CustomDrawGrView::scaleChanged,
                this, &CustomDrawWidget::slotChangeSliderValue);

        elevationGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
        qDebug()<<"ElevationOutputWidget::createSceneView out";
    }

    void ElevationOutputWidget::createButtons()
    {
        qDebug()<<"ElevationOutputWidget::createButtons in";
        CustomDrawWidget::createButtons();

        frameCombobox = new ReferenceCombobox(DATATYPE::TPFRAMEP, UnifiedDataType::Pstn, this);
        frameCombobox->setDefaultName(u8"（通りの選択）");
        frameCombobox->setFixedWidth(110);

        connect(frameCombobox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
                this, &ElevationOutputWidget::changeDrawingFrame);

        outTypeCombobox = new QComboBox(this);
        outTypeCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outTypeCombobox->setFixedWidth(200);

        outTypeCombobox->addItems(OutputControl::getInstance()->outputTypeStringList(OUTFIGCATEGORY::ELEVATION));

        connect(outTypeCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &ElevationOutputWidget::resetCaseCombobox);

        outCaseACombobox = new QComboBox(this);
        outCaseACombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outCaseACombobox->setFixedWidth(100);

        outCaseBCombobox = new QComboBox(this);
        outCaseBCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outCaseBCombobox->setFixedWidth(100);

        outCaseCCombobox = new QComboBox(this);
        outCaseCCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outCaseCCombobox->setFixedWidth(100);

        stepSpinBox = new QSpinBox(this);
        stepSpinBox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        stepSpinBox->setFixedWidth(50);
        maxStepLabel = new QLabel(tr("/0"), this);

        connect(outCaseACombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &ElevationOutputWidget::changeOutputCase);
        connect(outCaseBCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &ElevationOutputWidget::changeOutputCaseB);
        connect(outCaseCCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &ElevationOutputWidget::changeOutputCase);
        connect(stepSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &ElevationOutputWidget::changeOutputCase);

        resetCaseCombobox(0);

        //    controlButton=new QToolButton(this);
        //    controlButton->setAutoRaise(true);
        //    controlButton->setIcon(QIcon(QPixmap(":/icons/control.png")));
        //    controlButton->setToolTip(tr("Individual window display setting"));
        //    controlButton->setIconSize(QSize(30,30));
        //    controlButton->setFixedSize(30,30);

        printButton = new QToolButton(this);
        printButton->setAutoRaise(true);
        printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
        printButton->setFixedSize(30, 30);
        printButton->setIconSize(QSize(25, 25));
        connect(printButton, &QAbstractButton::clicked,
                this, &ElevationOutputWidget::slotClickedPrintButton);
        qDebug()<<"ElevationOutputWidget::createButtons out";
    }
    void ElevationOutputWidget::slotClickedPrintButton()
    {
        qDebug()<<"ElevationOutputWidget::slotClickedPrintButton in";
        CustomPrintDialog *dialog = new CustomPrintDialog(elevationGrScene);
        if (dialog->exec())
        {
        }
        qDebug()<<"ElevationOutputWidget::slotClickedPrintButton out";
        return;
    }
    void ElevationOutputWidget::createLayout()
    {
        qDebug()<<"ElevationOutputWidget::createLayout in";

        QHBoxLayout *hlay1 = new QHBoxLayout;
        hlay1->addWidget(frameCombobox, 0);
        hlay1->addSpacing(10);
        hlay1->addWidget(outTypeCombobox, 0);
        hlay1->addStretch();
        hlay1->addSpacing(10);
        hlay1->addWidget(zoomOutIcon, 0);
        hlay1->addWidget(zoomSlider, 0);
        hlay1->addWidget(zoomInIcon, 0);
        hlay1->addSpacing(5);
        hlay1->addWidget(fitButton, 0);

        qDebug() << "set hlay1";
        QHBoxLayout *hlay2 = new QHBoxLayout;
        hlay2->addWidget(outCaseACombobox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(outCaseBCombobox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(outCaseCCombobox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(stepSpinBox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(maxStepLabel, 0);
        hlay2->addStretch();
        hlay2->addSpacing(10);
        //    hlay2->addWidget(controlButton,0);
        //    hlay2->addSpacing(5);
        hlay2->addWidget(printButton, 0);
        qDebug() << "set hlay2";

        QVBoxLayout *vlay = new QVBoxLayout;
        vlay->setContentsMargins(0, 5, 0, 0);
        vlay->setSpacing(3);
        vlay->addLayout(hlay1, 0);
        vlay->addWidget(elevationGrView, 1);
        vlay->addLayout(hlay2, 0);

        setLayout(vlay);
        qDebug()<<"ElevationOutputWidget::createLayout out";
    }

    void ElevationOutputWidget::resetCaseCombobox(int idx)
    {
        qDebug()<<"ElevationOutputWidget::resetCaseCombobox in";

        outCaseACombobox->blockSignals(true);
        outCaseBCombobox->blockSignals(true);
        outCaseCCombobox->blockSignals(true);

        outCaseACombobox->clear();
        outCaseBCombobox->clear();
        outCaseCCombobox->clear();

        OutputControl *control = OutputControl::getInstance();
        listtype = control->listType(qMax(0, idx - 1), OUTFIGCATEGORY::ELEVATION);

        QStringList strlistA = control->getOutfigStringList(listtype, 1);
        if (strlistA.isEmpty())
            strlistA.append(u8"(条件A)");
        outCaseACombobox->addItems(strlistA);

        QStringList strlistB = control->getOutfigStringList(listtype, 2);
        if (strlistB.isEmpty())
            strlistB.append(u8"(条件B)");
        outCaseBCombobox->addItems(strlistB);

        QStringList strlistC = control->getOutfigStringList(listtype, 3);
        if (strlistC.isEmpty())
            strlistC.append(u8"(条件C)");
        outCaseCCombobox->addItems(strlistC);

        outCaseACombobox->blockSignals(false);
        outCaseBCombobox->blockSignals(false);
        outCaseCCombobox->blockSignals(false);
        maxStepLabel->setText(tr("/0"));
        stepSpinBox->setValue(0);

        if (listtype != LISTTYPE::COMBIMAP)
        {
            outCaseCCombobox->setVisible(false);
        }
        else
        {
            outCaseCCombobox->setVisible(true);
        }
        if (listtype != LISTTYPE::STRESS)
        {
            stepSpinBox->setVisible(false);
            maxStepLabel->setVisible(false);
        }
        else
        {
            stepSpinBox->setVisible(true);
            maxStepLabel->setVisible(true);
        }

        resetSceneView();
        qDebug()<<"ElevationOutputWidget::resetCaseCombobox out";
    }

    void ElevationOutputWidget::changeDrawingFrame(const QString &fr)
    {
        qDebug()<<"ElevationOutputWidget::changeDrawingFrame in";
        emit drawingFrameChanged(fr);
        resetSceneView();
        qDebug()<<"ElevationOutputWidget::changeDrawingFrame out";
    }

    void ElevationOutputWidget::changeOutputCaseB()
    {
        qDebug()<<"ElevationOutputWidget::changeOutputCaseB in";
        //stressじゃない場合はchangeOutputCase()
        //図面ごとにcomboのONOFF切り替える。
        if(listtype != LISTTYPE::STRESS){
            changeOutputCase();
            return;
        }

        stepSpinBox->blockSignals(true);
        // Bの値が変わったら最大ステップ数を変える
        STRESSCOMBISETTINGS scset = UnifiedProject::getInstance()->getStressCombinationSetting();

        // 荷重組み合わせの一覧
        QStringList clist = scset.combi; // CaseDL,CaseE1,....
        if (clist.count() == 0)
        {
            qDebug() << "no caseList";
            stepSpinBox->blockSignals(false);
            return;
        }

        // コンボボックスのインデックス（0は空欄）
        int indexOfCombi = clist.indexOf(outCaseBCombobox->currentText()); // CaseDLのとき、indexOfCombi=0
        if (indexOfCombi < 0)
        {
            qDebug() << "not match caseName";
            stepSpinBox->blockSignals(false);
            return;
        }

        int indexOfStep = scset.step.at(indexOfCombi) - 1; // ステップを刻む応力が定義されていないとき、indexOfStep=-1
        QStringList slist = scset.stress.value(outCaseBCombobox->currentText());
        if (indexOfStep >= slist.count())
        {
            qDebug() << "invalid stepcaseindex number";
            stepSpinBox->blockSignals(false);
            return;
        }
        if (indexOfStep == -1)
        {
            maxStepLabel->setText(tr(u8""));
            stepSpinBox->setVisible(false);
            stepSpinBox->blockSignals(false);
            changeOutputCase();
            return;
        }
        stepSpinBox->setVisible(true);
        QString stepStressName = slist.at(indexOfStep);

        int maxstep = UnifiedFixedData::getInstance()->getPstnData()->getMaxStep(stepStressName);
        maxStepLabel->setText(tr(u8"/") + QString::number(maxstep));
        stepSpinBox->setMaximum(maxstep);
        stepSpinBox->setValue(maxstep);
        stepSpinBox->blockSignals(false);
        changeOutputCase();
        qDebug()<<"ElevationOutputWidget::changeOutputCaseB out";

    }
    void ElevationOutputWidget::changeOutputCase()
    {
        qDebug()<<"ElevationOutputWidget::changeOutputCase in";

        QVector<int> idx = QVector<int>() << outCaseACombobox->currentIndex() << outCaseBCombobox->currentIndex() << outCaseCCombobox->currentIndex();
        auto figureTypeProperty = OutputControl::getInstance()->makeOutputFigureTypeProperty(SCENETYPE::ELEVATIONSCENE,elevationGrScene->getFrameFloorId(), elevationGrScene->getFigureTypeProperty().type,idx,stepSpinBox->value());
        elevationGrScene->setFigureTypeProperty(elevationGrScene->getFrameFloorId(), figureTypeProperty);
        qDebug()<<"ElevationOutputWidget::changeOutputCase out";
    }

    void ElevationOutputWidget::updateElevationView(DATATYPE type)
    {
        qDebug()<<"ElevationOutputWidget::updateElevationView() in";
        if (type != DATATYPE::TPFRAMEG && type != DATATYPE::TPFRAMEP)
            return;
        resetSceneView();
        qDebug()<<"ElevationOutputWidget::updateElevationView() out";
    }

    void ElevationOutputWidget::resetSceneView()
    {
        qDebug()<<"ElevationOutputWidget::resetSceneView() in";
        OUTFIGTYPE otype;
        if (outTypeCombobox->currentIndex() <= 0)
        {
            otype = OUTFIGTYPE::UNDEFINED;
        }
        else
        {
            otype = OutputControl::getInstance()->outputTypeList(OUTFIGCATEGORY::ELEVATION).at(outTypeCombobox->currentIndex() - 1);
        }
        QVector<int> idx = QVector<int>() << outCaseACombobox->currentIndex() << outCaseBCombobox->currentIndex() << outCaseCCombobox->currentIndex();

        auto figureTypeProperty = OutputControl::getInstance()->makeOutputFigureTypeProperty(SCENETYPE::ELEVATIONSCENE,frameCombobox->currentUuid(), otype, idx, stepSpinBox->value());
        elevationGrScene->setFigureTypeProperty(frameCombobox->currentUuid(), figureTypeProperty);
        qDebug()<<"ElevationOutputWidget::resetSceneView() out";
    }

    void ElevationOutputWidget::initializePlanView()
    {
        qDebug()<<"ElevationOutputWidget::initializePlanView in";

        frameCombobox->setCurrentIndex(0);
        qDebug()<<"ElevationOutputWidget::initializePlanView out";
    }

} // namespace post3dapp

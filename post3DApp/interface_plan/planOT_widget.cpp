#include "planOT_widget.h"

#include <QtGui>

#include "custom_print_dialog.h"
#include "fixed_data.h"
#include "output_control.h"
#include "planOT_grscene.h"
#include "planOT_grview.h"
#include "reference_combobox.h"
#include "unified_analysisdata.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "unified_output.h"
#include "unified_project.h"
#include "unified_settings.h"

namespace post3dapp
{

    PlanOutputWidget::PlanOutputWidget(QWidget *parent)
        : CustomDrawWidget(parent)
    {
        createSceneView();
        createButtons();
        createLayout();

        connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::dataUpdated,
                this, &PlanOutputWidget::updatePlanView);
        connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::dataCleared,
                this, &PlanOutputWidget::initializePlanView);
        connect(UnifiedProject::getInstance(), &UnifiedProject::stressCombi_SettingChanged,
                this, &PlanOutputWidget::resetSceneView);

        //    connect( UnifiedSettings::getInstance(), &UnifiedSettings::out2D_SettingChanged,
        //             this, &PlanOutputWidget::resetSceneView );
    }

CustomDrawGrView *PlanOutputWidget::getView() const
{
    return planGrView;
}


    void PlanOutputWidget::createSceneView()
    {
        planGrScene = new PlanOutputGrScene(UnifiedSettings::getInstance()->getOutput2DViewSettings(), this);
        connect(planGrScene, &OutputGrScene::resetSceneView, this, &PlanOutputWidget::resetSceneView);
        planGrView = new PlanOutputGrView(planGrScene, this);
        planGrView->setMinimumHeight(300);
        planGrView->setMinimumWidth(200);
        connect(planGrView, &CustomDrawGrView::scaleChanged,
                this, &CustomDrawWidget::slotChangeSliderValue);

        planGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
    }

    void PlanOutputWidget::createButtons()
    {
        CustomDrawWidget::createButtons();

        floorCombobox = new ReferenceCombobox(DATATYPE::TPAFLOOR, UnifiedDataType::Pstn, this);
        floorCombobox->setDefaultName(u8"（階の選択）");
        floorCombobox->setFixedWidth(110);

        connect(floorCombobox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
                this, &PlanOutputWidget::changeDrawingFloor);

        outTypeCombobox = new QComboBox(this);
        outTypeCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outTypeCombobox->setFixedWidth(200);

        outTypeCombobox->addItems(OutputControl::getInstance()->outputTypeStringList(OUTFIGCATEGORY::PLAN));

        connect(outTypeCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &PlanOutputWidget::resetCaseCombobox);

        outCaseACombobox = new QComboBox(this);
        outCaseACombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outCaseACombobox->setFixedWidth(100);

        outCaseBCombobox = new QComboBox(this);
        outCaseBCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outCaseBCombobox->setFixedWidth(100);

        outCaseCCombobox = new QComboBox(this);
        outCaseCCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outCaseCCombobox->setFixedWidth(100);

        outCaseDCombobox = new QComboBox(this);
        outCaseDCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outCaseDCombobox->setFixedWidth(100);

        outCaseECombobox = new QComboBox(this);
        outCaseECombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        outCaseECombobox->setFixedWidth(100);

        stepSpinBox = new QSpinBox(this);
        stepSpinBox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
        stepSpinBox->setFixedWidth(50);
        maxStepLabel = new QLabel(tr("/0"), this);

        connect(outCaseACombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &PlanOutputWidget::changeOutputCase);
        connect(outCaseBCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &PlanOutputWidget::changeOutputCaseB);
        connect(outCaseCCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &PlanOutputWidget::changeOutputCase);
        connect(outCaseDCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &PlanOutputWidget::changeOutputCase);
        connect(outCaseECombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &PlanOutputWidget::changeOutputCase);
        connect(stepSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &PlanOutputWidget::changeOutputCase);

        resetCaseCombobox(0);

        //    controlButton=new QToolButton(this);
        //    controlButton->setAutoRaise(true);
        //    controlButton->setIcon(QIcon(QPixmap(":/icons/control.png")));
        //    controlButton->setToolTip(tr("Individual window display setting"));
        //    controlButton->setIconSize(QSize(30,30));
        //    controlButton->setFixedSize(30,30);

        printButton = new QToolButton(this);
        printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
        printButton->setAutoRaise(true);
        printButton->setFixedSize(30, 30);
        printButton->setIconSize(QSize(25, 25));
        connect(printButton, &QAbstractButton::clicked,
                this, &PlanOutputWidget::slotClickedPrintButton);
    }
    void PlanOutputWidget::slotClickedPrintButton()
    {
        CustomPrintDialog *dialog = new CustomPrintDialog(planGrScene);
        if (dialog->exec())
        {
        }
        return;
    }

    void PlanOutputWidget::createLayout()
    {
        QHBoxLayout *hlay1 = new QHBoxLayout;
        hlay1->addWidget(floorCombobox, 0);
        hlay1->addSpacing(10);
        hlay1->addWidget(outTypeCombobox, 0);
        hlay1->addStretch();
        hlay1->addSpacing(10);
        hlay1->addWidget(zoomOutIcon, 0);
        hlay1->addWidget(zoomSlider, 0);
        hlay1->addWidget(zoomInIcon, 0);
        hlay1->addSpacing(5);
        hlay1->addWidget(fitButton, 0);

        QHBoxLayout *hlay2 = new QHBoxLayout;
        hlay2->addWidget(outCaseACombobox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(outCaseBCombobox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(outCaseCCombobox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(outCaseDCombobox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(outCaseECombobox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(stepSpinBox, 0);
        hlay2->addSpacing(5);
        hlay2->addWidget(maxStepLabel, 0);
        hlay2->addStretch();
        hlay2->addSpacing(10);
        //    hlay2->addWidget(controlButton,0);
        //    hlay2->addSpacing(5);
        hlay2->addWidget(printButton, 0);

        QVBoxLayout *vlay = new QVBoxLayout;
        vlay->setContentsMargins(0, 5, 0, 0);
        vlay->setSpacing(3);
        vlay->addLayout(hlay1, 0);
        vlay->addWidget(planGrView, 1);
        vlay->addLayout(hlay2, 0);

        setLayout(vlay);
    }

    void PlanOutputWidget::resetCaseCombobox(int idx)
    {
        outCaseACombobox->blockSignals(true);
        outCaseBCombobox->blockSignals(true);
        outCaseCCombobox->blockSignals(true);
        outCaseDCombobox->blockSignals(true);
        outCaseECombobox->blockSignals(true);

        outCaseACombobox->clear();
        outCaseBCombobox->clear();
        outCaseCCombobox->clear();
        outCaseDCombobox->clear();
        outCaseECombobox->clear();

        OutputControl *control = OutputControl::getInstance();
        LISTTYPE ltype = control->listType(qMax(0, idx - 1), OUTFIGCATEGORY::PLAN);
        qDebug() << "PlanOutputWidget:resetCaseCombobox:listtype" << (int)ltype;
        QStringList strlistA = control->getOutfigStringList(ltype, 1); // LISTTYPE::INOUT);
        QStringList strlistB = control->getOutfigStringList(ltype, 2);
        QStringList strlistC = control->getOutfigStringList(ltype, 3);
        QStringList strlistD = control->getOutfigStringList(ltype, 4);
        QStringList strlistE = control->getOutfigStringList(ltype, 5);

        //    QStringList strlistA = OutputControl::getInstance()->getCaseAStringList(idx,OUTFIGCATEGORY::OUTFIGCATEGORY_PLAN);
        if (strlistA.isEmpty())
            strlistA.append(u8"(条件A)");
        outCaseACombobox->addItems(strlistA);

        //    QStringList strlistB = OutputControl::getInstance()->getCaseBStringList(idx,OUTFIGCATEGORY::OUTFIGCATEGORY_PLAN);
        if (strlistB.isEmpty())
            strlistB.append(u8"(条件B)");
        outCaseBCombobox->addItems(strlistB);

        if (strlistC.isEmpty())
            strlistC.append(u8"(条件C)");
        outCaseCCombobox->addItems(strlistC);
        if (strlistD.isEmpty())
            strlistD.append(u8"(条件D)");
        outCaseDCombobox->addItems(strlistD);
        if (strlistE.isEmpty())
            strlistE.append(u8"(条件E)");
        outCaseECombobox->addItems(strlistE);

        outCaseACombobox->blockSignals(false);
        outCaseBCombobox->blockSignals(false);
        outCaseCCombobox->blockSignals(false);
        outCaseDCombobox->blockSignals(false);
        outCaseECombobox->blockSignals(false);
        maxStepLabel->setText(tr("/0"));
        stepSpinBox->setValue(0);
        if (ltype != LISTTYPE::COMBIMAP)
        {
            outCaseCCombobox->setVisible(false);
            outCaseDCombobox->setVisible(false);
            outCaseECombobox->setVisible(false);
        }
        else
        {
            outCaseCCombobox->setVisible(true);
            outCaseDCombobox->setVisible(true);
            outCaseECombobox->setVisible(true);
        }

        if (ltype != LISTTYPE::STRESS)
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
    }

    void PlanOutputWidget::changeDrawingFloor(const QString &fl)
    {
        emit drawingFloorChanged(fl);
        resetSceneView();
    }
    void PlanOutputWidget::changeOutputCaseB()
    {
        qDebug() << "changeoutputcaseB";

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

        // Dの値が変わったら最大ステップ数を変える
        int maxstep = UnifiedFixedData::getInstance()->getPstnData()->getMaxStep(stepStressName);
        maxStepLabel->setText(tr(u8"/") + QString::number(maxstep));

        stepSpinBox->setMaximum(maxstep);
        stepSpinBox->setValue(maxstep);
        stepSpinBox->blockSignals(false);
        changeOutputCase();
    }

    void PlanOutputWidget::changeOutputCase()
    {
        QVector<int> idx = QVector<int>() << outCaseACombobox->currentIndex() << outCaseBCombobox->currentIndex() << outCaseCCombobox->currentIndex()
                                          << outCaseDCombobox->currentIndex() << outCaseECombobox->currentIndex();
        auto figureProperty = OutputControl::getInstance()->makeOutputFigureTypeProperty(SCENETYPE::PLANSCENE,planGrScene->getFrameFloorId(),planGrScene->getFigureTypeProperty().type,idx,stepSpinBox->value());
        planGrScene->setFigureTypeProperty(planGrScene->getFrameFloorId(), figureProperty);
    }

    void PlanOutputWidget::updatePlanView(DATATYPE type)
    {
        /* 階の順番が変更された場合　→　floorComboBoxのUpdate　→　currentIndexの変更
           →　PlanInterfaceDrawWidget::changeDrawingFloor()
           → PlanInterfaceGrView::setBackGroundFrame() により自動的にアップデートされる */

        if (type != DATATYPE::TPFLOOR && type != DATATYPE::TPAFLOOR)
            return;
        //    if ( type != DATATYPE::TPFRAMEP ) return;
        resetSceneView();
    }

    void PlanOutputWidget::resetSceneView()
    {
        OUTFIGTYPE otype;
        if (outTypeCombobox->currentIndex() <= 0)
        {
            otype = OUTFIGTYPE::UNDEFINED;
        }
        else
        {
            otype = OutputControl::getInstance()->outputTypeList(OUTFIGCATEGORY::PLAN).at(outTypeCombobox->currentIndex() - 1);
        }
        QVector<int> idx = QVector<int>() << outCaseACombobox->currentIndex() << outCaseBCombobox->currentIndex() << outCaseCCombobox->currentIndex()
                                          << outCaseDCombobox->currentIndex() << outCaseECombobox->currentIndex();

        auto figureTypeProperty = OutputControl::getInstance()->makeOutputFigureTypeProperty(SCENETYPE::PLANSCENE,floorCombobox->currentUuid(),otype,idx,stepSpinBox->value());
        planGrScene->setFigureTypeProperty(floorCombobox->currentUuid(), figureTypeProperty);
    }

    void PlanOutputWidget::initializePlanView()
    {
        floorCombobox->setCurrentIndex(0);
    }
} // namespace post3dapp

#include "topmenu_widget.h"

#include <QtGui>
#include <QDebug>
#include <QTabWidget>

#include "additionalopen_dialog.h"
#include "analysis2dsetting_dialog.h"
#include "analysis3dsetting_dialog.h"
#include "analysiscondition_dialog.h"
#include "analysisconditionreader.h"
#include "analysisfloor_dialog.h"
#include "analysisgroup_dialog.h"
#include "app_version.h"
#include "autosave.h"
#include "calc_load.h"
#include "calculation_manager_dialog.h"
#include "calculationload2dsetting_dialog.h"
#include "calculationload3dsetting_dialog.h"
#include "define_material_struct.h"
#include "define_unifieddata.h"
#include "dividesave_dialog.h"
#include "executecontrol_dialog.h"
#include "fixed_tabledata.h"
#include "floorsetting_dialog.h"
#include "framesetting_dialog.h"
#include "handle_npstn.h"
#include "handle_pstn.h"
#include "input2dsetting_dialog.h"
#include "input3dsetting_dialog.h"
#include "jointloadsetting_dialog.h"
#include "manage_jointdata.h"
#include "material_dialog.h"
#include "memberloadsetting_dialog.h"
#include "menu_button.h"
#include "output_control.h"
#include "outputfigure_dialog.h"
#include "outputtext_dialog.h"
#include "read_from_bld3d.h"
#include "read_from_psv.h"
#include "section_base_dialog.h"
#include "section_beam_dialog.h"
#include "section_brace_dialog.h"
#include "section_col_dialog.h"
#include "section_damper_dialog.h"
#include "section_gir_dialog.h"
#include "section_isolator_dialog.h"
#include "section_opening_dialog.h"
#include "section_slab_dialog.h"
#include "section_uwall_dialog.h"
#include "section_wall_dialog.h"
#include "slabloadsetting_dialog.h"
#include "stress_combi_dialog.h"
#include "textinput_dialog.h"
#include "unified_analysisdata.h"
#include "unified_calcload.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "unified_output.h"
#include "unified_project.h"
#include "unified_settings.h"
#include "utility.h"
#include "vhline.h"

namespace post3dapp
{

    TopMenuWidget::TopMenuWidget(QWidget *parent)
        : QWidget(parent)
    {
        topMenuWidget = new QTabWidget(this);
        topMenuWidget->setFixedHeight(80);
        topMenuWidget->setTabShape(QTabWidget::Rounded);
        tabIcon.addPixmap(QPixmap(":/icons/play_yellow.png"), QIcon::Normal, QIcon::On);
        tabIcon.addPixmap(QPixmap(":/icons/play_blue.png"), QIcon::Normal, QIcon::Off);
        topMenuWidget->setIconSize(QSize(12, 12));

        qDebug() << AppVersion::Current().text();
        createFileTabMenu();
        createBaseTabMenu();
        createSectionTabMenu();
        createLoadTabMenu();
        createAnalysisTabMenu();
        createViewSettingTabMenu();
        createOtherTabMenu();

        connect(UnifiedInputData::getInInstance(), &UnifiedInputData::globalNameChanged,
                this, &TopMenuWidget::slotChangeTableName);

        QHBoxLayout *hlay = new QHBoxLayout;
        hlay->addWidget(topMenuWidget);
        hlay->setContentsMargins(3, 3, 3, 3);
        hlay->setSpacing(0);

        this->setLayout(hlay);
    }

    void TopMenuWidget::createFileTabMenu()
    {
        /* ファイル */

        MenuButton *newfile_button = new MenuButton(u8"新規作成", CBSTATUS::CB_DEFAULT, this);
        MenuButton *fileopen_button = new MenuButton(u8"ファイル読込", CBSTATUS::CB_DEFAULT, this);
        MenuButton *filesave_button = new MenuButton(u8"ファイル保存", CBSTATUS::CB_DEFAULT, this);
        saveAs_button = new MenuButton(u8"上書き保存", CBSTATUS::CB_DISABLE, this);
        saveAs_button->setEnabled(false);

        MenuButton *addopen_button = new MenuButton(u8"ファイル追加読込", CBSTATUS::CB_DEFAULT, this);
        MenuButton *dividesave_button = new MenuButton(u8"ファイルの分割保存", CBSTATUS::CB_DEFAULT, this);

        MenuButton *openpstn_button = new MenuButton(u8"pstnファイル読込", CBSTATUS::CB_DEFAULT, this);
        MenuButton *writecsv_button = new MenuButton(u8"psvファイル出力", CBSTATUS::CB_DEFAULT, this);
        MenuButton *estimate_button = new MenuButton(u8"資材量出力", CBSTATUS::CB_DEFAULT, this);

        MenuButton *plan_button = new MenuButton(u8"図面出力", CBSTATUS::CB_DISABLE, this);
        autosave_button = new QCheckBox(u8"自動保存");
        autosave_button->setEnabled(false);

        newfile_button->setFixedSize(90, 45);
        fileopen_button->setFixedSize(90, 45);
        filesave_button->setFixedSize(90, 45);
        saveAs_button->setFixedSize(90, 45);
        addopen_button->setFixedSize(90, 45);
        dividesave_button->setFixedSize(90, 45);
        openpstn_button->setFixedSize(90, 45);
        writecsv_button->setFixedSize(90, 45);
        estimate_button->setFixedSize(90, 45);
        plan_button->setFixedSize(90, 45);

        QHBoxLayout *file_hlay = new QHBoxLayout;
        file_hlay->setSpacing(3);
        file_hlay->addWidget(newfile_button);
        file_hlay->addWidget(new VFLine(Qt::lightGray, this));
        file_hlay->addWidget(fileopen_button);
        file_hlay->addWidget(filesave_button);
        file_hlay->addWidget(saveAs_button);
        file_hlay->addWidget(new VFLine(Qt::lightGray, this));
        file_hlay->addWidget(addopen_button);
        file_hlay->addWidget(dividesave_button);
        file_hlay->addWidget(new VFLine(Qt::lightGray, this));
        file_hlay->addWidget(openpstn_button);
        file_hlay->addWidget(writecsv_button);
        file_hlay->addWidget(estimate_button);
        file_hlay->addWidget(new VFLine(Qt::lightGray, this));
        file_hlay->addWidget(plan_button);
        file_hlay->addWidget(autosave_button);
        file_hlay->addStretch();
        file_hlay->setContentsMargins(5, 0, 10, 0);

        QWidget *file_widget = new QWidget(this);
        file_widget->setLayout(file_hlay);

        topMenuWidget->addTab(file_widget, tabIcon, u8"ファイル");

        connect(newfile_button, &MenuButton::clicked, this, &TopMenuWidget::slotNewFileOpen);
        connect(fileopen_button, &MenuButton::clicked, this, &TopMenuWidget::slotFileOpen);
        connect(filesave_button, &MenuButton::clicked, this, &TopMenuWidget::slotFileSave);
        connect(saveAs_button, &MenuButton::clicked, this, &TopMenuWidget::slotFileSaveAs);
        connect(addopen_button, &MenuButton::clicked, this, &TopMenuWidget::slotAdditionalFileOpen);
        connect(dividesave_button, &MenuButton::clicked, this, &TopMenuWidget::slotDivideFileSave);
        connect(openpstn_button, &MenuButton::clicked, this, &TopMenuWidget::slotPstnOpen);
        connect(writecsv_button, &MenuButton::clicked, this, &TopMenuWidget::slotWritePsv);
        connect(estimate_button, &MenuButton::clicked, this, &TopMenuWidget::slotEstimate);

        //thread miu
        connect(autosave_button, SIGNAL(clicked(bool)), this, SLOT(slotThreadAutoSave(bool)));
        //connect(autosave_button, SIGNAL(clicked(bool)), t_autosave, SLOT(doWork(bool)));
    }

    //thread起動 miu
    void TopMenuWidget::slotThreadAutoSave(bool onoff)
    {
        //AutoSave *t_autosave = new AutoSave;
        //QThread *m_thread = new QThread;
        //t_autosave->moveToThread(m_thread);
        qDebug() << onoff;
        if (onoff == true) {
            t_autosave->start();
        } else {
            t_autosave->stop(); //ループ終了（タイマー）
            t_autosave->quit(); //スレッド終了
            t_autosave->wait(); //終了を待つ
        }
        //        if (onoff == false) {
        //            if (t_autosave->isRunning()) {
        //                t_autosave->quit();
        //                t_autosave->wait();
        //            }
        //        } else if (onoff == true) {
        //            //m_thread->start();
        //            t_autosave->start();
        //        }
    }

    void TopMenuWidget::slotPstnOpen()
    {
        if (npstn != nullptr)
            npstn = nullptr;

        int check = 0;
        npstn = handleNPstn::CreateWithDialog(&check);
        if (check != 0)
        {
            qDebug() << "some error in pstn";
            return;
        }
        qDebug() << "pstn read";
        UnifiedFixedData::getInstance()->getAnalysisData()->setFileName(npstn->getFileName());
        OutputControl::getInstance()->slotOutputFileRead();
    }
    void TopMenuWidget::createBaseTabMenu()
    {
        /*基本設定*/
        MenuButton *floor_button = new MenuButton(u8"階定義", CBSTATUS::CB_NORMAL, this);
        MenuButton *frame_button = new MenuButton(u8"通り定義", CBSTATUS::CB_NORMAL, this);
        MenuButton *material_button = new MenuButton(u8"材料定義", CBSTATUS::CB_NORMAL, this);
        MenuButton *project_button = new MenuButton(u8"プロジェクト定義", CBSTATUS::CB_NORMAL, this);
        floor_button->setFixedSize(90, 45);
        frame_button->setFixedSize(90, 45);
        material_button->setFixedSize(90, 45);
        project_button->setFixedSize(90, 45);

        QHBoxLayout *base_hlay = new QHBoxLayout;
        base_hlay->setSpacing(3);
        base_hlay->addWidget(floor_button);
        base_hlay->addWidget(frame_button);
        base_hlay->addWidget(new VFLine(Qt::lightGray, this));
        base_hlay->addWidget(material_button);
        base_hlay->addWidget(new VFLine(Qt::lightGray, this));
        base_hlay->addWidget(project_button);
        base_hlay->addStretch();
        base_hlay->setContentsMargins(5, 0, 10, 0);

        QWidget *base_widget = new QWidget(this);
        base_widget->setLayout(base_hlay);

        topMenuWidget->addTab(base_widget, tabIcon, u8"基本設定");

        floorSettingDialog = new FloorSettingDialog(this);
        floorSettingDialog->setVisible(false);
        connect(floor_button, &MenuButton::clicked, floorSettingDialog, &QWidget::show);
        connect(floorSettingDialog, &FloorSettingDialog::statusChanged,
                floor_button, &MenuButton::changeButtonColor);

        frameSettingDialog = new FrameSettingDialog(this);
        frameSettingDialog->setVisible(false);
        connect(frame_button, &MenuButton::clicked, frameSettingDialog, &QWidget::show);
        connect(frameSettingDialog, &FrameSettingDialog::statusChanged,
                frame_button, &MenuButton::changeButtonColor);

        materialSettingDialog = new MaterialSettingDialog(this);
        materialSettingDialog->setVisible(false);
        connect(material_button, &MenuButton::clicked, materialSettingDialog, &QWidget::show);
        connect(materialSettingDialog, &MaterialSettingDialog::statusChanged,
                material_button, &MenuButton::changeButtonColor);
    }

    void TopMenuWidget::createSectionTabMenu()
    {
        /* 断面定義 */

        MenuButton *column_button = new MenuButton(u8"柱", CBSTATUS::CB_NORMAL, this);
        MenuButton *girder_button = new MenuButton(u8"大梁", CBSTATUS::CB_NORMAL, this);
        MenuButton *brace_button = new MenuButton(u8"ブレース", CBSTATUS::CB_NORMAL, this);
        MenuButton *beam_button = new MenuButton(u8"小梁", CBSTATUS::CB_NORMAL, this);
        MenuButton *slab_button = new MenuButton(u8"スラブ", CBSTATUS::CB_NORMAL, this);
        MenuButton *wall_button = new MenuButton(u8"壁", CBSTATUS::CB_NORMAL, this);
        MenuButton *uwall_button = new MenuButton(u8"地下外壁", CBSTATUS::CB_NORMAL, this);
        MenuButton *opening_button = new MenuButton(u8"開口", CBSTATUS::CB_NORMAL, this);
        MenuButton *columnbase_button = new MenuButton(u8"柱脚", CBSTATUS::CB_NORMAL, this);
        MenuButton *damper_button = new MenuButton(u8"制振要素", CBSTATUS::CB_NORMAL, this);
        MenuButton *iso_button = new MenuButton(u8"免震要素", CBSTATUS::CB_NORMAL, this);

        column_button->setFixedSize(80, 45);
        girder_button->setFixedSize(80, 45);
        brace_button->setFixedSize(80, 45);
        beam_button->setFixedSize(80, 45);
        slab_button->setFixedSize(80, 45);
        wall_button->setFixedSize(80, 45);
        uwall_button->setFixedSize(80, 45);
        opening_button->setFixedSize(80, 45);
        columnbase_button->setFixedSize(80, 45);
        damper_button->setFixedSize(80, 45);
        iso_button->setFixedSize(80, 45);

        QHBoxLayout *section_hlay = new QHBoxLayout;
        section_hlay->setSpacing(3);
        section_hlay->addWidget(column_button);
        section_hlay->addWidget(girder_button);
        section_hlay->addWidget(brace_button);
        section_hlay->addWidget(beam_button);
        section_hlay->addWidget(new VFLine(Qt::lightGray, this));
        section_hlay->addWidget(slab_button);
        section_hlay->addWidget(wall_button);
        section_hlay->addWidget(uwall_button);
        section_hlay->addWidget(new VFLine(Qt::lightGray, this));
        section_hlay->addWidget(opening_button);
        section_hlay->addWidget(columnbase_button);
        section_hlay->addWidget(new VFLine(Qt::lightGray, this));
        section_hlay->addWidget(damper_button);
        section_hlay->addWidget(iso_button);
        section_hlay->addStretch();
        section_hlay->setContentsMargins(5, 0, 10, 0);

        QWidget *section_widget = new QWidget(this);
        section_widget->setLayout(section_hlay);

        topMenuWidget->addTab(section_widget, tabIcon, u8"断面定義");

        columnSettingDialog = new SectionColumnDialog(this);
        columnSettingDialog->setVisible(false);
        connect(column_button, &MenuButton::clicked, columnSettingDialog, &QWidget::show);
        connect(columnSettingDialog, &SectionCustomDialog::statusChanged,
                column_button, &MenuButton::changeButtonColor);
        girderSettingDialog = new SectionGirderDialog(this);
        girderSettingDialog->setVisible(false);
        connect(girder_button, &MenuButton::clicked, girderSettingDialog, &QWidget::show);
        connect(girderSettingDialog, &SectionCustomDialog::statusChanged,
                girder_button, &MenuButton::changeButtonColor);
        braceSettingDialog = new SectionBraceDialog(this);
        braceSettingDialog->setVisible(false);
        connect(brace_button, &MenuButton::clicked, braceSettingDialog, &QWidget::show);
        connect(braceSettingDialog, &SectionCustomDialog::statusChanged,
                brace_button, &MenuButton::changeButtonColor);
        beamSettingDialog = new SectionBeamDialog(this);
        beamSettingDialog->setVisible(false);
        connect(beam_button, &MenuButton::clicked, beamSettingDialog, &QWidget::show);
        connect(beamSettingDialog, &SectionCustomDialog::statusChanged,
                beam_button, &MenuButton::changeButtonColor);
        slabSettingDialog = new SectionSlabDialog(this);
        slabSettingDialog->setVisible(false);
        connect(slab_button, &MenuButton::clicked, slabSettingDialog, &QWidget::show);
        connect(slabSettingDialog, &SectionCustomDialog::statusChanged,
                slab_button, &MenuButton::changeButtonColor);
        wallSettingDialog = new SectionWallDialog(this);
        wallSettingDialog->setVisible(false);
        connect(wall_button, &MenuButton::clicked, wallSettingDialog, &QWidget::show);
        connect(wallSettingDialog, &SectionCustomDialog::statusChanged,
                wall_button, &MenuButton::changeButtonColor);
        uwallSettingDialog = new SectionUWallDialog(this);
        uwallSettingDialog->setVisible(false);
        connect(uwall_button, &MenuButton::clicked, uwallSettingDialog, &QWidget::show);
        connect(uwallSettingDialog, &SectionCustomDialog::statusChanged,
                uwall_button, &MenuButton::changeButtonColor);
        baseSettingDialog = new SectionBaseDialog(this);
        baseSettingDialog->setVisible(false);
        connect(columnbase_button, &MenuButton::clicked, baseSettingDialog, &QWidget::show);
        connect(baseSettingDialog, &SectionCustomDialog::statusChanged,
                columnbase_button, &MenuButton::changeButtonColor);
        openSettingDialog = new SectionOpeningDialog(this);
        openSettingDialog->setVisible(false);
        connect(opening_button, &MenuButton::clicked, openSettingDialog, &QWidget::show);
        connect(openSettingDialog, &SectionCustomDialog::statusChanged,
                opening_button, &MenuButton::changeButtonColor);
        damperSettingDialog = new SectionDamperDialog(this);
        damperSettingDialog->setVisible(false);
        connect(damper_button, &MenuButton::clicked, damperSettingDialog, &QWidget::show);
        connect(damperSettingDialog, &SectionCustomDialog::statusChanged,
                damper_button, &MenuButton::changeButtonColor);
        isoSettingDialog = new SectionIsolatorDialog(this);
        isoSettingDialog->setVisible(false);
        connect(iso_button, &MenuButton::clicked, isoSettingDialog, &QWidget::show);
        connect(isoSettingDialog, &SectionCustomDialog::statusChanged,
                iso_button, &MenuButton::changeButtonColor);
    }

    void TopMenuWidget::createLoadTabMenu()
    {
        /*荷重定義*/

        MenuButton *jointload_button = new MenuButton(u8"節点荷重", CBSTATUS::CB_NORMAL, this);
        MenuButton *memberload_button = new MenuButton(u8"部材荷重", CBSTATUS::CB_NORMAL, this);
        MenuButton *slabload_button = new MenuButton(u8"床荷重", CBSTATUS::CB_NORMAL, this);

        jointload_button->setFixedSize(90, 45);
        memberload_button->setFixedSize(90, 45);
        slabload_button->setFixedSize(90, 45);

        QHBoxLayout *load_hlay = new QHBoxLayout;
        load_hlay->setSpacing(3);
        load_hlay->addWidget(jointload_button);
        load_hlay->addWidget(memberload_button);
        load_hlay->addWidget(slabload_button);
        load_hlay->addStretch();
        load_hlay->setContentsMargins(5, 0, 0, 0);

        QWidget *load_widget = new QWidget(this);
        load_widget->setLayout(load_hlay);

        jloadSettingDialog = new JointLoadSettingDialog(DATATYPE::TPJOINTLOAD, this);
        jloadSettingDialog->setVisible(false);
        connect(jointload_button, &MenuButton::clicked, jloadSettingDialog, &QWidget::show);
        connect(jloadSettingDialog, &JointLoadSettingDialog::statusChanged,
                jointload_button, &MenuButton::changeButtonColor);

        mloadSettingDialog = new MemberLoadSettingDialog(DATATYPE::TPMEMBERLOAD, this);
        mloadSettingDialog->setVisible(false);
        connect(memberload_button, &MenuButton::clicked, mloadSettingDialog, &QWidget::show);
        connect(mloadSettingDialog, &MemberLoadSettingDialog::statusChanged,
                memberload_button, &MenuButton::changeButtonColor);

        sloadSettingDialog = new SlabLoadSettingDialog(DATATYPE::TPSLABLOAD, this);
        sloadSettingDialog->setVisible(false);
        connect(slabload_button, &MenuButton::clicked, sloadSettingDialog, &QWidget::show);
        connect(sloadSettingDialog, &SlabLoadSettingDialog::statusChanged,
                slabload_button, &MenuButton::changeButtonColor);

        topMenuWidget->addTab(load_widget, tabIcon, u8"荷重定義");
    }

    void TopMenuWidget::createAnalysisTabMenu()
    {

        /* 荷重拾い */
        MenuButton *anaload_button = new MenuButton(u8"荷重拾い実行", CBSTATUS::CB_PLAY, this);

        /* 解析定義 */
        anagroup_button = new MenuButton(u8"解析グループ定義", CBSTATUS::CB_NORMAL, this);
        MenuButton *anafloor_button = new MenuButton(u8"解析階定義", CBSTATUS::CB_NORMAL, this);
        MenuButton *anamodel_button = new MenuButton(u8"解析条件", CBSTATUS::CB_NORMAL, this);

        /* 解析実行・出力 */
        MenuButton *anaexe_button = new MenuButton(u8"解析実行", CBSTATUS::CB_PLAYDONE, this);

        /* 結果表示・出力 */
        MenuButton *stresscombi_button = new MenuButton(u8"応力組合せ", CBSTATUS::CB_DEFAULT, this);
        MenuButton *text_button = new MenuButton(u8"任意テキスト出力", CBSTATUS::CB_DEFAULT, this);
        MenuButton *varout_button = new MenuButton(u8"任意図出力", CBSTATUS::CB_DEFAULT, this);

        /* 計算書 */
        MenuButton *statement_button = new MenuButton(u8"構造計算書", CBSTATUS::CB_NORMAL, this);

        /* 部分検討は未対応
        MenuButton *tool_button = new MenuButton(tr("Part study"), CBSTATUS::CB_DISABLE, this);
        */

        anaload_button->setFixedSize(85, 45);
        anagroup_button->setFixedSize(85, 45);
        anafloor_button->setFixedSize(85, 45);
        anamodel_button->setFixedSize(85, 45);
        anaexe_button->setFixedSize(85, 45);
        stresscombi_button->setFixedSize(85, 45);
        text_button->setFixedSize(85, 45);
        varout_button->setFixedSize(85, 45);
        /* 部分検討は未対応
        tool_button->setFixedSize(85, 45);
        */
        statement_button->setFixedSize(85, 45);

        QHBoxLayout *ana_hlay = new QHBoxLayout;
        ana_hlay->setSpacing(3);
        ana_hlay->addWidget(anaload_button);
        ana_hlay->addWidget(new VFLine(Qt::lightGray, this));
        ana_hlay->addWidget(anagroup_button);
        ana_hlay->addWidget(anafloor_button);
        ana_hlay->addWidget(anamodel_button);
        ana_hlay->addWidget(new VFLine(Qt::lightGray, this));
        ana_hlay->addWidget(anaexe_button);
        ana_hlay->addWidget(new VFLine(Qt::lightGray, this));
        ana_hlay->addWidget(stresscombi_button);
        ana_hlay->addWidget(new VFLine(Qt::lightGray, this));
        ana_hlay->addWidget(text_button);
        ana_hlay->addWidget(varout_button);
        ana_hlay->addWidget(statement_button);
        /* 部分検討は未対応
        ana_hlay->addWidget(new VFLine(Qt::lightGray, this));
        ana_hlay->addWidget(tool_button);
        */
        ana_hlay->addStretch();
        ana_hlay->setContentsMargins(5, 0, 10, 0);

        QWidget *ana_widget = new QWidget(this);
        ana_widget->setLayout(ana_hlay);

        connect(anaload_button, &MenuButton::clicked, this, &TopMenuWidget::slotCalculationLoad);
        connect(anagroup_button, &MenuButton::clicked, this, &TopMenuWidget::slotAnalysisGroupDialogShow);

        analysisFloorDialog = new AnalysisFloorDialog(this);
        analysisFloorDialog->setVisible(false);
        connect(anafloor_button, &MenuButton::clicked, analysisFloorDialog, &QWidget::show);
        connect(analysisFloorDialog, &AnalysisFloorDialog::statusChanged,
                anafloor_button, &MenuButton::changeButtonColor);

        analysisConditionDialog = new AnalysisConditionDialog(this);
        analysisConditionDialog->setVisible(false);
        connect(anamodel_button, &MenuButton::clicked, analysisConditionDialog, &QWidget::show);
        connect(analysisConditionDialog, &AnalysisConditionDialog::statusChanged,
                anamodel_button, &MenuButton::changeButtonColor);

        connect(anaexe_button, &MenuButton::clicked, this, &TopMenuWidget::slotExecuteAnalysis);
        connect(stresscombi_button, &MenuButton::clicked, this, &TopMenuWidget::slotStressCombiDialogShow);

        calculationManagerDialog = new CalculationManagerDialog(this);
        calculationManagerDialog->setVisible(false);
        connect(statement_button, &MenuButton::clicked, this, &TopMenuWidget::slotStructuralStatement);

        outputTextDialog = new OutputTextDialog(this);
        outputTextDialog->setVisible(false);
        connect(text_button, &MenuButton::clicked, this, &TopMenuWidget::slotCalculationTextDialogShow);

        outputFigureDialog = new OutputFigureDialog(this);
        outputFigureDialog->setVisible(false);
        connect(varout_button, &MenuButton::clicked, this, &TopMenuWidget::slotCalculationFigureDialogShow);

        topMenuWidget->addTab(ana_widget, tabIcon, u8"解析実行・出力");
    }

    void TopMenuWidget::createViewSettingTabMenu()
    {
        /*テキスト*/

        MenuButton *inp2d_button = new MenuButton(u8"形状モデル2D", CBSTATUS::CB_DEFAULT, this);
        MenuButton *inp3d_button = new MenuButton(u8"形状モデル3D", CBSTATUS::CB_DEFAULT, this);

        MenuButton *cal2d_button = new MenuButton(u8"荷重拾い2D", CBSTATUS::CB_DEFAULT, this);
        MenuButton *cal3d_button = new MenuButton(u8"荷重拾い3D", CBSTATUS::CB_DEFAULT, this);

        MenuButton *ana2d_button = new MenuButton(u8"解析モデル2D", CBSTATUS::CB_DEFAULT, this);
        MenuButton *ana3d_button = new MenuButton(u8"解析モデル3D", CBSTATUS::CB_DEFAULT, this);

        //    MenuButton *old_ana2d_button = new MenuButton(u8"OLD_解析モデル2D", CBSTATUS::CB_DEFAULT, this);
        //    MenuButton *old_ana3d_button = new MenuButton(u8"OLD_解析モデル3D", CBSTATUS::CB_DEFAULT, this);

        MenuButton *read_button = new MenuButton(u8"表示設定読込", CBSTATUS::CB_DEFAULT, this);
        MenuButton *save_button = new MenuButton(u8"表示設定保存", CBSTATUS::CB_DEFAULT, this);

        inp2d_button->setFixedSize(90, 45);
        inp3d_button->setFixedSize(90, 45);
        cal2d_button->setFixedSize(90, 45);
        cal3d_button->setFixedSize(90, 45);
        ana2d_button->setFixedSize(90, 45);
        ana3d_button->setFixedSize(90, 45);
        //    old_ana2d_button->setFixedSize(90, 45);
        //    old_ana3d_button->setFixedSize(90, 45);
        read_button->setFixedSize(90, 45);
        save_button->setFixedSize(90, 45);

        QHBoxLayout *display_hlay = new QHBoxLayout;
        display_hlay->setSpacing(3);
        display_hlay->addWidget(inp2d_button);
        display_hlay->addWidget(inp3d_button);
        display_hlay->addWidget(new VFLine(Qt::lightGray, this));
        display_hlay->addWidget(cal2d_button);
        display_hlay->addWidget(cal3d_button);
        display_hlay->addWidget(new VFLine(Qt::lightGray, this));
        display_hlay->addWidget(ana2d_button);
        display_hlay->addWidget(ana3d_button);
        display_hlay->addWidget(new VFLine(Qt::lightGray, this));
        //    display_hlay->addWidget(old_ana2d_button);
        //    display_hlay->addWidget(old_ana3d_button);
        //    display_hlay->addWidget(new VFLine(Qt::lightGray, this));
        display_hlay->addWidget(read_button);
        display_hlay->addWidget(save_button);
        display_hlay->addStretch();
        display_hlay->setContentsMargins(5, 0, 10, 0);

        QWidget *display_widget = new QWidget(this);
        display_widget->setLayout(display_hlay);

        input2DSettingDialog = new Input2DSettingDialog(this);
        input3DSettingDialog = new Input3DSettingDialog(this);
        calculation2DSettingDialog = new CalculationLoad2DSettingDialog(this);
        calculation3DSettingDialog = new CalculationLoad3DSettingDialog(this);
        analysis2DSettingDialog = new Analysis2DSettingDialog(this);
        analysis3DSettingDialog = new Analysis3DSettingDialog(this);
        //    old_analysis2DSettingDialog = new OLD_Analysis2DSettingDialog(this);
        //    old_analysis3DSettingDialog = new OLD_Analysis3DSettingDialog(this);

        input2DSettingDialog->setVisible(false);
        input3DSettingDialog->setVisible(false);
        calculation2DSettingDialog->setVisible(false);
        calculation3DSettingDialog->setVisible(false);
        analysis2DSettingDialog->setVisible(false);
        analysis3DSettingDialog->setVisible(false);
        //    old_analysis2DSettingDialog->setVisible(false);
        //    old_analysis3DSettingDialog->setVisible(false);

        connect(inp2d_button, &MenuButton::clicked, input2DSettingDialog, &QWidget::show);
        connect(inp3d_button, &MenuButton::clicked, input3DSettingDialog, &QWidget::show);
        connect(cal2d_button, &MenuButton::clicked, calculation2DSettingDialog, &CalculationLoad2DSettingDialog::show);
        connect(cal3d_button, &MenuButton::clicked, calculation3DSettingDialog, &CalculationLoad3DSettingDialog::show);
        connect(ana2d_button, &MenuButton::clicked, analysis2DSettingDialog, &Analysis2DSettingDialog::show);
        connect(ana3d_button, &MenuButton::clicked, analysis3DSettingDialog, &Analysis3DSettingDialog::show);
        //    connect ( old_ana2d_button, &MenuButton::clicked, old_analysis2DSettingDialog, &QWidget::show );
        //    connect ( old_ana3d_button, &MenuButton::clicked, old_analysis3DSettingDialog, &QWidget::show );

        topMenuWidget->addTab(display_widget, tabIcon, u8"表示設定");
    }

    void TopMenuWidget::createOtherTabMenu()
    {
        /* その他 */

        MenuButton *text_button = new MenuButton(u8"テキスト入力", CBSTATUS::CB_DEFAULT, this);
        MenuButton *backup_button = new MenuButton(u8"BackUp設定", CBSTATUS::CB_DEFAULT, this);
        MenuButton *operate_button = new MenuButton(u8"操作設定", CBSTATUS::CB_DEFAULT, this);
        MenuButton *window_button = new MenuButton(u8"ウィンドウ設定", CBSTATUS::CB_DEFAULT, this);

        text_button->setFixedSize(90, 45);
        backup_button->setFixedSize(90, 45);
        operate_button->setFixedSize(90, 45);
        window_button->setFixedSize(90, 45);

        textInputDialog = new TextInputDialog(this);
        textInputDialog->setVisible(false);
        connect(text_button, &MenuButton::clicked, textInputDialog, &QWidget::show);

        QHBoxLayout *other_hlay = new QHBoxLayout;
        other_hlay->setSpacing(3);
        other_hlay->addWidget(text_button);
        other_hlay->addWidget(new VFLine(Qt::lightGray, this));
        other_hlay->addWidget(backup_button);
        other_hlay->addWidget(new VFLine(Qt::lightGray, this));
        other_hlay->addWidget(window_button);
        other_hlay->addWidget(operate_button);
        other_hlay->addStretch();
        other_hlay->setContentsMargins(5, 0, 10, 0);

        QWidget *other_widget = new QWidget(this);
        other_widget->setLayout(other_hlay);

        topMenuWidget->addTab(other_widget, tabIcon, u8"オプション");
    }

    void TopMenuWidget::slotChangeTableName(DATATYPE dtype, const QString &str_a, const QString &str_b)
    {
        switch (dtype)
        {
        case DATATYPE::TPCOLUMN:
            columnSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPGIRDER:
            girderSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPBRACE:
            braceSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPBEAM:
            beamSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPSLAB:
            slabSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPWALL:
            wallSettingDialog->replaceMatchingWallName(str_a, str_b);
            break;
        case DATATYPE::TPUWALL:
            uwallSettingDialog->replaceMatchingUWallName(str_a, str_b);
            break;
        case DATATYPE::TPBASE:
            baseSettingDialog->replaceMatchingName(str_a, str_b);
            columnSettingDialog->replaceMatchingBaseName(str_a, str_b);
            break;
        case DATATYPE::TPWALLOPEN:
            openSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPDAMPER:
            damperSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPISO:
            isoSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPJOINTLOAD:
            jloadSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPMEMBERLOAD:
            mloadSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        case DATATYPE::TPSLABLOAD:
            sloadSettingDialog->replaceMatchingName(str_a, str_b);
            break;
        default:
            break;
        }
    }

    void TopMenuWidget::slotFileSave()
    {
        QString filename = QFileDialog::getSaveFileName(this, QString(), QString(), "File(*.p3d)");
        if (filename.isEmpty())
            return;
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly))
        {
            Utility::showErrorMessage(this, u8"ファイル保存に失敗しました。");
            return;
        }

        QDataStream outfile(&file);
        writeP3dDataFile(outfile);

        UnifiedEditingData::getInstance()->setCurrentFile(filename);
        emit saveFileChanged();

        UnifiedEditingData::getInstance()->sendLogMessage("save file. \"" + filename + "\"");
    }
    //MiuTest
    void TopMenuWidget::slotFileSaveAs()
    {
        QString filename = UnifiedEditingData::getInstance()->currentFilePath();
        if (filename.isEmpty())
            return;
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly))
        {
            Utility::showErrorMessage(this, u8"ファイル保存に失敗しました。");
            return;
        }

        QDataStream outfile(&file);
        writeP3dDataFile(outfile);
        UnifiedInputData::getInInstance()->setFileName(filename);

        emit saveFileChanged();

        //Miutest
        QTemporaryFile tmpFile;
        ///ファイルコピー
        QFile::copy(filename, tmpFile.fileTemplate());
        //QSettings::copyFile("C:/Users/miu/Desktop/NIKKEN/post3D/a.p3d", tmpFile.fileTemplate());
        //一時ファイル作成
        tmpFile.open();
        qDebug() << tmpFile.fileName() << "Miufile";
        //C:\Users\miu\AppData\Local\Temp\post3DMain.XXXXXに一時ファイル保存

        UnifiedEditingData::getInstance()->sendLogMessage("save as file. \"" + filename + "\"");
    }

    void TopMenuWidget::slotFileOpen()
    {
        QString filename =
            QFileDialog::getOpenFileName(this, u8"ファイルを開く", ".", "File(*.p3d *.psv *.csv)");
        if (filename.isEmpty())
            return;

        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly))
        {
            Utility::showErrorMessage(this, u8"ファイルが開けません。");
            return;
        }
        if (QFileInfo(file).suffix() == "p3d")
        {
            QDataStream infile(&file);

            // ファイルチェック
            if (!checkDataFile(infile))
            {
                Utility::showErrorMessage(this, u8"ファイルフォーマットが正しくありません。");
                return;
            }

            UnifiedEditingData::getInstance()->clearUndoStack();
            UnifiedInputData::getInInstance()->clearUnifiedData();
            UnifiedProject::getInstance()->clearData();

            //    QString msg;   read メッセージ　バージョンv000のデータです。

            // ファイルの読み込み
            readP3dFile(infile);

            UnifiedEditingData::getInstance()->setCurrentFile(filename);
        }
        else if (QFileInfo(file).suffix() == "psv")
        {
            UnifiedEditingData::getInstance()->clearUndoStack();
            UnifiedInputData::getInInstance()->clearUnifiedData();
            UnifiedProject::getInstance()->clearData();
            bool useGui = false;
            // bool useGui = true;
            if (useGui)
            {
                QTextStream in(&file);
                readPsvFile(in);
            }
            else
            {
                // debug
                ReadFromPsv psv;
                psv.ReadPsv(&file);
                reflectUnifiedData();
            }
            emit UnifiedInputData::getInInstance()->elementCountChanged();
        }
        else if (QFileInfo(file).suffix() == "csv")
        {
            UnifiedEditingData::getInstance()->clearUndoStack();
            UnifiedInputData::getInInstance()->clearUnifiedData();
            UnifiedProject::getInstance()->clearData();
            readFromBld3d b3d;
            b3d.readCsvData(&file);
            reflectUnifiedData();
            emit UnifiedInputData::getInInstance()->elementCountChanged();
        }
        file.close();

        UnifiedInputData::getInInstance()->setFileName(filename);

        emit saveFileChanged();

        UnifiedEditingData::getInstance()->sendLogMessage("open file. \"" + filename + "\"");
    }

    void TopMenuWidget::writeP3dDataFile(QDataStream &outfile) const
    {
        QIODevice *device = outfile.device();
        QHash<QString, qint64> keyToPos;

        device->seek(1000); // 1000バイト目まではヘッダー情報

        writeListP3dData(outfile, keyToPos);
        writeWidgetP3dData(outfile, keyToPos);

        keyToPos.insert("JOINTANDMEMBER", device->pos());
        UnifiedInputData::getInInstance()->writeP3dData(outfile);
        UnifiedProject::getInstance()->writeP3dData(outfile);

        // Header 記入
        device->seek(0);
        outfile << AppVersion::Current().text() << LibraryVersion::Current().text() << keyToPos;

        device->close();
    }

    void TopMenuWidget::slotWritePsv()
    {
        QString filename =
            QFileDialog::getSaveFileName(this, QString(), QString(), "File(*.psv)");
        if (filename.isEmpty())
            return;
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            Utility::showErrorMessage(this, u8"ファイル保存に失敗しました。");
            return;
        }

        QTextStream outfile(&file);
        outfile.setCodec("UTF-8");
        outfile << LibraryVersion::Current().text() << "\n";
        writeListPsvData(outfile);
        QString errorMessage = UnifiedInputData::getInInstance()->writePsvData(outfile);
        if (errorMessage != QString{})
            Utility::showErrorMessage(this, errorMessage);
        file.close();
        UnifiedEditingData::getInstance()->sendLogMessage("save file. \"" + filename + "\"");
    }

    void TopMenuWidget::slotEstimate()
    {
        QString filename =
            QFileDialog::getSaveFileName(this, QString(), QString(), "File(*.txt)");
        if (filename.isEmpty())
            return;
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            Utility::showErrorMessage(this, u8"ファイル保存に失敗しました。");
            return;
        }

        QTextStream outfile(&file);
        UnifiedInputData::getInInstance()->estimate(outfile);
        file.close();

        UnifiedEditingData::getInstance()->sendLogMessage("Estimate. \"" + filename + "\"");
    }

    void TopMenuWidget::slotAnalysisGroupDialogShow()
    {
        auto dialog = new AnalysisGroupDialog(this);
        connect(dialog, &AnalysisGroupDialog::statusChanged,
                anagroup_button, &MenuButton::changeButtonColor);
        if (dialog->exec() == QDialog::Accepted)
        {
        }
    }

    void TopMenuWidget::writeListP3dData(QDataStream &outfile, QHash<QString, qint64> &keyToPos) const
    {
        QIODevice *device = outfile.device();

        keyToPos.insert("FLOOR", device->pos());
        floorSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("FRAME", device->pos());
        frameSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("MATERIAL", device->pos());
        materialSettingDialog->writeBinaryFile(outfile);

        keyToPos.insert("COLUMN", device->pos());
        columnSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("GIRDER", device->pos());
        girderSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("BRACE", device->pos());
        braceSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("BEAM", device->pos());
        beamSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("SLAB", device->pos());
        slabSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("WALL", device->pos());
        wallSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("UWALL", device->pos());
        uwallSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("BASEPLATE", device->pos());
        baseSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("WALLOPEN", device->pos());
        openSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("DAMPER", device->pos());
        damperSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("ISOLATOR", device->pos());
        isoSettingDialog->writeBinaryFile(outfile);

        keyToPos.insert("JOINTLOAD", device->pos());
        jloadSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("MEMBERLOAD", device->pos());
        mloadSettingDialog->writeBinaryFile(outfile);
        keyToPos.insert("SLABLOAD", device->pos());
        sloadSettingDialog->writeBinaryFile(outfile);

        keyToPos.insert("ANALYSISFLOOR", device->pos());
        analysisFloorDialog->writeBinaryFile(outfile);

        keyToPos.insert("ANALYSISCONDITION", device->pos());
        analysisConditionDialog->writeBinaryFile(outfile);
    }

    void TopMenuWidget::writeWidgetP3dData(QDataStream &outfile, QHash<QString, qint64> &keyToPos) const
    {
        QIODevice *device = outfile.device();
        keyToPos.insert("OUTPUTTEXT", device->pos());
        outputTextDialog->writeBinaryFile(outfile);
        keyToPos.insert("OUTPUTFIGURE", device->pos());
        outputFigureDialog->writeBinaryFile(outfile);
        keyToPos.insert("CALCULATION", device->pos());
        calculationManagerDialog->writeBinaryFile(outfile);
    }

    void TopMenuWidget::readPsvFile(QTextStream &infile)
    {
        QString appVerText = infile.readLine(0);
        QString libVerText = infile.readLine();
        AppVersion appVer(appVerText);
        LibraryVersion libVer(libVerText);

        while (!infile.atEnd())
        {
            QString str = infile.readLine(0);
            if (str == "**FLOOR")
            {
                floorSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**FRAME")
            {
                frameSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**MATERIAL")
            {
                materialSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**COLUMN")
            {
                columnSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**GIRDER")
            {
                girderSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**BRACE")
            {
                braceSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**BEAM")
            {
                beamSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**SLAB")
            {
                slabSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**WALL")
            {
                wallSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**UWALL")
            {
                uwallSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**BASEPLATE")
            {
                baseSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**WALLOPEN")
            {
                openSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**DAMPER")
            {
                damperSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**ISOLATOR")
            {
                isoSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**JOINTLOAD")
            {
                jloadSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**MEMBERLOAD")
            {
                mloadSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**SLABLOAD")
            {
                sloadSettingDialog->readTextFile(infile, appVer);
            }
            else if (str == "**ANALYSISFLOOR")
            {
                analysisFloorDialog->readTextFile(infile, appVer);
            }
            else if (str == "**GENE")
            {
                analysisConditionDialog->readTextFile(infile, appVer);
            }
            else if (str == "**JOINT")
            {
                UnifiedInputData::getInInstance()->readTextJointData(infile, libVer);
            }
            else if (str == "**COLUMN_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPCOLUMN, libVer);
            }
            else if (str == "**GIRDER_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPGIRDER, libVer);
            }
            else if (str == "**BRACE_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPBRACE, libVer);
            }
            else if (str == "**BEAM_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPBEAM, libVer);
            }
            else if (str == "**SLAB_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPSLAB, libVer);
            }
            else if (str == "**WALL_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPWALL, libVer);
            }
            else if (str == "**UWALL_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPUWALL, libVer);
            }
            else if (str == "**DAMPER_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPDAMPER, libVer);
            }
            else if (str == "**ISOLATOR_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPISO, libVer);
            }
            else if (str == "**JOINTLOAD_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPJOINTLOAD, libVer);
            }
            else if (str == "**MEMBERLOAD_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPMEMBERLOAD, libVer);
            }
            else if (str == "**SLABLOAD_MEMBER")
            {
                UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPSLABLOAD, libVer);
            }
        }
    }

    void TopMenuWidget::reflectUnifiedData()
    {
        floorSettingDialog->reflectUnifiedData();
        frameSettingDialog->reflectUnifiedData();
        materialSettingDialog->reflectUnifiedData();
        columnSettingDialog->reflectUnifiedData();
        girderSettingDialog->reflectUnifiedData();
        braceSettingDialog->reflectUnifiedData();
        beamSettingDialog->reflectUnifiedData();
        slabSettingDialog->reflectUnifiedData();
        wallSettingDialog->reflectUnifiedData();
        uwallSettingDialog->reflectUnifiedData();
        baseSettingDialog->reflectUnifiedData();
        openSettingDialog->reflectUnifiedData();
        damperSettingDialog->reflectUnifiedData();
        isoSettingDialog->reflectUnifiedData();
        jloadSettingDialog->reflectUnifiedData();
        mloadSettingDialog->reflectUnifiedData();
        sloadSettingDialog->reflectUnifiedData();
        analysisFloorDialog->reflectUnifiedData();
    }
    void TopMenuWidget::readP3dFile(QDataStream &infile)
    {
        QString appVerText, libVerText, keyword;
        QHash<QString, qint64> keyToPos;
        AppVersion appVer(appVerText);
        LibraryVersion libVer(libVerText);
        // Header
        infile.device()->seek(0);
        infile >> appVerText >> libVerText >> keyToPos;
        qDebug() << "App ver" << appVerText << "Lib ver" << libVerText;
        keyword = "FLOOR";
        infile.device()->seek(keyToPos.value(keyword));
        floorSettingDialog->readBinaryFile(infile, appVer);

        keyword = "FRAME";
        infile.device()->seek(keyToPos.value(keyword));
        frameSettingDialog->readBinaryFile(infile, appVer);

        keyword = "MATERIAL";
        infile.device()->seek(keyToPos.value(keyword));
        materialSettingDialog->readBinaryFile(infile, appVer);

        keyword = "COLUMN";
        infile.device()->seek(keyToPos.value(keyword));
        columnSettingDialog->readBinaryFile(infile, appVer);

        keyword = "GIRDER";
        infile.device()->seek(keyToPos.value(keyword));
        girderSettingDialog->readBinaryFile(infile, appVer);

        keyword = "BRACE";
        infile.device()->seek(keyToPos.value(keyword));
        braceSettingDialog->readBinaryFile(infile, appVer);

        keyword = "BEAM";
        infile.device()->seek(keyToPos.value(keyword));
        beamSettingDialog->readBinaryFile(infile, appVer);

        keyword = "SLAB";
        infile.device()->seek(keyToPos.value(keyword));
        slabSettingDialog->readBinaryFile(infile, appVer);

        keyword = "WALL";
        infile.device()->seek(keyToPos.value(keyword));
        wallSettingDialog->readBinaryFile(infile, appVer);

        keyword = "UWALL";
        infile.device()->seek(keyToPos.value(keyword));
        uwallSettingDialog->readBinaryFile(infile, appVer);

        keyword = "BASEPLATE";
        infile.device()->seek(keyToPos.value(keyword));
        baseSettingDialog->readBinaryFile(infile, appVer);

        keyword = "WALLOPEN";
        infile.device()->seek(keyToPos.value(keyword));
        openSettingDialog->readBinaryFile(infile, appVer);

        keyword = "DAMPER";
        infile.device()->seek(keyToPos.value(keyword));
        damperSettingDialog->readBinaryFile(infile, appVer);

        keyword = "ISOLATOR";
        infile.device()->seek(keyToPos.value(keyword));
        isoSettingDialog->readBinaryFile(infile, appVer);

        keyword = "JOINTLOAD";
        infile.device()->seek(keyToPos.value(keyword));
        jloadSettingDialog->readBinaryFile(infile, appVer);

        keyword = "MEMBERLOAD";
        infile.device()->seek(keyToPos.value(keyword));
        mloadSettingDialog->readBinaryFile(infile, appVer);

        keyword = "SLABLOAD";
        infile.device()->seek(keyToPos.value(keyword));
        sloadSettingDialog->readBinaryFile(infile, appVer);

        keyword = "ANALYSISFLOOR";
        infile.device()->seek(keyToPos.value(keyword));
        analysisFloorDialog->readBinaryFile(infile, appVer);

        keyword = "ANALYSISCONDITION";
        infile.device()->seek(keyToPos.value(keyword));
        analysisConditionDialog->readBinaryFile(infile, appVer);

        keyword = "OUTPUTTEXT";
        infile.device()->seek(keyToPos.value(keyword));
        outputTextDialog->readBinaryFile(infile, appVer);

        keyword = "OUTPUTFIGURE";
        infile.device()->seek(keyToPos.value(keyword));
        outputFigureDialog->readBinaryFile(infile, appVer);

        keyword = "CALCULATION";
        infile.device()->seek(keyToPos.value(keyword));
        // calculationManagerDialog->readBinaryFile(infile, ver);

        keyword = "JOINTANDMEMBER";
        infile.device()->seek(keyToPos.value(keyword));

        UnifiedInputData::getInInstance()->readUnifiedData(infile, libVer);
        UnifiedProject::getInstance()->readP3dData(infile, libVer);

        infile.device()->close();
    }

    void TopMenuWidget::writeListPsvData(QTextStream &outfile) const
    {
        outfile << u8"**FLOOR\n";
        floorSettingDialog->writeTextFile(outfile, true);
        outfile << u8"**FRAME\n";
        frameSettingDialog->writeTextFile(outfile, true);
        outfile << u8"**MATERIAL\n";
        materialSettingDialog->writeTextFile(outfile);
        outfile << u8"**COLUMN\n";
        columnSettingDialog->writeTextFile(outfile);
        outfile << u8"**GIRDER\n";
        girderSettingDialog->writeTextFile(outfile);
        outfile << u8"**BRACE\n";
        braceSettingDialog->writeTextFile(outfile);
        outfile << u8"**BEAM\n";
        beamSettingDialog->writeTextFile(outfile);
        outfile << u8"**SLAB\n";
        slabSettingDialog->writeTextFile(outfile);
        outfile << u8"**WALL\n";
        wallSettingDialog->writeTextFile(outfile);
        outfile << u8"**UWALL\n";
        uwallSettingDialog->writeTextFile(outfile);
        outfile << u8"**BASEPLATE\n";
        baseSettingDialog->writeTextFile(outfile);
        outfile << u8"**WALLOPEN\n";
        openSettingDialog->writeTextFile(outfile);
        outfile << u8"**DAMPER\n";
        damperSettingDialog->writeTextFile(outfile);
        outfile << u8"**ISOLATOR\n";
        isoSettingDialog->writeTextFile(outfile);
        outfile << u8"**JOINTLOAD\n";
        jloadSettingDialog->writeTextFile(outfile);
        outfile << u8"**MEMBERLOAD\n";
        mloadSettingDialog->writeTextFile(outfile);
        outfile << u8"**SLABLOAD\n";
        sloadSettingDialog->writeTextFile(outfile);
        outfile << u8"**ANALYSISFLOOR\n";
        analysisFloorDialog->writeTextFile(outfile);

        analysisConditionDialog->writeTextFile(outfile);
    }

    bool TopMenuWidget::checkDataFile(QDataStream &infile) const
    {
        QString appVerText, libVerText, keyword;
        QHash<QString, qint64> keyToPos;

        // Header
        infile.device()->seek(0);
        infile >> appVerText >> libVerText >> keyToPos;
        AppVersion appVer(appVerText);

        qDebug() << "App:" << appVerText << ", Lib:" << libVerText;
        keyword = "FLOOR";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!floorSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "floor done";

        keyword = "FRAME";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!frameSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "frame done";

        keyword = "MATERIAL";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!materialSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "mat done";

        keyword = "COLUMN";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!columnSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "col done";

        keyword = "GIRDER";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!girderSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "gir done";

        keyword = "BRACE";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!braceSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "bra done";

        keyword = "BEAM";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!beamSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "beam done";

        keyword = "SLAB";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!slabSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "slab done";

        keyword = "WALL";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!wallSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "wall done";

        keyword = "UWALL";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!uwallSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "uwall done";

        keyword = "BASEPLATE";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!baseSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "base done";

        keyword = "WALLOPEN";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!openSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "op done";

        keyword = "DAMPER";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!damperSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "damp done";

        keyword = "ISOLATOR";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!isoSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "isol done";

        keyword = "JOINTLOAD";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!jloadSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "jload done";

        keyword = "MEMBERLOAD";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!mloadSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "mload done";

        keyword = "SLABLOAD";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!sloadSettingDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "slabload done";

        keyword = "ANALYSISFLOOR";
        if (!keyToPos.contains(keyword))
            return false;
        if (!infile.device()->seek(keyToPos.value(keyword)))
            return false;
        if (!analysisFloorDialog->checkBinaryFile(infile, appVer))
            return false;
        qDebug() << "afloor done";

        keyword = "JOINTANDMEMBER";
        if (!keyToPos.contains(keyword))
        {
            qDebug() << "j and m not found";
            return false;
        }
        qDebug() << "JOINTANDMEMBER1";
        if (!infile.device()->seek(keyToPos.value(keyword)))
        {
            qDebug() << "cannot seek";
            return false;
        }
        qDebug() << "JOINTANDMEMBER2";
        qDebug() << UnifiedInputData::getInInstance()->memberListOfAll();

        LibraryVersion libVer(libVerText);
        if (!UnifiedInputData::getInInstance()->checkUnifiedData(infile, libVer))
        {
            qDebug() << "cannot get instance";
            return false;
        }
        qDebug() << "j and m done";

        return true;
    }

    void TopMenuWidget::slotNewFileOpen()
    {
        QMessageBox msg(this);
        msg.setWindowTitle(u8"新規ファイル");
        msg.setText(u8"編集データが破棄されます。\n新規ファイルを作成しますか？");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Yes);
        msg.button(QMessageBox::Yes)->setText(u8"新規ファイル");
        msg.button(QMessageBox::Cancel)->setText(u8"キャンセル");
        int ret = msg.exec();
        if (ret == QMessageBox::Cancel)
            return;

        UnifiedEditingData::getInstance()->setCurrentFile(QString());
        emit saveFileChanged();

        UnifiedEditingData::getInstance()->clearUndoStack();
        UnifiedInputData::getInInstance()->clearUnifiedData();
        UnifiedInputData::getInInstance()->clearDefinedData();
        UnifiedProject::getInstance()->clearData();

        floorSettingDialog->clearData();
        frameSettingDialog->clearData();
        materialSettingDialog->clearData();

        columnSettingDialog->clearData();
        girderSettingDialog->clearData();
        braceSettingDialog->clearData();
        beamSettingDialog->clearData();
        slabSettingDialog->clearData();
        wallSettingDialog->clearData();
        uwallSettingDialog->clearData();
        baseSettingDialog->clearData();
        openSettingDialog->clearData();
        damperSettingDialog->clearData();
        isoSettingDialog->clearData();

        jloadSettingDialog->clearData();
        mloadSettingDialog->clearData();
        sloadSettingDialog->clearData();

        analysisFloorDialog->clearData();
        analysisConditionDialog->clearData();

        UnifiedEditingData::getInstance()->sendLogMessage("clear all data and open new file.");
    }

    void TopMenuWidget::slotAdditionalFileOpen()
    {
        AdditionalOpenDialog dialog(this);

        if (dialog.exec() == QDialog::Accepted)
        {

            FILEREADPATTERN fpt = dialog.getFileReadPattern();

            QString n_filename = fpt.fileB; // 後から追加されるファイル名

            if (!fpt.fileA.isEmpty())
            { // A,Bファイルを同時読込

                QString o_filename; // 最初に読み込まれるファイル名
                if (fpt.priorList == FILEPRIOR::PRIOR_A)
                {
                    o_filename = fpt.fileA;
                    n_filename = fpt.fileB;
                }
                else
                {
                    o_filename = fpt.fileB;
                    n_filename = fpt.fileA;
                }

                QFile o_file(o_filename);
                if (!o_file.open(QIODevice::ReadOnly))
                {
                    Utility::showErrorMessage(this, u8"ファイルが開けません。");
                    return;
                }
                QDataStream o_infile(&o_file);

                // ファイルチェック
                if (!checkDataFile(o_infile))
                {
                    Utility::showErrorMessage(this, u8"ファイルフォーマットが正しくありません。");
                    return;
                }

                UnifiedEditingData::getInstance()->clearUndoStack();
                UnifiedInputData::getInInstance()->clearUnifiedData();
                UnifiedProject::getInstance()->clearData();

                // ファイルの読み込み
                readP3dFile(o_infile);
            }

            // 追加されるファイルの読み込み
            QFile n_file(n_filename);
            if (!n_file.open(QIODevice::ReadOnly))
            {
                Utility::showErrorMessage(this, u8"ファイルが開けません。");
                return;
            }

            QDataStream n_infile(&n_file);
            QString appVerText, libVerText;
            QHash<QString, qint64> keyToPos;
            n_infile >> appVerText >> libVerText >> keyToPos;
            AppVersion appVer(appVerText);
            LibraryVersion libVer(libVerText);
            bool isReadOk = (keyToPos.contains("FLOOR"));
            if (isReadOk)
                isReadOk = (keyToPos.contains("FRAME"));
            if (isReadOk)
                isReadOk = (keyToPos.contains("JOINTANDMEMBER"));
            if (isReadOk)
                isReadOk = n_file.seek(keyToPos.value("FLOOR"));
            if (isReadOk)
                isReadOk = (floorSettingDialog->checkBinaryFile(n_infile, appVer));
            if (isReadOk)
                isReadOk = n_file.seek(keyToPos.value("FRAME"));
            if (isReadOk)
                isReadOk = (frameSettingDialog->checkBinaryFile(n_infile, appVer));
            if (isReadOk)
                isReadOk = n_file.seek(keyToPos.value("JOINTANDMEMBER"));
            if (isReadOk)
                isReadOk = (UnifiedInputData::getInInstance()->checkUnifiedData(n_infile, libVer));
            if (!isReadOk)
            {
                Utility::showErrorMessage(this, u8"ファイルフォーマットが正しくありません。");
                return;
            }
            UnifiedEditingData::getInstance()->clearUndoStack();

            QHash<QUuid, QString> fidToName; // 追加ファイル内の階名・通り名

            n_file.seek(keyToPos.value("FLOOR"));
            int ivar, rows;
            QStringList strlist;
            QList<QVariant> vlist;
            n_infile >> ivar >> rows >> strlist;
            for (int i = 0; i < rows; i++)
            {
                n_infile >> vlist;
                // Name:1,UUID:4
                fidToName.insert(vlist.at(4).toString(), vlist.at(1).toString());
            }

            n_file.seek(keyToPos.value("FRAME"));
            n_infile >> ivar >> rows >> strlist;
            for (int i = 0; i < rows; i++)
                n_infile >> vlist;
            n_infile >> ivar >> rows >> strlist;
            for (int i = 0; i < rows; i++)
            {
                n_infile >> vlist;
                // Name:3,UUID:9
                fidToName.insert(vlist.at(9).toString(), vlist.at(3).toString());
            }

            n_file.seek(keyToPos.value("JOINTANDMEMBER"));

            UnifiedInputData::getInInstance()->readAdditionalData(n_infile, libVer, fidToName, fpt);

            n_file.close();
        }

        UnifiedEditingData::getInstance()->sendLogMessage("additional read file.");
    }
    void TopMenuWidget::slotStressCombiDialogShow()
    {
        StressCombiDialog dialog(this);
        //            connect ( dialog, SIGNAL( statusChanged() ),
        //                      this, SLOT( updateElevationView(DATATYPE) ) );

        if (dialog.exec() == QDialog::Accepted)
        {
        }

        //    UnifiedInputData::getInInstance()->sendLogMessage( "" );
    }

    void TopMenuWidget::slotDivideFileSave()
    {
        DivideSaveDialog dialog(this);

        if (dialog.exec() == QDialog::Accepted)
        {

            FILESAVEPATTERN fpt = dialog.getFileSavePattern();

            if (fpt.fileB.isEmpty())
            {

                QFile fileA(fpt.fileA);
                if (!fileA.open(QIODevice::WriteOnly))
                {
                    Utility::showErrorMessage(this, u8"ファイルに書き込めません。");
                    return;
                }

                QDataStream outfileA(&fileA);
                QHash<QString, qint64> keyToPosA;

                fileA.seek(1000); // 1000バイト目まではヘッダー情報
                writeListP3dData(outfileA, keyToPosA);

                keyToPosA.insert("JOINTANDMEMBER", fileA.pos());
                UnifiedInputData::getInInstance()->writeLimitedData(outfileA, fpt.floorList);

                fileA.seek(0);
                outfileA << AppVersion::Current().text() << LibraryVersion::Current().text() << keyToPosA;
                fileA.close();
            }
            else
            {

                QFile fileA(fpt.fileA);
                QFile fileB(fpt.fileB);
                if (!fileA.open(QIODevice::WriteOnly))
                {
                    Utility::showErrorMessage(this, u8"ファイルに書き込めません。");
                    return;
                }
                if (!fileB.open(QIODevice::WriteOnly))
                {
                    Utility::showErrorMessage(this, u8"ファイルに書き込めません。");
                    return;
                }

                QDataStream outfileA(&fileA);
                QDataStream outfileB(&fileB);

                QHash<QString, qint64> keyToPosA, keyToPosB;

                fileA.seek(1000); // 1000バイト目まではヘッダー情報
                writeListP3dData(outfileA, keyToPosA);
                fileB.seek(1000); // 1000バイト目まではヘッダー情報
                writeListP3dData(outfileB, keyToPosB);

                keyToPosA.insert("JOINTANDMEMBER", fileA.pos());
                keyToPosB.insert("JOINTANDMEMBER", fileB.pos());
                UnifiedInputData::getInInstance()->writeDividedData(outfileA, outfileB,
                                                                    fpt.floorList);

                fileA.seek(0);
                outfileA << LibraryVersion::Current().text() << keyToPosA;
                fileA.close();
                fileB.seek(0);
                outfileB << LibraryVersion::Current().text() << keyToPosB;
                fileB.close();
            }

            Utility::showInformationMessage(this, u8"ファイル書き込みが正常に終了しました。");
        }
    }

    void TopMenuWidget::slotCalculationLoad()
    {
        if (UnifiedEditingData::getInstance()->currentFileBaseName().isEmpty())
        {
            Utility::showErrorMessage(this, u8"保存先ファイルが指定されていません。\nファイル保存をしてから再度実行してください。");
        }

        FixedTableData *copyData = FixedTableData::Create();
        setTableModel(copyData);
        UnifiedFixedData::setInstance(copyData);

        QProgressDialog progress(u8"荷重拾い実行中です...", u8"キャンセル", 0, 100, this);
        // ***_outputs/output_loads.csv へのファイル保存
        QString o_fpath = UnifiedEditingData::getInstance()->currentFileDir();
        QString o_dirname = UnifiedEditingData::getInstance()->currentFileBaseName() + "_outputs";
        if (!QDir(o_fpath + "/" + o_dirname).exists())
        {
            if (!QDir(o_fpath).mkdir(o_dirname))
            {
                Utility::showErrorMessage(this, u8"アウトプットをファイルに書き込めません。");
                return;
            }
        }
        o_fpath += "/" + o_dirname + "/output_loads.csv";

        QString err = UnifiedFixedData::getInstance()->getCalcLoadData()->executeCalculation(o_fpath, &progress, false);
        if (err == QString{})
        {
            Utility::showInformationMessage(this, u8"荷重拾いが完了しました。");
        }
        else
        {
            Utility::showErrorMessage(this, err);
        }
        /*
        if (UnifiedEditingData::getInstance()->currentFileBaseName().isEmpty() ) {
            Utility::showErrorMessage(this, u8"保存先ファイルが指定されていません。\nファイル保存をしてから再度実行してください。");
            return;
        }

        FixedTableData* copyData= FixedTableData::Create();
        setTableModel(copyData);
        UnifiedFixedData::setInstance(copyData);

        QProgressDialog progress(u8"荷重拾い実行中です...", u8"キャンセル", 0, 100, this );
        progress.setWindowModality( Qt::WindowModal );
        progress.setWindowTitle(u8"荷重拾い計算");
        progress.setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
        progress.setFont( QFont(tr("Microsoft JhengHei"), 9) );

        qApp->processEvents();
        progress.setValue(0);
        QDateTime s_time = QDateTime::currentDateTime();

        InputDataCheck check;
        QString err = check.executeCheck(&progress);
        if (!err.isEmpty() ){
            progress.setValue(100);
            progress.close();

            QString msg = u8"入力データにエラーがありました。\n";
            msg += err;
            Utility::showErrorMessage(this, msg);
        }
        else{
            progress.setValue(50);
        }


        CalculationLoad calc;
        err = calc.executeCalculation(&progress);
        progress.setValue(100);
        progress.close();

        QDateTime e_time = QDateTime::currentDateTime();

        qDebug() << s_time << e_time;

        if ( !err.isEmpty() ) {
            QString msg = u8"荷重拾いに失敗しました。\n";
            msg += err;
            Utility::showErrorMessage(this, msg);
        } else {
            QString msg = u8"荷重拾いが完了しました。\n";
            msg += err;
            Utility::showInformationMessage(this, msg);
        }
        */
    }

    void TopMenuWidget::slotExecuteAnalysis()
    {
        if (UnifiedEditingData::getInstance()->currentFileBaseName().isEmpty())
        {
            Utility::showErrorMessage(this, u8"保存先ファイルが指定されていません。\nファイル保存をしてから再度実行してください。");
        }

        FixedTableData *copyData = FixedTableData::Create();
        setTableModel(copyData);
        UnifiedFixedData::setInstance(copyData);

        ExecuteControlDialog dialog(this);
        dialog.exec();
    }

    void TopMenuWidget::slotStructuralStatement()
    {
        if (calculationManagerDialog->exec() == QDialog::Accepted)
        {
        }
    }

    void TopMenuWidget::slotCalculationTextDialogShow()
    {
        if (outputTextDialog->exec() == QDialog::Accepted)
        {
        }
    }
    void TopMenuWidget::slotCalculationFigureDialogShow()
    {
        if (outputFigureDialog->exec() == QDialog::Accepted)
        {
        }
    }

    void TopMenuWidget::enableSaveAsButton(bool onoff)
    {
        CBSTATUS status = (onoff) ? CBSTATUS::CB_DEFAULT : CBSTATUS::CB_DISABLE;
        saveAs_button->changeButtonColor(status);
        saveAs_button->setEnabled(onoff);
    }

    //AutoSave有効無効 miu
    void TopMenuWidget::enableAutoSaveButton(bool onoff)
    {
        autosave_button->setEnabled(onoff);
    }

    void TopMenuWidget::setTableModel(FixedTableData *copy)
    {
        copy->setTableModel(analysisFloorDialog->getTableModel(), DATATYPE::TPAFLOOR);
        copy->setTableModel(floorSettingDialog->getTableModel(), DATATYPE::TPFLOOR);
        copy->setTableModel(frameSettingDialog->getTableModel(), DATATYPE::TPFRAMEG);
        copy->setTableModel(frameSettingDialog->getSubTableModel(), DATATYPE::TPFRAMEP);
        copy->setTableModel(jloadSettingDialog->getTableModel(), DATATYPE::TPJOINTLOAD);
        copy->setTableModel(mloadSettingDialog->getTableModel(), DATATYPE::TPMEMBERLOAD);
        copy->setTableModel(sloadSettingDialog->getTableModel(), DATATYPE::TPSLABLOAD);
        copy->setTableModel(materialSettingDialog->getTableModel(), DATATYPE::TPMASTEEL);
        copy->setTableModel(baseSettingDialog->getSTableModel(), DATATYPE::TPBASE, STRUCTTYPE::STP_S);
        copy->setTableModel(baseSettingDialog->getSrcTableModel(), DATATYPE::TPBASE, STRUCTTYPE::STP_SRC);
        copy->setTableModel(beamSettingDialog->getRcTableModel(), DATATYPE::TPBEAM, STRUCTTYPE::STP_RC);
        copy->setTableModel(beamSettingDialog->getSTableModel(), DATATYPE::TPBEAM, STRUCTTYPE::STP_S);
        copy->setTableModel(beamSettingDialog->getSrcTableModel(), DATATYPE::TPBEAM, STRUCTTYPE::STP_SRC);
        copy->setTableModel(braceSettingDialog->getTableModel(), DATATYPE::TPBRACE);
        copy->setTableModel(columnSettingDialog->getRcTableModel(), DATATYPE::TPCOLUMN, STRUCTTYPE::STP_RC);
        copy->setTableModel(columnSettingDialog->getSTableModel(), DATATYPE::TPCOLUMN, STRUCTTYPE::STP_S);
        copy->setTableModel(columnSettingDialog->getSrcTableModel(), DATATYPE::TPCOLUMN, STRUCTTYPE::STP_SRC);
        copy->setTableModel(columnSettingDialog->getCftTableModel(), DATATYPE::TPCOLUMN, STRUCTTYPE::STP_CFT);
        copy->setTableModel(damperSettingDialog->getTableModel(), DATATYPE::TPDAMPER);
        copy->setTableModel(girderSettingDialog->getRcTableModel(), DATATYPE::TPGIRDER, STRUCTTYPE::STP_RC);
        copy->setTableModel(girderSettingDialog->getSTableModel(), DATATYPE::TPGIRDER, STRUCTTYPE::STP_S);
        copy->setTableModel(girderSettingDialog->getSrcTableModel(), DATATYPE::TPGIRDER, STRUCTTYPE::STP_SRC);
        copy->setTableModel(isoSettingDialog->getTableModel(), DATATYPE::TPISO);
        copy->setTableModel(openSettingDialog->getTableModel(), DATATYPE::TPWALLOPEN);
        copy->setTableModel(slabSettingDialog->getTableModel(), DATATYPE::TPSLAB);
        copy->setTableModel(wallSettingDialog->getRegularTableModel(), DATATYPE::TPWALL, STRUCTTYPE::STP_RC);
        copy->setTableModel(wallSettingDialog->getSteelTableModel(), DATATYPE::TPWALL, STRUCTTYPE::STP_S);
        copy->setTableModel(uwallSettingDialog->getUnderGroundTableModel(), DATATYPE::TPUWALL);
    }

} // namespace post3dapp

#include "outputtext_conditionwidget.h"

#include <QBoxLayout>
#include <QPushButton>

#include "abstracttext.h"
#include "app_version.h"
#include "outputtext_plaintextedit.h"

namespace post3dapp{

OutputTextConditionWidget::OutputTextConditionWidget(QWidget* parent)
    :QWidget(parent)
{
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    QVBoxLayout *lay = new QVBoxLayout();
    textEdit = new OutputTextPlainTextEdit();

    // temp
    QString text;
    /*
    text.append(TextCcs::getCommand()).append("\n");

    text.append(TextLsgir::getCommand()).append("\n");
    text.append(TextLscol::getCommand()).append("\n");
    text.append(TextLswal::getCommand()).append("\n");
    text.append(TextLsswa::getCommand()).append("\n");
    text.append(TextLsfwa::getCommand()).append("\n");
    text.append(TextLsbra::getCommand()).append("\n");
    text.append(TextLsbeam::getCommand()).append("\n");
    text.append(TextLsslab::getCommand()).append("\n");
    text.append(TextLsopen::getCommand()).append("\n");
    text.append(TextLsbs::getCommand()).append("\n");
    text.append(TextLsbsrc::getCommand()).append("\n");
    text.append(TextLsdamp::getCommand()).append("\n");
    text.append(TextLsmen::getCommand()).append("\n");
    text.append(TextLdl::getCommand()).append("\n");
    text.append(TextLdf::getCommand()).append("\n");
    text.append(TextLdm::getCommand()).append("\n");
    text.append(TextLdj::getCommand()).append("\n");
    text.append(TextEstim::getCommand()).append("\n");
    text.append(TextAnacon::getCommand()).append("\n");

    text.append(TextFloor::getCommand()).append("\n");
    text.append(TextStt::getCommand()).append("\n");
    text.append(TextMatc::getCommand()).append("\n");
    text.append(TextMatr::getCommand()).append("\n");
    text.append(TextMats::getCommand()).append("\n");
    text.append(TextAlist::getCommand()).append("\n");
    text.append(TextJnt::getCommand()).append("\n");
    text.append(TextIndex::getCommand()).append("\n");
    text.append(TextD3::getCommand()).append("\n");
    text.append(TextRfloor::getCommand()).append("\n");
    text.append(TextD4Joint::getCommand()).append("\n");
    text.append(TextMember::getCommand()).append("\n");
    text.append(TextWall::getCommand()).append("\n");
    text.append(TextZWall::getCommand()).append("\n");
    text.append(TextSlab::getCommand()).append("\n");
    text.append(TextPanel::getCommand()).append("\n");
    text.append(TextSpringList::getCommand()).append("\n");
    text.append(TextJspring::getCommand()).append("\n");
    text.append(TextRlForSpring::getCommand()).append("\n");
    text.append(TextAxialSpring::getCommand()).append("\n");
    text.append(TextShearSpring::getCommand()).append("\n");
    text.append(TextMssSpring::getCommand()).append("\n");
    text.append(TextBsSpring::getCommand()).append("\n");
    text.append(TextTorqueSpring::getCommand()).append("\n");
    text.append(TextWallSpring::getCommand()).append("\n");
    text.append(TextFrame::getCommand()).append("\n");
    text.append(TextMemberEndSpring::getCommand()).append("\n");

    text.append(TextSeismicLoad::getCommand()).append("\n");
    text.append(TextWindLoad::getCommand()).append("\n");
    text.append(TextJointLoad::getCommand()).append("\n");

    text.append(TextFrameModel::getCommand()).append("\n");
    text.append(TextQuColumn::getCommand()).append("\n");
    text.append(TextQuGirder::getCommand()).append("\n");
    text.append(TextQuWall::getCommand()).append("\n");
    text.append(TextNuBrace::getCommand()).append("\n");
    text.append(TextPnlmdl::getCommand()).append("\n");

    text.append(TextJointDisp::getCommand()).append("\n");
    text.append(TextJointIForce::getCommand()).append("\n");
    text.append(TextJointReact::getCommand()).append("\n");
    text.append(TextMemberJStress::getCommand()).append("\n");
    text.append(TextMemberFStress::getCommand()).append("\n");
    text.append(TextWallStress::getCommand()).append("\n");
    text.append(TextSlabStress::getCommand()).append("\n");
    text.append(TextPanelStress::getCommand()).append("\n");
    text.append(TextJointSpringStr::getCommand()).append("\n");
    text.append(TextAxialSpringStr::getCommand()).append("\n");
    text.append(TextShearSpringStr::getCommand()).append("\n");
    text.append(TextMssSpringStr::getCommand()).append("\n");
    text.append(TextBsSpringStr::getCommand()).append("\n");
    text.append(TextTorqueSpringStr::getCommand()).append("\n");
    text.append(TextWallSpringStr::getCommand()).append("\n");

    text.append(TextHfDistribution::getCommand()).append("\n");
    text.append(TextStoryStiffness::getCommand()).append("\n");
    text.append(TextMaximumDeform::getCommand()).append("\n");
    text.append(TextStfFactor::getCommand()).append("\n");
    text.append(TextEccFactor::getCommand()).append("\n");
    text.append(TextFrameStiffness::getCommand()).append("\n");
    text.append(TextFrDistribution::getCommand()).append("\n");

    text.append(TextFrCheckSummary::getCommand()).append("\n");
    text.append(TextCheckSummary1::getCommand()).append("\n");
    text.append(TextCheckSummary2::getCommand()).append("\n");
    text.append(TextColumnCheck::getCommand()).append("\n");
    text.append(TextGirderCheck::getCommand()).append("\n");
    text.append(TextBraceCheck::getCommand()).append("\n");
    text.append(TextWallCheck::getCommand()).append("\n");

    text.append(TextStoryPlastInfo::getCommand()).append("\n");
    text.append(TextQdCurve::getCommand()).append("\n");
    text.append(TextFrQdCurve::getCommand()).append("\n");
    text.append(TextFrDistPlast::getCommand()).append("\n");
    text.append(TextQuNeed::getCommand()).append("\n");
    text.append(TextUltimateState::getCommand()).append("\n");
    text.append(TextFrMemUlt::getCommand()).append("\n");
    text.append(TextFrRank::getCommand()).append("\n");
    text.append(TextFrMemR::getCommand()).append("\n");
    text.append(TextGirderRank::getCommand()).append("\n");
    text.append(TextColumnRank::getCommand()).append("\n");

    text.append(TextCwArea::getCommand()).append("\n");
    text.append(TextMessage::getCommand()).append("\n");
    */
    text.append("*FINALEND");

    textEdit->setPlainText(text);


    lay->addWidget(textEdit);
    runButton = new QPushButton(u8"実行");
    lay->addWidget(runButton);
    this->setLayout(lay);

}
void OutputTextConditionWidget::writeBinaryFile(QDataStream &outfile) const{
    outfile << textEdit->toPlainText();
}
void OutputTextConditionWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    QString temp;
    infile >> temp;
    textEdit->setPlainText(temp);
}

} // namespace post3dapp

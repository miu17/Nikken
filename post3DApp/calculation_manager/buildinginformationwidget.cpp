#include "buildinginformationwidget.h"

#include <QLabel>
#include <QLayout>
#include "app_version.h"

namespace post3dapp {

BuildingInformationWidget::BuildingInformationWidget(QWidget *parent)
    :QWidget(parent)
{
    QHBoxLayout *hlay1 = new QHBoxLayout();
    hlay1->addWidget(new QLabel(u8"建物名称"));
    name01 = new QLineEdit("");
    hlay1->addWidget(name01);

    QHBoxLayout *hlay2 = new QHBoxLayout();
    hlay2->addWidget(new QLabel(u8"建物場所"));
    place02 = new QLineEdit("");
    hlay2->addWidget(place02);

    QHBoxLayout *hlay3 = new QHBoxLayout();
    hlay3->addWidget(new QLabel(u8"主要用途"));
    use03 = new QLineEdit("");
    hlay3->addWidget(use03);

    QHBoxLayout *hlay4 = new QHBoxLayout();
    hlay4->addWidget(new QLabel(u8"工事種別"));
    category04 = new QLineEdit("");
    hlay4->addWidget(category04);

    QHBoxLayout *hlay5 = new QHBoxLayout();
    hlay5->addWidget(new QLabel(u8"増築予定"));
    future05 = new QLineEdit("");
    hlay5->addWidget(future05);

    QHBoxLayout *hlay6 = new QHBoxLayout();
    hlay6->addWidget(new QLabel(u8"延床面積"));
    totalarea06 = new QLineEdit("");
    hlay6->addWidget(totalarea06);

    QHBoxLayout *hlay7 = new QHBoxLayout();
    hlay7->addWidget(new QLabel(u8"建物面積"));
    buildingarea07 = new QLineEdit("");
    hlay7->addWidget(buildingarea07);

    QHBoxLayout *hlay8 = new QHBoxLayout();
    hlay8->addWidget(new QLabel(u8"階数・地下"));
    undergroundfloor08 = new QLineEdit("");
    hlay8->addWidget(undergroundfloor08);

    QHBoxLayout *hlay9 = new QHBoxLayout();
    hlay9->addWidget(new QLabel(u8"階数・地上"));
    floor09 = new QLineEdit("");
    hlay9->addWidget(floor09);

    QHBoxLayout *hlay10 = new QHBoxLayout();
    hlay10->addWidget(new QLabel(u8"階数・塔屋"));
    roof10 = new QLineEdit("");
    hlay10->addWidget(roof10);

    QHBoxLayout *hlay11 = new QHBoxLayout();
    hlay11->addWidget(new QLabel(u8"建物高さ"));
    height11 = new QLineEdit("");
    hlay11->addWidget(height11);

    QHBoxLayout *hlay12 = new QHBoxLayout();
    hlay12->addWidget(new QLabel(u8"軒の高さ"));
    eavesheight12 = new QLineEdit("");
    hlay12->addWidget(eavesheight12);

    QHBoxLayout *hlay13 = new QHBoxLayout();
    hlay13->addWidget(new QLabel(u8"構造種別"));
    structureX13 = new QLineEdit("");
    hlay13->addWidget(structureX13);

    QHBoxLayout *hlay14 = new QHBoxLayout();
    hlay14->addWidget(new QLabel(u8"骨組形式・X方向"));
    frameX14 = new QLineEdit("");
    hlay14->addWidget(frameX14);

    QHBoxLayout *hlay15 = new QHBoxLayout();
    hlay15->addWidget(new QLabel(u8"骨組形式・Y方向"));
    frameY15 = new QLineEdit("");
    hlay15->addWidget(frameY15);

    QHBoxLayout *hlay16 = new QHBoxLayout();
    hlay16->addWidget(new QLabel(u8"主スパン・X方向"));
    spanX16 = new QLineEdit("");
    hlay16->addWidget(spanX16);

    QHBoxLayout *hlay17 = new QHBoxLayout();
    hlay17->addWidget(new QLabel(u8"主スパン・Y方向"));
    spanY17 = new QLineEdit("");
    hlay17->addWidget(spanY17);

    QHBoxLayout *hlay18 = new QHBoxLayout();
    hlay18->addWidget(new QLabel(u8"基礎形式"));
    foundation18 = new QLineEdit("");
    hlay18->addWidget(foundation18);

    QHBoxLayout *hlay19 = new QHBoxLayout();
    hlay19->addWidget(new QLabel(u8"仕上げ等・床"));
    finishslab19 = new QLineEdit("");
    hlay19->addWidget(finishslab19);

    QHBoxLayout *hlay20 = new QHBoxLayout();
    hlay20->addWidget(new QLabel(u8"仕上げ等・屋根"));
    finishroof20 = new QLineEdit("");
    hlay20->addWidget(finishroof20);

    QHBoxLayout *hlay21 = new QHBoxLayout();
    hlay21->addWidget(new QLabel(u8"仕上げ等・外壁"));
    finishwall21 = new QLineEdit("");
    hlay21->addWidget(finishwall21);

    QHBoxLayout *hlay22 = new QHBoxLayout();
    hlay22->addWidget(new QLabel(u8"屋上付属物等"));
    accessories22 = new QLineEdit("");
    hlay22->addWidget(accessories22);



    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addLayout(hlay1);
    vlay->addLayout(hlay2);
    vlay->addLayout(hlay3);
    vlay->addLayout(hlay4);
    vlay->addLayout(hlay5);
    vlay->addLayout(hlay6);
    vlay->addLayout(hlay7);
    vlay->addLayout(hlay8);
    vlay->addLayout(hlay9);
    vlay->addLayout(hlay10);
    vlay->addLayout(hlay11);
    vlay->addLayout(hlay12);
    vlay->addLayout(hlay13);
    vlay->addLayout(hlay14);
    vlay->addLayout(hlay15);
    vlay->addLayout(hlay16);
    vlay->addLayout(hlay17);
    vlay->addLayout(hlay18);
    vlay->addLayout(hlay19);
    vlay->addLayout(hlay20);
    vlay->addLayout(hlay21);
    vlay->addLayout(hlay22);
    this->setLayout(vlay);

}

void BuildingInformationWidget::writeBinaryFile(QDataStream &outfile) const{
    outfile << name01->text();
    outfile << place02->text();
    outfile << use03->text();
    outfile << category04->text();
    outfile << future05->text();
    outfile << totalarea06->text();
    outfile << buildingarea07->text();
    outfile << undergroundfloor08->text();
    outfile << floor09->text();
    outfile << roof10->text();
    outfile << height11->text();
    outfile << eavesheight12->text();
    outfile << structureX13->text();
    outfile << frameX14->text();
    outfile << frameY15->text();
    outfile << spanX16->text();
    outfile << spanY17->text();
    outfile << foundation18->text();
    outfile << finishslab19->text();
    outfile << finishroof20->text();
    outfile << finishwall21->text();
    outfile << accessories22->text();
}
void BuildingInformationWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    QString temp;
    infile >> temp;
    name01->setText(temp);
    infile >> temp;
    place02->setText(temp);
    infile >> temp;
    use03->setText(temp);
    infile >> temp;
    category04->setText(temp);
    infile >> temp;
    future05->setText(temp);
    infile >> temp;
    totalarea06->setText(temp);
    infile >> temp;
    buildingarea07->setText(temp);
    infile >> temp;
    undergroundfloor08->setText(temp);
    infile >> temp;
    floor09->setText(temp);
    infile >> temp;
    roof10->setText(temp);
    infile >> temp;
    height11->setText(temp);
    infile >> temp;
    eavesheight12->setText(temp);
    infile >> temp;
    structureX13->setText(temp);
    infile >> temp;
    frameX14->setText(temp);
    infile >> temp;
    frameY15->setText(temp);
    infile >> temp;
    spanX16->setText(temp);
    infile >> temp;
    spanY17->setText(temp);
    infile >> temp;
    foundation18->setText(temp);
    infile >> temp;
    finishslab19->setText(temp);
    infile >> temp;
    finishroof20->setText(temp);
    infile >> temp;
    finishwall21->setText(temp);
    infile >> temp;
    accessories22->setText(temp);
}

} // namespace post3dapp

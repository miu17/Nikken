#ifndef DEFINE_TEXTINPUT_H
#define DEFINE_TEXTINPUT_H

#include "define_customtable.h"


/* 節点テーブル */

class JointTableForm : public CustomTableDataType
{

public:

    JointTableForm(){

        COLUMNS=7;
        uuidCol=6;

        CREGULATION column_regulation[7]={
            /* 0*/  { "No.", TB_STRING, QVariant(), -1, "NO" },
            /* 1*/  { "X" + QObject::tr("Coordinate") + "(m)", TB_DOUBLE, QVariant("0.0"), -1, "X" },
            /* 2*/  { "Y" + QObject::tr("Coordinate") + "(m)", TB_DOUBLE, QVariant("0.0"), -1, "Y" },
            /* 3*/  { "Z" + QObject::tr("Coordinate") + "(m)", TB_DOUBLE, QVariant("0.0"), -1, "Z" },
            /* 4*/  { QObject::tr("Affiliation Floor"), TB_STRING, QVariant(), -1, "FLOORS" },
            /* 5*/  { QObject::tr("Affiliation grid"), TB_STRING, QVariant(), -1, "FRAMES" },
            /* 6*/  { "UUID", TB_STRING, QVariant(), -1, "UUID" }
         };

        populateList(column_regulation);
    }

};

/* 部材テーブル */

class MemberTableForm : public CustomTableDataType
{

public:

    MemberTableForm(){

        COLUMNS=15;
        uuidCol=14;

        CREGULATION column_regulation[15]={
            /* 0*/  { "No.", TB_STRING, QVariant(), -1, "NO" },
            /* 1*/  { QObject::tr("Member type"), TB_STRING, QVariant(), -1, "TYPE" },
            /* 2*/  { QObject::tr("Member code"), TB_STRING, QVariant(), -1, "NAME" },
            /* 3*/  { QObject::tr("Opening mark"), TB_STRING, QVariant(), -1, "WALLOPEN" },
            /* 4*/  { QObject::tr("Node composition"), TB_STRING, QVariant(), -1, "JOINTS" },
            /* 5*/  { QObject::tr("Affiliation floor") + "\n(i" + QObject::tr("end") + ")", TB_STRING, QVariant(), -1, "FLOORS" },
            /* 6*/  { QObject::tr("Affiliation grid") + "\n(" + QObject::tr("All nodes")+ ")", TB_STRING, QVariant(), -1, "FRAMES" },
            /* 7*/  { QObject::tr("Cord Angle") + "\n(" + QObject::tr("Degree") + ")", TB_DOUBLE, QVariant("0.0"), -1, "ANGLE" },
            /* 8*/  { QObject::tr("Face rule") + "\ni" + QObject::tr("end") + ")", TB_STRING, QVariant(QObject::tr("Automatic")), -1, "IFACERULE" },
            /* 9*/  { QObject::tr("Face length") + "\ni" + QObject::tr("end")+ " (mm)", TB_DOUBLE, QVariant("0.0"), -1, "IFACELEN" },
            /*10*/  { QObject::tr("Face rule") + "\nj" + QObject::tr("end") + ")", TB_STRING, QVariant(QObject::tr("Automatic")), -1, "JFACERULE" },
            /*11*/  { QObject::tr("Face length") + "\nj" + QObject::tr("end") +" (mm)", TB_DOUBLE, QVariant("0.0"), -1, "JFACELEN" },
            /*12*/  { QObject::tr("Close length") + "\ny (mm)", TB_DOUBLE, QVariant("0.0"), -1, "SHIFTY" },
            /*13*/  { QObject::tr("Close length") + "\nz (mm)", TB_DOUBLE, QVariant("0.0"), -1, "SHIFTZ" },
            /*14*/  { "UUID", TB_STRING, QVariant(), -1, "UUID" }
        };

        populateList(column_regulation);
        disableColumns << taggedColumnNumber["FLOORS"] << taggedColumnNumber["FRAMES"];

        QStringList element_list;
        element_list << QObject::tr("Column") << QObject::tr("Girder") << QObject::tr("Brace")
                     << QObject::tr("Beam") << QObject::tr("Slab") << QObject::tr("Wall")
                     << QObject::tr("Foundation wall") << QObject::tr("Damping element") << QObject::tr("Seismic isolation element");
        limitedStrings.insert(taggedColumnNumber["TYPE"],element_list);

        QStringList face_rule;
        face_rule << QObject::tr("auto") << QObject::tr("Value specification");
        limitedStrings.insert(taggedColumnNumber["IFACERULE"],face_rule);
        limitedStrings.insert(taggedColumnNumber["JFACERULE"],face_rule);

    }
};


/* 荷重テーブル */

class LoadTableForm : public CustomTableDataType
{

public:

    LoadTableForm(){

        COLUMNS=7;
        uuidCol=6;

        CREGULATION column_regulation[7]={
            /* 0*/  { "No.", TB_STRING, QVariant(), -1, "NO" },
            /* 1*/  { QObject::tr("Load type"), TB_STRING, QVariant(), -1, "TYPE" },
            /* 2*/  { QObject::tr("Load sign"), TB_STRING, QVariant(), -1, "NAME" },
            /* 3*/  { QObject::tr("Node composition"), TB_STRING, QVariant(), -1, "JOINTS" },
            /* 4*/  { QObject::tr("Affiliation Floor") + "\n" + QObject::tr("(i end)"), TB_STRING, QVariant(), -1, "FLOORS" },
            /* 5*/  { QObject::tr("Affiliation Grid") + "\n" + QObject::tr("(All nodes)"), TB_STRING, QVariant(), -1, "FRAMES" },
            /* 6*/  { "UUID", TB_STRING, QVariant(), -1, "UUID" }
         };

        populateList(column_regulation);

        disableColumns << taggedColumnNumber["FLOORS"] << taggedColumnNumber["FRAMES"];

        QStringList element_list;
        element_list << QObject::tr("Joint load") << QObject::tr("Member load") << QObject::tr("Floor load");
        limitedStrings.insert(taggedColumnNumber["TYPE"],element_list);

    }


};


#endif

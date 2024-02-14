#include <QtGui>
#include <QMessageBox>

#include "analysisrange_tablemodel.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_inputdata.h"
namespace post3dapp{

AnalysisRangeTableModel::AnalysisRangeTableModel(const CustomTableDataType &type, QObject *parent)
    : CustomTableModel(type,parent)
{

}

AnalysisJointRangeTableModel::AnalysisJointRangeTableModel(const CustomTableDataType &type, QObject *parent)
    : AnalysisRangeTableModel(type,parent)
{
}

QVariant AnalysisJointRangeTableModel::data(const QModelIndex &index, int role) const
{
    int row=index.row();
    int col=index.column();

    if( col == myType.getTaggedColumn("JOINTS")){
        auto vlist=allItems.at(row, myType.getTaggedColumn("JOINTS"));
        QList<QVariant> uuidList = vlist.toList();
        if( role == Qt::DisplayRole )
            return QString(u8"計%1個 選択").arg(uuidList.count());
        if (role == Qt::TextColorRole ){
            // Uuidがあるかどうか
            bool hasUuid = true;
            foreach(auto u, uuidList){
                auto l = u.toUuid();
                if (!UnifiedInputData::getInInstance()->isExistedID(l, DATATYPE::TPJOINT)){
                    hasUuid = false;
                    break;
                }
            }
            if (hasUuid){
                return QColor(30,30,30);
            }else{
                return QColor(Qt::red);
            }
        }
    }
    if( col == myType.getTaggedColumn("JOINTNUMBERS")){
        auto vlist=allItems.at(row, myType.getTaggedColumn("JOINTS"));
        if( role == Qt::DisplayRole ){
            return getJointNumbersString(vlist.toList());
        }

        if (role == Qt::TextColorRole)
            return QColor(30, 30, 30);
    }
    return CustomTableModel::data(index, role);
}

bool AnalysisJointRangeTableModel::isDisableItem(const QModelIndex &index) const
{
    return ( index.column() == 0 ||
             index.column() == myType.getTaggedColumn("JOINTS") ||
             index.column() == myType.getTaggedColumn("JOINTNUMBERS"));
}

QList<GroupSetting> AnalysisJointRangeTableModel::getData() const
{
    QList<GroupSetting> list;
    for (int i = 0; i < rowCount(); i++){
        QList<QVariant> rlist = allItems.getRowData(i);
        QString name = rlist.at(myType.getTaggedColumn("NAME")).toString();
        QList<QVariant> vList= rlist.at(myType.getTaggedColumn("JOINTS")).toList();
        QList<QUuid> uList;
        for (auto v : vList){
            uList.append(v.toUuid());
        }
        QString comment = rlist.at(myType.getTaggedColumn("COMMENT")).toString();
        list.append(GroupSetting(name, uList, comment));
    }
    return list;
}

void AnalysisJointRangeTableModel::modelRewind(const QList<GroupSetting> &list)
{
    this->removeRows(0, this->rowCount());

    this->setRowCount(list.count());
    for (int i = 0; i < list.count(); i++){
        this->setDataDirect(this->index(i, 1), QVariant(list.at(i).group), false);
        QList<QVariant> vList;
        for (auto l : list.at(i).list){
            vList.append(QVariant(l));
        }
        this->setDataDirect(this->index(i, 2), QVariant(vList), false);
        this->setDataDirect(this->index(i, 3), QVariant(getJointNumbersString(vList)), false);
        this->setDataDirect(this->index(i, 4), QVariant(list.at(i).comment), false);
    }
}

QString AnalysisJointRangeTableModel::getJointNumbersString(const QList<QVariant> &list) const
{
    QList<QUuid> uList;
    for (auto v : list){
        uList.append(v.toUuid());
    }
    QList<JointData*> idList = UnifiedInputData::getInInstance()->jointIDsToPointers(uList);
    int nUnknown = 0;
    QString ret;
    for (auto l : idList){
        if (l == nullptr){
            nUnknown++;
            continue;
        }
        ret += QString::number(l->getJointNo(UnifiedDataType::Input)) + ",";
    }
    if (nUnknown > 0){
        ret += u8"不明：" + QString::number(nUnknown) + u8"個";
    }

    return ret;
}

AnalysisMemberRangeTableModel::AnalysisMemberRangeTableModel(const CustomTableDataType &type, QObject *parent)
    : AnalysisRangeTableModel(type,parent)
{

}

QVariant AnalysisMemberRangeTableModel::data(const QModelIndex &index, int role) const
{
    int row=index.row();
    int col=index.column();

    if( col == myType.getTaggedColumn("MEMBERS")){
        auto vlist=allItems.at(row, myType.getTaggedColumn("MEMBERS"));
        QList<QVariant> vList = vlist.toList();
        if( role == Qt::DisplayRole )
            return QString(u8"計%1個 選択").arg(vList.count());
        if (role == Qt::TextColorRole ){
            // Uuidがあるかどうか
            bool hasUuid = true;
            foreach(auto v, vList){
                if (!UnifiedInputData::getInInstance()->isExistedID(v.toUuid(), DATATYPE::TPCOLUMN)){
                    hasUuid = false;
                    break;
                }
            }
            if (hasUuid){
                return QColor(30,30,30);
            }else{
                return QColor(Qt::red);
            }
        }
    }
    if( col == myType.getTaggedColumn("DATATYPE")){
        auto vlist=allItems.at(row, myType.getTaggedColumn("MEMBERS"));
        QList<QVariant> vList = vlist.toList();
        if( role == Qt::DisplayRole ){
            return getDataTypeString(vList);
        }

        if (role == Qt::TextColorRole)
            return QColor(30, 30, 30);
    }
    return CustomTableModel::data(index, role);

}

QList<GroupSetting> AnalysisMemberRangeTableModel::getData() const
{
    QList<GroupSetting> list;
    for (int i = 0; i < rowCount(); i++){
        QList<QVariant> rlist = allItems.getRowData(i);
        QString name = rlist.at(myType.getTaggedColumn("NAME")).toString();
        QList<QVariant> vList= rlist.at(myType.getTaggedColumn("MEMBERS")).toList();
        QList<QUuid> uList;
        for (auto v : vList){
            uList.append(v.toUuid());
        }
        QString comment = rlist.at(myType.getTaggedColumn("COMMENT")).toString();
        list.append(GroupSetting(name, uList, comment));
    }
    return list;
}

void AnalysisMemberRangeTableModel::modelRewind(const QList<GroupSetting> &list)
{
    this->removeRows(0, this->rowCount());

    this->setRowCount(list.count());
    for (int i = 0; i < list.count(); i++){
        this->setDataDirect(this->index(i, 1), QVariant(list.at(i).group), false);
        QList<QVariant> vList;
        for (auto l : list.at(i).list){
            vList.append(QVariant(l));
        }
        this->setDataDirect(this->index(i, 2), QVariant(vList), false);
        this->setDataDirect(this->index(i, 3), QVariant(getDataTypeString(vList)), false);
        this->setDataDirect(this->index(i, 4), QVariant(list.at(i).comment), false);
    }
}

QString AnalysisMemberRangeTableModel::getDataTypeString(const QList<QVariant> &list) const
{
    QPair<QString, int> nColumn{u8"柱", 0};
    QPair<QString, int> nGirder{u8"大梁", 0};
    QPair<QString, int> nBrace{u8"ブレース", 0};
    QPair<QString, int> nSlab{u8"スラブ", 0};
    QPair<QString, int> nWall{u8"壁", 0};
    QPair<QString, int> nFoundationWall{u8"地下外壁", 0};
    QPair<QString, int> nDamper{u8"ダンパー", 0};
    QPair<QString, int> nIsolator{u8"アイソレータ", 0};
    QPair<QString, int> nUnknown{u8"不明", 0};
    QList<QUuid> uList;
    for (auto l : list){
        uList.append(l.toUuid());
    }
    QList<MemberData*> idList = UnifiedInputData::getInInstance()->memberIDsToPointers(uList);
    for (auto l : idList){
        if (l == nullptr){
            nUnknown.second++;
            continue;
        }

        switch(l->dataType()){
        case DATATYPE::TPCOLUMN:
            nColumn.second++;
            break;
        case DATATYPE::TPGIRDER:
            nGirder.second++;
            break;
        case DATATYPE::TPBRACE:
            nBrace.second++;
            break;
        case DATATYPE::TPSLAB:
            nSlab.second++;
            break;
        case DATATYPE::TPWALL:
            nWall.second++;
            break;
        case DATATYPE::TPUWALL:
            nFoundationWall.second++;
            break;
        case DATATYPE::TPDAMPER:
            nDamper.second++;
            break;
        case DATATYPE::TPISO:
            nIsolator.second++;
            break;
        default:
            throw;
        }
    }

    QString ret;
    QList<QPair<QString, int>> nList{nColumn, nGirder, nBrace, nSlab, nWall, nFoundationWall, nDamper, nIsolator, nUnknown};
    for(auto l : nList){
        if (l.second > 0){
            ret += l.first + ":" + QString::number(l.second) + u8"個,";
        }
    }
    return ret;
}

bool AnalysisMemberRangeTableModel::isDisableItem(const QModelIndex &index) const
{
    return ( index.column() == 0 ||
             index.column() == myType.getTaggedColumn("MEMBERS") ||
             index.column() == myType.getTaggedColumn("DATATYPE"));
}

} // namespace post3dapp

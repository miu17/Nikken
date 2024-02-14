#include "sortfilter_proxymodel.h"
#include <QtGui>


namespace post3dapp{
MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow,
                                              const QModelIndex &sourceParent) const
{
    QModelIndex index1 = sourceModel()->index(sourceRow, filterColumn, sourceParent);
    return sourceModel()->data(index1).toString().contains(filterRegExp());
}

bool MySortFilterProxyModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left, Qt::EditRole);
    QVariant rightData = sourceModel()->data(right, Qt::EditRole);

    if (leftData.type() == QVariant::Int) {
        return leftData.toInt() < rightData.toInt();
    } else if (leftData.type() == QVariant::Double) {
        return leftData.toDouble() < rightData.toDouble();
    } else {
        /*
                QRegExp *pattern = new QRegExp("([\\w\\.]*@[\\w\\.]*)");

                QString leftString = leftData.toString();
                if(left.column() == 1 && pattern->indexIn(leftString) != -1)
                    leftString = pattern->cap(1);

                QString rightString = rightData.toString();
                if(right.column() == 1 && pattern->indexIn(rightString) != -1)
                    rightString = pattern->cap(1);
        */
        QRegExp pattern("([\\w\\.]*@[\\w\\.]*)");

        QString leftString = leftData.toString();
        if (left.column() == 1 && pattern.indexIn(leftString) != -1)
            leftString = pattern.cap(1);

        QString rightString = rightData.toString();
        if (right.column() == 1 && pattern.indexIn(rightString) != -1)
            rightString = pattern.cap(1);

        return QString::localeAwareCompare(leftString, rightString) < 0;
    }
}
} // namespace post3dapp


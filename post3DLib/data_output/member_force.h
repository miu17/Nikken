#ifndef MEMBER_FORCE_H
#define MEMBER_FORCE_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class POST3D_CLASS_DLLSPEC MemberForceByIL
{
    //中間荷重　Intermediate Loadによって生じた応力（x = pos）
public:
    MemberForceByIL(){}
    MemberForceByIL(const QVector<qreal>& in){
        //force = calcAnyPointStress::zeroList1D(6);
        force.clear();
        for(int i= 0;i<in.count();i++){

            force << in.at(i);

        }
        //force = in;
    }
    qreal getN()const
    {
        return force.at(0);
    }
    qreal getQy()const
    {
        return force.at(1);
    }
    qreal getQz()const
    {
        return force.at(2);
    }
    qreal getMx()const
    {
        return force.at(3);
    }
    qreal getMy()const
    {
        return force.at(4);
    }
    qreal getMz()const
    {
        return force.at(5);
    }
private:
    QList<qreal> force;
};


class POST3D_CLASS_DLLSPEC MemberForce
{

    //部材端応力
    public:
        MemberForce( qreal _iMy = 0.0, qreal _iMz = 0.0, qreal _cMy = 0.0, qreal _cMz = 0.0,
                     qreal _jMy = 0.0, qreal _jMz = 0.0, qreal _iQy = 0.0, qreal _iQz = 0.0,
                     qreal _jQy = 0.0, qreal _jQz = 0.0,
                     qreal _N = 0.0, qreal _iMx = 0.0, qreal _jMx = 0.0 )
            : iMy(_iMy), iMz(_iMz), cMy(_cMy), cMz(_cMz), jMy(_jMy),
              jMz(_jMz), iQy(_iQy), iQz(_iQz), jQy(_jQy), jQz(_jQz), N(_N), iMx(_iMx), jMx(_jMx) { }
        qreal iMy;
        qreal iMz;
        qreal cMy;
        qreal cMz;
        qreal jMy;
        qreal jMz;
        qreal iQy;
        qreal iQz;
        qreal jQy;
        qreal jQz;
        qreal N;
        qreal iMx;
        qreal jMx;

        qreal getMy(qreal xPos)const
        {
            return calcParabora(iMy, cMy, jMy, xPos);   //0<=xPos<=1
        }
        qreal getMz(qreal xPos)const
        {
            return calcParabora(iMz, cMz, jMz, xPos);
        }
        qreal getQy(qreal xPos)const
        {
            return (1.0 - xPos) * iQy + xPos * jQy;   //0<=xPos<=1
        }
        qreal getQz(qreal xPos)const
        {
            return (1.0 - xPos) * iQz + xPos * jQz;
        }
        qreal getMx(qreal xPos)const
        {
            return (1.0 - xPos) * iMx + xPos * jMx;   //0<=xPos<=1
        }
        MemberForce operator+(const MemberForce &other) const
        {
            return MemberForce(iMy + other.iMy, iMz + other.iMz, cMy + other.cMy,
                               cMz + other.cMz, jMy + other.jMy, jMz + other.jMz,
                               iQy + other.iQy, iQz + other.iQz, jQy + other.jQy,
                               jQz + other.jQz, N + other.N, iMx + other.iMx, jMx + other.jMx);
        }
        MemberForce operator*(const qreal &coef) const
        {
            return MemberForce(iMy * coef, iMz * coef, cMy * coef,
                               cMz * coef, jMy * coef, jMz * coef,
                               iQy * coef, iQz * coef, jQy * coef,
                               jQz * coef, N * coef, iMx * coef, jMx * coef);
        }

    private:
        qreal calcParabora(const qreal &y0, const qreal &y1, const qreal &y2, const qreal &x)const
        {
            //二次関数近似
            //y0->i端の値,y1->中央の値,y2->j端の値
            qreal a = 2 * y0 + 2 * y2 - 4 * y1;
            qreal b = -3 * y0 + 4 * y1 - y2;
            qreal c = y0;
            return a * x * x + b * x + c;
        }

};

} // namespace post3dapp
#endif // MEMBER_FORCE_H

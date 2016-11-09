#include "qpointfanimator.h"
#include "boxeslist.h"

QPointFAnimator::QPointFAnimator() : ComplexAnimator()
{
    mXAnimator.setName("x");
    mYAnimator.setName("y");
    addChildAnimator(&mXAnimator);
    addChildAnimator(&mYAnimator);

    mXAnimator.blockPointer();
    mYAnimator.blockPointer();
}

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
int QPointFAnimator::saveToSql() {
    int xAnimatorId = mXAnimator.saveToSql();
    int yAnimatorId = mYAnimator.saveToSql();
    QSqlQuery query;
    if(!query.exec(
        QString("INSERT INTO qpointfanimator (xanimatorid, yanimatorid ) "
                "VALUES (%1, %2)").
                arg(xAnimatorId).
                arg(yAnimatorId) ) ) {
        qDebug() << query.lastError() << endl << query.lastQuery();
    }

    return query.lastInsertId().toInt();
}

#include <QSqlRecord>
void QPointFAnimator::loadFromSql(int posAnimatorId) {
    QSqlQuery query;

    QString queryStr = "SELECT * FROM qpointfanimator WHERE id = " +
            QString::number(posAnimatorId);
    if(query.exec(queryStr)) {
        query.next();
        int idxanimator = query.record().indexOf("xanimatorid");
        int idyanimator = query.record().indexOf("yanimatorid");

        mXAnimator.loadFromSql(query.value(idxanimator).toInt() );
        mYAnimator.loadFromSql(query.value(idyanimator).toInt() );
    } else {
        qDebug() << "Could not load qpointfanimator with id " << posAnimatorId;
    }
}

QPointF QPointFAnimator::getCurrentValue()
{
    return QPointF(mXAnimator.getCurrentValue(), mYAnimator.getCurrentValue());
}

QPointF QPointFAnimator::getCurrentValueWithNoise() const
{
    return QPointF(mXAnimator.getCurrentValueWithNoise(),
                   mYAnimator.getCurrentValueWithNoise());
}

qreal QPointFAnimator::getXValue()
{
    return mXAnimator.getCurrentValue();
}

qreal QPointFAnimator::getXValueWithNoise()
{
    return mXAnimator.getCurrentValueWithNoise();
}

qreal QPointFAnimator::getYValue()
{
    return mYAnimator.getCurrentValue();
}

qreal QPointFAnimator::getYValueWithNoise()
{
    return mYAnimator.getCurrentValueWithNoise();
}

void QPointFAnimator::setCurrentValue(QPointF val, bool finish)
{
    startNewUndoRedoSet();

    mXAnimator.setCurrentValue(val.x(), finish);
    mYAnimator.setCurrentValue(val.y(), finish);

    finishUndoRedoSet();
}

void QPointFAnimator::incCurrentValue(qreal x, qreal y)
{
    mXAnimator.incCurrentValue(x);
    mYAnimator.incCurrentValue(y);
}

void QPointFAnimator::incAllValues(qreal x, qreal y) {
    mXAnimator.incAllValues(x);
    mYAnimator.incAllValues(y);
}

void QPointFAnimator::incSavedValueToCurrentValue(qreal incXBy, qreal incYBy) {
    mXAnimator.incSavedValueToCurrentValue(incXBy);
    mYAnimator.incSavedValueToCurrentValue(incYBy);
}

QrealAnimator *QPointFAnimator::getXAnimator()
{
    return &mXAnimator;
}

QrealAnimator *QPointFAnimator::getYAnimator()
{
    return &mYAnimator;
}

void QPointFAnimator::multSavedValueToCurrentValue(qreal sx, qreal sy)
{
    mXAnimator.multSavedValueToCurrentValue(sx);
    mYAnimator.multSavedValueToCurrentValue(sy);
}

void QPointFAnimator::multCurrentValue(qreal sx, qreal sy)
{
    mXAnimator.multCurrentValue(sx);
    mYAnimator.multCurrentValue(sy);
}

QPointF QPointFAnimator::getSavedValue()
{
    return QPointF(mXAnimator.getSavedValue(), mYAnimator.getSavedValue() );
}

qreal QPointFAnimator::getSavedXValue()
{
    return mXAnimator.getSavedValue();
}

qreal QPointFAnimator::getSavedYValue()
{
    return mYAnimator.getSavedValue();
}

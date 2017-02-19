#ifndef QREALKEY_H
#define QREALKEY_H
#include "qrealpoint.h"
#include "pointhelpers.h"

class ComplexAnimator;

class ComplexKey;

class QrealAnimator;
class KeysClipboardContainer;

class QrealKey : public SmartPointerTarget
{
public:
    QrealKey(QrealAnimator *parentAnimator);
//    QrealPoint *mousePress(qreal frameT, qreal valueT,
//                    qreal pixelsPerFrame, qreal pixelsPerValue);
    virtual ~QrealKey();

    virtual QrealKey *makeQrealKeyDuplicate(QrealAnimator *targetParent);

    void updateCtrlFromCtrl(QrealPointType type);

    virtual qreal getValue();
    virtual void setValue(qreal value, bool saveUndoRedo = true);
    virtual void setStartValue(qreal value);
    virtual void setEndValue(qreal value);

    virtual void startFrameTransform();
    virtual void finishFrameTransform();

    int getFrame();
    virtual void setFrame(int frame);
    virtual void setStartFrame(qreal startFrame);
    virtual void setEndFrame(qreal endFrame);

    qreal getStartValue();
    qreal getEndValue();
    qreal getStartValueFrame();
    qreal getEndValueFrame();

    virtual void setStartEnabled(bool bT);

    virtual void setEndEnabled(bool bT);

    bool isInsideRect(QRectF valueFrameRect);

    void drawGraphKey(QPainter *p,
              qreal minFrameT, qreal minValueT,
              qreal pixelsPerFrame, qreal pixelsPerValue);
    void changeFrameAndValueBy(QPointF frameValueChange);
    void saveCurrentFrameAndValue();
    virtual void setCtrlsMode(CtrlsMode mode);

    void constrainStartCtrlMinFrame(int minFrame);
    void constrainEndCtrlMaxFrame(int maxFrame);

    QrealPoint *getStartPoint();
    QrealPoint *getEndPoint();

    bool isEndPointEnabled();
    bool isStartPointEnabled();

    qreal getPrevKeyValue();
    qreal getNextKeyValue();

    bool hasPrevKey();
    bool hasNextKey();
    void incFrameAndUpdateParentAnimator(int inc);

    QrealAnimator *getParentAnimator();

    virtual void mergeWith(QrealKey *key) { key->removeFromAnimator(); }
    void incValue(qreal incBy);

    virtual bool isDescendantSelected() { return isSelected(); }

    void removeFromAnimator();

    virtual void deleteKey() { removeFromAnimator(); }

    void setParentKey(ComplexKey *parentKey);

    bool isAncestorSelected();

    CtrlsMode getCtrlsMode();
    int saveToSql(int parentAnimatorSqlId);
    void loadFromSql(int keyId);

    virtual void copyToContainer(KeysClipboardContainer *container);
    virtual void cancelFrameTransform();
    virtual void scaleFrameAndUpdateParentAnimator(const int &relativeToFrame,
                                                   const qreal &scaleFactor);
    virtual void setSelected(bool bT);
    bool isSelected();

    bool hasParentKey() {
        return mParentKey != NULL;
    }

    virtual bool areAllChildrenSelected() {
        return false;
    }

    virtual void addToSelection(QList<QrealKey *> *selectedKeys);
    virtual void removeFromSelection(QList<QrealKey *> *selectedKeys);

    bool isHovered() {
        return mHovered;
    }

    void setHovered(const bool &bT) {
        mHovered = bT;
    }

protected:
    bool mIsSelected = false;
    QrealAnimator *mParentAnimator = NULL;
    ComplexKey *mParentKey = NULL;

    CtrlsMode mCtrlsMode = CTRLS_SYMMETRIC;

    QrealPoint *mStartPoint;
    QrealPoint *mEndPoint;

    qreal mValue;
    int mFrame;
    int mSavedFrame;
    qreal mSavedValue;

    qreal mSavedMaxStartFrameDist;
    qreal mSavedMaxEndFrameDist;

    qreal mStartValue = 0.;
    qreal mEndValue = 0.;
    qreal mStartFrame = 0.;
    qreal mEndFrame = 0.;
    bool mStartEnabled = false;
    bool mEndEnabled = false;
    bool mHovered = false;
};

struct QrealKeyPair {
    QrealKeyPair(QrealKey *key1T, QrealKey *key2T) {
        key1 = key1T;
        key2 = key2T;
    }

    void merge() {
        key1->mergeWith(key2);
    }

    QrealKey *key1;
    QrealKey *key2;
};

#endif // QREALKEY_H

#ifndef PARTICLEBOX_H
#define PARTICLEBOX_H
#include "boundingbox.h"

class ParticleBox;

struct ParticleState {
    ParticleState() {}

    ParticleState(const QPointF &posT,
              const qreal &scaleT,
              const qreal &sizeT,
              const qreal &opacityT,
              const SkPath &path) {
        pos = posT;
        size = scaleT*sizeT;
        opacity = opacityT;

        linePath = path;
    }

    void drawSk(SkCanvas *canvas,
                const SkPaint paint) const {
        if(size < 0.) return;
        SkPaint paintT = paint;
        paintT.setAlpha(opacity*255);
        paintT.setStrokeWidth(size);
        canvas->drawPath(linePath, paintT);
    }

    SkPath linePath;
    QPointF pos;
    qreal size;
    qreal opacity;
};

class Particle {
public:
    Particle(ParticleBox *parentBox);
    void initializeParticle(const int &firstFrame,
                            const int &nFrames,
                            const QPointF &iniPos,
                            const QPointF &iniVel,
                            const qreal &partSize);
    void generatePathNextFrame(const int &frame,
                               const qreal &velocityVar,
                               const qreal &velocityVarPeriod,
                               const QPointF &acc,
                               const qreal &finalScale,
                               const qreal &finalOpacity,
                               const qreal &decayFrames,
                               const qreal &length);

    bool isVisibleAtFrame(const int &frame);
    ParticleState getParticleStateAtFrame(const int &frame);
private:
    qreal mSize;
    QPointF mPrevVelocityVar;
    QPointF mNextVelocityVar;
    qreal mPrevVelocityDuration;

    ParticleBox *mParentBox;
    int mFirstFrame;
    int mNumberFrames;
    ParticleState *mParticleStates = NULL;

    qreal mLastScale;
    qreal mLastOpacity;
    QPointF mLastPos;
    QPointF mLastVel;
};

class ParticleEmitter : public ComplexAnimator {
public:
    ParticleEmitter(ParticleBox *parentBox);

    void setParentBox(ParticleBox *parentBox);

    void generateParticles();

    void drawParticlesSk(SkCanvas *canvas);
    void updateParticlesForFrame(const int &frame);
    QRectF getParticlesBoundingRect();
    void scheduleGenerateParticles();
    void scheduleUpdateParticlesForFrame();
    void generateParticlesIfNeeded();
    void updateParticlesForFrameIfNeeded(const int &frame);
    bool relPointInsidePath(const QPointF &relPos);

    Property *makeDuplicate();

    void duplicateAnimatorsFrom(QPointFAnimator *pos,
            QrealAnimator *width,
            QrealAnimator *srcVelInfl,

            QrealAnimator *iniVelocity,
            QrealAnimator *iniVelocityVar,
            QrealAnimator *iniVelocityAngle,
            QrealAnimator *iniVelocityAngleVar,
            QPointFAnimator *acceleration,

            QrealAnimator *particlesPerSecond,
            QrealAnimator *particlesFrameLifetime,

            QrealAnimator *velocityRandomVar,
            QrealAnimator *velocityRandomVarPeriod,

            QrealAnimator *particleSize,
            QrealAnimator *particleSizeVar,

            QrealAnimator *particleLength,

            QrealAnimator *particlesDecayFrames,
            QrealAnimator *particlesSizeDecay,
            QrealAnimator *particlesOpacityDecay);

    void makeDuplicate(Property *target);

    void setMinFrame(const int &minFrame);

    void setMaxFrame(const int &maxFrame);

    void setFrameRange(const int &minFrame,
                       const int &maxFrame);

    ColorAnimator *getColorAnimator();
    MovablePoint *getPosPoint();
private:
    QRectF mParticlesBoundingRect;
    bool mGenerateParticlesScheduled = false;
    bool mUpdateParticlesForFrameScheduled = false;
    int mMinFrame = -10;
    int mMaxFrame = 200;

    QList<ParticleState> mParticleStates;
    QList<Particle*> mParticles;
    QList<Particle*> mNotFinishedParticles;
    ParticleBox *mParentBox = NULL;

    QSharedPointer<ColorAnimator> mColorAnimator =
            (new ColorAnimator())->ref<ColorAnimator>();

    QSharedPointer<MovablePoint> mPos;
    QSharedPointer<QrealAnimator> mWidth =
            (new QrealAnimator())->ref<QrealAnimator>();

    QSharedPointer<QrealAnimator> mSrcVelInfl =
            (new QrealAnimator())->ref<QrealAnimator>();

    QSharedPointer<QrealAnimator> mIniVelocity =
            (new QrealAnimator())->ref<QrealAnimator>();
    QSharedPointer<QrealAnimator> mIniVelocityVar =
            (new QrealAnimator())->ref<QrealAnimator>();
    QSharedPointer<QrealAnimator> mIniVelocityAngle =
            (new QrealAnimator())->ref<QrealAnimator>();
    QSharedPointer<QrealAnimator> mIniVelocityAngleVar =
            (new QrealAnimator())->ref<QrealAnimator>();
    QSharedPointer<QPointFAnimator> mAcceleration =
            (new QPointFAnimator())->ref<QPointFAnimator>();

    QSharedPointer<QrealAnimator> mParticlesPerSecond =
            (new QrealAnimator())->ref<QrealAnimator>();
    QSharedPointer<QrealAnimator> mParticlesFrameLifetime =
            (new QrealAnimator())->ref<QrealAnimator>();

    QSharedPointer<QrealAnimator> mVelocityRandomVar =
            (new QrealAnimator())->ref<QrealAnimator>();
    QSharedPointer<QrealAnimator> mVelocityRandomVarPeriod =
            (new QrealAnimator())->ref<QrealAnimator>();

    QSharedPointer<QrealAnimator> mParticleSize =
            (new QrealAnimator())->ref<QrealAnimator>();
    QSharedPointer<QrealAnimator> mParticleSizeVar =
            (new QrealAnimator())->ref<QrealAnimator>();

    QSharedPointer<QrealAnimator> mParticleLength =
            (new QrealAnimator())->ref<QrealAnimator>();

    QSharedPointer<QrealAnimator> mParticlesDecayFrames =
            (new QrealAnimator())->ref<QrealAnimator>();
    QSharedPointer<QrealAnimator> mParticlesSizeDecay =
            (new QrealAnimator())->ref<QrealAnimator>();
    QSharedPointer<QrealAnimator> mParticlesOpacityDecay =
            (new QrealAnimator())->ref<QrealAnimator>();
};

class ParticleBox : public BoundingBox {
    Q_OBJECT
public:
    ParticleBox(BoxesGroup *parent);
    void getAccelerationAt(const QPointF &pos,
                           const int &frame,
                           QPointF *acc);
    void updateAfterFrameChanged(const int &currentFrame);
    void updateRelBoundingRect();
    void preUpdatePixmapsUpdates();
    bool relPointInsidePath(const QPointF &relPos);

    void addEmitter(ParticleEmitter *emitter);
    void drawSk(SkCanvas *canvas);
    void drawSelectedSk(SkCanvas *canvas,
                        const CanvasMode &currentCanvasMode,
                        const SkScalar &invScale);

    BoundingBox *createNewDuplicate(BoxesGroup *parent);

    void makeDuplicate(Property *targetBox);

    void startAllPointsTransform();

    MovablePoint *getPointAtAbsPos(const QPointF &absPtPos,
                             const CanvasMode &currentCanvasMode,
                             const qreal &canvasScaleInv);
    void selectAndAddContainedPointsToList(const QRectF &absRect,
                                           QList<MovablePoint *> *list);
    void applyPaintSetting(const PaintSetting &setting);
    MovablePoint *getBottomRightPoint();
    void addEmitterAtAbsPos(const QPointF &absPos);

    void prp_setAbsFrame(const int &frame);

    void setUpdateVars();

    bool SWT_isParticleBox();
public slots:
    void updateAfterDurationRectangleRangeChanged();
private:
    bool mFrameChangedUpdateScheduled = false;
    MovablePoint *mTopLeftPoint;
    MovablePoint *mBottomRightPoint;
    QList<ParticleEmitter*> mEmitters;
};

#endif // PARTICLEBOX_H

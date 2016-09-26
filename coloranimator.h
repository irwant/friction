#ifndef COLORANIMATOR_H
#define COLORANIMATOR_H
#include "complexanimator.h"
#include "Colors/color.h"

enum ColorMode {
    HSVMODE,
    HSLMODE,
    RGBMODE
};

class ColorAnimator : public ComplexAnimator
{
public:
    ColorAnimator();

    void setCurrentValue(Color colorValue);
    void setCurrentValue(QColor qcolorValue);

    Color getCurrentValue() const;
    void setColorMode(ColorMode colorMode);

    void startVal1Transform();
    void startVal2Transform();
    void startVal3Transform();
    void startAlphaTransform();

    QrealKey *getKeyAtPos(qreal relX, qreal relY,
                          int minViewedFrame, qreal pixelsPerFrame);
private:
    ColorMode mColorMode;
    QrealAnimator mVal1Animator;
    QrealAnimator mVal2Animator;
    QrealAnimator mVal3Animator;
    QrealAnimator mAlphaAnimator;
};

#endif // COLORANIMATOR_H

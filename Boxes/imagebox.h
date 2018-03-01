#ifndef IMAGEBOX_H
#define IMAGEBOX_H
#include "Boxes/boundingbox.h"
#include "skiaincludes.h"
class FileCacheHandler;
class ImageCacheHandler;

struct ImageBoxRenderData : public BoundingBoxRenderData {
    ImageBoxRenderData(FileCacheHandler *cacheHandler,
                       BoundingBox *parentBoxT) :
        BoundingBoxRenderData(parentBoxT) {
        mDelayDataSet = true;
        srcCacheHandler = cacheHandler;
    }

    virtual void loadImageFromHandler();

    void beforeUpdate() {
        if(!allDataReady()) {
            loadImageFromHandler();
        }
        BoundingBoxRenderData::beforeUpdate();
        if(image.get() == NULL) {
            qDebug() << "hell?";
        }
    }

    bool allDataReady() {
        return image.get() != NULL;
    }

    void updateRelBoundingRect() {
        relBoundingRect = QRectF(0., 0., image->width(), image->height());
    }

    sk_sp<SkImage> image;

    FileCacheHandler *srcCacheHandler;
private:
    void drawSk(SkCanvas *canvas) {
        SkPaint paint;
        paint.setFilterQuality(kHigh_SkFilterQuality);
        paint.setAntiAlias(true);
        if(image == NULL) {
            qDebug() << "no image to draw";
        }
        canvas->drawImage(image, 0, 0, &paint);
    }
};

class ImageBox : public BoundingBox {
public:
    ImageBox();
    ImageBox(const QString &filePath);
    ~ImageBox();

    void setFilePath(const QString &path);

    bool SWT_isImageBox() { return true; }
    void addActionsToMenu(QMenu *menu);
    bool handleSelectedCanvasAction(QAction *selectedAction);
    void changeSourceFile();

    void setupBoundingBoxRenderDataForRelFrame(const int &relFrame,
                                               BoundingBoxRenderData *data);
    void setupBoundingBoxRenderDataForRelFrameF(const qreal &relFrame,
                                                BoundingBoxRenderData *data);
    BoundingBoxRenderData *createRenderData();
    void writeBoundingBox(QIODevice *target);
    void readBoundingBox(QIODevice *target);
private:
    ImageCacheHandler *mImgCacheHandler = NULL;
    QString mImageFilePath;
};

#endif // IMAGEBOX_H

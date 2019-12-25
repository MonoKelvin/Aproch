#ifndef APROCHFLOWVIEW_H
#define APROCHFLOWVIEW_H

#include "Aproch.h"

#include <QGraphicsView>

APROCH_NAMESPACE_BEGIN

class AprochFlowScene;

class APROCH_EXPORT AprochFlowView : public QGraphicsView
{
    Q_OBJECT
public:
    AprochFlowView(QWidget *parent = nullptr);
    AprochFlowView(AprochFlowScene *scene, QWidget *parent = nullptr);
    ~AprochFlowView() override;

    AprochFlowView(const AprochFlowView &) = delete;
    AprochFlowView operator=(const AprochFlowView &) = delete;

    QAction *getClearSelectionAction() const;

    QAction *getDeleteSelectionAction() const;

    void setScene(AprochFlowScene *scene);

public Q_SLOTS:

    void scaleUp();

    void scaleDown();

    void deleteSelectedNodes();

protected:
//    void contextMenuEvent(QContextMenuEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void drawBackground(QPainter *painter, const QRectF &viewRect) override;

    void showEvent(QShowEvent *event) override;

protected:
    inline AprochFlowScene *scene() {
        return mScene;
    }

private:
    QAction *mClearSelectionAction;
    QAction *mDeleteSelectionAction;

    QPointF mClickPos;

    AprochFlowScene *mScene;
};

APROCH_NAMESPACE_END

#endif // APROCHFLOWVIEW_H

#ifndef APROCHFLOWVIEW_H
#define APROCHFLOWVIEW_H

#include "Aproch.h"

#include <QGraphicsView>

APROCH_NAMESPACE_BEGIN

class AFlowScene;

class APROCH_EXPORT AFlowView : public QGraphicsView
{
    Q_OBJECT
public:
    AFlowView(QWidget *parent = nullptr);
    AFlowView(AFlowScene *scene, QWidget *parent = nullptr);
    ~AFlowView() override;

    AFlowView(const AFlowView &) = delete;
    AFlowView operator=(const AFlowView &) = delete;

    QAction *getClearSelectionAction() const;

    QAction *getDeleteSelectionAction() const;

    void setScene(AFlowScene *scene);

public Q_SLOTS:

    void scaleUp();

    void scaleDown();

    void deleteSelectedNodes();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void drawBackground(QPainter *painter, const QRectF &viewRect) override;

    void showEvent(QShowEvent *event) override;

protected:
    inline AFlowScene *scene() {
        return mScene;
    }

private:
    QAction *mClearSelectionAction;
    QAction *mDeleteSelectionAction;

    QPointF mClickPos;

    AFlowScene *mScene;
};

APROCH_NAMESPACE_END

#endif // APROCHFLOWVIEW_H

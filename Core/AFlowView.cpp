#include "AFlowView.h"

//#include <QtOpenGL/QtOpenGL>
#include <QtWidgets>

#include "AFlowScene.h"
#include "ADataModelRegistry.h"
#include "ANode.h"
#include "AConnectionGraphicsObject.h"
#include "AStyle.h"

APROCH_NAMESPACE_BEGIN

AFlowView::AFlowView(QWidget *parent)
    : QGraphicsView(parent)
    , mClearSelectionAction(nullptr)
    , mDeleteSelectionAction(nullptr)
    , mScene(nullptr)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(AStyle::GetFlowViewStyle().BackgroundColor);

    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setCacheMode(QGraphicsView::CacheBackground);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
}

AFlowView::AFlowView(AFlowScene *scene, QWidget *parent)
    : AFlowView(parent)
{
    setScene(scene);
}

AFlowView::~AFlowView()
{
    if (mScene)
    {
        mScene->deleteLater();
    }
}

QAction *AFlowView::getClearSelectionAction() const
{
    return mClearSelectionAction;
}

QAction *AFlowView::getDeleteSelectionAction() const
{
    return mDeleteSelectionAction;
}

void AFlowView::setScene(AFlowScene *scene)
{
    mScene = scene;
    QGraphicsView::setScene(mScene);

    // TODO: need to modify.
    delete mClearSelectionAction;
    mClearSelectionAction = new QAction(tr("取消选择"), this);
    mClearSelectionAction->setShortcut(Qt::Key_Escape);
    connect(mClearSelectionAction, &QAction::triggered, mScene, &QGraphicsScene::clearSelection);
    addAction(mClearSelectionAction);

    delete mDeleteSelectionAction;
    mDeleteSelectionAction = new QAction(tr("删除选择的节点"), this);
    mDeleteSelectionAction->setShortcut(Qt::Key_Delete);
    connect(mDeleteSelectionAction, &QAction::triggered, this, &AFlowView::deleteSelectedNodes);
    addAction(mDeleteSelectionAction);
}

void AFlowView::contextMenuEvent(QContextMenuEvent *event)
{
    // 如果鼠标下有节点，则把事件传递给scene处理
    if (itemAt(event->pos()))
    {
        QGraphicsView::contextMenuEvent(event);
        return;
    }

    // TODO: 将以下菜单过滤及搜索功能抽象出来单独作为一个空间类
    QMenu modelMenu;

    auto skipText = QStringLiteral("skip me");

    // 为右键菜单添加节点过滤输入框
    auto *txtBox = new QLineEdit(&modelMenu);
    txtBox->setPlaceholderText(tr("过滤"));
    txtBox->setClearButtonEnabled(true);

    auto *txtBoxAction = new QWidgetAction(&modelMenu);
    txtBoxAction->setDefaultWidget(txtBox);

    modelMenu.addAction(txtBoxAction);

    // 为搜索结果创建树形视图
    auto *treeView = new QTreeWidget(&modelMenu);
    treeView->header()->close();

    auto *treeViewAction = new QWidgetAction(&modelMenu);
    treeViewAction->setDefaultWidget(treeView);

    modelMenu.addAction(treeViewAction);

    QMap<QString, QTreeWidgetItem *> topLevelItems;
    for (auto const &cat : mScene->getRegistry()->getCategories())
    {
        auto item = new QTreeWidgetItem(treeView);
        item->setText(0, cat);
        item->setData(0, Qt::UserRole, skipText);
        topLevelItems[cat] = item;
    }

    for (auto const &assoc : mScene->getRegistry()->getRegisteredModelsCategoryAssociation())
    {
        auto parent = topLevelItems[assoc.second];
        auto item = new QTreeWidgetItem(parent);
        item->setText(0, assoc.first);
        item->setData(0, Qt::UserRole, assoc.first);
    }

    treeView->expandAll();

    connect(treeView, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int)
    {
        QString modelName = item->data(0, Qt::UserRole).toString();

        if (modelName == skipText)
        {
            return;
        }

        auto type = mScene->getRegistry()->create(modelName);

        if (type)
        {
            auto &node = mScene->createNode(type.get());

            QPoint pos = event->pos();

            QPointF posView = this->mapToScene(pos);

            node.getNodeGraphicsObject()->setPos(posView);

            emit mScene->nodePlaced(node);
        }
        else
        {
            qDebug() << "Model not found";
        }

        modelMenu.close();
    });

    // Setup filtering
    connect(txtBox, &QLineEdit::textChanged, [&](const QString & text)
    {
        for (auto &topLvlItem : topLevelItems)
        {
            for (int i = 0; i < topLvlItem->childCount(); ++i)
            {
                auto child = topLvlItem->child(i);
                auto modelName = child->data(0, Qt::UserRole).toString();
                const bool match = (modelName.contains(text, Qt::CaseInsensitive));
                child->setHidden(!match);
            }
        }
    });

    // make sure the text box gets focus so the user doesn't have to click on it
    txtBox->setFocus();

    modelMenu.exec(event->globalPos());
}

void AFlowView::wheelEvent(QWheelEvent *event)
{
    QPoint delta = event->angleDelta();

    if (delta.y() == 0)
    {
        event->ignore();
        return;
    }

    double const d = delta.y() / std::abs(delta.y());

    if (d > 0.0)
    {
        scaleUp();
    }
    else
    {
        scaleDown();
    }
}

void AFlowView::scaleUp()
{
    double const step = 1.2;
    double const factor = std::pow(step, 1.0);

    if (transform().m11() > 2.0)
    {
        return;
    }

    scale(factor, factor);
}

void AFlowView::scaleDown()
{
    double const step = 1.2;
    double const factor = std::pow(step, -1.0);

    if (transform().m11() < 0.5)
    {
        return;
    }

    scale(factor, factor);
}

void AFlowView::deleteSelectedNodes()
{
    // 首先删除连线，为了确保它们不会由于删除节点时被自动删除
    for (QGraphicsItem *item : mScene->selectedItems())
    {
        if (auto c = qgraphicsitem_cast<AConnectionGraphicsObject *>(item))
        {
            mScene->deleteConnection(c->connection());
        }
    }

    for (QGraphicsItem *item : mScene->selectedItems())
    {
        if (auto n = qgraphicsitem_cast<ANodeGraphicsObject *>(item))
        {
            mScene->removeNode(n->getNode());
        }
    }
}

void AFlowView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Shift:
        setDragMode(QGraphicsView::RubberBandDrag);
        break;
    default:
        break;
    }

    QGraphicsView::keyPressEvent(event);
}

void AFlowView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Shift:
        setDragMode(QGraphicsView::ScrollHandDrag);
        break;
    default:
        break;
    }
    QGraphicsView::keyReleaseEvent(event);
}

void AFlowView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        mClickPos = mapToScene(event->pos());
    }
}

void AFlowView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (scene() && scene()->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton)
    {
        if ((event->modifiers() & Qt::ShiftModifier) == 0)
        {
            QPointF difference = mClickPos - mapToScene(event->pos());
            setSceneRect(sceneRect().translated(difference.x(), difference.y()));
        }
    }
}

void AFlowView::drawBackground(QPainter *painter, const QRectF &viewRect)
{
    QGraphicsView::drawBackground(painter, viewRect);

    QRect windowRect = rect();
    QPointF tl = mapToScene(windowRect.topLeft());
    QPointF br = mapToScene(windowRect.bottomRight());

    auto drawGrid = [&](double gridStep)
    {

        double left = std::floor(tl.x() / gridStep - 0.5);
        double right = std::floor(br.x() / gridStep + 1.0);
        double bottom = std::floor(tl.y() / gridStep - 0.5);
        double top = std::floor(br.y() / gridStep + 1.0);

        // vertical lines
        for (int xi = int(left); xi <= int(right); ++xi)
        {
            QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep, top * gridStep);
            painter->drawLine(line);
        }

        // horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi)
        {
            QLineF line(left * gridStep, yi * gridStep, right * gridStep, yi * gridStep);
            painter->drawLine(line);
        }
    };

    auto const &flowViewStyle = AStyle::GetFlowViewStyle();

    QBrush bBrush = backgroundBrush();

    QPen pfine(flowViewStyle.FineGridColor, 1.0);
    painter->setPen(pfine);
    drawGrid(20);

    QPen p(flowViewStyle.CoarseGridColor, 1.2);
    painter->setPen(p);
    drawGrid(120);

    QPen pc(flowViewStyle.CentralGridColor, 1.5);
    painter->setPen(pc);
    painter->drawLine(QLineF(tl.x(), 0.0, br.x(), 0.0));
    painter->drawLine(QLineF(0.0, tl.y(), 0.0, br.y()));
}

void AFlowView::showEvent(QShowEvent *event)
{
    if (mScene)
    {
        mScene->setSceneRect(this->rect());
    }

    QGraphicsView::showEvent(event);
}

APROCH_NAMESPACE_END

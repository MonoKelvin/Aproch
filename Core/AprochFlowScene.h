#ifndef APROCHFLOWSCENE_H
#define APROCHFLOWSCENE_H

#include "Aproch.h"

//#include <QUuid>
#include <QGraphicsScene>

//#include "QUuidStdHash.hpp"
//#include "DataModelRegistry.hpp"
//#include "TypeConverter.hpp"

//#include <memory>
//#include <utility>

APROCH_NAMESPACE_BEGIN

//class AprochNodeDataModel;
//class AprochFlowItemInterface;
//class AprochNode;
//class AprochNodeGraphicsObject;
//class AprochConnection;
//class AprochConnectionGraphicsObject;
//class AprochNodeStyle;

//#include <QSharedPointer>

class APROCH_EXPORT AprochFlowScene : public QGraphicsScene
{
    Q_OBJECT
public:
//    AprochFlowScene(QSharedPointer<AprochDataModelRegistry> registry, QObject * parent = nullptr);

    AprochFlowScene(QObject * parent = nullptr);

    ~AprochFlowScene();

private:

};

APROCH_NAMESPACE_END

#endif // APROCHFLOWSCENE_H

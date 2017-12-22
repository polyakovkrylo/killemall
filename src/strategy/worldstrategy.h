/*!
 * \file worldstrategy.h
 *
 * WorldStrategy class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#ifndef WORLDSTRATEGY_H
#define WORLDSTRATEGY_H

#include <QObject>

#include "model/worldmodel.h"
#include "controller/worldabstractcontroller.h"

/*!
 * \defgroup strategy
 * \brief AI implementation
 */

/*!
 * \ingroup strategy
 * \brief AI implementation class
 *
 * This class can be used as a base class for other strategies,
 * in that case nextMove should be re-implemented
 */
class WorldStrategy : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Class constructor
     * \param parent parent object
     */
    explicit WorldStrategy(QObject *parent = 0);
    /*!
     * \brief Set the model to control
     */
    void setModel(WorldModel* model);

private:
    WorldModel* model_;

public slots:
    /*!
     * \brief Run the AI strategy
     *
     * When no moves left, finished is emitted
     */
    void run();
    /*!
     * \brief Run/stop the AI strategy
     * \param b run on true, stop on false
     */
    void run(bool b);
    /*!
     * \brief Stop running the AI strategy
     */
    void stop();
    /*!
     * \brief Perform a next move
     *
     * When the strategy is running, each move is performed
     * after the previous step is done, shat is indicated with
     * animationDone signal
     */
    virtual void nextMove();

signals:
    /*!
     * \brief Strategy finished signal
     *
     * Whenever AI has no more moves this signal is emitted
     */
    void finished();
};

#endif // WORLDSTRATEGY_H

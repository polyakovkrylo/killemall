/*!
 * \file worldgraphicsview.h
 *
 * WorldGraphicsView class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#ifndef WORLDGRAPHICSVIEW_H
#define WORLDGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QProgressBar>
#include <QLabel>
#include <QKeyEvent>

#include "model/worldmodel.h"

/*!
 * \defgroup graphicsview
 *
 * \brief Graphics view component implementation
 */

/*!
 * \ingroup graphicsview
 * \brief Graphics view component class implementation
 *
 * The class is based on QGraphicsView and uses
 * QGraphicsScene as a graphical model(scene), and
 * a WorldModel instance as a data model for the scene.
 */
class WorldGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    /*!
     * \brief Class constructor
     * \param parent visual parent of the view
     */
    WorldGraphicsView(QWidget *parent = 0);

    /*!
     * \brief Set the data model
     *
     * Every time a new model is set, the view
     * re-initializes the scene in accordance
     * with a given model.
     *
     * \param model model to be set
     */
    void setModel(WorldModel *model);

    /*!
     * \brief Returns item at the position
     *
     * \param itemCenter position of item's center
     * \return item at the position
     */
    QGraphicsItem *itemAt(QPoint itemCenter);

    /*!
     * \brief Key press event handler
     * \param e key event
     */
    virtual void keyPressEvent(QKeyEvent *e) override;

    /*!
     * \brief Mouse press event handler
     * \param e mouse event
     */
    virtual void mousePressEvent(QMouseEvent *e) override;

    /*!
     * \brief Mouse move event handler
     * \param e mouse event
     */
    virtual void mouseMoveEvent(QMouseEvent *e) override;

private:
    WorldModel* model_;
    QGraphicsScene* scene_;
    QGraphicsEllipseItem* protagonist_;
    QProgressBar* healthBar_;
    QProgressBar* energyBar_;

public slots:
    /*!
     * \brief Update protagonist position
     *
     * Move protagonist item to a given point
     * and centers the view on this position
     * \param x horizontal coordinate
     * \param y vertical coordinate
     */
    void onProtagonistPositionChanged(int x, int y);
    /*!
     * \brief Reload the model slot
     *
     * Re-initializes the scene in accordance
     * with a given model.
     */
    void reloadScene();

};

#endif // WORLDGRAPHICSVIEW_H

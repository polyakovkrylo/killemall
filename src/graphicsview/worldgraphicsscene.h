/*!
 * \file worldscene.h
 *
 * WorldGraphicsScene class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#ifndef WORLDGRAPHICSSCENE_H
#define WORLDGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QPainter>

/*!
 * \ingroup graphicsview
 * \brief Graphics scene class
 *
 * This class is used as the scene for graphics view.
 * The only function that is re-implemented is drawBackground,
 * in order to draw not tiled background.
 */
class WorldGraphicsScene : public QGraphicsScene
{
public:
    /*!
     * \brief Class constructor
     * \param back background image
     * \param rect scene area
     * \param parent parent object
     */
    WorldGraphicsScene(QImage back, QRectF rect, QObject* parent = nullptr);
    /*!
     * \brief Draw background image
     *
     * This re-implemented function allows to draw background only once
     * \param painter painter object
     * \param rect view area (not used)
     */
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
private:
    QImage back_;
};

#endif // WORLDGRAPHICSSCENE_H

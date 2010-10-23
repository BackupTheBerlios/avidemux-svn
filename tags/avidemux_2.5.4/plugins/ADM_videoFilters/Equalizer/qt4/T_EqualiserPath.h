/***************************************************************************
                             T_EqualiserPath.h
                             -----------------

    begin                : Tue Oct 7 2008
    copyright            : (C) 2008 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EqualiserPath_H
#define EqualiserPath_H

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

#include "T_gridFrame.h"

class EqualiserPath : public GridFrame
{
    Q_OBJECT

public:
    EqualiserPath(QWidget *parent, int points[]);

    void paint(QPainter *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
	void updatePoint(int pointIndex, int value);
	void resetPoints(int points[]);

signals:
     void pointChanged(int pointIndex, int value);

private:
    int _pointCount;
    int _pointSize;
    int _activePoint;
    QVector<QPointF> _points;
    QPoint _mousePress;
};

#endif

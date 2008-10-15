/***************************************************************************
                             T_EqualiserPath.cpp
                             -------------------

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

#include <QtGui/QPalette>
#include <QtGui/QSizePolicy>

#include "T_EqualiserPath.h"

EqualiserPath::EqualiserPath(QWidget *parent, int points[]) : GridFrame(parent)
{
	_pointSize = 4;
	_activePoint = -1;
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	for (int i = 0; i < 300; i++)
	{
		if (points[i] == -1)
			break;

		_points << QPointF(points[i], points[i]);
	}
}

void EqualiserPath::resetPoints(int points[])
{
	_points.clear();

	for (int i = 0; i < 300; i++)
	{
		if (points[i] == -1)
			break;

		_points << QPointF(points[i], points[i]);
	}

	repaint();
}

void EqualiserPath::updatePoint(int pointIndex, int value)
{
	if (pointIndex >= 0 && pointIndex < _points.size())
	{
		_points[pointIndex] = QPointF(_points[pointIndex].x(), value);
		repaint();
		emit pointChanged(pointIndex, value);
	}
}

void EqualiserPath::paint(QPainter *painter)
{
	QPainterPath path;
	QPointF point;
	QPalette pal = palette();

	painter->setPen(Qt::NoPen);
	painter->setRenderHint(QPainter::Antialiasing);

	point = _points.at(0);
	path.moveTo(QPointF(point.x(), height() - point.y()));

	for (int i = 0; i < _points.size(); i++)
	{
		point = _points.at(i);
		path.lineTo(QPointF(point.x() + _pointSize, height() - point.y() - _pointSize));
	}

	QPen penYellow(Qt::yellow, 1.5, Qt::DashLine, Qt::FlatCap, Qt::BevelJoin);
	QPen penRed(Qt::red, 1.5, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);

	painter->setPen(penYellow);
	painter->drawLine(QPointF(0, height()), QPointF(width(), 0));
	painter->strokePath(path, penRed);
	painter->setPen(QColor(50, 100, 120, 200));
	painter->setBrush(QColor(200, 200, 210, 120));

	for (int i = 0; i < _points.size(); i++)
	{
		point = _points.at(i);
		painter->drawEllipse(QRectF(point.x(), height() - point.y() - (_pointSize * 2), _pointSize * 2, _pointSize * 2));
	}
}

void EqualiserPath::mousePressEvent(QMouseEvent *event)
{
	_activePoint = -1;
	qreal distance = -1;

	for (int i = 0; i < _points.size(); i++)
	{
		QPointF point = _points.at(i);
		qreal d = QLineF(event->pos(), QPointF(point.x(), height() - point.y())).length();

		if ((distance < 0 && d < 8 * _pointSize) || d < distance)
		{
			distance = d;
			_activePoint = i;
		}
	}

	if (_activePoint != -1)
		mouseMoveEvent(event);

	_mousePress = event->pos();
}

void EqualiserPath::mouseMoveEvent(QMouseEvent *event)
{
	if (_activePoint >= 0 && _activePoint < _points.size())
	{
		qreal y = height() - event->pos().y() - _pointSize;

		if (y < 0)
			y = 0;
		else if (y > (height() - (_pointSize * 2)))
			y = height() - (_pointSize * 2);

		_points[_activePoint] = QPointF(_points[_activePoint].x(), y);

		emit pointChanged(_activePoint, _points.at(_activePoint).y());
		update();
	}
}

void EqualiserPath::mouseReleaseEvent(QMouseEvent *event)
{
	_activePoint = -1;
}

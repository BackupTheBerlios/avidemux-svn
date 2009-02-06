/***************************************************************************
                               T_gridFrame.cpp
                               ---------------

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

#include <QtGui/QPainterPath>

#include "T_gridFrame.h"

GridFrame::GridFrame(QWidget *parent, unsigned int cornerRadius) : QWidget(parent)
{
	this->cornerRadius = cornerRadius;

	QColor colour(230, 230, 230);

	tileMap = QPixmap(100, 100);
	tileMap.fill(Qt::white);

	QPainter painter(&tileMap);

	painter.fillRect(0, 0, 50, 50, colour);
	painter.fillRect(50, 50, 50, 50, colour);
	painter.end();
}

void GridFrame::paintEvent(QPaintEvent *event)
{
	QPainter painter;

	painter.begin(this);
	painter.setClipRect(event->rect());
	painter.setRenderHint(QPainter::Antialiasing);

	QPainterPath clipPath;

	QRect rect = this->rect();
	int left = rect.x() + 1;
	int top = rect.y() + 1;
	int right = rect.right();
	int bottom = rect.bottom();

	clipPath.moveTo(right - cornerRadius, top);
	clipPath.arcTo(right - cornerRadius, top, cornerRadius, cornerRadius, 90, -90);
	clipPath.arcTo(right - cornerRadius, bottom - cornerRadius, cornerRadius, cornerRadius, 0, -90);
	clipPath.arcTo(left, bottom - cornerRadius, cornerRadius, cornerRadius, 270, -90);
	clipPath.arcTo(left, top, cornerRadius, cornerRadius, 180, -90);
	clipPath.closeSubpath();

	painter.save();
	painter.setClipPath(clipPath, Qt::IntersectClip);
	painter.drawTiledPixmap(this->rect(), tileMap, QPointF(0, -(height() % 50)));

	paint(&painter);

	painter.restore();
	painter.setPen(QPen(QColor(180, 180, 180), 2));
	painter.setBrush(Qt::NoBrush);
	painter.drawPath(clipPath);
}

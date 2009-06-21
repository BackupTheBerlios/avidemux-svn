/***************************************************************************
                                T_gridFrame.h
                                -------------

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

#ifndef GRIDFRAME_H
#define GRIDFRAME_H

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QWidget>

class GridFrame : public QWidget
{
	Q_OBJECT

public:
	GridFrame(QWidget *parent, unsigned int cornerRadius = 8);
	virtual void paint(QPainter *) {}

protected:
	void paintEvent(QPaintEvent *);

	unsigned int cornerRadius;
	QPixmap tileMap;
};

#endif

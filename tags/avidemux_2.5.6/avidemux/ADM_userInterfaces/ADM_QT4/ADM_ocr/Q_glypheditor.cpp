/***************************************************************************
                              Q_glypheditor.cpp
                              -----------------
    begin                : Fri Oct 3 2008
    copyright            : (C) 2008 by mean/gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "Q_glypheditor.h"
#include "DIA_fileSel.h"
#include "DIA_coreToolkit.h"
#include "DIA_factory.h"
#include "ADM_ocr/adm_glyph.h"
#include "ADM_toolkitQt.h"

GlyphEditorWindow::GlyphEditorWindow(QWidget *parent, char *glyphFileName, admGlyph *head, int nbGlyph, int glyphHeight) : QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.lineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(lineEdit_changed(const QString&)));

	connect(ui.previousButton, SIGNAL(clicked(bool)), this, SLOT(previousButton_clicked(bool)));
	connect(ui.nextButton, SIGNAL(clicked(bool)), this, SLOT(nextButton_clicked(bool)));
	
	connect(ui.prevEmptyButton, SIGNAL(clicked(bool)), this, SLOT(prevEmptyButton_clicked(bool)));
	connect(ui.nextEmptyButton, SIGNAL(clicked(bool)), this, SLOT(nextEmptyButton_clicked(bool)));

	connect(ui.findButton, SIGNAL(clicked(bool)), this, SLOT(findButton_clicked(bool)));
	connect(ui.homeButton, SIGNAL(clicked(bool)), this, SLOT(homeButton_clicked(bool)));
	connect(ui.deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButton_clicked(bool)));

	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(saveButton_clicked()));

	canvas = new ADM_QCanvas(ui.frame, 1, 1);
	canvas->move(4, 4);

	this->head = head;
	this->nbGlyph = nbGlyph;
	this->glyphFileName = glyphFileName;
	this->currentGlyph = head->next;
	this->glyphHeight = glyphHeight;
	this->image = NULL;

	glyphUpdate();
}

GlyphEditorWindow::~GlyphEditorWindow(void)
{
	delete canvas;

	if (image)
		delete image;
}

void GlyphEditorWindow::glyphUpdate(void)
{
	if (!currentGlyph)
		return;

	ui.frame->setMinimumSize(currentGlyph->width + 8, glyphHeight + 8);
	ui.frame->resize(currentGlyph->width + 8, glyphHeight + 8);
	canvas->changeSize(currentGlyph->width, currentGlyph->height);

	// Convert 8-bit image to 32-bit RGB
	if (image)
		delete image;

	image = new QImage(currentGlyph->width, currentGlyph->height, QImage::Format_RGB32);
	uint8_t *ptr = currentGlyph->data;

	for (int y = 0; y < currentGlyph->height; y++)
		for (int x = 0; x < currentGlyph->width; x++)
		{
			image->setPixel(x, y, qRgb(*ptr, *ptr, *ptr));
			ptr++;
		}

	// Display it
	canvas->dataBuffer = (uint8_t*)(image->bits());
	canvas->repaint();

	if (currentGlyph->code)
		ui.lineEdit->setText(currentGlyph->code);
}

void GlyphEditorWindow::lineEdit_changed(const QString &text)
{
	if(currentGlyph->code)
		delete [] currentGlyph->code;

	currentGlyph->code = NULL;

	if (text.length())
	{
		QByteArray bytes = text.toUtf8();

		currentGlyph->code = new char[bytes.length() + 1];
		strcpy(currentGlyph->code, bytes.constData());
	}
}

void GlyphEditorWindow::previousButton_clicked(bool checked)
{
	if (currentGlyph != head->next)
	{
		admGlyph *father = glyphSearchFather(currentGlyph, head);

		if (father)
		{
			currentGlyph = father;
			glyphUpdate();
		}
	}
}

void GlyphEditorWindow::nextButton_clicked(bool checked)
{
	if (currentGlyph->next)
	{
		currentGlyph = currentGlyph->next; 
		glyphUpdate();
	}
}

void GlyphEditorWindow::prevEmptyButton_clicked(bool checked)
{
	while(1)
	{
		if (currentGlyph != head->next)
		{
			admGlyph *father = glyphSearchFather(currentGlyph, head);

			if (father)
			{
				currentGlyph = father;
				glyphUpdate();

				if (!currentGlyph->code || !*(currentGlyph->code))
					break;
			} 
		}

		GUI_Error_HIG(GlyphEditorWindow::tr("Head reached").toUtf8().constData(), GlyphEditorWindow::tr("No more glyphs").toUtf8().constData());
		break;
	}
}

void GlyphEditorWindow::nextEmptyButton_clicked(bool checked)
{
	while (1)
	{
		if (currentGlyph->next)
		{
			currentGlyph = currentGlyph->next; 
			glyphUpdate();

			if (!currentGlyph->code || !*(currentGlyph->code))
				break;
		}
		else 
		{
			GUI_Error_HIG(GlyphEditorWindow::tr("End reached").toUtf8().constData(), GlyphEditorWindow::tr("No more glyphs").toUtf8().constData());
			break;
		}
	}
}

void GlyphEditorWindow::homeButton_clicked(bool checked)
{
	currentGlyph = head->next;
	glyphUpdate();
}

void GlyphEditorWindow::findButton_clicked(bool checked)
{
	char *tomatch = NULL;
	diaElemText txt(&tomatch, GlyphEditorWindow::tr("String").toUtf8().constData(), NULL);
	diaElem *elems[] = {&txt};

	if (!diaFactoryRun(GlyphEditorWindow::tr("Search string").toUtf8().constData(), 1, elems))
		return;

	printf("Searched string <%s>\n", tomatch);

	while (currentGlyph->next)
	{
		currentGlyph = currentGlyph->next;
		glyphUpdate();

		if (currentGlyph->code)
		{
			printf("%s vs %s\n", currentGlyph->code, tomatch);
			if (!strcmp(currentGlyph->code, tomatch))
			{
				glyphUpdate();
				break;
			}
		}
	}

	delete [] tomatch;

	if (!currentGlyph->next)
		GUI_Error_HIG(GlyphEditorWindow::tr("End reached").toUtf8().constData(), GlyphEditorWindow::tr("No more glyphs").toUtf8().constData());
}

void GlyphEditorWindow::deleteButton_clicked(bool checked)
{
	admGlyph *father = glyphSearchFather(currentGlyph, head);

	ADM_assert(father);

	father->next = currentGlyph->next;
	delete currentGlyph;

	currentGlyph = father;

	if (father == head && head->next)
		currentGlyph = head->next;

	nbGlyph--;
	glyphUpdate();
}

void GlyphEditorWindow::saveButton_clicked()
{
	if (saveGlyph(glyphFileName, head, nbGlyph))
		accept();
}

uint8_t DIA_glyphEdit(void)
{
	char *glyphName;
	admGlyph head(1,1);

	uint32_t nbGlyph = 0;
	uint8_t ret = 0;

	// First select a file
	GUI_FileSelRead(GlyphEditorWindow::tr("Select GlyphFile to edit").toUtf8().constData(), &glyphName);

	if (!glyphName)
		return 0;

	// Try to load it
	if (!loadGlyph(glyphName, &head, &nbGlyph) || !nbGlyph)
	{
		destroyGlyphTree(&head);
		return 0;
	}

	// Convert the linear glyph to glyph array
	admGlyph *glyphArray[nbGlyph];
	admGlyph *cur = head.next;
	uint32_t idx = 0;
	int glyphHeight = 0;

	while(cur)
	{
		if (glyphHeight < cur->height)
			glyphHeight = cur->height;

		glyphArray[idx++] = cur;
		cur = cur->next;
	}

	ADM_assert(idx <= nbGlyph);
	nbGlyph = idx;

	// Glyph loaded, here we go
	GlyphEditorWindow glyphWindow(qtLastRegisteredDialog(), glyphName, &head, nbGlyph, glyphHeight);

	qtRegisterDialog(&glyphWindow);

	if (glyphWindow.exec() == QDialog::Accepted)
		ret = 1;

	destroyGlyphTree(&head);
	qtUnregisterDialog(&glyphWindow);

	return ret;
}

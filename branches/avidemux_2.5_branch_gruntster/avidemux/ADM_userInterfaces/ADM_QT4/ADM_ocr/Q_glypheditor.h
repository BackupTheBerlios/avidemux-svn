#include "ui_glypheditor.h"
#include "DIA_flyDialogQt4.h"
#include "ADM_ocr/adm_glyph.h"

class GlyphEditorWindow : public QDialog
{
	Q_OBJECT

private:
	Ui_GlyphEditorDialog ui;
	ADM_QCanvas *canvas;
	admGlyph *head;
	admGlyph *currentGlyph;
	int nbGlyph;
	char *glyphFileName;
	int glyphHeight;
	QImage *image;

	void glyphUpdate(void);

public:
	GlyphEditorWindow(QWidget *parent, char *glyphFileName, admGlyph *head, int nbGlyph, int glyphHeight);
	~GlyphEditorWindow();

private slots:
	void lineEdit_changed(const QString &text);
	void previousButton_clicked(bool checked);
	void nextButton_clicked(bool checked);
	void prevEmptyButton_clicked(bool checked);
	void nextEmptyButton_clicked(bool checked);
	void homeButton_clicked(bool checked);
	void findButton_clicked(bool checked);
	void deleteButton_clicked(bool checked);
	void saveButton_clicked();
};

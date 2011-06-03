#ifndef XvidCustomMatrixDialog_h
#define XvidCustomMatrixDialog_h

#include "ui_xvidCustomMatrixDialog.h"

class XvidCustomMatrixDialog : public QDialog
{
	Q_OBJECT

private:
	Ui_XvidCustomMatrixDialog ui;

	void setIntra8x8Luma(const unsigned char intra8x8Luma[64]);
	void setInter8x8Luma(const unsigned char inter8x8Luma[64]);
	bool parseCqmFile(const char cqmFileName[1024], unsigned char intra8x8Luma[64], unsigned char inter8x8Luma[64]);

public:
	XvidCustomMatrixDialog(QWidget *parent, const unsigned char intra8x8Luma[64], const unsigned char inter8x8Luma[64]);
	void getMatrix(unsigned char intra8x8Luma[64], unsigned char inter8x8Luma[64]);

private slots:
	void loadFileButton_pressed();
};
#endif	// XvidCustomMatrixDialog_h

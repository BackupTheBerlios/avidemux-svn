#ifndef ADM_qtScript_ButtonControl
#define ADM_qtScript_ButtonControl

#include <QtScript/QScriptEngine>

#include "Control.h"

namespace ADM_qtScript
{
	/** \brief The ButtonControl %class provides a push button control.
	 */
	class ButtonControl : public Control
	{
		Q_OBJECT

	private:
		QString _title;
		QScriptValue _onClickFunction;

		const QString& getTitle();
		QScriptValue getOnClickFunction();

		void setTitle(const QString &title);
		void setOnClickFunction(QScriptValue onClickFunction);

		static void buttonCallback(void *buttonControl);

	public:
		/** \cond */
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		diaElem* createControl();
		/** \endcond */

		/** \brief Constructs a button widget with the given title and callback function to be executed upon clicking the button.
		 */
		ButtonControl(const QString& /*% String %*/ title, QScriptValue /*% Function %*/ onClickFunction);

		/** \brief Gets or sets the title of the button widget.
		 */
		Q_PROPERTY(const QString& /*% String %*/ title READ getTitle WRITE setTitle);

		/** \brief Gets or sets the function to be called when the button is clicked.
		 */
		Q_PROPERTY(QScriptValue /*% Function %*/ onClickFunction READ getOnClickFunction WRITE setOnClickFunction);
	};
}

#endif

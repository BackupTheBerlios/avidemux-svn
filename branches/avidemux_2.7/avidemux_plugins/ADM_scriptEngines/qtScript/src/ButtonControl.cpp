#include "ButtonControl.h"

namespace ADM_qtScript
{
	ButtonControl::ButtonControl(const QString& title, QScriptValue onClickFunction)
	{
		this->_title = title;
		this->_onClickFunction = onClickFunction;
	}

	QScriptValue ButtonControl::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		if (context->isCalledAsConstructor())
		{
			if (context->argumentCount() == 2 && context->argument(0).isString() && context->argument(1).isFunction())
			{
				return engine->newQObject(
					new ButtonControl(context->argument(0).toString(), context->argument(1)), QScriptEngine::ScriptOwnership);
			}
			else
			{
				return context->throwError("Invalid arguments passed to constructor");
			}
		}

		return engine->undefinedValue();
	}

	diaElem* ButtonControl::createControl(void)
	{
		return new diaElemButton(this->_title.toUtf8().constData(), buttonCallback, this);
	}

	void ButtonControl::buttonCallback(void *buttonControl)
	{
		(static_cast<ButtonControl*>(buttonControl))->getOnClickFunction().call();
	}

	const QString& ButtonControl::getTitle()
	{
		return this->_title;
	}

	QScriptValue ButtonControl::getOnClickFunction()
	{
		return this->_onClickFunction;
	}
	
	void ButtonControl::setTitle(const QString& title)
	{
		this->_title = title;
	}

	void ButtonControl::setOnClickFunction(QScriptValue onClickFunction)
	{
		this->_onClickFunction = onClickFunction;
	}
}

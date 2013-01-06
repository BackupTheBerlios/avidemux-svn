#include "VideoEncoder.h"
#include "FFcodecContext_param.h"

namespace ADM_qtScript
{
	VideoEncoder::VideoEncoder(
		QScriptEngine *engine, IEditor *editor, IVideoEncoderPlugin* encoder) : QtScriptConfigObject(editor)
	{
		std::map<const QString, QScriptEngine::FunctionSignature> configSubGroups;

		configSubGroups.insert(
			std::pair<const QString, QScriptEngine::FunctionSignature>(
				"lavcSettings", QtScriptConfigObject::defaultConfigGetterSetter));

		this->encoderPlugin = encoder;
		this->_configObject = this->createConfigContainer(
								  engine, QtScriptConfigObject::defaultConfigGetterSetter, &configSubGroups);
	}

	QScriptValue VideoEncoder::getName(void)
	{
		return this->encoderPlugin->name();
	}

	QScriptValue VideoEncoder::getConfiguration(void)
	{
		return this->_configObject;
	}

	void VideoEncoder::resetConfiguration(void)
	{
		this->encoderPlugin->resetConfiguration();
	}

	void VideoEncoder::getConfCouple(CONFcouple** conf, const QString& containerName)
	{
		if (containerName == "")
		{
			this->encoderPlugin->getConfiguration(conf);
		}
		else if (containerName == "lavcSettings")
		{
			char *configData;

			this->encoderPlugin->getConfiguration(conf);
			(*conf)->readAsString("lavcSettings", &configData);
			delete *conf;

			getCoupleFromString(conf, configData, FFcodecContext_param);
			delete configData;
		}
		else
		{
			*conf = NULL;
		}
	}

	void VideoEncoder::setConfCouple(CONFcouple* conf, const QString& containerName)
	{
		if (containerName == "")
		{
			this->encoderPlugin->setConfiguration(conf, true);
		}
		else if (containerName == "lavcSettings")
		{
			char *confString;
			CONFcouple *mainConf;

			lavCoupleToString(conf, &confString);
			this->encoderPlugin->getConfiguration(&mainConf);

			mainConf->updateValue(mainConf->lookupName("lavcSettings"), confString);
			this->encoderPlugin->setConfiguration(mainConf, true);

			delete [] confString;
			delete mainConf;
		}
	}
}

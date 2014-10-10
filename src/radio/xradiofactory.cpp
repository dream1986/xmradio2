/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QMap>
#include <QCoreApplication>
#include <QStringList>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>

#include "xradiofactory.h"
#include "xradio.h"
#include "xradioservice.h"
#include "xradioserviceplugin.h"

XMR_BEGIN_NS

class XRadioFactoryPrivate
{
public:
	explicit XRadioFactoryPrivate();
	~XRadioFactoryPrivate();

	void loadRadio();
	const QList<XRadio*>& availableRadio() const;
	XRadio* getRadioByIndex(int index);

	typedef QMap<XRadioService*, XRadioServiceFactoryInterface*> SERVICEMAP;
	SERVICEMAP m_services;
	QList<XRadio *> m_radioList;
};

XRadioFactoryPrivate::XRadioFactoryPrivate()
{
	loadRadio();
}

XRadioFactoryPrivate::~XRadioFactoryPrivate()
{
	SERVICEMAP::iterator it = m_services.begin();
	for ( ; it != m_services.end(); ++it)
	{
		(it.value())->release(it.key());
	}

	foreach(XRadio *radio, m_radioList)
	{
		delete radio;
	}
}

void XRadioFactoryPrivate::loadRadio()
{
	const QLatin1String suffix("/radio/");
	const QStringList paths = QCoreApplication::libraryPaths();

	foreach(QString path, paths)
	{
		const QString libPath = path + suffix;
		const QDir dir(libPath);
		if (!dir.exists())
			continue;

		QStringList plugins(dir.entryList(QDir::Files));

		foreach(QString plugin, plugins)
		{
			QPluginLoader pluginLoader(libPath + plugin);
			if (!pluginLoader.load())
			{
				qDebug() << "plugin load failed:" << pluginLoader.errorString();
				continue;
			}

			QObject *obj = pluginLoader.instance();
			if (obj && qobject_cast<XRadioServiceFactoryInterface *>(obj))
			{
				XRadioServiceFactoryInterface *factory = qobject_cast<XRadioServiceFactoryInterface *>(obj);
				XRadioService* service = factory->create();
				if (service)
				{
					m_services[service] = factory;
					m_radioList.push_back(new XRadio(service));
				}
			}
			else
			{
				pluginLoader.unload();
			}
		}
	}
}

inline const QList<XRadio *> &XRadioFactoryPrivate::availableRadio() const
{
	return m_radioList;
}

inline XRadio* XRadioFactoryPrivate::getRadioByIndex(int index)
{
	if (index < 0 || index >= m_radioList.count())
		return NULL;

	return m_radioList[index];
}

// ==================================================================================

XRadioFactory::XRadioFactory(QObject *parent)
	: QObject(parent)
	, d_ptr(new XRadioFactoryPrivate)
{
}

XRadioFactory::~XRadioFactory()
{
	delete d_ptr;
}

XRadioFactory* XRadioFactory::instance()
{
	static XRadioFactory s_factory;
	return &s_factory;
}

const QList<XRadio *>& XRadioFactory::availableRadio()
{
	Q_D(XRadioFactory);
	return d->availableRadio();
}

XRadio* XRadioFactory::getRadioByIndex(int index)
{
	Q_D(XRadioFactory);
	return d->getRadioByIndex(index);
}

XMR_END_NS

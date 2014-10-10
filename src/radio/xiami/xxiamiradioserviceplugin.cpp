/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QDebug>
#include <radio/xradioserviceplugin.h>
#include "xxiamiradio.h"

XMR_BEGIN_NS

class XXiaMiRadioServicePlugin
	: public XRadioServicePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID XRadioServiceFactoryInterface_iid)
public:
	virtual XRadioService* create()
	{
		return new XXiaMiRadio;
	}

	virtual void release(XRadioService *service)
	{
		delete service;
	}

	virtual QString identifier()
	{
		return "xiamiradio";
	}
};

XMR_END_NS

#include "xxiamiradioserviceplugin.moc"

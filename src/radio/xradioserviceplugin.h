/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __X_RADIO_PLUGIN_H__
#define __X_RADIO_PLUGIN_H__

#include <QObject>
#include <xglobal.h>

XMR_BEGIN_NS

class XRadioService;

struct XRadioServiceFactoryInterface
{
	virtual ~XRadioServiceFactoryInterface() {}
	virtual XRadioService* create() = 0;
	virtual void release(XRadioService *servuce) = 0;
	virtual QString identifier() = 0;
};

XMR_END_NS

#define XRadioServiceFactoryInterface_iid "xmradio2.plugins.radioservicefactoryinterface/2.0"
Q_DECLARE_INTERFACE(XMR_NS::XRadioServiceFactoryInterface, XRadioServiceFactoryInterface_iid)

XMR_BEGIN_NS

class XRadioServicePlugin
	: public QObject
	, public XRadioServiceFactoryInterface
{
	Q_OBJECT
	Q_INTERFACES(XMR_NS::XRadioServiceFactoryInterface)
public:
	virtual XRadioService* create() = 0;
	virtual void release(XRadioService *service) = 0;
	virtual QString identifier() = 0;
};

XMR_END_NS

#endif // __X_RADIO_PLUGIN_H__

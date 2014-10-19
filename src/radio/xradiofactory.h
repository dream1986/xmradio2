/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __X_RADIO_FACTORY_H__
#define __X_RADIO_FACTORY_H__

#include <QObject>
#include <xglobal.h>
#include "radio_export.h"

XMR_BEGIN_NS

class XRadio;
class XRadioFactoryPrivate;

class RADIO_EXPORT XRadioFactory : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(XRadioFactory)
protected:
	explicit XRadioFactory(QObject *parent = 0);
	~XRadioFactory();

public:
	static XRadioFactory* instance();

	const QList<XRadio *>& availableRadio();
	XRadio* getRadio(int index);
	XRadio* defaultRadio();

private:
	XRadioFactoryPrivate *d_ptr;
};

XMR_END_NS

#endif // __X_RADIO_FACTORY_H__

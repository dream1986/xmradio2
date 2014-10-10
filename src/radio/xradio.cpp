/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QDebug>

#include "xradio.h"
#include "xradioservice.h"

XMR_BEGIN_NS

class XRadioPrivate
{
public:
	XRadioPrivate(XRadioService *service);

	XRadioService *service;
};

XRadioPrivate::XRadioPrivate(XRadioService *service)
	: service(service)
{
	Q_ASSERT(service != NULL);
}

// ==============================================================

XRadio::XRadio(XRadioService *service, QObject *parent)
	: QObject(parent)
	, d_ptr(new XRadioPrivate(service))
{
	Q_D(XRadio);
	connect(d->service, SIGNAL(musicAvailable(XMusicInfo)), SIGNAL(musicAvailable(XMusicInfo)));
	connect(d->service, SIGNAL(stateChanged(int,int)), SIGNAL(stateChanged(int,int)));
}

XRadio::~XRadio()
{
	qDebug() << "~XRadio";
	delete d_ptr;
}

QString XRadio::name()
{
	return d_func()->service->name();
}

bool XRadio::nextMusic(XMusicInfo &music)
{
	return d_func()->service->nextMusic(music);
}

QList<XRadioStyle *> XRadio::radioStyles()
{
	return d_func()->service->radioStyles();
}

bool XRadio::changeStyle(XRadioStyle *style)
{
	return d_func()->service->changeStyle(style);
}

void XRadio::login(const QMap<QString, QVariant> &auths)
{
	d_func()->service->login(auths);
}

XMR_END_NS

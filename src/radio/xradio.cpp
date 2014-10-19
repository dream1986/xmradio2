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
	connect(d->service, SIGNAL(musicAvailable()), SIGNAL(musicAvailable()));
	connect(d->service, SIGNAL(styleChanged(XRadioStyle*)), SIGNAL(styleChanged(XRadioStyle*)));
	connect(d->service, SIGNAL(stateChanged(int,int)), SLOT(stateChanged(int,int)));
	connect(d->service, SIGNAL(needLogin(QMap<QString,QPair<QString,int> >)), SIGNAL(needLogin(QMap<QString,QPair<QString,int> >)));
}

XRadio::~XRadio()
{
	qDebug() << "~XRadio";
}

QString XRadio::name()
{
	return d_func()->service->name();
}

QList<XRadioStyle *> XRadio::radioStyles()
{
	return d_func()->service->radioStyles();
}

XRadioStyle* XRadio::defaultStyle()
{
	return d_func()->service->defaultRadioStyle();
}

bool XRadio::nextMusic(XMusicInfo &music)
{
	return d_func()->service->nextMusic(music);
}

void XRadio::rateMusic(int rate)
{
	d_func()->service->rateMusic(rate);
}

bool XRadio::changeStyle(XRadioStyle *style)
{
	return d_func()->service->changeStyle(style);
}

void XRadio::login(const QMap<QString, QVariant> &auths)
{
	d_func()->service->login(auths);
}

bool XRadio::isLoggedIn()
{
	return d_func()->service->isLoggedIn();
}

void XRadio::logout()
{
	d_func()->service->logout();
}

void XRadio::stateChanged(int state, int code)
{
	switch (state)
	{
	case XRadioService::LoggedIn:
		emit loginStateChanged(LoggedIn);
		break;

	case XRadioService::LoggedOut:
		emit loginStateChanged(LoggedOut);
		break;

	case XRadioService::LoginFailed:
		emit loginStateChanged(LoginFailed);
		break;

	case XRadioService::NetworkError:
		emit networkError(code);
		break;

	case XRadioService::NoData:
		emit serviceError();
		break;
	}
}

XMR_END_NS

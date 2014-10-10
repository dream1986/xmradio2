/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __X_RADIO_STYLE_H__
#define __X_RADIO_STYLE_H__

#include <QString>
#include <QList>
#include <xglobal.h>

XMR_BEGIN_NS

class XRadioStyle
{
	Q_DISABLE_COPY(XRadioStyle)
public:
	XRadioStyle(const QString &name,
				const QString &logo = QString())
		: m_name(name)
		, m_logo(logo)
	{
	}

	virtual ~XRadioStyle() {}

	const QString& name() const
	{
		return m_name;
	}

	const QString& logo() const
	{
		return m_logo;
	}

	const QList<XRadioStyle *>& subStyles() const
	{
		return m_substyles;
	}

protected:
	void setName(const QString &name)
	{
		m_name = name;
	}

	void setLogo(const QString &logo)
	{
		m_logo = logo;
	}

	void setSubStyles(const QList<XRadioStyle *>& substyles)
	{
		m_substyles = substyles;
	}

protected:
	QString m_name;
	QString m_logo;
	QList<XRadioStyle *> m_substyles;
};

XMR_END_NS

#endif // __X_RADIO_STYLE_H__

/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __X_UNZIP_H__
#define __X_UNZIP_H__

#include <QString>
#include <QScopedPointer>

#include "xglobal.h"

XMR_BEGIN_NS

class XUnZipPrivate;

class CORE_EXPORT XUnZip
{
	Q_DECLARE_PRIVATE(XUnZip)
public:
	enum Status
	{
		NoError = 0,
		UncompressError,
		FileError,
		FileNotFound,
	};

public:
	XUnZip(const QString &archive = QString());
	~XUnZip();

	void setArchive(const QString &archive);
	const QString& getArchive() const;

	QByteArray getData(const QString &name, int *error = 0);

	Status status() const;

private:
	QScopedPointer<XUnZipPrivate> d_ptr;
};

XMR_END_NS

#endif // __X_UNZIP_H__

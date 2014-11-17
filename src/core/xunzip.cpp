/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "xunzip.h"
#include "3rdparty/minizip/unzip.h"

XMR_BEGIN_NS

class XUnZipPrivate
{
public:
	XUnZipPrivate();
	~XUnZipPrivate();

	void cleanup();

	QString archiveFile;
	unzFile handle;
	XUnZip::Status status;
};

XUnZipPrivate::XUnZipPrivate()
	: handle(NULL)
	, status(XUnZip::NoError)
{
}

XUnZipPrivate::~XUnZipPrivate()
{
	cleanup();
}

void XUnZipPrivate::cleanup()
{
	if (handle)
	{
		unzCloseCurrentFile(handle);
		unzClose(handle);
		handle = NULL;
	}

	status = XUnZip::NoError;
}

// ==============================================================

XUnZip::XUnZip(const QString &archive)
	: d_ptr(new XUnZipPrivate)
{
	setArchive(archive);
}

XUnZip::~XUnZip()
{

}

void XUnZip::setArchive(const QString &archive)
{
	Q_D(XUnZip);
	d->archiveFile = archive;
	d->cleanup();

	d->handle = unzOpen(d->archiveFile.toLocal8Bit().constData());
	if (d->handle == NULL)
	{
		d->status = FileError;
	}
}

const QString& XUnZip::getArchive() const
{
	return d_func()->archiveFile;
}

QByteArray XUnZip::getData(const QString &name, int *error /*= 0*/)
{
	Q_D(XUnZip);

	QByteArray data;

	do
	{
		if (d->handle == NULL)
		{
			d->status = FileError;
			break;
		}

		int result = unzLocateFile(d->handle, name.toLocal8Bit().constData(), 2);
		if (result != UNZ_OK)
		{
			d->status = FileNotFound;
			break;
		}

		unz_file_info file_info;
		result = unzGetCurrentFileInfo(d->handle, &file_info, NULL, 0, NULL, 0, NULL, 0);
		if (result != UNZ_OK)
		{
			d->status = UncompressError;
			break;
		}

		result = unzOpenCurrentFile(d->handle);
		if (result != UNZ_OK)
		{
			d->status = UncompressError;
			break;
		}

		data.resize(file_info.uncompressed_size);
		char *buffer = data.data();
		result = unzReadCurrentFile(d->handle, buffer, data.length());
		if (result < 0)
		{
			d->status = UncompressError;
		}

		unzCloseCurrentFile(d->handle);
	} while (0);

	if (error)
	{
		*error = d->status;
	}

	return data;
}

XUnZip::Status XUnZip::status() const
{
	return d_func()->status;
}

XMR_END_NS

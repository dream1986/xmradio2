/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __RADIO_EXPORT_H__
#define __RADIO_EXPORT_H__

#ifndef RADIO_EXPORT
#  ifdef RADIO_LIB
#    define RADIO_EXPORT Q_DECL_EXPORT
#  else
#    define RADIO_EXPORT Q_DECL_IMPORT
#  endif
#endif

#endif // __RADIO_EXPORT_H__

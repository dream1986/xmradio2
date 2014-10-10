/*
 * Copyright (c) 2014 The xmradio Authors. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __X_GLOBAL_H__
#define __X_GLOBAL_H__

#define XMR_NS xmrns
#define XMR_BEGIN_NS namespace XMR_NS {
#define XMR_END_NS }
#define XMR_USE_NS using namespace XMR_NS;

#ifndef countof
#  define countof(x) (sizeof(x) / sizeof(x[0]))
#endif

#endif // __X_GLOBAL_H__

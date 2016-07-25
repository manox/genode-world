/*
 * \brief  Interface used by the backend to transfer ROM content to the remote clients.
 * \author Johannes Schlatow
 * \date   2016-02-17
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef __INCLUDE__REMOTE_ROM__ROM_FORWARDER_H_
#define __INCLUDE__REMOTE_ROM__ROM_FORWARDER_H_

#include <base/stdint.h>

namespace Remote_rom {
	using Genode::size_t;
	struct Rom_forwarder_base;
}

struct Remote_rom::Rom_forwarder_base
{
	virtual const char *module_name() const = 0;
	virtual size_t content_size() const = 0;
	virtual size_t transfer_content(char *dst, size_t dst_len, size_t offset=0) const = 0;
};

#endif

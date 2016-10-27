/*
 * \brief  Client-side Zynq PL IRQ session interface
 * \author Mark Albers
 * \date   2016-01-25
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__PL_IRQ_SESSION_H__CLIENT_H_
#define _INCLUDE__PL_IRQ_SESSION_H__CLIENT_H_

#include <pl_irq_session/zynq/capability.h>
#include <base/rpc_client.h>

namespace Pl_irq {

	struct Session_client : Genode::Rpc_client<Session>
	{
		explicit Session_client(Session_capability session)
		: Genode::Rpc_client<Session>(session) { }

        bool wait() { return call<Rpc_wait>(); }
	};
}

#endif /* _INCLUDE__PL_IRQ_SESSION_H__CLIENT_H_ */

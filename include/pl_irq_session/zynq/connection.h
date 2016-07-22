/*
 * \brief  Connection to Zynq PL IRQ session
 * \author Mark Albers
 * \date   2016-01-25
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__PL_IRQ_SESSION__CONNECTION_H_
#define _INCLUDE__PL_IRQ_SESSION__CONNECTION_H_

#include <pl_irq_session/zynq/client.h>
#include <base/connection.h>

namespace Pl_irq {

	struct Connection : Genode::Connection<Session>, Session_client
	{
        Connection(unsigned pl_irq_number)
        : Genode::Connection<Session>(session("ram_quota=8K, pl_irq=%zd", pl_irq_number)),
		  Session_client(cap()) { }
	};
}

#endif /* _INCLUDE__PL_IRQ_SESSION__CONNECTION_H_ */

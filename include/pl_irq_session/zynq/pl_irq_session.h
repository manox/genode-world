/*
 * \brief  Zynq PL IRQ session interface
 * \author Mark Albers
 * \date   2016-01-25
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__PL_IRQ_SESSION__VDMA_SESSION_H_
#define _INCLUDE__PL_IRQ_SESSION__VDMA_SESSION_H_

#include <base/signal.h>
#include <session/session.h>

namespace Pl_irq {

	struct Session : Genode::Session
	{
        static const char *service_name() { return "Pl_irq"; }

		virtual ~Session() { }

        virtual bool wait() = 0;

		/*******************
		 ** RPC interface **
		 *******************/

        GENODE_RPC(Rpc_wait, bool, wait);

        GENODE_RPC_INTERFACE(Rpc_wait);
    };
}

#endif /* _INCLUDE__PL_IRQ_SESSION__VDMA_SESSION_H_ */

/*
 * \brief  Zynq PL IRQ session capability type
 * \author Mark Albers
 * \date   2016-01-25
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__PL_IRQ_SESSION__CAPABILITY_H_
#define _INCLUDE__PL_IRQ_SESSION__CAPABILITY_H_

#include <base/capability.h>
#include <pl_irq_session/zynq/pl_irq_session.h>

namespace Pl_irq { typedef Genode::Capability<Session> Session_capability; }

#endif /* _INCLUDE__PL_IRQ_SESSION__CAPABILITY_H_ */

/*
 * \brief  PL IRQ Handler for Zynq
 * \author Mark Albers
 * \date   2016-01-25
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _DRIVERS__PL_IRQ__ZYNQ__HANDLER_H_
#define _DRIVERS__PL_IRQ__ZYNQ__HANDLER_H_

#include <io_mem_session/connection.h>
#include <util/mmio.h>
#include <vector>
#include <new>
#include "pl_irq.h"

namespace Pl_irq {
	using namespace Genode;
    class Handler;
}

class Pl_irq::Handler
{
    private:

        std::vector<Zynq_Pl_irq*> _pl_irq_bank;

        Handler(std::vector<unsigned> irqs)
        {
            for (unsigned i = 0; i < irqs.size(); i++)
            {
                _pl_irq_bank.push_back(new Zynq_Pl_irq(irqs[i]));
            }
        }

        ~Handler()
        {
            for (std::vector<Zynq_Pl_irq*>::iterator it = _pl_irq_bank.begin() ; it != _pl_irq_bank.end(); ++it)
            {
                delete (*it);
            }
            _pl_irq_bank.clear();
        }

	public:

        static Handler& factory(std::vector<unsigned> irqs);

        bool wait(unsigned irq_nr)
        {
            Zynq_Pl_irq *pl_irq_reg = _pl_irq_bank[irq_nr];
            pl_irq_reg->wait();
            return true;
        }
};

Pl_irq::Handler& Pl_irq::Handler::factory(std::vector<unsigned> irqs)
{
    static Pl_irq::Handler handler(irqs);
    return handler;
}

#endif /* _DRIVERS__PL_IRQ__ZYNQ__HANDLER_H_ */

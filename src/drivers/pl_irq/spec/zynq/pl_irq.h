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

#ifndef _PL_IRQ_H_
#define _PL_IRQ_H_

#include <irq_session/connection.h>

namespace Pl_irq {
	using namespace Genode;
    class Zynq_Pl_irq;
}

class Pl_irq::Zynq_Pl_irq : public Genode::Irq_connection
{
    private:

        Genode::Signal_receiver _irq_rec;
        Genode::Signal_context  _irq_ctx;

    public:

        Zynq_Pl_irq(Genode::Env &env, Genode::uint16_t number) : Irq_connection(env, number)
        {
            Irq_connection::sigh(_irq_rec.manage(&_irq_ctx));
            Irq_connection::ack_irq();
        }

        ~Zynq_Pl_irq()
        {
            _irq_rec.dissolve(&_irq_ctx);
        }

        void wait()
        {
            _irq_rec.wait_for_signal();
            Irq_connection::ack_irq();
        }
};

#endif // _PL_IRQ_H_

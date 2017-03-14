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

#include <pl_irq_session/zynq/pl_irq_session.h>
#include <base/attached_rom_dataspace.h>
#include <dataspace/client.h>
#include <base/log.h>
#include <base/heap.h>
#include <root/component.h>
#include <vector>
#include <libc/component.h>
#include "handler.h"

namespace Pl_irq {
	using namespace Genode;
	class Session_component;
	class Root;
};

class Pl_irq::Session_component : public Genode::Rpc_object<Pl_irq::Session>
{
	private:

        Handler &_handler;
        unsigned _number;

	public:

        Session_component(Handler &handler, unsigned irq_pl_number) : _handler(handler), _number(irq_pl_number) {}

        virtual bool wait()
        {
            return _handler.wait(_number);
        }
};

class Pl_irq::Root : public Genode::Root_component<Pl_irq::Session_component>
{
	private:

        Handler &_handler;

	protected:

		Session_component *_create_session(const char *args)
		{
            unsigned number = Genode::Arg_string::find_arg(args, "pl_irq").ulong_value(0);
            Genode::size_t ram_quota = Genode::Arg_string::find_arg(args, "ram_quota").ulong_value(0);

            if (ram_quota < sizeof(Session_component)) {
                Genode::warning("Insufficient dontated ram_quota (", ram_quota," bytes), require ", sizeof(Session_component), " bytes");
                throw Genode::Root::Quota_exceeded();
            }

            return new (md_alloc()) Session_component(_handler, number);
		}

	public:

		Root(Genode::Rpc_entrypoint *session_ep,
             Genode::Allocator *md_alloc, Handler &handler)
        : Genode::Root_component<Pl_irq::Session_component>(session_ep, md_alloc),
          _handler(handler) { }
};


struct Main
{
	Genode::Env         &env;
	Genode::Sliced_heap  sliced_heap;

	Genode::Attached_rom_dataspace config_rom { env, "config" };

	Main(Genode::Env &env)
	:
		env(env),
		sliced_heap(env.ram(), env.rm())
	{
		Genode::log("Zynq PL IRQ Hander");

        /*
         * Read config
         */
        std::vector<unsigned> irqs;

        try {
            Genode::Xml_node pl_irq_node = config_rom.xml().sub_node("irq");

            for (unsigned i = 0; ;i++, pl_irq_node = pl_irq_node.next("irq"))
            {
                irqs.push_back(0);
                pl_irq_node.attribute("number").value(&irqs[i]);
                Genode::log("PL IRQ with number ", irqs[i], " added.");

                if (pl_irq_node.is_last("irq")) break;
            }
        }
        catch (Genode::Xml_node::Nonexistent_sub_node) {
            Genode::warning("No PL IRQ config");
        }

        /*
         * Create handler
         */
        Pl_irq::Handler &handler = Pl_irq::Handler::factory(env, irqs);
        irqs.clear();

		/*
		 * Announce service
		 */
        static Pl_irq::Root root(&env.ep().rpc_ep(), &sliced_heap, handler);
		env.parent().announce(env.ep().manage(root));
	}
};

void Libc::Component::construct(Libc::Env &env) { static Main main(env); }

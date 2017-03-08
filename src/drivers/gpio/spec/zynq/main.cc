/*
 * \brief  Gpio Driver for Zynq
 * \author Mark Albers
 * \date   2015-03-30
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <gpio_session/zynq/gpio_session.h>
#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <dataspace/client.h>
#include <base/log.h>
#include <base/heap.h>
#include <root/component.h>
#include <vector>
#include <libc/component.h>
#include "driver.h"

namespace Gpio {
	using namespace Genode;
	class Session_component;
	class Root;
};


class Gpio::Session_component : public Genode::Rpc_object<Gpio::Session>
{
	private:

		Driver &_driver;
		unsigned _number;

	public:

		Session_component(Driver &driver, unsigned gpio_number)
		: _driver(driver), _number(gpio_number) {}

		virtual Genode::uint32_t read(bool isChannel2)
		{
			return _driver.read(_number, isChannel2);
		}

		virtual bool write(Genode::uint32_t data, bool isChannel2)
		{
			return _driver.write(_number, data, isChannel2);
		}
};


class Gpio::Root : public Genode::Root_component<Gpio::Session_component>
{
	private:

		Driver &_driver;

	protected:

		Session_component *_create_session(const char *args)
		{
			unsigned number = Genode::Arg_string::find_arg(args, "gpio").ulong_value(0);
			Genode::size_t ram_quota = Genode::Arg_string::find_arg(args, "ram_quota").ulong_value(0);

			if (ram_quota < sizeof(Session_component)) {
				Genode::warning("Insufficient dontated ram_quota (", ram_quota, " bytes), "
				                "require ", sizeof(Session_component), " bytes");
				throw Genode::Root::Quota_exceeded();
			}

			return new (md_alloc()) Session_component(_driver, number);
		}

	public:

		Root(Genode::Rpc_entrypoint *session_ep,
		     Genode::Allocator *md_alloc, Driver &driver)
		:
			Genode::Root_component<Gpio::Session_component>(session_ep, md_alloc),
			_driver(driver)
		{ }
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
		Genode::log("Zynq Gpio driver");

        /*
         * Read config
         */
        std::vector<Genode::addr_t> addr;

        try {
            Genode::Xml_node gpio_node = config_rom.xml().sub_node("gpio");

            for (unsigned i = 0; ;i++, gpio_node = gpio_node.next("gpio"))
            {
                addr.push_back(0);
                gpio_node.attribute("addr").value(&addr[i]);
                Genode::log("Gpio with mio address ", Genode::Hex(addr[i]), " added.");

                if (gpio_node.is_last("gpio")) break;
            }
        }
        catch (Genode::Xml_node::Nonexistent_sub_node) {
        Genode::warning("No Gpio config");
        }

        /*
         * Create Driver
         */
        Gpio::Driver &driver = Gpio::Driver::factory(env, addr);
        addr.clear();

		/*
		 * Announce service
		 */
        static Gpio::Root root(&env.ep().rpc_ep(), &sliced_heap, driver);
		env.parent().announce(env.ep().manage(root));
	}
};


void Libc::Component::construct(Libc::Env &env) { static Main main(env); }

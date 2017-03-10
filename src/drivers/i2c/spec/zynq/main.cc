/*
 * \brief  I2C Driver for Zynq
 * \author Mark Albers
 * \date   2015-03-12
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <i2c_session/zynq/i2c_session.h>
#include <dataspace/client.h>
#include <base/log.h>
#include <base/heap.h>
#include <root/component.h>
#include <base/component.h>
#include "driver.h"

namespace I2C {
	using namespace Genode;
	class Session_component;
	class Root;
};

class I2C::Session_component : public Genode::Rpc_object<I2C::Session>
{
	private:
		Driver &_driver;
		unsigned int _bus;
		Genode::Signal_context_capability _sigh;

	public:

		Session_component(Driver &driver, unsigned int bus_num) : _driver(driver), _bus(bus_num) {}

		virtual bool read_byte_16bit_reg(Genode::uint8_t adr, Genode::uint16_t reg, Genode::uint8_t *data)
		{
			return _driver.read_byte_16bit_reg(_bus, adr, reg, data);
		}

		virtual bool write_16bit_reg(Genode::uint8_t adr, Genode::uint16_t reg,
			Genode::uint8_t data)
		{
			return _driver.write_16bit_reg(_bus, adr, reg, data);
		}
};

class I2C::Root : public Genode::Root_component<I2C::Session_component>
{
	private:

		Driver &_driver;

	protected:

		Session_component *_create_session(const char *args)
		{
			unsigned int bus = Genode::Arg_string::find_arg(args, "bus").ulong_value(0);
			return new (md_alloc()) Session_component(_driver, bus);
		}

	public:

		Root(Genode::Rpc_entrypoint *session_ep,
		     Genode::Allocator *md_alloc, Driver &driver)
		: Genode::Root_component<I2C::Session_component>(session_ep, md_alloc),
		  _driver(driver) { }
};


struct Main
{
	Genode::Env         &env;
	Genode::Sliced_heap  sliced_heap;

	Main(Genode::Env &env)
	:
		env(env),
		sliced_heap(env.ram(), env.rm())
	{
		Genode::log("Zynq I2C driver");

        /*
         * Create Driver
         */
        I2C::Driver &driver = I2C::Driver::factory(env);

		/*
		 * Announce service
		 */
        static I2C::Root root(&env.ep().rpc_ep(), &sliced_heap, driver);
		env.parent().announce(env.ep().manage(root));
	}
};


void Component::construct(Genode::Env & env) { static Main main(env); }

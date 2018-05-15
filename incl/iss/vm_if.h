/*******************************************************************************
 * Copyright (C) 2017, MINRES Technologies GmbH
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Contributors:
 *       eyck@minres.com - initial API and implementation
 ******************************************************************************/

#ifndef _VM_IF_H_
#define _VM_IF_H_

#include "vm_types.h"
#include <string>
#include <memory>

namespace iss {
// forward declaration
class arch_if;
class vm_plugin;

class vm_if { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	/**
	 * register a plugin to the virtual machine
	 *
	 * @param plugin reference to the plugin to be registered
	 */
	virtual void register_plugin(vm_plugin& plugin) = 0;
    /**
     * get the underlying class of the core to be simulated
     *
     * @return pointer to the core
     */
    virtual arch_if *get_arch() = 0;
    /**
     * start the simulation
     *
     * @param cycles number if instructions to be simulated
     * @return number of executed instructions
     */
    virtual int start(uint64_t icount = std::numeric_limits<uint64_t>::max(), bool dump = false) = 0;
    /**
     * reset the core
     *
     * @param address start address after reset
     */
    virtual void reset(uint64_t address) = 0;
    /**
     * reset the core starting at address 0
     */
    virtual void reset() = 0;
    /**
     * Synchronization point at the before executing the next instruction
     */
    virtual void pre_instr_sync() = 0;
    /**
     * check if instruction disassembly is enabled
     *
     * @return true if enabled
     */
    bool isDisassEnabled() { return disass_enabled; }
    /**
     * set the disassembly flag
     *
     * @param enable the flag to enable disassembling
     */
    void setDisassEnabled(bool enable) { disass_enabled = enable; }

protected:
    bool disass_enabled;
};
/**
 * exception class signaling an error while decoding an instruction
 */
class decoding_error : public std::runtime_error {
public:
    const uint64_t addr;
    decoding_error(uint64_t a)
    : std::runtime_error("decoding error")
    , addr(a) {}
};
/**
 * exception class signaling end of simulation, state contains the result
 */
class simulation_stopped : public std::runtime_error {
public:
    const int state;
    simulation_stopped(int s)
    : std::runtime_error("simulation stopped")
    , state(s) {}
};
}

#endif /* _VM_IF_H_ */

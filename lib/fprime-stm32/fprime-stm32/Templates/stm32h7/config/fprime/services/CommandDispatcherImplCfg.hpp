/**
 * \file
 * \brief Project override for Svc::CommandDispatcher table sizing
 *
 * The framework default (150-entry dispatch table) is oversized for this
 * deployment's 48 declared command opcodes. Sized here with headroom for the
 * upcoming USART1 DMA ground-link driver's commands, to reclaim static AXI
 * SRAM `.bss` for the bare-metal STM32H753 image.
 */

#ifndef CMDDISPATCHER_COMMANDDISPATCHERIMPLCFG_HPP_
#define CMDDISPATCHER_COMMANDDISPATCHERIMPLCFG_HPP_

#include <Fw/FPrimeBasicTypes.hpp>

enum {
    // Must be >= number of opcodes registered in the topology (48 today).
    // 64 leaves headroom for new commands (e.g. the USART driver).
    CMD_DISPATCHER_DISPATCH_TABLE_SIZE = 64,  // !< The size of the table holding opcodes to dispatch
    // Number of commands that may be in-progress (awaiting a response)
    // simultaneously. The single-threaded cyclic executive dispatches one
    // command response at a time; 16 preserves ample margin over observed
    // usage while reclaiming space versus the framework default of 25.
    CMD_DISPATCHER_SEQUENCER_TABLE_SIZE = 16,  // !< The size of the table holding commands in progress
};

namespace Svc {
namespace CmdDispatcherCfg {

//! Include command opcodes in events when true.
//! When false, opcode fields are set to the maximum FwOpcodeType value.
constexpr bool IncludeCommandOpcodesInEvents = true;

constexpr FwOpcodeType getEventOpcode(const FwOpcodeType opcode) {
    return IncludeCommandOpcodesInEvents ? opcode : std::numeric_limits<FwOpcodeType>::max();
}

}  // namespace CmdDispatcherCfg
}  // namespace Svc

#endif /* CMDDISPATCHER_COMMANDDISPATCHERIMPLCFG_HPP_ */

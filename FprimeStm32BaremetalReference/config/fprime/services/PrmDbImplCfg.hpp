/*
 * PrmDbImplCfg.hpp
 *
 * Project override for Svc::PrmDb sizing.
 *
 * This deployment currently declares zero `param` entries across its
 * topology (bare-metal MicroFs-backed persistence is not yet wired up for
 * parameters). The framework default of 25 static entries is therefore
 * unused reserved space; trimmed to a small number with headroom for
 * near-term additions (e.g. a USART baud-rate/config parameter) to reclaim
 * AXI SRAM `.bss`.
 */

#ifndef PRMDB_PRMDBLIMPLCFG_HPP_
#define PRMDB_PRMDBLIMPLCFG_HPP_

// Anonymous namespace for configuration parameters
namespace {

enum {
    PRMDB_NUM_DB_ENTRIES = 8,     // !< Number of entries in the parameter database (0 in use today; headroom kept)
    PRMDB_ENTRY_DELIMITER = 0xA5  // !< Byte value that should precede each parameter in file; sanity check against
                                  // file integrity. Should match ground system.
};

}

#endif /* PRMDB_PRMDBLIMPLCFG_HPP_ */

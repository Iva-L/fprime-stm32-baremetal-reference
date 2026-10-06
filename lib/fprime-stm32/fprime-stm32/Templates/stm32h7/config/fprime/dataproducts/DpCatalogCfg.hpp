/*
 * DpCatalogCfg.hpp
 *
 * Project override for Svc::DpCatalog sizing.
 *
 * The framework default (127 tracked files) vastly exceeds what the
 * bare-metal `Os::Baremetal::MicroFs` RAM filesystem can hold in this
 * deployment (currently a 3-file, 2x1KB + 1x4KB configuration). Trimmed with
 * headroom over the current MicroFs capacity to reclaim AXI SRAM `.bss`.
 */

#ifndef SVC_DPCATALOG_CONFIG_HPP_
#define SVC_DPCATALOG_CONFIG_HPP_
#include <Fw/FPrimeBasicTypes.hpp>

namespace Svc {
// Sets the maximum number of directories where
// data products can be stored. The array passed
// to the initializer for DpCatalog cannot exceed
// this size.
static const FwIndexType DP_MAX_DIRECTORIES = 2;
static const FwIndexType DP_MAX_FILES = 16;
}  // namespace Svc

#endif /* SVC_DPCATALOG_CONFIG_HPP_ */

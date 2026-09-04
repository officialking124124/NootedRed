// GC 10.3.3 (Rembrandt) Register Offsets
//
// Copyright © 2026 ChefKiss. Licensed under the Thou Shalt Not Profit License version 1.5.
// See LICENSE for details.

#pragma once
#include <IOKit/IOTypes.h>

// Names carry a GC1033_ prefix because this header is included alongside Regs/GC.hpp (GC 9.x),
// whose register names would otherwise collide in the same translation unit.

constexpr UInt32 GC1033_CGTT_SPI_CS_CLK_CTRL                  = 0x507C;
constexpr UInt32 GC1033_CGTT_SPI_CS_CLK_CTRL_BASE_IDX         = 0x1;
constexpr UInt32 GC1033_CH_PIPE_STEER                         = 0x2D90;
constexpr UInt32 GC1033_CH_PIPE_STEER_BASE_IDX                = 0x1;
constexpr UInt32 GC1033_CPF_GCR_CNTL                          = 0x1F53;
constexpr UInt32 GC1033_CPF_GCR_CNTL_BASE_IDX                 = 0x0;
constexpr UInt32 GC1033_DB_DEBUG3                             = 0x13AE;
constexpr UInt32 GC1033_DB_DEBUG3_BASE_IDX                    = 0x0;
constexpr UInt32 GC1033_DB_DEBUG4                             = 0x13AF;
constexpr UInt32 GC1033_DB_DEBUG4_BASE_IDX                    = 0x0;
constexpr UInt32 GC1033_GB_ADDR_CONFIG                        = 0x13DE;
constexpr UInt32 GC1033_GB_ADDR_CONFIG_BASE_IDX               = 0x0;
constexpr UInt32 GC1033_GCR_GENERAL_CNTL                      = 0x1580;
constexpr UInt32 GC1033_GCR_GENERAL_CNTL_BASE_IDX             = 0x0;
constexpr UInt32 GC1033_GL1_PIPE_STEER                        = 0x2D10;
constexpr UInt32 GC1033_GL1_PIPE_STEER_BASE_IDX               = 0x1;
constexpr UInt32 GC1033_GL2_PIPE_STEER_0                      = 0x2E25;
constexpr UInt32 GC1033_GL2_PIPE_STEER_0_BASE_IDX             = 0x1;
constexpr UInt32 GC1033_GL2_PIPE_STEER_1                      = 0x2E26;
constexpr UInt32 GC1033_GL2_PIPE_STEER_1_BASE_IDX             = 0x1;
constexpr UInt32 GC1033_GL2A_ADDR_MATCH_MASK                  = 0x2E21;
constexpr UInt32 GC1033_GL2A_ADDR_MATCH_MASK_BASE_IDX         = 0x1;
constexpr UInt32 GC1033_GL2C_ADDR_MATCH_MASK                  = 0x2E03;
constexpr UInt32 GC1033_GL2C_ADDR_MATCH_MASK_BASE_IDX         = 0x1;
constexpr UInt32 GC1033_GL2C_CM_CTRL1                         = 0x2E08;
constexpr UInt32 GC1033_GL2C_CM_CTRL1_BASE_IDX                = 0x1;
constexpr UInt32 GC1033_GL2C_CTRL3                            = 0x2E0C;
constexpr UInt32 GC1033_GL2C_CTRL3_BASE_IDX                   = 0x1;
constexpr UInt32 GC1033_LDS_CONFIG                            = 0x10A2;
constexpr UInt32 GC1033_LDS_CONFIG_BASE_IDX                   = 0x0;
constexpr UInt32 GC1033_PA_CL_ENHANCE                         = 0x1025;
constexpr UInt32 GC1033_PA_CL_ENHANCE_BASE_IDX                = 0x0;
constexpr UInt32 GC1033_PA_SC_BINNER_TIMEOUT_COUNTER          = 0x1070;
constexpr UInt32 GC1033_PA_SC_BINNER_TIMEOUT_COUNTER_BASE_IDX = 0x0;
constexpr UInt32 GC1033_PA_SC_ENHANCE_2                       = 0x107C;
constexpr UInt32 GC1033_PA_SC_ENHANCE_2_BASE_IDX              = 0x0;
constexpr UInt32 GC1033_TA_CNTL_AUX                           = 0x12E2;
constexpr UInt32 GC1033_TA_CNTL_AUX_BASE_IDX                  = 0x0;
constexpr UInt32 GC1033_UTCL1_CTRL                            = 0x1588;
constexpr UInt32 GC1033_UTCL1_CTRL_BASE_IDX                   = 0x0;

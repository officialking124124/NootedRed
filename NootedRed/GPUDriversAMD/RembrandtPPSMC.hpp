// Rembrandt PowerPlay System Management Controller
//
// Copyright © 2026 ChefKiss. Licensed under the Thou Shalt Not Profit License version 1.5.
// See LICENSE for details.

#pragma once
#include <IOKit/IOTypes.h>

// Names carry a PPSMC13_ prefix because the Raven and Renoir PSMC headers define unprefixed
// names in the same namespace and cannot be included together with them in one TU.

// PMFW response codes
constexpr UInt32 PPSMC13_Result_OK                 = 0x1;
constexpr UInt32 PPSMC13_Result_Failed            = 0xFF;
constexpr UInt32 PPSMC13_Result_UnknownCmd        = 0xFE;
constexpr UInt32 PPSMC13_Result_CmdRejectedPrereq = 0xFD;
constexpr UInt32 PPSMC13_Result_CmdRejectedBusy   = 0xFC;

constexpr UInt32 PPSMC13_MSG_TestMessage            = 0x1;
constexpr UInt32 PPSMC13_MSG_GetSmuVersion          = 0x2;
constexpr UInt32 PPSMC13_MSG_GetDriverIfVersion     = 0x3;
constexpr UInt32 PPSMC13_MSG_EnableGfxOff           = 0x4;
constexpr UInt32 PPSMC13_MSG_DisableGfxOff          = 0x5;
constexpr UInt32 PPSMC13_MSG_PowerDownVcn           = 0x6;
constexpr UInt32 PPSMC13_MSG_PowerUpVcn             = 0x7;
constexpr UInt32 PPSMC13_MSG_SetHardMinVcn          = 0x8;
constexpr UInt32 PPSMC13_MSG_SetSoftMinGfxclk       = 0x9;
constexpr UInt32 PPSMC13_MSG_ActiveProcessNotify    = 0xA;    // Deprecated
constexpr UInt32 PPSMC13_MSG_ForcePowerDownGfx      = 0xB;
constexpr UInt32 PPSMC13_MSG_PrepareMp1ForUnload    = 0xC;
constexpr UInt32 PPSMC13_MSG_SetDriverDramAddrHigh  = 0xD;
constexpr UInt32 PPSMC13_MSG_SetDriverDramAddrLow   = 0xE;
constexpr UInt32 PPSMC13_MSG_TransferTableSmu2Dram = 0xF;
constexpr UInt32 PPSMC13_MSG_TransferTableDram2Smu = 0x10;
constexpr UInt32 PPSMC13_MSG_GfxDeviceDriverReset   = 0x11;
constexpr UInt32 PPSMC13_MSG_GetEnabledSmuFeatures  = 0x12;
constexpr UInt32 PPSMC13_MSG_SetHardMinSocclkByFreq = 0x13;
constexpr UInt32 PPSMC13_MSG_SetSoftMinFclk         = 0x14;
constexpr UInt32 PPSMC13_MSG_SetSoftMinVcn          = 0x15;
constexpr UInt32 PPSMC13_MSG_SPARE                  = 0x16;    // Deprecated
constexpr UInt32 PPSMC13_MSG_GetGfxclkFrequency     = 0x17;
constexpr UInt32 PPSMC13_MSG_GetFclkFrequency       = 0x18;
constexpr UInt32 PPSMC13_MSG_AllowGfxOff            = 0x19;
constexpr UInt32 PPSMC13_MSG_DisallowGfxOff         = 0x1A;
constexpr UInt32 PPSMC13_MSG_SetSoftMaxGfxClk       = 0x1B;
constexpr UInt32 PPSMC13_MSG_SetHardMinGfxClk       = 0x1C;
constexpr UInt32 PPSMC13_MSG_SetSoftMaxSocclkByFreq = 0x1D;
constexpr UInt32 PPSMC13_MSG_SetSoftMaxFclkByFreq   = 0x1E;
constexpr UInt32 PPSMC13_MSG_SetSoftMaxVcn          = 0x1F;
constexpr UInt32 PPSMC13_MSG_SetPowerLimitPercentage = 0x20;
constexpr UInt32 PPSMC13_MSG_PowerDownJpeg          = 0x21;
constexpr UInt32 PPSMC13_MSG_PowerUpJpeg            = 0x22;
constexpr UInt32 PPSMC13_MSG_SetHardMinFclkByFreq   = 0x23;
constexpr UInt32 PPSMC13_MSG_SetSoftMinSocclkByFreq = 0x24;
constexpr UInt32 PPSMC13_MSG_AllowZstates           = 0x25;
constexpr UInt32 PPSMC13_MSG_DisallowZstates        = 0x26;
constexpr UInt32 PPSMC13_MSG_RequestActiveWgp       = 0x27;
constexpr UInt32 PPSMC13_MSG_QueryActiveWgp         = 0x28;
constexpr UInt32 PPSMC13_Message_Count              = 0x29;

// Arguments for PPSMC13_MSG_GfxDeviceDriverReset
constexpr UInt32 PPSMC13_Mode1Reset = 0x1;
constexpr UInt32 PPSMC13_Mode2Reset = 0x2;

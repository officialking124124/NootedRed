# Rembrandt (Radeon 660M / 680M) support

Research and staged port plan for the AMD Yellow Carp family (Rembrandt and
Rembrandt-R: `Radeon 660M`, `Radeon 680M`; Ryzen 6000 and 7035 series, e.g. the
Acer Nitro 5 AN515-47 with Ryzen 5 7535HS). Upstream tracks this work as
ChefKissInc/NootedRed#92; it is a new hardware generation (RDNA 2), not another
Raven-family SKU.

## Status on this branch

- **Done: identification and a safe no-op gate.** `NRed::processPatcher` now
  recognises device IDs `0x1681` and `0x164D`, records the correct external
  revision, and leaves the machine unpatched (previously: boot-time panic
  "Unknown device ID"). No personalities are injected and no Raven-family
  patcher runs on Rembrandt, so the system boots exactly as if NootedRed were
  absent.
- **Done: stage 2/3 data layers** (all differentially verified against Linux):
  `GPUDriversAMD/RembrandtPPSMC.hpp` (complete SMU 13.0.3 message set and PMFW
  response codes), `Regs/GC10_3_3.hpp` (golden-register offsets and segments),
  and `gcGoldenSettingsRembrandt[]` in `GoldenSettings.hpp`. These headers are
  not yet referenced by any translation unit; add them to the Xcode project
  when the stage 3 wiring lands.
- **Not done: acceleration.** Rembrandt is GC 10.3.3 / DCN 3.1.2; every
  hardware-specific layer of this kext targets GC 9.x / DCN 1.x-2.1. The
  remaining stages and blockers are below.

## Hardware identification (verified against Linux)

| Item | Value | Source |
| --- | --- | --- |
| PCI IDs | `1002:1681` (660M/680M), `1002:164D` (secondary Yellow Carp) | `amdgpu_drv.c` |
| External revision | `0x1681`: fixed `0x20`; others: PCI revision + `0x01` | `nv.c`, `IP_VERSION(10, 3, 3)` |
| Graphics | GC 10.3.3 (RDNA 2; 6 CU on 660M, 12 CU on 680M) | kernel APU info table |
| Display | DCN 3.1.2 (DMCUB-based) | kernel APU info table |
| SDMA | 5.2.3 | kernel APU info table |
| SMU / PSP (MP0/MP1) | 13.0.3 | kernel APU info table |
| NBIO | 7.2; `RCC_STRAP0_RCC_DEV0_EPF0_STRAP0` at SEG2 + `0x11` | `nbio_7_2_0_offset.h` |
| MMIO segments | `NBIO_BASE` SEG2 `0xD20`, `DCN_BASE` SEG2 `0x34C0` (same as Raven/Renoir) | `yellow_carp_offset.h` |

The ATI revision strap moved from `RCC_DEV0_EPF0_STRAP0` (SEG2 + `0x0F`, NBIO
7.4, Raven) to `RCC_STRAP0_RCC_DEV0_EPF0_STRAP0` (SEG2 + `0x11`, NBIO 7.2,
Rembrandt); `NRed::hwLateInit` must be adapted once the port reaches stage 2.

## Display registers: DCN 3.1.2

The flip-path HUBP/OTG register offsets of DCN 3.1.2 are identical to DCN 2.1
(Renoir), so the register model of `AMDGFX9DCN2Display` largely carries over
(`dcn_3_1_2_offset.h` vs `Regs/DCN2.hpp`):

| Register | Offset | Stride |
| --- | --- | --- |
| HUBP surface / addr / tiling config | `0x5E5` / `0x5E6` / `0x5E7` | HUBP `0xDC` |
| HUBP primary viewport start / dimension | `0x5E9` / `0x5EA` | |
| HUBPREQ surface pitch | `0x607` | |
| HUBPREQ primary surface address (high) | `0x60A` (`0x60B`) | |
| HUBPREQ flip control | `0x61B` | |
| HUBPREQ surface earliest in-use (high) | `0x625` (`0x626`) | |
| HUBPRET control | `0x66C` | |
| OTG control / interlace control | `0x1B41` / `0x1B44` | OTG `0x80` |

The shift/mask sets were also verified equal to the values hardcoded in
`AMDGFX9DCN2Display::initDCNRegOffs` (`dcn_3_1_2_sh_mask.h`): viewport
Y/height `0x3FFF0000` shift `0x10`, `primarySurfaceHi` `0xFFFF`, flip type bit
1, interlace enable bit 0; the OTG enable field is named `OTG_MASTER_EN` on
DCN 3.1.2 but occupies the same bit 0. Conclusion: reuse the DCN 2.1 display
register model for Rembrandt; a separate DCN 3.1.2 display class is only
needed if reverse engineering reveals behavioural differences. What does
change: DMCUB replaces DMCU (firmware loading and the ABM/backlight path).

## SMU 13.0.3 messages

`GPUDriversAMD/RembrandtPPSMC.hpp` carries the complete Yellow Carp PMFW
interface: 39 messages (`PPSMC13_MSG_TestMessage` `0x1` through
`PPSMC13_MSG_QueryActiveWgp` `0x28`, count `0x29`), the five response codes
(`OK` `0x1`, `Failed` `0xFF`, `UnknownCmd` `0xFE`, `CmdRejectedPrereq` `0xFD`,
`CmdRejectedBusy` `0xFC`), and the mode-1/mode-2 reset arguments, from
`smu_v13_0_1_ppsmc.h`. Names carry a `PPSMC13_` prefix because the Raven and
Renoir PSMC headers define unprefixed names in the same namespace. The
driver-interface version handshake is 4 (`smu13_driver_if_yellow_carp.h`).

## Golden settings

In code: `gcGoldenSettingsRembrandt[]` in `GoldenSettings.hpp`, with register
offsets and segments in `Regs/GC10_3_3.hpp` (`GC1033_` prefix — the GC 9.x
names in `Regs/GC.hpp` would collide in the same translation unit). Source:
Linux `gfx_v10_0.c` `golden_settings_gc_10_3_3[]` (20 entries, including
`GB_ADDR_CONFIG` or-value `0x242`); offsets from `gc_10_1_0_offset.h` plus the
two registers locally defined in `gfx_v10_0.c` (`mmCGTT_SPI_CS_CLK_CTRL`
`0x507C` idx 1, `mmGCR_GENERAL_CNTL_Vangogh` `0x1580` idx 0 — note the plain
`mmGCR_GENERAL_CNTL` is `0x1583`, which is why the suffixed name exists).
SDMA 5.2.3 golden settings have no public source (`sdma_v5_2.c` ships none)
and must be extracted from AMD's driver, as must the DDI caps bitmap.

## Firmware manifest (linux-firmware)

| File | Size (bytes) | Header size | Role |
| --- | --- | --- | --- |
| `yellow_carp_ce.bin` | 263,296 | 44 | GC 10.3.3 CE |
| `yellow_carp_pfp.bin` | 263,424 | 44 | GC 10.3.3 PFP |
| `yellow_carp_me.bin` | 263,424 | 44 | GC 10.3.3 ME |
| `yellow_carp_mec.bin` | 268,160 | 44 | GC 10.3.3 MEC |
| `yellow_carp_mec2.bin` | 268,160 | 44 | GC 10.3.3 MEC2 |
| `yellow_carp_rlc.bin` | 178,128 | 172 | GC 10.3.3 RLC |
| `yellow_carp_sdma.bin` | 34,048 | 48 | SDMA 5.2.3 |
| `yellow_carp_ta.bin` | 246,784 | 36 | PSP 13.0.3 trusted application |
| `yellow_carp_toc.bin` | 1,792 | 68 | PSP 13.0.3 TOC |
| `yellow_carp_dmcub.bin` | 236,048 | 40 | DCN 3.1.2 DMCUB |

Source: the `amdgpu/` directory of linux-firmware (~2.0 MiB total; all ten
downloaded and header-parsed during this work). The CP blobs carry a common
version marker `0x0003000a` in their headers; the exact `AMDFirmware`
descriptor fields (ucode names, payload offsets) for Apple's firmware
directory format must be derived from Apple's kexts or cross-referenced with
NootRX's RDNA 2 constants. VCN 3.1.1 is not needed: NootedRed reports no VCN
support on any ASIC today.

## Port stages

1. **Identification + safe gate** (this branch) — done.
2. **Display bring-up (`X6000FB`)**: Yellow Carp CAIL/ASIC caps entry with DDI
   caps, branding table, reserved-VRAM layout (DMCUB region), NBIO strap read at
   SEG2 + `0x11`, DMCUB firmware load through the driver's DMCUB services (the
   plumbing already exists — the kext currently patches
   `initializeDmcubServices` back to DCN 2.1 semantics). Register data: done
   (reuse DCN 2.1 model, above). Start framebuffer-only with `-NRedNoAccel`.
3. **Firmware + PSP/SMU (`HWLibs`)**: embed the blobs above; PSP 13.0.3
   bootloader/security stubs modelled on the existing PSP 10/12
   reimplementation (`psp_v13_0.c` as reference); SMU 13.0.3 message layer —
   message data done (`RembrandtPPSMC.hpp`) — over the driver's SMU 9
   transport, as done for SMU 10/12.
4. **Accelerator (`X5000`)**: RDNA 2 code paths exist in Apple's
   `AMDRadeonX5000` (NootRX drives Navi 2x dGPUs through them), but every
   `ObjectField` offset table and binary pattern must be re-derived for the
   GFX10 configuration, and addrlib must switch from the Gfx9 spoofs
   (`ADDR_CHIP_FAMILY_AI` via `HwlConvertChipFamily`) to the GFX10/Gfx10Lib
   path.
5. **Integration**: golden settings and caps tables (GC golden done), personality
   `IOPCIPrimaryMatch` entries for `0x1681`/`0x164D`, backlight on DCN 3.1.2,
   end-to-end validation.

## Blockers

- **Apple kext reverse engineering.** The per-macOS-version field offsets and
  byte patterns for the GFX10 configuration can only be derived from Apple's
  kext binaries (`AMDRadeonX5000`, `AMDRadeonX6000Framebuffer`,
  `AMDRadeonX5000HWLibs`) on a Mac. NootRX is the existence proof that the
  RDNA 2 paths are present, and the closest reference.
- **DDI caps bitmap and SDMA 5.2.3 golden settings.** Both must be extracted
  from AMD's Windows driver; there is no public source (same provenance as
  `ddiCapsRaven`/`ddiCapsRenoir`).
- **Build.** Requires Xcode + MacKernelSDK + Lilu (submodules) on macOS. This
  cannot be built on Linux — verified: no Apple toolchain in the development
  sandbox used for this work.
- **Test hardware.** The Acer Nitro 5 AN515-47 (Ryzen 5 7535HS, Radeon 660M,
  `0x1681`) with OpenCore; a stolen-memory/UMA frame buffer of at least 512
  MiB; `-NRedDebug` for diagnostics.

## Stage 1 test plan (this branch)

1. On a Mac: `git submodule update --init`, then
   `xcodebuild -project NootedRed.xcodeproj -target NootedRed -configuration Release`.
2. Inject the built kext into an OpenCore EFI on the AN515-47 with
   `-NRedDebug`.
3. Expected: the two `SYSLOG` lines ("Rembrandt (Radeon 6xxM) iGPU
   detected...", "Leaving the device unpatched..."), the `DBGLOG` diagnostics,
   no panic, and a boot that behaves identically to not injecting the kext.
4. Regression: on a supported Raven/Renoir machine the behaviour must be
   unchanged — the dispatch harness used during development covers every known
   device ID plus the unknown-ID panic path.

## Verification performed on this branch (Linux sandbox)

- Dispatch logic harness (extracted verbatim from `NRed.cpp`): all known device
  IDs map to the correct attributes and enum revisions; unknown IDs still
  panic; the new attribute bit is isolated.
- Differential scripts: 39 SMU messages + 5 response codes vs
  `smu_v13_0_1_ppsmc.h`; 20 golden entries (AND/OR values, offsets, segments)
  vs `gfx_v10_0.c` + `gc_10_1_0_offset.h` + local defines.
- Compile harness: the new headers compile clean (`g++ -std=c++20 -Wall
  -Wextra`) and the `GOLDEN_REGISTER` `reg##_BASE_IDX` token paste resolves
  for every `GC1033_` name.
- Note: GCC rejects the repo's `GOLDEN_REGISTER` macro parameter names (`and`,
  `or` — alternative operator spellings) that Apple clang accepts; harmless on
  the real toolchain.

## Sources

- Linux kernel: `amdgpu/amdgpu_drv.c`, `amdgpu/nv.c` (`IP_VERSION(10, 3, 3)`),
  `include/yellow_carp_offset.h`, `include/asic_reg/dcn/dcn_3_1_2_offset.h`,
  `include/asic_reg/dcn/dcn_3_1_2_sh_mask.h`,
  `include/asic_reg/nbio/nbio_7_2_0_offset.h`, `amdgpu/gfx_v10_0.c`
  (`golden_settings_gc_10_3_3`),
  `pm/swsmu/inc/pmfw_if/smu13_driver_if_yellow_carp.h`,
  `pm/swsmu/inc/pmfw_if/smu_v13_0_1_ppsmc.h`, and the
  [APU info table](https://docs.kernel.org/gpu/amdgpu/amd-hardware-list-info.html).
- [linux-firmware](https://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git/tree/amdgpu)
  (`yellow_carp_*`).
- [ChefKissInc/NootedRed#92](https://github.com/ChefKissInc/NootedRed/issues/92)
  (upstream RDNA iGPU tracking) and
  [NootRX](https://github.com/ChefKissInc/NootRX) (RDNA 2 dGPU patch kext;
  reference for stage 4).

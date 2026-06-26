# Changelog
 All notable changes to this project will be documented in this file.
 The format follows Keep a Changelog and this project adheres to Semantic Versioning.
## [2.0.1] - 2026-06-26
 This release completes the architecture migration from 32-bit to 64-bit x86-64, upgrading the bootloader to GRUB2 with Multiboot2 protocol and enabling long mode.
## Added
 ### 64-bit Architecture Support
  - Full x86-64 long mode support with 4-level page tables (PML4 -> PDPT -> PD -> PT)
  - Identity mapping for the first 4 GiB of physical memory
  - 64-bit GDT and IDT structures with proper long-mode segment descriptors (L=1, D=0)
  - 64-bit interrupt handling with explicit register push/pop and `iretq`
  - System V AMD64 ABI compliant scheduler context switching
 ### Bootloader Upgrade
  - Migrated from 32-bit Multiboot1 to GRUB2 + Multiboot2 protocol
  - Proper Multiboot2 header alignment (8-byte boundary) with entry address tag
 ### Build System
  - Updated compiler flags for 64-bit (`-m64`, `-mno-red-zone`, `-mcmodel=large`)
  - NASM output format changed to `elf64`
  - Linker script updated to `elf64-x86-64` architecture
  - Disabled SSE/AVX generation (`-mno-sse -mno-sse2 -mno-mmx`) for kernel compatibility
## Changed
  - All pointer types (`size_t`, `ptrdiff_t`, `physaddr_t`, `virtaddr_t`) upgraded to 64-bit
  - Interrupt frame structure expanded to 64-bit registers
  - Task stack size increased from 16 KiB to 32 KiB
  - Task context switched from `esp` to `rsp`
## Fixed
  - GDT code segment descriptors now correctly set L-bit (long mode) preventing double faults on interrupt entry
  - Multiboot2 header size alignment causing GRUB `unsupported tag: 0x8` error
  - IDT gate offset handling for 64-bit handler addresses
## File Changes
  - 15+ files modified across boot, core, mm, sched, net, and build system
  - `src/boot/boot.asm`: complete rewrite for 64-bit Multiboot2 + long mode entry
  - `src/kernel/core/gdt.c`: fixed 64-bit segment granularity flags
  - `src/kernel/core/idt.c`, `isr.c`, `isr.asm`: 64-bit IDT/ISR overhaul
  - `src/kernel/sched/scheduler.c`: 64-bit context switch implementation
  - `Makefile`: 64-bit toolchain flags and `-mno-sse`
  - `linker.ld`: 64-bit ELF output format
## [1.2.0] - 2026-06-25
 This release completely redesigns the GUI desktop into a TempleOS-style tiling interface with an interactive graphical shell.
## Added
 ### Tiling GUI Desktop
  - TempleOS-style tiling desktop layout with header bar, left menu panel, right content area, and footer
  - Keyboard-driven menu navigation using arrow keys with yellow highlight selection
  - Enter key to switch between function panels (Shell, Files, Edit, Viewer, CATs)
  - 8x8 bitmap font rendering support for GUI text display
 ### Interactive Graphical Shell
  - Fully interactive shell embedded in the right content panel
  - Command input with backspace support and cursor tracking
  - Supported commands: `ls`, `cd`, `mkdir`, `touch`, `pwd`, `help`, `clear`, `version`, `whoami`, `shutdown`
  - Directory listings with color-coded entries (directories in light blue, files in white)
  - Auto-scroll with screen clearing when output exceeds panel bounds
 ### New Assets
  - `assets/shellgui.png`: screenshot of the interactive shell panel
  - `assets/mew.png`: screenshot of the CAT Viewer panel
## Changed
  - Redesigned `gui` command: replaced Windows 98 style window with pure tiling layout
  - Updated version string to v1.2.0 across boot message, GUI header, and shell commands
  - Updated README with GUI Desktop section and screenshots
## File Changes
  - 5 files changed, 470 insertions(+), 13 deletions(-)
  - `include/gfx/88front.h`: new 8x8 bitmap font data
  - `include/drivers/vga.h`: added font and rectangle drawing declarations
  - `src/kernel/drivers/vga.c`: added `vga_draw_rect()`, `vga_draw_string()`, `vga_draw_char()`
  - `src/kernel/shell/shell.c`: complete GUI rewrite with tiling layout, menu nav, and interactive shell
  - `src/kernel/core/main.c`: updated boot version string
## [1.1.0] - 2026-06-25
 This release introduces graphical display support to Kil0yOS, including a VGA graphics mode and a simple desktop environment.
## Added
 ### Graphical Display System
  - VGA Mode 13h (320x200, 256 colors) support via direct hardware register programming
  - `vga_set_mode_13h()`: switch from text mode to graphics mode
  - `vga_set_text_mode()`: restore standard 80x25 text mode
  - `vga_plot_pixel()`: draw individual pixels in graphics mode
  - `vga_fill_rect()`: draw filled rectangles for GUI rendering
  - `vga_draw_color_bars()`: display standard color bar test pattern
 ### New Shell Commands
  - `gfx`: switch to graphical mode and display a standard color bar test pattern, press `q` to return
  - `gui`: launch a simple Windows 98 style desktop with a cyan background and gray taskbar, press `q` to return
## Changed
  - Updated version string from v1.0.5 to v1.1.0 in kernel boot message and shell `version` command
## File Changes
  - 4 files modified, 190 insertions, 2 deletions
  - include/drivers/vga.h: added graphics mode declarations and constants
  - src/kernel/drivers/vga.c: implemented VGA graphics mode, pixel drawing, rectangle fill, and color bars
  - src/kernel/shell/shell.c: added `gfx` and `gui` commands, updated version string
  - src/kernel/core/main.c: updated boot version string
## [1.0.5] - 2026-06-25
 This is a critical maintenance and feature enhancement release focused on fixing core network subsystem issues and improving overall system stability and reliability.
 All users relying on network functionality are strongly recommended to upgrade.
## Added
 ### New shell network commands:
  - net chknic: List all available network interfaces
  - net wire <interface>: Establish wired network connection
  - ping: Test network connectivity
  - Official driver support for Intel PRO/1000 MT (E1000) NIC
  - Debug logging for received packets in E1000 driver
  - Early exit logic for DHCP client once a valid IP address is acquired
## Changed
  - Extended DHCP client waiting loop from 5 million iterations to 20 million iterations
  - Refractored entire project into categorized modular directories for better code maintainability
  - Rewrote ARP busy-wait logic to return error codes and delegate retry logic to callers
## Fixed
 ### Core Network Stack
  - Missing UDP protocol handling in IPv4 packet dispatcher
  - Failure to broadcast DHCP packets
 ### Intel E1000 NIC Driver
  - Fixed interrupt detection logic inside e1000_poll(), now captures all interrupt sources
  - Fixed driver hang caused by invalid MMIO access patterns
 ### Kernel & File System
  - Multiple critical kernel memory safety and stability defects
  - Fixed file system bugs leading to data corruption and system crashes
  - Fixed issues with ACPI shutdown, file system persistence and general driver reliability
### Known Issues
  - Network interrupt handling implementation remains incomplete; polling mode is recommended during heavy network operations
  - Network adapter configuration adjustments may be required under certain virtual machine environments for full connectivity
## File Changes
  - 7 files modified, 224 insertions, 69 deletions
  - include/include/net/e1000.h: E1000 network driver header
  - include/include/net/net.h: Core network stack header
  - src/kernel/net/e1000.c: E1000 driver implementation
  - src/kernel/net/net.c: Core network stack logic
  - src/kernel/net/rtl8139.c: RTL8139 network driver implementation
  - src/kernel/shell/shell.c: Shell built-in network command implementations
  - .gitignore: Git ignore configuration
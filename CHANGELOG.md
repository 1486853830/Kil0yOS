# Changelog
 All notable changes to this project will be documented in this file.
 The format follows Keep a Changelog and this project adheres to Semantic Versioning.
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
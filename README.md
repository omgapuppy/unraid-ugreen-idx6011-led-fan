# UGREEN iDX6011 LED & Fan Control for Unraid

An LCD-free Unraid plugin for the UGREEN NASync **iDX6011** and **iDX6012**. It drives the front RGB power, two LAN and six disk-bay LEDs without installing display modules, touching EFI boot entries, or changing the Unraid initrd.

## Current features

- Exact DMI allowlist for `iDX6011` and `iDX6012`.
- Per-bay disk health colours: healthy, SMART warning/failure, sleeping and empty.
- Optional controller-driven blinking for disk and network activity.
- Dynamic Intel/ASMedia SATA discovery plus an interactive calibration helper.
- Native Unraid Settings page for colours and behaviour.
- Firmware-managed fan mode is left untouched in the first release. The non-Pro two-fan EC map is known, but writes will only be exposed after physical validation and readback testing.

## Install

In **Plugins → Install Plugin**, paste:

```text
https://raw.githubusercontent.com/omgapuppy/unraid-ugreen-idx6011-led-fan/main/unraid-ugreen-idx6011-led-fan.plg
```

No reboot is required. Settings appear under **Settings → UGREEN iDX6011 Control**.

## Safety

The plugin no-ops on unsupported DMI product names. It unloads the conflicting `led_ugreen` module while active and uses the iDX SMBus-block LED protocol. It does not install LCD/touch drivers, kernel overlays, EFI entries, or fan-write code.

## Credits and licences

This project began as a focused extraction of the LED work in [noodlemctwoodle/unraid-ugreen-idx6011-pro](https://github.com/noodlemctwoodle/unraid-ugreen-idx6011-pro), with thanks to its author for the Unraid packaging, health monitor and physical bay-discovery work.

The Plugins-page icon is the upstream project's original iDX6011 Pro line-art by **Deivizzz**, reused with attribution.

The bundled `ugreen_leds_cli` comes from [klein0r/ugreen_leds_controller](https://github.com/klein0r/ugreen_leds_controller), based on [miskcoo/ugreen_leds_controller](https://github.com/miskcoo/ugreen_leds_controller). The iDX protocol was reverse-engineered in [miskcoo issue #93](https://github.com/miskcoo/ugreen_leds_controller/issues/93). `i2c-tools` is from the Linux I²C tools project. Their GPL-2.0 terms remain in force; see `THIRD-PARTY-NOTICES.md`.

The plugin glue and UI in this repository are MIT licensed. This independent project is not affiliated with or endorsed by UGREEN.

## Community Applications

Community Applications metadata lives in `ca_profile.xml` and `plugins/`. Stable releases publish both the `.plg` update descriptor and its checksum-verified payload; installation works independently of Community Applications review.

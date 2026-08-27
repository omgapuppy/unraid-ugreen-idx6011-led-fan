# Changelog

## 2026.08.27.1

- Correct the verified non-Pro physical SATA layout: ASMedia ports map to bays 1-4 and Intel PCH ports map to bays 5-6.

## 2026.08.26.6

- Do not apply rotational standby detection or its white LED state to SATA SSDs.

## 2026.08.26.5

- Clear the MCU's retained blink mode before applying a solid idle or standby LED state.

## 2026.08.26.4

- Publish update metadata and its payload checksum together through the latest stable GitHub release.

## 2026.08.26.3

- Corrected the non-Pro MCU layout after live hardware probing.
- Replaced Pro-oriented CLI writes with direct SMBus frames to prevent power-light flicker.

## 2026.08.26.2

- Reordered the controller refresh so the physical parity bay is always the final LED write.

## 2026.08.26.1

- Added the native fan dashboard card and grouped settings page.
- Improved whole-disk discovery and parity LED handling.

## 2026.08.26

- Initial LED-only release for non-Pro iDX6011/iDX6012 systems.

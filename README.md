# Mach3 + Ethernet SmoothStepper on Linux (Bazzite / Fedora)

A script that installs Mach3 in Lutris (Wine), with the setup needed for Mach3 to draw
its screen correctly and talk to an Ethernet SmoothStepper (ESS).

> ⚠️ Unofficial and lightly tested. Mach3 under Wine is supported by neither ArtSoft nor Warp9.
> Test the machine thoroughly, with no tool in the spindle, before cutting for real.

## What you need
- Bazzite or Fedora with **Lutris** installed as a regular package (not Flatpak)
- The Mach3 installer (`Mach3Version*.exe`) from machsupport.com
- The ESS plugin for Mach3 (`ESS.dll`, or a `.zip`/`.m3p` containing it) from warp9td.com
- Optional: the license file `Mach1Lic.dat`

Put the files in `~/Downloads`. The script contains no software from ArtSoft or Warp9.

## Installation
1. Start Lutris once and close it again.
2. Run (as your normal user, **not** with `sudo`):
   ```bash
   chmod +x install-mach3.sh
   ./install-mach3.sh
   ```
3. Click through the Mach3 installer when it appears, and install to `C:\Mach3`.
4. Pick the network card the ESS is connected to from the list (press Enter for the
   suggested one, 0 to skip). You can redo the network setup later with
   `./install-mach3.sh --network`.

## What the script does
- Uses or downloads **wine-staging** for Lutris. Older Wine (e.g. wine-ge-8) gives the error
  "context call failed in OpenGL" on systems with recent Mesa, because OSMesa has been removed.
- Creates a dedicated prefix in `~/Games/mach3`.
- Installs `gdiplus`, `mfc42` and `vcrun6` with Winetricks. Without them the Mach3 window stays blank.
- Adds Mach3 to Lutris with **X11** and **DXVK/VKD3D/D3D Extras/FSR off**.
- Copies `ESS.dll` to `PlugIns` and the license file to `C:\Mach3`.
- Allows port 69 (TFTP) for regular users (`/etc/sysctl.d/99-mach3-ess.conf`).
  The ESS plugin uses it to load firmware into the board. This step asks for your sudo password.
- Installs `forcebcast.so` (via `LD_PRELOAD` in Lutris). It enables `SO_BROADCAST` on UDP
  sockets, because the ESS plugin sends broadcast packets without requesting it, which Linux refuses.
  The source is included in the script and in `forcebcast.c`. If `gcc` is available, it is built from source.
- Optionally sets the network card to `10.9.9.1/24` and its firewall zone to `trusted`,
  so the ESS (`10.9.9.9`) can get through.

## Options
| Variable | Meaning |
|---|---|
| `MACH3_INSTALLER` | Path to the Mach3 installer |
| `ESS_PLUGIN` | Path to ESS.dll / .zip / .m3p |
| `MACH3_LICENSE` | Path to Mach1Lic.dat |
| `PREFIX` | Wine prefix (default `~/Games/mach3`) |
| `ESS_IFACE` | Network card the ESS is connected to (skips the question) |

Example: `ESS_IFACE=enp3s0 ./install-mach3.sh`

## After installation
1. Start Mach3 from Lutris.
2. Choose **Ethernet SmoothStepper** when Mach3 asks for a motion device.
3. **Config → Config Plugins → ESS**: IP address `10.9.9.9`.

## Troubleshooting
- **Blank Mach3 window:** View → Load Screens → `C:\Mach3\1024.set`
- **"context call failed in OpenGL":** choose a newer Wine (wine-staging) in Lutris → Configure → Runner options.
- **Mach3 can't find the ESS:** check that `ping 10.9.9.9` works, that the PC has an address in 10.9.9.x,
  that `sysctl net.ipv4.ip_unprivileged_port_start` shows `69`, and that `LD_PRELOAD` is set
  under Lutris → Configure → System options → Environment variables.
- **Debugging the network:** start Mach3 with `WINEDEBUG=+winsock` and look for
  `bind failed` or `sendto status 0xc0000022` in the log.
- **Screen doesn't fit the monitor:** Config → General Config → tick "AutoScreen Enlarge",
  or raise the DPI under Lutris → Configure → Runner options → Enable DPI Scaling.

## License
[MIT](LICENSE) — free for anyone to use, modify and share, commercially or not.

Provided as is, without warranty. You are responsible for safe operation of your machine.

Mach3 is a product of Newfangled Solutions / ArtSoft, and the Ethernet SmoothStepper of
Warp9 Tech Design. This project is not affiliated with either.

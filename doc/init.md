Sample init scripts and service configuration for vsyncd
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/vsyncd.service:    systemd service unit configuration
    contrib/init/vsyncd.openrc:     OpenRC compatible SysV style init script
    contrib/init/vsyncd.openrcconf: OpenRC conf.d file
    contrib/init/vsyncd.conf:       Upstart service configuration file
    contrib/init/vsyncd.init:       CentOS compatible SysV style init script

Service User
---------------------------------

All three Linux startup configurations assume the existence of a "vsync" user
and group.  They must be created before attempting to use these scripts.
The macOS configuration assumes vsyncd will be set up for the current user.

Configuration
---------------------------------

At a bare minimum, vsyncd requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, vsyncd will shutdown promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that vsyncd and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If vsyncd is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running vsyncd without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see contrib/debian/examples/vsync.conf.

Paths
---------------------------------

### Linux

All three configurations assume several paths that might need to be adjusted.

Binary:              /usr/bin/vsyncd
Configuration file:  /etc/vsync/vsync.conf
Data directory:      /var/lib/vsyncd
PID file:            `/var/run/vsyncd/vsyncd.pid` (OpenRC and Upstart) or `/run/vsyncd/vsyncd.pid` (systemd)
Lock file:           `/var/lock/subsys/vsyncd` (CentOS)

The configuration file, PID directory (if applicable) and data directory
should all be owned by the vsync user and group.  It is advised for security
reasons to make the configuration file and data directory only readable by the
vsync user and group.  Access to vsync-cli and other vsyncd rpc clients
can then be controlled by group membership.

NOTE: When using the systemd .service file, the creation of the aforementioned
directories and the setting of their permissions is automatically handled by
systemd. Directories are given a permission of 710, giving the vsync group
access to files under it _if_ the files themselves give permission to the
vsync group to do so (e.g. when `-sysperms` is specified). This does not allow
for the listing of files under the directory.

NOTE: It is not currently possible to override `datadir` in
`/etc/vsync/vsync.conf` with the current systemd, OpenRC, and Upstart init
files out-of-the-box. This is because the command line options specified in the
init files take precedence over the configurations in
`/etc/vsync/vsync.conf`. However, some init systems have their own
configuration mechanisms that would allow for overriding the command line
options specified in the init files (e.g. setting `BITCOIND_DATADIR` for
OpenRC).

### macOS

Binary:              `/usr/local/bin/vsyncd`
Configuration file:  `~/Library/Application Support/VSYNC/vsync.conf`
Data directory:      `~/Library/Application Support/VSYNC`
Lock file:           `~/Library/Application Support/VSYNC/.lock`

Installing Service Configuration
-----------------------------------

### systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start vsyncd` and to enable for system startup run
`systemctl enable vsyncd`

NOTE: When installing for systemd in Debian/Ubuntu the .service file needs to be copied to the /lib/systemd/system directory instead.

### OpenRC

Rename vsyncd.openrc to vsyncd and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/vsyncd start` and configure it to run on startup with
`rc-update add vsyncd`

### Upstart (for Debian/Ubuntu based distributions)

Upstart is the default init system for Debian/Ubuntu versions older than 15.04. If you are using version 15.04 or newer and haven't manually configured upstart you should follow the systemd instructions instead.

Drop vsyncd.conf in /etc/init.  Test by running `service vsyncd start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

### CentOS

Copy vsyncd.init to /etc/init.d/vsyncd. Test by running `service vsyncd start`.

Using this script, you can adjust the path and flags to the vsyncd program by
setting the VSYNCD and FLAGS environment variables in the file
/etc/sysconfig/vsyncd. You can also use the DAEMONOPTS environment variable here.

### macOS

Copy org.vsync.vsyncd.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.vsync.vsyncd.plist`.

This Launch Agent will cause vsyncd to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run vsyncd as the current user.
You will need to modify org.vsync.vsyncd.plist if you intend to use it as a
Launch Daemon with a dedicated vsync user.

Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.

Sample init scripts and service configuration for reefd
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/reefd.service:    systemd service unit configuration
    contrib/init/reefd.openrc:     OpenRC compatible SysV style init script
    contrib/init/reefd.openrcconf: OpenRC conf.d file
    contrib/init/reefd.conf:       Upstart service configuration file
    contrib/init/reefd.init:       CentOS compatible SysV style init script

1. Service User
---------------------------------

All three Linux startup configurations assume the existence of a "reefcore" user
and group.  They must be created before attempting to use these scripts.
The OS X configuration assumes reefd will be set up for the current user.

2. Configuration
---------------------------------

At a bare minimum, reefd requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, reefd will shutdown promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that reefd and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If reefd is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running reefd without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see `contrib/debian/examples/reef.conf`.

3. Paths
---------------------------------

3a) Linux

All three configurations assume several paths that might need to be adjusted.

Binary:              `/usr/bin/reefd`  
Configuration file:  `/etc/reefcore/reef.conf`  
Data directory:      `/var/lib/reefd`  
PID file:            `/var/run/reefd/reefd.pid` (OpenRC and Upstart) or `/var/lib/reefd/reefd.pid` (systemd)  
Lock file:           `/var/lock/subsys/reefd` (CentOS)  

The configuration file, PID directory (if applicable) and data directory
should all be owned by the reefcore user and group.  It is advised for security
reasons to make the configuration file and data directory only readable by the
reefcore user and group.  Access to reef-cli and other reefd rpc clients
can then be controlled by group membership.

3b) Mac OS X

Binary:              `/usr/local/bin/reefd`  
Configuration file:  `~/Library/Application Support/ReefCore/reef.conf`  
Data directory:      `~/Library/Application Support/ReefCore`
Lock file:           `~/Library/Application Support/ReefCore/.lock`

4. Installing Service Configuration
-----------------------------------

4a) systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start reefd` and to enable for system startup run
`systemctl enable reefd`

4b) OpenRC

Rename reefd.openrc to reefd and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/reefd start` and configure it to run on startup with
`rc-update add reefd`

4c) Upstart (for Debian/Ubuntu based distributions)

Drop reefd.conf in /etc/init.  Test by running `service reefd start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

4d) CentOS

Copy reefd.init to /etc/init.d/reefd. Test by running `service reefd start`.

Using this script, you can adjust the path and flags to the reefd program by
setting the REEFD and FLAGS environment variables in the file
/etc/sysconfig/reefd. You can also use the DAEMONOPTS environment variable here.

4e) Mac OS X

Copy org.reef.reefd.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.reef.reefd.plist`.

This Launch Agent will cause reefd to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run reefd as the current user.
You will need to modify org.reef.reefd.plist if you intend to use it as a
Launch Daemon with a dedicated reefcore user.

5. Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.

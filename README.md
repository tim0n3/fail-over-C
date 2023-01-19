# fail-over-C
basic and not at all proper way to port a bash script that functions as a fail-over script in C and run the binary as a systemD service

## Overview

This C program is designed to monitor the state of two network interfaces (primary and secondary) and ensure that internet connectivity is maintained. The primary interface is the preferred interface, and the secondary interface is used as a failover in case the primary interface goes down or loses internet connectivity.

The program uses the `system()` and `popen()` functions to run commands and parse their output, and `printf()` to print messages to the console.

The program is designed to work as a daemon and should be run as root.

## Configuration

The following constants are used to configure the program:

- `PRIMARY_IFACE`: The name of the primary interface (default: "eth0")
- `SECONDARY_IFACE`: The name of the secondary interface (default: "wlan0")
- `POLL_RATE`: The rate at which the program checks the interfaces (default: 1 second)
- `PING_TIMEOUT`: The number of consecutive ping failures required to trigger a failover (default: 4)
- `PRIMARY_IFACE_RECOVERY_TIME`: The time required for the primary interface to recover and trigger a failback (default: 600 seconds)

## Functionality

The program continuously performs the following actions:

1. Check the state of the primary interface:
   - If the primary interface is up, check for internet connectivity by pinging google.com:
     - If the ping is successful, reset the consecutive_failures counter and clear the primary_iface_failed flag
     - If the ping is not successful, increment the consecutive_failures counter.
   - If the consecutive_failures counter reaches the PING_TIMEOUT threshold, the primary interface is considered down, the primary_iface_fail_time is set to the current time, the primary_iface_failed flag is set to true, the primary interface is brought down and the secondary interface is brought up.
   
2. Check the state of the primary interface:
   - If the primary interface is down:
     - If the primary_iface_failed flag is set, check the time elapsed since the primary interface failed, if the time elapsed is greater than or equal to the PRIMARY_IFACE_RECOVERY_TIME, the primary interface is considered recovered and the secondary interface is brought down and the primary interface is brought up.
     - If the primary_iface_failed flag is not set, nothing happens.

3. Sleep for POLL_RATE seconds and repeat the process.

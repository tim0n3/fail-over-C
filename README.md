# fail-over-C
basic and not at all proper way to port a bash script that functions as a fail-over script in C and run the binary as a systemD service

As you can see, the C program uses C standard library functions such as system() and popen() to run commands and parse their output, and printf() to print messages to the console.
It's also important to note that this bin should run as a daemon and should be run as root, otherwise it won't be able to change the interfaces state.

This C program will continuously check the state of the primary and secondary interfaces and their connectivity to the internet. If the primary interface goes down or loses internet connectivity, the program will bring down the primary interface and bring up the secondary interface. And if the secondary interface goes down or loses internet connectivity, the program will bring down the secondary interface and bring up the primary interface.

This is just a simple example and it doesn't handle some of the edge cases, such as when both interfaces are down, it is recommended to test it in a safe environment and make any necessary adjustments.
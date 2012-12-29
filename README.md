dcn_udppinger
=============

In this lab you will implement a simple UDP-based ping client and server. The functionality provided by these programs is similar to the standard ping-program available in modern operating system. Ping works by sending Internet Control Message Protocol (ICMP) ECHO-messages which the remote machines echo back to the sender, who can then determine the round-trip times between itself and the computer it pinged. In this lab you will implement use standard UDP sockets ad messages to measure the round trip time (RTT) between a client and server.
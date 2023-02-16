# NTP

This code is a C++ program that connects to a Network Time Protocol (NTP) server to synchronize the local time with the accurate time provided by the NTP server. The program takes a single argument, which is the IP address or URL of the NTP server to be used for time synchronization.

The program first creates a socket and then uses the gethostbyname function to resolve the hostname of the NTP server into an IP address. It then sets up the sockaddr_in structure with the NTP server's IP address and port number.

The program then creates an NTP packet and sends it to the NTP server using the sendto function. The NTP packet contains a request for the current time from the NTP server.

After sending the NTP packet, the program waits for a response from the NTP server using the recvfrom function. Once the response is received, the program processes it and extracts the timestamp from the response.

The timestamp returned by the NTP server is in the format of the number of seconds since January 1, 1900. The program converts this timestamp to a time_t variable, which represents the number of seconds since January 1, 1970. The program then compares this timestamp with the local time to calculate the time difference.

If the time difference is positive, it means that the local time is behind the NTP server's time, and the program updates the local time by setting the system clock using the settimeofday function. If the time difference is negative, it means that the local time is ahead of the NTP server's time, and the program does not update the local time.

The program then outputs the NTP server's time, the local time, and the time difference between them to the console. If the local time was updated, the program outputs the new local time as well.

This project was developed in Ubuntu 20.04 operating system. It may not work on another operating system. To run the application, compile the NTP.cpp file and after the compilation is finished, you can run it by typing "./NTP <NTP server IP address or domain name>" on the terminal screen.

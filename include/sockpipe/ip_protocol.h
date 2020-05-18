#ifndef SP_IP_PROTOCOL
#define SP_IP_PROTOCOL

#define SP_ICMP    1	
#define SP_IGMP    2
#define SP_TCP     6
#define SP_UDP    17

#ifdef __MINGW32__
#   include <winsock2.h>
#else
#   include <arpa/inet.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <netinet/ip.h>
#endif

#endif

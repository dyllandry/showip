#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  // Validate usage from cli.
  if (argc != 2) {
    fprintf(stderr, "usage: showip hostname\n");
    return 1;
  }

  // Setup struct for getaddrinfo call.
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // Attempt getaddrinfo.
  if (status = getaddrinfo(argv[1], NULL, &hints, &res) != 0) {
    printf("status: %d", status);
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  printf("IP addresses for %s: \n\n", argv[1]);

  // Loop through linked list of addrinfo structs to print ips
  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;
    char *ipver;

    // Get pointer to addr's internet address via IPv6/IPv4 socket structure
    if (p->ai_family == AF_INET) { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else { // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    // Print presentation form of network ip address.
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
  }
  
  // Free memory allocated to the addrinfo linked list.
  freeaddrinfo(res);
  return 0;
}

#include <queue.h>
#include "skel.h"

// The routing table and its size.
struct route_table_entry *rtable;
int rtable_size;

// The ARP table and its number of entries and size.
struct arp_entry *arp_table;
int arp_table_len, arp_table_size;

// Function for reading the router's routing table.
int read_rtable(char *path) {
  // Open the file that contains the routing table's info.
  FILE *f = fopen(path, "rt");
  DIE(f == NULL, "failed to open rtable file");

  // Alloc memory for the routing table.
  rtable = calloc(10, sizeof(struct route_table_entry));
  DIE(rtable == NULL, "no memory for rtable");

  rtable_size = 0;

  char line[200];
  char prefix_str[50], next_hop_str[50], mask_str[50];
  int interface;
  int i = 0, rtable_new_size = 10;

  // Read each line from the file and create an entry in the routing table from it.
  for (i = 0; fgets(line, sizeof(line), f); i++) {
    // If necessary alloc more memory for the routing table.
    if (i == rtable_new_size) {
      rtable_new_size += 10;

      rtable = (struct route_table_entry *)realloc(
          rtable, sizeof(struct route_table_entry) * rtable_new_size);
      DIE(rtable == NULL, "no memory to realloc rtable");
    }

    /* 
       Split the line's info in prefix, next hop, mask and interface and create
       an entry in the routing table using them.
    */

    sscanf(line, "%s %s %s %u", prefix_str, next_hop_str, mask_str, &interface);

    rtable[i].prefix = inet_addr(prefix_str);
    rtable[i].next_hop = inet_addr(next_hop_str);
    rtable[i].mask = inet_addr(mask_str);
    rtable[i].interface = interface;
  }

  fclose(f);

  // Return the number of entries in the routing table.
  return i;
}

/* 
   Compare function used for sorting the routing table in ascending order by the
   entries' prefixes and descending by their masks if their prefixes match.
*/
int cmp_rtable_entries(const void *p, const void *q) {
  struct route_table_entry *p_entry = (struct route_table_entry *)p;
  struct route_table_entry *q_entry = (struct route_table_entry *)q;

  if (p_entry->prefix != q_entry->prefix) {
    return (ntohl(p_entry->prefix) - ntohl(q_entry->prefix));
  } else {
    return (ntohl(q_entry->mask) - ntohl(p_entry->mask));
  }
}

/*
   Function used for finding the best matching route for an ip address.
   
   It returns a pointer (&rtable[index]) to the best matching route for the
   given dest_ip or NULL if there is no matching route. 

   Basically the iterative binary search algorithm is applied on the sorted 
   routing table in order to find the best matching route for the destination ip. 
   
   Time complexity: T(n) = T(n/2) + c => Master Method Case 2 => Theta(log(n)).
*/
struct route_table_entry *get_best_route(uint32_t dest_ip) {
  int l = 0, r = rtable_size - 1, m;

  while (l <= r) {
    m = l + (r - l) / 2;

    // If a matching route is found choose the one with the longest mask.
    if ((rtable[m].mask & dest_ip) == rtable[m].prefix) {
      uint32_t prefix = rtable[m].prefix;
      do {
        m--;
      } while (m >= 0 && (rtable[m].mask & dest_ip) == rtable[m].prefix && 
      	       prefix == rtable[m].prefix);

      return &rtable[m + 1];
    }

    // Ignore the left half in order to find a matching route.
    if (ntohl(rtable[m].mask & dest_ip) > ntohl(rtable[m].prefix)) {
      l = m + 1;
    // Ignore the right half in order to find a matching route.
    } else {
      r = m - 1;
    }
  }

  // No matching route was found.
  return NULL;
}

// Function used for initialising the ARP table.
struct arp_entry *init_arp_table(int arp_table_initial_size) {
  // Alloc memory for the ARP table.
  struct arp_entry *arp_table =
      calloc(arp_table_initial_size, sizeof(struct arp_entry));
  DIE(arp_table == NULL, "no memory for arp table");

  arp_table_len = 0;

  return arp_table;
}

/*
   Function used for finding the best matching ARP entry.
   
   Returns a pointer (&arp_table[index]) to the best matching ARP entry for the
   given ip or NULL if there is no matching entry.
*/
struct arp_entry *get_arp_entry(uint32_t ip) {
  int i;

  for (i = 0; i < arp_table_len; i++) {
    // Check if the IPs match.
    if (ip == arp_table[i].ip) {
      return &arp_table[i];
    }
  }

  // No matching entry was found.
  return NULL;
}

// Function used for adding a new entry in the ARP table.
void add_arp_entry(struct arp_header *arp_hdr) {
  // If necessary alloc more memory for the ARP table.
  if (arp_table_size == arp_table_len) {
    arp_table_size += 10;

    arp_table = (struct arp_entry *)realloc(
        arp_table, sizeof(struct arp_entry) * arp_table_size);
    DIE(arp_table == NULL, "no memory to realloc arp_table");
  }

  // Set the new entry's IP.
  arp_table[arp_table_len].ip = arp_hdr->spa;

  // Set the new entry's MAC.
  memcpy(arp_table[arp_table_len].mac, arp_hdr->sha, ETH_ALEN);

  // Increment the number of entries in the ARP table.
  arp_table_len += 1;
}

// Function used for sending an ARP Reply.
void send_arp_reply(struct ether_header *eth_hdr, struct arp_header *arp_hdr,
                    int interface) {
  // Set the destination MAC to the old source MAC.
  memcpy(eth_hdr->ether_dhost, eth_hdr->ether_shost, ETH_ALEN);

  // Set the new source MAC to the router's interface MAC.
  get_interface_mac(interface, eth_hdr->ether_shost);

  /* 
     Interchange the destination IP with the source IP.
     Send the Arp Reply packet.
  */
  send_arp(arp_hdr->spa, arp_hdr->tpa, eth_hdr, interface, htons(ARPOP_REPLY));
}

// Function used for sending an ARP Request.
void send_arp_request(struct ether_header *eth_hdr,
                      struct route_table_entry *best_route) {
  // Set the ether type to ETHERTYPE_ARP.
  eth_hdr->ether_type = htons(ETHERTYPE_ARP);

  // Set the destination MAC to the broadcast MAC address: ff:ff:ff:ff:ff:ff.
  memset(eth_hdr->ether_dhost, 0xff, ETH_ALEN);

  // Set the new source MAC to the router's interface MAC.
  get_interface_mac(best_route->interface, eth_hdr->ether_shost);

  // Get the router's interface IP.
  uint32_t interface_ip = inet_addr(get_interface_ip(best_route->interface));

  // Send the ARP request package.
  send_arp(best_route->next_hop, interface_ip, eth_hdr, best_route->interface,
           htons(ARPOP_REQUEST));
}

/* 
   Function used for forwarding packets when a new entry is added in the ARP
   table (when the router receives an ARP Reply).
*/
void forward_packets(struct queue **packets, struct arp_entry *new_arp_entry) {
  // Queue for storing the packets that won't be forwarded.
  struct queue *packets_left = queue_create();

  struct ether_header *eth_hdr;
  struct iphdr *ip_hdr;

  // Check for each packet in the queue if it should be forwarded to that router.
  while (!queue_empty(*packets)) {
    packet *m = queue_deq(*packets);

    // Extract the IP header.
    ip_hdr = (struct iphdr *)(m->payload + sizeof(struct ether_header));

    // Get the best matching route.
    struct route_table_entry *best_route = get_best_route(ip_hdr->daddr);

    /*
       If necessary foward the packet, else add it to the queue that stores the
       packets that can't be forwarded at the moment.
    */
    if (best_route->next_hop == new_arp_entry->ip) {
      // Extract the Ethernet header.
      eth_hdr = (struct ether_header *)m->payload;

      // Set the new destination mac to the arp entry's mac.
      memcpy(eth_hdr->ether_dhost, new_arp_entry->mac, ETH_ALEN);

      // Set the new source MAC to the router's interface MAC.
      get_interface_mac(best_route->interface, eth_hdr->ether_shost);

      // Send the packet.
      send_packet(best_route->interface, m);
    } else {
      queue_enq(packets_left, m);
    }
  }

  // Update the packets' to be forwarded queue.
  free(*packets);
  *packets = packets_left;
}

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0);

  packet m;
  int rc;

  init(argc - 2, argv + 2);

  // Read the routing table and sort it.
  rtable_size = read_rtable(argv[1]);
  qsort(rtable, rtable_size, sizeof(struct route_table_entry),
        cmp_rtable_entries);

  // Initialise the ARP table.
  arp_table_size = 10;
  arp_table = init_arp_table(arp_table_size);

  // Queue for storing the packets to be forwarded.
  struct queue *packets = queue_create();

  struct ether_header *eth_hdr;
  struct iphdr *ip_hdr;

  while (1) {
    // Receive packet.
    rc = get_packet(&m);
    DIE(rc < 0, "get_message");

    // Extract the Ethernet and IP headers.
    eth_hdr = (struct ether_header *)m.payload;
    ip_hdr = (struct iphdr *)(m.payload + sizeof(struct ether_header));

    /*
       Try to extract the packet's ICMP header.
       If it isn't an ICMP packet icmp_hdr will be set to NULL.
    */
    struct icmphdr *icmp_hdr = parse_icmp(m.payload);

    /* 
       If it is an IP packet (if it's an ICMP one then it's also an IP one) and
       it is for the router:
    */
    if (icmp_hdr && inet_addr(get_interface_ip(m.interface)) == ip_hdr->daddr) {

      // If it's an ICMP ECHO request packet send an ICMP ECHO reply packet.
      if (icmp_hdr->type == ICMP_ECHO) {
      	// Interchange the destination IP(MAC) with the source one.
        send_icmp(ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost,
                  eth_hdr->ether_shost, ICMP_ECHOREPLY, icmp_hdr->code,
                  m.interface, icmp_hdr->un.echo.id,
                  icmp_hdr->un.echo.sequence);
        continue;
      }
    }

    /*
       Try to extract the packet's ARP header.

       If it isn't an ARP packet arp_hdr will be set to NULL.
    */
    struct arp_header *arp_hdr = parse_arp(m.payload);

    // If it is an ARP packet:
    if (arp_hdr != NULL) {

      // Check if it is an ARP request for one of the router's IPs.
      if (ntohs(arp_hdr->op) == ARPOP_REQUEST &&
          inet_addr(get_interface_ip(m.interface)) == arp_hdr->tpa) {

        // Send an ARP reply with the desired MAC address.
        send_arp_reply(eth_hdr, arp_hdr, m.interface);
      }
      // Check if it is an ARP reply.
      else if (ntohs(arp_hdr->op) == ARPOP_REPLY) {
        // Update the arp table with a new entry.
        add_arp_entry(arp_hdr);

        /* Forward the packets to that router (if there are any that should be
           forwarded there).
        */
        forward_packets(&packets, &arp_table[arp_table_len - 1]);
      }

      continue;
    }

    /* 
       Check if the packet's TTL <= 1 and send an ICMP_TIME_EXCEEDED packet if
       necessary.
    */
    if (ip_hdr->ttl <= 1) {
      // Interchange the destination IP(MAC) with the source one.
      send_icmp_error(ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost,
                      eth_hdr->ether_shost, ICMP_TIME_EXCEEDED, 0, m.interface);
      continue;
    }

    // Check the packet's checksum and drop the packet if necessary.
    if (ip_checksum(ip_hdr, sizeof(struct iphdr)) != 0) {
      continue;
    }

    // Decrement TTL and update checksum.
    ip_hdr->ttl--;
    ip_hdr->check = 0;
    ip_hdr->check = ip_checksum(ip_hdr, sizeof(struct iphdr));

    // Find the most specific entry in the routing table.
    struct route_table_entry *best_route = get_best_route(ip_hdr->daddr);

    // If no route is found send an ICMP_DEST_UNREACH packet if necessary.
    if (best_route == NULL) {
      // Interchange the destination IP(MAC) with the source one.
      send_icmp_error(ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost,
                      eth_hdr->ether_shost, ICMP_DEST_UNREACH, 0, m.interface);
    } else {
      // Find the best matching ARP entry in order to get the destination MAC.
      struct arp_entry *arp_table_entry = get_arp_entry(best_route->next_hop);

      /* 
         If there is no matching entry then the destination MAC isn't known by 
         the router:
      */
      if (arp_table_entry == NULL) {
        /* 
           Make a copy of the packet and add it to the packets to be forwarded
           queue.
        */
        packet *packet_copy = (packet *)calloc(1, sizeof(packet));
        DIE(packet_copy == NULL, "no memory for duplicating packet");

        memcpy(packet_copy, &m, sizeof(packet));

        queue_enq(packets, packet_copy);

        // Send an ARP request for the destination MAC.
        send_arp_request(eth_hdr, best_route);
      } else {
        // Set the destination MAC to the one from the ARP table entry.
        memcpy(eth_hdr->ether_dhost, arp_table_entry->mac, ETH_ALEN);

        // Set the new source MAC to the router's interface MAC.
        get_interface_mac(best_route->interface, eth_hdr->ether_shost);

        // Send the packet.
        send_packet(best_route->interface, &m);
      }
    }
  }
}
#ifndef _ICMP_PING_H_
#define _ICMP_PING_H_

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0

/* The IP header */
typedef struct _iphdr {
	unsigned int h_len:4;          // length of the header
	unsigned int version:4;        // Version of IP
	unsigned char tos;             // Type of service
	unsigned short total_len;      // total length of the packet
	unsigned short ident;          // unique identifier
	unsigned short frag_and_flags; // flags
	unsigned char  ttl; 
	unsigned char proto;           // protocol (TCP, UDP etc)
	unsigned short checksum;       // IP checksum

	unsigned int sourceIP;
	unsigned int destIP;
} IPHDR, *LPIPHDR;

// ICMP header
typedef struct _ihdr {
  BYTE i_type;
  BYTE i_code; /* type sub code */
  USHORT i_cksum;
  USHORT i_id;
  USHORT i_seq;
  /* This is not the std header, but we reserve space for time */
  ULONG timestamp;
} ICMPHDR, *LPICMPHDR;

#endif //_ICMP_PING_H_

#ifndef KADEMLIA_ROUTER_H
#define KADEMLIA_ROUTER_H

typedef struct NodeRef NodeRef;
typedef struct KBucket KBucket;
typedef struct KademliaMap KademliaMap;
KademliaMap* init_kademlia_node(int local_id);
void register_remote_peer(KademliaMap* km, int remote_id, const char* addr);
int locate_closest_peer_bucket(KademliaMap* km, int target_key);

#endif

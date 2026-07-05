#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_LEN_BITS 8
#define BUCKET_K 4

typedef struct NodeRef {
    int node_id;
    char network_address[32];
} NodeRef;

typedef struct KBucket {
    NodeRef peers[BUCKET_K];
    int peer_count;
} KBucket;

typedef struct {
    int local_node_id;
    KBucket routing_table[ID_LEN_BITS];
} KademliaMap;

#ifdef _WIN32
    __declspec(dllexport) KademliaMap* init_kademlia_node(int local_id);
    __declspec(dllexport) void register_remote_peer(KademliaMap* km, int remote_id, const char* addr);
    __declspec(dllexport) int locate_closest_peer_bucket(KademliaMap* km, int target_key);
#endif

KademliaMap* init_kademlia_node(int local_id) {
    KademliaMap* km = (KademliaMap*)malloc(sizeof(KademliaMap));
    km->local_node_id = local_id;
    for (int i = 0; i < ID_LEN_BITS; i++) km->routing_table[i].peer_count = 0;
    return km;
}

void register_remote_peer(KademliaMap* km, int remote_id, const char* addr) {
    // Determine target bucket index using bitwise XOR distance metrics
    int xor_distance = km->local_node_id ^ remote_id;
    if (xor_distance == 0) return;

    int bucket_index = 0;
    while ((xor_distance >> (bucket_index + 1)) > 0 && bucket_index < ID_LEN_BITS - 1) {
        bucket_index++;
    }

    KBucket* bucket = &km->routing_table[bucket_index];
    if (bucket->peer_count < BUCKET_K) {
        bucket->peers[bucket->peer_count].node_id = remote_id;
        strncpy(bucket->peers[bucket->peer_count].network_address, addr, sizeof(bucket->peers[bucket->peer_count].network_address) - 1);
        bucket->peer_count++;
    }
}

int locate_closest_peer_bucket(KademliaMap* km, int target_key) {
    int closest_bucket = 0;
    int min_distance = km->local_node_id ^ target_key;

    for (int i = 0; i < ID_LEN_BITS; i++) {
        if (km->routing_table[i].peer_count > 0) {
            int current_distance = km->routing_table[i].peers[0].node_id ^ target_key;
            if (current_distance < min_distance) {
                min_distance = current_distance;
                closest_bucket = i;
            }
        }
    }
    return closest_bucket;
}

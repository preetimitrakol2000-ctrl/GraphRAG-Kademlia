from kad_bridge import KademliaBridge

if __name__ == "__main__":
    # Local Node defined with static ID hash 12
    p2p_router = KademliaBridge(local_node_id=12)

    # Register remote infrastructure clusters holding knowledge shards
    p2p_router.join_network_peer(node_id=45, network_ip="192.168.10.45")
    p2p_router.join_network_peer(node_id=210, network_ip="192.168.50.210")

    # Locate closest vector storage bucket containing matching data hash ranges
    document_target_key = 42
    assigned_bucket = p2p_router.find_nearest_routing_bucket(document_target_key)

    print("=== GRAPHRAG-KADEMLIA DISTRIBUTED INTEL ROUTER ===")
    print(f"[*] Search Target Document Key: {document_target_key}")
    print(f"[*] Routed via Bitwise XOR Distance to Local Bucket Matrix Entry: [{assigned_bucket}]")

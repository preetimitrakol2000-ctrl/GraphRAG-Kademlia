import ctypes
import os
import sys

class KademliaBridge:
    def __init__(self, local_node_id: int):
        if not os.path.exists("./libkad.so") and not os.path.exists("./libkad.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libkad.dll kademlia_router.c")
                lib_path = "./libkad.dll"
            else:
                os.system("gcc -shared -fPIC -o libkad.so kademlia_router.c")
                lib_path = "./libkad.so"
        else:
            lib_path = "./libkad.dll" if sys.platform.startswith("win") else "./libkad.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_kademlia_node.restype = ctypes.c_void_p
        self.lib.register_remote_peer.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]
        self.lib.locate_closest_peer_bucket.argtypes = [ctypes.c_void_p, ctypes.c_int]
        self.lib.locate_closest_peer_bucket.restype = ctypes.c_int
        
        self.node_ptr = self.lib.init_kademlia_node(local_node_id)

    def join_network_peer(self, node_id: int, network_ip: str):
        self.lib.register_remote_peer(self.node_ptr, node_id, network_ip.encode('utf-8'))

    def find_nearest_routing_bucket(self, search_key: int) -> int:
        return self.lib.locate_closest_peer_bucket(self.node_ptr, search_key)

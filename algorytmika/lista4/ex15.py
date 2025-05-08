import networkx as nx
import random
import copy

def karger_min_cut_nx(graph: nx.Graph) -> int:
    g = copy.deepcopy(nx.MultiGraph(graph))

    while g.number_of_nodes() > 2:
        u, v = random.choice(list(g.edges()))
        g = nx.contracted_nodes(g, u, v, self_loops=False)

    return g.number_of_edges()


G = nx.MultiGraph(nx.star_graph(10))
print(karger_min_cut_nx(G))

G = nx.MultiGraph(nx.complete_graph(10))
print(karger_min_cut_nx(G))

G = nx.MultiGraph(nx.complete_bipartite_graph(5, 5))
print(karger_min_cut_nx(G))

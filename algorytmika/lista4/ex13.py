import networkx as nx

def expected_cut_size(g: nx.Graph, A: list[int], v:int) -> int:
    res = 0
    for e in g.edges():
        if v in e and (e[0] in A or e[1] in A):
            res += 1
    return res

def max_cut_size(g: nx.Graph) -> tuple[list[int], list[int], int]:
    A = []
    B = []
    for v in g.nodes():
        add_to_A = expected_cut_size(g, A, v)
        add_to_B = expected_cut_size(g, B, v)
        if add_to_A < add_to_B:
            A.append(v)
        else:
            B.append(v)
    
    res = 0
    for e in g.edges():
        if (e[0] in A and e[1] in B) or (e[0] in B and e[1] in A):
            res += 1
    return A, B, res
            

g= nx.star_graph(10)
print(max_cut_size(g))

g= nx.complete_graph(5)
print(max_cut_size(g))

g= nx.path_graph(10)
print(max_cut_size(g))

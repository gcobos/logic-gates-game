
from enum import Enum
from itertools import combinations
from pprint import pprint


class Gate(Enum):
    NOT = 1
    AND = 2
    OR = 3
    XOR = 4
    
    def ins(self):
        return 1 if self is Gate.NOT else 2
    
    def outs(self):
        return 1
    
    def __str__(self):
        if self is Gate.NOT:
            return "NOT"
        elif self is Gate.AND:
            return "AND"
        elif self is Gate.OR:
            return "OR"
        elif self is Gate.XOR:
            return "XOR"


MAX_BITS = 5
SEP = "."
ALL_GATES = (Gate.NOT, Gate.AND, Gate.OR, Gate.XOR)
"""
REGLAS:
- El mínimo de puertas es 1 el máximo es 20
- El número mínimo de entradas es 1, el máximo es MAX_BITS
- El número mínimo de salidas es 1, el máximo es MAX_BITS
- las entradas al circuito se utilizan como salidas de puertas
- las salidas del circuito son igual que entradas de puertas

"""

# Constrains
REMOVE_DIRECT_CONNECTIONS = 1       # Don't allow wires connecting directly without a gate
REMOVE_SAME_GATE_CONNECTIONS = 2    # Don't allow connections withing the same gate
ENSURE_ALL_CONNECTED = 3            # Ensure all inputs and outputs are used for connections


def generate_connections(inputs, outputs, constrains = {
    REMOVE_DIRECT_CONNECTIONS,
    REMOVE_SAME_GATE_CONNECTIONS,
    ENSURE_ALL_CONNECTED
}):
    """
    Generate all possible connections where:
    - Each output can connect to one or more inputs.
    - Each input can only be connected to one output.
    - Connections must include all the inputs and outputs

    Parameters:
    inputs (list): List of input identifiers.
    outputs (list): List of output identifiers.

    Returns:
    list: A list of all possible valid connection configurations.
    """
    # Generate all possible mappings of inputs to outputs
    input_output_pairs = [(output, inp) for output in outputs for inp in inputs]

    # Generate all subsets of the pairs to cover all combinations
    all_combinations = []
    for r in range(1, len(input_output_pairs) + 1):
        all_combinations.extend(combinations(input_output_pairs, r))
        
    valid_connections = all_combinations
    #print("All combs: ", )
    #pprint(all_combinations)
    
    # Remove combinations where a gate connects to itself
    if REMOVE_SAME_GATE_CONNECTIONS in constrains:
        valid_connections = [
            list(pair for pair in combination \
            if pair[0].split(SEP)[0] != pair[1].split(SEP)[0]) \
            for combination in valid_connections
        ]

    # Remove direct connections from circuit's inputs to circuit's outputs
    if REMOVE_DIRECT_CONNECTIONS in constrains:
        valid_connections = [
#            pair for combination in valid_connections for pair in combination]
            list(pair for pair in combination \
                if "".join(pair).count(SEP) != 0) \
                for combination in valid_connections
        ]

    # Filter combinations where each output is mapped to at most one input
    valid_connections = [
        combination for combination in valid_connections
        if len(set(pair[1] for pair in combination)) == len(combination)
    ]
    
    # Remove combinations where not all ins and outs are used
    if ENSURE_ALL_CONNECTED in constrains:
        valid_connections = [
            combination for combination in valid_connections
            if len(set(item for pair in combination for item in pair)) == len(inputs) + len(outputs)
        ]
    
    # Remove empty combinations and duplications
    valid_connections = list({tuple(combination):None for combination in valid_connections if combination}.keys())
    
    return valid_connections


## Each input of the level can go to one or more inputs of gates in the same or higher level
def generate_all_levels(gates = [Gate.NOT], inputs = 1, outputs = 1):
    "Generates all possible circuits with the constrains defined"

    # Register all possible inputs
    in_nodes = list(f"OUT{i}" for i in range(outputs)) 
    occurrences = {g:0 for g in ALL_GATES}
    for i, g in enumerate(gates):
        for j in range(g.ins()):
            in_nodes.append(f"{g}{occurrences[g]}.in{j}")
        occurrences[g]+=1

    # Register all possible outputs
    out_nodes = list(f"IN{i}" for i in range(inputs))
    occurrences = {g:0 for g in ALL_GATES}
    for i, g in enumerate(gates):
        for j in range(g.outs()):
            out_nodes.append(f"{g}{occurrences[g]}.out{j}")
        occurrences[g]+=1
    #print("ins", in_nodes)
    #print("outs", out_nodes)

    connections = generate_connections(in_nodes, out_nodes)
    
    return connections

if __name__ == '__main__':

    #generate_all_levels([Gate.NOT], 1, 2)
    o1 = [] #generate_all_levels([Gate.AND, Gate.NOT], 2, 1)
    o2 = generate_all_levels([Gate.AND, Gate.AND], 2, 1)
    
    #print("Connections:", )
    pprint(sorted(o1), depth = 2); print("len", len(o1))
    pprint(sorted(o2)); print("len", len(o2))

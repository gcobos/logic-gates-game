
from enum import Enum
from itertools import combinations, product, permutations, combinations_with_replacement
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

def generate_connections(inputs, outputs, constrains = {
    REMOVE_DIRECT_CONNECTIONS,
    REMOVE_SAME_GATE_CONNECTIONS,
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
    input_output_pairs = product(outputs, inputs) #[(output, inp) for output in outputs for inp in inputs]

    # Remove direct connections from circuit's inputs to circuit's outputs
    if REMOVE_DIRECT_CONNECTIONS in constrains:
        input_output_pairs = [pair for pair in input_output_pairs if "".join(pair).count(SEP) != 0]

    if REMOVE_SAME_GATE_CONNECTIONS in constrains:
        input_output_pairs = [pair for pair in input_output_pairs if pair[0].split(SEP)[0] != pair[1].split(SEP)[0]]

    #print(input_output_pairs)
    # Generate all subsets of the pairs to cover all combinations
    #all_combinations = combinations(input_output_pairs, len(input_output_pairs))
    #all_combinations = permutations(input_output_pairs, len(input_output_pairs))
    all_combinations = []
    for r in range(1, len(input_output_pairs) + 1):
        all_combinations.extend(combinations(input_output_pairs, r))

    #print("All combs")
    #for i in all_combinations:
    #    print(str(i).replace(' ', ''))
    #print("Bien1", len(list(all_combinations)))
    
    #print("Bien2", len(list(valid_combinations)))
    #for i in list(valid_combinations):
    #    print(len(set(item for pair in i for item in pair)))
    #    #print(str(i).replace(' ', ''))
    
    # Remove connections where the same output (or circuit's input) connects to a same gate
    combinations_without_overlapping = (
        {(pair[0], pair[1].split(SEP)[0]):pair for pair in combination}.values() \
        for combination in all_combinations
    )
    #for i in combinations_without_overlapping:
    #    print(str(i).replace(' ', ''))
    #print("Bien3", len(list(combinations_without_overlapping)))
    # Remove combinations where not all ins and outs are used
    valid_combinations = (
        combination for combination in combinations_without_overlapping
        if len(set(item for pair in combination for item in pair)) == len(inputs) + len(outputs)
    )
    # Remove empty combinations and duplications
    unique_combinations = {tuple(sorted(combination)):None for combination in valid_combinations if combination}.keys()
    # print("To4", len(list(unique_combinations)))
    # for i in sorted(unique_combinations):
    #     print(str(i).replace(' ', ''))
    #print("Bien4", len(list(valid_combinations)))
    return unique_combinations

def generate_formula(connections):
    """
    Generate the logical formula for the given circuit connections.
    
    Parameters:
    connections (list): List of tuples representing the connections in the circuit.
    
    Returns:
    dict: A dictionary where keys are output identifiers and values are the logical formulas.
    """
    from collections import defaultdict

    # Initialize a dictionary to store the formulas for each output
    formulas = defaultdict(str)

    # Process each connection
    for output, input_ in connections:
        if '.' in input_:            
            # Determine the gate and its inputs
            gate, gate_input = input_.split('.', 2)
            gate_name, _ = gate[:-1], gate[-1]
            
            # Replace the gate input with its formula if it already exists
            if gate_input in formulas:
                gate_input = formulas[gate_input]
            else:
                gate_input = gate_input.replace("in", "IN")

            # Create the formula for the gate output
            if gate_name == "NOT":
                if formulas[output]:
                    formulas[output] = f"~{gate_input}"
                else:
                    formulas[output] = gate_input
            elif gate_name == "AND":
                if formulas[output]:
                    formulas[output] = f"({formulas[output]}&{gate_input})"
                else:
                    formulas[output] = gate_input
            elif gate_name == "OR":
                if formulas[output]:
                    formulas[output] = f"({formulas[output]}|{gate_input})"
                else:
                    formulas[output] = gate_input
            elif gate_name == "XOR":
                if formulas[output]:
                    formulas[output] = f"({formulas[output]}^{gate_input})"
                else:
                    formulas[output] = gate_input
        else:
            # input_ is a circuit's output
            formulas[output] = input_

    # Construct the final formula for each output
    final_formulas = formulas
    """for output in formulas:
        if output.startswith("OUT"):
            final_formulas[output] = formulas[output]
    """
    return final_formulas

## Each input of the level can go to one or more inputs of gates in the same or higher level
def generate_all_circuits(gates = [Gate.NOT], inputs = 1, outputs = 1):
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

    #o1 = generate_all_circuits([Gate.NOT], 1, 1)
    #o1 = generate_all_circuits([Gate.AND], 2, 1)
    o1 = generate_all_circuits([Gate.NOT, Gate.NOT], 1, 1)
    #o1 = generate_all_circuits([Gate.AND, Gate.NOT], 2, 1)
    #o1 = generate_all_circuits([Gate.AND, Gate.AND], 2, 1)
    #o1 = generate_all_circuits([Gate.AND, Gate.OR, Gate.NOT, Gate.XOR, Gate.AND], 3, 1)
    
    print("Circuits:", )
    for i in sorted(o1):
        print(str(i).replace(' ', ''))   
    print("len", len(list(o1)))
    #pprint(sorted(o2), depth = 2); print("len", len(o2))
    #for i in o2[:1]:
    #    print("For:", list(i))
    #    print("Formula:", generate_formula(i))

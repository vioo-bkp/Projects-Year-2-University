# AA HW4 - KVC to SAT
# Author: Mateescu Viorel-Cristian
# Grupa: 322CB

# Function for reading a value from a file and converting it to an int.
def read_variable():
    temp = input()
    temp = int(temp)

    return temp


# A k Vertex Cover of a graph has k slots: S(1) S(2) ... S(k)
# Using this function k variables are created for each vertex in the graph:
# X(i, j) where node i occupies slot S(j) in the Vertex Cover.
# Conditions:
#   - i E [1, n];
#   - j E [1, k];
#   - k is the number of vertexes wanted for the KVC.
# So we'll have n * k variables for the SAT encoding of the KVC problem.
def create_variable(i, j, k):
    return (i - 1) * k + j


# Step 1 : Every edge is incident to at least one vertex in the vertex cover.
# The clauses created at this step verify the existence of the k Vertex Cover.
# -> For an edge to be covered by the Vertex Cover one of its two vertices must be in the Cover.
# In the SAT encoding that we're using this condition can be translated as:
# -> At least one of the variables corresponding to one of the edge's vertices must be set as True for the edge to be
# covered by the Vertex Cover.
# In order to check this we create for each edge a clause containing all its vertices corresponding variables,
# interconnected by "V".
# Number of clauses: |E| = m < n * (n - 1) / 2 => O(n^2) clauses.
def incidence_of_edges_to_the_vertex_cover(k, m):
    # Variable used for storing the clauses.
    clauses = ""

    # Read all the edges in the graph and create a clause for each.
    for i in range(m):
        # Variable used for storing the current clause that we're creating.
        clause = "("

        # Read the defining vertices for each edge and convert them to int.
        start_vertex, end_vertex = input().split(" ")

        start_vertex = int(start_vertex)
        end_vertex = int(end_vertex)

        # Create all of the variables for the two vertices and add them to the clause.
        for j in range(1, k + 1):
            start_variable = create_variable(start_vertex, j, k)
            end_variable = create_variable(end_vertex, j, k)

            clause += str(start_variable) + "V" + str(end_variable) + "V"

        # Add the previously created clause to the rest of the clauses.
        clause = clause[:-1] + ")"
        clauses += clause + "^"

    # Return the clauses created.
    return clauses


# Step 2: Each slot S(j) must be occupied.
# The clauses created at this point verify that there is no empty slot left in the k Vertex Cover.
# -> Each slot is occupied by a vertex.
# In the SAT encoding that we're using this condition can be translated as:
# -> At least one of the variables for each slot must be set to True.
# In order to check this we create for each existing slot a clause containing all the vertices' variables for the
# current slot, interconnected by "V".
# Number of clauses: k <= n => O(n) clauses.
def occupy_all_slots(k, n):
    # Variable used for storing the clauses.
    clauses = ""

    # Create a clause for each existing slot.
    for i in range(1, k + 1):
        # Variable used for storing the current clause that we're creating.
        clause = "("

        # Create all the vertices' variables for the current slot and add them to its clause.
        for j in range(1, n + 1):
            variable = create_variable(j, i, k)
            clause += str(variable) + "V"

        # Add the previously created clause to the rest of the clauses.
        clause = clause[:-1] + ")"
        clauses += clause + "^"

    # Return the clauses created.
    return clauses


# Step 3: No vertex can appear twice in the k Vertex Cover.
# The clauses created at this point verify that a vertex does not appear both in positions a and b of the Vertex Cover.
# -> A vertex can occupy only one slot in the Vertex Cover.
# In the SAT encoding that we're using this condition can be translated as:
# -> Only one of each vertex’s corresponding variables can be set as True.
# In order to check this we create for each vertex in the graph clauses consisting of the negated forms of any two of
# its variables.
# Number of clauses: O(n^3) clauses.
def no_node_twice(k, n):
    # Variable used for storing the clauses.
    clauses = ""

    # Create clauses for each vertex in the graph.
    for i in range(1, n + 1):
        # Create clauses for each variable of the current vertex.
        for j in range(1, k + 1):
            # Create the first variable of the clause.
            variable_1 = create_variable(i, j, k)

            # Create the following variables after the current one (variable_1 < variable_2) and a clause for each pair.
            for a in range(j + 1, k + 1):
                # Create the second variable of the clause.
                variable_2 = create_variable(i, a, k)

                # Create the clause and add it to the rest of the clauses.
                clause = "(" + "~" + str(variable_1) + "V" + "~" + str(variable_2) + ")"
                clauses += clause + "^"

    # Return the clauses created.
    return clauses


# Step 4: No two vertices a and b can occupy the same slot S(c).
# The clauses created at this point verify that only one vertex appears on the c’th position of the Vertex cover.
# -> A slot can't be occupied by two vertices simultaneously.
# In the SAT encoding that we're using this condition can be translated as:
# -> For each slot only one variable from the vertices' variables for it can be set as True.
# In order to check this we create for each slot clauses consisting of the negated forms of any two of the vertices'
# variables for it.
# Number of clauses: O(n^3) clauses.
def no_two_nodes_on_same_slot(k, n):
    # Variable used for storing the clauses.
    clauses = ""

    # Create clauses for each slot in the Vertex Cover.
    for i in range(1, k + 1):
        # Create clauses for each vertices' variables for the current slot.
        for j in range(1, n + 1):
            # Create the first variable of the clause.
            variable_1 = create_variable(j, i, k)

            # Create the following variables after the current one (vertex corresponding to variable_1 < vertex
            # corresponding to variable_2) for the current slot and a clause for each pair.
            for a in range(j + 1, n + 1):
                # Create the second variable of the clause.
                variable_2 = create_variable(a, i, k)

                # Create the clause and add it to the rest of the clauses.
                clause = "(" + "~" + str(variable_1) + "V" + "~" + str(variable_2) + ")"
                clauses += clause + "^"

    # Return the clauses created
    return clauses


# Function used for creating the polynomial transformation from k Vertex Cover to SAT.
# Total number of clauses created: O(n^2) + O(n) + O(n^3) + O(n^3) => O(n^3) clauses.
def kvc_to_sat():
    # Read the number of vertices in the Vertex Cover.
    k = read_variable()

    # Read the number of vertices in the graph.
    n = read_variable()

    # Read the number of edges in the graph.
    m = read_variable()

    # Create the clauses and add them to the CNF formula.
    # All the clauses are interconnected by "^"

    # Step 1 : Every edge is incident to at least one vertex in the vertex cover.
    sat_input = incidence_of_edges_to_the_vertex_cover(k, m)

    # Step 2: Each slot S(j) must be occupied.
    sat_input += occupy_all_slots(k, n)

    # Step 3: No vertex can appear twice in the k Vertex Cover.
    sat_input += no_node_twice(k, n)

    # Step 4: No two vertices a and b can occupy the same slot S(c).
    sat_input += no_two_nodes_on_same_slot(k, n)

    # Remove the ending "^" because there aren't any more clauses to add to the CNF formula.
    sat_input = sat_input[:-1]

    # Return the CNF formula.
    return sat_input


# Main function.
def main():
    # Read the graph and convert the k Vertex Cover input to SAT input.
    cnf_formula = kvc_to_sat()

    # Print the CNF formula (SAT input)
    print(cnf_formula)


if __name__ == "__main__":
    main()
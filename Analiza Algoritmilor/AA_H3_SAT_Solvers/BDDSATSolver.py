# Mateescu Viorel-Cristian
# Group: 322CB

# Class Node.
# It has two fields:
#   - cnf_expression - stores a CNF expression;
#   - literal_index - the index of the next literal to be evaluated to either True or False
class Node:
    def __init__(self, cnf_expression, literal_index):
        self.cnf_expression = cnf_expression
        self.literal_index = literal_index


# Class BDDSATSolver.
# Used for checking whether a CNF expression is satisfiable or not by the BDD method.
class BDDSATSolver:
    # Initializing the expression tree used for checking whether a CNF expression is satisfiable or not and the list of
    # literals in it.
    def __init__(self):
        self.expression_tree = []
        self.literals = []

    # Get the literals from the CNF expression.
    # Most of this method is found in the create_expression_matrix method in CNFSATSolver.
    # The CNF expression is split in clauses(^) and each clause in literals(V).
    # Each time a new literal is found it is added to the literals list.
    def get_literals(self, cnf_expression):
        # Split the CNF expression in clauses.
        clauses = cnf_expression.split("^")

        for clause in clauses:
            # Remove the trailing ( and ) in each clause and split it in literals(V).
            clause = clause.strip("()")
            literals_in_clause = clause.split("V")

            # Check for each literal in the clause whether or not it's in the literals list.
            # If it's not, then add it.
            for literal in literals_in_clause:
                # If it's negated remove its preceding ~.
                if literal.startswith("~"):
                    literal = literal[1:]

                # Convert it to an int.
                int_literal = int(literal)

                # Add the literal to the literals list if it isn't already in it.
                if int_literal not in self.literals:
                    self.literals.append(int_literal)

        # Sort the literals list in descending order.
        # I'm doing this because when I was replacing for example 1 with True in the expression, also 11 was replaced
        # with TrueTrue.
        self.literals.sort(reverse=True)

    # Static method used for replacing a literal in the CNF expression with the its value of truth(either True or False)
    # and simplifying it after that by removing the clauses evaluated to True, the False in a clause etc.
    # Rules:
    #   - ~True = False
    #   - ~False = True.
    @staticmethod
    def expression_simplifier(cnf_expression, literal_to_replace, value, negated_value):
        # Replace the literal with its value of truth.
        expression = cnf_expression.replace(literal_to_replace, value)

        # Replace the negated value of the literal (~Value) to its value of truth.
        expression = expression.replace("~" + value, negated_value)

        # If a clause in the expression is evaluated to False return False.
        if "(False)" in expression:
            return "False"

        # Split the expression in clauses.
        clauses = expression.split("^")

        # Remove the clauses evaluated to True and keep the rest of them.
        clauses = [_ for _ in clauses if "True" not in _]

        # If all the clauses are evaluated to True return True.
        if len(clauses) == 0:
            return "True"

        # Rebuild the expression from the remaining clauses.
        expression = "^".join(clauses)

        # Remove all the False's in the expression.
        expression = expression.replace("False", "")

        # Remove V if it appears after a (/V or before a ).
        # These expression can be found because there was a False that has been removed between/after/before the V.
        expression = expression.replace("VV", "V")
        expression = expression.replace("V)", ")")
        expression = expression.replace("(V", "(")

        # Return the simplified expression.
        return expression

    # Static method used for simplifying an expression and creating a node for it.
    @staticmethod
    def create_node(cnf_expression, literal_index, literal_to_replace, value, negated_value):
        expression = BDDSATSolver.expression_simplifier(cnf_expression, literal_to_replace, value, negated_value)
        return Node(expression, literal_index)

    # Method used for building the expression tree for the CNF expression.
    # The expression tree is stored in a list using the rule:
    #   - a node's left child is found at the index 2 * i + 1;
    #   - and its right child at the index 2 * i + 2, where i is the current node's index.
    # Each node has a literal index (the index of the literal to be evaluated to either True or False in its children
    # nodes) and a CNF expression (where the literal will be replaced to its value of truth in its children nodes).
    # In a node's left child its corresponding literal will be evaluated to False and in its right child to True.
    # If at least one node that stores an expression evaluated to True is found the building of the expression tree
    # stops and 1 (True) is returned
    # If there isn't any node with an expression evaluated to True then 0 (False) is returned (in order to be sure of
    # that the whole tree has to be built first).
    def build_expression_tree(self, cnf_expression):
        # Create a root node and add it to the expression tree.
        root = Node(cnf_expression, 0)
        self.expression_tree = [root]

        # Index used for going through the expression tree's nodes.
        node_index = 0

        # Try to create the left and right child for each node in the expression tree and check if their corresponding
        # CNF expressions are either True/False or their value of truth hasn't been determined yet.
        while node_index != len(self.expression_tree):
            # Get the current node.
            current_node = self.expression_tree[node_index]

            # If the current node's expression has already been evaluated to False move to the next one.
            if current_node.cnf_expression == "False":
                node_index += 1
                continue

            # Get the literal to be replaced by either True or False in the current node's expression and compute the
            # next literal's index.
            literal = str(self.literals[current_node.literal_index])
            literal_index = current_node.literal_index + 1

            # Create the current node's left child and add it to its position in the expression tree.
            # The corresponding literal will be evaluated to False, and its negation to True.
            left_child = self.create_node(current_node.cnf_expression, literal_index, literal, "False", "True")
            self.expression_tree.insert(2 * node_index + 1, left_child)

            # If the expression has been evaluated to true then the initial CNF expression (the one from the root node)
            # is satisfiable and 1 (True) is returned.
            if left_child.cnf_expression == "True":
                return 1

            # Create the current node's right child and add it to its position in the expression tree.
            # The corresponding literal will be evaluated to True, and its negation to False.
            right_child = self.create_node(current_node.cnf_expression, literal_index, literal, "True", "False")
            self.expression_tree.insert(2 * node_index + 2, right_child)

            # If the expression has been evaluated to true then the initial CNF expression (the one from the root node)
            # is satisfiable and 1 (True) is returned.
            if right_child.cnf_expression == "True":
                return 1

            # Move to the next node in the expression tree.
            node_index += 1

        # If the initial CNF expression hasn't been evaluated to True in any node then it is not satisfiable and 0
        # (False) is returned.
        return 0


# Main function.
def main():
    # Read the CNF expression.
    cnf_expression = input()

    # Initialize the BDD SAT Solver.
    bdd_sat_solver = BDDSATSolver()

    # Get the literals in the expression.
    bdd_sat_solver.get_literals(cnf_expression)

    # Check if the CNF expression is satisfiable or not.
    print(bdd_sat_solver.build_expression_tree(cnf_expression))


if __name__ == "__main__":
    main()

# Mateescu Viorel-Cristian
# Group: 322CB
from operator import mul

# Class CNFSATSolver.
# Used for checking whether a CNF expression is satisfiable or not by the matrix method.
class CNFSATSolver:
    # Initializing the matrix in which the clauses of the CNF expression will be stored and the number of literals in
    # the expression.
    def __init__(self):
        self.expression_matrix = []
        self.no_literals = 0

    # Compute the number of literals in the CNF expression by splitting the expression in clauses(^) and each clause
    # in literals(V).
    # The clause with the greatest number of literals will determinate the number of literals in the expression.
    def get_no_literals(self, clauses):
        # Remove the trailing ( and ) in each clause, split it in literals(V) and compute the number of
        # literals in each one.
        for clause in clauses:
            clause = clause.strip("()")
            literals = clause.split("V")

            # Keep the greatest number of literals from all the clauses in the no_literals field of the class.
            if len(literals) > self.no_literals:
                self.no_literals = len(literals)

    # Create the matrix in which each line corresponds to a clause in the CNF expression.
    # Rules for creating the expression matrix:
    #   - if a literal starts with '~' its sign in the matrix will be -1.
    #   - else it will be 1.
    def create_expression_matrix(self, cnf_expression):
        # List of literals in the CNF expression.
        literals_in_expression = []

        # Split the CNF expression in clauses and compute the number of literals in the expression.
        clauses = cnf_expression.split("^")
        self.get_no_literals(clauses)

        # For each clause create a line in the expression matrix.
        for clause in clauses:
            # Each line has a cell for each literal in the expression.
            line = [0] * self.no_literals

            # Remove the trailing ( and ) in each clause and split it in literals(V).
            clause = clause.strip("()")
            literals_in_clause = clause.split("V")

            # Fill the line's cells with the sign of each literal in the clause.
            for literal in literals_in_clause:
                # Suppose the literal isn't negated.
                sign = 1

                # If it's negated remove its preceding ~ and change its sign.
                if literal.startswith("~"):
                    sign = -1
                    literal = literal[1:]

                # Add the literal to the literals list if it isn't already in it.
                if literal not in literals_in_expression:
                    literals_in_expression.append(literal)

                # Fill its matrix cell with its sign.
                line[literals_in_expression.index(literal)] = sign

            # Add the created line to the expression matrix.
            self.expression_matrix.append(line)

    # Evaluate an interpretation by multiplying it with all the lines in the matrix.
    # An interpretation doesn't satisfy the expression if there is at least one clause(line in the matrix) that doesn't
    # satisfy it.
    # A clause is not satisfied by an interpretation if after multiplying the interpretation by the line corresponding
    # to the clause there isn't at lest one literal evaluated to 1 (True).
    def evaluate_interpretation(self, interpretation):
        print(interpretation)
        # Check if each clause in the expression is satisfied by the interpretation.
        for clause in self.expression_matrix:
            # Multiply each clause (line) in the expression (expression_matrix) by the interpretation.
            evaluated_clause = [*map(mul, clause, interpretation)]

            # If there's at least one literal evaluated to 1 (True) then the clause is satisfied.
            if 1 in evaluated_clause:
                continue

            # If this line is reached then a clause wasn't satisfied by the interpretation.
            return 0

        # If this line is reached the expression is satisfied by the interpretation.
        return 1

    # Build interpretations for the literals in the expression and check if they satisfy the CNF expression.
    # When an interpretation which satisfies the expression is found the method returns 1, which means that the CNF
    # expression is satisfiable.
    # If after building all the possible interpretations for the CNF expression and checking them there isn't any
    # interpretation that satisfies the expression the method returns 0, which means that the CNF expression is not
    # satisfiable.
    # A literal can be evaluated to True (1) or False (-1).
    def build_interpretations(self, interpretation):
        # If a complete interpretation has been built check if it satisfies the CNF expression.
        # An interpretation is complete if it has a value of truth (either -1 or 1) for each literal that exists in the
        # expression.
        if len(interpretation) == self.no_literals:
            return self.evaluate_interpretation(interpretation)

        # Make a copy of the interpretation.
        interpretation_copy = interpretation.copy()

        # Create two interpretations for the current literal:
        #   - one in which the literal is evaluated to False (-1).
        #   - one in which the literal is evaluated to True (1).
        interpretation_copy.append(-1)
        interpretation.append(1)

        # Build the rest of the interpretation for the first one and check if it satisfies the CNF expression.
        # If it does then return 1 (True), else do the same with the second one.
        # If none of them satisfy the expression then 0 is returned.
        return self.build_interpretations(interpretation_copy) or self.build_interpretations(interpretation)


# Main function.
def main():
    # Read the CNF expression.
    cnf_expression = input()

    # Initialize the CNF SAT Solver.
    cnf_sat_solver = CNFSATSolver()

    # Build the expression matrix.
    cnf_sat_solver.create_expression_matrix(cnf_expression)

    # Check if the CNF expression is satisfiable or not.
    print(cnf_sat_solver.build_interpretations([]))


if __name__ == "__main__":
    main()

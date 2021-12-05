{-# LANGUAGE ExistentialQuantification #-}
{-# LANGUAGE FlexibleInstances #-}

{-
  PP Project 2021

  This is where you will write the implementation for the given tasks.
  You can add other modules aswell.

  Name: Mateescu Viorel-Cristian
-}

module Tasks where

import Dataset
import Text.Printf
import Data.List
import Text.Read
import Data.Maybe

type CSV = String
type Value = String
type Row = [Value]
type Table = [Row]

{-
  TASK SET 1

  I only used the Table type when the [[String]] had a header.

-}

-- Task 1

{- Function which takes an Exam grades table and returns a table with columns 
   "Nume", "Punctaj Exam".
-}
compute_exam_grades :: Table -> Table
compute_exam_grades = \l -> table_header : pair_exam_grades l
    where
        table_header = ["Nume", "Punctaj Exam"] 

-- Function which paires each student with his/hers exam grade.
pair_exam_grades :: Table -> [Row]
pair_exam_grades (h:hs) = [[name, grade] | (name, grade) <- zip (map head hs) (get_exam_grades hs)]

-- Function which computes each student's exam grade.
get_exam_grades :: Table -> Row
get_exam_grades l = map ((printf "%.2f") . (`compute_exam_grade` 0) . tail) l

{- Function which computes a student's exam grade using the formula: 
   sum(Q_i | i = 1..6) / 4 + Ex.scris.
   
   If the student didn't answer a question his grade for that question will be 0.
-}
compute_exam_grade :: [Value] -> Float -> Float
compute_exam_grade [h] grade = (grade / 4) + (read h :: Float)
compute_exam_grade (h:hs) grade 
    | h /= "" = compute_exam_grade hs (grade + (read h :: Float))
    | otherwise = compute_exam_grade hs grade

-- Task 2

{- Function which computes the number of students who have passed the exam.
   
   If a student's exam grade is greater than 2.5 he has passed the exam.
-}
get_passed_students_num :: Table -> Int
get_passed_students_num = \l -> length (filter (>= 2.5) (extract_exam_grades l))

-- Function which extracts the student's exam grades from a "Nume", "Punctaj Exam" table.
extract_exam_grades :: Table -> [Float]
extract_exam_grades l = map (read . (!! 1)) (tail $ compute_exam_grades l)

{- Function which computes the percentage of students who have passed the exam.
   
   The formula is: number of students who have passed the exam / total number of students. 
-}
get_passed_students_percentage :: Table -> Float
get_passed_students_percentage = \l -> ((fromIntegral . get_passed_students_num) l) / (compute_no_students l)

-- Function which computes the number of students enrolled at this course.
compute_no_students :: Table -> Float
compute_no_students l = (fromIntegral . length . tail) l

-- Function which computes the average exam grade.
get_exam_avg :: Table -> Float
get_exam_avg = \l -> read $ printf "%.2f" $ compute_avg l

{- Auxiliary function used for computing the average exam grade using the formula:
   sum(student's exam grade) / total number of students.
-}
compute_avg :: Table -> Float
compute_avg l = (sum $ extract_exam_grades l) / (compute_no_students l)

-- Function which computes the number of students who gained at least 1.5p from homework.
get_passed_hw_num :: Table -> Int
get_passed_hw_num = \(h:hs) -> length (filter (>=1.5) (map ((compute_hw_grade 0 0) . (drop 2)) hs))

{- Function used for computing a student's homework grade using the formula:
   sum(T_i | i = 1..3).
   
   If the student didn't do a homework its grade for that homework will be 0.  
-}
compute_hw_grade :: Float -> Integer -> Row -> Float
compute_hw_grade hw no_hw (h:hs)
    | no_hw == 3 = hw
    | h /= "" = compute_hw_grade (hw + read h :: Float) (no_hw + 1) hs
    | otherwise = compute_hw_grade hw (no_hw + 1) hs

-- Task 3

-- Function which computes the average score for each question from the interview.
get_avg_responses_per_qs :: Table -> Table
get_avg_responses_per_qs = \l -> table_header : (create_avg_responses_table l) : []
    where
        table_header = ["Q" ++ (show x) | x <- [1..6]]

{- Function used for creating the values row for an average score table of the 
   questions from the interview.
-}
create_avg_responses_table :: Table -> Row
create_avg_responses_table (h:hs) = map (printf "%.2f") (compute_avg_response_per_qs hs)

{- Function used for computing each question's average score.
   
   The formula is: sum(Q_i_answers) / no_questions.
-}
compute_avg_response_per_qs :: Table -> [Float]
compute_avg_response_per_qs l = map ((/ (fromIntegral $ length l)) . sum) (create_responses_table l)

-- Function used for creating a table which has each questions' answers as lines.
create_responses_table :: Table -> [[Float]]
create_responses_table l = [map (get_response ind) l | ind <- [1..6]]

{- Function used for extracting an answer from a "Question", "Answer" table by its index.
   
   If the question hasn't been answered it will be marked with 0.
-}
get_response :: Int -> Row -> Float
get_response ind l
    | l !! ind /= "" = read (l !! ind) :: Float
    | otherwise = 0

-- Task 4

{- Function used for counting how many answers were marked 0/1/2 on each question.
   
   Each question is paired with its answer summary.
-}
get_exam_summary :: Table -> Table
get_exam_summary = \l -> table_header : (zipWith (++) (questions) (compute_exam_summary l))
    where
        table_header = ["Q"] ++ [(show ind) | ind <- [0..2]]
        questions = [["Q" ++ (show ind)] | ind <- [1..6]]

{- Auxiliary function used for computing each questions' answers summary.
   
   Each question's answers summary is created by uniting the summary for each 
   answer of the questions.
   
   For example: The first question's answers summary consists of the union of 
   the first element of each answers summary list (I'm using tranpose for this task).
-}
compute_exam_summary :: Table -> [Row]
compute_exam_summary l = map (map (printf "%d")) (transpose $ get_ans_summary l)

{- Function used for computing the answer summary by creating three lists composed of
   the summary of an answer for each list.
   
   The summary looks like this: [[no "i" answers for each question] | i = 0..2]
-}
get_ans_summary :: Table -> [[Int]]
get_ans_summary (h:hs) = [map (length . filter (ans == )) (create_responses_table hs) | ans <- [0..2]]

-- Task 5

{- Function used for sorting the students by their final exam grades in ascending 
   order. 

   If two students have the same final score, they will be listed in alphabetical 
   name order.
-}
get_ranking :: Table -> Table
get_ranking = \l -> table_header : (sortBy cmp (tail (compute_exam_grades l)))
    where 
        table_header = ["Nume", "Punctaj Exam"]

        cmp :: Row -> Row -> Ordering
        cmp s1 s2
            | (read (s1 !! 1) :: Float) > (read (s2 !! 1) :: Float) = GT
            | (s1 !! 0) > (s2 !! 0) && (read (s1 !! 1) :: Float) == (read (s2 !! 1) :: Float) = GT
            | otherwise = LT

-- Task 6

{- Function used for sorting the students by their difference between the written 
   part and the interview one of an online exam in ascending order. 

   If two students have the same difference, they will be listed in alphabetical 
   name order.  
-}
get_exam_diff_table :: Table -> Table
get_exam_diff_table = \l -> table_header : (sortBy cmp (create_exam_diff_table l))
    where
        table_header = ["Nume", "Punctaj interviu", "Punctaj scris", "Diferenta"]
        
        cmp :: Row -> Row -> Ordering
        cmp s1 s2
            | (read (s1 !! 3) :: Float) > (read (s2 !! 3) :: Float) = GT
            | (s1 !! 0) > (s2 !! 0) && (read (s1 !! 3) :: Float) == (read (s2 !! 3) :: Float) = GT
            | otherwise = LT

{- Function used for pairing each student's info about the final exam with the 
   difference between its two parts.
-}
create_exam_diff_table :: Table -> [Row]
create_exam_diff_table l = [stud_data ++ diff | (stud_data, diff) <- zip (create_exam_table l) (get_diffs l)]

{- Function used for pairing each student's name with its grades for the two 
   parts of the final exam.
-}
create_exam_table :: Table -> [Row]
create_exam_table (h:hs) = [[name] ++ grades | (name, grades) <- zip (map head hs) (get_online_grades hs)]

{- Function used for computing each student's difference between the two parts 
of the final exam.
-}
get_diffs :: Table -> [Row]
get_diffs l = map compute_diff (create_exam_table l)

{- Function used for computing a student's difference between the two parts of the
   final exam.
-}
compute_diff :: Row -> Row
compute_diff l = [printf "%.2f" $ abs $ (read (l !! 1) :: Float) - (read (l !! 2) :: Float)]

-- Function used for extracting each student's interview grade and written exam grade.
get_online_grades :: Table -> [Row]
get_online_grades l = map ((`get_online_exam_grades` 0) . tail) l

-- Function used for extracting a student's interview grade and written exam grade.
get_online_exam_grades :: [String] -> Float -> [String]
get_online_exam_grades [h] grade = [printf "%.2f" (grade / 4), printf "%.2f" (read h :: Float)]
get_online_exam_grades (h:hs) grade 
    | h /= "" = get_online_exam_grades hs (grade + (read h :: Float))
    | otherwise = get_online_exam_grades hs grade

-- Task Set 2

{- 
   Function which reads a string in CSV-format to a Table.
   
   The string is split first by '\n' and then, each each entry string produced 
   by the previous split is split by ',' to get the data for each column in a 
   table's entry.
-}
read_csv :: CSV -> Table
read_csv csv = map (split ',') $ split '\n' csv

{- 
   Function used for splitting a string in tokens by a specified character.
   
   I wrote this function during L03: Higher-order functions and I reused it 
   for this project. 
-}
split :: Char -> Value -> Row
split sep str = foldr op [[]] str
    where 
      op x (y:ys)
        | x == sep = []:y:ys
        | otherwise = (x:y):ys

{-
   Function which writes a string in CSV-format from a Table.

   Each column from an entry is joined by ',' and then, each entry string is 
   joined by '\n' to generate a CSV-format string.
-}
write_csv :: Table -> CSV
write_csv = (bind '\n') . (map (bind ','))

{-
   Function used for joining strings by a specified character.

   I wrote a similar function in L04: Working with matrices and images and 
   I adapted it for this task.
-}
bind :: Char -> Row -> Value
bind _ [] = []
bind _ [x] = x
bind sep (x:xs) =  x ++ sep:(bind sep xs)

-- Task 1

{-
   Function which takes a column name and a Table and returns the values from 
   that column as a list.

   In order to get the content from each column in same lines I transposed the 
   table.
-}
as_list :: String -> Table -> [String]
as_list name table = extract_col_by_name name (transpose table)

{-
   Function which takes a column name and a table and returns a row containing 
   all the data from the column with the specified name.
-}
extract_col_by_name :: Value -> Table -> Row
extract_col_by_name _ [] = []
extract_col_by_name name (x:xs)
    | name == head x = tail x
    | otherwise = extract_col_by_name name xs

-- Task 2

{-
   Function which takes a column name and a Table and returns the Table sorted 
   by that column (if multiple entries have the same values, then it is sorted 
   by the first column). I used two comparing functions for this: one for 
   strings and one for numeric data. 

   Sorting is ascending for columns with numeric values and lexicographical for 
   strings.

   If a value is missing I replaced it by "-1" in order for those entries to be 
   the first ones if the table has to be sorted by that column. After the 
   sorting is done I replace it back with "".
-}

tsort :: String -> Table -> Table
tsort name table = (head table) : (map (replace "-1" "") $ sortBy cmp (tail $ map (replace "" "-1") table))
    where 
        -- The index of the column after which the table must be sorted.
        index = get_col_index name 0 (head table)
        
        -- Deciding which comparing function should be used by checking the data type. 
        cmp
            | (readMaybe ((table !! 1) !! index) :: Maybe Float) == Nothing = cmpString
            | otherwise = cmpNumber
        
        -- Comparing function for strings.
        cmpString :: Row -> Row -> Ordering
        cmpString s1 s2
            | (s1 !! index) > (s2 !! index) = GT
            | otherwise = LT
        
        -- Comparing function for numeric strings.
        cmpNumber :: Row -> Row -> Ordering
        cmpNumber s1 s2
            | (read (s1 !! index) :: Float) > (read (s2 !! index) :: Float) = GT
            | (read (s1 !! index) :: Float) == (read (s2 !! index) :: Float) = cmpFirstCol s1 s2
            | otherwise = LT
        
        -- Comparing function for the first column.
        cmpFirstCol :: Row -> Row -> Ordering
        cmpFirstCol s1 s2
            | (s1 !! 0) > (s2 !! 0) = GT
            | otherwise = LT

{- 
   Function for replacing all occurences of a character in a string with 
   another one.
-}
replace :: [Char] -> [Char] -> [String] -> [String]
replace c1 c2 s = map (\c -> if c == c1 then c2 else c) s

-- Function for getting a colum's index in a table by its name.
get_col_index :: Value -> Int -> Row -> Int
get_col_index _ _ [] = -1
get_col_index name index (x:xs)
    | name == x = index
    | otherwise = get_col_index name (index + 1) xs

-- Task 3

-- Function which applies a function to all values in a table.
vmap :: (Value -> Value) -> Table -> Table
vmap op table = map (map op) table

-- Task 4

{- 
   Function which applies a function to all entries (rows) in a table (except 
   the table's header).
-}
rmap :: (Row -> Row) -> [String] -> Table -> Table
rmap op names table = names : tail (map op table)

{- 
   Function which takes a Row from hw_grades Table and returns a Row with 2 
   values: name, total points (the first two columns will be ignored when 
   calculating the total points => drop 2).
-}
get_hw_grade_total :: Row -> Row
get_hw_grade_total row = (head row) : [((printf "%.2f") . (compute_grade_total 0) . (drop 2)) row]

{- 
   Function for computing a student's total point by summing up all the points
   that he gained during course activities.

   If he didn't take part of an activity his points for it will be 0.
-}
compute_grade_total :: Float -> Row -> Float
compute_grade_total hw [] = hw
compute_grade_total hw (h:hs)
    | h /= "" = compute_grade_total (hw + read h :: Float) hs
    | otherwise = compute_grade_total hw hs

-- Task 5
{-
   Function which takes Tables t1 and t2 and adds all rows from t2 at the end 
   of t1, if column names coincide. 

   If columns names are not the same, t1 remains unchanged.
-}
vunion :: Table -> Table -> Table
vunion t1 t2 = if cmp_headers (head t1) (head t2) == True then (t1 ++ (tail t2)) else t1

{-
   Function which checks if two tables' headers are the same by comparing each 
   column name from their headers.

   In order to have two matching headers they should be of the same length and 
   teh columns should have the same names in the same order.
-}
cmp_headers :: Row -> Row -> Bool
cmp_headers [] [] = True
cmp_headers [] _ = False
cmp_headers _ [] = False
cmp_headers h1 h2
    | head h1 == head h2 = cmp_headers (tail h1) (tail h2)
    | otherwise = False

-- Task 6

{- 
   Function which adds new columns to a table (simple horizontal union of 2 
   tables).
-}
hunion :: Table -> Table -> Table
hunion t1 t2 = make_union t1 t2 (((length . head) t1) - 1) (((length . head) t2) - 1)

{- 
   Function for concatenating each entry from a table with its corresponding 
   one from another one, or with an empty one if it doesn't have a pair.
-}
make_union :: Table -> Table -> Int -> Int -> Table
make_union [] [] _ _ = []
make_union t1 [] no_col1 no_col2 = ((head t1) ++ (generate_empty_entry no_col2)) : make_union (tail t1) [] no_col1 no_col2
make_union [] t2 no_col1 no_col2 = ((generate_empty_entry no_col1) ++ (head t2)) : make_union [] (tail t2) no_col1 no_col2
make_union t1 t2 no_col1 no_col2 = ((head t1) ++ (head t2)) : make_union (tail t1) (tail t2) no_col1 no_col2

-- Function which generates a list of a specified number of empty strings.
generate_empty_entry :: Int -> [String]
generate_empty_entry n = ["" | x <- [0..n]]

-- Task 7

{-
   Function which does table join with respect to a key (a column name).

   If an entry from table1 (t1) has a match for the key in table2 (t2) then 
   they are merged.
   Else it is merged with an empty entry.

   In the end the rest of entries from table2 (t2) which didn't match with 
   any entry from table1 (t1) are merged with an emptry entry.
-}
tjoin :: String -> Table -> Table -> Table
tjoin key t1 t2 = table_header : ((match_entries key 1 t1 t2) ++ (add_left_entries key 1 t1 t2))
    where
      table_header = (head t1) ++ (get_entry_by_key (get_col_index key 0 (head t2)) key t2)

{-
   Function used for matching each entry from table1 (t1) with one from (t2) by a key.
   If no match is found then it is matched with an empty entry.
-}
match_entries :: Value -> Int -> Table -> Table -> Table
match_entries key index t1 t2
    | index == length t1 = []
    | match /= [] = make_full_entry key index t1 t2 : match_entries key (index + 1) t1 t2
    | otherwise = ((t1 !! index) ++ ((tail . empty_entry_helper) t2)) : match_entries key (index + 1) t1 t2

    where
        -- Checking if the entry has a match
        match = is_matching_entry key index t1 t2

{-
   Function used for matching the extra entries from table2 (t2) (the ones 
   that didn't match any entry from table1 by the key) with an empty string.
-}
add_left_entries :: Value -> Int -> Table -> Table -> Table
add_left_entries key index t1 t2
    | index == length t2 = []
    | match /= [] = add_left_entries key (index + 1) t1 t2
    | otherwise = (empty_entry_helper t1 ++ match) : add_left_entries key (index + 1) t1 t2

    where
        -- Checking if the entry has a match
        match = is_matching_entry key index t2 t1

{-
   Function used for checking if an entry from table1 (t1) has a matching 
   entry in table2 (t2) by a key.
-}
is_matching_entry :: String -> Int -> Table -> Table -> Row
is_matching_entry col_name index t1 t2 = get_entry_by_key (get_col_index col_name 0 (head t2)) key t2
    where
      {- 
         The key that should be searched in table2 (t2) in order to find the 
         matching entry for the entry from table1 (t1).
      -}
      key = (t1 !! index) !! (get_col_index col_name 0 (head t1))

{- 
   Function used for matching an entry from table1 (t1) with one from table2 
   (t2) by a key (the key is found on a specific column).
-}
make_full_entry :: String -> Int -> Table -> Table -> Row
make_full_entry key index t1 t2 = (t1 !! index) ++ (is_matching_entry key index t1 t2)

{-
   Helper function used for generating a list of empty strings (an empty 
   entry) for a table.

   The number of empty strings in the entry is equal to the number of columns 
   that the table has. 
-}
empty_entry_helper :: Table -> Row
empty_entry_helper table = generate_empty_entry (((length . head) table) - 1)

{-
   Function used for getting an entry from a table by a key and the column that 
   should be checked for the key. 

   The key is removed from the entry before returning it.
-}
get_entry_by_key :: Int -> String -> Table -> Row
get_entry_by_key index key [] = []
get_entry_by_key index key table
    | (head table) !! index == key = (take index (head table)) ++ (drop (index + 1) (head table))
    | otherwise = get_entry_by_key index key (tail table)


-- Task 8
{-
   Function which computes the cartesian product of 2 tables.

   Each entry from table1 (t1) is matched with each entry from table2 (t2) and 
   the specified operation is applied on each pair of entries.
-}
cartesian :: (Row -> Row -> Row) -> [String] -> Table -> Table -> Table
cartesian op names t1 t2 = names : [op entry1 entry2 | entry1 <- (tail t1), entry2 <- (tail t2)]

-- Task 9

{-
   Function which extracts from the table only the specified columns.

   Each column from the specified columns list is extracted using the function 
   from Task 1 and added to the new table. 

   In the end the table is transposed.  
-}
projection :: [String] -> Table -> Table
projection names table = names : (transpose [as_list name table | name <- names])

-- Task Set 3

data Query =
    FromCSV CSV
    | ToCSV Query
    | AsList String Query
    | Sort String Query
    | ValueMap (Value -> Value) Query
    | RowMap (Row -> Row) [String] Query
    | VUnion Query Query
    | HUnion Query Query
    | TableJoin String Query Query
    | Cartesian (Row -> Row -> Row) [String] Query Query
    | Projection [String] Query
    | forall a. FEval a => Filter (FilterCondition a) Query
    | Graph EdgeOp Query

type EdgeOp = Row -> Row -> Maybe Value

data QResult = CSV CSV | Table Table | List [String]

-- Task 3.1

{- 
   Enrolling QResult in class Show.
   
   For Table write_csv is used.
   For CSV and List the default show is used.
-}
instance Show QResult where
    show (Table table) = write_csv table
    show (List list) = show list
    show (CSV csv) = show csv

-- Task 3.2

class Eval a where
    eval :: a -> QResult

{-
   Enrolling Query in class Eval.
-}
instance Eval Query where
    --  Converts string str to a Table.
    eval (FromCSV str) = Table (read_csv str)

    -- Converts a table obtained from the evaluation of query to a String in CSV format.
    eval (ToCSV query) = CSV (write_csv (get_table query))

    -- Returns values from column colname as a list.
    eval (AsList colname query) = List (as_list colname (get_table query))

    -- Sorts table by column colname.
    eval (Sort colname query) = Table (tsort colname (get_table query))

    -- Maps all values from table, using op.
    eval (ValueMap op query) = Table (vmap op (get_table query))

    -- Maps all rows from table, using op.
    eval (RowMap op colnames query) = Table (rmap op colnames (get_table query))

    -- Vertical union of the 2 tables obtained through the evaluations of query1 and query2.
    eval (VUnion query1 query2) = Table (vunion (get_table query1) (get_table query2))

    -- Horizontal union of the 2 tables.
    eval (HUnion query1 query2) = Table (hunion (get_table query1) (get_table query2))

    -- Table join with respect to column colname.
    eval (TableJoin colname query1 query2) = Table (tjoin colname (get_table query1) (get_table query2))

    -- Cartesian product.
    eval (Cartesian op colnames query1 query2) = Table (cartesian op colnames (get_table query1) (get_table query2))

    -- Extract specified columns from table.
    eval (Projection colnames query) = Table (projection colnames (get_table query))
    
    -- Task 3.4
    eval (Filter filtercondition query) = Table (table_header : (filter (feval table_header filtercondition) (tail (get_table query))))
       where
        table_header = head (get_table query)

    -- Task 3.5
    eval (Graph edgeop query) = Table (["From", "To", "Value"] : (get_graph_table (get_table query) edgeop))

-- Function for extracting a table from a Query.
get_table :: Eval a => a -> Table
get_table query = un_table (eval query)

-- Function for extracting a table's content from a Table.
un_table :: QResult -> Table
un_table (Table table) = table

{- 
   Function for creating a table where each entry describes an edge between 
   node_i and node_j from the graph and will have the values:
    "From" = first column from node_i
    "To" = first column from node_j
    "Value" = edgeop node_i node_j

  The logic I used:
    Each node (entry) from the table can have endges with any other node (entry) 
    that is placed after it in the table.
    
    The edges for each node are computed and in the end the lists containing the 
    edges for each node are concatenated in one table.
-}
get_graph_table :: Table -> EdgeOp -> Table
get_graph_table table edgeop = concat [get_edges entry (drop index table) edgeop | (entry, index) <- zip (tail table) [2..]]

{-
   Function for computing a node's edges and adding them to a list.
   
   If the edgeop function returns Nothing then the two nodes don't have an edge 
   between them.

   Else if it returns a (Just value) then they are connected by an edge and 
   should have an entry for that edge (where the first column is lexicographically 
   before the second one). 
-}
get_edges :: Row -> Table -> EdgeOp -> [Row]
get_edges _ [] _ = []
get_edges entry (x:xs) edgeop
    | isNothing value == True = get_edges entry xs edgeop
    | (entry !! 0) > (x !! 0) = (make_graph_entry x entry (fromJust value)) : (get_edges entry xs edgeop)
    | otherwise = (make_graph_entry entry x (fromJust value)) : (get_edges entry xs edgeop)
      
      where
        value = edgeop entry x

{- 
   Function for making a graph entry:
     [ first column from first entry, 
     first column from second entry, 
     the computed distance between the nodes (entries) ]
-}
make_graph_entry :: Row -> Row -> Value -> Row
make_graph_entry entry1 entry2 value = [(entry1 !! 0), (entry2 !! 0), value]

-- Task 3.3

data FilterCondition a =
    Eq String a |
    Lt String a |
    Gt String a |
    In String [a] |
    FNot (FilterCondition a) |
    FieldEq String String

type FilterOp = Row -> Bool

class FEval a where
    feval :: [String] -> (FilterCondition a) -> FilterOp

-- Function for extracting the value from a specific column in a table entry.
get_value :: Row -> String -> Row -> Value
get_value entry colname colnames = entry !! (get_col_index colname 0 colnames)

-- Filtering functions for floats.
instance FEval Float where
    -- Checks if value from column colname is equal to ref.
    feval colnames (Eq colname ref) = \entry -> read (get_value entry colname colnames) == ref
    
    -- Checks if value from column colname is less than ref.
    feval colnames (Lt colname ref) = \entry -> read (get_value entry colname colnames) < ref

    -- Checks if value from column colname is greater than ref.
    feval colnames (Gt colname ref) = \entry -> read (get_value entry colname colnames) > ref

    -- Checks if value from column colname is in list.
    feval colnames (In colname list) = \entry -> if (value entry) == "" then False else elem (read (value entry)) list
      where
        value entry = get_value entry colname colnames

    -- Negates condition.
    feval colnames (FNot cond) = \entry -> not (feval colnames cond entry)
    
    -- Checks if values from columns colname1 and colname2 are equal.
    feval colnames (FieldEq colname1 colname2) = \entry -> get_value entry colname1 colnames == get_value entry colname2 colnames

-- Filtering functions for strings.
instance FEval String where
    -- Checks if value from column colname is equal to ref.
    feval colnames (Eq colname ref) = \entry -> get_value entry colname colnames == ref

    -- Checks if value from column colname is less than ref.
    feval colnames (Lt colname ref) = \entry -> get_value entry colname colnames < ref

    -- Checks if value from column colname is greater than ref.
    feval colnames (Gt colname ref) = \entry -> get_value entry colname colnames > ref

    -- Checks if value from column colname is in list.
    feval colnames (In colname list) = \entry -> if (value entry) == "" then False else elem (value entry) list
      where
        value entry = get_value entry colname colnames

    -- Negates condition.    
    feval colnames (FNot cond) = \entry -> not (feval colnames cond entry)
    
    -- Checks if values from columns colname1 and colname2 are equal.    
    feval colnames (FieldEq colname1 colname2) = \entry -> get_value entry colname1 colnames == get_value entry colname2 colnames

-- Task 3.6

{- 
   Graph containing similarities between students lecture points.
   
   The edges in the resulting graph (the rows in the resulting table) are sorted 
   by the "Value" column.
-}
similarities_query :: Query
similarities_query = Sort "Value" (Graph edge_op (FromCSV lecture_grades_csv))

{-
   If a student's email is missing the entry won't be included. => return Nothing
   
   If the distance between two students is smaller than 5 the entry won't be 
   included. => return Nothing 

   Otherwise there is an edge between the two students of weight value. => return (Just value)
-}
edge_op :: EdgeOp
edge_op l1 l2
    | head l1 == "" || head l2 == "" = Nothing
    | value >= 5 = Just (show value)
    | otherwise = Nothing
      
      where
        value = cmp_lecture_grade 0 (tail l1) (tail l2)

{-
   Function for comparing two students lecture grades and computing the distance 
   between their points.

   The distance between two students is the sum of questions where they both 
   received the same points.
-}
cmp_lecture_grade :: Integer -> Row -> Row -> Integer 
cmp_lecture_grade grade [] [] = grade
cmp_lecture_grade grade (x:xs) (y:ys)
    | x == y = cmp_lecture_grade (grade + 1) xs ys
    | otherwise = cmp_lecture_grade grade xs ys

-- Task Set 4

{-
    Function which receives the name of the column containing typos, the table 
    containing typos in CSV format, a reference table in CSV format (a table 
    where the values from that column are correct, but not necessarily in the 
    same order) and returns the first table where the typos have been fixed.
-}
correct_table :: String -> CSV -> CSV -> CSV
correct_table colname csv1 csv2 = write_csv (table_header : correct_entries)
    where
        {-
           Checks if the entry should be corrected (if it has a typo in the 
           column colname) and append the corrected entry to the table or the 
           original entry if it has no typo. 
        -}
        correct_entries :: Table
        correct_entries = [if (check_typo entry) then (correct_entry entry) else entry | entry <- (tail $ get_table values1)] 
        
        {- 
           Check if the entry has a typo (its info from the specified column 
           can be found in the 'diffs' list).
        -}
        check_typo :: Row -> Bool
        check_typo entry = elem (get_info entry) diffs

        {- 
           Function for correcting a typo (the value from the column containing 
           the typo is replaced with the correct value).
        -}
        correct_entry :: Row -> Row
        correct_entry entry = (get_match (get_info entry) values2) : (tail entry)
        
        -- Function for extracting the value from a specified column in a table entry.
        get_info :: Row -> Value
        get_info entry = get_value entry colname (head $ get_table values1)
        
        {- 
           List containing the entries from the specified column which contains 
           typos (the info from the specified column in the first table doesn't 
           match any info from that column in the second table).
        -}
        diffs :: [Value]
        diffs = un_list $ eval $ AsList colname (Filter (FNot (In colname values2)) values1)
        
        -- Function for extracting a list's content from a List.
        un_list :: QResult -> [Value]
        un_list (List list) = list

        -- Convert the first CSV to Query.
        values1 :: Query
        values1 = FromCSV csv1

        -- Extract the column containg typos by its name as a list from the second CSV.
        values2 :: [Value]
        values2 = un_list $ eval $ AsList colname (FromCSV csv2)

        table_header = head (get_table values1)

{-
   Function for getting an entry's matching entry which doesn't contain a typo 
   in the specified column.

   Compute the string distance between the value with typos and all the values from 
   the same column in the second table.

   Extract the minimum distance between the entries and return the matching entry. 
-}
get_match :: Value -> [Value] -> Value
get_match entry table = table !! (fromJust (elemIndex (get_index entry table) (map (distance entry) table)))

-- Function for extracting the minimum distance between the entries.
get_index :: Value -> [Value] -> Int
get_index entry table = (minimum (map (distance entry) table))

{- 
   Function for computing the minimum distance between two strings using dynamic programming.

   The edit distance that I chose to use for strings is the Levenshtein distance.
-}
distance :: String -> String -> Int
distance str1 str2 = dp !! (length str1) !! (length str2) 
    where
        -- Dynamic programming table for computing the edit distance.
        dp :: [[Int]]
        dp = [[compute_distance i j | j <- [0..(length str2)]] | i <- [0..(length str1)]]
        
        -- Function for computing the edit distance.
        compute_distance i 0 = i
        compute_distance 0 j = j
        compute_distance i j
            | str1 !! (i - 1) == str2 !! (j - 1) = dp !! (i - 1) !! (j - 1)
            | otherwise = 1 + minimum [dp !! (i - 1) !! j,  dp !! i !! (j - 1), dp !! (i - 1) !! (j - 1)]

{- 
   Function for joining tables: hw_grades, lecture_grades, exam_grades in one 
   table using a correct email_map table.

   The table is sorted by the first column (column "Nume").
-}
grades :: CSV -> CSV -> CSV -> CSV -> CSV
grades email_map_csv hw_grades_csv exam_grades_csv lecture_grades_csv = write_csv $ tsort "Nume" $ table_header : final_table
    where
        -- Function for deleting the Email column from the final table.
        final_table :: Table
        final_table = map (\entry -> comp_total ((take 1 entry) ++ (drop 2 entry))) (tail comp_final_table)

        -- Function for joining the tables by the columns "Email" or "Name", depending which one was used for storing the students' data.
        comp_final_table :: Table
        comp_final_table = tjoin "Nume" (tjoin "Email" (tjoin "Nume" table (comp_hw_grade (get_table (FromCSV hw_grades_csv)))) comp_lecture_table) comp_exam_table

        {- 
           Function for creating the lecture table containing the final lecture 
           grade for all students and their emails.
        -}
        comp_lecture_table :: Table
        comp_lecture_table = table_header : [(head entry) : [((lecture_grade 0 ((length entry) - 1)) . tail) entry] | entry <- (tail lecture_grades_table)]
            where
                table_header = ["Email", "Punctaj Curs"]

        {- 
           Function for removing the entries that don't have an email in the 
           lecture_grades table.
        -}
        lecture_grades_table :: Table
        lecture_grades_table = get_table (Filter (FNot (Eq "Email" "")) (FromCSV lecture_grades_csv))

        {- 
           Function for creating the homework table containing the final homework 
           grade for all students and their names.
        -}
        comp_hw_grade :: Table -> Table
        comp_hw_grade table = ["Nume", "Punctaj Teme"] : [(head entry) : [((hw_grade 0) . tail) entry] | entry <- (tail table)]

        {- 
           Function for creating the exam table containing the final exam 
           grade for all students and their names.
        -}
        comp_exam_table :: Table
        comp_exam_table = table_header : [(head entry) : [(((printf "%.2f") . (`compute_exam_grade` 0) . tail) entry)] | entry <- (tail exam_grades_table)]
            where
                table_header = ["Nume", "Punctaj Exam"]

        -- Function for computing a student's homework grade.
        hw_grade :: Float -> Row -> String
        hw_grade hw [] = (printf "%.2f") hw
        hw_grade hw (h:hs)
           | h /= "" = hw_grade (hw + read h :: Float) hs
           | otherwise = hw_grade hw hs
        
        -- Function for computing a student's lecture grade.
        lecture_grade :: Float -> Int -> Row -> String
        lecture_grade lecture no_lectures [] = (printf "%.2f") $ (lecture * 2) / (fromIntegral no_lectures)
        lecture_grade lecture no_lectures (h:hs)
           | h /= "" = lecture_grade (lecture + read h :: Float) no_lectures hs
           | otherwise = lecture_grade lecture no_lectures hs

        -- The exam grades table.
        exam_grades_table :: Table
        exam_grades_table = (get_table (FromCSV exam_grades_csv))
        
        -- Function for computing a student's final grade.
        comp_total :: Row -> Row
        comp_total entry
            | course_points entry < 2.5 || (get_grade entry 3)  < 2.5 = entry ++ ["4.00"]
            | otherwise = entry ++ [printf "%.2f" ((min (course_points entry) 5) + (get_grade entry 3))]
        
        -- Function for computing a student's course grade/points.
        course_points :: Row -> Float
        course_points entry = (get_grade entry 1) + (get_grade entry 2)
        
        -- Function for extracting a grade from a table.
        get_grade :: Row -> Int -> Float
        get_grade entry col = if entry !! col /= "" then (read (entry !! col) :: Float) else 0
        
        -- The correct_table containing the mapping between the students' names and their emails. 
        table :: Table
        table = get_table (FromCSV (correct_table "Nume" email_map_csv hw_grades_csv))

        table_header = ["Nume", "Punctaj Teme", "Punctaj Curs", "Punctaj Exam", "Punctaj Total"]

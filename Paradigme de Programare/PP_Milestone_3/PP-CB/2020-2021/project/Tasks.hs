{-
	PP Project 2021

	This is where you will write the implementation for the given tasks.
	You can add other modules aswell.
-}

{-# LANGUAGE ExistentialQuantification #-}
{-# LANGUAGE FlexibleInstances #-}

module Tasks where

import Dataset
import Text.Printf (printf)
import Data.List ( sortBy, sortOn, transpose )
import Data.Function (on)

import Data.Ord

type CSV = String
type Value = String
type Row = [Value]
type Table = [Row]

{-
	TASK SET 1
-}

-- Task 1
compute_exam_grades :: Table -> Table
compute_exam_grades tabel = ["Nume", "Punctaj Exam"] : map each_line (tail tabel) where
        each_line tabel = head tabel : [printf "%.2f" (formula tabel + exam_scris tabel)]

-- Functia "string_float" ia un element de tip String si il transforma in Integer

string_integer :: String -> Integer
string_integer x = read x :: Integer

-- Functia "string_float" ia un element de tip Char si il transforma in valoare Float
-- daca gaseste "" (spatiu liber) atunci pune valoarea de  0

string_float :: String -> Float
string_float x  | x == "" = 0 :: Float
                | otherwise = read x :: Float

-- transform fiecare element de pe coloana "Ex. Scris" in valoarea de tip Float
exam_scris :: [String] -> Float
exam_scris ceva = string_float (last ceva)

integer_float :: Integer -> Float
integer_float x = fromInteger x :: Float

-- Functia "sum_list_to_int" primeste o lista de caractere si le aduna intre ele
-- La fel ca in cazul precedent, daca gaseste "" inlocuieste cu 0, si aduna in continuare

sum_list_to_int :: [String] -> Integer
sum_list_to_int [] = 0
sum_list_to_int(elem:xs)  | elem == "" = 0 + sum_list_to_int xs
            | otherwise = string_integer elem + sum_list_to_int xs

formula :: [String] -> Float
formula ceva = integer_float (sum_list_to_int (init (tail ceva))) / 4

-- Task 2
-- Number of students who have passed the exam:
get_passed_students_num :: Table -> Int
get_passed_students_num tabel =  count_in_range 2.5 (list_char_to_float(tail(last_table_element (compute_exam_grades tabel))))

last_table_element :: [[b]] -> [b]
last_table_element = map last

-- transforma list char in list float
list_char_to_float :: [String] -> [Float]
list_char_to_float = map (read :: String -> Float)

-- count_numbers numere mai mari ca > 2.50p

sort_numbers :: Ord a => a -> a -> Bool
sort_numbers search_number x    | x > search_number = True
                                | otherwise = False

filter_numbers :: Ord a => a -> [a] -> [a]
filter_numbers search_number = filter(sort_numbers search_number)

count_numbers :: Num a => [b] -> a
count_numbers list = sum (map (const 1) list)

count_in_range :: (Num a, Ord b) => b -> [b] -> a
count_in_range search_number list = count_numbers (filter_numbers search_number list)

-- Percentage of students who have passed the exam:

get_passed_students_percentage :: Table -> Float
get_passed_students_percentage tabel = procent_studenti (pass_students tabel) (total_students tabel)

first_table_element :: [[b]] -> [b]
first_table_element = map head

-- totalul de studenti care au trecut examenul
pass_students :: Table -> Int
pass_students ceva = count_in_range 2.5 (list_char_to_float(tail(last_table_element(compute_exam_grades ceva))))


-- totalul de studenti din serie
total_students :: Table -> Int
total_students tabel = length (tail (first_table_element (compute_exam_grades tabel)))

procent_studenti x y = a / b
  where a = fromIntegral x
        b = fromIntegral y

-- Average exam grade
get_exam_avg :: Table -> Float
get_exam_avg tabel = average_exam (sumNotes_students tabel) (total_students tabel)

sumNotes_students :: Table -> Float
sumNotes_students ceva = sum (list_char_to_float (tail(last_table_element (compute_exam_grades ceva))))

average_exam :: Float -> Int -> Float
average_exam x y = a / b
  where
    a = x :: Float
    b = fromIntegral y

-- Number of students who gained at least 1.5p from homework:
get_passed_hw_num :: Table -> Int
get_passed_hw_num = average_homework

-- string_to_float_all (diferenta este ca imi converterste toate elementele)
-- si nu mai transforma "" in "0"

string_to_float_all :: String -> Float
string_to_float_all x = read x :: Float

-- aduna toate elementele dintr-o lista de stringuri si intoarce un numar float
-- iar lista initiala trebuie sa inceapa cu elementul 0

suma_string_float :: [String] -> Float
suma_string_float [] = 0
suma_string_float(x:xs) | x == "" = 0 + suma_string_float xs
                        | otherwise = string_to_float_all x + suma_string_float xs

--functia sum_homework (care este putin overkill), elimina "Nume", "Lab" din tabel folosind tail
-- elimina apoi restul de elemente de la coada prin init, pana raman doar coloanele cu teme (T1 T2 T3)
-- apoi prin functia suma_string_float adun fiecare linie cu teme pentru studentul respectiv


sum_homework :: [[String]] -> [Float]
sum_homework tabel =  map op (tail tabel) where
        op linie = suma_string_float (init(init(init (tail (init(tail linie))))))

-- folosind functia de la task ul precedent, numar cate note sunt mai mari ca ~1.50
-- daca era un student care avea fix >= 1.49, mai puneam o conditie speciala in functie

average_homework :: Num a => [[String]] -> a
average_homework tabel = count_in_range 1.49 (sum_homework tabel)

-- Task 3
get_avg_responses_per_qs :: Table -> Table
get_avg_responses_per_qs = table_Q_procent

-- functia "focus_Q" prelucreaza fiecare linie din Matrice/Tabela si elimina 
-- coloanele "Nume" si "Ex scris"

focus_Q :: [[String]] -> [[String]]
focus_Q tabel = map op (tail tabel) where
        op [] = []
        op linie = init(tail linie)

-- functia "get_row_matrix" i-a elementul linie specificat de pe matrice
-- lista reprezinta rows din matrice, iar position pozitia elementului
-- care trebuie luat, si folosit in prelucrare folosit

get_row_matrix :: [[b]] -> Int -> [b]
get_row_matrix lista position = map (!! position) lista

-- functia "replace_empty" inlocuieste elementele goale " ", cu valori de "0"
-- daca value == valoare de char atunci ramane value, if value == "" -> value = "0"

replace_empty unwanted wanted = map (\value -> if value == unwanted then wanted else value)

-- map read folosit pentru a transforma o lista de [String] in lista de [Int] 
-- [String] -> [Int]
-- apoi ma folosesc de sum pentru a aduna toate elementele de pe coloana Q1, Q2, etc
-- mai jos este reprezentata suma elementelor de pe fiecare coloana in parte, pentru Q-uri
-- de la 0 la 5, unde 0 este indexul primei coloane
sum_Q1 :: Int
sum_Q1 = sum (map read (replace_empty "" "0"(get_row_matrix (focus_Q exam_grades) 0)))
sum_Q2 = sum (map read (replace_empty "" "0"(get_row_matrix (focus_Q exam_grades) 1)))
sum_Q3 = sum (map read (replace_empty "" "0"(get_row_matrix (focus_Q exam_grades) 2)))
sum_Q4 = sum (map read (replace_empty "" "0"(get_row_matrix (focus_Q exam_grades) 3)))
sum_Q5 = sum (map read (replace_empty "" "0"(get_row_matrix (focus_Q exam_grades) 4)))
sum_Q6 = sum (map read (replace_empty "" "0"(get_row_matrix (focus_Q exam_grades) 5)))

-- avand in vedere ca fiecare suma a Q-urilor este de tip Int, iar numarul de studenti = 132
-- impartirea acestora pentru medie va fi de tipul Float
-- am scris functia convert_int_float pentru a reusi conversia impartirii a doua int-uri
-- in tip float, pentru a scoate procentul

convert_int_float :: Int -> Int -> Float
convert_int_float x y = a / b
  where
    a = fromIntegral x :: Float
    b = fromIntegral y :: Float

-- Se calculeaza procentul prin impartirea Sumei de Q-uri la nr de studenti
-- folosind functia convert_int_float
-- total_students = 132, functie pe care am creat-o la task precedent si care
-- imi afiseaza numarul total de studenti

resultQ1 :: String
resultQ1 = printf "%.2f" (convert_int_float sum_Q1 (total_students exam_grades))
resultQ2 = printf "%.2f" (convert_int_float sum_Q2 (total_students exam_grades))
resultQ3 = printf "%.2f" (convert_int_float sum_Q3 (total_students exam_grades))
resultQ4 = printf "%.2f" (convert_int_float sum_Q4 (total_students exam_grades))
resultQ5 = printf "%.2f" (convert_int_float sum_Q5 (total_students exam_grades))
resultQ6 = printf "%.2f" (convert_int_float sum_Q6 (total_students exam_grades))

-- imi adauga toate procentele calculate intr-o lista pe care urmeaza sa o concatenez

pp :: [String]
pp = [resultQ1, resultQ2, resultQ3, resultQ4, resultQ5, resultQ6]

-- Aici creez un tabel care concateneaza Q-urile din Matrice calculate, cu rezultatele fiecarui
-- procent al Q-urilor
-- prima data elimin "Nume" si "Ex scris", ca sa ramana doar Q1, Q2, etc. la care adaug
-- lista de procente calculate

table_Q_procent :: Table -> Table
table_Q_procent tabel = init (tail (head tabel)) : [pp]

-- Task 4
get_exam_summary :: Table -> Table
get_exam_summary = average_Qs

-- functie care imi intoarce True, daca gaseste numarul pe care il caut

sort_numbers_idem :: Ord a => a -> a -> Bool
sort_numbers_idem search_number x       | x == search_number = True
                                        | otherwise = False

-- cautam in fiecare element din lista, si sortam dupa numere, care sunt
-- True, si care sunt False.. Un fel de eticheta pentru fiecare element

filtram_numerele :: Ord a => a -> [a] -> [a]
filtram_numerele search_number = filter(sort_numbers_idem search_number)

-- adunam cate elemente au fost gasite cu True

sum_filtram_numere :: Num a => [b] -> a
sum_filtram_numere list = sum (map (const 1) list)

-- scoatem numarul total de elemente din fiecare lista in parte
-- in functie de numarul de 0, 1, 2 al fiecarui student

countInRange1 :: (Num a, Ord b) => b -> [b] -> a
countInRange1 search_number list = sum_filtram_numere (filtram_numerele search_number list)

-- De aici, am fost putin overkil (mai mult); imi cer scuze pentru abordarea barbara pe care
-- am adoptat-o, dar nu mi-a venit o idee/algoritm mai eficient .. am facut babeste (rau de tot) :(

count = show (countInRange1 0 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 0))))
count1 = show (countInRange1 1 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 0))))
count2 = show (countInRange1 2 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 0))))

res_q1 :: [String]
res_q1 = ["Q1", count, count1, count2]

count3 = show(countInRange1 0 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 1))))
count4 = show(countInRange1 1 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 1))))
count5 = show(countInRange1 2 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 1))))

res_q2 = ["Q2", count3, count4, count5]

count6 = show(countInRange1 0 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 2))))
count7 = show(countInRange1 1 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 2))))
count8 = show(countInRange1 2 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 2))))

res_q3 = ["Q3", count6, count7, count8]

count9 = show(countInRange1 0 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 3))))
count11 = show(countInRange1 1 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 3))))
count12 = show(countInRange1 2 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 3))))

res_q4 = ["Q4", count9, count11, count12]

count13 = show(countInRange1 0 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 4))))
count14 = show(countInRange1 1 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 4))))
count15 = show(countInRange1 2 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 4))))

res_q5 = ["Q5", count13, count14, count15]

count16 = show(countInRange1 0 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 5))))
count17 = show(countInRange1 1 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 5))))
count18 = show(countInRange1 2 (map read(replace_empty"" "0"(get_row_matrix (focus_Q exam_grades) 5))))

res_q6 = ["Q6", count16, count17, count18]

res_q =  [res_q1,res_q2, res_q3, res_q4, res_q5, res_q6]

average_Qs :: Table -> Table
average_Qs tabel = ["Q", "0", "1", "2"] : res_q

-- Task 5
get_ranking :: Table -> Table
get_ranking tabel = ["Nume", "Punctaj Exam"] : sortOn (\(x : y : _) -> (read y :: Float, x)) (tail(compute_exam_grades exam_grades))

-- Am folosit functia sortOn, pentru a sorta elementele bazate pe rezultatul apelului de functie, (un fel de recursie)
-- sortOn este echivalent cu sortBy (comparand o functie), dar are avantajul de a evalua lista o singura data,
-- pentru fiecare element din lista de intrare. Elementele sunt aranjate de la cel mai mic la cel mai mare, 
-- pastrand duplicatele in ordinea in care au aparut, apoi sunt sortate alfabetic.

-- Task 6
get_exam_diff_table :: Table -> Table
get_exam_diff_table = undefined


{-
	TASK SET 2
-}

splitBy :: Char -> String -> [String]
splitBy c = foldr op []
  where
    op x []
      | x == c = "" : [""]
      | otherwise = [[x]]
    op x (y : ys)
      | x == c = [] : (y : ys)
      | otherwise = (x : y) : ys

read_csv :: CSV -> Table
read_csv string = map (splitBy ',') (splitBy '\n' string)

linetoStr :: [String] -> String
linetoStr = foldr op "" where
  op x "" = x++ "\n"
  op x acc = x ++ "," ++ acc

remove_first_char :: Char -> String -> String
remove_first_char _ [] = []
remove_first_char c1 (c2 : cs) = if c1 == c2 then cs else c2 : remove_first_char c1 cs

remove_last_char :: Char -> String -> String
remove_last_char c1 = reverse . remove_first_char c1 . reverse

f :: Table -> CSV
f = foldr op "" where -- spatiu la final daca ramane " "
  op line acc = linetoStr line ++ acc

write_csv :: Table -> CSV
write_csv tabel = remove_last_char '\n' (f tabel)

-- task 1

-- here we go again. O alta functie overkill (scuze, promit ca la urmatorul milestone o modific)

as_list :: String -> Table -> [String]
as_list column tabel    | column == head (head tabel) =  map head (tail tabel)
                        | column == head (tail (head tabel)) = map (head . tail) (tail tabel)
                        | column == head (tail (tail (head tabel))) = map (head . tail . tail) (tail tabel)
                        | column == head (tail (tail (tail (head tabel)))) = map (head . tail . tail . tail) (tail tabel)
                        | column == head (tail (tail (tail (tail (head tabel))))) = map (head . tail . tail . tail . tail) (tail tabel)
                        | column == head (tail (tail (tail (tail (tail (head tabel)))))) = map (head . tail . tail . tail . tail . tail) (tail tabel)
                        | column == last (init (head tabel)) = map (last . init) (tail tabel)
                        | column == last (head tabel) = map last (tail tabel)

-- task 2

tsort :: String -> Table -> Table
tsort = undefined

-- task 3

-- o functie simpla apelata prin doua map-uri asupra unui tabel

vmap :: (Value -> Value) -> Table -> Table
vmap f = map (map f)

-- task 4

-- sum_list_to_float, este copie identica cu functia sum_list_to_int de la milestone 1
-- doar ca aceasta intoarce un float in loc de int ca suma

sum_list_to_float :: [String] -> Float
sum_list_to_float [] = 0
sum_list_to_float (x : xs)
  | x == "" = 0 + sum_list_to_float xs
  | otherwise = string_float x + sum_list_to_float xs

rmap :: (Row -> Row) -> [String] -> Table -> Table
rmap v row tabel = row : map v (tail tabel)

get_hw_grade_total :: Row -> Row
get_hw_grade_total linie = head linie : [printf "%.2f" (sum_list_to_float (tail (tail linie)))]

-- task 5

vunion :: Table -> Table -> Table
vunion tabel tabel1 = if head tabel == head tabel1 then tabel ++ tail tabel1 else tabel

-- task 6

combineTable :: [[a]] -> [[a]] -> [[a]]
combineTable xs [] = xs
combineTable [] ys = ys
combineTable (x : xs) (y : ys) = (x ++ y) : combineTable xs ys

-- in divisionTable adaug diferenta dintre tabele (lengthul fiecarui tabel in parte)

divisionTable :: Table -> Table -> Table
divisionTable t1 t2 = if length t1 < length t2 then t1 ++ rowEmptyTable (length t2 - length t1) t1 else t1

-- un row de empty strings (len chestii goale)

rowEmpty :: Int -> Row
rowEmpty 0 = []
rowEmpty len = "" : rowEmpty (len - 1)

-- creaza un tabel cu len randuri goale

rowEmptyTable :: Int -> Table -> Table
rowEmptyTable 0 t = []
rowEmptyTable len t = rowEmpty(length (head t)) : rowEmptyTable (len-1) t

-- tabel 1 = 20 tabel 2 = 30 || if (tabel1 <= tabel2) --> dif = 30 - 20 = 10 

hunion :: Table -> Table -> Table
hunion tabel tabel1 = combineTable (divisionTable tabel tabel1) (divisionTable tabel1 tabel)

-- task 7

tjoin :: String -> Table -> Table -> Table
tjoin = undefined

-- task 8

cartesian :: (Row -> Row -> Row) -> [String] -> Table -> Table -> Table
cartesian = undefined

-- task 9

projection :: [String] -> Table -> Table
projection lista tabel = undefined

{-- 
----------------------------------------------------------------------------------------
Etapa 3 - PP
----------------------------------------------------------------------------------------
--}

data Query
  = FromCSV CSV
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

-- where EdgeOp is defined:
type EdgeOp = Row -> Row -> Maybe Value



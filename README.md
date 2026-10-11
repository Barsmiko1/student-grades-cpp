# Student Final Grade Calculator

A C++ console application that collects students' homework and exam results
and calculates each student's final grade.

Functionality:

- enter student data by hand (name, surname, homework results, exam result) -
  the number of homework results does not have to be known in advance;
- generate homework and exam results randomly;
- read student data from a text file;
- print a formatted table with the final grades, sorted by name.

## Final grade formula

```
Final points = 0.4 * (Average of HW | Median of HW) + 0.6 * Exam
```

The homework part is either the **average** or the **median** of all homework
results, depending on the user's choice.

## Build and run

```
make
./grades                 # interactive menu
./grades Students.txt    # read students from a file
```

Without `make`: `g++ -std=c++17 -O2 main.cpp Person.cpp -o grades`

## How to use

Running `./grades` shows a menu with three ways to provide the results:

```
How do you want to provide the results?
  1 - type them in
  2 - generate them randomly
  3 - read them from a file
Choice:
```

In options 1 and 2 there is no fixed number of students. Add as many
students as you like, then press Enter on an empty line to finish. After
that the program asks whether to calculate the final grade from the
**average** (1) or the **median** (2) of the homework results and prints
the table, sorted by name.

### 1 - type them in

Type one student per line: name, surname, the homework results and finally
the exam result. The number of homework results is up to you and can differ
between students - the last number on the line is always the exam. Results
must be whole numbers from 0 to 10.

```
Student 1> John Smith 8 9 10 9
Student 2> Anna Brown 7 6 5
Student 3>

Calculate the final grade using:
  1 - average of homework
  2 - median of homework
Choice: 1

Name           Surname        Final_Point(Aver.)
------------------------------------------------
Anna           Brown                        5.60
John           Smith                        9.00
```

Here John has three homework results (8, 9, 10) and exam 9; Anna has two
homework results (7, 6) and exam 5.

### 2 - generate them randomly

First enter how many homework results each student should get (this is
**not** the number of students). Then type only the name and surname of
each student; the homework and exam results are random numbers from 1 to 10.

```
How many homework results should each student get? 3

Now enter the students, one per line: Name Surname
Add as many students as you like, then press Enter on an empty line to finish.
Student 1> John Smith
Student 2> Anna Brown
Student 3>
```

### 3 - read them from a file

Enter the file name, or just press Enter to use `Students.txt`. The same
can be done without the menu: `./grades Students.txt`. For a file the
program prints both final grades (average and median) for every student.

## Data file format

The first line is a header; every other line is one student. Any number of
homework columns is accepted - the last number on the line is the exam.

```
Name          Surname        HW1   HW2   HW3   HW4   HW5   Exam
Michael       Johnson          8     9    10     6    10      9
```

For a file the program prints both final grades:

```
Name                Surname              Final (Avg.) | Final (Med.)
--------------------------------------------------------------------
Ava                 Wilson                       9.84 |        10.00
Emma                Williams                     6.32 |         6.40
```

The large test files (`students10000.txt`, `students100000.txt`,
`students1000000.txt`) are not stored in the repository because of their
size. For those it is handy to redirect the output:
`./grades students1000000.txt > results.txt`

## Project structure

- `Person.h`, `Person.cpp` - class `Person`: student data, rule of three,
  overloaded `>>` and `<<`, final grade calculation;
- `main.cpp` - menu, input, file reading, sorting and output;
- `Students.txt` - sample data file.

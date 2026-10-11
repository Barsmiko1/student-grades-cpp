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

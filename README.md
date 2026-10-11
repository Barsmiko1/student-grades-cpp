# Student Final Grade Calculator

A C++ console application that collects students' homework and exam results
and calculates each student's final grade.

Functionality:

- enter student data by hand (name, surname, homework results, exam result) -
  the number of homework results does not have to be known in advance;
- generate homework and exam results randomly;
- read student data from a text file;
- print a formatted table with the final grades, sorted by name;
- generate large random data files for testing (v0.2);
- divide the students of a file into "passed" and "failed", write each group
  to its own file and measure how long every step takes (v0.2);
- do the same with `std::vector`, `std::list` or `std::deque` and compare
  their speed (v0.25).

## Final grade formula

```
Final points = 0.4 * (Average of HW | Median of HW) + 0.6 * Exam
```

The homework part is either the **average** or the **median** of all homework
results, depending on the user's choice.

## Build and run

```
make
./grades                    # menu
./grades Students.txt       # read a file and print both final grades
./grades generate           # generate the five test data files
./grades test               # speed test: all three containers, five generated files
./grades test list          # speed test with one container (vector, list or deque)
./grades test Students.txt  # speed test on the given file(s)
```

Without `make`:
`g++ -std=c++17 -O2 main.cpp Person.cpp Students.cpp FileIO.cpp Console.cpp SpeedTest.cpp -o grades`

## How to use

Running `./grades` shows a menu:

```
What do you want to do?
  1 - type the results in
  2 - generate the results randomly
  3 - read the results from a file
  4 - generate the five test data files
  5 - split a file into passed/failed and measure the speed
Choice:
```

In options 1 and 2 there is no fixed number of students. Add as many
students as you like, then press Enter on an empty line to finish. After
that the program asks whether to calculate the final grade from the
**average** (1) or the **median** (2) of the homework results and prints
the table, sorted by name.

### 1 - type the results in

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

Name                Surname             Final_Point(Aver.)
----------------------------------------------------------
Anna                Brown                             5.60
John                Smith                             9.00
```

Here John has three homework results (8, 9, 10) and exam 9; Anna has two
homework results (7, 6) and exam 5.

### 2 - generate the results randomly

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

### 3 - read the results from a file

Enter the file name, or just press Enter to use `Students.txt`. The same
can be done without the menu: `./grades Students.txt`. For a file the
program prints both final grades (average and median) for every student.

### 4 - generate the five test data files

Enter how many homework results each student should get. The program then
creates five files with 1 000, 10 000, 100 000, 1 000 000 and 10 000 000
students named `Name1 Surname1`, `Name2 Surname2`, ... with random results:

```
generated1000.txt  generated10000.txt  generated100000.txt
generated1000000.txt  generated10000000.txt
```

With 5 homework results the largest file is about 730 MB, so make sure
there is enough free disk space. `./grades generate` does the same with
5 homework results.

### 5 - split a file into passed/failed and measure the speed

Enter a file name, or just press Enter to process all five generated files.
Then choose the container (`std::vector`, `std::list`, `std::deque` or all
three to compare them) and average or median. For every file the program:

1. reads the students and calculates their final grades;
2. sorts them by final grade, best first (same grade: by name);
3. divides them into two groups - **passed** (final grade >= 5.0) and
   **failed** (final grade < 5.0);
4. writes each group to a new file: for `data.txt` these are
   `data_passed.txt` and `data_failed.txt`.

At the end it prints how many seconds every step took.

## Speed analysis (v0.2)

Whole program operating time for the five generated files, in seconds
(`./grades test`, final grade by average, 5 homework results per student,
`std::vector` container):

| Students   | Read   | Sort  | Split | Write passed | Write failed | Total  |
|-----------:|-------:|------:|------:|-------------:|-------------:|-------:|
| 1 000      | 0.001  | 0.000 | 0.000 | 0.001        | 0.000        | 0.002  |
| 10 000     | 0.012  | 0.001 | 0.000 | 0.005        | 0.004        | 0.022  |
| 100 000    | 0.132  | 0.016 | 0.005 | 0.031        | 0.021        | 0.208  |
| 1 000 000  | 1.201  | 0.200 | 0.076 | 0.365        | 0.249        | 2.128  |
| 10 000 000 | 10.592 | 2.633 | 1.958 | 3.492        | 2.617        | 22.131 |

"Read" includes calculating the final grades. Generating the files is not
part of the measured time (it took 9.9 s for the 10 000 000 file).

Test machine: Apple M3, 8 GB RAM, SSD, macOS 26.5, Apple clang 21, `-O2`.

What the numbers show:

- the time grows in step with the number of students: ten times more
  students take about ten times longer;
- reading the file is the slowest step - about half of the total time -
  because every line has to be turned from text into numbers;
- sorting 10 000 000 students takes under 3 seconds, because students are
  moved, not copied, while sorting.

## Speed analysis of the containers (v0.25)

The same steps were repeated with three containers holding the students:
`std::vector`, `std::list` and `std::deque` (`./grades test`, same five
files, same machine, one run each). All three produce exactly the same
passed / failed files.

Total program operating time in seconds:

| Students   | std::vector | std::list | std::deque |
|-----------:|------------:|----------:|-----------:|
| 1 000      | 0.005       | 0.001     | 0.001      |
| 10 000     | 0.020       | 0.014     | 0.013      |
| 100 000    | 0.190       | 0.172     | 0.157      |
| 1 000 000  | 2.298       | 2.320     | 1.855      |
| 10 000 000 | 24.398      | 38.869    | 24.055     |

Every step for the largest file (10 000 000 students), in seconds:

| Container   | Read   | Sort  | Split | Write passed | Write failed | Total  |
|-------------|-------:|------:|------:|-------------:|-------------:|-------:|
| std::vector | 11.472 | 2.758 | 1.552 | 4.516        | 3.421        | 24.398 |
| std::list   | 10.732 | 8.078 | 8.065 | 4.546        | 3.282        | 38.869 |
| std::deque  | 10.497 | 2.968 | 1.227 | 5.033        | 3.323        | 24.055 |

What the numbers show:

- reading and writing take about the same time with every container - that
  time is spent on the files, not on the container;
- up to 1 000 000 students the three containers are close; the differences
  for the smallest files are a few milliseconds and mostly measurement noise
  (the vector row is run first);
- at 10 000 000 students `std::list` is clearly the slowest: sorting takes
  about 3 times longer and splitting about 5 times longer than with
  `std::vector`. A list keeps every student in a separate piece of memory,
  so it has to jump around the memory and needs more of it;
- `std::vector` and `std::deque` are practically equal; `std::deque` is
  slightly faster at reading and splitting because it grows in blocks and
  never has to move the students it already holds.

## Exception handling (v0.2)

Errors are reported with exceptions (`Errors.h`) and caught in `main()`:

- `FileError` - a file cannot be opened, created or written;
- `DataError` - a file opens but contains no students;
- `std::bad_alloc` - not enough memory for that many students;
- `std::invalid_argument` / `std::out_of_range` - a menu answer that is not
  a number or is outside the allowed range (the question is asked again).

In the speed test a file that cannot be processed is reported and the test
continues with the next file.

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

Large data files (`students1*.txt`, `generated*.txt`) and the
`*_passed.txt` / `*_failed.txt` results are not stored in the repository
because of their size.

## Project structure

- `Person.h`, `Person.cpp` - class `Person`: student data, rule of three,
  overloaded `>>` and `<<`, final grade calculation;
- `Students.h`, `Students.cpp` - sorting a list of students and dividing it
  into passed and failed (templates: any of the three containers);
- `FileIO.h`, `FileIO.cpp` - reading, writing and generating data files
  (reading and writing are templates);
- `Console.h`, `Console.cpp` - keyboard input and tables on the screen;
- `SpeedTest.h`, `SpeedTest.cpp` - the speed analysis;
- `Timer.h` - class `Timer` that measures elapsed time;
- `Errors.h` - exception classes `FileError` and `DataError`;
- `main.cpp` - the menu and exception handling;
- `Students.txt` - sample data file.

## Releases

- **v0.1** - `Person` class, manual / random / file input, average or median.
- **v0.2** - code divided into several `.h` / `.cpp` files, exception
  handling, test file generation, passed / failed split, speed analysis.
- **v0.25** - the same code working with `std::vector`, `std::list` and
  `std::deque`, speed comparison of the three containers.

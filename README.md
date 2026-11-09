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
  their speed (v0.25);
- divide the students with two different strategies, written both with
  plain loops and with `<algorithm>`, and compare speed and memory (v1.0).

## Final grade formula

```
Final points = 0.4 * (Average of HW | Median of HW) + 0.6 * Exam
```

The homework part is either the **average** or the **median** of all homework
results, depending on the user's choice.

## Usage guide

### 1. Build

With **make** (macOS, Linux):

```
make
```

This creates the program `grades` in the project folder. `make clean`
removes it again.

With **CMake** (any OS, including Windows):

```
cmake -S . -B build
cmake --build build --config Release
```

The program is then `build/grades` (`build\Release\grades.exe` with Visual
Studio).

Without either: `g++ -std=c++17 -O2 main.cpp Person.cpp Students.cpp FileIO.cpp Console.cpp SpeedTest.cpp -o grades`

### 2. Try it on the sample file

```
./grades Students.txt
```

prints every student of `Students.txt` with both final grades.

### 3. Generate the test data

```
./grades generate
```

creates `generated1000.txt` ... `generated10000000.txt` (1 000 to
10 000 000 students, 5 homework results each). The largest file is about
730 MB, so make sure there is enough disk space.

### 4. Run the speed test

```
./grades test                              # every container and strategy, all five files
./grades test deque partition              # one container, one strategy
./grades test vector list copy erase       # several containers and strategies
./grades test list Students.txt            # on your own file(s)
./grades test vector erase --full          # do not skip the very slow erase loop
```

Containers: `vector`, `list`, `deque`. Strategies: `copy`, `erase`,
`copy-alg`, `partition` (see [v1.0](#strategies-v10)). For `data.txt` the
results are written to `data_passed.txt` and `data_failed.txt`. At the end
the program prints a table of the times and the peak memory used.

### 5. Or use the menu

Running `./grades` without arguments shows a menu:

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

**1 - type the results in.** Type one student per line: name, surname, the
homework results and finally the exam result. The number of homework results
can differ between students - the last number on the line is always the
exam. Results must be whole numbers from 0 to 10.

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

**2 - generate the results randomly.** First enter how many homework results
each student should get (this is **not** the number of students), then type
only the name and surname of each student.

**3 - read the results from a file.** Enter the file name, or press Enter to
use `Students.txt`. The program prints both final grades for every student.

**4 - generate the five test data files.** Same as `./grades generate`, but
asks how many homework results each student should get.

**5 - split a file into passed/failed and measure the speed.** Enter a file
name (or press Enter for all five generated files), then choose the
container, the strategy and average or median:

```
Container to use:
  1 - std::vector
  2 - std::list
  3 - std::deque
  4 - all three, to compare them
Choice: 3

How to divide the students into passed and failed:
  1 - Strategy 1 (copy into two new containers, for loop)
  2 - Strategy 2 (move failed out and erase, for loop)
  3 - Strategy 1 with algorithms (std::remove_copy_if + std::copy_if)
  4 - Strategy 2 with algorithms (std::partition)
  5 - all of them, to compare them
Choice: 4
```

For every file the program reads the students and calculates their final
grades, divides them into **passed** (final grade >= 5.0) and **failed**
(< 5.0), sorts both groups (best grade first) and writes each group to its
own file, timing every step.

## Data file format

The first line is a header; every other line is one student. Any number of
homework columns is accepted - the last number on the line is the exam.

```
Name          Surname        HW1   HW2   HW3   HW4   HW5   Exam
Michael       Johnson          8     9    10     6    10      9
```

Large data files (`students1*.txt`, `generated*.txt`) and the
`*_passed.txt` / `*_failed.txt` results are not stored in the repository
because of their size.

## Results

Test machine for all measurements: Apple M3, 8 GB RAM, SSD, macOS 26.5,
Apple clang 21, `-O2`. Final grade by average, 5 homework results per
student. Times are in seconds, one run each.

### Speed analysis (v0.2)

`std::vector`, sort first, then copy into "passed" and "failed":

| Students   | Read   | Sort  | Split | Write passed | Write failed | Total  |
|-----------:|-------:|------:|------:|-------------:|-------------:|-------:|
| 1 000      | 0.001  | 0.000 | 0.000 | 0.001        | 0.000        | 0.002  |
| 10 000     | 0.012  | 0.001 | 0.000 | 0.005        | 0.004        | 0.022  |
| 100 000    | 0.132  | 0.016 | 0.005 | 0.031        | 0.021        | 0.208  |
| 1 000 000  | 1.201  | 0.200 | 0.076 | 0.365        | 0.249        | 2.128  |
| 10 000 000 | 10.592 | 2.633 | 1.958 | 3.492        | 2.617        | 22.131 |

- the time grows in step with the number of students;
- reading the file is the slowest step - about half of the total time -
  because every line has to be turned from text into numbers;
- sorting is fast because students are moved, not copied.

### Containers (v0.25)

Total time with the three containers:

| Students   | std::vector | std::list | std::deque |
|-----------:|------------:|----------:|-----------:|
| 1 000      | 0.005       | 0.001     | 0.001      |
| 10 000     | 0.020       | 0.014     | 0.013      |
| 100 000    | 0.190       | 0.172     | 0.157      |
| 1 000 000  | 2.298       | 2.320     | 1.855      |
| 10 000 000 | 24.398      | 38.869    | 24.055     |

- reading and writing take about the same time with every container;
- at 10 000 000 students `std::list` is clearly the slowest: sorting and
  splitting jump around the memory, one node per student;
- `std::vector` and `std::deque` are practically equal.

### Strategies (v1.0)

Two ways of dividing the students were compared, each written twice:

| Name        | Strategy | How it works |
|-------------|----------|--------------|
| `copy`      | 1        | `for` loop, every student is copied into a new `passed` or `failed` container; the base container keeps everyone |
| `erase`     | 2        | `for` loop, a failed student is moved into `failed` and `erase()`d from the base container, which then holds only the passed ones; the base is shrunk with `shrink_to_fit()` |
| `copy-alg`  | 1        | `std::remove_copy_if` (passed) and `std::copy_if` (failed) |
| `partition` | 2        | `std::partition` moves the passed students to the front in one pass, the failed tail is moved into `failed` and cut off with a single `erase()` (`splice()` for `std::list`); then `shrink_to_fit()` |

In v1.0 the students are divided **before** sorting. On a sorted container
the failed students would already sit together at the end, which hides the
cost of erasing them. All twelve combinations produce exactly the same
`_passed` / `_failed` files.

**Time of the split step only:**

| Strategy    | vector 1M | list 1M | deque 1M | vector 10M | list 10M | deque 10M |
|-------------|----------:|--------:|---------:|-----------:|---------:|----------:|
| `copy`      | 0.066     | 0.037   | 0.037    | 1.244      | 0.404    | 0.409     |
| `erase`     | *~12 min* | 0.016   | *~4 min* | *~20 h*    | 0.255    | *~7 h*    |
| `copy-alg`  | 0.039     | 0.038   | 0.039    | 1.295      | 1.057    | 0.453     |
| `partition` | 0.011     | 0.007   | 0.009    | 0.420      | 0.095    | 0.178     |

The `erase` loop on `std::vector` and `std::deque` was measured only up to
100 000 students; bigger files are skipped by default (`--full` forces
them). Its time grows with the **square** of the number of students:

| Students | vector `erase` | deque `erase` |
|---------:|---------------:|--------------:|
| 10 000   | 0.075          | 0.026         |
| 100 000  | 7.163          | 2.591         |

Ten times more students take about 95 (vector) and 100 (deque) times
longer, because every `erase()` shifts all the students behind it. The
values in *italics* above are extrapolated from this.

**Total time (read + split + sort + write):**

| Strategy    | vector 1M | list 1M | deque 1M | vector 10M | list 10M | deque 10M |
|-------------|----------:|--------:|---------:|-----------:|---------:|----------:|
| `copy`      | 1.539     | 1.320   | 1.056    | 16.800     | 21.222   | 14.769    |
| `erase`     | -         | 1.195   | -        | -          | 20.851   | -         |
| `copy-alg`  | 1.053     | 1.177   | 1.075    | 16.348     | 21.020   | 15.398    |
| `partition` | 1.066     | 1.184   | 1.071    | 14.882     | 19.438   | 14.302    |

(The first row, vector `copy` at 1M, includes a slower first read of the
file; its split and sort times are in line with the others.)

**Peak memory, 10 000 000 students** (each combination run as a separate
process):

| Strategy    | std::vector | std::list | std::deque |
|-------------|------------:|----------:|-----------:|
| `copy`      | 1 513 MB    | 2 338 MB  | 1 908 MB   |
| `erase`     | -           | 1 383 MB  | -          |
| `copy-alg`  | 1 754 MB    | 1 846 MB  | 1 716 MB   |
| `partition` | 1 646 MB    | 1 383 MB  | 1 476 MB   |

At 100 000 students: vector `copy` 35.3 MB vs `erase` 29.0 MB, deque
`copy` 24.8 MB vs `erase` 13.3 MB.

### What the v1.0 results show

- **Is Strategy 1 really wasteful with memory?** Yes for `std::list`
  (-41 % with Strategy 2) and `std::deque` (-23 %), where every copied
  student is a new allocation on top of the base container. For
  `std::vector` the difference is small: its peak is set while *reading*,
  when the vector doubles its capacity and the old and new buffers exist at
  the same time; `shrink_to_fit()` in Strategy 2 also makes one more copy of
  the buffer. The machine has only 8 GB RAM and macOS compresses memory, so
  the big numbers are approximate.
- **Strategy 2 with `erase()` in a loop is "painful"** for `std::vector`
  and `std::deque`: quadratic, hours instead of seconds for 10 000 000
  students. For `std::list` it is the fastest loop version, because erasing
  a list node does not move anything else.
- **Algorithms:** `std::remove_copy_if` + `std::copy_if` are as fast as the
  hand-written loop (they make two passes instead of one, so they are not
  faster). `std::partition` is the real improvement: it does Strategy 2 in
  one pass with no per-student `erase()`, so it is the fastest split for
  every container and removes the quadratic cost completely.
- **The split itself is small** compared to reading (~10 s for 10M) and
  sorting, so the total time changes much less than the split time.
- **Fastest combination: `std::deque` with Strategy 2 + `std::partition`**
  (14.3 s for 10 000 000 students, 13.9 s in a repeated run) - it is also
  among the lowest in memory. `std::vector` + `partition` is a close second;
  `std::list` loses because its sort is about 2.5 times slower.

## Exception handling (v0.2)

Errors are reported with exceptions (`Errors.h`) and caught in `main()`:

- `FileError` - a file cannot be opened, created or written;
- `DataError` - a file opens but contains no students;
- `std::bad_alloc` - not enough memory for that many students;
- `std::invalid_argument` / `std::out_of_range` - a menu answer that is not
  a number or is outside the allowed range (the question is asked again).

In the speed test a file that cannot be processed is reported and the test
continues with the next file.

## Project structure

- `Person.h`, `Person.cpp` - class `Person`: student data, rule of three,
  overloaded `>>` and `<<`, final grade calculation;
- `Students.h`, `Students.cpp` - sorting and the four ways of dividing the
  students (templates: any of the three containers);
- `FileIO.h`, `FileIO.cpp` - reading, writing and generating data files;
- `Console.h`, `Console.cpp` - keyboard input and tables on the screen;
- `SpeedTest.h`, `SpeedTest.cpp` - the speed and memory analysis;
- `Timer.h` - class `Timer` that measures elapsed time;
- `Errors.h` - exception classes `FileError` and `DataError`;
- `main.cpp` - the menu, command line and exception handling;
- `Makefile` (Unix) and `CMakeLists.txt` (any OS) - build files;
- `Students.txt` - sample data file.

## Releases

- **v0.1** - `Person` class, manual / random / file input, average or median.
- **v0.2** - code divided into several `.h` / `.cpp` files, exception
  handling, test file generation, passed / failed split, speed analysis.
- **v0.25** - the same code working with `std::vector`, `std::list` and
  `std::deque`, speed comparison of the three containers.
- **v1.0** - two splitting strategies (copy into two containers / move out
  and erase), rewritten with `<algorithm>` (`std::remove_copy_if`,
  `std::copy_if`, `std::partition`, `std::find_if`, `std::copy`,
  `std::transform`), speed and memory comparison, CMake build, comments
  cleaned up.

# Student Final Grade Calculator

A C++ console application that collects students' homework and exam results
and calculates each student's final grade.

Planned functionality:

- enter student data by hand (name, surname, homework results, exam result);
- generate homework and exam results randomly;
- read student data from a text file;
- print a formatted, sorted table with the final grades.

## Final grade formula

```
Final points = 0.4 * (Average of HW | Median of HW) + 0.6 * Exam
```

The homework part is either the **average** or the **median** of all homework
results, depending on the user's choice.

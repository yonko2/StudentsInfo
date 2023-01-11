# StudentsInfo

A console application storing and manipulating information about students, written in C++.

## Available options
1) Insert student in a group
2) Remove student from a group
3) Print all students from a group
4) Sort student group(s)
5) Exit

## Note on data files
There are eight csv files in the `csv/` directory. In this repo, each .csv file has two examples.
Data is stored in the following format:

In the .csv
`Name,FN,Courses`
`Ivan Ivanov Ivanov,0MI0000000,Algebra/5.50;DIS/5.20`

representing
```markdown
| Name               | FN			  | Courses               |
|--------------------|----------------|-----------------------|
| Ivan Ivanov Ivanov | 0MI0000000     | Algebra/5.50;DIS/5.20 |
```
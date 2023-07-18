# Cache_Performance_Simulator
# Computer Organization and Assembly Language Programming: Project 2: Cache Performance Simulator

## Authors:
- Kirolous Fouty (900212444)
- Omar Ibrahim (900192095)
- Raafat El Saeed (900191080)
- Yehia Ragab (900204888)
- Ziyad Amin (900201190)

### Program Guide (How to use)
```
0- In case you wish to edit the source code, after applying your changes, compile "main.cpp" into "main.exe" using a C++ compiler such as g++.

1- Run the program through the cmd line that is open in the same folder of the program
   and typing "main.exe",
   or by simply double-clicking the "main.exe" file from File Explorer.

2- The program will start simulating the cache by running six generators for every case of the 9 cases below:
   Line_size=16 && Number_of_ways=4
   Line_size=32 && Number_of_ways=4
   Line_size=64 && Number_of_ways=4
   Line_size=128 && Number_of_ways=4
   Number_of_ways=1 && Line_size=32
   Number_of_ways=2 && Line_size=32
   Number_of_ways=4 && Line_size=32
   Number_of_ways=8 && Line_size=32
   Number_of_ways=16 && Line_size=32

```

## Source Files Description
- main.cpp: the main C++ file where the source code of the simulator exists.
- main.exe: the executable file of "main.cpp" after compilation.
- results.csv: a comma separated values file that contains values that are either labels like "memGen1()" or hit rate value like "66".
- Graphs.xlsx: an excel file that contains the graphs produced using the given values of "results.csv".
- Report.pdf: a pdf file that contains the team's report of the project with thorough analysis and explanations.
- .gitignore: used to ignore other files related to debugging such as "output.txt".
- README.md: a readme file that contains a short description of the project.
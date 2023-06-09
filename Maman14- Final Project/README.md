# C Assembler Project
This is an Assembler project for unique 32 base system.
This project is a major assignment during my studies at the University, the assignment is the main factor for the course "C programming".

The project contains:
- Pre-assembler: Identification, detection and replacement of macros from a source file.
                  The Pre-assembler writes a new file which will be proccessed by the first-run later on.
                  
- First run: This part iterates through a provided file from the Pre-assembler and provides with error detection throughout the file, according to the project's requirements - each error is printed to the end-user with a line indicator (from the provided file),
              The First-run writes a new file which will be proccessed by the second-run later on.

- Second run: This part iterates through a provided file from the First run and provides with error detection of a specific printing issues which may still occur after the first-run, this part is in charge of printing each row of the original file (assuming we've reached here, the original file is error-proof, since the first run should've handled any issues), in a format of: "LineNumber    DataOfLineNum" in a unique 32 base.
              The Second-run writes a new file which will contain all of the original file lines with LineNumber and DataOfLineNum format.

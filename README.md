# Macro_Assembler

A C++ implementation of the Macro-11 Assembler by Digital Equipment Corporation ([Macro-11 Reference](http://bitsavers.org/pdf/dec/pdp11/rsx11/RSX11M_V3.2_Jun79/3A_ProgramDevelopment/DEC-11-OIMRA-B-D_MACRO-11_Reference_Dec76.pdf))

## Assembler Construction

two pass assembler

### Pass 1
	-Locate and read all required macros from libraries
	-Build symbol tables and program section tables
	-Rudimentary assembling of each source statement

### Pass 2

	-Generate machine code instructions
	-Use symbol table to resolve reference (in following order)
		1) Macro symbol table
		2) Permanent symbol table
		3) User-defined symbol table
	-Flag errors
	-Generate assembly listing


See [Documents](https://github.com/13mtfb/Macro_Assembler/tree/parser/documents) for information on Assembler Syntax and scanner/parser implementation.



## Building

 1. Clone the repository and open with vscode 
 2. In vscode:
   - open `Macro_Assembler.cpp`
   - Click `Terminal` -> `Run Build Task...`
   - An .exe will be created in the directory which can be invoked via the command line

## Testing

 1. `./test_runner.sh`
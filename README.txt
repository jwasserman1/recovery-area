Welcome to the Recovery Area Statistical Analysis Tool!

FUNCTIONALITY: 				This tool takes in a text file containing data on patient use of the recovery area and outputs a statistical analysis of that data.

INSTRUCTIONS FOR USE:		

(1) Formatting Input

	In order to use this tool, you must have a properly formatted text file. The best way to accomplish this is by starting in Microsoft Excel. Create a spreadsheet as follows:
	
	In cell A1, you should have a single integer with no decimal places that gives the number of patient interactions in the data set.
	
	The data set will begin in row 2:
	(A)	In column A beginning on row 2, enter the date of the patient interaction in the format MM/DD/YYYY.
	(B) In column B beginning on row 2, enter an integer representation of the final service time of the interaction, created as follows: 
		Let x be the Service Time formatted as HH:MM:SS. Integer representation = HOUR(x)*60+MINUTE(x)
		Be sure that the cell is formatted to a number with 0 decimal places.
	(C) In column C beginning on row 2, enter the number of minutes that the patient spent in the recovery room.

	Press File->Export->Change File Type and select "Text (tab delimited) (*.txt)" as the file type. Save the .txt file in the same folder as the program recovery_data.exe.

	I've included example text files in the folder as well as their respective Excel sheets, so feel free to look at them to better understand how your text file should be formatted.

(2) Running the Tool

	Double-click on the file recovery_data.exe. This should open a terminal.
	Type the name of the file you wish to analyze, i.e. "data.txt" (don't include the quotation marks).

(3) Reading the Output

	The output of the statistical analysis will be written to another text file in the same folder as the input file. The file name will be the same as the input, with "_output" appended onto the end.
	So, if the input was called "data.txt" then the output file would be called "data_output.txt". 
	
	Please note that all analyzed values are based on a 10-hour workday from 7am to 5pm. 


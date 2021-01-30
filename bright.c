/*
	bright, A simple program to adjust brightness on linux computers using
	intel_backlight.
	Copyright (C) 2021 Jonas Jørgensen 

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

//Function prototypes

int readwrite (char *path, int write);
int calculate (int cur, char arg);
void usage (void);

//Global variables

char *setp = "/sys/class/backlight/intel_backlight/brightness";
char *maxp = "/sys/class/backlight/intel_backlight/max_brightness";
int max;
float min = 5;
float inc = 5;
int arg = -1;

int main (int argc, char *argv[])
{

// getopt - handling the arguments.
	int c;
	int i = 0;
	int dflag = 0;
	int iflag = 0;
	int vflag = 0;
	int errflag = 0;
	int value = 0;

	opterr = 0;

	while ((c = getopt (argc, argv, ":div:h")) != -1)
		switch (c)
		{
			case 'd':
				if (iflag > 0)
					errflag++;
				else
					dflag++;
				break;
			case 'i':
				if (dflag > 0)
					errflag++;
				else
					iflag++;
				break;
			case 'v':
				vflag++;
				value = atoi(optarg);
				break;
			case 'h':
				usage();
				return 0;
			case ':':
				fprintf (stderr, "Option -%c requires a value\n", optopt);
				break;
			case '?':
				fprintf (stderr, "Unreqognized option -%c. Try 'bright -h' for help\n", optopt);
				return 1;
		}

	for (i = optind; i < argc; i++)
	{
		printf ("Non-option argument %s\n", argv[i]);
		usage();
		return 1;
	}

	if (errflag)
	{
		printf ("-i and -d can not be used at the same time\n");
		return 1;
	}

	if (vflag)
	{
		if (value > 0 && value < 50)
		{
			inc = value;
		}
		else
		{
			printf ("Option -v requires an integer value between 0 and 50\n");
			return 1;
		}
	}

	if (dflag)
	{
		arg = 0;
		readwrite (setp, 1);
		printf ("brightness: %d.\n", readwrite (setp, 0)); 
		return 0;
	}
	else if (iflag)
	{
		arg = 1;
		readwrite (setp, 1);
		printf ("brightness: %d.\n", readwrite (setp, 0)); 
		return 0;
	}
	else
	{
		printf ("brightness: %d.\n", readwrite (setp, 0)); 
		return 0;
	}

}

int readwrite (char *path, int write)
{
	int var;
	char *mode;

	if (write == 0)
		mode = "r";
	else if (write == 1)
		mode = "r+";

	//Open file
	FILE *f = fopen (path, mode);
	if (f == NULL)
	{
		fprintf (stderr, "Could not open %s.\n", path);
		return 2;
	}
	//Read out value
	fscanf (f, "%d", &var);

	//Write new value
	if (write == 1)
		fprintf (f, "%d", calculate (var, arg));

	fclose (f);
	return var;
}

int calculate (int cur, char arg)
{
	int set = 0;
	int max = readwrite (maxp, 0);
	const int INC = round (max * (inc / 100));
	const int MIN = round (max * (min / 100));

	if (arg == -1)
	{
		printf("arg = -1");
		return 1;
	}

	// Calculate new brightness value:
	if (arg == 1)
	{
		if (cur < max - INC)
			set = cur + INC;
		else
			set = max;
	}	
	else if (arg == 0)
	{
		if (cur > MIN + INC)
			set = cur - INC;
		else
			set = MIN;
	}
	return set;
}

void usage (void)
{
	printf ("Usage: bright [-i | -d] [-h] [-v <value>]  \n\
options:\n\
  -i\tincrease backlight\n\
  -d\tdecrease backlight\n\
  -v\tgives the adjustment step a new value in percent (0-50)\n\
  -h\tshow this message\n\n\
example: bright -iv 20 - increase brightness by a value of 20 percent.\n\
if -v is not passed, default increment is 5 percent.\n\n");
}

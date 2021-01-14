#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

//Function prototypes

int readwrite (char *path, int write);
int calculate (int cur, char arg);

//Global variables

char *setp = "/sys/class/backlight/intel_backlight/brightness";
char *maxp = "/sys/class/backlight/intel_backlight/max_brightness";
int max;
int min = 5;
float inc = 5;
int arg = -1;

int main (int argc, char *argv[])
{

// getopt - handling the arguments.
	int c;
	opterr = 0;

	while ((c = getopt (argc, argv, "di")) != -1)
		switch (c)
		{
			case 'd':
				arg = 0;
				readwrite (setp, 1);
				return 0;
			case 'i':
				arg = 1;
				readwrite (setp, 1);
				return 0;
			default:
				printf ("Usage: backlight [-i] [-d]\n\
options:\n\
\t-i - increase backlight\n\
\t-d - decrease backlight\n");
				return 1;
		}

printf ("brightness: %d.\n", readwrite (setp, 0)); 
return 0;

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

#include <stdio.h>
#include <string.h>

int main( int argc, char *argv[])
{
	char *setp = "/sys/class/backlight/intel_backlight/brightness";
	char *maxp = "/sys/class/backlight/intel_backlight/max_brightness";
	int set;
	int max;
	const int MIN = 250;
	const int INC = 250;

	//Check correct input
	if( argc != 2)
	{
		printf("Usage: backlight [options]\noptions:\n\ti - increase backlight\n\td - decrease backlight\n");
		return 1;
	}

	//Open max_brightness and read out values
	FILE *maxf = fopen( maxp, "r");
	if ( maxf == NULL)
	{
		fprintf( stderr, "Could not open %s.\n", maxp);
		return 2;
	}
	fscanf( maxf, "%d", &max);
	fclose( maxf);

	// Open brightness:
	FILE *setf = fopen( setp, "r+");
	if ( setf == NULL)
	{
		fprintf( stderr, "Could not open %s.\n", setp);
		return 4;
	}
	// Read contens:
	fscanf( setf, "%d", &set);

	// Calculate new brightness value:
	if( strcmp( argv[1], "i") == 0)
	{
		if( set < max - INC )
			set+= INC;
		else
			set = max;
	}	
	else if( strcmp( argv[1], "d") == 0)
	{
		if( set > MIN + INC )
			set-= INC;
		else
			set = MIN;
	}
	fprintf( setf, "%d", set);
	fclose( setf);	

	return 0;
}

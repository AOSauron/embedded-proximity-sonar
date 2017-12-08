#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
	FILE *display, *sonar;
    char value[8];

    // Check args
    if (argc < 3) {
        perror("You must specify a duration for measuring and an interval for each measure(in seconds)\nUsage : ./SonarDisplay [DURATION] [INTERVAL]");
        return -1;
    }

    // Get the duration
    int meantime = strtol(argv[1], NULL, 10);
    int interval = strtol(argv[2], NULL, 10);

    printf("Duration : %d seconds\n", meantime);
    printf("Interval : %d second(s)\n", interval);

    if (!(display = fopen("/dev/display", "wb"))) {
        return -1;
    }

    if (!(sonar = fopen("/dev/sonar", "rb"))) {
        return -1;
    }

    printf("Measuring ... Look at the display\n");

    while (meantime > 0) {
        fread(&value, 8*sizeof(char), 1, sonar);
        fseek(sonar, 0, SEEK_SET);

        // printf("%ull\n", value);

        fwrite(&value, 8*sizeof(char), 1, display);
        fseek(display, 0, SEEK_SET);

        sleep(interval);
        meantime -= interval;
        printf("%d seconds remaining ...\n", meantime);
    }

    fclose(sonar);
    fclose(display);
	
    printf("Ended.\n");

	return 0;
}


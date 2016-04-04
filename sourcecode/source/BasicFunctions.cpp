/* 
 * File:   BasicFunctions.cpp
 * Author: Pieter Loubser
 * 
 * Created on December 30, 2015, 5:54 PM
 */

//Libraries
#include "../include/BasicFunctions.h"

static const char s_directions_str[] = "in\0out";
static const char s_values_str[] = "01";

/*
 * piBoardRev:
 *	Return a number representing the hardware revision of the board.
 *	Revision is currently 1 or 2. -1 is returned on error.
 *
 *	Much confusion here )-:
 *	Seems there are some boards with 0000 in them (mistake in manufacture)
 *	and some board with 0005 in them (another mistake in manufacture?)
 *	So the distinction between boards that I can see is:
 *	0000 - Error
 *	0001 - Not used
 *	0002 - Rev 1
 *	0003 - Rev 1
 *	0004 - Rev 2 (Early reports?
 *	0005 - Rev 2 (but error?)
 *	0006 - Rev 2
 *	0008 - Rev 2 - Model A
 *	000e - Rev 2 + 512MB
 *	000f - Rev 2 + 512MB
 *
 *	A small thorn is the olde style overvolting - that will add in
 *		1000000
 *
 *********************************************************************************
 */

static void piBoardRevOops(string why) {
	fprintf(stderr,
			"piBoardRev: Unable to determine board revision from /proc/cpuinfo\n");
	fprintf(stderr, " -> %s\n", why.c_str());
	fprintf(stderr, " ->  You may want to check:\n");
	fprintf(stderr,
			" ->  http://www.raspberrypi.org/phpBB3/viewtopic.php?p=184410#p184410\n");
	exit(EXIT_FAILURE);
}

/**
 * piBoardRev:
 *	Return a number representing the hardware revision of the board.
 *	Revision is currently 1 or 2. -1 is returned on error.
 *
 *	Much confusion here )-:
 *	Seems there are some boards with 0000 in them (mistake in manufacture)
 *	and some board with 0005 in them (another mistake in manufacture?)
 *	So the distinction between boards that I can see is:
 *	0000 - Error
 *	0001 - Not used
 *	0002 - Rev 1
 *	0003 - Rev 1
 *	0004 - Rev 2 (Early reports?
 *	0005 - Rev 2 (but error?)
 *	0006 - Rev 2
 *	0008 - Rev 2 - Model A
 *	000e - Rev 2 + 512MB
 *	000f - Rev 2 + 512MB
 *
 *	A small thorn is the olde style overvolting - that will add in
 *		1000000
 *
 *********************************************************************************
 */
int piBoardVersion(void) {
	FILE *cpuFd;
	char line[120];
	char *c, lastChar;
	static int boardRev = -1;

	if (boardRev != -1) // No point checking twice
		return boardRev;

	if ((cpuFd = fopen("/proc/cpuinfo", "r")) == NULL)
		piBoardRevOops(string("Unable to open /proc/cpuinfo"));

	while (fgets(line, 120, cpuFd) != NULL)
		if (strncmp(line, "Revision", 8) == 0)
			break;

	fclose(cpuFd);

	if (strncmp(line, "Revision", 8) != 0)
		piBoardRevOops(string("No \"Revision\" line"));

	for (c = &line[strlen(line) - 1]; (*c == '\n') || (*c == '\r'); --c)
		*c = 0;

#if (ENABLE_DEBUG_OUTPUT == 1)
	printf("piboardRev: Revision string: %s\n", line);
#endif

	for (c = line; *c; ++c)
		if (isdigit(*c))
			break;

	if (!isdigit(*c))
		piBoardRevOops(string("No numeric revision string"));

	// If you have overvolted the Pi, then it appears that the revision
	//	has 100000 added to it!

#if (ENABLE_DEBUG_OUTPUT == 1)
	if (strlen(c) != 4)
	printf("piboardRev: This Pi has/is overvolted!\n");
#endif

	lastChar = line[strlen(line) - 1];

#if (ENABLE_DEBUG_OUTPUT == 1)
	printf("piboardRev: lastChar is: '%c' (%d, 0x%02X)\n", lastChar, lastChar, lastChar);
#endif

	/**/if ((lastChar == '2') || (lastChar == '3'))
		boardRev = 1;
	else
		boardRev = 2;

#if (ENABLE_DEBUG_OUTPUT == 1)
	printf("piBoardRev: Returning revision: %d\n", boardRev);
#endif

	return boardRev;
}

int GPIOExport(int pin) {
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return (-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	usleep(50000);
	return (0);
}

int GPIOUnexport(int pin) {
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return (-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return (0);
}

int GPIODirection(int pin, int dir) {

#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return (-1);
	}

	if (-1 == write(fd, &s_directions_str[gpio::DIRECTION::INPUT == dir ? 0 : 3],
			gpio::DIRECTION::INPUT == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return (-1);
	}

	close(fd);
	return (0);
}

int GPIORead(int pin) {
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return (-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return (-1);
	}

	close(fd);

	return (atoi(value_str));
}

int GPIOWrite(int pin, int value) {

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return (-1);
	}

	if (1 != write(fd, &s_values_str[gpio::VALUE::LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return (-1);
	}
	close(fd);
	return (0);
}

void pinModeGPIO(int pin, int dir) {
	if (GPIOExport(pin) == -1) {
		return;
	}
	GPIODirection(pin, dir);
}

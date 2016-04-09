#include "auxillary.h"
using namespace std;

void swapFlags(char *argv[]) {
	char* tempStr;
	tempStr = argv[1];
	argv[1] = argv[3];
	argv[3] = tempStr;
	tempStr = argv[2];
	argv[2] = argv[4];
	argv[4] = tempStr;
	return;
}

int isValid_ComandLine(int argc, char *argv[]) {
	if (argc == 1) {
		return 0;
	} else if (argc == 3) {
		if (strcmp(argv[1], "-config") == 0 || strcmp(argv[1], "-house_path") == 0) {
			if (strcmp(argv[2], "-config") == 0 || strcmp(argv[2], "-house_path") == 0) {
				cout << "Usage(optional):[-config filename1] [-house_path filename2]\n" << endl;
				return -1;
			}
			return (strcmp(argv[1], "-config") == 0) ? 1:2;
		} else {
			cout<< "Usage(optional):[-config filename1] [-house_path filename2]\n" << endl;
			return -1;
		}
	} else if (argc == 5) {
		if ((strcmp(argv[1], "-house_path") == 0) && (strcmp(argv[3], "-config") == 0)) {
			swapFlags(argv); //change (-o file1 -v file2) to -----> (-v file2 -o file1)
		}
		if ((strcmp(argv[1], "-config") == 0) && (strcmp(argv[3], "-house_path") == 0)) {
			if (strcmp(argv[2], argv[4]) == 0) {
				cout<< "Files must be different\n"<< endl;
				return -1;
			} else if (strcmp(argv[2], "-config") == 0 || strcmp(argv[2], "-house_path") == 0
					|| strcmp(argv[4], "-config") == 0
					|| strcmp(argv[4], "-house_path") == 0) {
				cout<< "Usage(optional):[-config filename1] [-house_path filename2]\n"<< endl;
				return -1;
			}

			return 3;
		}
	} else {
		cout<< "Usage(optional):[-config filename1] [-house_path filename2]\n"<< endl;
		return -1;
	}
	return -1; //default return - shouldn't get here
}

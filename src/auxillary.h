/*
 * auxillary.h
 *
 *  Created on: 20 Mar 2016
 *      Author: Amit
 */

#ifndef AUXILLARY_H_
#define AUXILLARY_H_

#include <cstring>
#include <iostream>

//-----aux function for isValid_ComandLine - swap flags to the wanted order----//


/*swap flags (-config <> -house_config <>) to ( -house_config <> -config <>)
 * @param
 * argv - array of user input
 */
void swapFlagss(char *argv[]);

/*checks command-line arguments and flags validity. returns a number which encodes the arguments status
 * @param
 * argc,argv - user input and input size
 * return code:
 * -1 : not valid
 * 0 : no arguments
 * 1 : only config
 * 2 : only house_path
 * 3 : both
 */
int isValid_ComandLine(int argc, char *argv[]);


#endif /* AUXILLARY_H_ */

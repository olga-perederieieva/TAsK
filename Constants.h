#ifndef CONSTANTS_
#define CONSTANTS_

/** This file contains used constants.
*/
enum PathAlgoType {Nothing, PE, GP, PG, ISP}; /**< path-based algorithms that are implemented. */
enum PathApp {AppNothing, APP1, APP2, APP3}; /**< existing approaches, are applicable only in the case 
												of path-based algorithms.*/

enum OriginAlgoType {B, Bstep, LUCE, TAPAS}; /**< origin-based algorithms that are implemented. */
enum ODSetType {PEAPP3, WITH_STEP, GPAPP3 }; /**< possible types of ODSet. */

enum FncType {CONSTFNC, LINEARFNC, BPRFNC, 
	SPIESSFNC, SPIESSCOMBFNC, CONSTLINEARFNC, 
	SPIESSLINEARFNC, SPIESSCOMBLINEARFNC, FD12, FD22, ERROR}; /**< Existing types of link
																cost functions.*/

#endif

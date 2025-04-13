/**
 * @file first.cpp
 * @brief basic use of newcols and addrow
 * to solve the model
 *  max 2 x1 + 3 x2 + w
 *    x1 + x2 <= 5 z ---> 		x1 + x2 - 5 z <= 0									
 *    x2 + 9 y1 + 9 y2 + 8w = 2
 *    8 y1 >= -1									
 *    -4 y1 + 7z + 5w <= 9				
 *    x1,x2 >=0
 *    y1 <=0
 *    z in {0,1}
 *    w in Z+					
 */

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "cpxmacro.h"

using namespace std;


// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

int main (int argc, char const *argv[])
{
	try
	{
	///////////////////////// init
	DECL_ENV( env );
	DECL_PROB( env, lp );
		
	///////////////////////// create variables with newcols
	//
	//    status =      CPXnewcols (env, lp, ccnt, obj, lb, ub, xctype, colname);
	//
	// all variables will be created in an array and each variable will be identified by the related index
	//  => we assume that variables are SORTED as ** x1, x2, y1, y2, z, w **
	int ccnt = 6;
	double objCost[6] = {	2.0, 3.0, 0.0, 0.0, 0.0, 1.0 };
	  // cost in the objective function;
    double lb[6]      = {	0.0       , 0.0         , -CPX_INFBOUND,   -CPX_INFBOUND, 0.0, 0.0          };
    double ub[6]      = { CPX_INFBOUND, CPX_INFBOUND,	0.0          , CPX_INFBOUND , 1.0, CPX_INFBOUND };
      // variable lower and upper bounds
    char xtype[6]     = { 'C' , 'C' , 'C' , 'C' , 'B' , 'I' };
      // variable types 'C' or 'B' or 'I' ...
    char ** xname = NULL; 
      // no names
    CHECKED_CPX_CALL( CPXnewcols, env, lp, ccnt, &objCost[0], &lb[0], &ub[0], &xtype[0], xname );
    
    ///////////////////////// create constraints
    //
    //    status = CPXaddrows (env, lp, colcnt, rowcnt, nzcnt, rhs, sense, rmatbeg, rmatind, rmatval , newcolname, newrowname);
    //
    int colcount = 0; 
      // no new columns
    int rowcount = 4; 
      // number of constraints
    int nzcnt = 11;   
      // number of NON-ZERO coefficients
    double rhs[4] = {0,2,-1,9}; 
      // right-hand-sides: notice that constraints are rewritten so that NO variable appears in the RHS!!!
    char sense[4] = {'L','E','G','L'};
      // constraint type 'L' or 'E' or 'G' ... 
    // the coefficient matrix will be linearized in a vector and ONLY NON-ZERO coefficients
    //  need to be stored: the following three vectors are used
    double rmatval[11] = { 1.0, 1.0, -5.0,   1.0, 9.0, 9.0, 8.0,     8.0,      -4.0, 7.0, 5.0 };
      // the linearized vector of non-zero coefficients
    int rmatbeg[4]	= {0,3,7,8};
      // one element for each constraint (row), reporting the index where each row of the coefficient matrix starts
    int rmatind[11]    = {0,1,4, 1,2,3,5, 2, 2,4,5};
      // one element for each zon-zero coefficient, reporting its column index
		char ** newcolnames = NULL;
		char ** rownames = NULL;
		  // no names
		CHECKED_CPX_CALL( CPXaddrows, env, lp, colcount, rowcount, nzcnt, &rhs[0], &sense[0], &rmatbeg[0], &rmatind[0], &rmatval[0], newcolnames , rownames );
		
	CHECKED_CPX_CALL( CPXchgobjsen, env, lp, CPX_MAX );
		// change to MAXimize (default is MINimize)

    ///////////////////////// print (debug)
	CHECKED_CPX_CALL( CPXwriteprob, env, lp, "first.lp", NULL );

	///////////////////////// optimize
	CHECKED_CPX_CALL( CPXmipopt, env, lp );

    ///////////////////////// print
	double objval;
	CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
	// get the objective function value into objval
	std::cout << "Objval: " << objval << std::endl;
	int n = CPXgetnumcols(env, lp);
	// get the number of variables (columns) into n (simple routine, no need for return status);
	/////// get the value of the variables using
	//
	//    status = CPXgetx (env, lp, varVals, fromIdx, toIdx);
  	//
  	std::vector<double> varVals;
	varVals.resize(n);
	int fromIdx = 0;
	int toIdx = n - 1;
	// we prepare a vector to get n values from index 0 to index n-1
  	CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], fromIdx, toIdx );
  	  // get the value of the variables from index fromIdx to index toIdx into an array having (toIdx - fromIdx + 1) available positions
	for ( int i = 0 ; i < n ; ++i ) {
  	  std::cout << "var in position " << i << " : " << varVals[i] << std::endl;
  	  /// to get variable name, use the RATHER TRICKY "CPXgetcolname"
  	  /// status = CPXgetcolname (env, lp, cur_colname, cur_colnamestore, cur_storespace, &surplus, 0, cur_numcols-1);
  	}
	double value_of_zed_var;
	CHECKED_CPX_CALL( CPXgetx, env, lp, &value_of_zed_var, 4, 4 );
  	
	CHECKED_CPX_CALL( CPXsolwrite, env, lp, "first.sol" );
	// write the solution to a text file
		
	// free
	CPXfreeprob(env, &lp);
	CPXcloseCPLEX(&env);
	}
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
	return 0;
}

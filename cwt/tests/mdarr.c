/**
 * @file   mdarr.c
 * @author wladt
 * @date   Oct 11, 2012 11:32:00 PM
 *
 * @brief Multidimensional arrays usage
 */

#include <stdio.h>
#include <string.h>
#include <cwt/types.h>
#include <cwt/test.h>

static int two_dim_arr[][3] = {
	  { 9, 1, 2 }
	, { 3, 4, 5 }
	, { 6, 7, 8 }
	, { 9, 0, 1 }
};


int main(int argc, char *argv[])
{
	int *arr;
	int ncols;
	int nrows;
	int i, j;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(39);

	ncols = CWT_A2_NCOLS(two_dim_arr);
	nrows = CWT_A2_NROWS(two_dim_arr);

	arr = &two_dim_arr[0][0];
	CWT_TEST_OK(ncols == 3);
	CWT_TEST_OK(nrows == 4);

	CWT_TEST_OK(*arr == 9);
	CWT_TEST_OK(arr[0] == 9);
	CWT_TEST_OK(arr[1] == 1);
	CWT_TEST_OK(arr[2] == 2);
	CWT_TEST_OK(arr[3] == 3);
	CWT_TEST_OK(arr[4] == 4);
	CWT_TEST_OK(arr[5] == 5);
	CWT_TEST_OK(arr[6] == 6);
	CWT_TEST_OK(arr[7] == 7);
	CWT_TEST_OK(arr[8] == 8);
	CWT_TEST_OK(arr[9] == 9);
	CWT_TEST_OK(arr[10] == 0);
	CWT_TEST_OK(arr[11] == 1);

	for( i = 0; i < nrows; i++ )
		for( j = 0; j < ncols; j++ )
			CWT_TEST_OK(arr[i * ncols + j] == two_dim_arr[i][j]);
/*
			CWT_TEST_OK3(arr[i * nrows + j] == two_dim_arr[i][j]
			    , _T("arr[%d * %d + %d] == two_dim_arr[%d][%d]")
			    , (i, nrows, j, i, j));
*/

	for( i = 0; i < nrows; i++ )
		for( j = 0; j < ncols; j++ )
			CWT_TEST_OK( CWT_A2_ELEM(arr, ncols, i, j) == two_dim_arr[i][j]);

	CWT_END_TESTS;
}



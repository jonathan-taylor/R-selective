#include <Rcpp.h>                // need to include the main Rcpp header file 
#include <randomized_lasso.h>    // where densities are defined

// [[Rcpp::export]]
Rcpp::NumericVector log_density_gaussian_(double noise_scale,                         // Scale of randomization
					  Rcpp::NumericMatrix internal_linear,        // A_D -- linear part for data
					  Rcpp::NumericMatrix internal_state,         // D -- data state -- matrix of shape (nopt, npts)
					  Rcpp::NumericMatrix optimization_linear,    // A_O -- linear part for optimization variables
					  Rcpp::NumericMatrix optimization_state,     // O -- optimization state -- matrix of shape (ninternal, npts)
					  Rcpp::NumericMatrix offset) {               // h -- offset in affine transform -- "p" dimensional 

  int npt = internal_state.ncol();         // Function is vectorized
  if (optimization_state.ncol() != npt) {  // Assuming each column is an internal or opt state because arrays are column major
    Rcpp::stop("Number of optimization samples should equal the number of (internally represented) data.");
  }

  int ndim = optimization_linear.nrow();  
  if (internal_linear.nrow() != ndim) {  
    Rcpp::stop("Dimension of optimization range should be the same as the dimension of the data range.");
  } 
  int ninternal = internal_linear.ncol();
  int noptimization = optimization_linear.ncol();

  Rcpp::NumericVector result(npt);

  int ipt;
  for (ipt=0; ipt<npt; ipt++) {
    result[ipt] = log_density_gaussian(noise_scale,
				       ndim,
				       ninternal,
				       noptimization,
				       (double *) internal_linear.begin(),
				       ((double *) internal_state.begin() + ipt * ninternal),
				       (double *) optimization_linear.begin(),
				       ((double *) optimization_state.begin() + ipt * noptimization),
				       (double *) offset.begin());
  }

  return(result);
}

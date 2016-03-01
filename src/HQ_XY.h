#ifndef HQ_XY_H
#define HQ_XY_H

#include <random>
#include <iostream>
#include <vector>
#include "fwd.h"
#include <cmath>
#include <boost/multi_array.hpp>

struct ele
{
	int i,j;
	double val;
};

class HQ_XY
{
	private:

	double const dx_, dy_, tau0_, x_min_, y_min_, norm;
	int const Nx_max_, Ny_max_, N_sample_;
	const fs::path filename_in_, filename_out_;

	boost::multi_array<double, 2> T_AB_;
	boost::multi_array<double, 2> XY_list_;
	std::vector<ele> PDF;
	double Int_T_AB_; //Integrated T_AB_ as weighted factor

	std::default_random_engine generator;
	
	void IC_read(void);
	void init_PDF(void);
	void iPDF(double val, int & ix, int & iy);

	public:

	HQ_XY(const VarMap& var_map);
	~HQ_XY(){};
	void HQ_XY_sample(void);
};

#endif

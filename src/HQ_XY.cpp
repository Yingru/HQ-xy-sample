#include "HQ_XY.h"

HQ_XY::HQ_XY(const VarMap& var_map)
:	filename_in_(var_map["IC_file"].as<fs::path>()),
	filename_out_(var_map["XY_file"].as<fs::path>()),
	Nx_max_(var_map["IC_Nx_max"].as<int>()),
	Ny_max_(var_map["IC_Ny_max"].as<int>()),
	dx_(var_map["IC_dx"].as<double>()),
	dy_(var_map["IC_dy"].as<double>()),
	tau0_(var_map["IC_tau0"].as<double>()),
	x_min_(-var_map["IC_Nx_max"].as<int>()/2.0*var_map["IC_dx"].as<double>()),
	y_min_(-var_map["IC_Ny_max"].as<int>()/2.0*var_map["IC_dy"].as<double>()),
	T_AB_(boost::extents[var_map["IC_Nx_max"].as<int>()][var_map["IC_Ny_max"].as<int>()]),
	XY_list_(boost::extents[var_map["N_sample"].as<int>()][2]),
	N_sample_(var_map["N_sample"].as<int>()),
	norm(1.0)
{
	std::cout << "echo grid: x = [" << x_min_ << ", " << x_min_ + dx_*Nx_max_ << "], y = [" << y_min_ << ", " << y_min_ + dy_*Ny_max_ << "]" << std::endl;
	IC_read();
	init_PDF();
}

void HQ_XY::IC_read(void)
{
	fs::ifstream filein(filename_in_);
	std::cout << "reading IC event from file --> " << filename_in_ << std::endl; 
	std::string line, entry;
	int ix = 0, iy;
    while(!(filein.eof()))
    {
        getline(filein, line);
        if(line.length() == 0 || line[0] == '#')
        {
			std::cout << line << std::endl;
            continue;
        }
		if(ix < Nx_max_)
		{
			std::istringstream sline(line);
			iy = 0;
        	do
  			{
        		sline >> entry;
				T_AB_[ix][iy] = std::stof(entry);
				iy ++;
	    	}while(sline and iy < Ny_max_);
		}
		ix++;
	}
	if (ix == Nx_max_)std::cout << "reading IC finished" << std::endl;
	else std::cout << "somethings wrong with Nx_max" << std::endl;
	
	/* check
	fs::ofstream fo("out.txt");
	for(int i=0;i<Nx_max_;i++)
	{
		for(int j=0;j<Ny_max_;j++)
		{
			fo << T_AB_[i][j] << " ";
		}
		fo << std::endl;
	}
	*/
}

void HQ_XY::init_PDF(void)
{
	//always clear PDF first
	PDF.clear();
	Int_T_AB_ = 0.0;
	for(int i=0;i<Nx_max_;i++)
	{
		for(int j=0;j<Ny_max_;j++)
		{
			if(T_AB_[i][j] > 0.0)
			{	
				Int_T_AB_ += T_AB_[i][j];
				ele e1;	
				e1.val = Int_T_AB_;
				e1.i = i;
				e1.j = j;
				PDF.push_back(e1);
			}
		}
	}
	//normalize
	for(int i=0;i<PDF.size();i++)
	{
		PDF[i].val /= Int_T_AB_;
	}
	//recover unit of Int_T_AB_
	Int_T_AB_ *= dx_*dy_;
}

void HQ_XY::iPDF(double val, int & ix, int & iy)
{
	std::uniform_real_distribution<double> unit_distribution(0.0, 1.0);
	int first, last, it;
	if (val<0.0) it = 0;
	else if (val>=1.0) it = PDF.size()-1;
	else
	{
		first = 0;
		last = PDF.size()-2;
		while (first < last-1)
		{
			it = int((last+first)/2.0);
			if(PDF[first].val <= val and val < PDF[it].val) last = it;
			else first = it;
		}
		it = int((last+first)/2.0);
	}
	ix = PDF[it].i;
	iy = PDF[it].j;
	//std::cout << PDF[it].val << " < " << val << "<" << PDF[it+1].val << ", i = " << ix << ", j = " << iy << std::endl;
}

void HQ_XY::HQ_XY_sample()
{
	int ix, iy;
	std::uniform_real_distribution<double> unit_distribution(0.0, 1.0);
	fs::ofstream fo("out.txt");
	for(int i=0;i<N_sample_;i++)
	{
		iPDF(unit_distribution(generator), ix, iy);
		XY_list_[i][0] = x_min_ + ix*dx_ + dx_*(unit_distribution(generator));
		XY_list_[i][1] = y_min_ + iy*dy_ + dy_*(unit_distribution(generator));
		fo << XY_list_[i][0] << " "  << XY_list_[i][1] << std::endl;
	}
	
}





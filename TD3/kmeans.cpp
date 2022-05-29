#include <iostream>
#include <cassert>
#include <cmath>	// for sqrt, fabs
#include <cfloat>	// for DBL_MAX
#include <cstdlib>	// for rand, srand
#include <ctime>	// for rand seed
#include <fstream>
#include <cstdio>	// for EOF
#include <string>
#include <string.h>
#include <algorithm>	// for count
#include <vector>

using std::rand;
using std::srand;
using std::time;

class point
{
    public:

        static int d;
        double *coords;
        int label;

		point () {
			coords = new double[d];
			label = 0;
		}

		~point () {
			delete[] coords;
		}

		void print() const {
			for (int i = 0; i<d; i++){
				std::cout << coords[i];
				if(i==d-1){
					std::cout << "\n";
				}
				else
				{
					std::cout << "\t";
				}
				
			}
		}

		double squared_dist(const point &q) const{
			
			double dist = 0;
			for (int j = 0; j< d; j++){
				dist = dist + std::pow(coords[j]-q.coords[j], 2);
			}
			return dist;
		}
};

int point::d;

class cloud
{
	private:

	int d;
	int n;
	int k;

	// maximum possible number of points
	int nmax;

	point *points;
	point *centers;


	public:

	cloud(int _d, int _nmax, int _k)
	{
		d = _d;
		point::d = _d;
		n = 0;
		k = _k;

		nmax = _nmax;

		points = new point[nmax];
		centers = new point[k];

		srand(time(0));
	}

	~cloud()
	{
		delete[] centers;
		delete[] points;
	}

	void add_point(const point &p, int label)
	{
		for(int m = 0; m < d; m++)
		{
			points[n].coords[m] = p.coords[m];
		}

		points[n].label = label;

		n++;
	}

	int get_d() const
	{
		return d;
	}

	int get_n() const
	{
		return n;
	}

	int get_k() const
	{
		return k;
	}

	point& get_point(int i)
	{
		return points[i];
	}

	point& get_center(int j)
	{
		return centers[j];
	}

	void set_center(const point &p, int j)
	{
		for(int m = 0; m < d; m++)
			centers[j].coords[m] = p.coords[m];
	}

	double intracluster_variance() const
	{
		double var = 0;
		for(int i = 0; i<nmax; i++){
			var += points[i].squared_dist(centers[points[i].label]);
		}
		return var/nmax;
	}

	int set_voronoi_labels()
	{
		int nCh = 0; //# of points changing their label
		double dis;
		double d0;
		for(int i = 0; i<nmax; i++){
			d0 = points[i].squared_dist(centers[points[i].label]);
			dis = d0;
			for(int j=0; j<k; j++){
				
				if(points[i].squared_dist(centers[j]) < dis){
					points[i].label = j;
					dis = points[i].squared_dist(centers[j]);
				}
			}
			if(d0!=dis){
				nCh++;
			}
		}
		return nCh;
	}

	void set_centroid_centers()
	{
		
		int* nC = new int[k](); // # of points belonging to center i
		double coC[k][d]; //sum of coordinates of points belonging to center i
		memset(coC, 0, sizeof(coC));
		for(int i = 0; i<nmax; i++){
			nC[points[i].label]++;
			for(int j = 0; j<d; j++){
				coC[points[i].label][j] += points[i].coords[j];
			}
		}
		for(int u = 0; u<k; u++){
			if(nC[u]==0){
				continue;
			}
			else{
				for(int v = 0; v<d; v++){
					centers[u].coords[v] = coC[u][v] / nC[u];
				}
			}
		}
		// delete[] nC;
		// delete[] coC;

	}

	void init_random_partition()
	{
		for(int i = 0; i<nmax; i++){
			points[i].label = rand() % k;
		}
		set_centroid_centers();
	}

	void lloyd()
	{
		// initial
		// init_random_partition();
		// init_plusplus(); 
		// init_forgy();
		int nCh = -1;
		while(nCh!=0){
			nCh = set_voronoi_labels();
			set_centroid_centers();
		}
	}

	void init_forgy()
	{
		
		int* a = new int[k];
		int b;
		bool c;
		for(int i = 0; i<k; i++){
			do
			{
				c=true;
				b = rand() % nmax;
				for(int j=0; j<i; j++){
					c = c&&(b!=a[j]);
				}
			} while (c==false);
			a[i]=b;
			for (int j = 0; j < d; j++) {
				centers[i].coords[j] = points[b].coords[j];
            }
		}
		
	}

	void init_plusplus()
	{
		//set center0 at c1, uniformly and randomly from P
		int r = rand() % nmax;
		for (int j = 0; j < d; j++) {
			centers[0].coords[j] = points[r].coords[j];
		}
		double*dist = new double[nmax];
		double rd, sum, total_dist;
		for(int i = 1; i<k; i++){
			total_dist = 0;
            for (int j = 0; j < nmax; j++) {
                dist[j] = points[j].squared_dist(centers[0]);
                for (int l = 1; l < i; l++) {
                    dist[j] = std::min(points[j].squared_dist(centers[l]), dist[j]);
                }
                total_dist += dist[j];
            }
            rd = (double)rand() / RAND_MAX;
            sum = 0;
            r = -1;
            for (int j = 0; j < nmax; j++) {
                r++;
                sum += dist[j] / total_dist;
                if (rd < sum) {
                    break;
                }
            }

            for (int j = 0; j < d; j++) {
                centers[i].coords[j] = points[r].coords[j];
            }
		}
	}
};

#pragma once

/** 
	The point class stores a vector.
*/
class point
{
    /**
      The dimension of the vector (shared by all objects).
    */
	static int d;

public:
    /**
      The coordinates of the vector.
    */
	double *coords;
    /**
      The label (not used here).
    */
	int label;

    /**
      Dimension setter.
    */
	static bool set_dim (int _d);
    /**
      Dimension getter.
    */
	static int get_dim ();

	point ();
	~point ();

	void print() const;
    /**
      Standard dist (not squared)
	  @param q (point&) distance from current point to q.
    */
	double dist (point &q) const;
};

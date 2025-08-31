#ifndef __SIMPLEMATRIX_H
#define __SIMPLEMATRIX_H

#include <vector>  // Need this to store matrix values

template < typename Type = double> class SimpleMatrix {
    private :
    // Use a "vector of vectors " to store the values
        std::vector<std::vector<Type> > mat;

    public:
        SimpleMatrix(); //Default Constructor

        //Constructor specifying rows , columns and default value
        SimpleMatrix(const int& rows, const int& columns ,const Type& val);

        //Copy Constructor
        SimpleMatrix(const SimpleMatrix<Type>& rhs);

        //Assignment operator overloaded
        SimpleMatrix<Type>& operator= (const SimpleMatrix<Type>& _rhs);

        virtual ~SimpleMatrix(); // Destructor

        std::vector<std::vector<Type> > get_mat() const;

        Type& value(const int& row ,const int& col);




};


// This is needed as SimpleMatrix is a generic class
#include "simplematrix.cpp"


#endif
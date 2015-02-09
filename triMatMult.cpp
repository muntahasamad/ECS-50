//
//  main.cpp
//  triMatMult
//
//  Created by Muntaha Samad on 10/15/14.
//  Copyright (c) 2014 Muntaha Samad. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int factorial(int n)
{
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

int main(int argc, const char * argv[])
{
    int i,k, grid_size1, grid_size2, j=0, size1, size2, counter, grid_final1[50], grid_final2[50], column;
    int grid1[50], grid2[50];
    string infile1 = argv[1];
    string infile2 = argv[2];
    ifstream infile(infile1);
    if (!infile)
    {
        cout << "There was a problem opening file " << infile1 << " for reading." << endl;
        return 0;
    }
    cout << "Opened " << infile1 << " for reading." << endl;
    while (infile >> i)
    {
        grid1[j]=i;
        cout << grid1[j]<<endl;
        j++;
        
    }
    grid_size1 = grid1[0];
    counter = 0;
    column = grid_size1-1;
    size1 = factorial(grid_size1);
    j = 0;
    for (i=1; i <= size1; i++)
    {
        
        if (counter == grid_size1)
        {
            for (k=0; k<(counter-column); k++)
            {
                grid_final1[j] = 0;
                j++;
            }
            counter=0;
            counter+=k;
            column-=1;
            i--;
           
        }
        else
        {
            grid_final1[j]= grid1[i];
            counter++;
            j++;
            
        }
    }
    j=0;
    ifstream second_infile(infile2);
    if (!second_infile)
    {
        cout << "There was a problem opening file " << infile2 << " for reading." << endl;
        return 0;
    }
    cout << "Opened " << infile2 << " for reading." << endl;
    while (second_infile >> i)
    {
        grid2[j]=i;
        cout << grid2[j]<<endl;
        j++;
        
    }
    grid_size2 = grid2[0];
    counter = 0;
    column = grid_size2-1;
    size2 = factorial(grid_size2);
    j = 0;
    for (i=1; i <= size2; i++)
    {
        if (counter == grid_size2)
        {
            for (k=0; k<(counter-column); k++)
            {
                grid_final2[j] = 0;
                j++;
            }
            counter=0;
            counter+=k;
            column-=1;
            i--;
            
        }
        else
        {
            grid_final2[j]= grid2[i];
            counter++;
            j++;
            
        }
    }
    int solution[size2];
    int solution_counter=0, solutions;
    j=0;
    while (solution_counter!=size2)
    {
        for (i=0; i<grid_size2; i++)
        {
            solutions += grid_final1[i] * grid_final2[j];
            j+=grid_size2;
        }
        solution[solution_counter] = solutions;
        solution_counter++;
        if ((solution_counter%grid_size2)==0)
        {
            j=0;
        }
    }
    return 0;

}


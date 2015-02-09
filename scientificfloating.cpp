//
//  main.cpp
//  scientifcFloating.cpp
//
//  Created by Muntaha Samad on 10/14/14.
//  Copyright (c) 2014 Muntaha Samad. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

int main()
{
    double start_num, decimal_part, conversion;
    int int_part_conversion, sign=0, count, count2, int_part;
    int remainder[100];
    int remainder2[25];
    cout << "Please enter a float: ";
    cin >> start_num;
    if (start_num<0)
    {
        sign = -1;
        start_num *= -1;
    }
    int_part = int_part_conversion = trunc(start_num);
    int i=0;
    while (int_part_conversion!=0)
    {
        remainder[i] = int_part_conversion % 2;
        int_part_conversion /= 2;
        i++;
    }
    count2 = i-1;
    count = i-1;
    decimal_part = start_num - int_part;
    conversion = decimal_part*2;
    i =0;
    while (count < 23)
    {
        if (conversion>=1)
        {
            remainder2[i] = 1;
            count++;
            i++;
            conversion-=1;
        }
        else if (conversion<1)
        {
            remainder2[i] = 0;
            count++;
            i++;
        }
        conversion*=2;
    }
    if (sign==0)
    {
        while (true)
        {
            if (remainder2[i]==0)
            {
                i--;
            }
            else if (remainder2[i]==1)break;
        }
        cout << "1.";
        for (int j=count2-1; j>=0; j--)
        {
            cout << remainder[j];
        }
        for (int j = 0; j <= i; j++)
        {
            cout << remainder2[j];
        }
        cout << "E" << count2 << endl;
    }
    else if (sign==-1)
    {
        while (true)
        {
            if (remainder2[i]==0)
            {
                i--;
            }
            else if (remainder2[i]==1)break;
        }
        cout << "-1.";
        for (int j=count2-1; j>=0; j--)
        {
            cout << remainder[j];
        }
        for (int j = 0; j <= i; j++)
        {
            cout<<remainder2[j];
        }
        cout << "E" << count2 << endl;
    }
}


//
//  main.cpp
//  changebase.cpp
//
//  Created by Muntaha Samad on 10/14/14.
//  Copyright (c) 2014 Muntaha Samad. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
using namespace std;


int main()
{
    //Get values
    unsigned int start_base, end_base, num=0, base_10, count;
    string start_num;
    char alphabet [26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    char numbers[10] = {'0','1','2','3','4','5','6','7','8','9'};
    int remainder[100];
    cout << ("Please enter the number's base: ");
    cin >> start_base;
    cout << ("Please enter the number: ");
    cin >> start_num;
    unsigned long num1_length = start_num.length();
    int num1 [num1_length];
    for (int i=0; i < num1_length; i++)
    {
        char val = start_num.at(i);
        for (int j=0; j<26; j++)
        {
            if (val==alphabet[j])
            {
                num = j+10;
                break;
            }
            else if(j < 10)
            {
                if (val==numbers[j])
                {
                    num = j;
                    break;
                }
            }
        }
        num1[i] = num;
    }
    cout << ("Please enter the new base: ");
    cin >> end_base;
    
    //Convert to base 10
    base_10 = 0;
    for (int i=0; i < num1_length; i++)
        {
            //int value = pow(start_base,num1_length-(1+i));
            //cout << num1[i] << endl << value << endl;
            base_10 += num1[i]*pow(start_base,num1_length-(1+i));
            
        }
    //Convert to new base
    if (base_10 == (0 | 10))
    {
        cout << start_num << " base " << start_base << " is " << base_10 << " base " << end_base << endl;
    }
    else
    {
        int i=0;
        while (base_10!=0)
        {
            remainder[i] = base_10 % end_base;
            base_10 /= end_base;
            i++;
        }
        count = i-1;
        //Print
        cout << start_num << " base " << start_base << " is ";
        for (int j=count; j>=0; j--)
        {
            if (remainder[j]>9)
            {
                cout << alphabet[remainder[j]-10];
            }
            else
            {
                cout << remainder[j];
            }
        }
        cout << " base " << end_base << endl;
    }
}


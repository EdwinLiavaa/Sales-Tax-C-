/*
 * salestax.cpp
 *
 *  Created on: April 8, 2022
 *  Author: Edwin Liava'a
 */

#include <iostream>
#include <sstream> 
#include <fstream> 
#include <vector> 
#include <string> 
#include <iomanip>  
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <math.h>
using namespace std;

// constant variables

const int ITEM_WITH_NOSALESTAX_AND_NOIMPORTDUTY = 1;
const int ITEM_WITH_NOSALESTAX_ONLY_IMPORTDUTY = 2;
const int ITEM_WITH_SALESTAX_AND_NOIMPORTDUTY = 3;
const int ITEM_WITH_SALESTAX_AND_IMPORTDUTY = 4;

const double SALES_TAX_RATE = 10;
const double IMPORT_DUTY_RATE = 5;

// int, string, float
// Quantity,Product,Price
struct record {
    int quantity;
    string product;
    float price;
};

// create vector record
record CreateRecord(vector<string> vstr)
{
    if(vstr.size() != 3)
        {
        cout << "Internal logic error - vector of substrings != 3\n";
        exit(-1);        
        }
    record Rec;
    stringstream ss;
    ss << vstr[0];
    ss >> Rec.quantity;
    ss.clear();
    Rec.product = vstr[1];
    ss << vstr[2];
    ss >> Rec.price;
    return Rec;
}

// rounded up to the nearest 0.05
double RoundOff(double aTax)
{
    int taxTemp = (int)aTax;
    double decimaltaxTemp = (double)(aTax - (int)taxTemp);
    int tempy = (int)(1000 * decimaltaxTemp) / 100;
    int tempz = (int)(1000 * decimaltaxTemp - tempy * 100);
    int temp = (int)(tempz / 10);
    int t = tempz % 10;

    if(t >= 5)
        temp += 1;

    return (double)(taxTemp + tempy * (0.1) + temp * (0.01));
}

// calculate sales tax
double ComputeSalesTax(double aPrice, double aRate, double aImportduty)
{
    double tx = (aPrice * aRate / (double(100))) + (aPrice * aImportduty / (double(100)));
    return RoundOff(tx);
}

// function main
int main(int argc, char* argv[])
{
    string filename = "input.csv";
    std::ifstream in(filename.c_str()); 

    // open file and check if OK
    if(!in)
        {
        std::cout << "File Failed!\n";
        return -1;
        }

    // declare the vector array
    vector<record> vr;
    vector<string>::iterator itr;
    std::string line; 
    bool first = true;
    
    // calculations
    int decision; // holds decision on how to apply sales tax
    int numofitems; // holds the quantity
    string prod; // holds the products
    double itemprice; // holds itemprice read from file
    double tax = 0.0; // holds item tax
    double taxtotal = 0.0; // holds total tax
    double subtotal = 0.0; // holds total itemprice
    double grandTotal= 0.0; // holds gross subtotal+tax

    while(getline(in, line)) 
        {
        // cout << line << endl;
        if(first)
            {
            first = false;
            continue; // skip first line
            }
        // string, string, float
        // split into 3 substrings
        vector<string> vs;
        string subst;
        string::size_type offs = 0, loc = string::npos;
        while((loc = line.find(',', offs)) != string::npos)
            {
            subst = line.substr(offs, loc - offs);
            vs.push_back(subst);
            offs = loc + 1;
            }
        subst = line.substr(offs);
        vs.push_back(subst);
        vr.push_back(CreateRecord(vs));
        } // while(getline(in, line)) 

    if(!in.eof())
        {
        cout << "Error reading the file!\n";
        return -1;
        }

    cout << endl;

    for(size_t n=0; n<vr.size(); ++n)
    {
        numofitems = vr[n].quantity;
        prod = vr[n].product;
        itemprice = vr[n].price;

        if (prod == " book") 
        {
            decision = ITEM_WITH_NOSALESTAX_AND_NOIMPORTDUTY;
        }
        else if (prod == " music cd")
        {
            decision = ITEM_WITH_SALESTAX_AND_NOIMPORTDUTY;
        }
        else if (prod == " chocolate bar")
        {
            decision = ITEM_WITH_NOSALESTAX_AND_NOIMPORTDUTY;
        }
        else if (prod == " imported box of chocolates")
        {
            decision = ITEM_WITH_NOSALESTAX_ONLY_IMPORTDUTY;
        }
        else if (prod == " imported bottle of perfume")
        {
            decision = ITEM_WITH_SALESTAX_AND_IMPORTDUTY;
        }
        else if (prod == " bottle of perfume")
        {
            decision = ITEM_WITH_SALESTAX_AND_NOIMPORTDUTY;
        }
        else if (prod == " packet of headache pills")
        {
            decision = ITEM_WITH_NOSALESTAX_AND_NOIMPORTDUTY;
        }
        else if (prod == " box of imported chocolates")
        {
            decision = ITEM_WITH_NOSALESTAX_ONLY_IMPORTDUTY;
        }
        else {
            cout << "New Product Not Found!" << endl;
        }

        switch(decision) {

            case 1: //ITEM_WITH_NOSALESTAX_AND_NOIMPORTDUTY

                tax = ComputeSalesTax(itemprice,0.0,0.0);
                taxtotal = (taxtotal + tax) * numofitems;
                itemprice = (itemprice + tax) * numofitems;
                subtotal = subtotal + itemprice;

            break;

            case 2: //ITEM_WITH_NOSALESTAX_ONLY_IMPORTDUTY

                tax = ComputeSalesTax(itemprice,0.0,IMPORT_DUTY_RATE);
                taxtotal = (taxtotal + tax) * numofitems;
                itemprice = (itemprice + tax) * numofitems;
                subtotal = subtotal + itemprice;
                
            break;

            case 3: //ITEM_WITH_SALESTAX_AND_NOIMPORTDUTY

                tax = ComputeSalesTax(itemprice,SALES_TAX_RATE,0.0);
                taxtotal = (taxtotal + tax) * numofitems;
                itemprice = (itemprice + tax) * numofitems;
                subtotal = subtotal + itemprice;
                
            break;

            case 4: //ITEM_WITH_SALESTAX_AND_IMPORTDUTY

                tax = ComputeSalesTax(itemprice,SALES_TAX_RATE,IMPORT_DUTY_RATE);
                taxtotal = (taxtotal + tax) * numofitems;
                itemprice = (itemprice + tax) * numofitems;
                subtotal = subtotal + itemprice;
          
            break;

            default:
                cout << "New Product Not Found!" << endl;          
        }

        cout << fixed << setprecision(2);
        cout << numofitems << "," << prod << ", "
             << itemprice << endl;             
    } 

    grandTotal = grandTotal + subtotal;   

    cout << fixed << setprecision(2);
    cout << endl << "Sales Taxes: " << taxtotal << endl;
    cout << "Total: " << grandTotal << endl << endl; 

    return 0;
}



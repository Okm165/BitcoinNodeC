/*
          _           _               
         | |         (_)              
      ___| |__   __ _ _ _ __  _ __    
     / __| '_ \ / _` | | '_ \| '_ \   
    | (__| | | | (_| | | | | | |_) |  
     \___|_| |_|\__,_|_|_| |_| .__/   
                             | |      
                             |_|      
                              
Chain Parser functionalty --> parse state
------------------------------------------------------------------------------------------
In order to use parser create object ChainP and call parse() functionality
set progress to true if you want to display progress bars,
parser will take some time and than save parsed chainstate,
(output of composeAddressDict and composeAmountDict) in path,
spacified by ("cache": ?) in config/paths.json
------------------------------------------------------------------------------------------
void composeAddressDict()
Composes address dictionary from chainstate,
specified by Chain* with help of OpCodes* to decode raw TxOut data,
address dictionary consits of "key" and "value" pairs, "key" is base58 address code,
and "value" is sum of all TxOut(s) corresponding to this address.
NOTE: chainstate stores all TxOuts
(transactions outputs waiting to be resolved by appropriate users),
it is common that more than one TxOut corresponds to single address,
that is why it is reasonable to parse chainstate, in order to get,
dictionary with address("key"), and total amount("value")
------------------------------------------------------------------------------------------
void composeAmountDict()
Composes amount dictionary from AddressDict,
each address in AddressDict has some specific amount by it,
AmountDict consits of "key" and "value" pairs,
"keys" are amounts (values from AddressDict),
"values" is number of how many time this amount occured in AddressDict,
AmountDict stores information about how bitcoin is distributed between addresses,
each amount has value of how many times it occured,
very usefull to answer question like: 
"give me number of addresses that have saldo over 100000 SAT"
------------------------------------------------------------------------------------------
void parse()
runs parser
------------------------------------------------------------------------------------------
*/

#ifndef CHAINP_H
#define CHAINP_H

#include "../scripts/scripts.h"

class ChainP
{
    protected:

    Dict* paths;
    Chain* chain;
    AddressDecoder* opcodes;
    
    adDict addict;
    amDict amdict;

    bool progress;

    ProgressBar* bar;
    
    public:

    ChainP(){};
    ChainP(Dict* paths, Chain* chain, AddressDecoder* opcodes, bool progress)
    {
        this->paths = paths;
        this->chain = chain;
        this->opcodes = opcodes;
        this->progress = progress;
    }
    void title();

    void composeAddressDict();
    void composeAmountDict();
    void parse();
};

#endif
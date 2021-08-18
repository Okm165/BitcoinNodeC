#include "chainp.h"

void ChainP::title(){std::cout << "chain parser" << std::endl;}

void ChainP::composeAddressDict()
{
    std::cout << "\r" << "calculating chainstate length..." << std::flush;
    uint64_t chainstate_length = chain->getLength(10000);
    std::cout << "\r" << "chainstate length = " << chainstate_length << "              " << std::endl;

    ProgressBar bar ("composing address dict", chainstate_length, PROGRESS_BAR_SETTINGS);
    for(uint64_t it = 0; it < chainstate_length; it++)
    {
        CTx tx = readCTx(chain);
        
        Address address;
        addrdec->addressDecode(&address, &tx.scriptSig, TYPE);

        if(address.IsValid())
            dictWrite(&addict, address.address, tx.amount);
        
        bar.update();
    }
    bar.close();
}

void ChainP::composeAmountDict()
{
    ProgressBar bar ("composing amount dict", addict.size(), PROGRESS_BAR_SETTINGS);
    for(adDict::iterator it = addict.begin(); it != addict.end(); it++)
    {
        dictWrite(&amdict, it->second, 1);
        bar.update();
    }
    bar.close();
}

void ChainP::parse()
{
    std::string path = (*paths)["cache"] + StringToHexReversed(chain->hash);
    if(dirExists(path.c_str())){clearDir(path);}
    else{mkDir(path.c_str());}

    composeAddressDict();
    saveDict(path + "/addressDb", addict);
    composeAmountDict();
    saveDict(path + "/amountDb", amdict);
}

int main()
{
    Dict paths = composeJsonDict("../config/paths.json");
    Chain chain (paths["chainstate"].c_str());
    AddressDecoder addrdec;
    ChainP chainp(&paths ,&chain, &addrdec);
    chainp.parse();
}
#include "chainp.h"

void ChainP::title(){std::cout << "chain parser" << std::endl;}

void ChainP::composeAddressDict()
{
    std::cout << "\r" << "calculating chainstate length..." << std::flush;
    uint64_t chainstate_length = chain->getLength();
    std::cout << "\r" << "chainstate length = " << chainstate_length << "              " << std::endl;

    ProgressBar bar ("composing address dict", chainstate_length, PROGRESS_BAR_SETTINGS);
    for(uint64_t it = 0; it < chainstate_length; it++)
    {
        CBlock tx = readCBlock(chain, opcodes);
        if(tx.address.IsValid())
            dictWrite(&addict, tx.address.address, tx.amount);
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
    Chain chain(paths["chainstate"].c_str());
    AddressDecoder addrdec;
    ChainP chainp(&paths ,&chain, &addrdec);
    chainp.parse();
}
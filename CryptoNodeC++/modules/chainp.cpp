#include "chainp.h"

void ChainP::title()
{
//    std::cout << "||         _           _                 ||" << std::endl;
//    std::cout << "||        | |         (_)                ||" << std::endl;
//    std::cout << "||     ___| |__   __ _ _ _ __  _ __      ||" << std::endl;
//    std::cout << "||    / __| '_ \\ / _` | | '_ \\| '_ \\     ||" << std::endl;
//    std::cout << "||   | (__| | | | (_| | | | | | |_) |    ||" << std::endl;
//    std::cout << "||    \\___|_| |_|\\__,_|_|_| |_| .__/     ||" << std::endl;
//    std::cout << "||                            | |        ||" << std::endl;
//    std::cout << "||                            |_|        ||" << std::endl;
//    std::cout << std::endl;
   std::cout << "chain parser" << std::endl;
}

void ChainP::composeAddressDict()
{
    if(progress){std::cout << "\r" << "calculating chainstate length..." << std::flush;}
    uint64_t chainstate_length = chain->getLength();
    if(progress){std::cout << "\r" << "chainstate length = " << chainstate_length << "              " << std::endl;}

    if(progress){bar = new ProgressBar("composing address dict...", chainstate_length, PROGRESS_BAR_SETTINGS);}
    for(uint64_t it = 0; it < chainstate_length; it++)
    {
        CBlock tx = readCBlock(chain, opcodes);
        if(!tx.address.addressFlag || tx.amount == 0 || tx.address.type == FAIL){bar->update(); continue;}
        if(addict.find(tx.address.address) != addict.end()){addict[tx.address.address] = addict[tx.address.address] + tx.amount;}
        else{addict[tx.address.address] = tx.amount;}
        if(progress){bar->update();}
    }
    if(progress){bar->close();}
    if(progress){delete bar;}

}

void ChainP::composeAmountDict()
{
    if(progress){bar = new ProgressBar("composing amount dict...", addict.size(), PROGRESS_BAR_SETTINGS);}
    for(adDict::iterator it = addict.begin(); it != addict.end(); it++)
    {
        if(amdict.find(it->second) != amdict.end()){amdict[it->second] = amdict[it->second] + 1;}
        else{amdict[it->second] = 1;}
        if(progress){bar->update();}
    }
    if(progress){bar->close();}
    if(progress){delete bar;}
}

void ChainP::parse()
{
    std::string path = (*paths)["cache"] + StringToHexReversed(chain->hash);
    if(dirExists(path.c_str())){clearDir(path);}
    else{mkDir(path.c_str());}

    composeAddressDict();
    saveDict(path + "/addressDb", true, addict, progress);
    composeAmountDict();
    saveDict(path + "/amountDb", true, amdict, progress);
}

int main()
{
    Dict paths = composeJsonDict("../config/paths.json");
    Chain chain(paths["chainstate"].c_str());
    AddressDecoder addrdec;
    ChainP chainp(&paths ,&chain, &addrdec, true);
    chainp.title();
    chainp.parse();
}
#include "../scripts/scripts.h"

int main()
{
    AddressDecoder addrdec;
    std::string pkscript(HexToString("232102ad5d7d59d2072bda607bd06e61f4e9b37b07304785bb44352a495b617166f9f3ac"));
    std::cout << pkscript.size() << std::endl;
    Address addr_buf;
    BStream stream(&pkscript);
    addrdec.addressDecode(&addr_buf, &stream, SCRIPT);
    std::cout << addr_buf.print() << std::endl;
}
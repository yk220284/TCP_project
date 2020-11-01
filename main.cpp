#include <iostream>
#include "Message.h"
using namespace std;

int main()
{
    Header h1(132, 172, 258427), h2;
    // messageType=1, listingID, orderId, orderQuantity, orderPrice, side
    NewOrder no1(12, 1, 20, 1500, 'b'), no2;
    uint8_t buffer[16 + 35] = {0};
    h1.serialize(buffer);
    no1.serialize(buffer);

    cout << h1;
    cout << no1;
    cout << endl;
    cout << "main: size " << sizeof(buffer);
    h2.deserialize(buffer);
    no2.deserialize(buffer);
    cout << h2;
    cout << no2;
}

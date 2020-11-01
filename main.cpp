/*
    main.cpp
    1. Deomstrate how to place, modify, confirm trade though member functions of Portfolio class.
*/

#include <iostream>
#include <vector>
#include "Message.h"
#include "Portfolio.h"
using namespace std;

enum STOCKS
{
    A,
    B,
    C,
};
int main()
{
    /* Test serialize and deserialise */
    cout << "----Testing Serialisation----\n";
    Header h1(132, 172, 258427), h2;
    NewOrder no1(12, 1, 20, 1500, 'b'), no2; // messageType=1, listingID, orderId, orderQuantity, orderPrice, side
    uint8_t buffer[16 + 35] = {0};
    h1.serialize(buffer);
    no1.serialize(buffer);
    cout << "Before serialisation----\n";
    cout << h1;
    cout << no1;
    cout << "After serialisation----\n";
    h2.deserialize(buffer);
    no2.deserialize(buffer);
    cout << h2;
    cout << no2;
    cout << endl;

    /* Test place_order */
    cout << "----Placing Orders----\n";
    uint8_t order_id = 0;
    Portfolio p(20, 15);
    vector<NewOrder> orders;
    // Place orders
    cout << p.place_order(NewOrder(A, order_id++, 15, 1500, 's'));
    cout << p.place_order(NewOrder(A, order_id++, 4, 1500, 'b'));
    cout << p.place_order(NewOrder(A, order_id++, 20, 1500, 's'));
    // Delete order
    p.delete_order(DeleteOrder(0));
    // Place more
    cout << p.place_order(NewOrder(A, order_id++, 10, 1500, 's'));
    cout << p.place_order(NewOrder(A, order_id++, 10, 1500, 'b'));
    cout << endl;
    // Modify Order
    cout << "----Modifying Order (id 1)----\n"
         << "before modification" << endl
         << p.orders[1]
         << endl
         << "modify quantity to 6\n";
    p.modify_order(ModifyOrderQuantity(1, 6));
    cout << p.orders[1] << endl;

    cout << endl
         << "modify quantity to 13 (Rejected)\n"; // Rejectd, too high.
    p.modify_order(ModifyOrderQuantity(1, 13));
    cout << p.orders[1] << endl;

    // Confirm Trade
    cout << "----confirm trade (sell)----\n";
    p.confirm_trade(Trade(A, 2, 3, 1500));
    cout << p.place_order(NewOrder(A, order_id++, 3, 1500, 's')); //Rejected.
    return 0;
}

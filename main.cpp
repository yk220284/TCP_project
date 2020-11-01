#include <iostream>
#include <vector>
#include "Message.h"
#include "Portfolio.h"
using namespace std;
enum MESSAGE_TYPE
{
    ORDER = 1,
    DELETE,
    MODIFY,
    TRADE,
};
enum STOCKS
{
    A,
    B,
    C,
};
int main()
{
    /* Test serialize and deserialise */
    // Header h1(132, 172, 258427), h2;
    // // messageType=1, listingID, orderId, orderQuantity, orderPrice, side
    // NewOrder no1(12, 1, 20, 1500, 'b'), no2;
    // uint8_t buffer[16 + 35] = {0};
    // h1.serialize(buffer);
    // no1.serialize(buffer);

    // cout << h1;
    // cout << no1;
    // cout << endl;
    // cout << "main: size " << sizeof(buffer);
    // h2.deserialize(buffer);
    // no2.deserialize(buffer);
    // cout << h2;
    // cout << no2;

    /* Test place_order */
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
    // Modify Order
    cout << endl
         << "before modification" << endl
         << p.orders[1];
    cout << endl
         << "modify to 6\n";
    p.modify_order(ModifyOrderQuantity(1, 6));
    cout << p.orders[1] << endl;

    cout << endl
         << "modify to 13\n";
    p.modify_order(ModifyOrderQuantity(1, 13));
    cout << p.orders[1] << endl;

    // Confirm Trade
    cout << "confirm trade" << endl;
    p.confirm_trade(Trade(A, 1, 3, 1500));
    cout << p.place_order(NewOrder(A, order_id++, 3, 1500, 'b')); //Unsuccessful.
    return 0;
}

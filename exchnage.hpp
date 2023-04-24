#pragma once
#include <iostream>
#include <string>
using std::string;
#include <map>
using std::map;


// #include "useraccount.hpp"
#include "utility.hpp"

class Exchange {
 public:
  map< string, map<string, int>>user_portfolio;
  std::map<std::string, std::vector<Order>> open_orders;
  std::map<std::string, std::vector<Order>> open_orders2;
  
  std::map<std::string, std::vector<Order>> filled_orders;
  Order buyer_new_order;
  Order seller_new_order;
  void MakeDeposit(const std::string &username, const std::string &asset,
                   int amount);
  void PrintUserPortfolios(std::ostream &os);
  bool MakeWithdrawal(const std::string &username, const std::string &asset,
                      int amount);
  bool buyingCheck(const Order& buy_order);
  bool salesCheck(const Order& sales_order);
  void sellOrder(const Order&order);
  void buyOrder(const Order&order);
  bool AddOrder(const Order &order);
  void PrintUsersOrders(std::ostream &out);

  
};

#pragma once
#include <iostream>
#include <string>
#include<vector>
using std::string;
#include <map>
#include <unordered_map>

using std::map;


// #include "useraccount.hpp"
#include "utility.hpp"

class Exchange {
 public:
  map< string, map<string, int>>user_portfolio;
  std::map<std::string, std::vector<Order>> open_orders;
  // std::map<string, std::vector<Order>>trades;
  std::vector<Trade> trades;
  int number = 0;
  // std::vector< std::map< string, std::vector<Order>>> trades;
  std::map<std::string, std::vector<Order>> best_vec;
  std::map<std::string, std::vector<Order>> filled_orders;
  // bool truth_value = false;
  void MakeDeposit(const std::string &username, const std::string &asset,
                   int amount);
  void PrintUserPortfolios(std::ostream &os);
  bool MakeWithdrawal(const std::string &username, const std::string &asset,
                      int amount);
  bool buyingCheck( const Order& buy_order);
  bool salesCheck(const Order& sales_order);
 std::vector<Order> makeVector();
  void buyOrder(Order &order1, Order &best_order1);
  void sellOrder(Order &order1, Order &best_order1);
  void PrintUsersOrders(std::ostream &out);
  void PrintTradeHistory(std::ostream &o);
  bool AddOrder(const Order &new_order1);
  void eraseOrder(Order orde);
  // bool compareBuyOrdersByPrice(Order& a, Order& b);
  // bool compareSellOrdersByPrice( Order& a,  Order& b);
 std::vector<Order> bestTrade(Order const &order, std::map<std::string, std::vector<Order>>open_orders);
  void PrintBidAskSpread(std::ostream & os);
  // int Amount(const Order &first, const Order &second);

  
};
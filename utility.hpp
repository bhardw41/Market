#pragma once
#include <iostream>
#include <string>
// #include "exchange.hpp"

struct Order {

  std::string username;
  std::string side;  // Can be "Buy" or "Sell"
  std::string asset;
  int amount;
  int price;


 Order(const std::string &username, const std::string &action,
        const std::string &asset, int quantity, int price)
      : username(username), side(action), asset(asset), amount(quantity),
        price(price) {}
  Order(const Order &input)
      : username(input.username), side(input.side), asset(input.asset),
        amount(input.amount), price(input.price) {}
  Order();
  
  friend std::ostream &operator<<(std::ostream &oss, const Order &order_placed){
    oss << order_placed.side << " " << order_placed.amount << " " 
        << order_placed.asset << " at " << order_placed.price << " USD " << "by " 
        << order_placed.username;
    
    return oss;
  }
  friend bool operator==(Order& compare1, Order& compare2){
    return ((compare1.username == compare2.username) && (compare1.side == compare2.side)
        && (compare1.asset == compare2.asset) && ( compare1.amount == compare2.amount)
        && (compare1.price == compare2.price));    
}
  
};

struct Trade {
  std::string buyer_username;
  std::string seller_username;
  std::string asset;
  int amount;
  int price;
     friend std::ostream &operator<<(std::ostream &oss, const Trade &ord){
     oss << ord.buyer_username << " Bought " << ord.amount  << " of " << ord.asset << " From " << ord.seller_username << " for " << ord.price << " USD\n";
  
    
    return oss;
  }
    friend bool operator==(Trade& compare1, Trade& compare2){
    return ((compare1.buyer_username == compare2.buyer_username) && (compare1.seller_username == compare2.seller_username)
        && (compare1.asset == compare2.asset) && ( compare1.amount == compare2.amount)
        && (compare1.price == compare2.price));    
}
};
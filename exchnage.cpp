#include "exchange.hpp"
#include "utility.hpp"
#include <string>
#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

using std::string;
using std::ostream;
void Exchange::MakeDeposit(const string &name, const string &assets, int amt) {
  user_portfolio[name][assets] += amt;
}
void Exchange::PrintUserPortfolios(ostream &out) {
  out << "User Portfolios (in alphabetical order):"
      << "\n";
  for (const auto & [ name, assets ] : user_portfolio) {
    out << name << "'s Portfolio: ";
    for (const auto & [ asset, amt ] : assets) {
      if (amt > 0) out << amt << " " << asset << ", ";
    }
    out << "\n";
  }
}

bool Exchange::MakeWithdrawal(const std::string &name,
                              const std::string &assets, int amt) {
  if (user_portfolio.count(name) > 0 &&
      user_portfolio[name].count(assets) > 0) {
    if (user_portfolio[name][assets] >= amt) {
      user_portfolio[name][assets] -= amt;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool Exchange::salesCheck(const Order &sales_order) {
  return user_portfolio[sales_order.username].count(sales_order.asset) > 0 &&
         user_portfolio[sales_order.username][sales_order.asset] >=
             sales_order.amount;
}
bool Exchange::buyingCheck(const Order &buy_order) {
  return user_portfolio[buy_order.username].count("USD") > 0 &&
         user_portfolio[buy_order.username]["USD"] >=
             buy_order.amount * buy_order.price;
}
void Exchange::buyOrder(const Order&order){
  Order buyer_new_order ;
  Order seller_new_order ;
  buyer_new_order = order;
  Order partial = order;
    
    for (const auto & [ name, orders ] : open_orders) {
      open_orders2.clear();

      for (const auto & orderss : orders) {
        seller_new_order = orderss;
        if (orderss.side == "Sell" 
             && order.asset == orderss.asset && orderss.price <= order.price) {
          seller_new_order.price = buyer_new_order.price;
          if(order.amount > orderss.amount) {
            // quantity = buyer_new_order.amount - seller_new_order.amount;
            // open_orders2[name].push_back()
            partial.amount = buyer_new_order.amount - seller_new_order.amount;
            buyer_new_order.amount = seller_new_order.amount;
            
            }
          else {
            partial.amount = seller_new_order.amount - buyer_new_order.amount;
            seller_new_order.amount = buyer_new_order.amount;
          }
          filled_orders[buyer_new_order.username].push_back(buyer_new_order);
          filled_orders[name].push_back(seller_new_order);
     
          user_portfolio[name]["USD"] += buyer_new_order.amount * buyer_new_order.price;
          user_portfolio[order.username][buyer_new_order.asset] += (buyer_new_order.amount );
          // open_orders2 
          // open_orders2[order.username].push_back(partial);
        }
        else{
          partial = seller_new_order;
        }
      }
      open_orders2[order.username].push_back(partial);
      
    }
  }

void Exchange::sellOrder(const Order&order){
  Order buyer_new_order ;
  Order seller_new_order ;
  Order partial;
  for (const auto & [name, orders] : open_orders) {
      open_orders2.clear(); 
      for (const auto &orderss : orders) {
        buyer_new_order = orderss;
        partial = order;
        if (orderss.side == "Buy" && order.asset == orderss.asset && orderss.price >= order.price){
          buyer_new_order.price = seller_new_order.price;
          partial.price = buyer_new_order.price;
          if(order.amount > orderss.amount) {
            partial.amount = seller_new_order.amount - buyer_new_order.amount;
            seller_new_order.amount  = buyer_new_order.amount;
            }
          else {
            partial.amount = buyer_new_order.amount - seller_new_order.amount;
            buyer_new_order.amount  = seller_new_order.amount;
          }

          filled_orders[seller_new_order.username].push_back(seller_new_order);
          filled_orders[name].push_back(buyer_new_order);
          // open_orders.erase(order.username);
          user_portfolio[name][seller_new_order.asset] -= seller_new_order.amount;
          user_portfolio[seller_new_order.username]["USD"] += seller_new_order.amount * seller_new_order.price;
          user_portfolio[name][buyer_new_order.asset] += buyer_new_order.amount;
          // open_orders2[order.username].push_back(partial);
        }
         else{
          partial = buyer_new_order;
        }
        open_orders2[order.username].push_back(partial);
      }
      
    }
  }

bool Exchange::AddOrder(const Order &order) {
  if (order.side == "Sell") {
    if (salesCheck(order)){
      user_portfolio[order.username][order.asset] -= order.amount;
    }
    
    else  return false;
    
  } else if (order.side == "Buy") {
    if (buyingCheck(order)) {
      // user_portfolio[order.username][order.asset] += order.amount;
      user_portfolio[order.username]["USD"] -= order.amount * order.price;
    } else {
      return false;
    }
  }


  open_orders[order.username].push_back(order);
  Order buyer_new_order ;
  Order seller_new_order ;
  // open_orders2.clear(); 
  // int quantity;

  if (order.side == "Buy") {
    buyOrder(order);
  }
  else if(order.side == "Sell"){
    sellOrder(order);
  }
  
  return true;
}

void Exchange::PrintUsersOrders(ostream &out){
  out << "Users Orders (in alphabetical order):\n";
  for (const auto & [ names, assets ] : user_portfolio){
    out << names << "'s Open Orders (in chronological order):\n";
    for (const auto & order : open_orders2[names]){
      out << order.side << " " << order.amount << " " << order.asset
          << " at " << order.price << " USD by " << order.username << "\n";
    }
    out << names << "'s Filled Orders (in chronological order):\n";
    for (const auto &order : filled_orders[names]){
      out << order.side << " " << order.amount << " " << order.asset
          << " at " << order.price << " USD by " << order.username << "\n";
    }
  }
}










// buyer_new_order = order;
    
    // for (const auto & [ name, orders ] : open_orders) {
    //   open_orders2.clear(); 
    //   for (const auto & orderss : orders) {
    //     seller_new_order = orderss;
    //     if (orderss.side == "Sell" 
    //          && order.asset == orderss.asset && orderss.price <= order.price) {
    //       seller_new_order.price = buyer_new_order.price;
    //       if(order.amount > orderss.amount) {
    //         // quantity = buyer_new_order.amount - seller_new_order.amount;
    //         // open_orders2[name].push_back()
    //         buyer_new_order.amount = seller_new_order.amount;
            
    //         }
    //       else seller_new_order.amount = buyer_new_order.amount;
    //       filled_orders[buyer_new_order.username].push_back(buyer_new_order);
    //       filled_orders[name].push_back(seller_new_order);
     
    //       user_portfolio[name]["USD"] += buyer_new_order.amount * buyer_new_order.price;
  //         user_portfolio[order.username][buyer_new_order.asset] += (buyer_new_order.amount );
  //         // open_orders2 
  //         // open_orders2[name].push_back();
  //       }
  //       else{
  //         open_orders2[name].push_back(seller_new_order);
  //       }
  //     }
      
  //   }
  // }
  // else if (order.side == "Sell" ) {
  //   seller_new_order = order;
  //   for (const auto & [name, orders] : open_orders) {
  //     open_orders2.clear(); 
  //     for (const auto &orderss : orders) {
  //       buyer_new_order = orderss;
  //       if (orderss.side == "Buy" && order.asset == orderss.asset && orderss.price >= order.price){
  //         buyer_new_order.price = seller_new_order.price;
  //         if(order.amount > orderss.amount) {seller_new_order.amount  = buyer_new_order.amount;}
  //         else buyer_new_order.amount  = seller_new_order.amount;

  //         filled_orders[seller_new_order.username].push_back(seller_new_order);
  //         filled_orders[name].push_back(buyer_new_order);
  //         // open_orders.erase(order.username);
  //         user_portfolio[name][seller_new_order.asset] -= seller_new_order.amount;
  //         user_portfolio[seller_new_order.username]["USD"] += seller_new_order.amount * seller_new_order.price;
  //         user_portfolio[name][buyer_new_order.asset] += buyer_new_order.amount;
  //       }
  //        else{
  //         open_orders2[name].push_back(buyer_new_order);
  //       }

  //     }
      
  //   }
  // }
  

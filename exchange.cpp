#include "exchange.hpp"
#include "utility.hpp"
#include <string>
#include <iostream>
#include <algorithm>
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
    }
     else {
      return false;
    }
  } else {
    return false;
  }
  return true;
}
// CHECK(e.AddOrder({"Nahum", "Sell", "BTC", 5, 100}));
bool Exchange::salesCheck(const Order &sales_order) {
  return user_portfolio[sales_order.username].count(sales_order.asset) > 0 &&
         user_portfolio[sales_order.username][sales_order.asset] >=
             sales_order.amount;
}
// CHECK(e.AddOrder({"Dolson", "Buy", "BTC", 7, 100}));
bool Exchange::buyingCheck(const Order &buy_order) {
  return user_portfolio[buy_order.username].count("USD") > 0 &&
         user_portfolio[buy_order.username]["USD"] >=
             buy_order.amount * buy_order.price;
}
// bool compareBuyOrdersByPrice(Order& a,Order& b) {
//     return a.price < b.price;
// }
// bool compareSellOrdersByPrice( Order& a, Order& b) {
//     return a.price > b.price;
// }

std::vector<Order> Exchange::bestTrade(Order const  &order, std::map<std::string, std::vector<Order>>open_orders){
  std::vector<Order> best_order;
  int price;
  price = order.price;
    for (const auto & [name, orders] : open_orders) { 
      for (const auto &each_order : orders){
        if(order.side == "Buy" && each_order.side == "Sell"
           && order.asset == each_order.asset && price >= each_order.price ){
              best_order.push_back(each_order);
                  std::sort(best_order.begin(), best_order.end(), [](const Order& a, const Order& b) {
        return a.price < b.price;
    });
              // std:: cout << order.username << std::endl;
              // std::cout << each_order.username << std::endl << std::endl;

           }
        else if(order.side == "Sell" && each_order.side == "Buy"
           && order.asset == each_order.asset && price <= each_order.price){
              // std:: cout << order.username << std::endl;
              // std::cout << each_order.username << std::endl << std::endl;
              best_order.push_back(each_order);
                  std::sort(best_order.begin(), best_order.end(), [](const Order& a, const Order& b) {
        return a.price > b.price;
    });

           }
      }
    }
    // std::cout  << "Name: "<< best_order.username << " " << best_order.price << std::endl;
    return best_order;
}

void Exchange::eraseOrder( Order ord){
  std::map<std::string, std::vector<Order>> new_orders;
  for (auto & [name, orders] : open_orders) {
      for (auto &orderss : orders) {
        if (orderss == ord){
          // new_orders[name].push_back(orderss);
          continue;
        }
        else{
          new_orders[name].push_back(orderss);
        }
      }
  }
  open_orders = new_orders;
}
void Exchange::buyOrder(Order &order, Order &best_order){
  int partial;
  int price;
  Trade usage;
  // eraseOrder( best_order);
  // open_orders.erase(best_order.username);
  if(order.amount >= best_order.amount && order.side != best_order.side){
    eraseOrder( best_order);
    partial= order.amount - best_order.amount;
  
    best_order.price = order.price;
  
    filled_orders[best_order.username].push_back(best_order);
    order.amount = best_order.amount;
    filled_orders[order.username].push_back(order);
    usage.buyer_username = order.username;
    usage.seller_username = best_order.username;
    usage.price = order.price;
    usage.amount = best_order.amount;
    usage.asset = order.asset;
    trades.push_back(usage);
    // trades[best_order.username].push_back(order);
    user_portfolio[order.username][order.asset] += best_order.amount;
    //user_portfolio[best_order.username]["USD"] += best_order.amount * best_order.price;
    user_portfolio[best_order.username]["USD"] += best_order.amount * order.price;
    order.amount = partial;
   
    // std::cout << "I came in here" << std::endl;
  }
  else if(order.side != best_order.side){
    // open_orders.erase(best_order.username);
    // eraseOrder( best_order);
    partial = best_order.amount - order.amount;
   for(auto &[name, order]: open_orders){
      for(auto & change : order){
        if(change == best_order){
          change.amount = partial;
        }
      }
    }
  
    price = order.price;
    //find pos
    best_order.price = order.price;
    filled_orders[order.username].push_back(order);
    usage.buyer_username = order.username;
    usage.seller_username = best_order.username;
    usage.price = order.price;
    usage.amount = order.amount;
    usage.asset = order.asset;
    trades.push_back(usage);
    // trades[order.username].push_back(best_order);

    best_order.amount = order.amount;

    filled_orders[best_order.username].push_back(best_order);
    // trades[order.username].push_back(best_order);
    user_portfolio[order.username][order.asset] += order.amount;
    // user_portfolio[best_order.username]["USD"] += order.amount * best_order.price;
    user_portfolio[best_order.username]["USD"] += order.amount * order.price;

    // std::cout << "Should I have come in here?" << std::endl;
    // partial = best_order.amount - order.amount;
    order = best_order;
    order.amount = 0;
    order.price = price;

  
  }
}
void Exchange::sellOrder(Order &order, Order &best_order){
  int partial;
  int price;
  Trade usage;
  
  // open_orders.erase(best_order.username);
  if(order.amount >= best_order.amount && order.side != best_order.side){
    eraseOrder(best_order);
    partial = order.amount - best_order.amount;


    best_order.price = order.price;
    filled_orders[best_order.username].push_back(best_order);
    usage.buyer_username = best_order.username;
    usage.seller_username = order.username;
    usage.price = order.price;
    usage.amount = best_order.amount;
    usage.asset = best_order.asset;
    trades.push_back(usage);
    // trades[order.username].push_back(best_order);
    
    order.amount = best_order.amount;

    filled_orders[order.username].push_back(order);
    // trades[order.username].push_back(order);
    user_portfolio[best_order.username][order.asset] += best_order.amount;
    // user_portfolio[order.username]["USD"] += best_order.amount * best_order.price;
    user_portfolio[order.username]["USD"] += best_order.amount * order.price;
    // std::cout << "Should I have come in here? This is a sale order?" << std::endl;
    order.amount = partial;

    
  }
  else if(order.side != best_order.side){
    // eraseOrder(best_order);
    // open_orders.erase(best_order.username);
    partial = best_order.amount - order.amount;
    for(auto &[name, order]: open_orders){
      for(auto & change : order){
        if(change == best_order){
          change.amount = partial;
        }
      }
    }
    
    filled_orders[order.username].push_back(order);
    // trades[order.username].push_back(order);
    price = best_order.price;
    best_order.amount = order.amount;
    best_order.price = order.price;
    usage.buyer_username = best_order.username;
    usage.seller_username = order.username;
    usage.price = order.price;
    usage.amount = order.amount;
    usage.asset = order.asset;
    trades.push_back(usage);
    // trades[order.username].push_back(best_order);
    filled_orders[best_order.username].push_back(best_order);
    // trades[best_order.username].push_back(best_order);
    // std::cout << "Hello:" << user_portfolio[order.username][order.asset] << std::endl;
    user_portfolio[best_order.username][order.asset] += order.amount;
    // user_portfolio[order.username]["USD"] += order.amount * best_order.price;
    user_portfolio[order.username]["USD"] += order.amount * order.price;
    partial = best_order.amount - order.amount;
    order = best_order;
    order.amount = 0;
    //user_portfolio[order.username][order.asset] += order.amount;
    order.price = price;
    // std::cout << "I came in here. Should I?" << std::endl;
  }
}
// CHECK(e.AddOrder({"Dolson", "Buy", "BTC", 7, 100}));
bool Exchange::AddOrder(const Order &new_order){
  if(new_order.side == "Buy"){
    if(buyingCheck(new_order)){
      Order order = new_order; //CHECK(e.AddOrder({"Dolson", "Buy", "BTC", 7, 100}));
      user_portfolio[new_order.username]["USD"] -= order.amount * order.price;
     
      std::vector<Order> best_orders = bestTrade(order, open_orders);
      // std::cout << best_order << "Buy" << std::endl;
      
      for (auto &best : best_orders) {
        //std::cout << "Best Order: " << best  << std::endl;
        //std::cout << "Order: " << order  << std::endl << std::endl;
        buyOrder(order,best);
       }
      
      if(order.amount > 0 ){
        std::cout << "Order: " << order  << std::endl << std::endl;
        open_orders[order.username].push_back(order);
      }
      // else open_orders[new_order.username].push_back(new_order);

    }
    else return false;
  }
  // CHECK(e.AddOrder({"Nahum", "Sell", "BTC", 5, 100}));
  else if(new_order.side == "Sell"){
    if(salesCheck(new_order)){
      Order order = new_order; //CHECK(e.AddOrder({"Nahum", "Sell", "BTC", 5, 100}));
      user_portfolio[new_order.username][order.asset] -= order.amount;
      // Order best_order = bestTrade(order, open_orders);
      // // std::cout << best_order << "Sale " <<  std::endl;
      // if(best_order.username != "")
      // sellOrder(order, best_order);
      // if(order.amount !=0){
      //   open_orders[order.username].push_back(order);
      //   // std::cout << order << std::endl;
      //   // open_orders[order.username].erase(order);
      // }
       std::vector<Order> best_orders = bestTrade(order, open_orders);
      //  std::cout << best_order << "Buy" << std::endl;
      
      for (auto &best : best_orders) {
        //std::cout << "Best Order: " << best  << std::endl;
        //std::cout << "Order: " << order  << std::endl << std::endl;
        sellOrder(order,best);

       }
      
      if(order.amount !=0){
        std::cout << "Order: " << order  << std::endl << std::endl;
        open_orders[order.username].push_back(order);
      }
      // else open_orders[new_order.username].push_back(new_order);

    }
    else return false;

  }
  return true;
}

void Exchange::PrintUsersOrders(ostream &out){
  out << "Users Orders (in alphabetical order):\n";
  for (const auto & [ names, assets ] : user_portfolio){
    out << names << "'s Open Orders (in chronological order):\n";
    for (const auto & order : open_orders[names]){
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

void Exchange::PrintTradeHistory(ostream &o){
  o << "Trade History (in chronological order):\n";
  for(auto print : trades){
   o << print;
   
  }

}
// std::vector<Order> Exchange::makeVector(){
//   std::vector<Order> new_bid;
//   for (const auto & orders : open_orders){
//     new_bid.insert(new_bid.end(), orders.second.begin(), orders.second.end());
//   }
  
//   return new_bid;
// }
// void Exchange::PrintBidAskSpread(ostream &out) {
//   vector<Order> bid;
//   bid = makeVector();
//   // vector<string> assets;
//   out << "Asset Bid Ask Spread (in alphabetical order):\n";
//   // std::map<std::string, std::vector<Order>> bid;
  


//   std::string asset_name = "";
//   std::sort(bid.begin(), bid.end(), [](Order &order1, Order &order2){ return order1.asset < order2.asset;});
//   int maximum = 0;
//   int minimum = 1000000000;
//   for ( auto order: bid){
//     if(order.asset != asset_name){
//       if(!asset_name.empty()){
//       out << asset_name << ": ";
//       out << "Highest Open Buy = " << (maximum? std::to_string(maximum): "NA") << " USD and Lowest Open Sell = " <<
//       (minimum != 1000000000? std::to_string(minimum ): "NA") << " USD\n";
//       }
    
//     asset_name = order.asset;
//     maximum = 0;
//     minimum = 1000000000;
//     }
  
//   if(order.side == "Buy"){
//     maximum = std::max(maximum, order.price);
//   }
//   else{
//     minimum = std::min(minimum, order.price);
//   }
//   }
//   if(!asset_name .empty()){
//     out << asset_name << ": ";
//       out << "Highest Open Buy = " << (maximum? std::to_string(maximum): "NA") << " USD and Lowest Open Sell = " <<
//       (minimum != 1000000000? std::to_string(minimum ): "NA") << " USD\n";
//   }
  
 
// }
void Exchange::PrintBidAskSpread(ostream &out) {
    out << "Asset Bid Ask Spread (in alphabetical order):\n";

    // Group the orders by asset
    std::map<std::string, std::vector<Order>> orders_by_asset;
    for (const auto& [name, orders] : open_orders) {
        for (const auto& order : orders) {
            orders_by_asset[order.asset].push_back(order);
        }
    }

    // Sort the orders by asset name
    std::vector<std::pair<std::string, std::vector<Order>>> orders_sorted;
    for (const auto& [asset, orders] : orders_by_asset) {
        orders_sorted.push_back({asset, orders});
    }
    std::sort(orders_sorted.begin(), orders_sorted.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });

    // Print the bid-ask spread for each asset
    for (const auto& [asset, orders] : orders_sorted) {
        int max_buy = 0;
        int min_sell = std::numeric_limits<int>::max();
        for (const auto& order : orders) {
            if (order.side == "Buy") {
                max_buy = std::max(max_buy, order.price);
            } else if (order.side == "Sell") {
                min_sell = std::min(min_sell, order.price);
            }
        }
        out << asset << ": ";
        if (max_buy > 0) {
            out << "Highest Open Buy = " << max_buy << " USD ";
        } else {
            out << "Highest Open Buy = NA USD ";
        }
        if (min_sell < std::numeric_limits<int>::max()) {
            out << "and Lowest Open Sell = " << min_sell << " USD\n";
        } else {
            out << "and Lowest Open Sell = NA USD\n";
        }
    }
}


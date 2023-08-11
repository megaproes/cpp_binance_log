#include "order_update.h"
OrderUpdate::OrderUpdate() : eventTime(0), transactionTime(0), originalQuantity(0), originalPrice(0),
					    averagePrice(0), stopPrice(0), orderId(0), orderLastFilledQuantity(0),
					    orderFilledAccumulatedQuantity(0), lastFilledPrice(0), commission(0),
					    orderTradeTime(0), tradeId(0), isTradeMakerSide(false), isReduceOnly(false),
					    isCloseAll(false), activationPrice(0), callbackRate(0), ignorePP(false),
					    ignoreSI(0), ignoreSS(0), realizedProfit(0) {}
void OrderUpdate::parse(const std::string &jsonStr)
{
	json j = json::parse(jsonStr);
	// std::cout << "1\n";
	eventType = j["e"];
	// std::cout << "2\n";
	eventTime = j["E"];
	// std::cout << "3\n";
	transactionTime = j["T"];

	json o = j["o"];
	// std::cout << "4\n";
	symbol = o["s"];
	// std::cout << "5\n";
	clientOrderId = o["c"];
	// std::cout << "6\n";
	side = o["S"];
	// std::cout << "7\n";
	orderType = o["o"];
	// std::cout << "8\n";
	timeInForce = o["f"];
	// std::cout << "9\n";
	originalQuantity = std::stod(o["q"].get<std::string>());
	//::cout << "10\n";
	originalPrice = std::stod(o["p"].get<std::string>());
	// std::cout << "11\n";
	averagePrice = std::stod(o["ap"].get<std::string>());
	// std::cout << "12\n";
	stopPrice = std::stod(o["sp"].get<std::string>());
	//::cout << "13\n";
	executionType = o["x"];
	// std::cout << "14\n";
	orderStatus = o["X"];
	// std::cout << "15\n";
	orderId = o["i"];
	// std::cout << "16\n";
	orderLastFilledQuantity = std::stod(o["l"].get<std::string>());
	// std::cout << "17\n";
	orderFilledAccumulatedQuantity = std::stod(o["z"].get<std::string>());
	//::cout << "18\n";
	lastFilledPrice = std::stod(o["L"].get<std::string>());
	// std::cout << "19\n";
	commissionAsset = o["N"];
	// std::cout << "20\n";
	commission = std::stod(o["n"].get<std::string>());
	// std::cout << "21\n";
	orderTradeTime = o["T"];
	// std::cout << "22\n";
	tradeId = o["t"];
	// std::cout << "23\n";
	bidsNotional = o["b"].get<std::string>();
	// std::cout << "24\n";
	askNotional = o["a"].get<std::string>();
	// std::cout << "25\n";
	isTradeMakerSide = o["m"];
	// std::cout << "26\n";
	isReduceOnly = o["R"];
	// std::cout << "27\n";
	stopPriceWorkingType = o["wt"];
	// std::cout << "28\n";
	originalOrderType = o["ot"];
	// std::cout << "29\n";
	positionSide = o["ps"];
	// std::cout << "30\n";
	isCloseAll = o["cp"];
	// std::cout << "31\n";
	//  activationPrice = o["AP"].get<double>();
	// std::cout << "32\n";
	//  callbackRate = std::stod(o["cr"].get<std::string>());
	// std::cout << "33\n";
	ignorePP = o["pP"];
	// std::cout << "34\n";
	ignoreSI = o["si"];
	// std::cout << "35\n";
	ignoreSS = o["ss"];
	// std::cout << "36\n";
	realizedProfit = std::stod(o["rp"].get<std::string>());
}

void OrderUpdate::print()
{
	std::cout << "Event Type: " << eventType << "\n"
			<< "Event Time: " << eventTime << "\n"
			<< "Transaction Time: " << transactionTime << "\n"
			<< "Symbol: " << symbol << "\n"
			<< "Client Order Id: " << clientOrderId << "\n"
			<< "Side: " << side << "\n"
			<< "Order Type: " << orderType << "\n"
			<< "Time in Force: " << timeInForce << "\n"
			<< "Original Quantity: " << originalQuantity << "\n"
			<< "Original Price: " << originalPrice << "\n"
			<< "Average Price: " << averagePrice << "\n"
			<< "Stop Price: " << stopPrice << "\n"
			<< "Execution Type: " << executionType << "\n"
			<< "Order Status: " << orderStatus << "\n"
			<< "Order Id: " << orderId << "\n"
			<< "Order Last Filled Quantity: " << orderLastFilledQuantity << "\n"
			<< "Order Filled Accumulated Quantity: " << orderFilledAccumulatedQuantity << "\n"
			<< "Last Filled Price: " << lastFilledPrice << "\n"
			<< "Commission Asset: " << commissionAsset << "\n"
			<< "Commission: " << commission << "\n"
			<< "Order Trade Time: " << orderTradeTime << "\n"
			<< "Trade Id: " << tradeId << "\n"
			<< "Bids Notional: " << bidsNotional << "\n"
			<< "Ask Notional: " << askNotional << "\n"
			<< "Is Trade Maker Side: " << (isTradeMakerSide ? "True" : "False") << "\n"
			<< "Is Reduce Only: " << (isReduceOnly ? "True" : "False") << "\n"
			<< "Stop Price Working Type: " << stopPriceWorkingType << "\n"
			<< "Original Order Type: " << originalOrderType << "\n"
			<< "Position Side: " << positionSide << "\n"
			<< "Is Close All: " << (isCloseAll ? "True" : "False") << "\n"
			<< "Activation Price: " << activationPrice << "\n"
			<< "Callback Rate: " << callbackRate << "\n"
			<< "Ignore PP: " << (ignorePP ? "True" : "False") << "\n"
			<< "Ignore SI: " << ignoreSI << "\n"
			<< "Ignore SS: " << ignoreSS << "\n"
			<< "Realized Profit: " << realizedProfit << "\n\n";
}
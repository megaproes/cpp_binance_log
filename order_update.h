#pragma once
#include <iostream>
#include <string>
#include "libs/nlohmann/json.hpp"

using json = nlohmann::json;

class OrderUpdate
{
public:
	std::string eventType;
	long eventTime;
	long transactionTime;
	std::string symbol;
	std::string clientOrderId;
	std::string side;
	std::string orderType;
	std::string timeInForce;
	double originalQuantity;
	double originalPrice;
	double averagePrice;
	double stopPrice;
	std::string executionType;
	std::string orderStatus;
	long orderId;
	double orderLastFilledQuantity;
	double orderFilledAccumulatedQuantity;
	double lastFilledPrice;
	std::string commissionAsset;
	double commission;
	long orderTradeTime;
	long tradeId;
	std::string bidsNotional;
	std::string askNotional;
	bool isTradeMakerSide;
	bool isReduceOnly;
	std::string stopPriceWorkingType;
	std::string originalOrderType;
	std::string positionSide;
	bool isCloseAll;
	double activationPrice;
	double callbackRate;
	bool ignorePP;
	int ignoreSI;
	int ignoreSS;
	double realizedProfit;
	OrderUpdate();
	void parse(const std::string &jsonStr);
	void print();
};

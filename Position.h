#pragma once
#include <string>
#include "libs/nlohmann/json.hpp"
#include <vector>
#include "order_update.h"

class Position
{
public:
	Position();
	Position(const OrderUpdate &trade);

	std::string symbol;
	std::string side;

	double average_entry_price;
	int count_entry_prices;

	double average_exit_price;
	int count_exit_prices;

	double commission;
	double realizedPnl_NET;
	double realizedPnl_Gross;

	double volume_dollar;
	double volume_quantity;
	double exit_volume_quantity;

	long long time_start;
	long long time_finished;

	std::vector<OrderUpdate> all_trades_inside_position;

	static std::vector<Position> parseTrades(std::vector<OrderUpdate> &trades);

	bool isClosed() const;
	void update(const OrderUpdate &trade);
	bool is_closed;
};

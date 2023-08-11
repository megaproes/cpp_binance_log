#include "Position.h"
#include <cmath> 
std::vector<Position> Position::parseTrades(std::vector<OrderUpdate> &trades)
{
	std::vector<Position> positions;
	for (size_t i = 0, j = 0; i < trades.size(); i++)
	{
		Position pos{};
		pos.symbol = trades[i].symbol;
		pos.side = trades[i].side;

		pos.count_entry_prices++;
		pos.time_start = trades[i].transactionTime;
		pos.commission += trades[i].commission;

		pos.average_entry_price += trades[i].averagePrice * trades[i].originalQuantity;
		pos.volume_quantity += trades[i].originalQuantity;
		pos.volume_dollar += trades[i].averagePrice * trades[i].originalQuantity;
		for (j = i + 1; j <= trades.size(); j++)
		{
			if (pos.side == trades[j].side)
			{
				pos.average_entry_price += trades[j].averagePrice * trades[j].originalQuantity;
				pos.volume_quantity += trades[j].originalQuantity;
				pos.volume_dollar += trades[j].originalQuantity;
				pos.commission += trades[j].commission;

				pos.count_entry_prices++;
			}
			else if (pos.side != trades[j].side)
			{
				pos.average_exit_price += trades[j].averagePrice * trades[j].originalQuantity;
				pos.exit_volume_quantity += trades[j].originalQuantity;

				pos.realizedPnl_NET += trades[j].realizedProfit;
				pos.commission += trades[j].commission;

				pos.count_exit_prices++;

				if (abs(pos.volume_quantity - pos.exit_volume_quantity) < 0.000001)
				{
					pos.average_entry_price /= pos.volume_quantity;
					pos.average_exit_price /= pos.exit_volume_quantity;
					pos.realizedPnl_Gross = pos.realizedPnl_NET - pos.commission;
					pos.time_finished = trades[j].transactionTime;

					i = j;

					break; // breakes nested 'for'
				}
			}
		}

		positions.push_back(pos);
	}

	return positions;
}

void Position::update(const OrderUpdate &trade)
{
	if (trade.side == side)
	{
		average_entry_price += trade.averagePrice * trade.originalQuantity;
		volume_quantity += trade.originalQuantity;

		volume_dollar += trade.originalQuantity;
		commission += trade.commission;

		count_entry_prices++;
	}
	else if (trade.side != side)
	{
		average_exit_price += trade.averagePrice * trade.originalQuantity;
		exit_volume_quantity += trade.originalQuantity;

		realizedPnl_NET += trade.realizedProfit;
		commission += trade.commission;

		count_exit_prices++;

		if (std::abs(volume_quantity - exit_volume_quantity) < 0.000001) // = (volume_quantity - exit_volume_quantity) == 0  
		{
			average_entry_price /= volume_quantity;
			average_exit_price /= exit_volume_quantity;
			realizedPnl_Gross = realizedPnl_NET - commission;
			time_finished = trade.transactionTime;
			is_closed = true;
		}
	}
}

bool Position::isClosed() const
{
	return is_closed;
}

Position::Position(const OrderUpdate &trade) : Position() // call the default constructor
{
	symbol = trade.symbol;
	side = trade.side;
	time_start = trade.transactionTime;
	is_closed = false;
	update(trade);
}

Position::Position() : average_entry_price(0.0), count_entry_prices(0),
				   average_exit_price(0.0), count_exit_prices(0),
				   commission(0.0), realizedPnl_NET(0.0), realizedPnl_Gross(0.0),
				   volume_dollar(0.0), volume_quantity(0.0), exit_volume_quantity(0.0),
				   time_start(0), time_finished(0) {}
/* Программа-заготовка для домашнего задания
*/

#include "hw/l2_ApplicationLayer.h"

#include <algorithm>
#include <thread>
using namespace std;


const int OUTPUT_LIMIT = 1000;

void Application::work()
{
    std::vector<std::string> args = split(_command);
    if (args.empty())
        return;

    if (args[0] == "a" || args[0] == "add")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды add");
            return;
        }

        _col.addItem(std::make_shared<Lottery>(stoi(args[1])));
        return;
    }

    //add bet
    if (args[0] == "ab" || args[0] == "add_bet")
    {
        if (args.size() != 6)
        {
            _out.Output("Некорректное количество аргументов команды add_bet");
            return;
        }

        try
            {
               Lottery & p = _col.getLottery(stoi(args[1]));
               p.addBet(Bet(stoi(args[2]),stoi(args[3]),stof(args[4]),stof(args[5])));
            }
            catch (std::exception& ex)
            {
                return;
            }

        return;
    }


    if (args[0] == "r" || args[0] == "remove")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды remove");
            return;
        }

        _col.removeItem(stoul(args[1]));
        return;
    }

    if (args[0] == "u" || args[0] == "update")
    {
        if (args.size() != 3)
        {
            _out.Output("Некорректное количество аргументов команды update");
            return;
        }

        _col.updateItem(stoul(args[1]), make_shared<Lottery>(stoi(args[2])));
        return;
    }

    if (args[0] == "v" || args[0] == "view")
    {
        if (args.size() > 3)
        {
            _out.Output("Некорректное количество аргументов команды view");
            return;
        } 

        size_t lines_limit = OUTPUT_LIMIT;
        if (args.size() > 1)
            lines_limit = stoul(args[1]);

        size_t bets_limit = OUTPUT_LIMIT;
        if (args.size() > 2)
            bets_limit = stoul(args[2]);
        
        size_t count = 0;
        for(size_t i=0; i <= _col.getSize(); ++i)
        {
            if (!_col.isRemoved(i))
            {
                const Lottery& item = _col.getLottery(i);

                if (count < lines_limit) {
                _out.Output("[" + to_string(i) + "] "
                        + to_string(item.getTax()) + " "
                        );
                
                size_t bets_count = 0;
                for(const Bet b : item.getBets()){
                    if (bets_count < bets_limit)
                    _out.Output(to_string(b.getNumberOfBet()) + " " + to_string(b.getClientId()) + " " + to_string(b.getIndex())+ " " + to_string(b.getSumOfBet()));
                    bets_count ++;
                }

                 if (bets_count >= bets_limit)
                        _out.Output("\t... " + std::to_string(bets_count) + " ставок");
                }
                else if (count == lines_limit)
                    _out.Output("Выведено первые " + std::to_string(lines_limit) + " строк");
                
                count ++;
            }

        }

        _out.Output("Количество элементов в коллекции: " + to_string(count));
        return;
    }

    // report
    if (args[0] == "rp" || args[0] == "report")
    {
        if (args.size() > 2)
        {
            _out.Output("Некорректное количество аргументов команды report");
            return;
        }
        
        size_t lines_limit = OUTPUT_LIMIT;
        if (args.size() == 2)
            lines_limit = stoul(args[1]);
        
        vector<pair<int,float>> report_set;

        for(size_t i=0; i < _col.getSize(); ++i)
        {
            if (!_col.isRemoved(i))
            {
                Lottery & p = _col.getLottery(i);
                p.play();
                if (p.getBets().size() > 0)
                for(const Bet & b : p.getBets())
                {
                    if (b.getIsWinner())
                    { 
                        float prize = p.getSumOfPrize()*b.getSumOfBet()/p.getSumOfBets();
                        report_set.push_back({b.getClientId(),prize});
                    }
                }   
            }
        }

        size_t count = 0;
        for(const auto & [client_id,prize] : report_set){
            if (count < lines_limit) 
                _out.Output(to_string(client_id) + " " + to_string(prize));
            else if (count == lines_limit) {
                _out.Output("Выведено первые " + std::to_string(lines_limit) + " строк");
                break;
        }
        
        }
        return;
    }
    _out.Output("Недопустимая команда '" + args[0] + "'");
    return;
}

std::vector<std::string> Application::split(const std::string & str)
{
    std::vector<std::string> res;
    size_t                   start_pos = 0, 
                             end_pos   = 0;

    while(end_pos < str.size()) 
    {
        for(start_pos = end_pos; start_pos < str.size(); ++start_pos)
            if (str[start_pos] != ' ')
                break;

        if (start_pos == str.size())
            return res;

        for(end_pos = start_pos; end_pos < str.size(); ++end_pos)
            if (str[end_pos] == ' ')
                break;

        res.push_back(str.substr(start_pos,end_pos-start_pos));
    }

    return res;
}

/* 
MIT License 

Copyright (c) 2019 МГТУ им. Н.Э. Баумана, кафедра ИУ-6, Михаил Фетисов, 

Программа-заготовка для домашнего задания
*/

#ifndef HW_L3_DOMAIN_LAYER_H
#define HW_L3_DOMAIN_LAYER_H

#include "hw/l4_InfrastructureLayer.h"
#include <vector>

const float MAX_SUM_OF_BET    = 1900;
const float MIN_SUM_OF_BET  = 50;

class Bet 
{
    int _number_of_bet;
    int _client_id;
    float _index;
    float _sum_of_bet;
    bool _is_won;

protected:
    bool invariant() const;

public:
    Bet() = delete;

    Bet(int number_of_bet, int client_id, float index, float sum_of_bet)
    : _number_of_bet(number_of_bet), _client_id(client_id), _index(index), _sum_of_bet(sum_of_bet),_is_won(false) { }
    int getNumberOfBet() const;
    int getClientId() const;
    float getIndex() const;
    float getSumOfBet() const;
    void makeWinner();
    bool getIsWinner() const;  //предаолагается что один человек может делать несколько ставок с разными коэф-тами и выигрыш считается относительно каждой ставки отдельно,человек может выиграть 2 раза
    
};

class Lottery: public ICollectable
{
    int _tax;
    int _sum_of_bets;
    int _sum_of_prize;
    std::vector<Bet> _bets;
    mutable std::mutex _bets_mutex;

protected:
    bool invariant() const;
    
public:
    Lottery() = delete;
    Lottery(const Lottery & p) = delete;

    Lottery & operator = (const Lottery & p) = delete;

    Lottery(int tax);
    Lottery(int tax, std::vector<Bet> bets);

    int getTax() const; 
    int getSumOfBets() const;
    float getSumOfPrize() const;
    void setBets(const std::vector<Bet> bets);
    void addBet(const Bet & bet);
    std::vector<Bet> getBets() const;
    
    virtual bool   write(std::ostream& os) override;
    void play();
};

class ItemCollector: public ACollector
{
public:
    virtual std::shared_ptr<ICollectable> read(std::istream& is) override;

    Lottery & getLottery(size_t index);
};

#endif // HW_L3_DOMAIN_LAYER_H

/* Программа-заготовка для домашнего задания
*/
#include <iostream>

#include "hw/l3_DomainLayer.h"
using namespace std;

bool Lottery::invariant() const
{
    return  _tax>0
        && _tax<100;
}


Lottery::Lottery(int tax)
    : _tax(tax)
{
    assert(invariant());
}

Lottery::Lottery(int tax, std::vector<Bet> bets)
    : _tax(tax)
    , _sum_of_bets(0)
    , _sum_of_prize(0)
    , _bets(bets)
{
    assert(invariant());
}

int Lottery::getTax() const
{
    return _tax;
}

void Lottery::setBets(const std::vector<Bet> bets)
{
    std::lock_guard locker(_bets_mutex);
    _bets = bets;
}

void Lottery::addBet(const Bet & bet)
{
    std::lock_guard locker(_bets_mutex);
    _bets.push_back(bet);
}

std::vector<Bet> Lottery::getBets() const
{
    std::lock_guard locker(_bets_mutex);
    return _bets;
}

bool Bet::invariant() const
{
    return _sum_of_bet >= MIN_SUM_OF_BET
        && _sum_of_bet <= MAX_SUM_OF_BET
        && _index >= 1
        && _client_id >=1;
}


int Bet::getNumberOfBet() const
{
    return _number_of_bet;
}

int Bet::getClientId() const
{
    return _client_id;
}

float Bet::getSumOfBet() const
{
    return _sum_of_bet;
}

float Bet::getIndex() const
{
    return _index;
}

void Bet::makeWinner()
{
    _is_won=true;
}

bool Bet::getIsWinner() const
{
    return _is_won;
}

bool   Lottery::write(ostream& os)
{
    writeNumber(os, _tax);

    size_t bets_count = _bets.size();
    writeNumber(os, bets_count);

    for(const Bet & b : _bets)
    {
        writeNumber(os, b.getNumberOfBet());
        writeNumber(os, b.getClientId());
        writeNumber(os, b.getIndex());
        writeNumber(os, b.getSumOfBet());
    }
    return os.good();
}

void Lottery::play()
{
    size_t count=0;
    for(Bet & b : _bets) 
    {
        _sum_of_bets+=b.getSumOfBet();
        /* выбор победителя с помощью псевдорандомных чисел, для тестирования победителем будем брать последнего поставившего, чтобы поведение програмы было предсказуемым
        if ((rand()%2 == 0) && (rand()%2 == 0))
        { 
            b.makeWinner();
            count_wins+=1;
        }
       
    if (count_wins==0){
            int ind=rand()%_bets.size();
            _bets[ind].makeWinner();
        }*/ 
        count+=1;
        if (count==_bets.size()){
            b.makeWinner();
        }
    } 

    _sum_of_prize=(1-_tax/100)*_sum_of_bets;
}

int Lottery::getSumOfBets() const
 {
    return _sum_of_bets;
 }

float Lottery::getSumOfPrize() const
{
    return _sum_of_prize;
}

shared_ptr<ICollectable> ItemCollector::read(istream& is)
{
    int tax = readNumber<int>(is);
    size_t bets_count = readNumber<size_t>(is);

    vector<Bet> b;

    b.reserve(bets_count);
    for(size_t i=0; i < bets_count; ++i)
    {
        int  number_of_bet = readNumber<int>(is);
        int  client_id     = readNumber<int>(is);
        float index        = readNumber<float>(is);
        float sum_of_bet   = readNumber<float>(is);

        b.push_back(Bet(number_of_bet, client_id, index,sum_of_bet));
    }

    std::shared_ptr<ICollectable> p = std::make_shared<Lottery>(tax, b);

    return p;
}

Lottery & ItemCollector::getLottery(size_t index)
{
    Lottery * p = static_cast<Lottery *>(getItem(index).get());
    
    if (p==nullptr){
        throw std::runtime_error("invalid cast to lottery");
    }
    return *p;
}

#include "purple_coins_comet_stage.hpp"

PurpleCoinsCometStage::PurpleCoinsCometStage(int total_purple_coins, int purple_coins_requirement)
    : CometStage(COMET_STAGE_PURPLE_COINS)
    , _current_purple_coins(0)
    , _total_purple_coins(total_purple_coins)
    , _purple_coins_requirement(purple_coins_requirement)
{}

PurpleCoinsCometStage::PurpleCoinsCometStage(int total_purple_coins)
    : PurpleCoinsCometStage(total_purple_coins, total_purple_coins) // same total of coins as requirement
{}


int PurpleCoinsCometStage::get_total_purple_coins() const
{
    return _total_purple_coins;
}

int PurpleCoinsCometStage::get_purple_coins_requirement() const
{
    return _purple_coins_requirement;
}

int PurpleCoinsCometStage::get_current_purple_coins() const
{
    return _current_purple_coins;
}
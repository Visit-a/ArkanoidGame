#include "ATMState.h"
#include <iostream>

namespace Patterns
{
    ATM::ATM() : balance(1000.0)
    {
        state.reset(new NoCardState());
    }

    void ATM::SetState(std::unique_ptr<IAtmState> newState)
    {
        state = std::move(newState);
    }

    void ATM::InsertCard() { state->InsertCard(*this); }
    void ATM::EnterPin(const std::string& pin) { state->EnterPin(*this, pin); }
    void ATM::Withdraw(double amount) { state->Withdraw(*this, amount); }
    void ATM::EjectCard() { state->EjectCard(*this); }
    const char* ATM::GetStateName() const { return state->Name(); }

    void NoCardState::InsertCard(ATM& atm)
    {
        std::cout << "Карта вставлена.\n";
        atm.SetState(std::unique_ptr<IAtmState>(new CardInsertedState()));
    }
    void NoCardState::EnterPin(ATM&, const std::string&)
    {
        std::cout << "Операция отклонена: сначала вставьте карту.\n";
    }
    void NoCardState::Withdraw(ATM&, double)
    {
        std::cout << "Операция отклонена: карта не вставлена.\n";
    }
    void NoCardState::EjectCard(ATM&)
    {
        std::cout << "Карта уже извлечена.\n";
    }

    void CardInsertedState::InsertCard(ATM&)
    {
        std::cout << "Карта уже вставлена.\n";
    }
    void CardInsertedState::EnterPin(ATM& atm, const std::string& pin)
    {
        if (pin == "1234")
        {
            std::cout << "PIN принят.\n";
            atm.SetState(std::unique_ptr<IAtmState>(new PinVerifiedState()));
        }
        else
        {
            std::cout << "Неверный PIN. Попробуйте ещё раз.\n";
        }
    }
    void CardInsertedState::Withdraw(ATM&, double)
    {
        std::cout << "Сначала подтвердите PIN.\n";
    }
    void CardInsertedState::EjectCard(ATM& atm)
    {
        std::cout << "Карта извлечена.\n";
        atm.SetState(std::unique_ptr<IAtmState>(new NoCardState()));
    }

    void PinVerifiedState::InsertCard(ATM&)
    {
        std::cout << "Карта уже вставлена.\n";
    }
    void PinVerifiedState::EnterPin(ATM&, const std::string&)
    {
        std::cout << "PIN уже подтверждён.\n";
    }
    void PinVerifiedState::Withdraw(ATM& atm, double amount)
    {
        if (amount <= 0.0)
            std::cout << "Сумма должна быть положительной.\n";
        else if (amount > atm.balance)
            std::cout << "Недостаточно средств.\n";
        else
        {
            atm.balance -= amount;
            std::cout << "Выдано: " << amount
                      << ". Остаток: " << atm.balance << "\n";
        }
    }
    void PinVerifiedState::EjectCard(ATM& atm)
    {
        std::cout << "Карта извлечена. Сессия завершена.\n";
        atm.SetState(std::unique_ptr<IAtmState>(new NoCardState()));
    }
}

#pragma once
#include <memory>
#include <string>
#include <iostream>

namespace Patterns
{
    class ATM;

    class IAtmState
    {
    public:
        virtual ~IAtmState() {}
        virtual void InsertCard(ATM& atm) = 0;
        virtual void EnterPin(ATM& atm, const std::string& pin) = 0;
        virtual void Withdraw(ATM& atm, double amount) = 0;
        virtual void EjectCard(ATM& atm) = 0;
        virtual const char* Name() const = 0;
    };

    class ATM
    {
    public:
        ATM();
        void SetState(std::unique_ptr<IAtmState> state);
        void InsertCard();
        void EnterPin(const std::string& pin);
        void Withdraw(double amount);
        void EjectCard();
        const char* GetStateName() const;

    private:
        std::unique_ptr<IAtmState> state;
        double balance;
        friend class PinVerifiedState;
    };

    class NoCardState : public IAtmState
    {
    public:
        void InsertCard(ATM& atm) override;
        void EnterPin(ATM&, const std::string&) override;
        void Withdraw(ATM&, double) override;
        void EjectCard(ATM&) override;
        const char* Name() const override { return "Карта не вставлена"; }
    };

    class CardInsertedState : public IAtmState
    {
    public:
        void InsertCard(ATM&) override;
        void EnterPin(ATM& atm, const std::string& pin) override;
        void Withdraw(ATM&, double) override;
        void EjectCard(ATM& atm) override;
        const char* Name() const override { return "Карта вставлена"; }
    };

    class PinVerifiedState : public IAtmState
    {
    public:
        void InsertCard(ATM&) override;
        void EnterPin(ATM&, const std::string&) override;
        void Withdraw(ATM& atm, double amount) override;
        void EjectCard(ATM& atm) override;
        const char* Name() const override { return "PIN подтверждён"; }
    };
}

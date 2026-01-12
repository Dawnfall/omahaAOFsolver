#pragma once
#include <functional>
#include <vector>

template<typename... Args>
class Event
{
public:
    using Listener = std::function<void(Args...)>;

    // Add a listener to the event
    void AddListener(const Listener& listener)
    {
        listeners.push_back(listener);
    }

    // Remove a listener from the event
    void RemoveListener(const Listener& listener)
    {
        listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
    }

    // Trigger the event and notify all listeners
    void Trigger(Args... args)
    {
        for (const auto& listener : listeners)
        {
            listener(args...);
        }
    }

private:
    std::vector<Listener> listeners;
};
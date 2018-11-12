#include "BBQ.h"
#include <functional>

/**
 * Construct a new thread-safe blocking bounded queue.
 */
BBQ::BBQ(): m_head(0), m_tail(0) {}

void BBQ::insert(int item)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_itemRemoved.wait(lock, std::bind(&BBQ::canInsert, this));
    m_items[m_tail % MAX] = item;
    m_tail++;
    m_itemAdded.notify_one();
}

int BBQ::remove()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_itemAdded.wait(lock, std::bind(&BBQ::canRemove, this));
    int item = m_items[m_head % MAX];
    m_head++;
    m_itemRemoved.notify_one();
    return item;
}
# Integration Report
- FoodMenu manages the custom linked list.
- orderQueue stores tokens in FIFO order.
- TokenTable maps tokens to order records using a custom hash table.
- cancellationStack stores accepted cancellation records.
- Cancelled orders are skipped when they reach the front of the queue.
- No priority queue or file handling is used.

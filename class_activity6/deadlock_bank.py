import threading
import time

class Account:
    def __init__(self, name, balance):
        self.name = name
        self.balance = balance
        self.lock = threading.Lock()

def transfer(from_acc, to_acc, amount):
    print(f"{threading.current_thread().name} locking {from_acc.name}")
    from_acc.lock.acquire()
    print(f"{threading.current_thread().name} LOCKED {from_acc.name}")

    time.sleep(1)

    print(f"{threading.current_thread().name} waiting for {to_acc.name}")
    to_acc.lock.acquire()

    # This part will never be reached due to deadlock
    from_acc.balance -= amount
    to_acc.balance += amount

    to_acc.lock.release()
    from_acc.lock.release()

def main():
    acc1 = Account("Account1", 1000)
    acc2 = Account("Account2", 1000)

    t1 = threading.Thread(target=transfer, args=(acc1, acc2, 100), name="Thread1")
    t2 = threading.Thread(target=transfer, args=(acc2, acc1, 50), name="Thread2")

    t1.start()
    t2.start()

    t1.join()
    t2.join()

if __name__ == "__main__":
    main()
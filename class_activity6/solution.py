import threading
import time

class Account:
    def __init__(self, name, balance):
        self.name = name
        self.balance = balance
        self.lock = threading.Lock()

def transfer(from_acc, to_acc, amount):
    # Global ordering to prevent deadlock
    first, second = sorted([from_acc, to_acc], key=lambda x: x.name)

    print(f"{threading.current_thread().name} locking {first.name}")
    with first.lock:
        print(f"{threading.current_thread().name} LOCKED {first.name}")
        time.sleep(1)

        print(f"{threading.current_thread().name} locking {second.name}")
        with second.lock:
            print(f"{threading.current_thread().name} LOCKED {second.name}")

            from_acc.balance -= amount
            to_acc.balance += amount

            print(f"{threading.current_thread().name} transferred {amount}")

def main():
    acc1 = Account("Account1", 1000)
    acc2 = Account("Account2", 1000)

    t1 = threading.Thread(target=transfer, args=(acc1, acc2, 100), name="Thread1")
    t2 = threading.Thread(target=transfer, args=(acc2, acc1, 50), name="Thread2")

    t1.start()
    t2.start()

    t1.join()
    t2.join()

    print("\nFinal Balances:")
    print(acc1.name, acc1.balance)
    print(acc2.name, acc2.balance)

if __name__ == "__main__":
    main()
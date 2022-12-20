import os
import threading
import argparse

lock = threading.Lock()
threads = []
funcs = [
        "python demo.py",
        "cd Paint",
        "python paint.py"
    ]


def parse_args():
    parser = argparse.ArgumentParser(description="setup parameters")

    parser.add_argument("--need_install", required=True, default=False,
                        help="declare whether pip install or not")

    return parser.parse_args()


def installer():
    with open("requirements.txt", "r") as origin_file:
        for i in origin_file:
            if "@" not in i:
                os.system(f"pip install {i}")
    

def runner(num):
    try:
        lock.acquire()
        os.system(f"{funcs[num]}")
        lock.release()
    except:
        pass

if __name__ == "__main__":
    args = parse_args()
    # if args.need_install:
    #     installer()
    
    for num in range(len(funcs)):
        t = threading.Thread(target=runner(num))
        threads.append(t)
        t.start() 

    for t in threads:
        t.join()

    

    
import os
import random
import string

def main():
    myPid = os.getpid()
    print(myPid);
    file_1 = "tic"+str(myPid)
    file_2 = "tac"+str(myPid)
    file_3 = "toe"+str(myPid)
    f1 = open(file_1, "w")
    f2 = open(file_2, "w")
    f3 = open(file_3, "w")
    rng1 = randomword(10) + "\n"
    rng2 = randomword(10) + "\n"
    rng3 = randomword(10) + "\n"
    print(rng1+rng2+rng3,end = "")
    f1.write(rng1)
    f2.write(rng2)
    f3.write(rng3)
    f1.close()
    f2.close()
    f3.close()
    rng_num1 = random.randint(1,42)
    rng_num2 = random.randint(1,42)
    print(rng_num1)
    print(rng_num2)
    print(rng_num1 * rng_num2)

# source:http://stackoverflow.com/questions/2030053/random-strings-in-python
def randomword(length):
   return ''.join(random.choice(string.ascii_lowercase) for i in range(length))

main()

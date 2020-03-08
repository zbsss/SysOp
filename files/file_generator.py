import os
import argparse
import string
import random


#run by:
#python3 file_generator.py -num 10 -mw 10


def generate_file(name,args):
    words = random.randint(1,args.words)
    text = ""

    for i in range(words):
        times = random.randint(1,10)
        letter = random.choice(string.ascii_lowercase)
        
        text += letter * times 
        text += " "
        if random.choice([True,False]):
            text += "\n"
    
    with open(str(name) + ".txt",'w') as file:
        file.write(text)

def run(args):
    for i in range(0,args.num):
        generate_file(i, args)
    

def main():
    parser = argparse.ArgumentParser("")
    parser.add_argument("-num",help="number of files",dest="num",type=int,required=False,default=2)
    parser.add_argument("-mw",help="max number of words in file",dest="words"
                        ,type=int,required=False,default=10);
    parser.set_defaults(func=run)
    args = parser.parse_args()
    args.func(args)

if __name__=="__main__":
    main()
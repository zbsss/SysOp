import os
import argparse
import string
import random

#os.system("ls")

#run by:
#python3 file_generator.py -num 10 -size medium

#lines:
#small 1-5
#medium 6-15
#large  16-30

def generate_file(name,args):
    lines = random.randint(1,args.lines)
    text = ""

    for i in range(lines):
        times = random.randint(1,10)
        letter = random.choice(string.ascii_lowercase)

        if random.choice([True,False]):
            text += "\n"
        else:
            text += letter * times 
            text += " "
    
    with open(str(name) + ".txt",'w') as file:
        file.write(text)

def run(args):
    for i in range(0,args.num):
        generate_file(i, args)
    

def main():
    parser = argparse.ArgumentParser("")
    parser.add_argument("-num",help="number of files",dest="num",type=int,required=False,default=2)
    parser.add_argument("-maxl",help="max number of lines",dest="lines"
                        ,type=int,required=False,default=10);
    parser.set_defaults(func=run)
    args = parser.parse_args()
    args.func(args)

if __name__=="__main__":
    main()